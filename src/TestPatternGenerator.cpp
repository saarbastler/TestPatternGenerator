/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TestPatternGenerator.cpp
 * Author: Joerg
 * 
 * Created on 2. Dezember 2018, 11:37
 */

#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#include "TestPatternGenerator.h"
#include "icons.h"
#include "TPGException.h"

std::array<TestPatternGenerator::SpriteDef, 3> TestPatternGenerator::sprites
{
  {
    {
      sprite1, 146, 142, true, 6, 90
    },
    {
      sprite2, 138, 140, false, 166, 90
    },
    {
      sprite1, 146, 142, false, 323, 90
    }
  }
};

TestPatternGenerator::TestPatternGenerator(const char *frameBufferDevice, const char *touchDevice, const char *path)
: framebuffer(frameBufferDevice), touch(touchDevice, framebuffer.xres(), framebuffer.yres()
, std::bind(&TestPatternGenerator::touchCalback, this, std::placeholders::_1, std::placeholders::_2))
, path(path)
{
  findJpgFiles();

  if (files.size() == 0)
    throw TPGException("no *.jpg files found");

  showPicture(pictureIndex);
}

void TestPatternGenerator::touchCalback(TouchScreen::AreaTouch areaTouch, int areaId)
{
  std::cout << int(areaTouch) << ", " << areaId << " Vis: " << menuVisible << std::endl;

  if (menuVisible)
  {
    if (areaId >= 0)
    {
      const SpriteDef& sd = sprites[areaId];

      if (areaTouch == TouchScreen::AreaTouch::Enter)
        framebuffer.drawSprite(sd.xPaint, sd.yPaint, sd.data, sd.width, sd.height, sd.reverse, 0, Framebuffer::defineColor(0xc0, 0xc0, 0xc0));
      else
        framebuffer.drawSprite(sd.xPaint, sd.yPaint, sd.data, sd.width, sd.height, sd.reverse);
    }

    if (areaTouch == TouchScreen::AreaTouch::Clicked)
    {
      switch (areaId)
      {
        case 0:
          pictureIndex--;
          if (pictureIndex < 0)
            pictureIndex = files.size() - 1;
          
          framebuffer.restoreScreen();
          menuVisible = false;
          touch.clearAreas();
          shutdownCount= 0;
          
          showPicture(pictureIndex);
          break;

        case 1:
          shutdownCount++;
          if( shutdownCount > 1 )
          {
            shutdown();
            exit(0);
          }
          break;
          
        case 2:
          pictureIndex++;
          if (pictureIndex >= files.size())
            pictureIndex = 0;
          
          framebuffer.restoreScreen();
          menuVisible = false;
          touch.clearAreas();
          shutdownCount= 0;
          
          showPicture(pictureIndex);
          break;

        default:
          ;
      }
    }
  }
  else
  {
    paintMenu();
    menuVisible = true;
  }
}

void TestPatternGenerator::paintMenu()
{
  framebuffer.saveScreen();

  framebuffer.rectangleFilled(0, 80, 479, 242, 0);

  for (unsigned i = 0; i < 3; i++)
  {
    unsigned x = i * 159;
    framebuffer.rectangle(x, 82, x + 157, 240, Framebuffer::defineColor(128, 128, 128));
    framebuffer.rectangle(x + 1, 83, x + 156, 239, Framebuffer::defineColor(128, 128, 128));
  }

  touch.clearAreas();
  for (auto it = sprites.begin(); it != sprites.end(); it++)
  {
    framebuffer.drawSprite(it->xPaint, it->yPaint, it->data, it->width, it->height, it->reverse);
    touch.addArea({it - sprites.begin(), it->xPaint, it->yPaint, it->width, it->height});
  }
}

void TestPatternGenerator::findJpgFiles()
{
  DIR *dir;
  struct dirent *entry;

  dir = opendir(path.c_str());
  if (dir != nullptr)
  {
    while ((entry = readdir(dir)) != nullptr)
    {
      std::string name(entry->d_name);
      if (name != "." && name != ".." && name.length() > 4 && name.find(".jpg") == name.length() - 4)
        files.emplace_back(name);
    }

    closedir(dir);
  }
}

void TestPatternGenerator::showPicture(unsigned index)
{
  if (fork() == 0)
  {
    std::string cmd(path);
    cmd += "/show.sh ";
    cmd += files[index];
    
    int error = execl("/bin/sh", "sh", "-c", cmd.c_str(), nullptr);

    char buffer[1000];
    strerror_r(errno, buffer, 1000);
    
    std::cout << "exec error: " << error << " Errno: " << errno << " : " << buffer << std::endl;
  }
}

void TestPatternGenerator::shutdown()
{
  execl("/bin/sh", "sh", "-c", "halt", nullptr);
}
