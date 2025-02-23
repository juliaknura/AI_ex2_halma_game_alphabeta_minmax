#include "GameStructures.hpp"
#include <algorithm>
#include <queue>

void copyExceptValue(const std::vector<Coord>& from, std::vector<Coord>& to, const Coord value)
{
    to.reserve(from.size()-1);
    for(const auto& elem: from)
    {
        if(elem not_eq value)
            to.emplace_back(elem);
    }
}

void GameState::generateChildren()
{   
    auto& positions = currentPlayer ? firstPositions : secondPositions;
    const auto pawn = currentPlayer ? Pawn::First : Pawn::Second;

    for(const auto& pawnCoords : positions)
    {
        //steps
        for(int i=-1;i<2;i++)
            for(int j=-1;j<2;j++)
            {
                const auto newPos = Coord{pawnCoords.x+i,pawnCoords.y+j};
                if(isWithinBounds(newPos) and not isSquareOccupied(newPos) and not (i == 0 and j == 0))
                    generateChild(newPos,pawnCoords,pawn);
            }
        //jumps
        auto finalCoords = discoverJumpsIter(pawnCoords);
        for(const auto& newPos : finalCoords)
            generateChild(newPos,pawnCoords,pawn);
    }
}

void GameState::generateChild(const Coord& newPos, const Coord& currentPos, const Pawn pawn)
{
    GameState state{
    .currentPlayer=!currentPlayer,
    .board=makeMove(currentPos,newPos,pawn),
    .firstPositions{},
    .secondPositions{},
    .children{},
    .maxingPlayer{maxingPlayer}
    };
    if(currentPlayer)
    {
        copyExceptValue(firstPositions,state.firstPositions,currentPos);
        state.firstPositions.push_back(newPos);
        state.secondPositions = secondPositions;
    }
    else
    {
        copyExceptValue(secondPositions,state.secondPositions,currentPos);
        state.secondPositions.push_back(newPos);
        state.firstPositions = firstPositions;
    }
    children.push_back(state);
}

Coord jumpEnd(const Coord& start, const Coord& adj)
{
    return {(adj.x-start.x)+adj.x , (adj.y-start.y)+adj.y};
}

std::vector<Coord> GameState::discoverJumps(const Coord& start, std::unordered_set<Coord,Hash> prev)
{
    std::vector<Coord> jumpDest;
    prev.insert(start);
    for(int i=-1;i<2;i++)
        for(int j=-1;j<2;j++)
        {
            const auto adjPos = Coord{start.x+i,start.y+j};
            if(not(i==0 and j==0) and isWithinBounds(adjPos) and isSquareOccupied(adjPos))
            {
                const auto newPos = jumpEnd(start,adjPos);
                if(isWithinBounds(newPos) and not isSquareOccupied(newPos) and prev.count(newPos)==0)
                {
                    jumpDest.push_back(newPos);
                    const auto rec = discoverJumps(newPos,prev);
                    std::copy(rec.cbegin(),rec.cend(),std::back_inserter(jumpDest));
                }
            }
        }

    return jumpDest;
}

std::unordered_set<Coord,Hash> GameState::discoverJumpsIter(const Coord& start)
{
    std::unordered_set<Coord,Hash> jumpDestinations;
    std::queue<Coord> startPositions;
    startPositions.push(start);
    jumpDestinations.insert(start);
    while(not startPositions.empty())
    {
        Coord consideredPos = startPositions.front();
        startPositions.pop();
        for(int i=-1;i<2;i++)
            for(int j=-1;j<2;j++)
            {
                const auto adjPos = Coord{consideredPos.x+i,consideredPos.y+j};
                if(not(i==0 and j==0) and isWithinBounds(adjPos) and isSquareOccupied(adjPos))
                {
                    const auto newPos = jumpEnd(consideredPos,adjPos);
                    if(isWithinBounds(newPos) and not isSquareOccupied(newPos) and jumpDestinations.count(newPos)==0)
                    {
                        jumpDestinations.insert(newPos);
                        startPositions.push(newPos);
                    }
                }
            }
    }
    jumpDestinations.erase(start);
    return jumpDestinations;
}

GameBoard GameState::makeMove(const Coord& start, const Coord& end, const Pawn pawn)
{
    GameBoard newBoard = board;
    newBoard.set(Pawn::None,start);
    newBoard.set(pawn,end);
    return newBoard;
}

inline bool GameState::isWithinBounds(const Coord pos)
{
    return pos.x >= 0 and pos.x < board.size() and pos.y >=0 and pos.y < board.size();
}

inline bool GameState::isSquareOccupied(const Coord pos)
{
    return board.at(pos) == Pawn::First or board.at(pos) == Pawn::Second;
}

Pawn GameState::gameWonBy()
{
    bool firstWins = true;
    bool secondWins = true;

    for(const auto& firstBasePos : firstBase)
    {
        if(board.at(firstBasePos) not_eq Pawn::Second)
        {
            secondWins = false;
            break;
        }   
    }

    if(secondWins)
        return Pawn::Second;

    for(const auto& secondBasePos: secondBase)
    {
        if(board.at(secondBasePos) not_eq Pawn::First)
        {
            firstWins = false;
            break;
        }
    }

    if(firstWins)
        return Pawn::First;

    return Pawn::None;
}

double manhattanDistance(Coord a, Coord b)
{   
    return std::abs(a.x-b.x) + std::abs(a.y-b.y);
}

bool isInStartArea(Coord a, Pawn player)
{
    return player == Pawn::First ? std::find(firstBase.cbegin(), firstBase.cend(), a) not_eq firstBase.cend() : std::find(secondBase.cbegin(), secondBase.cend(), a) not_eq secondBase.cend();
}

bool isInFinishArea(Coord a, Pawn player)
{
    return player == Pawn::First ? std::find(secondBase.cbegin(), secondBase.cend(), a) not_eq secondBase.cend() : std::find(firstBase.cbegin(), firstBase.cend(), a) not_eq firstBase.cend();
}

bool GameState::isEndingStage()
{
    auto& base = maxingPlayer == Pawn::First ? secondBase : firstBase;
    for(const auto& coord: base)
    {
        if(board.at(coord) == maxingPlayer)
            return true;
    }
    return false;
}

double GameState::distanceHeuristic()
{
    auto& positions = maxingPlayer == Pawn::First ? firstPositions : secondPositions;
    Coord start = maxingPlayer == Pawn::First ? Coord{0,0} : Coord{15,15};
    double heuristicValue{};
    for(const auto& elem : positions)
    {
        heuristicValue += manhattanDistance(start,elem);
    }
    return heuristicValue;
}

double GameState::baseHeuristic()
{
    auto& positions = maxingPlayer == Pawn::First ? firstPositions : secondPositions;
    double heuristicValue{};
    for(const auto& elem: positions)
    {
        heuristicValue += isInFinishArea(elem, maxingPlayer) ? 30 : isInStartArea(elem,maxingPlayer) ? -30 : 0;
    }
    return heuristicValue;
}

double GameState::jumpHeuristic()
{    
    double heuristicValue{};
    Coord start = maxingPlayer == Pawn::First ? Coord{0,0} : Coord{15,15};
    auto& positions = maxingPlayer == Pawn::First ? firstPositions : secondPositions;
    auto ij = maxingPlayer == Pawn::First ? Coord{1,1} : Coord{-1,-1};
    for(const auto& pos : positions)
    {
        const auto adjPos = Coord{pos.x+ij.x,pos.y+ij.y};
        if(isWithinBounds(adjPos) and isSquareOccupied(adjPos))
        {
            const auto newPos = jumpEnd(pos,adjPos);
            if(isWithinBounds(newPos) and not isSquareOccupied(newPos))
            {
                heuristicValue += manhattanDistance(start, newPos);
            }
                
        }
    }
    return heuristicValue;
}

double GameState::meanGirlHeuristic()
{
    auto& opponentPositions = maxingPlayer == Pawn::First ? secondPositions : firstPositions;
    Coord opponentEnd = maxingPlayer == Pawn::First ? Coord{0,0} : Coord{15,15};
    double heuristicValue{};
    for(const auto& elem : opponentPositions)
    {
        heuristicValue += manhattanDistance(opponentEnd,elem);
    }
    return heuristicValue;
}

double GameState::calculateHeuristic(const Strategy& s)
{
    double h1{}, h2{}, h3{}, h4{};
    if(s.mult1!=0)
        h1 = distanceHeuristic();
    if(s.mult2!=0)
        h2 = jumpHeuristic();
    if(s.mult3!=0)
        h3 = baseHeuristic();
    if(s.mult4!=0)
        h4 = meanGirlHeuristic();
    
    if(isEndingStage() and s.mult2 != 0)
    {
        return 2*s.mult1*h1 + s.mult3*h3 + s.mult4*h4;
    }
    return s.mult1*h1 + s.mult2*h2 + s.mult3*h3 + s.mult4*h4;
}

double GameState::loserScore(Pawn winner)
{
    auto& positions = winner == Pawn::First ? secondPositions : firstPositions;
    Pawn loser = winner == Pawn::First ? Pawn::Second : Pawn::First;
    double score{};
    Coord end = winner == Pawn::First ? Coord{0,0} : Coord{15,15};
    for(const auto& pos : positions)
    {
        if(not isInFinishArea(pos,loser))
            score += manhattanDistance(pos,end);
    }
    return score;
}


std::ostream& operator<<(std::ostream& os, Pawn pawn)
{
    switch(pawn)
    {
        case Pawn::First : os<<"x";
        return os;
        case Pawn::Second : os<<"o";
        return os;
        case Pawn::None : os<<".";
        return os;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, Coord coord)
{
    os<<"("<<coord.x<<","<<coord.y<<")";
    return os;
}

Pawn GameBoard::at(const Coord pos) const
{
    return board.at(pos.y).at(pos.x);
}

void GameBoard::set(const Pawn pawn, const Coord coord)
{
    board.at(coord.y).at(coord.x) = pawn;
}

Pawn GameBoard::at(const int i, const int j) const
{
    return board.at(i).at(j);
}

int GameBoard::size()
{
    return dim;
}

void GameBoard::print() const
{
    std::cout<<". ";
    for(int i=0;i<board.size();i++)
    {
        std::cout<<"- ";
    }
    std::cout<<".\n";
    for(int i=0;i<board.size();i++)
    {
        std::cout<<"| ";
        for(int j=0;j<board.at(0).size();j++)
        {
            std::cout<<board.at(i).at(j)<<" ";
        }
        std::cout<<"|\n";
    }
        std::cout<<". ";
    for(int i=0;i<board.size();i++)
    {
        std::cout<<"- ";
    }
    std::cout<<".\n";
}

Pawn makePawn(char ch)
{
    switch(ch)
    {
        case '1' : return Pawn::First;
        case '2' : return Pawn::Second;
    }
    return Pawn::None;
}