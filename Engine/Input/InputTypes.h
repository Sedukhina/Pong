#pragma once

#include <functional>

typedef std::function<void()> InputActionFunction;

// GLFW SCHEME COPY
enum class InputAction
{
	RELEASED,
	PRESSED,
	REPEATED,
	LAST_ACTION_VALUE = REPEATED
};

enum class InputKey
{
	// Alphabet based on ASCII
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,
	ESC = 256,
	RIGHT = 262,
	LEFT = 263,
	DOWN = 264, 
	UP = 265,
	LAST_KEY_VALUE = UP
};