#ifndef NONAME_GUIMODEMINER_H
#define NONAME_GUIMODEMINER_H

#include "GUIMode.h"
#include "../../state/GameMode.h"

class GUIModeMiner : public GUIMode {
public:
    GUIModeMiner();
    bool handleEvent(sf::Event &event) override;
    void addWindows(sfg::Desktop& desktop) override;
    void removeWindows(sfg::Desktop& desktop) override;
    int getTag() override;
    std::string getName() override;
    void rescale(float newScale) override;
    void resize(int newWindowWidth, int newWindowHeight) override;
private:
    std::string name = "miner mode";
    int tag = GameMode::gameModesEnum::minerMode;

    sfg::Window::Ptr placeHolder;
};

#endif //NONAME_GUIMODEMINER_H
