#include "../effects.h"

void Effects::invert(RGB_IMAGE& img) {
    for (auto& row : img) {
        for (PIXEL& pixel : row) {
            pixel[0] = 255 - pixel[0];
            pixel[1] = 255 - pixel[1];
            pixel[2] = 255 - pixel[2];
        }
    }
}

RGB_IMAGE Effects::rotate90deg(const RGB_IMAGE& img, bool clockwise) {
    size_t rows = img.size();
    size_t cols = img[0].size();

    RGB_IMAGE res(cols, std::vector<PIXEL>(rows));
    if (clockwise) {
        for (size_t i{}; i < rows; ++i)
            for (size_t j{}; j < cols; ++j)
                res[j][rows - 1 - i] = img[i][j];
    }
    // counterclockwise
    else {
        for (size_t i{}; i < rows; ++i)
            for (size_t j{}; j < cols; ++j)
                res[cols - 1 - j][i] = img[i][j];
    }

    return res;
}

void Effects::channelSwap(RGB_IMAGE& img) {
    for (auto& i : img)
        for (PIXEL& pix : i) { // RGB -> BRG
            std::swap(pix[0], pix[1]); // R <-> G
            std::swap(pix[0], pix[2]); // G <-> B
        }
}

void Effects::brightness(RGB_IMAGE& img, int value) {
    for (auto& i : img) {
        for (PIXEL& pix : i) {
            int r = value + pix[0];
            int g = value + pix[1];
            int b = value + pix[2];

            int new_r = (value >= 0) ? std::min(255, r) : std::max(0, r);
            int new_g = (value >= 0) ? std::min(255, g) : std::max(0, g);
            int new_b = (value >= 0) ? std::min(255, b) : std::max(0, b);

            pix[0] = static_cast<uint8_t>(new_r);
            pix[1] = static_cast<uint8_t>(new_g);
            pix[2] = static_cast<uint8_t>(new_b);
        }
    }
}

void Effects::contrast(RGB_IMAGE& img, float factor) {
    for (auto& i : img) {
        for (PIXEL& pix : i) {
            float r = factor * (float)(pix[0] - 128) + 128;
            float g = factor * (float)(pix[1] - 128) + 128;
            float b = factor * (float)(pix[2] - 128) + 128;

            int new_r = (r >= 0) ? std::min(255, (int)r) : std::max(0, (int)r);
            int new_g = (g >= 0) ? std::min(255, (int)g) : std::max(0, (int)g);
            int new_b = (b >= 0) ? std::min(255, (int)b) : std::max(0, (int)b);

            pix[0] = static_cast<uint8_t>(new_r);
            pix[1] = static_cast<uint8_t>(new_g);
            pix[2] = static_cast<uint8_t>(new_b);
        }
    }
}

void Effects::duotone(RGB_IMAGE& img, const PIXEL& high, const PIXEL& low) {
    GRAYSCALE gray = grayscale(img);

    for (size_t i = 0; i < img.size(); ++i)
        for (size_t j = 0; j < img[0].size(); ++j) {
            float intensity = gray[i][j] / 255.f;

            PIXEL& pix = img[i][j];
            pix[0] = static_cast<uint8_t>((1.f - intensity) * low[0] + intensity * high[0]);
            pix[1] = static_cast<uint8_t>((1.f - intensity) * low[1] + intensity * high[1]);
            pix[2] = static_cast<uint8_t>((1.f - intensity) * low[2] + intensity * high[2]);
        }
}

void Effects::pixelate(RGB_IMAGE& img, int blockSize) {
    size_t height = img.size();
    size_t width  = img[0].size();

    for (size_t y{}; y < height; y += blockSize) {
        for (size_t x{}; x < width; x += blockSize) {
            // calculate average color in the block
            int sumR{}, sumG{}, sumB{};
            int count{};

            for (int dy{}; dy < blockSize && (y + dy) < height; ++dy)
                for (int dx{}; dx < blockSize && (x + dx) < width; ++dx, ++count) {
                    PIXEL& pix = img[y + dy][x + dx];
                    sumR += pix[0];
                    sumG += pix[1];
                    sumB += pix[2];
                }

            const auto avgR = static_cast<uint8_t>(sumR / count);
            const auto avgG = static_cast<uint8_t>(sumG / count);
            const auto avgB = static_cast<uint8_t>(sumB / count);

            // fill block with average color
            for (int dy{}; dy < blockSize && (y + dy) < height; ++dy)
                for (int dx{}; dx < blockSize && (x + dx) < width; ++dx)
                    img[y + dy][x + dx] = { avgR, avgG, avgB };
        }
    }
}
