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

#include "Framebuffer.h"
#include "icons.h"

/*
 * 
 */
int main(int argc, char** argv)
{
  Framebuffer fb("/dev/fb1");
  
  fb.saveScreen();

  fb.rectangleFilled(0, 80, 479, 242, 0);
  
  for(unsigned i=0;i < 3;i++)
  {
    unsigned x= i* 159;
    fb.rectangle(x,   82, x + 157, 240, Framebuffer::defineColor(128,128,128));
    fb.rectangle(x+1, 83, x + 156, 239, Framebuffer::defineColor(128,128,128));
  }
  
  fb.drawSprite(  6, 90, sprite1, 146, 142, true);
  fb.drawSprite(166, 90, sprite2, 138, 140, false);
  fb.drawSprite(323, 90, sprite1, 146, 142, false);
  
  std::this_thread::sleep_for(std::chrono::seconds(5));
    
  fb.restoreScreen();
  
  return 0;
}

