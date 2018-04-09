#ifndef NONAME_OBJECT_H
#define NONAME_OBJECT_H


#include <SFML/Graphics/RenderWindow.hpp>
#include <chrono>

class Object {
    int position[2];
public:
    void render(sf::RenderWindow);
    void clickOn();
    void update(std::chrono::microseconds deltaTime);
};


#endif //NONAME_OBJECT_H
