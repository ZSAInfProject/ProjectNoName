#ifndef NONAME_MINERMODE_H
#define NONAME_MINERMODE_H

#include "GUIMode.h"
#include "../../state/GameMode.h"

class GUIModeMiner : public GUIMode {
public:
    GUIModeMiner();
    bool handleEvent(sf::Event &event) override;
    void addWindows(sfg::Desktop &desktop) override;
    void removeWindows(sfg::Desktop &desktop) override;
    int getTag() override;
private:
    std::string name = "miner mode";
    int tag = GameMode::gameModesEnum::minerMode;

};


#endif //NONAME_MINERMODE_H