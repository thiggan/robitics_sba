#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>

SoftwareSerial mySerial(7, 8); // RX, TX
Adafruit_GPS GPS(&mySerial);

void setup()
{
  Serial.begin(9600);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  delay(1000);
}

void loop()
{
  String ptr = "{";

  char c = GPS.read();
  if (GPS.newNMEAreceived())
  {
    if (GPS.parse(GPS.lastNMEA()))
    {
      if (GPS.fix)
      {
        ptr += "\t'latitude':"; ptr += GPS.latitudeDegrees; ptr += ",\r";
        ptr += "\t'longitude':"; ptr += GPS.longitudeDegrees; ptr += ",\r";
        ptr += "\t'altitude':"; ptr += GPS.altitude; ptr += ",\r";
        ptr += "\t'speed':"; ptr += GPS.speed; ptr += ",\r";
        ptr += "\t'angle':"; ptr += GPS.angle; ptr += ",\r";
        ptr += "\t'satellites':"; ptr += GPS.satellites; ptr += ",\r";
        ptr += "\t'fixType':"; ptr += GPS.fix; ptr += ",\r";
        ptr += "\t'fixTypeText':'"; ptr += GPS.fix == 3 ? "3D Fix" : (GPS.fix == 2 ? "2D Fix" : "No Fix"); ptr += "',\r";
        ptr += "\t'year':"; ptr += GPS.year; ptr += ",\r";
        ptr += "\t'month':"; ptr += GPS.month; ptr += ",\r";
        ptr += "\t'day':"; ptr += GPS.day; ptr += ",\r";
        ptr += "\t'hour':"; ptr += GPS.hour; ptr += ",\r";
        ptr += "\t'minute':"; ptr += GPS.minute; ptr += ",\r";
        ptr += "\t'seconds':"; ptr += GPS.seconds; ptr += "\r";

        // Serial.print("Latitude: "); Serial.println(latitude, 6);
        // Serial.print("Longitude: "); Serial.println(longitude, 6);
        // Serial.print("Altitude (meters): "); Serial.println(altitude);
        // Serial.print("Speed (knots): "); Serial.println(speed);
        // Serial.print("Heading (degrees): "); Serial.println(heading);
        // Serial.print("Number of Satellites: "); Serial.println(satellites);
        // Serial.print("Fix Type: "); Serial.println(fixType == 3 ? "3D Fix" : (fixType == 2 ? "2D Fix" : "No Fix"));
        // Serial.print("Date (YYYY/MM/DD): "); Serial.print(year); Serial.print("/"); Serial.print(month); Serial.print("/"); Serial.println(day);
        // Serial.print("Time (HH:MM:SS): "); Serial.print(hour); Serial.print(":"); Serial.print(minute); Serial.print(":"); Serial.println(second);
      }
      else
      {
        ptr += "'err':{ 'msg':'no gps fix ' }\r";
      }
    }
    else
    {
      ptr += "'err':{ 'msg':'no last NMEA' }\r";
    }
  }
  else
  {
    ptr += "'err':{ 'msg':'no new NMEA Recieved' }\r";
  }
  ptr += "}\n";

  Serial.println(ptr);
  delay(1000);
}
