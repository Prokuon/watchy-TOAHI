#include "Watchy_7_SEG.h"

// change it to your location 
#define Latit_SET 31.00
#define Longit_SET 121.00
#define TimeZone_SET 8

RTC_DATA_ATTR bool DARKMODE = false;
RTC_DATA_ATTR int showState = 0;
RTC_DATA_ATTR bool HOUR_SET = true;

moonPhaser moonP;


void Watchy7SEG::handleButtonPress()
{
    if (guiState == WATCHFACE_STATE)
    {
        uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
        if (wakeupBit & UP_BTN_MASK) 
        {
            showState++;
            if (showState > 2) { showState = 0; }
            RTC.read(currentTime);
            showWatchFace(true);
            return;
        }
        if (wakeupBit & DOWN_BTN_MASK) 
        {
            showState--;
            if (showState < 0) { showState = 2; }
            RTC.read(currentTime);
            showWatchFace(true);
            return;
        }
        if (wakeupBit & BACK_BTN_MASK) 
        {
            DARKMODE = !DARKMODE;
            RTC.read(currentTime);
            showWatchFace(true);
            return;
        } 
        if (wakeupBit & MENU_BTN_MASK) 
        {
            Watchy::handleButtonPress();
            return;
        }
    } 
    else {Watchy::handleButtonPress();}
    return;
}

void Watchy7SEG::drawWatchFace()
{
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    // drawHands();
    drawBg ();
    drawTime();
    drawDate();
    drawSteps();
    drawBattery();
    display.drawBitmap(135, 103, WIFI_CONFIGURED ? wifion : wifioff, 25, 20, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.drawBitmap(182, 105, BLE_CONFIGURED ? bleon : bleoff, 10, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    drawSun();
    drawMoon();
    drawAngle();
}

void Watchy7SEG::drawBg()
{
    if (showState == 0)
        display.drawBitmap(0, 0, background0, 200, 200, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    if (showState == 1)
        display.drawBitmap(0, 0, background1, 200, 200, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    if (showState == 2)
        display.drawBitmap(0, 0, background2, 200, 200, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

void Watchy7SEG::drawTime()
{
    long ss = currentTime.Hour * 60 + currentTime.Minute;
    // long l5 = 33 * ss / (60 * 24);

    // display.fillRect(158, 52, l5, 7, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);


    int sh = ss / 60;

    // // PM
    // if (HOUR_SET == false && sh >= 12)
    //     display.drawBitmap(26, 112, PM, 9, 5, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    // // AM
    // else if (HOUR_SET == false && sh < 12)
    //     display.drawBitmap(26, 95, AM, 9, 5, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    int sk = sh;
    // adjust hour to 21-hour
    if (HOUR_SET == false && sh > 12)
        sk = sh - 12;

    int sm = ss % 60;
    int a = sk >= 10 ? sk / 10 : 0;
    int b = sk % 10;
    int c = sm >= 10 ? sm / 10 : 0;
    int d = sm % 10;

    drawNuma(6, 6, a);
    drawNuma(70, 6, b);
    drawNuma(6, 70, c);
    drawNuma(70, 70, d);
}


void Watchy7SEG::drawDate()
{

    String dayOfWeek = dayStr(currentTime.Wday);
    drawWeek(133, 69, dayOfWeek);
    display.drawBitmap(165, 69, yao, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    String month = monthShortStr(currentTime.Month);

    drawMonth(month);

    int da = currentTime.Day;
    int a = da / 10;
    int b = da % 10;
    drawNumc(101, 133, a);
    drawNumc(116, 133, b);

    if (showState == 0 || showState == 1)
    {
        int ye = currentTime.Year + 1970;

        int c = ye / 1000;
        ye = ye % 1000;
        int d = ye / 100;
        ye = ye % 100;
        int e = ye / 10;
        ye = ye % 10;
        int f = ye;
        drawNumc(5, 148, c);
        drawNumc(20, 148, d);
        drawNumc(37, 148, e);
        drawNumc(52, 148, f);
    }
}


void Watchy7SEG::drawSteps()
{
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0)
    {
        sensor.resetStepCounter();
    }
    uint32_t stepCount = sensor.getCounter();

    uint32_t l5 = 57 * stepCount / 6000;

    if (l5 > 57)
    {
        l5 = 57;
    }

    display.fillRect(135, 54, l5, 10, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    if (showState == 0)
    {

        // display.
        if (stepCount >= 10000)
            stepCount = 9999;
        // int a = stepCount / 10000;
        stepCount = stepCount % 10000;
        int b = stepCount / 1000;
        stepCount = stepCount % 1000;
        int c = stepCount / 100;
        stepCount = stepCount % 100;
        int d = stepCount / 10;
        int e = stepCount % 10;

            // drawNumb(27, 26, a);
        drawNumb(5, 165, b);
        drawNumb(37, 165, c);
        drawNumb(69, 165, d);
        drawNumb(101, 165, e);
    }

}

void Watchy7SEG::drawBattery()
{
    int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
    if(VBAT > 4.1)
    {
        batteryLevel = 57;
    }
    else if(VBAT > 4.05 && VBAT <= 4.1)
    {
        batteryLevel = 50;
    }
    else if(VBAT > 4 && VBAT <= 4.05)
    {
        batteryLevel = 43;
    }

    else if(VBAT > 3.95 && VBAT <= 4)
    {
        batteryLevel = 37;
    }
    else if(VBAT > 3.9 && VBAT <= 3.95)
    {
        batteryLevel = 30;
    }
    else if(VBAT > 3.85 && VBAT <= 3.9)
    {
        batteryLevel = 23;
    }
    else if(VBAT > 3.8 && VBAT <= 3.85)
    {
        batteryLevel = 15;
    }
    else if(VBAT > 3.75 && VBAT <= 3.8)
    {
        batteryLevel = 8;
    }
    else if(VBAT <= 3.75)
    {
        batteryLevel = 0;
    }


    display.fillRect(135, 22, batteryLevel, 10, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

void Watchy7SEG::drawSun() 
{
    Dusk2Dawn location(Latit_SET, Longit_SET, TimeZone_SET);
    int year = currentTime.Year + 1970;
    int32_t month = currentTime.Month;
    int32_t day = currentTime.Day;
    int sr = location.sunrise(year, month, day, false);
    int ss = location.sunset(year, month, day, false);

    // int lft = 33 * sr / 24 / 60;
    // int rgt = 33 * ss / 24 / 60;

    // display.fillRect(158 + lft, 60, rgt - lft, 3, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    int rh = sr / 60;
    int rm = sr % 60;
    int sh = ss / 60;
    int sm = ss % 60;

    if (HOUR_SET == false && rh > 12)
        rh -= 12;

    if (HOUR_SET == false && sh >12)
        sh -= 12;

    int a = sh >= 10 ? sh / 10 : 0;
    int b = sh % 10;
    int c = sm >= 10 ? sm / 10 : 0;
    int d = sm % 10;


    int e = rh >= 10 ? rh / 10 : 0;
    int f = rh % 10;
    int g = rm >= 10 ? rm / 10 : 0;
    int h = rm % 10;

    if (showState == 1)
    {
        drawNumc(69, 165, a);
        drawNumc(84, 165, b);
        drawNumc(101, 165, c);
        drawNumc(116, 165, d);

        drawNumc(5, 180, e);
        drawNumc(20, 180, f);
        drawNumc(37, 180, g);
        drawNumc(52, 180, h);
    }
}

void Watchy7SEG::drawMoon() 
{
    moonData_t moon;                        // variable to receive the data
    
    int year = currentTime.Year + 1970;
    int32_t month = currentTime.Month;
    int32_t day = currentTime.Day;
    double hour = currentTime.Hour + 0.1;

    moon = moonP.getPhase(year, month, day, hour);

    int ag = moon.angle;
    double lt = moon.percentLit;

    if (ag <= 180)
    {
        if (lt < 0.1)
            display.drawBitmap(133, 133, luna1, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (lt < 0.25)
            display.drawBitmap(133, 133, luna12, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (lt < 0.4)
            display.drawBitmap(133, 133, luna11, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (lt < 0.6)
            display.drawBitmap(133, 133, luna10, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (lt < 0.75)
            display.drawBitmap(133, 133, luna9, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (lt < 0.9)
            display.drawBitmap(133, 133, luna8, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else 
            display.drawBitmap(133, 133, luna7, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else
    {
        if (lt < 0.1)
            display.drawBitmap(133, 133, luna1, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (lt < 0.25)
            display.drawBitmap(133, 133, luna2, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (lt < 0.4)
            display.drawBitmap(133, 133, luna3, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (lt < 0.6)
            display.drawBitmap(133, 133, luna4, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (lt < 0.75)
            display.drawBitmap(133, 133, luna5, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (lt < 0.9)
            display.drawBitmap(133, 133, luna6, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else 
            display.drawBitmap(133, 133, luna7, 61, 61, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void Watchy7SEG::drawAngle()
{
    String c = monthShortStr(currentTime.Month);
    int mn = 1;
    // drawMonth(month);
    if (c == "Jan") mn = 1;
    else if (c == "Feb") mn = 2;
    else if (c == "Mar") mn = 3;
    else if (c == "Apr") mn = 4;
    else if (c == "May") mn = 5;
    else if (c == "Jun") mn = 6;
    else if (c == "Jul") mn = 7;
    else if (c == "Aug") mn = 8;
    else if (c == "Sep") mn = 9;
    else if (c == "Oct") mn = 10;
    else if (c == "Nov") mn = 11;
    else if (c == "Dec") mn = 12;

    int da = currentTime.Day;
    int ye = currentTime.Year + 1970;


    std::vector<int> vec = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

    int y = ye;
    int m = mn;
    int d = da;
    double res = 0.0;
    if (y % 4 == 0 && m > 2)
        if (y != 1900)
            res++;
    res += vec[m - 1];
    res += d;

    if (showState == 2)
    {
        int phi = CalculateElevationAngle(Latit_SET, Longit_SET, res, TimeZone_SET, currentTime.Hour, currentTime.Minute);
        int theta = CalculateAzimuth(Latit_SET, Longit_SET, res, TimeZone_SET, currentTime.Hour, currentTime.Minute);

        if (theta <= 11.25 || theta > 348.75)
            display.fillRect(34, 133, 3, 6, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 11.25 && theta <= 33.75)
            display.fillRect(45, 135, 3, 4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 33.75 && theta <= 56.25)
            display.fillRect(53, 142, 4, 4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 56.25 && theta <= 78.75)
            display.fillRect(61, 151, 3, 3, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 78.75 && theta <= 101.25)
            display.fillRect(61, 162, 5, 3, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 101.25 && theta <= 123.75)
            display.fillRect(61, 173, 3, 3, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 123.75 && theta <= 146.25)
            display.fillRect(53, 181, 4, 4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 146.25 && theta <= 168.75)
            display.fillRect(45, 188, 3, 4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 168.75 && theta <= 191.25)
            display.fillRect(34, 188, 3, 6, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 191.25 && theta <= 213.75)
            display.fillRect(23, 188, 3, 4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 213.75 && theta <= 236.25)
            display.fillRect(14, 181, 4, 4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 236.25 && theta <= 258.75)
            display.fillRect(7, 173, 3, 3, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 258.75 && theta <= 281.25)
            display.fillRect(5, 162, 5, 3, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 281.25 && theta <= 303.75)
            display.fillRect(7, 151, 3, 3, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 303.75 && theta <= 326.25)
            display.fillRect(14, 142, 4, 4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (theta > 326.25 && theta <= 348.75)
            display.fillRect(23, 135, 3, 4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

        if (phi >= 0 && theta <= 180)
            display.drawBitmap(69, 165, arr1, 7, 7, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (phi >= 0 && theta > 180)
            display.drawBitmap(69, 165, arr2, 7, 7, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (phi < 0 && theta < 360)
            display.drawBitmap(69, 165, arr3, 7, 7, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (phi < 0 && theta >= 0)
            display.drawBitmap(69, 165, arr4, 7, 7, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);


        if (phi < 0)
            phi = -phi;

        if (phi >= 0 && phi <= 11.25)
            display.drawLine(69, 193, 71, 193, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (phi > 11.25 && phi <= 33.75)
            display.drawLine(72, 183, 75, 184, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (phi > 33.75 && phi <= 56.25)
            display.drawLine(78, 174, 80, 176, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (phi > 56.25 && phi <= 78.75)
            display.drawLine(87, 168, 88, 171, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        else if (phi > 78.75 && phi <= 90)
            display.drawLine(97, 165, 97, 167, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        


        int a = phi / 10;
        int b = phi % 10;
        int c = theta / 100;
        theta %= 100;
        int d = theta / 10;
        int e = theta % 10;

        drawNumc(13, 149, c);
        drawNumc(29, 149, d);
        drawNumc(44, 149, e);
        drawNumc(101, 165, a);
        drawNumc(116, 165, b);
    }
}

void Watchy7SEG::drawNuma(int a, int b, int c) 
{
    if (c == 0)
        display.drawBitmap(a, b, numa0, 59, 59, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 1)
        display.drawBitmap(a, b, numa1, 59, 59, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 2)
        display.drawBitmap(a, b, numa2, 59, 59, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 3)
        display.drawBitmap(a, b, numa3, 59, 59, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 4)
        display.drawBitmap(a, b, numa4, 59, 59, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 5)
        display.drawBitmap(a, b, numa5, 59, 59, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 6)
        display.drawBitmap(a, b, numa6, 59, 59, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 7)
        display.drawBitmap(a, b, numa7, 59, 59, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 8)
        display.drawBitmap(a, b, numa8, 59, 59, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 9)
        display.drawBitmap(a, b, numa9, 59, 59, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

void Watchy7SEG::drawNumb(int a, int b, int c)
{
    if (c == 0)
        display.drawBitmap(a, b, numb0, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 1)
        display.drawBitmap(a, b, numb1, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 2)
        display.drawBitmap(a, b, numb2, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 3)
        display.drawBitmap(a, b, numb3, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 4)
        display.drawBitmap(a, b, numb4, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 5)
        display.drawBitmap(a, b, numb5, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 6)
        display.drawBitmap(a, b, numb6, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 7)
        display.drawBitmap(a, b, numb7, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 8)
        display.drawBitmap(a, b, numb8, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 9)
        display.drawBitmap(a, b, numb9, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

void Watchy7SEG::drawWeek(int a, int b, String c)
{
    if (c == "Monday")
        display.drawBitmap(a, b, monday, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == "Tuesday")
        display.drawBitmap(a, b, tuesday, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == "Wednesday")
        display.drawBitmap(a, b, wednesday, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == "Thursday")
        display.drawBitmap(a, b, thursday, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == "Friday")
        display.drawBitmap(a, b, friday, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == "Saturday")
        display.drawBitmap(a, b, saturday, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == "Sunday")
        display.drawBitmap(a, b, sunday, 29, 29, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

void Watchy7SEG::drawMonth(String c)
{
    if (c == "Jan")
    {
        display.drawBitmap(69, 133, numc0, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc1, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else if (c == "Feb")
    {
        display.drawBitmap(69, 133, numc0, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc2, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else if (c == "Mar")
    {
        display.drawBitmap(69, 133, numc0, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc3, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else if (c == "Apr")
    {
        display.drawBitmap(69, 133, numc0, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc4, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else if (c == "May")
    {
        display.drawBitmap(69, 133, numc0, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc5, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else if (c == "Jun")
    {
        display.drawBitmap(69, 133, numc0, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc6, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else if (c == "Jul")
    {
        display.drawBitmap(69, 133, numc0, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc7, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else if (c == "Aug")
    {
        display.drawBitmap(69, 133, numc0, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc8, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else if (c == "Sep")
    {
        display.drawBitmap(69, 133, numc0, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc9, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else if (c == "Oct")
    {
        display.drawBitmap(69, 133, numc1, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc0, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else if (c == "Nov")
    {
        display.drawBitmap(69, 133, numc1, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc1, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
    else if (c == "Dec")
    {
        display.drawBitmap(69, 133, numc1, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
        display.drawBitmap(84, 133, numc2, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void Watchy7SEG::drawNumc(int a, int b, int c)
{
    if (c == 0)
        display.drawBitmap(a, b, numc0, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 1)
        display.drawBitmap(a, b, numc1, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 2)
        display.drawBitmap(a, b, numc2, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 3)
        display.drawBitmap(a, b, numc3, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 4)
        display.drawBitmap(a, b, numc4, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 5)
        display.drawBitmap(a, b, numc5, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 6)
        display.drawBitmap(a, b, numc6, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 7)
        display.drawBitmap(a, b, numc7, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 8)
        display.drawBitmap(a, b, numc8, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    else if (c == 9)
        display.drawBitmap(a, b, numc9, 14, 14, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

double Watchy7SEG::CalculateHourAngle(double longitude, double dayOfYear, double timeZone, double hours, double minutes)
{
    double LSTM = 15 * timeZone;

    double B = 360.0 / 365.0 * (dayOfYear - 81) * PI / 180;
    double EoT = 9.87 * sin(2 * B) - 7.53 * cos(B) - 1.5 * sin(B);

    double TC = 4 * (longitude - LSTM) + EoT;

    double LST = (hours + minutes / 60) + TC / 60;

    double HRA = 15 * (LST - 12);

    return HRA;
}

double Watchy7SEG::CalculateDeclinationAngle(double dayOfYear)
{
    return -23.44 * cos(360.0 / 365.0 * (dayOfYear + 10.0) * PI / 180);
}

double Watchy7SEG::CalculateElevationAngle(double latitude, double longitude, double dayOfYear, double timeZone, double hours, double minutes)
{
    double delta = CalculateDeclinationAngle(dayOfYear) * PI / 180;
    double phi = latitude * PI / 180;
    double HRA = CalculateHourAngle(longitude, dayOfYear, timeZone, hours, minutes) * PI / 180;

    double elevation_angle = asin(sin(delta) * sin(phi) + cos(delta) * cos(phi) * cos(HRA));

    return elevation_angle * 180 / PI;
}

double Watchy7SEG::CalculateAzimuth(double latitude, double longitude, double dayOfYear, double timeZone, double hours, double minutes)
{
    double delta = CalculateDeclinationAngle(dayOfYear) * PI / 180;
    double phi = latitude * PI / 180;
    double HRA = CalculateHourAngle(longitude, dayOfYear, timeZone, hours, minutes) * PI / 180;
    double alpha = CalculateElevationAngle(latitude, longitude, dayOfYear, timeZone, hours, minutes) * PI / 180;

    double azimuth = acos((sin(delta) * cos(phi) - cos(delta) * sin(phi) * cos(HRA)) / cos(alpha));

    if (HRA > 0)
        azimuth = 2 * PI - azimuth;

    return azimuth * 180 / PI;
}