#include "GUIModeSwitcher.h"
#include "../../Game.h"
#include "../../state/GameState.h"

GUIModeSwitcher::GUIModeSwitcher(std::string modeName, float scale) {
    GUIModeSwitcher::scale = scale;
    windowWidth = Settings::get().get<unsigned int>("resolution_x");
    windowHeight = Settings::get().get<unsigned int>("resolution_y");
    activeModeName = modeName;
    create(true);
}

void GUIModeSwitcher::changeMode(int newMode) {
    activeModeName = dynamic_cast<GameState*>(&Game::getState())->getGUI()->getGUIMode(newMode)->getName();
    dynamic_cast<sfg::Label*>(switchWindow->GetWidgetById("activeMode").get())->SetText(activeModeName);
}

bool GUIModeSwitcher::handleEvent(sf::Event &event) {
    if (switchWindow->GetChildren().size() > 0)
        for (sfg::Widget::Ptr w : switchWindow->GetChildren())
            if (w->GetAllocation().contains(event.mouseButton.x - w->GetAbsolutePosition().x,
                                            event.mouseButton.y - w->GetAbsolutePosition().y))
                return true;
    return false;
}

void GUIModeSwitcher::addWindows(sfg::Desktop &desktop) {
    desktop.Add(switchWindow);
}

void GUIModeSwitcher::removeWindows(sfg::Desktop &desktop) {
    switchWindow->Show(false);
}

int GUIModeSwitcher::getTag() {
    return tag;
}

std::string GUIModeSwitcher::getName() {
    return name;
}

void GUIModeSwitcher::rescale(float newScale) {
    scale = newScale;
    create(true);
}

void GUIModeSwitcher::resize(int newWindowWidth, int newWindowHeight) {
    windowHeight = newWindowHeight;
    windowWidth = newWindowWidth;
    create(false);
}

void GUIModeSwitcher::createSwitchWindow() {
    if(!switchWindow)
        switchWindow = sfg::Window::Create(0b00010);
    else
        switchWindow->RemoveAll();
    switchWindow->SetRequisition(sf::Vector2f(alloc->switchWindowWidth, alloc->switchWindowHeight));
    switchWindow->SetAllocation(sf::FloatRect(sf::Vector2f(alloc->switchWindowPositionX, alloc->switchWindowPositionY),
                                              sf::Vector2f(alloc->switchWindowWidth, alloc->switchWindowHeight)));
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, alloc->boxSpacing);
    auto switchers = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, alloc->boxSpacing);
    auto labelActiveMode = sfg::Label::Create(activeModeName);
    labelActiveMode->SetId("activeMode");
    auto buttonMinerMode = sfg::Button::Create("G");
    buttonMinerMode->SetId("minerMode");
    buttonMinerMode->SetRequisition(sf::Vector2f(alloc->buttonWidth, alloc->buttonHeight));
    auto buttonArchitectMode = sfg::Button::Create("A");
    buttonArchitectMode->SetId("architectMode");
    buttonArchitectMode->SetRequisition(sf::Vector2f(alloc->buttonWidth, alloc->buttonHeight));
    auto* test = new char[50];
    sprintf(test, "%i, %i", switchWindow->GetAllocation().height, alloc->switchWindowHeight);
    Log::info("BTN", test);
    auto buttonManagementMode = sfg::Button::Create("Z");
    buttonManagementMode->SetId("managementMode");
    buttonManagementMode->SetRequisition(sf::Vector2f(alloc->buttonWidth, alloc->buttonHeight));
    switchers->Pack(buttonMinerMode);
    switchers->Pack(buttonArchitectMode);
    switchers->Pack(buttonManagementMode);
    box->Pack(labelActiveMode);
    box->Pack(switchers);
    switchWindow->Add(box);
    switchWindow->RequestResize();

    eventSetup(labelActiveMode);
}

void GUIModeSwitcher::create(bool rescale) {
    alloc = std::make_unique<ModeSwitcherAllocation>(ModeSwitcherAllocation(scale, windowWidth, windowHeight));
    auto* test = new char[50];
    sprintf(test, "%f, %i, %i", scale, alloc->buttonWidth, alloc->buttonHeight);
    Log::info("ALLOC", test);
    if(!rescale)
        switchWindow->SetPosition(sf::Vector2f(alloc->switchWindowPositionX, alloc->switchWindowPositionY));
    else
        createSwitchWindow();
}

void GUIModeSwitcher::eventSetup(sfg::Label::Ptr &labelActiveMode) {
    switchWindow->GetWidgetById("minerMode")->
            GetSignal(sfg::Widget::OnLeftClick).Connect([this] {
        dynamic_cast<GameState *>(&Game::get().getState())->setGameMode(GameMode::gameModesEnum::minerMode);
    });
    switchWindow->GetWidgetById("architectMode")->
            GetSignal(sfg::Widget::OnLeftClick).Connect([this] {
        dynamic_cast<GameState *>(&Game::get().getState())->setGameMode(GameMode::architectMode);
    });
    switchWindow->GetWidgetById("managementMode")->
            GetSignal(sfg::Widget::OnLeftClick).Connect([this] {
        dynamic_cast<GameState *>(&Game::get().getState())->setGameMode(GameMode::managementMode);
    });
}
