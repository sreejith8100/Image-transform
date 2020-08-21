#include <iostream>
#include <cmath>
#include <cstdlib>

#include "uiuc/PNG.h"
#include "uiuc/HSLAPixel.h"
#include "ImageTransform.h"

/* ******************
(Begin multi-line comment...)

Write your name and email address in the comment space here:

Name:Sreejith S
Email:sreejith.zidane@gmail.com

(...end multi-line comment.)
******************** */

using uiuc::PNG;
using uiuc::HSLAPixel;

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
PNG grayscale(PNG image) {
  /// This function is already written for you so you can see how to
  /// interact with our PNG class.
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);
      // `pixel` is a reference to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly. No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel.s = 0;
    }
  }

  return image;
}



/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel euclidean
 * distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 160 pixels away, the luminance will always decreased by 80%.
 * 
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
PNG createSpotlight(PNG image, int centerX, int centerY) {
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);
      
      //Finding the Euclidian distancefrom the current pixel to (centerX,centerY)
      double d=sqrt(((centerX-x)*(centerX-x))+((centerY-y)*(centerY-y)));
      
      //The luminance reduces by 0.5% per pixel upto 80%
      d=d*0.5;
      if(d!=0){
        if(d>80){
          pixel.l*=0.2;
        }
        else{
          pixel.l-=(d/100)*pixel.l;
        }
      }
    }
  }
  return image;
}
 

/**
 * Returns a image transformed to Illini colors.
 *
 * The hue of every pixel is set to the a hue value of either orange or
 * blue, based on if the pixel's hue value is closer to orange than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The illinify'd image.
**/
PNG illinify(PNG image) {
  double iorange = 11;
  double iblue = 216;
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);
     
      //To find if the current hue is nearer to illini blue or orange:
     
      //The entire Hue circle can be divided into two parts.
     
      //Part 1: Checking whether the hue of the current pixel lies to the right side of the 
      //illini blue and to the left side of ilini orange.
     
      if(pixel.h>iblue || pixel.h<iorange){
        if(pixel.h<iorange){
          pixel.h=iorange;
        }
        else{
          pixel.h=(pixel.h-iblue)<((360-pixel.h)+iorange)?iblue:iorange;
        }
      }
     
      //Part 2: Checking whether the hue of the current pixel lies to the left side of the 
      //illini blue and to the right side of ilini orange.
     
      if(pixel.h<iblue && pixel.h>iorange){
        pixel.h=(pixel.h-iorange)<(iblue-pixel.h)?iorange:iblue;
      }
    }
  }
  return image;
}
 

/**
* Returns an image that has been watermarked by another image.
*
* The luminance of every pixel of the second image is checked, if that
* pixel's luminance is 1 (100%), then the pixel at the same location on
* the first image has its luminance increased by 0.2.
*
* @param firstImage  The first of the two PNGs, which is the base image.
* @param secondImage The second of the two PNGs, which acts as the stencil.
*
* @return The watermarked image.
*/
PNG watermark(PNG firstImage, PNG secondImage) {
  for (unsigned x = 0; x < firstImage.width(); x++) {
    for (unsigned y = 0; y < firstImage.height(); y++) {
      HSLAPixel & pixel1 = firstImage.getPixel(x, y);
      HSLAPixel & pixel2 = secondImage.getPixel(x, y);
      
      //Checking if the luminance of the stencil image is 1.
      
      if(pixel2.l==1){
      
        //Adding 0.2 to the luminance without exceeding 1.
        if(pixel1.l<=0.8){
          pixel1.l+=0.2;
        }
        else{
          pixel1.l=1;
        }
      }
    }
  }

  return firstImage;
}