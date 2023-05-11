#ifndef ANIMATION_H
#define ANIMATION_H

#pragma once

#include "animation_frame.h"

enum class AnimationDirection { FORWARD, REVERSE, LOOP, PING_PONG };

struct Animation {
	std::string                 name;
	std::vector<AnimationFrame> frames;
	AnimationDirection          direction = AnimationDirection::FORWARD;

	Animation() {}
	Animation(std::string                 name,
	          std::vector<AnimationFrame> frames,
	          AnimationDirection          direction = AnimationDirection::FORWARD)
	    : name(name), frames(frames), direction(direction) {}
	Animation(std::string        name,
	          SDL_Rect           start,
	          int                rows,
	          int                columns,
	          AnimationDirection direction = AnimationDirection::FORWARD,
	          float              duration  = 1.0f)
	    : name(name), direction(direction) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				frames.push_back(
				    AnimationFrame({start.x + start.w * j, start.y + start.h * i, start.w, start.h}, duration));
			}
		}
	}
	Animation(const Animation& other): name(other.name), frames(other.frames) {}

	AnimationFrame& get_frame(int index) {
		if (index >= frames.size()) {
			printf("Animation::get_frame() - Index out of range: %d not in [0-%d]\n", index, frames.size() - 1);
			throw std::out_of_range("Animation::get_frame() - Index out of range");
		}
		return frames[index];
	}
	void add_frame(AnimationFrame frame) { frames.push_back(frame); }
	void remove_frame(int index) { frames.erase(frames.begin() + index); }
	void clear_frames() { frames.clear(); }

	// overload the ostream operator<< to print the frames
	friend std::ostream& operator<<(std::ostream& os, const Animation& animation) {
		os << "{\n\tname: " << animation.name << ",\n\tframes: [";

		for (int i = 0; i < animation.frames.size(); i++) {
			os << "\n\t\t" << animation.frames[i];

			if (i < animation.frames.size() - 1) os << ",";
		}

		os << "\n\t]\n}";

		return os;
	}
};

#endif
