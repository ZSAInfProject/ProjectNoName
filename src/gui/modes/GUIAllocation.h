#ifndef NONAME_GUIALLOCATION_H
#define NONAME_GUIALLOCATION_H

#include "../../utils/Settings.h"
#include "../../utils/Log.h"
#include "../../world/chunk/Chunk.h"

class ArchitectModeAllocation{
public:
    //! @param scale GUIScale, do not use values lower than one
    ArchitectModeAllocation(float scale) :
            buttonSize(int(baseBlockSize * scale)),
            blockScale(scale * baseBlockSize/Chunk::TILE_SIZE),
            categorySelectorWindowPositionY(Settings::get<unsigned int>("resolution_y") - 316),
            categoryWindowPositionY(Settings::get<unsigned int>("resolution_y") - 121),
            tooltipPositionX(Settings::get().get<unsigned int>("resolution_x") - 150),
            tooltipPositionY(Settings::get().get<unsigned int>("resolution_y") - 215)
    {
        if(scale < 1.f)
            Log::warn("GUI", "Scale value is incorrect, expect GUI to have strange behaviour");
    }

    const float blockScale;
    const int categorySelectorWindowPositionY;
    const int categoryWindowPositionY;
    const int tooltipPositionX;
    const int tooltipPositionY;
    const int buttonSize;

private:
    static const int baseBlockSize = 80;
};

class ManagementModeAllocation{
public:
    const float placeholder = 80.f;
};

#endif //NONAME_GUIALLOCATION_H
