#pragma once

#include "../effects/effects.h"

using Effects::RGB_IMAGE, Effects::GRAYSCALE, Effects::PIXEL;

bool processImage(const char* input, const char* output, const char* effect);
void rgbWrite(int width, int height, const RGB_IMAGE& image, const char* output);
void grayscaleWrite(int width, int height, const GRAYSCALE& image, const char* output);
