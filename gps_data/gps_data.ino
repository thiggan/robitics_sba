#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>

SoftwareSerial mySerial(10, 11); // RX, TX
Adafruit_GPS GPS(&mySerial);

void setup() {
  Serial.begin(9600);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  delay(1000);
}

void loop() {
  char c = GPS.read();
  if (GPS.newNMEAreceived()) {
    if (GPS.parse(GPS.lastNMEA())) {
      if (GPS.fix) {
        float latitude = GPS.latitudeDegrees;
        float longitude = GPS.longitudeDegrees;
        float altitude = GPS.altitude;
        float speed = GPS.speed;
        float heading = GPS.angle;
        int satellites = GPS.satellites;
        int fixType = GPS.fix;
        int year = GPS.year;
        int month = GPS.month;
        int day = GPS.day;
        int hour = GPS.hour;
        int minute = GPS.minute;
        int second = GPS.seconds;

        

        Serial.print("Latitude: "); Serial.println(latitude, 6);
        Serial.print("Longitude: "); Serial.println(longitude, 6);
        Serial.print("Altitude (meters): "); Serial.println(altitude);
        Serial.print("Speed (knots): "); Serial.println(speed);
        Serial.print("Heading (degrees): "); Serial.println(heading);
        Serial.print("Number of Satellites: "); Serial.println(satellites);
        Serial.print("Fix Type: "); Serial.println(fixType == 3 ? "3D Fix" : (fixType == 2 ? "2D Fix" : "No Fix"));
        Serial.print("Date (YYYY/MM/DD): "); Serial.print(year); Serial.print("/"); Serial.print(month); Serial.print("/"); Serial.println(day);
        Serial.print("Time (HH:MM:SS): "); Serial.print(hour); Serial.print(":"); Serial.print(minute); Serial.print(":"); Serial.println(second);
        Serial.println("---------------");
      } else {
        Serial.println("No GPS.fix");
      }
    } else {
      Serial.println("No lastNMEA");
    }
  } else {
    Serial.println("No newNMEAreceived");
  }
}
