# Simple CLI Image Editor

A lightweight command-line image editor written in C++. This tool allows you to apply a variety of visual effects to images directly from the terminal.

### Currently supported features

- Invert colors
- Sepia tone
- Grayscale conversion
- Luminance conversion (similar to grayscale)
- Rotate 90° (clockwise & counter-clockwise)
- Seam carving (content-aware resizing)
- Gaussian blur

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
- `grayscale`
- `luminance`
- `sepia`
- `rotate`
- `seam_carving`
- `blur`

## Dependencies

- C++20 (or later)
- CMake 3.10+
- [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) and [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h)
