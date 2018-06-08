#include "GUI.h"
#include "../Game.h"
#include "../state/GameState.h"

GUI::GUI(int mode) : GUI(mode, 1.f) {}

GUI::GUI(int mode, float scale) {
    GUIModes.push_back(std::shared_ptr<GUIMode>(new GUIModeMiner()));
    GUIModes.push_back(std::shared_ptr<GUIMode>(new GUIModeArchitect(scale)));
    GUIModes.push_back(std::shared_ptr<GUIMode>(new GUIModeManagement()));
    GUIModes.push_back(std::shared_ptr<GUIMode>(new GUIModeSwitcher(GUIModes.at(mode)->getName(), scale)));
    GUIModes.at(mode)->addWindows(desktop);
    GUIModes.back()->addWindows(desktop);
    desktop.LoadThemeFromFile("res/themes.txt");
    alloc = std::unique_ptr<GUIAllocation>(new GUIAllocation(scale));
    desktop.SetProperty("*", "FontSize", alloc->fontSize);
}

bool GUI::handleEvent(sf::Event &event) {
    desktop.HandleEvent(event);
    GUI::event = event;
    if (event.type != sf::Event::MouseButtonPressed)
        return false;
    for (std::shared_ptr<GUIMode> mode : GUIModes)
        if (mode->handleEvent(event))
            return true;
    return false;
}

void GUI::display(sf::RenderWindow &renderWindow, float deltaTime) {
    desktop.Update(deltaTime / (float) 1000);
    sfgui.Display(renderWindow);
}

void GUI::changeMode(int newMode) {
    for (std::shared_ptr<GUIMode> mode : GUIModes)
        if (mode->getTag() < 0)
            continue;
        else if (mode->getTag() == newMode)
            mode->addWindows(desktop);
        else
            mode->removeWindows(desktop);

    std::dynamic_pointer_cast<GUIModeSwitcher>(GUIModes.back())->changeMode(newMode);
}

std::shared_ptr<GUIMode> GUI::getGUIMode(int guiMode) {
    return GUIModes.at(guiMode);
}

sf::Event *GUI::getEvent() {
    return &event;
}
