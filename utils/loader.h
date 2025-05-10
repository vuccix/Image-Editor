#pragma once

#include "../effects/effects.h"

bool processImage(const char* input, const char* output, const char* effect);
void rgbWrite(int width, int height, const RGB_IMAGE& image, const char* output);
void grayscaleWrite(int width, int height, const GRAYSCALE& image, const char* output);
