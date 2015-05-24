#ifndef ENUMS_H
#define ENUMS_H

/**
 * @brief Enum with available file format names.
 *
 */
typedef enum
{
    NO_FORMAT = 0,
    JPG,
    PNG,
    BMP,
    J2K,
    JPG2000,
    TGA,
    TIFF,
    BINARY,
    I2DC,   // Index 2D Data Compression.
    L2DC,   // Line 2D Data Compression.
} FileFormats;

/**
 * @brief Enum with available image processing algorithm names.
 *
 */
typedef enum
{
    NO_ALGORITHM = 0,
    NEGATIVE,
    HIT_AND_MISS,
    CLOSING,
    DILATION,
    EROSION,
    CONTRAST,
    RED,
    BLUE,
    GREEN,
    PIXELATE,
    EDGES,
    BRIGHTNESS,
    WARM,
    COOL,
    SATURATION,
    GRADIENT,
    GRAY,
    BINARIZATION,
    GAUSSIANBLUR,
    GAMMA,
    SHARPEN,
    CUSTOM,

    // Don't use it. How can I put a value here even?
    ALGORITHM_COUNT
} Algorithms;

#endif // ENUMS_H
