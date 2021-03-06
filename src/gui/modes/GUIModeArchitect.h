#ifndef NONAME_GUIMODEARCHITECT_H
#define NONAME_GUIMODEARCHITECT_H

#include "GUIMode.h"
#include "../../state/GameMode.h"
#include "../../world/chunk/Chunk.h"
#include "../GUIAllocation.h"

class GUIModeArchitect : public GUIMode {
public:
    GUIModeArchitect(float scale);
    bool handleEvent(sf::Event &event) override;
    void addWindows(sfg::Desktop &desktop) override;
    void removeWindows(sfg::Desktop &desktop) override;
    int getTag() override;
    std::string getName() override;
    void rescale(float newScale) override;
    void resize(int newWindowWidth, int newWindowHeight) override;

private:
    std::string name = "architect mode";
    std::shared_ptr<std::vector<std::string>> categoryNames;
    int tag = GameMode::gameModesEnum::architectMode;
    int lastCategory = 0;
    int lastTooltip = 0;
    sfg::ScrolledWindow::Ptr categorySelectorWindow;
    std::vector<sfg::ScrolledWindow::Ptr> categoryWindows;
    std::vector<sfg::Window::Ptr> blockTooltips;

    std::unique_ptr<ArchitectModeAllocation> alloc;
    float scale;
    int windowHeight;
    int windowWidth;


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

    /*!
     * Refactors string to have no more than LineSize charaters in each line
     * @param str String to refactor
     * @param lineSize Size of a line
     * @return Returns refactored string
     */
    std::string refactorString(std::string str, int lineSize);

    //! Rescales and Resizes GUImode @param rescale True value creates/recreate GUI for this mode
    void create(bool rescale);
};

#endif //NONAME_GUIMODEARCHITECT_H
