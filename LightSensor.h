#define LIGHT_SENSOR_PIN A0

float resistance;
const int threshold = 10;
bool isLightOn = false;

void readLightSensor() {
    int sensor_value = analogRead(LIGHT_SENSOR_PIN);

    // Calculate resistance of sensor in K
    resistance = (float)(1023 - sensor_value) * 10 / sensor_value;

    if (resistance > threshold)
    {
       isLightOn = true;
    } 
    else 
    {
       Serial.println("Light is off");
       isLightOn = false;
    }
}
