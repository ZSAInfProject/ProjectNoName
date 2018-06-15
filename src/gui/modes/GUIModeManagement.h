#ifndef NONAME_GUIMODEMANAGEMENT_H
#define NONAME_GUIMODEMANAGEMENT_H

#include "GUIMode.h"
#include "../../state/GameMode.h"

class GUIModeManagement : public GUIMode {
public:
    GUIModeManagement();
    bool handleEvent(sf::Event &event) override;
    void addWindows(sfg::Desktop& desktop) override;
    void removeWindows(sfg::Desktop& desktop) override;
    int getTag() override;
    std::string getName() override;
    void rescale(float newScale) override;
    void resize(int newWindowWidth, int newWindowHeight) override;
private:
    std::string name = "management mode";
    int tag = GameMode::gameModesEnum ::managementMode;

    sfg::Window::Ptr placeHolder;
};

#endif //NONAME_GUIMODEMANAGEMENT_H
