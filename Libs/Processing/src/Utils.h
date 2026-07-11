#pragma once

#include <cstdint>
#include <vector>
#include <mdspan>

struct Pixel;

namespace Utils {

    template <typename T>
    std::vector<T> promote(std::span<const Pixel> pixels);

    template <typename T>
    void demote(std::span<const T> data, std::span<Pixel> pixels);

    template <typename T>
    using std_mdspan = std::mdspan<T, std::dextents<size_t, 2>>;

    template <typename T, typename Op, typename Writer>
    void convolution(std_mdspan<const T> image, int32_t kHeight, int32_t kWidth, Op&& op, Writer&& writer);

}

#include "Utils.tpp"
