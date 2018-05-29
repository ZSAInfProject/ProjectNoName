//
// Created by zakrent on 5/29/18.
//

#ifndef NONAME_BUFFER_H
#define NONAME_BUFFER_H

#include <cstring>
#include <string>
#include <fstream>

enum bufferMode{
    store,
    load
};

class Buffer {
    int pointer;
    int currentSize;
    char* data;
    enum bufferMode mode;
    void resize(int size);
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

    void load(const std::string &filename);
    void save(const std::string &filename);

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

void Buffer::resize(int size) {
    auto* newData = new char[size];
    memcpy(newData, data, (size_t)currentSize);
    delete data;
    currentSize = size;
    data = newData;
}

void Buffer::load(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if(file.is_open() && !file.fail()){
        auto fsize = file.tellg();
        file.seekg( 0, file.end );
        fsize = file.tellg() - fsize;
        file.seekg( 0, file.beg );
        resize((int)fsize);
        file.read(data, fsize);
    }
    file.close();
}

void Buffer::save(const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if(file.is_open() && !file.fail()){
        file.write(data, currentSize);
    }
    file.close();
}


#endif //NONAME_BUFFER_H
