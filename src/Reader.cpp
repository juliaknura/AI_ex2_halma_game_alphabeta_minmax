#include "Reader.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

GameState loadStateFromIStream()
{
    std::string line;
    std::string pawn;
    std::vector<std::vector<Pawn>> boardVec;
    std::vector<Coord> firstPositions;
    std::vector<Coord> secondPositions;
    int i{}, j{};

    while(i<DIM and getline(std::cin,line))
    {
        std::vector<Pawn> vec;
        std::stringstream ss{line};
        j=0;
        while(getline(ss,pawn,' '))
        {
            const auto p = makePawn(pawn.at(0));
            vec.push_back(p);
            if(p==Pawn::First)
                firstPositions.push_back(Coord{j,i});
            if(p==Pawn::Second)
                secondPositions.push_back(Coord{j,i});
            j++;
        }
        boardVec.push_back(vec);
        i++;
    }

    GameBoard board{boardVec};

    board.print();

    return {
        true,board,firstPositions,secondPositions,{},Pawn::First
    };
}

GameState loadStateFromFile()
{
    std::ifstream file("input.txt");
    std::string line;
    std::string pawn;
    std::vector<std::vector<Pawn>> boardVec;
    std::vector<Coord> firstPositions;
    std::vector<Coord> secondPositions;
    int i{}, j{};

    while(i<DIM and getline(file,line))
    {
        std::vector<Pawn> vec;
        std::stringstream ss{line};
        j=0;
        while(getline(ss,pawn,' '))
        {
            const auto p = makePawn(pawn.at(0));
            vec.push_back(p);
            if(p==Pawn::First)
                firstPositions.push_back(Coord{j,i});
            if(p==Pawn::Second)
                secondPositions.push_back(Coord{j,i});
            j++;
        }
        boardVec.push_back(vec);
        i++;
    }

    GameBoard board{boardVec};

    board.print();

    return {
        true,board,firstPositions,secondPositions,{},Pawn::First
    };
}