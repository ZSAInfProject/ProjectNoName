#include "GUI.h"
#include "../Game.h"
#include "../state/GameState.h"
#include "GUIAllocation.h"

GUI::GUI(int mode) : GUI(mode, 1.f) {   }

GUI::GUI(int mode, float scale) {
    GUIModes.push_back(std::shared_ptr<GUIMode>(new GUIModeMiner()));
    GUIModes.push_back(std::shared_ptr<GUIMode>(new GUIModeArchitect(scale)));
    GUIModes.push_back(std::shared_ptr<GUIMode>(new GUIModeManagement()));
    GUIModes.push_back(std::shared_ptr<GUIMode>(new GUIModeSwitcher(GUIModes.at(mode)->getName(), scale)));
    GUIModes.at(mode)->addWindows(desktop);
    GUIModes.back()->addWindows(desktop);
    desktop.LoadThemeFromFile("themes.txt");
    alloc = std::make_unique<GUIAllocation>(GUIAllocation(scale));
    desktop.SetProperty("*", "FontSize", alloc->fontSize);
}

bool GUI::handleEvent(sf::Event& event) {
    desktop.HandleEvent(event);
    GUI::event = event;
    if(event.type == sf::Event::Resized)
        for(std::shared_ptr<GUIMode> mode : GUIModes)
            mode->resize(event.size.width, event.size.height);
    if(event.type != sf::Event::MouseButtonPressed)
        return false;
    for(std::shared_ptr<GUIMode> mode : GUIModes)
        if(mode->handleEvent(event))
            return true;
    return false;
}

void GUI::display(sf::RenderWindow& renderWindow, float deltaTime) {

    desktop.Update(deltaTime/(float)1000);
    sfgui.Display(renderWindow);
}

void GUI::changeMode(int newMode) {
    for(std::shared_ptr<GUIMode> mode : GUIModes)
        if(mode->getTag() < 0)
            continue;
        else if(mode->getTag() == newMode)
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

void GUI::rescale(float newScale) {
    alloc = std::make_unique<GUIAllocation>(GUIAllocation(newScale));
    desktop.SetProperty("*", "FontSize", alloc->fontSize);
    for(std::shared_ptr<GUIMode> mode : GUIModes)
        mode->rescale(newScale);
}
