#ifndef NONAME_MODE_H
#define NONAME_MODE_H

#include <SFML/Window/Event.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

/*!
 * Base gui class for every game mode
*/
class GUIMode {
public:
    //! returns whether mouse click occured inside of any active window
    virtual bool handleEvent(sf::Event &event) = 0;
    //! adds windows to desktop
    virtual void addWindows(sfg::Desktop& desktop) = 0;
    /*!
     * removes windows from desktop, note: due to SFGUI limitations,
     * it only makes windows invisible instead of actually removing them
     */
    virtual void removeWindows(sfg::Desktop& desktop) = 0;
    //! returns tag of GUIMode
    virtual int getTag() = 0;
    //! returns name of GUIMode
    virtual std::string getName() = 0;
};

#endif //NONAME_MODE_H
