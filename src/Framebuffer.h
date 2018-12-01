/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Framebuffer.h
 * Author: Joerg
 *
 * Created on 29. November 2018, 18:10
 */

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>
#include <linux/fb.h>

class Framebuffer
{
public:
  /**
   * Construct the Framebuffer
   * @param device the device name, typically /dev/fb*
   */
  Framebuffer(const char *device);
  virtual ~Framebuffer();
  
  void saveScreen();
  void restoreScreen();
  
  static inline uint16_t defineColor(uint8_t r, uint8_t g, uint8_t b)
  {
    return ((r & 0xf8) << (11-3)) | ((g & 0xf0) << (5-2)) | ((b & 0xf8) >> 3);
  }
  
  void setPixel(unsigned x, unsigned y, uint16_t color);

  void rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, uint16_t color);
  
  void rectangleFilled(unsigned x1, unsigned y1, unsigned x2, unsigned y2, uint16_t color);

  void drawSprite( unsigned x, unsigned y, const uint16_t* data, unsigned width, unsigned height, bool reverse);
  
private:

  void close();
  
  int handle= -1;
  uint8_t *memory= nullptr;
  uint8_t *saveMemory = nullptr;
  
  struct fb_fix_screeninfo screeninfo;
  struct fb_var_screeninfo varScreeninfo;
};

#endif /* FRAMEBUFFER_H */

