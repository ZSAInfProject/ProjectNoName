#include "GUIModeArchitect.h"
#include "../../state/ArchitectMode.h"
#include "../../state/GameState.h"
#include "../../tile/TileDatabase.h"
#include "../../Game.h"


GUIModeArchitect::GUIModeArchitect(float scale) {
    GUIModeArchitect::scale = scale;
    windowWidth = Settings::get().get<unsigned int>("resolution_x");
    windowHeight = Settings::get().get<unsigned int>("resolution_y");
    categoryNames = TileDatabase::get().getCategoryNames();
    create(true);
}

bool GUIModeArchitect::handleEvent(sf::Event &event) {
    if (categorySelectorWindow->GetAllocation().contains(event.mouseButton.x, event.mouseButton.y))
        return true;
    for (sfg::ScrolledWindow::Ptr w : categoryWindows)
        if (w->GetAllocation().contains(event.mouseButton.x, event.mouseButton.y))
            return true;
    return false;
}

void GUIModeArchitect::addWindows(sfg::Desktop &desktop) {
    desktop.Add(categorySelectorWindow);
    for (sfg::ScrolledWindow::Ptr w : categoryWindows)
        desktop.Add(w);
    for (sfg::Window::Ptr w : blockTooltips)
        desktop.Add(w);
    categorySelectorWindow->Show(true);
    categoryWindows.at(lastCategory)->Show(true);
    blockTooltips.at(lastTooltip)->Show(true);
    desktop.Refresh();
}

void GUIModeArchitect::removeWindows(sfg::Desktop &desktop) {
    categorySelectorWindow->Show(false);
    categoryWindows.at(lastCategory)->Show(false);
    blockTooltips.at(lastTooltip)->Show(false);
}

int GUIModeArchitect::getTag() {
    return tag;
}

std::string GUIModeArchitect::getName() {
    return name;
}

void GUIModeArchitect::rescale(float newScale) {
    scale = newScale;
    create(true);
}

void GUIModeArchitect::resize(int newWindowWidth, int newWindowHeight) {
    windowHeight = newWindowHeight;
    windowWidth = newWindowWidth;
    create(false);
}

void GUIModeArchitect::createCategorySelectorWindow() {
    if(!categorySelectorWindow)
        categorySelectorWindow = sfg::ScrolledWindow::Create();
    else
        categorySelectorWindow->RemoveAll();
    categorySelectorWindow->SetId("categorySelectorWindow");
    categorySelectorWindow->SetScrollbarPolicy(
            sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_NEVER);
    categorySelectorWindow->SetRequisition(
            sf::Vector2f(alloc->categorySelectorWindowWidth, alloc->categorySelectorWindowHeight));
    categorySelectorWindow->SetAllocation(
            sf::FloatRect(sf::Vector2f(alloc->categorySelectorWindowPositionX, alloc->categorySelectorWindowPositionY),
                          sf::Vector2f(alloc->categorySelectorWindowWidth, alloc->categorySelectorWindowHeight)));

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, alloc->boxSpacing);
    box->SetId("categorySelectorBox");

    box->SetRequisition(sf::Vector2f(alloc->categorySelectorWindowWidth, alloc->categorySelectorWindowHeight));
    for (uint i = 0; i < categoryNames->size(); i++) {
        auto button = sfg::Button::Create(refactorString(categoryNames->at(i), 10));
        if (button->GetLabel().getSize())
            button->SetId("category" + i);
        button->SetRequisition(sf::Vector2f(alloc->buttonSize, alloc->buttonSize));
        button->GetSignal(sfg::Widget::OnLeftClick).Connect([this, i] { chooseCategory(i); });
        box->Pack(button);
    }

    categorySelectorWindow->AddWithViewport(box);
    categorySelectorWindow->GetViewport()->SetRequisition(
            sf::Vector2f(alloc->categorySelectorWindowWidth, alloc->categorySelectorWindowHeight));
    categorySelectorWindow->GetViewport()->GetVerticalAdjustment()->SetLower(0.f);
    categorySelectorWindow->GetViewport()->GetVerticalAdjustment()->SetUpper(10000.f);
}

void GUIModeArchitect::createCategoryWindow() {
    categoryWindows.clear();
    for (uint i = 0; i < categoryNames->size(); i++) {
        categoryWindows.push_back(sfg::ScrolledWindow::Create());
        categoryWindows.at(i)->SetId("categoryWindow" + i);
        categoryWindows.at(i)->SetScrollbarPolicy(
                sfg::ScrolledWindow::VERTICAL_NEVER | sfg::ScrolledWindow::HORIZONTAL_ALWAYS);
        categoryWindows.at(i)->SetRequisition(sf::Vector2f(alloc->categoryWindowsWidth, alloc->categoryWindowsHeight));
        categoryWindows.at(i)->SetAllocation(
                sf::FloatRect(sf::Vector2f(alloc->categoryWindowsPositionX, alloc->categoryWindowsPositionY),
                              sf::Vector2f(alloc->categoryWindowsWidth, alloc->categoryWindowsHeight)));
    }
}

void GUIModeArchitect::createBlockButtons() {
    std::vector<sfg::Box::Ptr> blocks;
    for (uint i = 0; i < categoryNames->size(); i++) {
        auto box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, alloc->boxSpacing);
        box->SetAllocation(sf::FloatRect(sf::Vector2f(0, 0),
                                         sf::Vector2f(alloc->categoryWindowsWidth, alloc->categoryWindowsHeight)));
        box->SetId("blocks" + i);
        blocks.push_back(box);

        categoryWindows.at(i)->AddWithViewport(blocks.at(i));
        categoryWindows.at(i)->GetViewport()->SetRequisition(
                sf::Vector2f(alloc->categoryWindowsWidth, alloc->categoryWindowsHeight));
        categoryWindows.at(i)->GetViewport()->GetVerticalAdjustment()->SetLower(0.f);
        categoryWindows.at(i)->GetViewport()->GetVerticalAdjustment()->SetUpper(100000000.f);
        categoryWindows.at(i)->Show(false);
    }

    for (int i = 0; i < TileDatabase::get().size(); i++) {
        auto button = sfg::Button::Create(refactorString(TileDatabase::get()[i].name, 10));
        button->SetRequisition(sf::Vector2f(alloc->buttonSize, alloc->buttonSize));
        createBlockButtonsEvents(i, button);
        blocks.at(TileDatabase::get()[i].category)->Pack(button);
    }
}

void GUIModeArchitect::createBlockButtonsEvents(int id, sfg::Widget::Ptr button) {
    button->GetSignal(sfg::Widget::OnLeftClick).Connect([this, id] { chooseBlock(id); });
    button->GetSignal(sfg::Widget::OnMouseEnter).Connect([this, id] { showFloatingTooltip(true, id); });
    button->GetSignal(sfg::Widget::OnMouseLeave).Connect([this, id] { showFloatingTooltip(false, id); });
    button->GetSignal(sfg::Widget::OnMouseMove).Connect([this, id] { moveFloatingTooltip(id); });
}

void GUIModeArchitect::createBlockTooltips() {
    blockTooltips.clear();
    for (int i = 0; i < TileDatabase::get().size(); i++) {
        blockTooltips.push_back(sfg::Window::Create(0b00010));
        blockTooltips.at(i)->SetPosition(sf::Vector2f(alloc->tooltipPositionX, alloc->tooltipPositionY));
        blockTooltips.at(i)->SetRequisition(sf::Vector2f(alloc->tooltipWidth, alloc->tooltipHeight));

        auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
        box->SetId("BlockTooltip" + i);
        auto labelCategory = sfg::Label::Create(categoryNames->at(TileDatabase::get()[i].category));
        auto labelSolid = sfg::Label::Create((TileDatabase::get()[i].isSolid) ? "solid" : "not solid");
        auto labelName = sfg::Label::Create(TileDatabase::get()[i].name);
        char *hardnessText = new char[16];
        sprintf(hardnessText, "hardness: %4.1f", TileDatabase::get()[i].hardness);
        auto labelHardness = sfg::Label::Create(hardnessText);

        box->Pack(labelName);
        box->Pack(generateImage(i));
        box->Pack(labelCategory);
        box->Pack(labelSolid);
        box->Pack(labelHardness);

        blockTooltips.at(i)->Add(box);
        blockTooltips.at(i)->Show(false);
    }
}

sfg::Image::Ptr GUIModeArchitect::generateImage(int id) const {
    sf::Texture texture;
    texture.loadFromFile("res/texture.png", sf::IntRect(TileDatabase::get()[id].texture_x * Chunk::TILE_SIZE,
                                                    TileDatabase::get()[id].texture_y * Chunk::TILE_SIZE,
                                                    Chunk::TILE_SIZE, Chunk::TILE_SIZE));
    sf::Sprite sprite;
    sprite.setTexture(texture, true);
    sprite.setScale(alloc->blockScale, alloc->blockScale);
    sf::RenderTexture render;
    render.clear();
    render.create(alloc->buttonSize, alloc->buttonSize);
    render.draw(sprite);
    render.display();
    return sfg::Image::Create((render.getTexture()).copyToImage());
}

void GUIModeArchitect::chooseCategory(int id) {
    categoryWindows.at(lastCategory)->Show(false);
    categoryWindows.at(id)->Show(true);
    lastCategory = id;
}

void GUIModeArchitect::chooseBlock(int id) {
    blockTooltips.at(id)->SetPosition(sf::Vector2f(alloc->tooltipPositionX, alloc->tooltipPositionY));
    blockTooltips.at(lastTooltip)->Show(false);
    blockTooltips.at(id)->Show(true);
    lastTooltip = id;
    std::dynamic_pointer_cast<ArchitectMode>(dynamic_cast<GameState *>
                                             (&Game::get().getState())->getGameMode())->setBlock(id);
}

void GUIModeArchitect::showFloatingTooltip(bool show, int id) {
    if (id != lastTooltip) {
        blockTooltips.at(id)->SetHierarchyLevel(19865467);
        categoryWindows.at(lastCategory)->SetHierarchyLevel(0);
        blockTooltips.at(id)->Show(show);
    }
}

void GUIModeArchitect::moveFloatingTooltip(int id) {
    if (id != lastTooltip)
        blockTooltips.at(id)->SetPosition(sf::Vector2f(
                dynamic_cast<GameState *>(&Game::get().getState())->getGUI()->getEvent()->mouseMove.x + 5,
                dynamic_cast<GameState *>(&Game::get().getState())->getGUI()->getEvent()->mouseMove.y -
                blockTooltips.at(id)->GetAllocation().height - 5));
}

std::string GUIModeArchitect::refactorString(std::string str, int lineSize) {
    int lineLength = 0;
    int wordLength = 0;
    for (uint i = 0; i < str.length(); i++) {
        if (str[i] != ' ' || str[i] != '\n')
            wordLength++;
        else
            wordLength = 0;
        if (++lineLength > lineSize) {
            if (str[i] == ' ')
                str.replace(i, 1, "\n");
            else if (wordLength == 1)
                str.replace(i - 1, 1, "\n");
            else
                str.insert(i - 1, "-\n");
            wordLength = 0;
            lineLength = 0;
        }
    }
    return str;
}

void GUIModeArchitect::create(bool rescale) {
    alloc = std::make_unique<ArchitectModeAllocation>(ArchitectModeAllocation(scale, windowWidth, windowHeight));
    if(!rescale) {
        categorySelectorWindow->SetPosition(
                sf::Vector2f(alloc->categorySelectorWindowPositionX, alloc->categorySelectorWindowPositionY));
        for (sfg::ScrolledWindow::Ptr w : categoryWindows)
            w->SetPosition(sf::Vector2f(alloc->categoryWindowsPositionX, alloc->categoryWindowsPositionY));
        for (sfg::Window::Ptr w : blockTooltips)
            w->SetPosition(sf::Vector2f(alloc->tooltipPositionX, alloc->tooltipPositionY));
    } else {
        createCategorySelectorWindow();
        createCategoryWindow();
        createBlockButtons();
        createBlockTooltips();
    }
}

