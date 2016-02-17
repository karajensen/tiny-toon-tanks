////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - Input.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <unordered_map>
#include <functional>
#include "glm/glm.hpp"

struct GLFWwindow;

/*
* Manages keyboard and mouse input
*/
class Input
{
public:

    typedef std::function<void(void)> KeyFn;

    /**
    * Constructor
    * @param window The application window to get input from
    */
    Input(GLFWwindow& window);

    /**
    * Updates the input states
    */
    void Update();

    /**
    * Adds a callback for when the key is down
    * @param key The GLFW keyboard code
    * @param onContinous Whether to call on key press or key press continously
    * @param onKeyFn The function to call when the conditions are true
    */
    void AddCallback(unsigned int key, bool onContinous, KeyFn onKeyFn);

    /**
    * @return the normalized direction the mouse is moving
    */
    const glm::vec2& GetMouseDirection() const;

    /**
    * @return whether the mouse is currently being held down
    */
    bool IsRightMouseDown() const;

    /**
    * @return whether the mouse was pressed during this tick
    */
    bool IsLeftMousePressedThisTick() const;

    /**
    * @return whether the mouse was pressed during this tick
    */
    bool IsLeftMouseReleasedThisTick() const;

    /**
    * @return whether the mouse moved during this tick
    */
    bool IsMouseMovedThisTick() const;

    /**
    * @return the mouse X position
    */
    int GetMouseX() const;

    /**
    * @return the mouse Y position
    */
    int GetMouseY() const;

private:

    /**
    * Prevent copying
    */
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    /**
    * Determines if a key is being pressed and held
    * @param state The state of the key
    * @return Whether a key is currently being pressed or not
    */
    bool IsKeyDownContinous(unsigned int state) const;

    /**
    * Whether a key was pressed but not held
    * Once queried will return false until key is released
    * @param state The state of the key
    * @return Whether a key is currently being pressed or not
    */
    bool IsKeyDown(unsigned int& state);

    /**
    * Updates the key state
    * @param pressed Whether the key is currently being pressed
    * @param state The state of the key
    */
    void UpdateKey(bool pressed, unsigned int& state);

    /**
    * Updates the mouse state
    */
    void UpdateMouse();

    /**
    * Masks for state of input key
    */
    enum InputMask
    {
        KEY_NONE = 0,    ///< No current state
        KEY_DOWN = 1,    ///< Whether the key is currently being pressed
        KEY_QUERIED = 2  ///< Whether the key has been queried since pressed
    };

    /**
    * Key state information
    */
    struct Key
    {
		unsigned int key = 0;     ///< The ID of the key
        unsigned int state = 0;   ///< Current state of the key
        KeyFn onKeyFn = nullptr;  ///< Function to call if key is down
        bool continuous = false;  ///< Whether key should look at continous or not
    };

    /**
    * State of the mouse for this tick
    */
    enum MouseState
    {
        NO_STATE = 0,
        PRESSED = 1,
        RELEASED = 2,
        MOVED = 4
    };

    std::vector<Key> m_keys;              ///< Key states and callbacks
    GLFWwindow& m_window;                 ///< The window to get input from
    glm::vec2 m_mouseDirection;           ///< Direction mouse has moved (normalized) between ticks
    int m_mouseX = 0;                     ///< X screen coordinate of the mouse
    int m_mouseY = 0;                     ///< Y screen coordinate of the mouse
    bool m_leftMousePressed = false;      ///< Whether the mouse is currently being pressed
    bool m_rightMousePressed = false;     ///< Whether the mouse is currently being pressed
    unsigned int m_mouseState = NO_STATE; ///< State of the mouse for this tick
};