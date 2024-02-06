#ifndef WATCHY_7_SEG_H
#define WATCHY_7_SEG_H
#include <math.h>
#include <cmath>
#include <vector>
#include <Watchy.h>
#include "Dusk2Dawn.h"
#include "icons.h"
#include "moonPhaser.h"

class Watchy7SEG : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        void drawTime();
        void drawDate();
        void drawSteps();
        void drawBattery();
        void drawBg();
        void drawSun();
        void drawMoon();
        void drawAngle();

        void drawNuma(int a, int b, int c);
        void drawNumb(int a, int b, int c);
        void drawNumc(int a, int b, int c);
        void drawWeek(int a, int b, String c);
        void drawMonth(String c);
        virtual void handleButtonPress();
        double CalculateHourAngle(double longitude, double dayOfYear, double timeZone, double hours, double minutes);
        double CalculateDeclinationAngle(double dayOfYear);
        double CalculateElevationAngle(double latitude, double longitude, double dayOfYear, double timeZone, double hours, double minutes);
        double CalculateAzimuth(double latitude, double longitude, double dayOfYear, double timeZone, double hours, double minutes);



};



#endif
