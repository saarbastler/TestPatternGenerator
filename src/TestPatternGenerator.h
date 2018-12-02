/*
 * GNU General Public License v3.0
 * File:   TestPatternGenerator.h
 * Author: Saarbastler, joerg@saarbastler.de
 *
 * Created on 29. November 2018, 18:20
 */

#ifndef TESTPATTERNGENERATOR_H
#define TESTPATTERNGENERATOR_H

#include <vector>

#include "Framebuffer.h"
#include "TouchScreen.h"

class TestPatternGenerator
{
public:
  TestPatternGenerator(const char *frameBufferDevice, const char *touchDevice, const char *path);

  //virtual ~TestPatternGenerator();

  TouchScreen &touchScreen()
  {
    return touch;
  }

private:

  void touchCalback(TouchScreen::AreaTouch touch, int areaId);

  void paintMenu();

  Framebuffer framebuffer;
  TouchScreen touch;

  struct SpriteDef
  {
    const uint16_t *data;
    unsigned width;
    unsigned height;
    bool reverse;
    unsigned xPaint;
    unsigned yPaint;
  };

  static std::array<SpriteDef, 3> sprites;

  bool menuVisible = false;

  void findJpgFiles();

  void showPicture(unsigned index);
  void shutdown();
  
  std::vector<std::string> files;
  
  int pictureIndex = 0;
  int shutdownCount = 0;

  std::string path;  
};

#endif /* TESTPATTERNGENERATOR_H */

