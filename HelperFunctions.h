#pragma once

#include <iostream>
#include <png++/png.hpp>
#include <random>

png::image<png::gray_pixel> applyMatrix(png::image<png::gray_pixel> image, double modMatrix[]);

int threshold(int value);

png::image<png::gray_pixel> addGaussNoise(png::image<png::gray_pixel> image, double stdev);
