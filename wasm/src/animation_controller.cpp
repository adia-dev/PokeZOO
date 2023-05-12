#include "animation_controller.h"

const AnimationFrame& AnimationController::get_current_frame() {
	if (_animations.empty() || _current_animation_name.empty()) {
		throw std::runtime_error(
		    "AnimationController::get_current_frame() - No animations or current animation name is empty");
	}

	return _animations[_current_animation_name].get_frame(current_frame_index);
}

void AnimationController::update(float delta_time) {
	if (_animations.empty() || _current_animation_name.empty()) return;
	if (_animations.find(_current_animation_name) == _animations.end()) return;

	Animation& animation = _animations[_current_animation_name];

	_timer += delta_time * speed;

	if (_timer >= animation.get_frame(current_frame_index).duration) {
		_timer = 0.0f;

		switch (animation.direction) {
			case AnimationDirection::FORWARD:
				current_frame_index++;

				if (current_frame_index >= animation.frames.size()) current_frame_index = 0;

				break;
			case AnimationDirection::REVERSE:
				current_frame_index--;

				if (current_frame_index < 0) current_frame_index = animation.frames.size() - 1;

				break;
			case AnimationDirection::LOOP:
				current_frame_index++;

				if (current_frame_index >= animation.frames.size()) current_frame_index = 0;

				break;
			case AnimationDirection::PING_PONG:
				if (current_frame_index == 0) {
					animation.direction = AnimationDirection::FORWARD;
					current_frame_index++;
				} else if (current_frame_index == animation.frames.size() - 1) {
					animation.direction = AnimationDirection::REVERSE;
					current_frame_index--;
				} else if (animation.direction == AnimationDirection::FORWARD) {
					current_frame_index++;
				} else if (animation.direction == AnimationDirection::REVERSE) {
					current_frame_index--;
				}

				break;
		}
	}
}
