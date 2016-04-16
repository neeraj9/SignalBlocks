//
// Created by nsharma on 4/16/16.
//

#include "MemInputStream.h"

using namespace sigblocks;

MemInputStream::MemInputStream(char* data, std::size_t len) {
    setg(data, data, data + len);
    setp(data, data + len);
}
