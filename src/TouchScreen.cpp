/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TouchScreen.cpp
 * Author: Joerg
 * 
 * Created on 1. Dezember 2018, 14:51
 */

#include "TouchScreen.h"
#include "TPGException.h"

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

constexpr bool testBit(unsigned bit, uint8_t bits[])
{
  return (bits[bit >> 3] & (1 << (bit & 7))) != 0;
}

TouchScreen::TouchScreen(const char *device, unsigned xres, unsigned yres, Callback&& callback) : callback(callback)
{
  res.x = xres;
  res.y = yres;

  handle = open(device, O_RDWR);
  if (handle < 0)
    throw TPGException("unable to open input device '%s'", device);

  uint8_t bits[1 + (KEY_MAX >> 3)];
  struct input_absinfo abs;
  ioctl(handle, EVIOCGBIT(EV_ABS, KEY_MAX), bits);
  if (!testBit(ABS_MT_POSITION_X, bits) || !testBit(ABS_MT_POSITION_Y, bits))
  {
    ioctl(handle, EVIOCGABS(ABS_X), &abs);
    max.x = abs.maximum;
    min.x = abs.minimum;

    ioctl(handle, EVIOCGABS(ABS_Y), &abs);
    max.y = abs.maximum;
    min.y = abs.minimum;
  }
  else
  {
    ioctl(handle, EVIOCGABS(ABS_MT_POSITION_X), &abs);
    max.x = abs.maximum;
    min.x = abs.minimum;

    ioctl(handle, EVIOCGABS(ABS_MT_POSITION_Y), &abs);
    max.y = abs.maximum;
    min.y = abs.minimum;
  }

  std::cout
    << "X: " << min.x << " - " << max.x << " R: " << res.x << std::endl
    << "Y: " << min.y << " - " << max.y << " R: " << res.y << std::endl;
}

TouchScreen::~TouchScreen()
{
  close();
}

void TouchScreen::close()
{
  if (handle >= 0)
    ::close(handle);

  handle = -1;
}

void TouchScreen::readEvents()
{
  if (handle < 0)
    throw TPGException("touch not opened");

  constexpr const unsigned EventBufferSize = 64;

  struct input_event events[EventBufferSize];

  auto eventCount = read(handle, events, sizeof (struct input_event) * EventBufferSize);
  if (int(eventCount) > 0)
  {
    unsigned count = unsigned(eventCount) / sizeof (input_event);
    for (unsigned i = 0; i < count; i++)
    {
      processEvent(events[i]);
    }
  }
}

void TouchScreen::processEvent(const struct input_event& event)
{
  switch (event.type)
  {
      /*case EV_SYN:
        std::cout << "start Event" << std::endl;
        break;*/
    case EV_KEY:
      if (event.code == BTN_TOUCH)
      {
        if (event.value)
          touchEvent(TouchEvent::Touch);
          //std::cout << "start Touch" << std::endl;
        else
          touchEvent(TouchEvent::Untouch);
        //std::cout << "end   Touch" << std::endl;
      }
      break;
    case EV_ABS:
      if (event.value > 0)
        switch (event.code)
        {
            // for any reason X/Y is swapped
          case ABS_Y:
            touch.x = res.x - res.x * (event.value - min.x) / (max.x - min.x);
            //std::cout << "X: " << event.value << ": " << touch.x << std::endl;
            break;
          case ABS_X:
            touch.y = res.y * (event.value - min.y) / (max.y - min.y);
            //std::cout << "Y: " << event.value << ": " << touch.y << std::endl;
            break;
          case ABS_PRESSURE:
            pressure = event.value;
            touchEvent(TouchEvent::Drag);
            //std::cout << '(' << touch.x << ',' << touch.y << ") P: " << event.value << std::endl;
            break;
        }
      break;
  }
}

void TouchScreen::touchEvent(TouchEvent touchEvent)
{
  int id, areaIndex;
  
  switch (touchEvent)
  {
    case TouchEvent::Touch:
      touchDown= true;
      break;
    case TouchEvent::Untouch:
      //std::cout << "Done " << lastAreaIndex << std::endl;
      
      id= lastAreaIndex < 0 ? -1 : areas[lastAreaIndex].id;
      callback(AreaTouch::Clicked, id);
      touchDown= false;
      lastAreaIndex= -1;
      break;
    case TouchEvent::Drag:      
      areaIndex= findCoordinate();
      //std::cout << "(" << touch.x << "," << touch.y << "):" << areaIndex << std::endl;
      if( areaIndex != lastAreaIndex)
      {
        //std::cout << "leave " << lastAreaIndex << " enter " << areaIndex << std::endl;
        
        id= lastAreaIndex < 0 ? -1 : areas[lastAreaIndex].id;
        callback(AreaTouch::Leave, id);
        lastAreaIndex= areaIndex;
        
        id= lastAreaIndex < 0 ? -1 : areas[lastAreaIndex].id;
        callback(AreaTouch::Enter, id);
      }
      break;
  }
}

int TouchScreen::findCoordinate()
{
  for(auto it= areas.begin();it != areas.end();it++)
  {
    if( touch.x >= it->x && touch.x < it->x + it->w 
      && touch.y >= it->y && touch.y < it->y + it->h)
      return it - areas.begin();
  }
  
  return -1;
}
