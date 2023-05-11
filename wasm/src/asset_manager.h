#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <map>
#include <memory>
#include <stdio.h>
#include <string>

class AssetManager {
  public:
	AssetManager();
	~AssetManager();

	AssetManager(const AssetManager &)            = delete;
	AssetManager &operator=(const AssetManager &) = delete;

	static AssetManager &Get() {
		static AssetManager instance;
		return instance;
	}

	static SDL_Texture &get_texture(const std::string &path);

  private:
	std::map<std::string, SDL_Texture *> _textureMap;
};

#endif
