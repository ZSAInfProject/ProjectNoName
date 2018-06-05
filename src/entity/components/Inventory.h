//
// Created by zakrent on 6/5/18.
//

#ifndef NONAME_INVENTORY_H
#define NONAME_INVENTORY_H

#include "Component.h"
#include "../../../deps/json.h"
#include "../../utils/Resource.h"

class InventoryComponent : public Component{
public:
    static const componentId Id = componentId::Inventory;

    std::map<resourceType, Resource> resources;

    nlohmann::json serialize() override {
        nlohmann::json json;
        json["type"] = "Inventory";
        for(auto& resource : resources){
            nlohmann::json resData;
            resData["type"]=getResourceType(resource.second.type);
            resData["amount"]=resource.second.amount;
            json["resources"].push_back(resData);
        }
        return json;
    };

    std::unique_ptr<Component> clone() override {
        return std::make_unique<InventoryComponent>(*this);
    }

    explicit InventoryComponent(nlohmann::json json) {
        if(json.find("resources") != json.end()){
            for(const auto &resource : json["resources"]){
                Resource res{getResourceType(resource["type"].get<std::string>()), resource["amount"].get<unsigned int>()};
                resources[res.type] = res;
            }
        }
    };
};

#endif //NONAME_INVENTORY_H
