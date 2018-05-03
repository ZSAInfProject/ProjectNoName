#include <string>
#include "json.hpp"
#include "Log.h"

#ifndef NONAME_SETTINGSMANAGER_H
#define NONAME_SETTINGSMANAGER_H


class Settings {
public:
    //! Returns reference to singleton object.
    static Settings& get() {
        static Settings instance;
        return instance;
    }

    Settings(Settings const&) = delete;
    void operator= (Settings const&) = delete;

private:
    Settings() = default;

    nlohmann::json json;

    static constexpr auto TAG = "Settings";

public:
    template<typename T>
    static T get(std::string name) {
        if (Settings::get().json.find(name) != Settings::get().json.end()) {
            return Settings::get().json[name].get<T>();
        } else {
            Log::error(TAG, "No setting named: " + name);
            throw std::invalid_argument("No setting named: " + name);
        }
    };
    void setFile(std::string path) {
        std::ifstream ifs(path);
        if (ifs.is_open()) {
            json = nlohmann::json::parse(ifs);
        } else {
            Log::error(TAG, "Could not open Settings file");
        }
    };
};

#endif // NONAME_SETTINGSMANAGER_H