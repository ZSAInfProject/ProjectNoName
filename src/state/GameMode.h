#ifndef NONAME_GAMEMODE_H
#define NONAME_GAMEMODE_H

/*!
 * Abstract class for every in-game mode
 */
class GameMode {
public:
    enum gameModesEnum{
        minerMode = 0,
        architectMode,
        managementMode
    };

    virtual int getTag() = 0;
};

#endif //NONAME_GAMEMODE_H