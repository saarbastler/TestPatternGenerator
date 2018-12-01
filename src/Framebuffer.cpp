/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Framebuffer.cpp
 * Author: Joerg
 * 
 * Created on 29. November 2018, 18:10
 */

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <stdint.h>

#include "Framebuffer.h"
#include "TPGException.h"

Framebuffer::Framebuffer(const char *device)
{
  handle = open(device, O_RDWR);
  if (handle < 0)
    throw TPGException("unable to open framebuffer device '%s'", device);

  if (ioctl(handle, FBIOGET_FSCREENINFO, &screeninfo) < 0)
  {
    close();

    throw TPGException("unable to get Screeninfo for framebuffer device '%s'", device);
  }
  if (ioctl(handle, FBIOGET_VSCREENINFO, &varScreeninfo) < 0)
  {
    close();

    throw TPGException("unable to get VScreeninfo for framebuffer device '%s'", device);
  }

  memory = (uint8_t*) mmap(0, screeninfo.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, handle, 0);
  if (memory == MAP_FAILED)
  {
    close();

    throw TPGException("unable to get VScreeninfo for framebuffer device '%s'", device);
  }
}

Framebuffer::~Framebuffer()
{
  close();
}

void Framebuffer::close()
{
  if (saveMemory != nullptr)
    delete saveMemory;

  saveMemory = nullptr;

  if (memory != nullptr && memory != MAP_FAILED)
    munmap(memory, screeninfo.smem_len);

  memory = nullptr;

  if (handle != -1)
    ::close(handle);

  handle = -1;
}

void Framebuffer::rectangle(unsigned x1, unsigned y1, unsigned x2, unsigned y2, uint16_t color)
{
  if (x2 < x1)
    std::swap(x1, x2);
  if (y2 < y1)
    std::swap(y1, y2);

  for (unsigned x = x1; x < x2; x++)
  {
    setPixel(x, y1, color);
    setPixel(x, y2, color);
  }
  for (unsigned y = y1; y < y2; y++)
  {
    setPixel(x1, y, color);
    setPixel(x2, y, color);
  }
}

void Framebuffer::setPixel(unsigned x, unsigned y, uint16_t color)
{
  if (memory == nullptr)
    throw TPGException("Framebuffer not open");

  if (x < varScreeninfo.xres && y < varScreeninfo.yres)
  {
    unsigned pixelAddress = 2 * x + y * screeninfo.line_length;

    *(uint16_t*)&memory[pixelAddress] = color;
  }
}

void Framebuffer::rectangleFilled(unsigned x1, unsigned y1, unsigned x2, unsigned y2, uint16_t color)
{
  if (memory == nullptr)
    throw TPGException("Framebuffer not open");

  if (x2 < x1)
    std::swap(x1, x2);
  if (y2 < y1)
    std::swap(y1, y2);

  if (x2 < varScreeninfo.xres && y2 < varScreeninfo.yres)
  {
    unsigned pixelAddress = 2 * x1 + y1 * screeninfo.line_length;

    for (unsigned y = y1; y <= y2; y++)
    {
      uint16_t *ptr = (uint16_t*) & memory[pixelAddress];
      pixelAddress += screeninfo.line_length;

      for (unsigned x = x1; x <= x2; x++)
        *ptr++ = color;
    }
  }
}

void Framebuffer::saveScreen()
{
  if (memory == nullptr)
    throw TPGException("Framebuffer not open");

  if (saveMemory == nullptr)
    saveMemory = new uint8_t [screeninfo.smem_len];

  if (saveMemory)
    memcpy(saveMemory, memory, screeninfo.smem_len);
}

void Framebuffer::restoreScreen()
{
  if (memory == nullptr)
    throw TPGException("Framebuffer not open");

  if (saveMemory)
    memcpy(memory, saveMemory, screeninfo.smem_len);
}

void Framebuffer::drawSprite(unsigned x, unsigned y, const uint16_t* data, unsigned width, unsigned height, bool reverse)
{
  if (memory == nullptr)
    throw TPGException("Framebuffer not open");

  if (x < varScreeninfo.xres && y < varScreeninfo.yres)
  {
    unsigned pixelAddress = 2 * x + y * screeninfo.line_length;

    for (unsigned i=0; y+i < varScreeninfo.yres && i < height; y++, height--)
    {
      uint16_t *ptr = (uint16_t*) & memory[pixelAddress];
      pixelAddress += screeninfo.line_length;

      if (reverse)
      {
        data += width;
        const uint16_t *source = data;
        for (unsigned i = 0; i < width; i++)
          if (i+x < varScreeninfo.xres)
            *ptr++ = *--source;
      }
      else
      {
        for (unsigned i = 0; i < width; i++)
          if (i+x < varScreeninfo.xres)
            *ptr++ = *data++;
      }
    }
  }
}
