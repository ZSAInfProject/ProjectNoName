#include "GUIModeManagement.h"

GUIModeManagement::GUIModeManagement() {
    placeHolder = sfg::Window::Create();
    placeHolder->SetPosition(sf::Vector2f(520, 330));
    auto label = sfg::Label::Create(name + " placeholder");
    placeHolder->Add(label);
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

