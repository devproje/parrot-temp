#include <DHT.h>

#define DHT_PIN 2
#define DHT_TYPE DHT11

#define TEMP_1 8
#define TEMP_2 7
#define TEMP_3 6

#define BOO 4
#define BOO_OFF 3

bool checked = false;

DHT dht(DHT_PIN, DHT_TYPE);

void siren()
{
	Serial.println("siren detected");

	digitalWrite(BOO, 1);
	delay(1000);

	digitalWrite(BOO, 0);
	delay(300);
}

void check_temp(float temp, float humi)
{
	if (temp >= 30.0) {
		digitalWrite(TEMP_1, 1);
		checked = 0;
	}
	
	if (temp >= 36.0) {
		digitalWrite(TEMP_2, 1);
		checked = 0;
	}

	if (temp >= 37.5) {
		digitalWrite(TEMP_3, 1);
		checked = 0;
	}
}

void setup()
{
	Serial.begin(19200);
	dht.begin();
	
	pinMode(BOO, OUTPUT);
	pinMode(BOO_OFF, INPUT_PULLUP);

	pinMode(TEMP_1, OUTPUT);
	pinMode(TEMP_2, OUTPUT);
	pinMode(TEMP_3, OUTPUT);

	delay(2000);
}

void loop()
{
	float humidity = dht.readHumidity();
	float temperature = dht.readTemperature();

	if (isnan(humidity) || isnan(temperature)) {
		Serial.println("cannot read temperature from sensor");
		return;
	}

	digitalWrite(TEMP_1, 0);
	digitalWrite(TEMP_2, 0);
	digitalWrite(TEMP_3, 0);

	int signal = digitalRead(BOO_OFF);
	if ((temperature < 30.0 || temperature > 38.0) && !checked)
		siren();

	if (signal == 0)
		goto off_siren;

	check_temp(temperature, humidity);

	Serial.print("temperature: ");
	Serial.print(temperature);
	Serial.print("°C | humidity: ");
	Serial.print(humidity);
	Serial.print("% | ");
	Serial.print("btn: ");
	Serial.print(signal);
	Serial.print(" | ");
	Serial.print("checked: ");
	Serial.println(checked);

	delay(300);

off_siren:
	Serial.println("siren off");
	checked = true;
	delay(10);
}
