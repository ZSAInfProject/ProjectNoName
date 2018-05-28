#include "GUIModeSwitcher.h"
#include "../../Game.h"
#include "../../state/GameState.h"

GUIModeSwitcher::GUIModeSwitcher() {
    switchWindow = sfg::Window::Create(0b00010);
    auto container = sfg::Fixed::Create();
    container->SetId("container");
    containerSetup(container);
    switchWindow->Add(container);
    switchWindow->SetRequisition(sf::Vector2f(150, 40));
    switchWindow->SetAllocation(sf::FloatRect(sf::Vector2f(350, 25), sf::Vector2f(150, 40)));
}

void GUIModeSwitcher::containerSetup(sfg::Fixed::Ptr &container) const {
    auto buttonMinerMode = sfg::Button::Create("G");
    buttonMinerMode->SetId("minerMode");
    buttonMinerMode->SetRequisition(sf::Vector2f(20, 24));
    auto buttonArchitectMode = sfg::Button::Create("A");
    buttonArchitectMode->SetId("architectMode");
    buttonArchitectMode->SetRequisition(sf::Vector2f(20, 24));
    auto buttonManagementMode = sfg::Button::Create("Z");
    buttonManagementMode->SetId("managementMode");
    buttonManagementMode->SetRequisition(sf::Vector2f(20, 24));
    auto labelActiveMode = sfg::Label::Create("architect mode"); //TODO fetch name from GameState
        labelActiveMode->SetId("activeMode");
    //don't change allocation values
    container->Put(buttonMinerMode, sf::Vector2f(24, 20));
    container->Put(buttonArchitectMode, sf::Vector2f(54, 20));
    container->Put(buttonManagementMode, sf::Vector2f(84, 20));
    container->Put(labelActiveMode, sf::Vector2f(0, 0));
    container->GetWidgetById("activeMode")->SetAllocation(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(125, 10)));

    eventSetup(labelActiveMode);
}

void GUIModeSwitcher::eventSetup(sfg::Label::Ptr &labelActiveMode) const {
    switchWindow->GetWidgetById("minerMode")->
            GetSignal(sfg::Widget::OnLeftClick).Connect([this]{
        dynamic_cast<GameState*>(&Game::get().getState())->setGameMode(GameMode::minerMode);
    });
    switchWindow->GetWidgetById("architectMode")->
            GetSignal(sfg::Widget::OnLeftClick).Connect([this]{
        dynamic_cast<GameState*>(&Game::get().getState())->setGameMode(GameMode::architectMode);
    });
    switchWindow->GetWidgetById("managementMode")->
            GetSignal(sfg::Widget::OnLeftClick).Connect([this]{
        dynamic_cast<GameState*>(&Game::get().getState())->setGameMode(GameMode::managementMode);
    });
}

bool GUIModeSwitcher::handleEvent(sf::Event &event) {
    if(switchWindow->GetChildren().size() > 0)
        for(sfg::Widget::Ptr w : switchWindow->GetChildren())
            if(w->GetAllocation().contains(event.mouseButton.x - w->GetAbsolutePosition().x,
                                           event.mouseButton.y - w->GetAbsolutePosition().y))
                return true;
    return false;
}

void GUIModeSwitcher::addWindows(sfg::Desktop &desktop) {
    desktop.Add(switchWindow);
}

void GUIModeSwitcher::removeWindows(sfg::Desktop &desktop) {
    removeWindows(desktop);
}

void GUIModeSwitcher::changeMode(int newMode) const {
    //TODO rewrite function so it uses gameState::GameMode instead of int

    dynamic_cast<GameState*>(&Game::get().getState())->setGameMode(newMode);
    dynamic_cast<sfg::Label*>(switchWindow->GetWidgetById("activeMode").get())->SetText("not working now");
    Game::get().getGUI().changeMode(newMode);
}

int GUIModeSwitcher::getTag() {
    return tag;
}