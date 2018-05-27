//
// Created by arek on 26.05.18.
//

#ifndef NONAME_MAINMODE_H
#define NONAME_MAINMODE_H

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "GUIMode.h"



class MainMode : public GUIMode {
public:
    MainMode();
    void changeMode(int newMode) const;
    bool handleEvent(sf::Event &event);
    void addWindows(sfg::Desktop &desktop);
    void removeWidows(sfg::Desktop &desktop);

private:
    sfg::Window::Ptr switchWindow;

    void containerSetup(sfg::Fixed::Ptr &container) const;
    void eventSetup(sfg::Label::Ptr &labelActiveMode) const;


    //TODO move enum and map to other location (preferably GameState.h)
    //enum for mode ids
    enum modesEnum{
        minerMode = 0,
        architectMode = 1,
        managementMode = 2
    };

    //map for mode ids, use mode id +3 to get mode name
    const std::map<int, std::string> modes ={
            {0, "minerMode"},
            {1, "architectMode"},
            {2, "managementMode"},
            {3, "miner mode"},
            {4, "architect mode"},
            {5, "management mode"}
    };
};


#endif //NONAME_MAINMODE_H
