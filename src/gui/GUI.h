#ifndef NONAME_GUI_H
#define NONAME_GUI_H

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>

class GUI {
public:
    sfg::Label::Ptr label;

    void setup();
    void setupEvents();
    bool handleEvent(sf::Event& event);
    void display(sf::RenderWindow& renderWindow);

private:
    sfg::Desktop desktop;
    sfg::Window::Ptr window;
    sfg::SFGUI sfgui;
    sf::Event event;

    //singleton stuff
public:
    //! Returns reference to singleton object.
    static GUI& get() {
        static GUI instance;
        return instance;
    }

    GUI(GUI const&) = delete;
    void operator= (GUI const&) = delete;

private:
    GUI() = default;
};


#endif //NONAME_GUI_H
