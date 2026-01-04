#include "input.hpp"
#include "game.hpp"

Input* g_Input = nullptr;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Input::IsKeyDown(SDL_Scancode key)
{
    return m_keyDown[key];
}

bool Input::IsKeyPressed(SDL_Scancode key)
{
    return m_keyPressed[key];
}

bool Input::IsKeyReleased(SDL_Scancode key)
{
    return m_keyReleased[key];
}

//-----------------------------------------------------------------------------
bool Input::IsButtonDown(Uint8 button)
{
    return m_mouseDown[button];
}

bool Input::IsButtonPressed(Uint8 button)
{
    return m_mousePressed[button];
}

bool Input::IsButtonReleased(Uint8 button)
{
    return m_mouseReleased[button];
}

//-----------------------------------------------------------------------------
void Input::SetRelativeMouseMode(bool value)
{
    SDL_SetWindowRelativeMouseMode(g_Game->GetWindow(), value);
}

bool Input::GetRelativeMouseMode()
{
    return SDL_GetWindowRelativeMouseMode(g_Game->GetWindow());
}

void Input::GetMousePos(float* x, float* y)
{
    SDL_GetMouseState(x, y);
}

void Input::GetMouseDelta(float* x, float* y)
{
    SDL_GetRelativeMouseState(x, y);
}

void Input::GetMouseScroll(float* x, float* y)
{
    x = &m_scrollDeltaX;
    y = &m_scrollDeltaY;
}

void Input::Reset()
{
    for (int i = 0; i < SDL_SCANCODE_COUNT; ++i)
    {
        m_keyPressed[(SDL_Scancode)i] = false;
        m_keyReleased[(SDL_Scancode)i] = false;
    }

    for (int i = 0; i < 8; ++i)
    {
        m_mousePressed[i] = false;
        m_mouseReleased[i] = false;
    }

    m_scrollDeltaX = 0;
    m_scrollDeltaY = 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Input::OnKey(SDL_KeyboardEvent keyEvent)
{
    if (keyEvent.type == SDL_EVENT_KEY_DOWN && !keyEvent.repeat)
    {
        m_keyDown[keyEvent.scancode] = true;
        m_keyPressed[keyEvent.scancode] = true;
        if (m_keyReleased[keyEvent.scancode])
            m_keyReleased[keyEvent.scancode] = false;
    }
    else if (keyEvent.type == SDL_EVENT_KEY_UP)
    {
        m_keyDown[keyEvent.scancode] = false;
        if(!m_keyReleased[keyEvent.scancode])
            m_keyReleased[keyEvent.scancode] = true;
    }
}

void Input::OnMouseMotion(SDL_MouseMotionEvent mouseEvent)
{

}

void Input::OnMouseButton(SDL_MouseButtonEvent buttonEvent)
{
    if (buttonEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        m_mouseDown[buttonEvent.button] = true;
        m_mousePressed[buttonEvent.button] = true;
        if (m_mouseReleased[buttonEvent.button])
            m_mouseReleased[buttonEvent.button] = false;
    }
    else if (buttonEvent.type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        m_mouseDown[buttonEvent.button] = false;
        if (!m_mouseReleased[buttonEvent.button])
            m_mouseReleased[buttonEvent.button] = true;
    }
}

void Input::OnMouseWheel(SDL_MouseWheelEvent wheelEvent)
{
    m_scrollDeltaX += wheelEvent.x;
    m_scrollDeltaY += wheelEvent.y;
}

const SDL_FPoint Input::GetMousePoint()
{
    float x, y;
    SDL_GetMouseState(&x, &y);
    SDL_FPoint point = { x, y };
    return point;
}