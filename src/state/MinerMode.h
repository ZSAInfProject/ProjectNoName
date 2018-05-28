#ifndef NONAME_MINERMODE_H
#define NONAME_MINERMODE_H

#include "GameMode.h"

//!Placeholder for miner mode, as of now it only stores its tag
class MinerMode : public GameMode {
public:
    MinerMode();
    int getTag() override;
private:
    int tag = GameMode::gameModesEnum::minerMode;
};

#endif //NONAME_MINERMODE_H
