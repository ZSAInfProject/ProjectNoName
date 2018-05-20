#ifndef NONAME_ZONE_H
#define NONAME_ZONE_H


#include <SFML/System.hpp>
#include <vector>

struct Zone {
    int x;
    int y;
    int w;
    int h;
    bool enabled;
};

class ZoneManager {
public:
    const Zone& getZone(int id);
    void setZone(int id, Zone zone);
    int addZone(Zone zone);

    bool isInZone(int id, int x, int y);
    bool isInZone(int id, sf::Vector2i vector);
    std::pair<bool, int> isInAnyZone(int x, int y);
    std::pair<bool, int> isInAnyZone(sf::Vector2i vector);

    void save();
    void load();
private:
    std::vector<Zone> zones;

};


#endif //NONAME_ZONE_H
