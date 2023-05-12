#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#pragma once

#include "asset_manager.h"

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
		return Vector2f(x / mag, y / mag);
	}

	T magnitude() const { return std::sqrt(x * x + y * y); }
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
	static void set_key_state(const SDL_Keycode& code, InputState new_state) { get()._key_states[code] = new_state; }

	static bool is_key_pressed(const SDL_Keycode& code) { return get()._key_states[code] == InputState::PRESSED; }
	static bool is_key_down(const SDL_Keycode& code) { return get()._key_states[code] == InputState::DOWN; }
	static bool is_key_released(const SDL_Keycode& code) { return get()._key_states[code] == InputState::RELEASED; }

	static std::string input_state_to_string(InputState state);
	static std::string key_code_to_string(int code);

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

	Vector2f _mouse_position;
	Vector2f _last_mouse_position;
	Vector2f _mouse_delta;

	Vector2f _mouse_wheel;
	Vector2f _last_mouse_wheel;
	Vector2f _mouse_wheel_delta;
};

#endif
