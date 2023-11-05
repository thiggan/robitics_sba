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
  String ptr = "{ ";

  char c = GPS.read();
  if (GPS.newNMEAreceived())
  {
    if (GPS.parse(GPS.lastNMEA()))
    {
      if (GPS.fix)
      {
        ptr += "\"latitude\":"; ptr += GPS.latitudeDegrees; ptr += ", ";
        ptr += "\"longitude\":"; ptr += GPS.longitudeDegrees; ptr += ", ";
        ptr += "\"altitude\":"; ptr += GPS.altitude; ptr += ", ";
        ptr += "\"speed\":"; ptr += GPS.speed; ptr += ", ";
        ptr += "\"angle\":"; ptr += GPS.angle; ptr += ", ";
        ptr += "\"satellites\":"; ptr += GPS.satellites; ptr += ", ";
        ptr += "\"fixType\":"; ptr += GPS.fix; ptr += ", ";
        ptr += "\"fixTypeText\":\""; ptr += GPS.fix == 3 ? "3D Fix" : (GPS.fix == 2 ? "2D Fix" : "No Fix"); ptr += "\", ";
        ptr += "\"year\":"; ptr += GPS.year; ptr += ", ";
        ptr += "\"month\":"; ptr += GPS.month; ptr += ", ";
        ptr += "\"day\":"; ptr += GPS.day; ptr += ", ";
        ptr += "\"hour\":"; ptr += GPS.hour; ptr += ", ";
        ptr += "\"minute\":"; ptr += GPS.minute; ptr += ", ";
        ptr += "\"seconds\":"; ptr += GPS.seconds; ptr += " ";

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
        ptr += "\"err\":\"no gps fix\" ";
      }
    }
    else
    {
      ptr += "\"err\":\"no last NMEA\" ";
    }
  }
  else
  {
    ptr += "\"err\":\"no new NMEA Recieved\" ";
  }
  ptr += "}";

  Serial.println(ptr);
  delay(500);
}
