#include <Arduino.h>
#include "display7seg_manager.h"
#include "LCD_manager.h"
#include "rfid_manager.h"

#define TEST_DELAY 500

// Function declaration
void dig7seg_test();
void crystal_Test();
void I2C_Scanner(int address);
void RFID_test();
void ledStatus(String LED_COLOR, bool LED_STAT);
void updatButtonStatus();
void Freq_Meter();

static int counter = 0;
bool tagPresence = 0;
bool startButton = false;
bool stopButton = false;
String RFID_ID="";
int Fuel_Solde = 0;
bool user_access = 0,Vann_state = 0;
int Liquid_Flow = 0;


int startPin = 4;
int buzzer_Pin = A2, Red_LED_PIN = 3, Green_LED_PIN = 6, VANN_Relay_Pin = 7;
int stopPin = 5;
const byte FlowMeterPin = 2;
int count=0;
int bFlag = 0;
// Setup function
void setup()
{
	pinMode(Red_LED_PIN, OUTPUT);
	pinMode(Green_LED_PIN, OUTPUT);	 //
	pinMode(buzzer_Pin, OUTPUT);	 // BUZZER
	pinMode(VANN_Relay_Pin, OUTPUT); // RELAY
	pinMode(startPin, INPUT);
	pinMode(stopPin, INPUT);

	digitalWrite(buzzer_Pin, HIGH);
	delay(50);
	digitalWrite(buzzer_Pin, LOW);


	pinMode(FlowMeterPin, INPUT_PULLUP);
  	attachInterrupt(digitalPinToInterrupt(FlowMeterPin), Freq_Meter, RISING);

	Serial.begin(9600);
	Serial.println("Start...");
	init_LCD();
	init_7seg();
	init_RFID();
	Serial.println("Init Done...");
}

// Main Loop
void loop()
{
	showTextLine1("Put your Tag");

	tagPresence = check_new_Card_Presence();

	if (tagPresence == 1)
	{
		showTextLine1("Tag detected");


		RFID_ID = getID_RFID();
		Fuel_Solde = getValue_RFID();();
		if (Fuel_Solde > 0)
		{
			user_access = 1;
		}
		else
		{
			user_access = 0;
		}
	}

	if (user_access == 1)
	{

		showTextLine1("Fuel = " + String(Fuel_Solde));
		if (Vann_state == HIGH)
		{
			digitalWrite(Red_LED_PIN, LOW);
			digitalWrite(Green_LED_PIN, HIGH);
		}

		while (Fuel_Solde > 0 && Vann_state == HIGH)
		{
			digitalWrite(VANN_Relay_Pin, HIGH);
			Liquid_Flow = count*2;
			Fuel_Solde = Fuel_Solde - Liquid_Flow;
			delay(300);
			showValue7seg(Liquid_Flow);
			setData_RFID(Fuel_Solde);
			if (Fuel_Solde <= 0)
			{
				break;
			}
		}

		if (Vann_state == LOW)
		{
			Vanne_Controle = LOW;
		}
	
		if (digitalRead(startPin) )
		{
			Vann_state = HIGH;
		}
		if (digitalRead(stopButton) )
		{
			Vann_state = LOW;
		}
	}

	// updatButtonStatus();
	// ledStatus("green",startButton);
	// ledStatus("red",stopButton);

	// Serial.println("loop...");
	// delay(300);
	// dig7seg_test();
	// crystal_Test();
	// RFID_test();
}
void RFID_test()
{
	bool tagPresence = 0;

	tagPresence = check_new_Card_Presence();
	if (tagPresence)
	{
		digitalWrite(A2, HIGH);
		delay(100);
		digitalWrite(A2, LOW);
		Serial.println("Tag is present ");
		Serial.println(getID_RFID());
		Serial.println(getData_RFID());
		Serial.print("the returned value is = ");
		Serial.println(getValue_RFID());

		delay(1000);
		setData_RFID(10999);
	}
}

// digital 7 seg Test
void dig7seg_test()
{
	float value = 12.34f + ((float)counter) * 0.1;
	showValue7seg(value);
	// delay(TEST_DELAY);
	//  clear7seg();
	//  delay(TEST_DELAY);
}

// LCD crystal test
void crystal_Test()
{
	showTextLine1("CNT = " + String(counter));
	// delay(TEST_DELAY);
	counter++;
}

void Freq_Meter() {
  count +=1;
}
// I2C
void I2C_Scanner(int address)
{

	int error;
	Wire.begin();
	Wire.beginTransmission(0x27);
	error = Wire.endTransmission();
	Serial.print("Error: ");
	Serial.print(error);

	if (error == 0)
	{
		Serial.println(": LCD found.");
	}
	else
	{
		Serial.println(": LCD not found.");
	} // if
}

void ledStatus(String LED_COLOR, bool LED_STAT)
{
	if (LED_COLOR == "green")
	{
		digitalWrite(3, LED_STAT);
	}
	if (LED_COLOR == "red")
	{
		digitalWrite(6, LED_STAT);
	}
}

void updatButtonStatus()
{
	if (digitalRead(startPin) && bFlag == 0)
	{
		// bFlag = 1;
		startButton = !startButton;
	}
	if (digitalRead(stopPin) && bFlag == 0)
	{
		// bFlag = 1;
		stopButton = !stopButton;
	}
}