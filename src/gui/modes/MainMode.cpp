#include "MainMode.h"
#include "../../Game.h"
#include "../../state/GameState.h"

MainMode::MainMode() {
    switchWindow = sfg::Window::Create(0b00010);
    auto container = sfg::Fixed::Create();
    container->SetId("container");
    containerSetup(container);
    switchWindow->Add(container);
    switchWindow->SetRequisition(sf::Vector2f(150, 40));
    switchWindow->SetAllocation(sf::FloatRect(sf::Vector2f(350, 25), sf::Vector2f(150, 40)));
}

void MainMode::containerSetup(sfg::Fixed::Ptr &container) const {
    auto buttonMinerMode = sfg::Button::Create("G");
    buttonMinerMode->SetId(modes.at(modesEnum::minerMode));
    buttonMinerMode->SetRequisition(sf::Vector2f(20, 24));
    auto buttonArchitectMode = sfg::Button::Create("A");
    buttonArchitectMode->SetId(modes.at(modesEnum::architectMode));
    buttonArchitectMode->SetRequisition(sf::Vector2f(20, 24));
    auto buttonManagementMode = sfg::Button::Create("Z");
    buttonManagementMode->SetId(modes.at(modesEnum::managementMode));
    buttonManagementMode->SetRequisition(sf::Vector2f(20, 24));
    auto labelActiveMode = sfg::Label::Create("architect mode");
        labelActiveMode->SetId("activeMode");
    //don't change allocation values
    container->Put(buttonMinerMode, sf::Vector2f(24, 20));
    container->Put(buttonArchitectMode, sf::Vector2f(54, 20));
    container->Put(buttonManagementMode, sf::Vector2f(84, 20));
    container->Put(labelActiveMode, sf::Vector2f(0, 0));
    container->GetWidgetById("activeMode")->SetAllocation(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(125, 10)));

    eventSetup(labelActiveMode);
}

void MainMode::eventSetup(sfg::Label::Ptr &labelActiveMode) const {
    switchWindow->GetWidgetById(modes.at(modesEnum::minerMode))->
            GetSignal(sfg::Widget::OnLeftClick).Connect([this]{
        changeMode(modesEnum::minerMode);
    });
    switchWindow->GetWidgetById(modes.at(modesEnum::architectMode))->
            GetSignal(sfg::Widget::OnLeftClick).Connect([this]{
        changeMode(modesEnum::architectMode);
    });
    switchWindow->GetWidgetById(modes.at(modesEnum::managementMode))->
            GetSignal(sfg::Widget::OnLeftClick).Connect([this]{
        changeMode(modesEnum::managementMode);
    });
}

bool MainMode::handleEvent(sf::Event &event) {
    if(switchWindow->GetChildren().size() > 0)
        for(sfg::Widget::Ptr w : switchWindow->GetChildren())
            if(w->GetAllocation().contains(event.mouseButton.x - w->GetAbsolutePosition().x,
                                           event.mouseButton.y - w->GetAbsolutePosition().y))
                return true;
    return false;
}

void MainMode::addWindows(sfg::Desktop &desktop) {
    desktop.Add(switchWindow);
}

void MainMode::removeWidows(sfg::Desktop &desktop) {
    removeWidows(desktop);
}

void MainMode::changeMode(int newMode) const {
    if(newMode == dynamic_cast<GameState*>(&Game::get().getState())->getGameMode())
        return;
    dynamic_cast<GameState*>(&Game::get().getState())->setGameMode(newMode);
    dynamic_cast<sfg::Label*>(switchWindow->GetWidgetById("activeMode").get())->SetText(modes.at(newMode + 3));
    Game::get().getGUI().changeMode(newMode);
}


