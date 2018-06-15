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
    //! Returns whether mouse click occured inside of any active window
    virtual bool handleEvent(sf::Event &event) = 0;
    //! Adds windows to desktop
    virtual void addWindows(sfg::Desktop& desktop) = 0;
    /*!
     * Removes windows from desktop, note: due to SFGUI limitations,
     * it only makes windows invisible instead of actually removing them
     */
    virtual void removeWindows(sfg::Desktop& desktop) = 0;
    //! Returns tag of GUIMode
    virtual int getTag() = 0;
    //! Returns name of GUIMode
    virtual std::string getName() = 0;
    //! Rescales GUIMode and relocates accordingly
    virtual void rescale(float newScale) = 0;
    //! Relocates GUIMode according to new window dimensions
    virtual void resize(int newWindowWidth, int newWindowHeight) = 0;
};

#endif //NONAME_MODE_H
