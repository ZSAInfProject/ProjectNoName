#include "GUI.h"

GUI::GUI() {
    GUImodes.push_back(std::shared_ptr<GUIMode>(new GUIModeMiner()));
    GUImodes.push_back(std::shared_ptr<GUIMode>(new GUIModeArchitect()));
    GUImodes.push_back(std::shared_ptr<GUIMode>(new GUIModeManagement()));
    GUImodes.push_back(std::shared_ptr<GUIMode>(new GUIModeSwitcher()));
    GUImodes.at(1)->addWindows(desktop);
    GUImodes.back()->addWindows(desktop);
}

bool GUI::handleEvent(sf::Event& event) {
    desktop.HandleEvent(event);
    if(event.type != sf::Event::MouseButtonPressed)
        return false;
    for(std::shared_ptr<GUIMode> m : GUImodes)
        if(m->handleEvent(event))
            return true;
    return false;
}

void GUI::display(sf::RenderWindow& renderWindow, float deltaTime) {
    desktop.Update(deltaTime/(float)1000);
    sfgui.Display(renderWindow);
}

void GUI::changeMode(int newMode) {

    //!does not work as of now
    for(std::shared_ptr<GUIMode> m : GUImodes)
        if(m->getTag() != newMode && m->getTag() >= 0)
            m->removeWindows(desktop);
        else
            m->addWindows(desktop);


    std::dynamic_pointer_cast<GUIModeSwitcher>(GUImodes.back())->changeMode(newMode);
}

std::shared_ptr<GUIMode> GUI::getGUIMode(int guiMode) {
    return GUImodes.at(guiMode);
}
