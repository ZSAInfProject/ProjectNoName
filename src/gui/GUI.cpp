#include "GUI.h"
#include "../Game.h"
#include "../state/GameState.h"

void GUI::init() {
    GUImodes.push_back(std::shared_ptr<GUIMode>(new GUIModeMiner()));
    GUImodes.push_back(std::shared_ptr<GUIMode>(new GUIModeArchitect()));
    GUImodes.push_back(std::shared_ptr<GUIMode>(new GUIModeManagement()));
    GUImodes.push_back(std::shared_ptr<GUIMode>(new GUIModeSwitcher()));
    GUImodes.at(dynamic_cast<GameState*>(&Game::getState())->getGameMode()->getTag())->addWindows(desktop);
    GUImodes.back()->addWindows(desktop);
}

bool GUI::handleEvent(sf::Event& event) {
    desktop.HandleEvent(event);
    if(event.type != sf::Event::MouseButtonPressed)
        return false;
    for(std::shared_ptr<GUIMode> mode : GUImodes)
        if(mode->handleEvent(event))
            return true;
    return false;
}

void GUI::display(sf::RenderWindow& renderWindow, float deltaTime) {

    desktop.Update(deltaTime/(float)1000);
    sfgui.Display(renderWindow);
}

void GUI::changeMode(int newMode) {
    for(std::shared_ptr<GUIMode> mode : GUImodes)
        if(mode->getTag() < 0)
            continue;
        else if(mode->getTag() == newMode)
            mode->addWindows(desktop);
        else
            mode->removeWindows(desktop);

    std::dynamic_pointer_cast<GUIModeSwitcher>(GUImodes.back())->changeMode(newMode);
}

std::shared_ptr<GUIMode> GUI::getGUIMode(int guiMode) {
    return GUImodes.at(guiMode);
}
