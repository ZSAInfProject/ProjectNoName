#ifndef NONAME_MANAGEMENTMODE_H
#define NONAME_MANAGEMENTMODE_H

#include "GUIMode.h"
#include "../../state/GameMode.h"

class GUIModeManagement : public GUIMode {
public:
    GUIModeManagement();
    bool handleEvent(sf::Event &event) override;
    void addWindows(sfg::Desktop &desktop) override;
    void removeWindows(sfg::Desktop &desktop) override;
    int getTag() override;
    std::string getName() override;
private:
    std::string name = "management mode";
    int tag = GameMode::gameModesEnum ::managementMode;
};


#endif //NONAME_MANAGEMENTMODE_H
