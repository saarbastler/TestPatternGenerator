/*
 * GNU General Public License v3.0
 *
 * File:   Framebuffer.h
 * Author: Saarbastler, joerg@saarbastler.de
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>
#include <linux/fb.h>

/**
 * Direct Framebuffer access, works only on RGB565 Display
 */
class Framebuffer
{
public:
  /**
   * Construct the Framebuffer
   * @param device the device name, typically /dev/fb*
   */
  Framebuffer(const char *device);
  virtual ~Framebuffer();
  
  /**
   * save the screen content in a buffer
   */
  void saveScreen();
  
  /**
   * restore the Screen from the buffer
   */
  void restoreScreen();
  
  /**
   * Calculate the Color from single Red/GreenBlue values
   * @param r Red 0..255 (only 5 upper Bits used)
   * @param g Green 0..255 (only 6 upper Bits used)
   * @param b Blue 0..255 (only 5 upper Bits used)
   * @return the 16 Bit color Value
   */
  static inline uint16_t defineColor(uint8_t r, uint8_t g, uint8_t b)
  {
    return ((r & 0xf8) << (11-3)) | ((g & 0xf0) << (5-2)) | ((b & 0xf8) >> 3);
  }
  
  /**
   * ste a Pixel
   * @param x the X coordinate
   * @param y the Y coordinate
   * @param color the Pixel color
   */
  void setPixel(unsigned x, unsigned y, uint16_t color);

  /**
   * draw a rectangle
   * @param x1 first corner x 
   * @param y1 first corner y
   * @param x2 opposide corner x
   * @param y2 opposide corner y
   * @param color the rectangles color
   */
  void rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, uint16_t color);
  
  /**
   * draw a filled rectangle
   * @param x1 first corner x 
   * @param y1 first corner y
   * @param x2 opposide corner x
   * @param y2 opposide corner y
   * @param color the rectangles color
   */
  void rectangleFilled(unsigned x1, unsigned y1, unsigned x2, unsigned y2, uint16_t color);

  /**
   * Draw a Sprite. The Sprite is stored as 16 Bit color values starting at left top, row per row.
   * 
   * @param x The left x coordinate to draw
   * @param y The top x coordinate to draw
   * @param data pointer to the sprite data
   * @param width the sprite width
   * @param height the sprite height
   * @param reverse if true, reverse the sprite horizontally
   * @param backgroundColor background color to replace
   * @param backgroundColorReplace replacement of background color
   */
  void drawSprite( unsigned x, unsigned y, const uint16_t* data, unsigned width, unsigned height, bool reverse
  , uint16_t backgroundColor = 1, uint16_t backgroundColorReplace = 1);
  
  unsigned xres()
  {
    return varScreeninfo.xres;
  }
  
  unsigned yres()
  {
    return varScreeninfo.yres;
  }

private:

  void close();
  
  int handle= -1;
  uint8_t *memory= nullptr;
  uint8_t *saveMemory = nullptr;
  
  struct fb_fix_screeninfo screeninfo;
  struct fb_var_screeninfo varScreeninfo;
};

#endif /* FRAMEBUFFER_H */

