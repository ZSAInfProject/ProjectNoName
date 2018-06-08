#ifndef NONAME_MANAGEMENTMODE_H
#define NONAME_MANAGEMENTMODE_H

#include "GameMode.h"

//!Placeholder for architect mode, as of now it only stores its tag
class ManagementMode : public GameMode {
public:
    ManagementMode();
    int getTag() override;
private:
    int tag = GameMode::gameModesEnum::managementMode;
};



#endif //NONAME_MANAGEMENTMODE_H
