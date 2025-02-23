#include "Algorithms.hpp"
#include <limits>

std::pair<double,std::optional<GameState>> minimax(GameState& state, bool isMaxPlayer, int depth, int initDepth, const Strategy& strat)
{
    Counter::value += 1;
    if(depth==0 or state.gameWonBy() != Pawn::None)
        return {state.calculateHeuristic(strat),{}};
    double value = isMaxPlayer ? std::numeric_limits<double>::lowest() : std::numeric_limits<double>::max();
    state.generateChildren();
    std::optional<GameState> returned;
    if(isMaxPlayer)
    {
        for(auto& child: state.children)
        {
            auto [childVal,childRet] = minimax(child,!isMaxPlayer,depth-1,initDepth,strat);
            if(childVal > value)
            {
                value = childVal;
                if(depth==initDepth)
                    returned=child;
            }
        }
    }
    else
    {
        for(auto& child: state.children)
        {
            auto [childVal,childRet] = minimax(child,!isMaxPlayer,depth-1,initDepth,strat);
            if(childVal < value)
            {
                value = childVal;
                if(depth==initDepth)
                    returned=child;
            }
        }
    }
    return {value,returned};
}

std::pair<double,std::optional<GameState>> alphabeta(GameState& state, bool isMaxPlayer, int depth, int initDepth, double alpha, double beta, const Strategy& strat)
{
    Counter::value += 1;
    if(depth==0 or state.gameWonBy() != Pawn::None)
        return {state.calculateHeuristic(strat),{}};
    double value = isMaxPlayer ? std::numeric_limits<double>::lowest() : std::numeric_limits<double>::max();
    state.generateChildren();
    std::optional<GameState> returned;
    if(isMaxPlayer)
    {
        for(auto& child: state.children)
        {
            auto [childVal,childRet] = alphabeta(child,!isMaxPlayer,depth-1,initDepth,alpha,beta,strat);
            if(childVal > value)
            {
                value = childVal;
                if(depth==initDepth)
                    returned=child;
            }
            if(value > beta)
                break;
            alpha = std::max(alpha,value);
        }
    }
    else
    {
        for(auto& child: state.children)
        {
            auto [childVal,childRet] = alphabeta(child,!isMaxPlayer,depth-1,initDepth,alpha,beta,strat);
            if(childVal < value)
            {
                value = childVal;
                if(depth==initDepth)
                    returned=child;
            }
            if(value < alpha)
                break;
            beta = std::min(beta,value);
        }
    }
    return {value,returned};
}