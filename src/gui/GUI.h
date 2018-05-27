#ifndef NONAME_GUI_H
#define NONAME_GUI_H

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "modes/MainMode.h"
#include "modes/ArchitectMode.h"
#include "modes/GUIMode.h"

class GUI {
public:
    GUI();
    bool handleEvent(sf::Event& event);
    void display(sf::RenderWindow& renderWindow, float deltaTime);
    void changeMode(int newMode);


private:
    sfg::Desktop desktop;
    sfg::SFGUI sfgui = sfg::SFGUI();

    //TODO add other gamemodes
    std::vector<GUIMode*> modes = {
//            MinerMode(),;
            new ArchitectMode(),
//            ManagementMode();
            new MainMode()
    };
};

#endif //NONAME_GUI_H
