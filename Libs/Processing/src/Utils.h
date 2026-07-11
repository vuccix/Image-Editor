#pragma once

#include <Canvas/Layer.h>

namespace Utils {

    template <typename T>
    using std_mdspan = std::mdspan<T, std::dextents<size_t, 2>>;

    template <typename T>
    std::vector<T> promote(std::span<const Pixel> pixels);

    template <typename T>
    void demote(std::span<const T> data, std::span<Pixel> pixels);

    template <typename T, typename U>
    std::vector<T> convolution(std_mdspan<const T> image, std_mdspan<const U> kernel);

}
