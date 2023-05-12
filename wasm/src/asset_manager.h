#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#pragma once

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

class AssetManager {
  public:
	AssetManager();
	~AssetManager();

	AssetManager(const AssetManager &)            = delete;
	AssetManager &operator=(const AssetManager &) = delete;

	static AssetManager &get() {
		static AssetManager instance;
		return instance;
	}

	static SDL_Texture &get_texture(const std::string &path);
	static bool         load_texture(const std::string &path);

	static TTF_Font &get_font(const std::string &path, int size);
	static bool      load_font(const std::string &path, int size);

  private:
	std::map<std::string, SDL_Texture *> _textureMap;
	std::map<std::string, TTF_Font *>    _fontMap;
};

#endif
