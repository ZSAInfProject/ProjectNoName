#ifndef NONAME_CONTROLS_H
#define NONAME_CONTROLS_H

#include <queue>
#include <mutex>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <future>
#include <map>

class Controls {

public:
    static constexpr auto TAG = "Controls";

    //! Checks if mouse button was pressed in last frame.
    static bool isMouseButtonPressed(sf::Mouse::Button);
    //! Checks if keyboard button was pressed in last frame.
    static bool isKeyboardButtonPressed(sf::Keyboard::Key);
    //! Return a mouse position in last frame
    static sf::Vector2f getMousePosition();

    //! Method used by render thread to set mouse position
    static void setMousePosition(sf::Vector2f pos);
    //! Method used by render thread to set mouse button
    static void setMouseButton(sf::Mouse::Button, bool val);
    //! Method used by render thread to set keyboard key
    static void setKeyboardKey(sf::Keyboard::Key, bool val);

public:
    //! Returns reference to singleton object.
    static Controls& get() {
        static Controls instance;
        return instance;
    }

    Controls(Controls const&) = delete;
    void operator=(Controls const&) = delete;

private:
    sf::Vector2f mousePos;
    Controls() = default;
    std::map<sf::Keyboard::Key, bool> keyboardMap;
    std::map<sf::Mouse::Button, bool> mouseMap;
};


#endif //NONAME_CONTROLS_H
