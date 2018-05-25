//
// Created by arek on 22.05.18.
//

#include "GUI.h"

void GUI::setup(){
    GUI::label = sfg::Label::Create( "Hello world!" );
    auto buttonArc = sfg::Button::Create( "test" );
    buttonArc->SetId("btnArc");
    auto buttonMin = sfg::Button::Create( "Miner\nmode" );
    auto buttonMan = sfg::Button::Create( "Manage\n mode" );

    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f );
    auto fixed = sfg::Fixed::Create();

    GUI::window = sfg::Window::Create();
    GUI::window->SetAllocation(sf::FloatRect(5, 435, 80, 290));
    //GUI::window->SetStyle(0b00000);
    GUI::window->SetTitle( "Hello world!" );

    buttonArc->SetRequisition(sf::Vector2f(80, 80));
    buttonMin->SetRequisition(sf::Vector2f(80, 80));
    buttonMan->SetRequisition(sf::Vector2f(80, 80));

    sfg::Context::Get().GetEngine().SetProperties(""
                                                  "Button#btnArc {"
                                                  "BackgroundColor: #00FF0040;"
                                                  "}");

    //fixed->Put( label, sf::Vector2f(0, 0));
    fixed->Put( buttonArc, sf::Vector2f(0, 00));
    fixed->Put( buttonMin, sf::Vector2f(0, 90));
    fixed->Put( buttonMan, sf::Vector2f(0, 180));
    box->Pack(fixed, true, false);
    GUI::window->SetStyle(0b00000);
    GUI::window->Add(box);
//    auto labelBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f );

    GUI::desktop.Add(label);
    GUI::desktop.Add( GUI::window );

    char* test = new char[30];

    buttonArc->GetSignal( sfg::Widget::OnLeftClick ).Connect( [test, this] {
        snprintf(test, 30, "%i", GUI::event.mouseButton.x);
        GUI::label->SetPosition(sf::Vector2f(200, 500));
        GUI::label->SetText( test );
    } );

    buttonArc->GetSignal(sfg::Widget::OnMouseMove).Connect([test, this] {
        GUI::label->SetPosition(sf::Vector2f(GUI::event.mouseMove.x+50, GUI::event.mouseMove.y-50));
    });
    buttonArc->GetSignal(sfg::Widget::OnMouseLeave).Connect([test, this] {
        GUI::label->Show(false);
    });
    buttonArc->GetSignal(sfg::Widget::OnMouseEnter).Connect([test, this] {
        GUI::label->Show(true);
    });

}

bool GUI::handleEvent(sf::Event& event) {
    GUI::event = event;
    GUI::desktop.HandleEvent( event );

    /*Check whether action was performed inside of any button to ensure that one event will not call any
     * unwanted function.
     */
    if(GUI::event.type == sf::Event::MouseButtonPressed)
        for(sfg::Widget::Ptr w : GUI::window->GetChildren())
            if(w->GetAllocation().contains(GUI::event.mouseButton.x - w->GetAbsolutePosition().x,
                                           GUI::event.mouseButton.y - w->GetAbsolutePosition().y))
                return true;
    return false;
}

void GUI::display(sf::RenderWindow& renderWindow){
    //likely needs to get time to next update from game::deltatime
    desktop.Update(.017f);
    sfgui.Display(renderWindow);
}
