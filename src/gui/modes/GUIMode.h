#ifndef NONAME_MODE_H
#define NONAME_MODE_H

#include <SFML/Window/Event.hpp>
#include <SFGUI/Desktop.hpp>

/*!
 * Base class for every in-game mode
*/
class GUIMode {
public:
    //! returns whether mouse click from event was inside of any active window
    virtual bool handleEvent(sf::Event &event) = 0;
    //! adds its own windows to desktop
    virtual void addWindows(sfg::Desktop &desktop) = 0;
    //! removes its own windows from destkop
    virtual void removeWidows(sfg::Desktop &desktop) = 0;
};

#endif //NONAME_MODE_H
