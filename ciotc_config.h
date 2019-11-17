// Update this file with your Google Cloud project's parameters

// Wifi newtork parameters
const char *ssid = "yourSSID";
const char *password = "yourPassword";

// Cloud IoT parameters
const char *project_id = "yourProjectID";
const char *location = "yourLocation";
const char *registry_id = "yourDeviceRegistry";
const char *device_id = "yourDevice";
const char *subfolder;  // Pub/Sub subfolder, not yet implemented

// Configuration for time synchronization
const char* ntp_primary = "pool.ntp.org";
const char* ntp_secondary = "time.nist.gov";


// Paste your private key here
const char *private_key_str =
    "aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:"
    "aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:aa:"
    "aa:aa";

// Time in seconds before the token expires and needs to be re-generated
const int JWT_exp_secs = 1200; 



