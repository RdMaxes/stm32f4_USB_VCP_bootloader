set mem inaccessible-by-default off
target extended-remote /dev/ttyACM1
mon swdp_scan
attach 1
