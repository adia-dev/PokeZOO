#pragma once

#include "yet_another_math_header_file.h"

/**
 * Enum for input states.
 * NOT_PRESSED: Key is not pressed.
 * PRESSED: Key is pressed.
 * RELEASED: Key is released.
 * DOWN: Key is down.
 */
enum class InputState {
	NOT_PRESSED,
	PRESSED,
	RELEASED,
	DOWN,
};

/**
 * Enum for mouse buttons.
 * UNKNOWN: Unknown mouse button.
 * LEFT: Left mouse button.
 * MIDDLE: Middle mouse button.
 * RIGHT: Right mouse button.
 */
enum class MouseButton {
	UNKNOWN,
	LEFT,
	MIDDLE,
	RIGHT,
};

/**
 * Enum for directions.
 * UP: Up direction.
 * DOWN: Down direction.
 * LEFT: Left direction.
 * RIGHT: Right direction.
 */
enum class Direction { NONE, UP, DOWN, LEFT, RIGHT };

/**
 * Enum for animation directions.
 * FORWARD: Animation plays forward once.
 * REVERSE: Animation plays in reverse once.
 * LOOP: Animation plays forward and then loops.
 * PING_PONG: Animation plays forward and then in reverse.
 */
enum class AnimationDirection { FORWARD, REVERSE, LOOP, PING_PONG };
