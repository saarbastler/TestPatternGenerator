/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FramebufferException.h
 * Author: Joerg
 *
 * Created on 29. November 2018, 18:20
 */

#ifndef FRAMEBUFFEREXCEPTION_H
#define FRAMEBUFFEREXCEPTION_H

#include <exception>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <linux/fb.h>

class FramebufferException : public std::exception
{
public:
  
  FramebufferException(const char *format, ...)
  {
    va_list args;
    va_start(args, format);
    char buffer[1024];

    std::vsprintf(buffer, format, args);
    va_end(args);

    text = buffer;
  }

  virtual const char* what() const noexcept
  {
    return text.c_str();
  }

private:

  std::string text;
};

#endif /* FRAMEBUFFEREXCEPTION_H */

