//
// Created by zakrent on 5/29/18.
//

#ifndef NONAME_BUFFER2_H
#define NONAME_BUFFER2_H

#include <cstring>
#include <string>
#include <fstream>

enum bufferMode{
    store,
    load
};

class Buffer {
    int pointer;
    uint currentSize;
    char* data;
    enum bufferMode mode;
    void resize(uint size);
public:
    template<typename T>
    void io(T* ptr){
        constexpr size_t size = sizeof(T);
        switch(mode){
            case bufferMode::store:
                if(pointer+size > currentSize){
                    resize(currentSize+(int)size*3);
                }
                memcpy(data+pointer, ptr, size);
                pointer += size+1;
                break;
            case bufferMode::load:
                memcpy(ptr, data+pointer, size);
                pointer += size+1;
                break;
        }
    }

    bool load(const std::string &filename);
    bool save(const std::string &filename);

    void setMode(enum bufferMode _mode){
        mode = _mode;
        pointer = 0;
    }

    explicit Buffer(enum bufferMode _mode, int size = 1) {
        data = new char[size];
        currentSize = size;
        mode = _mode;
        pointer = 0;
    }

    ~Buffer() {
        delete data;
    }
};


#endif //NONAME_BUFFER_H
