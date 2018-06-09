#ifndef NONAME_LADDER_H
#define NONAME_LADDER_H

#include "Component.h"
#include "../../../deps/json.h"

class LadderComponent : public Component{
public:
    static const componentId Id = componentId::Ladder;

    nlohmann::json serialize() override {
        nlohmann::json json;
        json["type"] = "Ladder";
        return json;
    };

    std::unique_ptr<Component> clone() override {
        return std::make_unique<LadderComponent>(*this);
    }

    explicit LadderComponent(nlohmann::json json){
    };
};


#endif //NONAME_LADDER_H
