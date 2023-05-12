#ifndef ANIMATION_H
#define ANIMATION_H

#pragma once

#include "animation_frame.h"

#include <iomanip>

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
			printf("Animation::get_frame() - Index out of range: %d not in [0-%zu]\n", index, frames.size() - 1);
			throw std::out_of_range("Animation::get_frame() - Index out of range");
		}
		return frames[index];
	}
	void add_frame(AnimationFrame frame) { frames.push_back(frame); }
	void remove_frame(int index) { frames.erase(frames.begin() + index); }
	void clear_frames() { frames.clear(); }

	// overload the ostream operator<< to print the frames
	friend std::ostream& operator<<(std::ostream& os, const Animation& animation) {
		os << "{" << std::endl;
		os << "  \"name\": \"" << animation.name << "\"," << std::endl;
		os << "  \"direction\": \"" << static_cast<int>(animation.direction) << "\"," << std::endl;
		os << "  \"frames\": [" << std::endl;

		for (size_t i = 0; i < animation.frames.size(); i++) {
			const AnimationFrame& frame = animation.frames[i];
			os << "    {" << std::endl;
			os << "      \"x\": " << frame.rect.x << "," << std::endl;
			os << "      \"y\": " << frame.rect.y << "," << std::endl;
			os << "      \"width\": " << frame.rect.w << "," << std::endl;
			os << "      \"height\": " << frame.rect.h << "," << std::endl;
			os << "      \"duration\": " << std::fixed << std::setprecision(1) << frame.duration;
			os << (i < animation.frames.size() - 1 ? "," : "") << std::endl;
			os << "    }" << std::endl;
		}

		os << "  ]" << std::endl;
		os << "}";

		return os;
	}
};

#endif
