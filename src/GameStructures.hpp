#pragma once
#include <vector>
#include <iostream>
#include <tuple>
#include <unordered_set>
#include <memory>

struct Coord{
    int x; int y;

    bool operator!=(const Coord& rhs) const { return std::tie(x,y) != std::tie(rhs.x,rhs.y); }
    bool operator==(const Coord& rhs) const { return not operator!=(rhs); }
};

std::ostream& operator<<(std::ostream& os, Coord cord);

struct Hash {
   size_t operator()(const Coord &coord) const {
     std::string temp = std::to_string(coord.x) + std::to_string(coord.y);
     return (std::stoi(temp));
   }
};

enum class Pawn{
    First, Second, None
};

Pawn makePawn(char ch);

std::ostream& operator<<(std::ostream& os, Pawn pawn);

struct GameBoard {
private:
    std::vector<std::vector<Pawn>> board;
    int dim;
public:
    GameBoard(std::vector<std::vector<Pawn>> board):board(board),dim(board.size()){};
    Pawn at(const Coord coord) const;
    void set(const Pawn pawn, const Coord coord);
    Pawn at(const int i, const int j) const;
    void print() const;
    int size();
};

struct Strategy{
    double mult1;
    double mult2;
    double mult3;
    double mult4;
};

struct GameState{
    bool currentPlayer; //1 = true, 2 = false
    GameBoard board;
    std::vector<Coord> firstPositions;
    std::vector<Coord> secondPositions;
    std::vector<GameState> children;
    Pawn maxingPlayer;

    void generateChildren();
    std::vector<Coord> discoverJumps(const Coord& start, std::unordered_set<Coord,Hash> prev);
    std::unordered_set<Coord,Hash> discoverJumpsIter(const Coord& start);
    GameBoard makeMove(const Coord& start, const Coord& end, const Pawn pawn);
    void generateChild(const Coord& newPos, const Coord& currentPos, const Pawn pawn);

    bool isWithinBounds(const Coord pos);
    bool isSquareOccupied(const Coord pos);

    Pawn gameWonBy();
    bool isEndingStage();
    double calculateHeuristic(const Strategy& s);
    double distanceHeuristic();
    double jumpHeuristic();
    double baseHeuristic();
    double meanGirlHeuristic();
    double loserScore(Pawn winner);
};

static constexpr std::array<Coord,19> firstBase = {Coord{0,0},Coord{0,1},Coord{0,2},Coord{0,3},Coord{0,4},Coord{1,0},Coord{1,1},Coord{1,2},Coord{1,3},Coord{1,4},Coord{2,0},Coord{2,1},Coord{2,2},Coord{2,3},Coord{3,0},Coord{3,1},Coord{3,2},Coord{4,0},Coord{4,1}};
static constexpr std::array<Coord,19> secondBase = {Coord{15,15},Coord{15,14},Coord{15,13},Coord{15,12},Coord{15,11},Coord{14,15},Coord{14,14},Coord{14,13},Coord{14,12},Coord{14,11},Coord{13,15},Coord{13,14},Coord{13,13},Coord{13,12},Coord{12,15},Coord{12,14},Coord{12,13},Coord{11,15},Coord{11,14}};

