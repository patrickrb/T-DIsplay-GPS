#include <TFT_eSPI.h>
#include <TinyGPSPlus.h>
#include "HardwareSerial.h"
#include <DigitalRainAnimation.hpp>

TFT_eSPI tft = TFT_eSPI();
DigitalRainAnimation<TFT_eSPI> matrix_effect = DigitalRainAnimation<TFT_eSPI>();

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

// GPS
static const int RXPin = 18;
static const int TXPin = 17;
static const uint32_t GPSBaud = 9600;

void setup()
{
    Serial.begin(115200);
    Serial.println("\n\n TFT_eSPI Settings");

    SerialGPS.begin(GPSBaud, SERIAL_8N1, TXPin, RXPin);

    tft.begin();
    tft.setRotation(0);

    matrix_effect.init(&tft);
    matrix_effect.setup(
        15, /* Line Min */
        15, /* Line Max */
        24, /* Speed Min */
        25, /* Speed Max */
        60 /* Screen Update Interval */);
}

void displayInfo()
{
    matrix_effect.loop();

    Serial.print(F("Speed: "));
    if (gps.speed.isValid())
    {
        Serial.print(gps.speed.mph());
    }

    Serial.print(F("Location: "));
    if (gps.location.isValid())
    {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F("  Date/Time: "));
    if (gps.date.isValid())
    {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (gps.time.isValid())
    {
        if (gps.time.hour() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10)
            Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
    }
    else
    {
        Serial.print(F("INVALID"));
    }

    Serial.println();
}

void loop()
{
    while (SerialGPS.available())
    {
        int c = SerialGPS.read();
        if (gps.encode(c))
        {
            displayInfo();
        }
    }
    if (Serial.available())
    {
        Serial.write(Serial.read());
    }
    if (Serial.available())
    {
        Serial.write(Serial.read());
    }
}
