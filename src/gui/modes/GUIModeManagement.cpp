#include "GUIModeManagement.h"
#include "../../Game.h"
#include "../../state/GameState.h"

GUIModeManagement::GUIModeManagement() {
    placeHolder = sfg::Window::Create();
    placeHolder->SetPosition(sf::Vector2f(520, 330));
    auto button = sfg::Button::Create("rescale to x2");
    button->GetSignal(sfg::Widget::OnLeftClick).Connect([] {
        dynamic_cast<GameState *>(&Game::get().getState())->getGUI()->rescale(2.f);
    });
    placeHolder->Add(button);
}

bool GUIModeManagement::handleEvent(sf::Event &event) {
    if(placeHolder->GetAllocation().contains(event.mouseButton.x, event.mouseButton.y))
        return true;
    return false;
}

void GUIModeManagement::addWindows(sfg::Desktop& desktop) {
    desktop.Add(placeHolder);
    placeHolder->Show(true);
}

void GUIModeManagement::removeWindows(sfg::Desktop& desktop) {
    placeHolder->Show(false);
}

int GUIModeManagement::getTag() {
    return tag;
}

std::string GUIModeManagement::getName() {
    return name;
}

void GUIModeManagement::rescale(float newScale) {

}

void GUIModeManagement::resize(int newWindowWidth, int newWindowHeight) {

}

