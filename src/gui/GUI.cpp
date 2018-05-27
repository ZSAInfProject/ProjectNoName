#include "GUI.h"

GUI::GUI() {
    modes.at(modes.size()-1)->addWindows(desktop);
    modes.at(0)->addWindows(desktop);
}

bool GUI::handleEvent(sf::Event& event) {
    desktop.HandleEvent(event);
    if(event.type != sf::Event::MouseButtonPressed)
        return false;
    for(GUIMode* m : modes)
        if(m->handleEvent(event))
            return true;
    return false;
}

void GUI::display(sf::RenderWindow& renderWindow, float deltaTime) {
    desktop.Update(deltaTime/(float)1000);
    sfgui.Display(renderWindow);
}

void GUI::changeMode(int newMode) {
    //TODO change game modes (removes windows of last mode then adds windows of new mode) needs other game mode classes
//    architectMode.removeWidows(desktop);
//    mainMode.removeWidows(desktop);
}