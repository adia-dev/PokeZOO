#pragma once

#include "tinyxml2.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <stdio.h>
#include <string>
#include <vector>

#define WINDOW_WIDTH  960
#define WINDOW_HEIGHT 512
#define WINDOW_FLAGS  SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS

#define TILE_SIZE      16
#define CHARACTER_SIZE 32

#define FPS               60
#define FRAME_TARGET_TIME (1000 / FPS)

#define MAX_ENTITIES 1024