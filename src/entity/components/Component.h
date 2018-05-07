//
// Created by zakrent on 5/7/18.
//

#ifndef NONAME_COMPONENT_H
#define NONAME_COMPONENT_H

enum componentId{
    Unknown,
    Position
};

class Component{
public:
    static const componentId Id = componentId::Unknown;
};

#endif //NONAME_COMPONENT_H
