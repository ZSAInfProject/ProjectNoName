#include "ZoneManager.h"
#include "../utils/Settings.h"

const Zone &ZoneManager::getZone(int id) {
    return zones[id];
}

void ZoneManager::setZone(int id, Zone zone) {
    if (id < zones.size()) {
        zones[id] = zone;
    }
}

int ZoneManager::addZone(Zone zone) {
    zones.push_back(zone);
    return static_cast<int>(zones.size() - 1);
}


bool ZoneManager::isInZone(int id, int x, int y) {
    return x < zones[id].x && x > zones[id].x - zones[id].w &&
        y < zones[id].y && y > zones[id].y - zones[id].h;
}

bool ZoneManager::isInZone(int id, sf::Vector2i vector) {
    return isInZone(id, vector.x, vector.y);
}

std::pair<bool, int> ZoneManager::isInAnyZone(int x, int y) {
    for (int i=0; i<zones.size(); i++) {
        if (isInZone(i, x, y) && zones[i].enabled) return std::make_pair(true, i);
    }
    return std::make_pair(false, -1);
}

std::pair<bool, int> ZoneManager::isInAnyZone(sf::Vector2i vector) {
    return isInAnyZone(vector.x, vector.y);
}

void ZoneManager::save() {
    std::ofstream file(Settings::get<std::string>("save_path") + "zones.td");
    if(file.good()) {
        file.write(reinterpret_cast<const char*>(&zones[0]), zones.size()*sizeof(Zone));
        file.close();
    }

}

void ZoneManager::load() {
    std::ifstream file(Settings::get<std::string>("save_path") + "zones.td", std::ifstream::in | std::ifstream::binary);
    if(file.good()) {
        const size_t count = static_cast<const size_t>(file.tellg() / sizeof(Zone));
        zones = std::vector<Zone>(count);
        file.read(reinterpret_cast<char*>(&zones[0]), count*sizeof(Zone));
    }
}
