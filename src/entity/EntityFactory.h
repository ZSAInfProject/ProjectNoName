#include <string>
#include "Entity.h"

class EntityFactory {
public:
    EntityFactory(std::string file);
    std::shared_ptr<Entity> get(std::string name);

private:
    std::unordered_map<std::string, std::unique_ptr<Entity>> entities;
    static constexpr auto TAG = "EntityFactory";
};
