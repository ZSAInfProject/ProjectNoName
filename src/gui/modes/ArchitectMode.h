#ifndef NONAME_ARCHITECTMODE_H
#define NONAME_ARCHITECTMODE_H

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include "GUIMode.h"

class ArchitectMode : public GUIMode {
public:
    ArchitectMode();
    bool handleEvent(sf::Event &event);
    void addWindows(sfg::Desktop &desktop);
    void removeWidows(sfg::Desktop &desktop);

private:
    sfg::ScrolledWindow::Ptr categoryWindow;
};


#endif //NONAME_ARCHITECTMODE_H
