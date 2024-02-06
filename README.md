# watchy-TOAHI

TOAHI watchface for watchy

This watchface is primarily inspired by the UI and font designs of the fictional company "TOA Heavy Industries" created by Japanese manga artist Tsutomu Nihei.

* step count
* sunrise/sunset time
* moonphase
* solar azimuth/elevation angles

![picture](/assets/pic.png)

# settings

* To toggle between Dark Mode/Light Mode, push back button
* To toggle to sunrise/sunset time, push down button
* To toggle to azimut/eleuation angle, push up button
* To calculate exact sunrise/sunset time and azimut/eleuation angle, change `#define Latit_SET 31.00 #define Longit_SET 121.00 #define TimeZone_SET 8` in `Watchy_7_SEG.cpp`, the three data represents latitude, longitude and timezone
* To set the GMT offset for network timing, change `#define GMT_OFFSET_SEC 3600 * 8` in `settings.h`

# Credits

* [moonPhase-esp32](https://github.com/CelliesProjects/moonPhase-esp32) 

  Utilized code from this repository to calculate lunar phases

* [Dusk2Dawn](https://github.com/dmkishi/Dusk2Dawn)

  Utilized code from this repository to calculate sunrise and sunset time

* [sun-angle-calculator](https://github.com/sandervanthul/sun-angle-calculator)

  Utilized code from this repository to calculate solar azimut and elevation angles
