#ifndef NONAME_GUIMODESWITCHER_H
#define NONAME_GUIMODESWITCHER_H

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <vector>
#include "GUIMode.h"
#include "../../state/GameMode.h"
#include "../GUIAllocation.h"

class GUIModeSwitcher : public GUIMode {
public:
    //! @param modeName Name of active mode
    GUIModeSwitcher(std::string modeName, float scale);
    //! Makes changes to its windows when changing modes, do not call without changing gui mode
    void changeMode(int newMode) const;
    bool handleEvent(sf::Event &event) override;
    void addWindows(sfg::Desktop& desktop) override;
    void removeWindows(sfg::Desktop& desktop) override;
    int getTag() override;
    std::string getName() override;

private:
    sfg::Window::Ptr switchWindow;
    std::string name = "mode switcher";

    //! this value does not correspond to any actual game mode as it is not a game mode
    int tag = -999;

    std::unique_ptr<ModeSwitcherAllocation> alloc;

    //! Extracted function from ctor
    void eventSetup(sfg::Label::Ptr &labelActiveMode);
};

#endif //NONAME_GUIMODESWITCHER_H