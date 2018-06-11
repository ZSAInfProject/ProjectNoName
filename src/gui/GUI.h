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

/*!
 * Shows in-game gui, use GUI::init to setup a class
 */
class GUI {
public:
    //! GUI with default scale @param mode mode to load during a start-up
    GUI(int mode);
    /*!
     * @param mode mode to load during a start-up
     * @param scale GUI scale
     */
    GUI(int mode, float scale);
    /*!
     * Handles events related to gui
     * @param event Event to handle
     * @return Returns true if mouse click was performed inside any gui window
     */
    bool handleEvent(sf::Event& event);
    //! Refreshes gui
    void display(sf::RenderWindow& renderWindow, float deltaTime);
    //! Changes gui mode, do not call without changing game mode
    void changeMode(int newMode);
    //! Returns a pointer to a gui mode
    std::shared_ptr<GUIMode> getGUIMode(int guiMode);
    //! Returns a pointer to last event (allows GUIModes to access event
    sf::Event* getEvent();
    //! Rescales GUI
    void rescale(float newScale);

private:
    sfg::Desktop desktop;
    sfg::SFGUI sfgui;

    std::unique_ptr<GUIAllocation> alloc;

    sf::Event event;
    //! A vector which stores every gui mode
    std::vector<std::shared_ptr<GUIMode>> GUIModes;
};

#endif //NONAME_GUI_H
