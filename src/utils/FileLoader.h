#ifndef NONAME_FILELOADER_H
#define NONAME_FILELOADER_H

#include <fstream>
#include "../../deps/json.h"
#include "Log.h"

nlohmann::json loadJsonResource(const std::string& filename) {
    std::ifstream data("res/" + filename);
    if(data.is_open()) {
        try {
            nlohmann::json json = nlohmann::json::parse(data);
            return json;
        }
        catch(nlohmann::json::parse_error& parse_error) {
            Log::error("FileLoader", "Could not parse json resource: res/" + filename);
            return nlohmann::json();
        }
    }
    else {
        Log::error("FileLoader", "Could not open json resource: res/" + filename);
        return nlohmann::json();
    }
}


#endif //NONAME_FILELOADER_H
