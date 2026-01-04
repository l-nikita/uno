#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>

class Input
{
public:
	bool IsKeyDown(SDL_Scancode key);
	bool IsKeyPressed(SDL_Scancode key);
	bool IsKeyReleased(SDL_Scancode key);

	bool IsButtonDown(Uint8 button);
	bool IsButtonPressed(Uint8 button);
	bool IsButtonReleased(Uint8 button);

	void Reset();

	void SetRelativeMouseMode(bool value);
	bool GetRelativeMouseMode();

	void GetMousePos(float* x, float* y);
	void GetMouseDelta(float* x, float* y);
	void GetMouseScroll(float* x, float* y);

	void OnKey(SDL_KeyboardEvent keyEvent);
	void OnMouseMotion(SDL_MouseMotionEvent mouseEvent);
	void OnMouseButton(SDL_MouseButtonEvent buttonEvent);
	void OnMouseWheel(SDL_MouseWheelEvent wheelEvent);

	const SDL_FPoint GetMousePoint();

private:
	float m_scrollDeltaX = 0;
	float m_scrollDeltaY = 0;

	std::unordered_map<SDL_Scancode, bool> m_keyDown;
	std::unordered_map<SDL_Scancode, bool> m_keyPressed;
	std::unordered_map<SDL_Scancode, bool> m_keyReleased;

	std::unordered_map<Uint8, bool> m_mouseDown;
	std::unordered_map<Uint8, bool> m_mousePressed;
	std::unordered_map<Uint8, bool> m_mouseReleased;
};

extern Input* g_Input;