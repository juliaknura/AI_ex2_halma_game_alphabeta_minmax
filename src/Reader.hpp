#pragma once
#include "GameStructures.hpp"

GameState loadStateFromIStream();
GameState loadStateFromFile();

static constexpr int DIM{16};
