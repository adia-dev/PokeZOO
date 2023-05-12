#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#pragma once

#include "utils.h"

enum class InputState {
	NOT_PRESSED,
	PRESSED,
	RELEASED,
	DOWN,
};

enum class MouseButton {
	UNKNOWN,
	LEFT,
	MIDDLE,
	RIGHT,
};

enum class Direction { UP, DOWN, LEFT, RIGHT };

template<typename T>
class Vector2 {
  public:
	T x = 0.0f;
	T y = 0.0f;

	Vector2<T>() = default;
	Vector2<T>(T x, T y): x(x), y(y) {}

	Vector2<T> operator+(const Vector2<T>& other) const { return Vector2<T>(x + other.x, y + other.y); }
	Vector2<T> operator-(const Vector2<T>& other) const { return Vector2<T>(x - other.x, y - other.y); }
	Vector2<T> operator*(const Vector2<T>& other) const { return Vector2<T>(x * other.x, y * other.y); }
	Vector2<T> operator/(const Vector2<T>& other) const { return Vector2<T>(x / other.x, y / other.y); }

	Vector2<T> normalized() const {
		T mag = magnitude();
		if (mag == 0.0f) return zero();
		return Vector2<T>(x / mag, y / mag);
	}

	T dot(const Vector2<T>& other) const { return x * other.x + y * other.y; }
	T cross(const Vector2<T>& other) const { return x * other.y - y * other.x; }

	Vector2<T> lerp(const Vector2<T>& other, float t) const {
		return Vector2<T>(x + t * (other.x - x), y + t * (other.y - y));
	}
	Vector2<T> slerp(const Vector2<T>& other, float t) const {
		float dot           = normalized().dot(other.normalized());
		dot                 = std::clamp(dot, -1.0f, 1.0f);
		float      theta    = std::acos(dot) * t;
		Vector2<T> relative = other - (*this) * dot;
		relative            = relative.normalized();
		return ((*this) * std::cos(theta)) + (relative * std::sin(theta));
	}

	Vector2<T> smooth_damp(const Vector2<T>& target,
	                       const Vector2<T>& current_velocity,
	                       float             smooth_time,
	                       float             max_speed) const {
		float t           = 2.0f / smooth_time;
		t                 = std::clamp(t, 0.0f, 1.0f);
		float      omega  = 2.0f / smooth_time;
		float      x      = omega * t;
		float      exp    = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
		Vector2<T> change = target - (*this);
		Vector2<T> temp   = (current_velocity + change * omega) * t;
		Vector2<T> result = (*this) + (change * exp);
		if (result.magnitude() > max_speed) {
			result = result.normalized() * max_speed;
		}
		return result;
	}

	T magnitude() const { return std::sqrt(x * x + y * y); }

	static Vector2<T> zero() { return Vector2<T>(0, 0); }
	static Vector2<T> one() { return Vector2<T>(1, 1); }
	static Vector2<T> up() { return Vector2<T>(0, 1); }
	static Vector2<T> down() { return Vector2<T>(0, -1); }
	static Vector2<T> left() { return Vector2<T>(-1, 0); }
	static Vector2<T> right() { return Vector2<T>(1, 0); }
};

typedef Vector2<int>          Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float>        Vector2f;
typedef Vector2<double>       Vector2d;

class InputHandler {
  public:
	InputHandler(const InputHandler&) = delete;

	InputHandler()  = default;
	~InputHandler() = default;

	static InputHandler& get() {
		static InputHandler instance;
		return instance;
	}

	static void update_key_states();

	static std::map<SDL_Keycode, InputState> get_key_states() { return get()._key_states; }
	static Vector2f                          get_key_direction();
	static void set_key_state(const SDL_Keycode& code, InputState new_state) { get()._key_states[code] = new_state; }

	static bool is_key_pressed(const SDL_Keycode& code) { return get()._key_states[code] == InputState::PRESSED; }
	static bool is_key_down(const SDL_Keycode& code) { return get()._key_states[code] == InputState::DOWN; }
	static bool is_key_released(const SDL_Keycode& code) { return get()._key_states[code] == InputState::RELEASED; }

	static std::string input_state_to_string(InputState state);
	static std::string key_code_to_string(int code);
	static std::string direction_to_string(const Direction& direction);
	static Vector2f    direction_to_vector(const Direction& direction);

	static void update_mouse_states();

	static std::map<MouseButton, InputState> get_mouse_states() { return get()._mouse_states; }
	static void                              set_mouse_button_state(const MouseButton& button, InputState new_state) {
        get()._mouse_states[button] = new_state;
	}

	static bool is_mouse_pressed(const MouseButton& button) {
		return get()._mouse_states[button] == InputState::PRESSED;
	}
	static bool is_mouse_down(const MouseButton& button) { return get()._mouse_states[button] == InputState::DOWN; }
	static bool is_mouse_released(const MouseButton& button) {
		return get()._mouse_states[button] == InputState::RELEASED;
	}

	static Vector2f get_mouse_position() { return get()._mouse_position; }
	static Vector2i get_mouse_coords(int cell_width = 32, int cell_height = 32) {
		return Vector2i(get()._mouse_position.x / cell_width, get()._mouse_position.y / cell_height);
	}
	static void set_mouse_position(const Vector2f& position) {
		get()._last_mouse_position = get()._mouse_position;
		get()._mouse_position      = position;
	}
	static void set_mouse_position(float x, float y) {
		get()._last_mouse_position = get()._mouse_position;
		get()._mouse_position      = Vector2f(x, y);
	}

	static Vector2f get_mouse_delta() { return get()._last_mouse_position - get()._mouse_position; }

	static Vector2f get_mouse_wheel() { return get()._mouse_wheel; }
	static void     set_mouse_wheel(const Vector2f& wheel) {
        get()._last_mouse_wheel = get()._mouse_wheel;
        get()._mouse_wheel      = wheel;
	}
	static void set_mouse_wheel(float x, float y) {
		get()._last_mouse_wheel = get()._mouse_wheel;
		get()._mouse_wheel      = Vector2f(x, y);
	}

	static Vector2f get_mouse_wheel_delta() { return get()._last_mouse_wheel - get()._mouse_wheel; }

	static std::string mouse_button_to_string(MouseButton button);

	static MouseButton uint8_to_mouse_button(uint8_t button);

	friend std::ostream& operator<<(std::ostream& os, const InputHandler& inputHandler) {
		os << "{\n";
		os << "  \"key_states\": {\n";
		for (auto it = inputHandler._key_states.begin(); it != inputHandler._key_states.end(); ++it) {
			os << "    \"" << key_code_to_string(it->first) << "\": \"" << input_state_to_string(it->second) << "\",\n";
		}
		os << "  },\n";
		os << "  \"key_direction\": {\n";
		os << "    \"x\": " << inputHandler._key_direction.x << ",\n";
		os << "    \"y\": " << inputHandler._key_direction.y << "\n";
		os << "  },\n";
		os << "  \"mouse_states\": {\n";
		for (auto it = inputHandler._mouse_states.begin(); it != inputHandler._mouse_states.end(); ++it) {
			os << "    \"" << mouse_button_to_string(it->first) << "\": \"" << input_state_to_string(it->second)
			   << "\",\n";
		}
		os << "  },\n";
		os << "  \"mouse_position\": {\n";
		os << "    \"x\": " << inputHandler._mouse_position.x << ",\n";
		os << "    \"y\": " << inputHandler._mouse_position.y << "\n";
		os << "  },\n";
		os << "  \"last_mouse_position\": {\n";
		os << "    \"x\": " << inputHandler._last_mouse_position.x << ",\n";
		os << "    \"y\": " << inputHandler._last_mouse_position.y << "\n";
		os << "  },\n";
		os << "  \"mouse_coords\": {\n";
		os << "    \"x\": " << inputHandler.get_mouse_coords().x << ",\n";
		os << "    \"y\": " << inputHandler.get_mouse_coords().y << "\n";
		os << "  },\n";
		os << "  \"mouse_wheel\": {\n";
		os << "    \"x\": " << inputHandler._mouse_wheel.x << ",\n";
		os << "    \"y\": " << inputHandler._mouse_wheel.y << "\n";
		os << "  },\n";
		os << "  \"last_mouse_wheel\": {\n";
		os << "    \"x\": " << inputHandler._last_mouse_wheel.x << ",\n";
		os << "    \"y\": " << inputHandler._last_mouse_wheel.y << "\n";
		os << "  }\n";
		os << "}";
		return os;
	}

  private:
	std::map<SDL_Keycode, InputState> _key_states;
	std::map<MouseButton, InputState> _mouse_states;
	Vector2f                          _key_direction = Vector2f(0, 0);

	Vector2f _mouse_position;
	Vector2f _last_mouse_position;
	Vector2f _mouse_delta;

	Vector2f _mouse_wheel;
	Vector2f _last_mouse_wheel;
	Vector2f _mouse_wheel_delta;
};

#endif
