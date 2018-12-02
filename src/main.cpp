/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Joerg
 *
 * Created on 29. November 2018, 18:07
 */

#include <cstdlib>
#include <cstdint>
#include <thread>
#include <chrono>
#include <array>

#include "Framebuffer.h"
#include "TouchScreen.h"
#include "icons.h"

struct SpriteDef
{
  const uint16_t *data;
  unsigned width;
  unsigned height;
  bool reverse;
  unsigned xPaint;
  unsigned yPaint;
};

std::array<SpriteDef,3> sprites {{
  { sprite1, 146, 142, true,    6, 90 },
  { sprite2, 138, 140, false, 166, 90 },
  { sprite1, 146, 142, false, 323, 90 }
                                 }};
/*
 * 
 */
int main(int argc, char** argv)
{
  Framebuffer fb("/dev/fb1");
  TouchScreen touch("/dev/input/event0", fb.xres(), fb.yres(), [&fb](TouchScreen::AreaTouch touch, int areaId)
  {
    if( areaId >= 0 )
    {
      const SpriteDef& sd= sprites[areaId];
      
      if( touch == TouchScreen::AreaTouch::Enter )
        fb.drawSprite( sd.xPaint, sd.yPaint, sd.data, sd.width, sd.height, sd.reverse, 0, Framebuffer::defineColor(0xc0, 0xc0, 0xc0));
      else
        fb.drawSprite( sd.xPaint, sd.yPaint, sd.data, sd.width, sd.height, sd.reverse);
    }
  });
  
  fb.saveScreen();

  fb.rectangleFilled(0, 80, 479, 242, 0);
  
  for(unsigned i=0;i < 3;i++)
  {
    unsigned x= i* 159;
    fb.rectangle(x,   82, x + 157, 240, Framebuffer::defineColor(128,128,128));
    fb.rectangle(x+1, 83, x + 156, 239, Framebuffer::defineColor(128,128,128));
  }
  
  for(auto it= sprites.begin(); it != sprites.end(); it++)
  {
    fb.drawSprite( it->xPaint, it->yPaint, it->data, it->width, it->height, it->reverse);
    touch.addArea( {it - sprites.begin(), it->xPaint, it->yPaint, it->width, it->height} );
  }
  /*fb.drawSprite(  6, 90, sprite1, 146, 142, true);
  fb.drawSprite(166, 90, sprite2, 138, 140, false);
  fb.drawSprite(323, 90, sprite1, 146, 142, false);
  
  touch.addArea({0,   0, 82, 159, 160});
  touch.addArea({1, 159, 82, 159, 160});
  touch.addArea({2, 318, 82, 159, 160});*/
  
  for(unsigned i=0;i < 500;i++)
  {
    touch.readEvents();
  
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }  
  fb.restoreScreen();
  
  return 0;
}

