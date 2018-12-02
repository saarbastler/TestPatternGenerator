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
#include <iostream>

#include "TestPatternGenerator.h"
/*
 * 
 */
int main(int argc, char** argv)
{
  if( argc != 4)
  {
    std::cout << "TestPatternGenerator usage:" << std::endl
      << "tpg framebufferDevice inputDevice path" << std::endl
      << "  framebufferDevice is typically /dev/fb1" << std::endl
      << "  inputDevice is typicall /dev/input/event0"<< std::endl
      << "  path is the path to the images and scripts"<< std::endl;
    
    exit (-1);
  }
  TestPatternGenerator testPatternGenerator( argv[1], argv[2], argv[3]); //"/dev/fb1", "/dev/input/event0");
  
  for(;;)
  {
    testPatternGenerator.touchScreen().readEvents();
  
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }  
  
  return 0;
}

