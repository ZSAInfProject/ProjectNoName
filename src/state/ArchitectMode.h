#ifndef NONAME_ARCHITECTMODE_H
#define NONAME_ARCHITECTMODE_H

#include "GameMode.h"

//!Placeholder for architect mode, as of now it only stores its tag
class ArchitectMode : public GameMode {
public:
    ArchitectMode();
    int getTag() override;
private:
    int tag = GameMode::gameModesEnum::architectMode;
};


#endif //NONAME_ARCHITECTMODE_H
