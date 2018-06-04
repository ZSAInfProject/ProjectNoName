#include "GUIModeArchitect.h"
#include "../../state/ArchitectMode.h"
#include "../../state/GameState.h"
#include "../../tile/TileDatabase.h"
#include "../../Game.h"

GUIModeArchitect::GUIModeArchitect() {
    categoryNames = TileDatabase::get().getCategoryNames();
    createCategorySelectorWindow();
    createCategoryWindow();
    createBlockButtons();
    createBlockTooltips();
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

void GUIModeArchitect::createCategorySelectorWindow() {
    categorySelectorWindow = sfg::ScrolledWindow::Create();
    categorySelectorWindow->SetId("categorySelectorWindow");
    categorySelectorWindow->SetScrollbarPolicy(
            sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_NEVER);
    categorySelectorWindow->SetRequisition(sf::Vector2f(60, 300));
    categorySelectorWindow->SetAllocation(sf::FloatRect(sf::Vector2f(15, 404), sf::Vector2f(60, 150)));

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
    box->SetId("container");
    box->SetRequisition(sf::Vector2f(80, 300));
    for (int i = 0; i < categoryNames->size(); i++) {
        auto button = sfg::Button::Create(categoryNames->at(i));
        button->SetId("category" + i);
        button->SetRequisition(sf::Vector2f(80, 80));
        button->GetSignal(sfg::Widget::OnLeftClick).Connect([this, i] { chooseCategory(i); });
        box->Pack(button);
    }

    categorySelectorWindow->AddWithViewport(box);
    categorySelectorWindow->GetViewport()->SetRequisition(sf::Vector2f(60, 300));
    categorySelectorWindow->GetViewport()->GetVerticalAdjustment()->SetLower(0.f);
    categorySelectorWindow->GetViewport()->GetVerticalAdjustment()->SetUpper(10000.f);
}

void GUIModeArchitect::createCategoryWindow() {
    for (int i = 0; i < categoryNames->size(); i++) {
        categoryWindows.push_back(sfg::ScrolledWindow::Create());
        categoryWindows.at(i)->SetId("categorySelectorWindow");
        categoryWindows.at(i)->SetScrollbarPolicy(
                sfg::ScrolledWindow::VERTICAL_NEVER | sfg::ScrolledWindow::HORIZONTAL_ALWAYS);
        categoryWindows.at(i)->SetRequisition(sf::Vector2f(300, 60));
        categoryWindows.at(i)->SetAllocation(sf::FloatRect(sf::Vector2f(138, 599), sf::Vector2f(300, 60)));
    }
}

void GUIModeArchitect::createBlockButtons() {
    std::vector<sfg::Box::Ptr> blocks;
    for (int i = 0; i < categoryNames->size(); i++) {
        auto box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 10.f);
        box->SetAllocation(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(80, 300)));
        box->SetId("blocks" + i);
        blocks.push_back(box);

        categoryWindows.at(i)->AddWithViewport(blocks.at(i));
        categoryWindows.at(i)->GetViewport()->SetRequisition(sf::Vector2f(300, 60));
        categoryWindows.at(i)->GetViewport()->GetVerticalAdjustment()->SetLower(0.f);
        categoryWindows.at(i)->GetViewport()->GetVerticalAdjustment()->SetUpper(100000000.f);
        categoryWindows.at(i)->Show(false);
    }

    for (int i = 0; i < TileDatabase::get().size(); i++) {
        auto button = sfg::Button::Create(TileDatabase::get()[i].name);
        button->SetRequisition(sf::Vector2f(80, 80));
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
    for (int i = 0; i < TileDatabase::get().size(); i++) {
        blockTooltips.push_back(sfg::Window::Create(0b00010));
        blockTooltips.at(i)->SetPosition(sf::Vector2f(1130, 505));
        blockTooltips.at(i)->SetRequisition(sf::Vector2f(125, 200));

        auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
        box->SetId("BlockTooltip" + i);
        auto labelCategory = sfg::Label::Create(categoryNames->at(TileDatabase::get()[i].category));
        auto labelSolid = sfg::Label::Create((TileDatabase::get()[i].isSolid) ? "solid" : "not solid");
        auto labelName = sfg::Label::Create(TileDatabase::get()[i].name);
        char *test = new char[16];
        sprintf(test, "hardness: %5.1f", TileDatabase::get()[i].hardness);
        auto labelHardness = sfg::Label::Create(test);

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
    texture.loadFromFile("texture.png", sf::IntRect(TileDatabase::get()[id].texture_x * 16, TileDatabase::get()[id].texture_y * 16 , 16, 16));
    sf::Sprite sprite;
    sprite.setTexture(texture, true);
    sprite.setScale(5.f, 5.f);
    sf::RenderTexture render;
    render.clear();
    render.create(80, 80);
    render.draw(sprite);
    render.display();
    auto img = sfg::Image::Create((render.getTexture()).copyToImage());
    return img;
}

void GUIModeArchitect::chooseCategory(int id) {
    categoryWindows.at(lastCategory)->Show(false);
    categoryWindows.at(id)->Show(true);
    lastCategory = id;
}

void GUIModeArchitect::chooseBlock(int id) {
    blockTooltips.at(id)->SetPosition(sf::Vector2f(1130, 505));
    blockTooltips.at(lastTooltip)->Show(false);
    blockTooltips.at(id)->Show(true);
    lastTooltip = id;
    std::dynamic_pointer_cast<ArchitectMode>(dynamic_cast<GameState*>
                                             (&Game::get().getState())->getGameMode())->setBlock(id);
}

void GUIModeArchitect::showFloatingTooltip(bool show, int id) {
    if (id != lastTooltip){
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