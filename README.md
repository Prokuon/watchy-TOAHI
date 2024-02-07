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
* To toggle to azimut/elevation angle, push up button
* To calculate exact sunrise/sunset time and azimut/elevation angle, change `#define Latit_SET 31.00 #define Longit_SET 121.00 #define TimeZone_SET 8` in `Watchy_7_SEG.cpp`, the three data represents latitude, longitude and timezone
* To set the GMT offset for network timing, change `#define GMT_OFFSET_SEC 3600 * 8` in `settings.h`

# Extra Details

The watch face features a significant number of Japanese Kanji characters, which may make it difficult to understand

* 零 0 壱 1 弐 2 参 3 肆 4 伍 5 陸 6 漆 7 捌 8 玖 9
* 西暦年号 Year
* つき Month
* 日付 Day
* 曜日 Week
* 日曜日 Sunday 月曜日 Monday 火曜日 Tuesday 水曜日 Wednesday 木曜日 Thursday 金曜日 Friday 土曜日 Saturday 
* 電池残量 Battery Level
* 歩數統計 Step count
* 藍牙 Bluetooth
* 日出時刻 Sunrise time
* 日没時刻 Sunset time
* 方位角 Solar Azimut Angle
* 高度角 Solar elevation Angle

# Credits

* [moonPhase-esp32](https://github.com/CelliesProjects/moonPhase-esp32) 

  Utilized code from this repository to calculate lunar phases

* [Dusk2Dawn](https://github.com/dmkishi/Dusk2Dawn)

  Utilized code from this repository to calculate sunrise and sunset time

* [sun-angle-calculator](https://github.com/sandervanthul/sun-angle-calculator)

  Utilized code from this repository to calculate solar azimut and elevation angles