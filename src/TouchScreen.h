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
#include <vector>
#include <functional>

class TouchScreen
{
public:

  enum class AreaTouch
  {
    Enter, Leave, Clicked
  };
  
  using Callback= std::function<void(AreaTouch,int)>;
  
  /**
   * opens the touchscreen device, typically /dev/input/event0
   * try cat /proc/bus/input/devices
   * @param device the device name
   */
  TouchScreen(const char *device, unsigned xres, unsigned yres, Callback&& callback);
  virtual ~TouchScreen();

  struct TouchArea
  {
    int id;
    unsigned x;
    unsigned y;
    unsigned w;
    unsigned h;
  };
  
  void addArea(TouchArea&& touchArea)
  {
    areas.emplace_back(touchArea);
  }
  
  void readEvents();
  
private:

  /**
   * close the input device
   */
  void close();
  
  /**
   * process a single event
   * @param event the single input event to process
   */
  void processEvent(const struct input_event& event);
  
  enum class TouchEvent
  {
    Touch, Untouch, Drag
  };
  
  /**
   * Called by processEvent, if a Touch Event is recognized. 
   * In case of a Drag, the touch variable contains the coordinates.
   * @param touchEvent the event type
   */
  void touchEvent(TouchEvent touchEvent);
  
  /**
   * Find the coordinate stored in touch in the areas
   * @return the areas index, if found, or -1, if not
   */
  int findCoordinate();
  
  int handle= -1;
  
  struct Coords
  {
    int x = 0;
    int y = 0;
  };
  
  Coords min, max;
  Coords res, touch;
  
  int pressure;
  
  std::vector<TouchArea> areas;
  
  int lastAreaIndex= -1;
  bool touchDown= false;
  
  Callback callback;
};

#endif /* TOUCHSCREEN_H */

