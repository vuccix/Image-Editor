# Simple CLI Image Editor

A lightweight command-line image editor written in C++. This tool allows you to apply a variety of visual effects to images directly from the terminal.

### Currently supported features

- Invert colors
- Sepia tone
- Grayscale & luminance conversions
- Rotate 90° (clockwise & counter-clockwise)
- Brightness adjusting
- Channel swap (RGB -> BRG)
- Duotone effect
- Seam carving (content-aware resizing)
- Gaussian blur (3x3 & 5x5 kernels)
- Edge detection (Sobel & Canny)

## How to build

This project uses **CMake** for cross-platform building. To compile the project:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

Run the editor from the terminal as follows: `./image_editor <input_image_path> <output_image_path> <effect_name>`

### Example:

`./image_editor input.jpg output.jpg grayscale`

### Effect names:

- `invert`
- `grayscale`, `luminance`
- `brightness`
- `sepia`
- `rotate`
- `swap`
- `duotone`
- `seam_carving`
- `blur3`, `blur5`
- `sobel`, `canny`

## Dependencies

- C++20 (or later)
- CMake 3.10+
- [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) and [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)
