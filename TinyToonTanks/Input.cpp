////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Input.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "input.h"
#include "Glcommon.h"

Input::Input(GLFWwindow& window) :
    m_window(window)
{
}

void Input::UpdateMouse()
{
    m_mouseState = NO_STATE;

    m_rightMousePressed = glfwGetMouseButton(
        &m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    const bool leftPressed = glfwGetMouseButton(
        &m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (leftPressed && !m_leftMousePressed)
    {
        m_mouseState |= PRESSED;
    }
    else if (!leftPressed & m_leftMousePressed)
    {
        m_mouseState |= RELEASED;
    }

    m_leftMousePressed = leftPressed;

    double xPosition = 0.0, yPosition = 0.0;
    glfwGetCursorPos(&m_window, &xPosition, &yPosition);

    const int x = static_cast<int>(xPosition);
    const int y = static_cast<int>(yPosition);

    m_mouseDirection.x = static_cast<float>(m_mouseX) - x;
    m_mouseDirection.y = static_cast<float>(m_mouseY) - y;

    const float length = m_mouseDirection.Length();
    if (length != 0.0f)
    {
        m_mouseDirection.x /= length;
        m_mouseDirection.y /= length;
        m_mouseState |= MOVED;
    }

    m_mouseX = x;
    m_mouseY = y;
}

void Input::Update()
{
    UpdateMouse();

    for(auto& key : m_keys)
    {
        const bool pressed = glfwGetKey(&m_window, key.first) == GLFW_PRESS;
        UpdateKey(pressed, key.second.state);

        const bool keyDown = key.second.continuous ? 
            IsKeyDownContinous(key.second.state) : 
            IsKeyDown(key.second.state);

        if(keyDown && key.second.onKeyFn != nullptr)
        {
            key.second.onKeyFn();
        }
    }
}

void Input::UpdateKey(bool pressed, unsigned int& state)
{
    if(pressed && (state & KEY_DOWN) != KEY_DOWN)
    {
        //save key as pressed
        state |= KEY_DOWN;
    }
    else if(!pressed && (state & KEY_DOWN) == KEY_DOWN)
    {
        //save key as not pressed
        state &= ~KEY_DOWN;
    }
}

bool Input::IsKeyDownContinous(unsigned int state) const
{
    return (state & KEY_DOWN) == KEY_DOWN;
}

bool Input::IsKeyDown(unsigned int& state)
{
    if(((state & KEY_DOWN) == KEY_DOWN) &&
       ((state & KEY_QUERIED) != KEY_QUERIED))
    {
        // Add user has queried for key to prevent constant queries
        state |= KEY_QUERIED;
        return true;
    }
    else if((state & KEY_DOWN) != KEY_DOWN)
    {
        // Key is lifted, allow user to query again
        state &= ~KEY_QUERIED;
    }
    return false;
}

void Input::AddCallback(unsigned int key, bool onContinous, KeyFn onKeyFn)
{
    m_keys[key].state = KEY_NONE;
    m_keys[key].continuous = onContinous;
    m_keys[key].onKeyFn = onKeyFn;
}

const Float2& Input::GetMouseDirection() const
{
    return m_mouseDirection;
}

bool Input::IsRightMouseDown() const
{
    return m_rightMousePressed;
}

bool Input::IsLeftMousePressedThisTick() const
{
    return (m_mouseState & PRESSED) == PRESSED;
}

bool Input::IsLeftMouseReleasedThisTick() const
{
    return (m_mouseState & RELEASED) == RELEASED;
}

bool Input::IsMouseMovedThisTick() const
{
    return (m_mouseState & MOVED) == MOVED;
}

int Input::GetMouseX() const
{
    return m_mouseX;
}

int Input::GetMouseY() const
{
    return m_mouseY;
}