#include <Canvas/Pixel.h>
#include <cassert>

uint8_t Pixel::operator[](const uint32_t channel) const {
    assert(channel <= 3);

    switch (channel) {
        case 0:  return r;
        case 1:  return g;
        case 2:  return b;
        default: return a;
    }
}

uint8_t& Pixel::operator[](const uint32_t channel) {
    assert(channel <= 3);

    switch (channel) {
        case 0:  return r;
        case 1:  return g;
        case 2:  return b;
        default: return a;
    }
}
