#ifndef __ESP32_GCIOTC_HTTP_H__
#define __ESP32_GCIOTC_HTTP_H__

#include <Client.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <HTTPClient.h>

#include <base64.h>

#include <CloudIoTCore.h>
#include <CloudIoTCoreMqtt.h>
#include "ciotc_config.h" // Update this file with your configuration

// Cloud IoT Core parameters
CloudIoTCoreDevice *device;
unsigned long iss = 0;
String JWT; // JSON Web Token for server authorization

String pub_sub_url; // URL which the POST request is sent to

void getURL() 
{
  String _location = "/locations/" + String(location);
  String _registry = "/registries/" + String(registry_id);
  String _device = "/devices/" + String(device_id);
  String _url = "https://cloudiotdevice.googleapis.com/v1/projects/" + String(project_id);
  _url = _url + _location;
  _url = _url + _registry;
  _url = _url + _device;
  _url = _url + ":publishEvent";
  pub_sub_url = _url;
}

// Renew the JWT
void getJWT()
{
  iss = time(nullptr);
  Serial.println("Refreshing JWT");
  JWT = device->createJWT(iss, JWT_exp_secs);
}

// Check if the JWT is expired
void verifyJWT() {
  if (time(nullptr) - iss > JWT_exp_secs) {
    getJWT();
  }
}

// Setup WiFi connection with the parameters defined in ciotc_config.h
void setupWifi()
{
  Serial.println("Starting wifi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
  }

  configTime(0, 0, ntp_primary, ntp_secondary);
  Serial.println("Waiting on time sync...");
  while (time(nullptr) < 1510644967)
  {
    delay(10);
  }
}

void connectWifi()
{
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
}

// Publishing data via http POST request
void publishTelemetry(String data)
{
  HTTPClient http;
  http.begin(pub_sub_url); // Specify destination for HTTP request

  http.addHeader("authorization", "Bearer " + JWT); // Authorization header containing the JWT
  http.addHeader("content-type", "application/json");
  http.addHeader("cache-control", "no-cache");
  int httpResponseCode = http.POST("{\"binary_data\": \"" + base64::encode(data) + "\"}");  // Actual POST request. Binary data has to be Base64 encoded

  Serial.println("Publishing telemetry data via HTTP...");
  Serial.print("Payload: ");
  Serial.println(data);

  if (httpResponseCode > 0)
  {
    String response = http.getString(); // Get the response to the request
    Serial.print(httpResponseCode); // Print return code
    Serial.println(": Message sent.");
    Serial.print("Answer from server: ");
    Serial.println(response);         // Print request answer
  }
  else
  {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }

  http.end(); //Free resources
}

// Cloud IoT Setup
void setupCloudIoT()
{
  device = new CloudIoTCoreDevice(
      project_id, location, registry_id, device_id,
      private_key_str);
  getURL();
  setupWifi();
  getJWT();
}
#endif //__ESP32_GCIOTC_HTTP_H__
