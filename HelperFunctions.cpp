#include "HelperFunctions.h"

png::image<png::gray_pixel> applyMatrix(png::image<png::gray_pixel> image, double modMatrix[])
{
  auto modImage = image;
  
  // Go through image
  for(unsigned int y = 1; y < modImage.get_height()-1; y++)
  {
    for(unsigned int x = 1; x < modImage.get_width()-1; x++)
    {
      // For each pixel, get surrounding elements and store in temp array
      double currentMatrix[9];
      int counter = 0;
      double runningTotal = 0.0;
      for(unsigned int i = y-1; i <= y+1; i++)
      {
        for(unsigned int k = x-1; k <= x+1; k++)
        {
          currentMatrix[counter] = (double)image[i][k];
          runningTotal += currentMatrix[counter];
          counter++;
        }
      }

      double accumulator = 0.0;
      for(int i = 0; i < 9; i++)
      {
        accumulator += modMatrix[i] * currentMatrix[i];
      }

      if(accumulator > 255.0)
      {
        modImage[y][x] = png::gray_pixel(255);
      }
      else if(accumulator < 0.0)
      {
        modImage[y][x] = png::gray_pixel(0);
      }
      else
      {
        modImage[y][x] = (int)accumulator;
      }
    }
  }
  return modImage;
}

int threshold(int value)
{
  const int THRESHOLD = 20;
  if(value > THRESHOLD)
  {
    return value;
  }
  else
  {
    return 0;
  }
}

png::image<png::gray_pixel> addGaussNoise(png::image<png::gray_pixel> modImage, double stdev)
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::normal_distribution<double> distribution(0.0, stdev);

  for(unsigned int y = 1; y < modImage.get_height()-1; y++)
  {
    for(unsigned int x = 1; x < modImage.get_width()-1; x++)
    {
      // Get random  number
      double modifier = distribution(generator);

      // Check if it is valid to add/subtract from image part
      if((int)modImage[y][x] + modifier > 255)
      {
        modImage[y][x] = png::gray_pixel(255);
      }
      else if((int)modImage[y][x] + modifier < 0)
      {
        modImage[y][x] = png::gray_pixel(0);
      }
      else
      {
        modImage[y][x] += modifier;
      }
    }
  }
  return modImage;
}