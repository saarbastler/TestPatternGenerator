#! /bin/sh
### BEGIN INIT INFO
# Provides: noip
# Required-Start: $syslog
# Required-Stop: $syslog
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: noip server
# Description:
### END INIT INFO
 
case "$1" in
    start)
        echo "tpg is starting"
        # Starting Programm
        sudo /home/pi/tpg/tpg /dev/fb1 /dev/input/event0 /home/pi/tpg
        ;;
    stop)
        echo "tpg is ending"
        # Ending Programm
        killall tpg
        ;;
    *)
        echo "Use: /etc/init.d/tpg {start|stop}"
        exit 1
        ;;
esac
 
exit 0
