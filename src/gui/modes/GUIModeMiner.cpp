#include "GUIModeMiner.h"
#include "../../state/GameState.h"

GUIModeMiner::GUIModeMiner() {
    placeHolder = sfg::Window::Create();
    placeHolder->SetPosition(sf::Vector2f(520, 330));
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
    box->Pack(sfg::Label::Create("management mode placeholder"));
    auto button = sfg::Button::Create("rescale to x1");
    button->GetSignal(sfg::Widget::OnLeftClick).Connect([] {
        dynamic_cast<GameState *>(&Game::get().getState())->getGUI()->rescale(1.f);
    });
    box->Pack(button);
    placeHolder->Add(box);
}

bool GUIModeMiner::handleEvent(sf::Event &event) {
    if(placeHolder->GetAllocation().contains(event.mouseButton.x, event.mouseButton.y))
        return true;
    return false;
}

void GUIModeMiner::addWindows(sfg::Desktop& desktop) {
    desktop.Add(placeHolder);
    placeHolder->Show(true);
}

void GUIModeMiner::removeWindows(sfg::Desktop& desktop) {
    placeHolder->Show(false);
}

int GUIModeMiner::getTag() {
    return tag;
}

std::string GUIModeMiner::getName() {
    return name;
}

void GUIModeMiner::rescale(float newScale) {

}

void GUIModeMiner::resize(int newWindowWidth, int newWindowHeight) {

}

