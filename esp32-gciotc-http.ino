#include "esp32-gciotc-http.h"

String message = "Hello Google Cloud IoT Core";

const int publishing_interval = 10000;

void setup()
{
  Serial.begin(115200);
  setupCloudIoT();
}

unsigned long lastMillis = 0;
void loop()
{
  // Publish a message every x milliseconds. The interval can be changed in ciotc_config.h
  if (millis() - lastMillis > publishing_interval)
  {
    lastMillis = millis();
    publishTelemetry(message);
  }
}
