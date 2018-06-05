#ifndef NONAME_GUIMODEARCHITECT_H
#define NONAME_GUIMODEARCHITECT_H

#include "GUIMode.h"
#include "../../state/GameMode.h"
#include "../../world/chunk/Chunk.h"
#include "GUIAllocation.h"

class GUIModeArchitect : public GUIMode {
public:
    GUIModeArchitect();
    bool handleEvent(sf::Event &event) override;
    void addWindows(sfg::Desktop& desktop) override;
    void removeWindows(sfg::Desktop& desktop) override;
    int getTag() override;
    std::string getName() override;

private:
    std::string name = "architect mode";
    std::shared_ptr<std::vector<std::string>> categoryNames;
    int tag = GameMode::gameModesEnum::architectMode;
    int lastCategory = 0;
    int lastTooltip = 0;
    sfg::ScrolledWindow::Ptr categorySelectorWindow;
    std::vector<sfg::ScrolledWindow::Ptr> categoryWindows;
    std::vector<sfg::Window::Ptr> blockTooltips;

    ArchitectModeAllocation alloc = ArchitectModeAllocation(1.5f);

    //! Generates categorySelectorWindow
    void createCategorySelectorWindow();
    //! Generates categoryWindows
    void createCategoryWindow();
    //! Generates blocks
    void createBlockButtons();
    //! Adds events for every block @param id buttons's id
    void createBlockButtonsEvents(int id, sfg::Widget::Ptr button);
    //! Generates tooltip for every block
    void createBlockTooltips();
    //! Functions below are sub-functions for specific events
    //! Changes visibility of categories @param id Category to show
    void chooseCategory(int id);
    //! Changes visibility of tooltips @param id Category to show
    void chooseBlock(int id);
    //! Handles showing tooltips @param id Tooltip to handle
    void showFloatingTooltip(bool show, int id);
    //! Handles movement of tooltip @param id Tooltip to handle
    void moveFloatingTooltip(int id);
    //! Generates images for tooltips @param id Tooltip's id
    sfg::Image::Ptr generateImage(int id) const;
};

#endif //NONAME_GUIMODEARCHITECT_H
