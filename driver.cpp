#include <iostream>
#include <png++/png.hpp>
#include <random>
#include <chrono>
#include "HelperFunctions.h"

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
  cout << "\n***********************************\n";

  std::string inputFilename = "steam.png";

  if(argc > 1)
  {
    inputFilename = argv[1];
    if(inputFilename.length() > 4)
    {
      if(inputFilename.substr(inputFilename.length()-4, 4) != ".png")
      {
        inputFilename += ".png"; 
      }
    }
    else
    {
      inputFilename += ".png";
    }
  }

  png::image< png::gray_pixel > image(inputFilename);
  cout <<"Image width: " << image.get_width() <<"\n";
  cout << "Image height: " << image.get_height() <<"\n";

  double blurCoeff = 0.1;
  double blurMatrix[9] = {1, 1, 1, 1, 2, 1, 1, 1, 1};
  for(int i = 0; i < 9; i++)
  {
    blurMatrix[i] *= blurCoeff;
  }

  auto blurImage = applyMatrix(image, blurMatrix);

  // Add Gaussian noise
  blurImage = addGaussNoise(blurImage, 3.0);

  double gxMatrix[9] = {1.0, 0.0, -1.0, 1.0, 0.0, -1.0, 1.0, 0.0, -1.0};
  auto gxImage = applyMatrix(blurImage, gxMatrix);

  double gyMatrix[9] = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, -1.0, -1.0, -1.0};
  auto gyImage = applyMatrix(blurImage, gyMatrix);

  png::image<png::gray_pixel> finalImage(blurImage);
  for(unsigned int y = 1; y < finalImage.get_height()-1; y++)
  {
    for(unsigned int x = 1; x < finalImage.get_width()-1; x++)
    {
      auto linearConvolve = sqrt(((int)gxImage[y][x]*(int)gxImage[y][x])+((int)gyImage[y][x]*(int)gyImage[y][x]));
      if(linearConvolve > 255)
      {
        finalImage[y][x] = 255;
      }
      else if(linearConvolve < 0)
      {
        finalImage[y][x] = 0;
      }
      else
      {
        finalImage[y][x] = (int)linearConvolve;
      }
    }
  }

  for(unsigned int y = 1; y < finalImage.get_height()-1; y++)
  {
    for(unsigned int x = 1; x < finalImage.get_width()-1; x++)
    {
      finalImage[y][x] = threshold(finalImage[y][x]);
    }
  }

  finalImage.write("output.png");
  cout << "\n***********************************\n";

  return 0;
}
