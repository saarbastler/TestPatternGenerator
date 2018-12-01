/*
 * GNU General Public License v3.0
 * File:   TouchScreen.h
 * Author: Saarbastler, joerg@saarbastler.de
 *
 * Created on 29. November 2018, 18:20
 */

#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <linux/input.h>

class TouchScreen
{
public:
  
  /**
   * opens the touchscreen device, typically /dev/input/event0
   * try cat /proc/bus/input/devices
   * @param device the device name
   */
  TouchScreen(const char *device, unsigned xres, unsigned yres);
  virtual ~TouchScreen();

  void readEvents();
  void processEvent(const struct input_event& event);
  
private:

  void close();
  
  int handle= -1;
  
  struct Coords
  {
    int x = 0;
    int y = 0;
  };
  
  Coords min, max;
  Coords res, touch;
  
  int pressure;
};

#endif /* TOUCHSCREEN_H */

