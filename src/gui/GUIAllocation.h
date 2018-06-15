#ifndef NONAME_GUIALLOCATION_H
#define NONAME_GUIALLOCATION_H

#include "../utils/Settings.h"
#include "../world/chunk/Chunk.h"

class ArchitectModeAllocation {
public:
    ArchitectModeAllocation(float scale, int windowWidth, int windowHeight) :
            GUIScale(scale),
            buttonSize(int(baseBlockSize * GUIScale)),
            blockScale(buttonSize / Chunk::TILE_SIZE),

            categorySelectorWindowHeight(baseCategorySelectorWindowHeight * GUIScale),
            categorySelectorWindowWidth(buttonSize),
            categorySelectorWindowPositionX(windowsSpacing),
            categorySelectorWindowPositionY(windowHeight - 1 - windowsSpacing - categorySelectorWindowHeight),

            categoryWindowsHeight(buttonSize),
            categoryWindowsWidth(baseCategoryWindowsWidth * GUIScale),
            categoryWindowsPositionX(categorySelectorWindowPositionX + 26 + buttonSize + windowsSpacing),
            categoryWindowsPositionY(windowHeight - windowsSpacing - 26 - categoryWindowsHeight),

            tooltipHeight(baseTooltipHeight * GUIScale),
            tooltipWidth(baseTooltipWidth * GUIScale),
            tooltipPositionX(windowWidth - windowsSpacing - tooltipWidth),
            tooltipPositionY(windowHeight - windowsSpacing - tooltipHeight),

            boxSpacing(baseBoxSpacing * GUIScale) {}

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

class ModeSwitcherAllocation {
public:
    ModeSwitcherAllocation(float scale, int windowWidth, int windowHeight) :
            GUIScale(scale),
            buttonWidth(GUIScale * baseButtonWidth),
            buttonHeight(GUIScale * baseButtonHeight),
            switchWindowHeight(GUIScale * baseSwitchWindowHeight),
            switchWindowWidth(GUIScale * baseSwitchWindowWidth),
            switchWindowPositionX(baseSwitchWindowPositionX),
            switchWindowPositionY(windowSpacing),
            boxSpacing(GUIScale * baseBoxSpacing) {}

    const float GUIScale;
    const int buttonWidth;
    const int buttonHeight;

    const int switchWindowHeight;
    const int switchWindowWidth;
    const int switchWindowPositionX;
    const int switchWindowPositionY;

    const float boxSpacing;

private:
    static const int baseButtonWidth = 27;
    static const int baseButtonHeight = 24;

    static const int windowSpacing = 15;

    static const int baseSwitchWindowHeight = 70;
    static const int baseSwitchWindowWidth = 123;
    static const int baseSwitchWindowPositionX = 350;

    static const int baseBoxSpacing = 10.f;
};

class GUIAllocation {
public:
    GUIAllocation(float scale) :
            GUIScale(scale),
            fontSize(int(12 * GUIScale)) {}

    const float GUIScale;
    const int fontSize;
};

#endif //NONAME_GUIALLOCATION_H
