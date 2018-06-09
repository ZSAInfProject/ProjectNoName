#include <imgui.h>
#include "Debug.h"
#include "../Game.h"
#include "Log.h"
#include "Controls.h"
#include "../entity/components/Position.h"
#include "../entity/components/Control.h"
#include "../entity/components/Mining.h"

void Debug::update() {
    while(logMessages.size() > 50){
        logMessages.erase(logMessages.begin());
    }

    if(Controls::isKeyboardButtonPressed(sf::Keyboard::F3)){
        if(!toggled){
            toggled = true;
            active = !active;
        }
    }else{
        toggled = false;
    }

    if(Game::canUpdateimGui()){
        updateErrors();
        updateMainDebug();
    }

}

void Debug::updateErrors() {
    for(int i = 0; i < errorMessages.size(); i++){
        ImGui::SetNextWindowPos(sf::Vector2f(ImGui::GetIO().DisplaySize)*0.5f+sf::Vector2f(-150, -250+i*10));
        ImGui::SetNextWindowSizeConstraints(sf::Vector2f(300, 10), sf::Vector2f(300, 500));
        ImGui::PushStyleColor(ImGuiCol_TitleBg, IM_COL32(0xff, 0x00, 0x00, 0xaa));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(0xff, 0x00, 0x00, 0xaa));
        ImGui::Begin(("Error##"+std::to_string(i)).c_str(), nullptr,
                     ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
        ImGui::TextWrapped(errorMessages[i].c_str());
        ImGui::SetCursorPosX(ImGui::GetWindowWidth()/2-50);
        if(ImGui::Button("Ok", sf::Vector2f(100,20))){
            errorMessages.erase(errorMessages.begin()+i);
        }
        ImGui::End();
        ImGui::PopStyleColor(2);
    }
}

void Debug::updateMainDebug() {
    if(active) {
        ImGui::SetNextWindowSizeConstraints(sf::Vector2f(300, 10), sf::Vector2f(300, 500));
    }

    ImGui::Begin("Performance info", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowPos(sf::Vector2f(5,5));
    ImGui::Text("Press F3 to toggle");

    if(active) {
        ImGui::Text("FPS: %-6.1f (%-7.3f ms/frame)", fps, 1000.0f / fps);
        ImGui::Text("UPS: %-6.1f (%-7.3f µs/update)", ups, 10e5 / ups);
        if (ImGui::CollapsingHeader("Plots")) {
            ImGui::PlotLines("FPS plot", fpsHistory.data(), static_cast<int>(fpsHistory.size()));
            ImGui::PlotLines("UPS plot", upsHistory.data(), static_cast<int>(upsHistory.size()));
        }

        ImGui::Text("Entities: %.i", entityCount);
        ImGui::Text("Chunk cache: %.i", loadedChunks);
        sf::Vector2f& playerPos = player->getComponent<PositionComponent>()->position;
        ImGui::Value("X", playerPos.x);
        ImGui::SameLine();
        ImGui::Value("Y", playerPos.y);

        if (ImGui::CollapsingHeader("Cheats")) {
            ImGui::SliderFloat("Speed", &player->getComponent<ControlComponent>()->speed, 100.0f, 10000.0f);
            ImGui::InputFloat2("Position", &player->getComponent<PositionComponent>()->position.x);
            ImGui::SliderInt("Mining", (int*)&player->getComponent<MiningComponent>()->miningDuration, 1, 200);
        }

        if (ImGui::CollapsingHeader("Log")) {
            ImGui::BeginChild("log-scroll", sf::Vector2f(0, 200), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
            for (auto &log: logMessages) {
                switch (log.first) {
                    case WARN:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0xff, 0xff, 0x00, 0xaa));
                        break;
                    case ERROR:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0xff, 0x00, 0x00, 0xaa));
                        break;
                    default:
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0xff, 0xff, 0xff, 0xaa));
                        break;
                }
                ImGui::TextWrapped(log.second.c_str());
                ImGui::PopStyleColor();
            }
            ImGui::SetScrollHere(1.0f);
            ImGui::EndChild();
        }
    }
    ImGui::End();
}

void Debug::reportUpdateTime(std::chrono::microseconds time) {
    if(time.count() == 0)
        return;
    ups = (static_cast<float>(10e5 / time.count())+200*ups)/201.0f;
}

void Debug::reportRenderTime(std::chrono::milliseconds time) {
    if(time.count() == 0)
        return;
    fps = (static_cast<float>(10e2 / time.count())+60*fps)/61.0f;
}

void Debug::notify(enum Mode mode, std::string tag, std::string msg) {
    std::string modeString;
    switch(mode){
        case VERBOSE:
            modeString = "V";
            break;
        case DEBUG:
            modeString = "D";
            break;
        case INFO:
            modeString = "I";
            break;
        case WARN:
            modeString = "W";
            break;
        case ERROR:
            modeString = "E";
            break;
        case NOLOG:
            modeString = "N";
            break;
    }
    logMessages.emplace_back(mode, "["+modeString+"]"+tag+": "+msg);
    if(mode == ERROR){
        errorMessages.push_back(tag+": "+msg);
    }
}

Debug::Debug() {
    ups = 0.0f;
    fps = 0.0f;
    active = true;
    toggled = false;
    Log::get().subscribe(*this);
    for(int i = 0; i < 60; i++){
        fpsHistory.push_back(0.0f);
        upsHistory.push_back(0.0f);
    }
}

void Debug::reportLoadedChunks(int n) {
    loadedChunks = n;
}

void Debug::tick() {
    upsHistory.push_back(ups);
    upsHistory.erase(upsHistory.begin());
    fpsHistory.push_back(fps);
    fpsHistory.erase(fpsHistory.begin());
}

void Debug::reportEntityCount(int n) {
    entityCount = n;
}

void Debug::reportPlayerEntity(std::shared_ptr<Entity> _player) {
    player = _player;
}


