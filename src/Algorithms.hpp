#pragma once

#include "GameStructures.hpp"
#include "NodeCounter.hpp"
#include <optional>


std::pair<double,std::optional<GameState>> minimax(GameState& state, bool isMaxPlayer, int depth, int initDepth, const Strategy& strat);

std::pair<double,std::optional<GameState>> alphabeta(GameState& state, bool isMaxPlayer, int depth, int initDepth, double alpha, double beta, const Strategy& strat);