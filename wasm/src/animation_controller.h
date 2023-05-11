#pragma once

#include "animation.h"

class AnimationController {
  public:
	AnimationController(/* args */) {}
	AnimationController(const std::map<std::string, Animation>& animations, bool is_playing = true)
	    : _animations(animations), _is_playing(is_playing) {
		if (!_animations.empty()) {
			_current_animation_name = _animations.begin()->first;
		}
	}
	~AnimationController() {}

	void add_animation(const std::string& name, const Animation& animation) { _animations[name] = animation; }
	void remove_animation(const std::string& name) { _animations.erase(name); }
	void clear_animations() { _animations.clear(); }

	void set_animation(const std::string& name) {
		_current_animation_name = name;
		current_frame_index     = 0;
		_timer                  = 0.0f;
	}
	void set_animation_speed(float speed) { this->speed = speed; }

	bool has_animations() { return !_animations.empty(); }

	void play(const std::string& name = "") {
		if (_animations.empty()) return;
		if (_current_animation_name == name) return;
		if (_animations.find(name) == _animations.end()) return;

		if (name != "") {
			_current_animation_name = name;
			current_frame_index     = 0;
			_timer                  = 0.0f;
		}
		_is_playing = true;
	}
	void pause() { _is_playing = false; }

	const AnimationFrame& get_current_frame();

	void update(float delta_time);

  private:
	std::map<std::string, Animation> _animations;
	std::string                      _current_animation_name;

	float speed               = 1.0f;
	int   current_frame_index = 0;
	float _timer              = 0.0f;
	bool  _is_playing         = false;
};
