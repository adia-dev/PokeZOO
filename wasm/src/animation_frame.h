#pragma once

#include "asset_manager.h"

struct AnimationFrame {
	SDL_Rect              rect;
	int                   duration;
	bool                  is_flipped;
	std::function<void()> callback;
	int                   loop_count         = 0;
	int                   current_loop_count = 0;

	AnimationFrame() {}

	AnimationFrame(
	    SDL_Rect              rect,
	    int                   duration,
	    bool                  is_flipped         = false,
	    int                   loop_count         = 0,
	    int                   current_loop_count = 0,
	    std::function<void()> callback           = []() {})
	    : rect(rect), duration(duration), is_flipped(is_flipped), loop_count(loop_count),
	      current_loop_count(current_loop_count), callback(callback) {}

	AnimationFrame(const AnimationFrame& other)
	    : rect(other.rect), duration(other.duration), is_flipped(other.is_flipped), loop_count(other.loop_count),
	      current_loop_count(other.current_loop_count), callback(other.callback) {}

	// overload the ostream operator<< to print the callback
	friend std::ostream& operator<<(std::ostream& os, const AnimationFrame& animation_frame) {
		os << "{\n";
		os << "    \"rect\": {\n";
		os << "        \"x\": " << animation_frame.rect.x << ",\n";
		os << "        \"y\": " << animation_frame.rect.y << ",\n";
		os << "        \"w\": " << animation_frame.rect.w << ",\n";
		os << "        \"h\": " << animation_frame.rect.h << "\n";
		os << "    },\n";
		os << "    \"duration\": " << animation_frame.duration << ",\n";
		os << "    \"is_flipped\": " << std::boolalpha << animation_frame.is_flipped << ",\n";
		os << "    \"loop_count\": " << animation_frame.loop_count << ",\n";
		os << "    \"current_loop_count\": " << animation_frame.current_loop_count << "\n";
		os << "}";
		return os;
	}
};
