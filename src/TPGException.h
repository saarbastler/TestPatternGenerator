/*
 * GNU General Public License v3.0
 * File:   Test Pattern Generator Exception.h
 * Author: Saarbastler, joerg@saarbastler.de
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

class TPGException : public std::exception
{
public:
  
  TPGException(const char *format, ...)
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

