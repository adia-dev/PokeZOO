#ifndef CHARACTER_H
#define CHARACTER_H

#pragma once

#include "sprite.h"

class Character: public Sprite {
  public:
	Character(SDL_Texture& texture, const SDL_Rect& frame_rect, const SDL_Rect& world_rect);
	Character(SDL_Texture& texture, const SDL_Rect& frame_rect, int x, int y, int w, int h);
	Character(const Character& other);
	virtual ~Character() = default;

	virtual void render(SDL_Renderer* renderer) override;
	virtual void update(float delta_time) override;

	void move(int x, int y) override;

	void set_on_bike(bool is_on_bike);
	void toggle_on_bike();
	bool get_on_bike() const;

  private:
	bool _on_bike = false;
};

#endif