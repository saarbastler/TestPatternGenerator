# TestPatternGenerator
Raspberry PI Image Test Pattern Generator

Using a 480x320 64 Color LCD Image Files are visualized on the LCD and the HDMI/Video Output.
The touchscreen allows to switch between images and shutdown the PI.

Sourcecode in src, a compiled binary in tpg directory.

# Installation

* Install the latest Raspbian Stretch lite
* Install the Display driver as described by the manufacturer
* Calibrate the touch as described by the manufacturer
* Install fbi: sudo apt-get install fbi
* copy the tpg directory to /home/pi/tpg
* copy some sample files to /home/pi/tpg
* cd /home/pi/tpg
* sudo chmod +x show.sh
* sudo chmod +x tpg
* test using a testimage: sudo ./show.sh image-file.jpg the image should be visible on LCD and HDMI/Video
* start tpg using: sudo /home/pi/tpg/tpg /dev/fb1 /dev/input/event0 /home/pi/tpg

# Autostart

* sudo cp tpg.sh /etc/init.d/tpg
* sudo chmod +x /etc/init.d/tpg
* sudo cp autologin.conf /etc/systemd/system/getty@tty1.service.d/
* sudo reboot
* edit sudo nano /etc/profile
* add this line at the end: sudo /etc/init.d/tpg start
* sudo reboot

# Manual

Touching the screen will open a menu:

![Menu](/img/screen.jpg)

The arrows will show the next or previous *.jpg file on LCD and HDMI/Video. 
Tapping the Off Button twice will shutdown the PI.
