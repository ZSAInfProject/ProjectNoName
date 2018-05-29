#ifndef NONAME_ARCHITECTMODE_H
#define NONAME_ARCHITECTMODE_H

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include "GUIMode.h"
#include "../../state/GameMode.h"

class GUIModeArchitect : public GUIMode {
public:
    GUIModeArchitect();
    bool handleEvent(sf::Event &event) override;
    void addWindows(sfg::Desktop &desktop) override;
    void removeWindows(sfg::Desktop &desktop) override;
    int getTag() override;
    std::string getName() override;

private:
    std::string name = "architect mode";
    int tag = GameMode::gameModesEnum::architectMode;
    sfg::ScrolledWindow::Ptr categoryWindow;
};


#endif //NONAME_ARCHITECTMODE_H
