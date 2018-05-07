//
// Created by zakrent on 5/7/18.
//

#include "Controls.h"

bool Controls::isMouseButtonPressed(sf::Mouse::Button button) {
    return Controls::get().mouseMap[button];
}

bool Controls::isKeyboardButtonPressed(sf::Keyboard::Key key) {
    return Controls::get().keyboardMap[key];
}

sf::Vector2f Controls::getMousePosition() {
    return Controls::get().mousePos;
}

void Controls::setMousePosition(sf::Vector2f val) {
    Controls::get().mousePos = val;
}

void Controls::setMouseButton(sf::Mouse::Button button, bool val) {
    Controls::get().mouseMap[button] = val;
}

void Controls::setKeyboardKey(sf::Keyboard::Key key, bool val) {
    Controls::get().keyboardMap[key] = val;
}

