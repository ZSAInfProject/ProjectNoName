#include "ArchitectMode.h"

ArchitectMode::ArchitectMode() {}

int ArchitectMode::getTag() {
    return tag;
}

void ArchitectMode::setBlock(int newBlock) {
    selectedBlock = newBlock;
}

int ArchitectMode::getSelectedBlock() {
    return selectedBlock;
}
