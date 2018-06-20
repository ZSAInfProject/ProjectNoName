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

//! Class managing all NPC zones
class ZoneManager {
public:
    static constexpr auto TAG = "ZoneManager";

    //! Returns zone with supplied id
    const Zone& getZone(int id);

    //! Replaces zone at supplied id with supplied zone
    void setZone(int id, Zone zone);

    //! Adds zone
    int addZone(Zone zone);

    //! Checks if coordinates are inside zone with supplied id
    bool isInZone(int id, int x, int y);
    //! Checks if coordinates are inside zone with supplied id
    bool isInZone(int id, sf::Vector2i vector);
    //! Checks if coordinates are inside any zone
    std::pair<bool, int> isInAnyZone(int x, int y);
    //! Checks if coordinates are inside any zone
    std::pair<bool, int> isInAnyZone(sf::Vector2i vector);

    //! Saves all zones to file
    void save();
    //! Loads all zones from file
    void load();
private:
    std::vector<Zone> zones;

};


#endif //NONAME_ZONE_H
