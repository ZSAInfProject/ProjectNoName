#include "GUI.h"

GUI::GUI() {
    GUImodes.push_back(new GUIModeMiner::GUIModeMiner());
    GUImodes.push_back(new GUIModeArchitect::GUIModeArchitect());
    GUImodes.push_back(new GUIModeManagement::GUIModeManagement());
    GUImodes.push_back(new GUIModeSwitcher::GUIModeSwitcher());
    GUImodes.at(1)->addWindows(desktop);
    GUImodes.at(GUImodes.size()-1)->addWindows(desktop);
}

bool GUI::handleEvent(sf::Event& event) {
    desktop.HandleEvent(event);
    if(event.type != sf::Event::MouseButtonPressed)
        return false;
    for(GUIMode* m : GUImodes)
        if(m->handleEvent(event))
            return true;
    return false;
}

void GUI::display(sf::RenderWindow& renderWindow, float deltaTime) {
    desktop.Update(deltaTime/(float)1000);
    sfgui.Display(renderWindow);
}

void GUI::changeMode(int newMode) {
    for(GUIMode* m : GUImodes)
        if(m->getTag() != newMode && m->getTag() >= 0)
            m->removeWindows(desktop);
        else
            m->addWindows(desktop);
}