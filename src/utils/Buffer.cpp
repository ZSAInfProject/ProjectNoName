#include "Buffer.h"

void Buffer::resize(uint size) {
    auto* newData = new char[size];
    memcpy(newData, data, (size_t)currentSize);
    delete data;
    currentSize = size;
    data = newData;
}

bool Buffer::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if(file.is_open() && !file.fail()) {
        auto fsize = file.tellg();
        file.seekg(0, std::ifstream::end);
        fsize = file.tellg() - fsize;
        file.seekg(0, std::ifstream::beg);
        resize(static_cast<uint>(fsize));
        file.read(data, fsize);
        file.close();
        return true;
    }
    file.close();
    return false;
}

bool Buffer::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if(file.is_open() && !file.fail()) {
        file.write(data, currentSize);
        file.close();
        return true;
    }
    file.close();
    return false;
}

