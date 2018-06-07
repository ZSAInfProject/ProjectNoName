#ifndef NONAME_GUIALLOCATION_H
#define NONAME_GUIALLOCATION_H

#include "../utils/Settings.h"
#include "../utils/Log.h"
#include "../world/chunk/Chunk.h"

class ArchitectModeAllocation{
public:
    ArchitectModeAllocation(float scale) :
            GUIScale(scale),
            buttonSize(int(baseBlockSize * GUIScale)),
            blockScale(buttonSize/Chunk::TILE_SIZE),

            categorySelectorWindowHeight(baseCategorySelectorWindowHeight * GUIScale),
            categorySelectorWindowWidth(buttonSize),
            categorySelectorWindowPositionX(windowsSpacing),
            categorySelectorWindowPositionY(Settings::get<unsigned int>("resolution_y") - 1 - windowsSpacing - categorySelectorWindowHeight),

            categoryWindowsHeight(buttonSize),
            categoryWindowsWidth(baseCategoryWindowsWidth * GUIScale),
            categoryWindowsPositionX(categorySelectorWindowPositionX + 26 +  buttonSize + windowsSpacing),
            categoryWindowsPositionY(Settings::get<unsigned int>("resolution_y") - windowsSpacing - 26 - categoryWindowsHeight),

            tooltipHeight(baseTooltipHeight * GUIScale),
            tooltipWidth(baseTooltipWidth * GUIScale),
            tooltipPositionX(Settings::get().get<unsigned int>("resolution_x") - windowsSpacing - tooltipWidth),
            tooltipPositionY(Settings::get().get<unsigned int>("resolution_y") - windowsSpacing - tooltipHeight),

            boxSpacing(baseBoxSpacing * GUIScale) {   }

    const float GUIScale;
    const int buttonSize;
    //! Used in tooltip to generate blocks sprite
    const float blockScale;

    //! Actual width of this window is this + 1
    const int categorySelectorWindowHeight;
    //! Actual width of this window is this + 26
    const int categorySelectorWindowWidth;
    const int categorySelectorWindowPositionX;
    //! -1: see categorySelectorWindowHeight
    const int categorySelectorWindowPositionY;

    const int categoryWindowsHeight;
    const int categoryWindowsWidth;
    //! 26: see categorySelectorWindowWidth
    const int categoryWindowsPositionX;
    //! 26: see categorySelectorWindowWidth
    const int categoryWindowsPositionY;

    //! Same as categorySelectorWindowWidth
    const int tooltipHeight;
    //! Same as categorySelectorWindowHeight
    const int tooltipWidth;
    const int tooltipPositionX;
    const int tooltipPositionY;

    const float boxSpacing;
private:
    static const int baseBlockSize = 80;
    static const int baseCategorySelectorWindowHeight = 300;
    static const int baseCategoryWindowsWidth = 300;
    //! value by which windows are away from other windows/ from borders
    static const int windowsSpacing = 15;
    static const int baseTooltipHeight = 200;
    static const int baseTooltipWidth = 125;
    static const int baseBoxSpacing = 10.f;
};

class ManagementModeAllocation{
public:
    const float placeholder = 80.f;
};

class GUIAllocation{
public:
    GUIAllocation(float scale) :
        GUIScale(scale),
        fontSize(int(12 * GUIScale)) {
        if(GUIScale < 1.f)
            Log::warn("GUI", "Scale value is incorrect, expect GUI to have strange behaviour");
    }
    const float GUIScale;
    const int fontSize;
};

#endif //NONAME_GUIALLOCATION_H
