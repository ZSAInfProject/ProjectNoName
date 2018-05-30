#include "GUIModeArchitect.h"

GUIModeArchitect::GUIModeArchitect() {
    categoryWindow = sfg::ScrolledWindow::Create();
    categoryWindow->SetId("categoryWindow");
    categoryWindow->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_NEVER);
    categoryWindow->SetAllocation(sf::FloatRect(sf::Vector2f(15, 400), sf::Vector2f(60, 150)));

    auto container = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
    container->SetAllocation(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(80, 300)));
    container->SetId("container");
    auto buttonMinerMode = sfg::Button::Create("C1");
    buttonMinerMode->SetId("C1");
    auto buttonArchitectMode = sfg::Button::Create("C2");
    buttonArchitectMode->SetId("C2");
    auto buttonManagementMode = sfg::Button::Create("C3");
    buttonManagementMode->SetId("C3");

    container->Pack(buttonMinerMode, true, true);
    container->Pack(buttonArchitectMode, true, true);
    container->Pack(buttonManagementMode, true, true);
    container->GetWidgetById("C1")->SetRequisition(sf::Vector2f(80, 80));
    container->GetWidgetById("C2")->SetRequisition(sf::Vector2f(80, 80));
    container->GetWidgetById("C3")->SetRequisition(sf::Vector2f(80, 80));

    categoryWindow->AddWithViewport(container);
    categoryWindow->SetRequisition(sf::Vector2f(60, 300));
    categoryWindow->GetViewport()->SetRequisition(sf::Vector2f(60, 300));
    categoryWindow->GetViewport()->GetVerticalAdjustment()->SetLower( 0.f );
    categoryWindow->GetViewport()->GetVerticalAdjustment()->SetUpper( 100000000.f );
}

bool GUIModeArchitect::handleEvent(sf::Event &event) {
    if(categoryWindow->GetAllocation().contains(event.mouseButton.x, event.mouseButton.y))
        return true;
    return false;
}

void GUIModeArchitect::addWindows(sfg::Desktop& desktop) {
    desktop.Add(categoryWindow);
    categoryWindow->Show(true);
}

void GUIModeArchitect::removeWindows(sfg::Desktop& desktop) {
    categoryWindow->Show(false);
}

int GUIModeArchitect::getTag() {
    return tag;
}

std::string GUIModeArchitect::getName() {
    return name;
}
