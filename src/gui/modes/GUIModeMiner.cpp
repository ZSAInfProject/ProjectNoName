#include "GUIModeMiner.h"

GUIModeMiner::GUIModeMiner() {
    placeHolder = sfg::Window::Create();
    placeHolder->SetPosition(sf::Vector2f(540, 330));
    auto label = sfg::Label::Create(name + " placeholder");
    placeHolder->Add(label);
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

