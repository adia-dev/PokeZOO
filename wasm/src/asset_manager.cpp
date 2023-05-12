#include "asset_manager.h"

#include "application.h"

#include <stdexcept>

AssetManager::AssetManager() {}

AssetManager::~AssetManager() {}

SDL_Texture &AssetManager::get_texture(const std::string &path) {
	load_texture(path);
	return *(AssetManager::get())._textureMap[path];
}

bool AssetManager::load_texture(const std::string &path) {
	auto &textureMap = AssetManager::get()._textureMap;
	if (textureMap.find(path) == textureMap.end()) {
		SDL_Surface *surface = IMG_Load(path.c_str());
		if (surface == nullptr) {
			throw std::runtime_error("Failed to load image: " + path);
		}

		SDL_Texture *texture = SDL_CreateTextureFromSurface(Application::get_renderer(), surface);
		if (texture == nullptr) {
			throw std::runtime_error("Failed to create texture from surface: " + path);
		}

		textureMap[path] = texture;
		SDL_FreeSurface(surface);
	}

	return true;
}

TTF_Font &AssetManager::get_font(const std::string &path, int size) {
	load_font(path, size);
	return *(AssetManager::get())._fontMap[path];
}

bool AssetManager::load_font(const std::string &path, int size) {
	auto &fontMap = AssetManager::get()._fontMap;
	if (fontMap.find(path) == fontMap.end()) {
		TTF_Font *font = TTF_OpenFont(path.c_str(), size);
		if (font == nullptr) {
			throw std::runtime_error("Failed to load font: " + path);
		}

		fontMap[path] = font;
	}

	return true;
}
