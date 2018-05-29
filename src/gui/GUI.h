#ifndef NONAME_GUI_H
#define NONAME_GUI_H

#include "modes/GUIModeMiner.h"
#include "modes/GUIModeManagement.h"
#include "modes/GUIModeArchitect.h"
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "modes/GUIModeSwitcher.h"

class GUI {
public:
    GUI();
    bool handleEvent(sf::Event& event);
    void display(sf::RenderWindow& renderWindow, float deltaTime);
    void changeMode(int newMode);
    std::shared_ptr<GUIMode> getGUIMode(int guiMode);

private:
    sfg::Desktop desktop;
    sfg::SFGUI sfgui = sfg::SFGUI();

    std::vector<std::shared_ptr<GUIMode>> GUImodes;
};

#endif //NONAME_GUI_H
