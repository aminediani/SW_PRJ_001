#include <Arduino.h>
// #include "display7seg_manager.h"
#include "LCD_manager.h"
#include "rfid_manager.h"
#include <avr/wdt.h>

#define TEST_DELAY 500

// Function declaration
void dig7seg_test();
void crystal_Test();
void I2C_Scanner(int address);
void RFID_test();
void ledStatus(String LED_COLOR, bool LED_STAT);
void updatButtonStatus();
void Freq_Meter();
void Tag_reset(int sold);
void streamSold();

static int counter = 0;
bool tagPresence = 0;
bool startButton = false;
bool stopButton = false;
String RFID_ID = "";
int Fuel_Solde = 0;
bool user_access = 0, Vann_state = 0;
int Liquid_Flow = 0;
bool recheck_new_tag = 1;
int startPin = 5;
int buzzer_Pin = A2, Red_LED_PIN = 3, Green_LED_PIN = 6, VANN_Relay_Pin = 7;
int stopPin = 5;
const byte FlowMeterPin = 2;
int count = 0;
int bFlag = 0;
int sold = 5;
int pushpin = A0;
bool good_value = 0;
int good_val_err = 0;
// Setup function
void setup()
{
	

	pinMode(pushpin, INPUT);
	Serial.begin(9600);
	Serial.println("Start...");
	init_LCD();
	// init_7seg();
	init_RFID();
	Serial.println("Init Done...");
	showText("   E-BlueTek    ","  Tag  Charger  ");
	delay(1500);
}

// Main Loop
void loop()
{
	delay(50);
	// streamSold();
	showTextLine1("Set Sold " + String(sold) + " L");
	if (digitalRead(pushpin))
	{

		sold = sold + 5;
		if (sold >= 25)
			sold = 5;
		// showTextLine1("Set Sold " + String(sold) + " L");
		delay(20);
	}
	Tag_reset(sold);

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

void Tag_reset(int sold)
{
	bool tagPresence = 0, succes_operat;
	halt_RFID();
	delay(50);
	recheck_new_tag = 0;
	tagPresence = check_new_Card_Presence();

	if (tagPresence)
	{
		while (!digitalRead(pushpin)|| !good_value)
		{
			if (tagPresence == 1)
			{
				Serial.println("tag detected");
				RFID_ID = getID_RFID();

				int value_of_read = getValue_RFID();
				if (value_of_read == 99)
				{
					// error state

					Serial.println("99 Error state");
					{
						tagPresence = 0;
						recheck_new_tag = 1;
						user_access = 0;
						good_value = 0;
					}
				}
				else
				{
					Serial.println("GOOD VALUE");
					good_value = 1;
					Fuel_Solde = value_of_read;
					showText("Set Sold " + String(sold) + " L", "Sold :" + String((float)Fuel_Solde / 1000) + " L");
				}

				// int valueSold = (float)Fuel_Solde / 1000;
			}

			if ((!tagPresence) || recheck_new_tag)
			{

				Serial.println("check new tag");
				halt_RFID();
				tagPresence = check_new_Card_Presence();
				if (tagPresence == 1)
					recheck_new_tag = 0;
				
				showTextLine1("Set Sold " + String(sold) + " L");
			}

			delay(20);
		}

		// Serial.println("Tag is present ");
		// Serial.println(getID_RFID());
		// Serial.println(getData_RFID());
		//  Serial.print("the returned value is = ");
		//  Serial.println(getValue_RFID());
		showTextLine1("WAIT...");
		delay(1000);
		succes_operat = setData_RFID(sold * 1000);
		int write_counter = 0;
		while (((!succes_operat) && write_counter <= 3)||write_counter == 0)
		{

			if (succes_operat)
			{
				showTextLine1("Tag is charged");
				delay(1000);
				break;
			}
			else
			{
				showTextLine1("Error Try again!");
				succes_operat = setData_RFID(sold * 1000);
				Serial.println("Try..." + String(write_counter));
				if (!succes_operat)
					write_counter++;
				// if(write_counter <= 10)
				delay(100);
				// wdt_disable();
				// wdt_enable(WDTO_15MS);
				// while (1)
				// {
				// }
			}
		}

		if (succes_operat)
		{
			showText("Tag is charged","New Sold "+String(sold)+" L");
			delay(2500);
		}
		else
		{
			showText("Error Try again!","Wait...");
			delay(1000);
			// wdt_disable();
			// wdt_enable(WDTO_15MS);
			// while (1)
			// {
			// }
		}
	}
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

void streamSold()
{

	if (!tagPresence)
	{
		user_access = 0;
		Serial.println("tag not detected");
	}

	// check_new_Card_Presence();
	if ((!tagPresence) || recheck_new_tag)
	{

		Serial.println("check new tag");
		halt_RFID();
		tagPresence = check_new_Card_Presence();
		if (tagPresence == 1)
			recheck_new_tag = 0;
	}

	Serial.println("check presnec //" + String(tagPresence));

	if (tagPresence == 1)
	{
		Serial.println("tag detected");
		RFID_ID = getID_RFID();

		int value_of_read = getValue_RFID();
		if (value_of_read == 99)
		{
			// error state

			Serial.println("99 Error state");
			{
				tagPresence = 0;
				recheck_new_tag = 1;
				user_access = 0;
				return;
			}
		}
		else
		{
			Serial.println("GOOD VALUE");
			Fuel_Solde = value_of_read;
			showTextLine2("Acctual Sold :" + String((float)Fuel_Solde / 1000) + " L");
		}

		// int valueSold = (float)Fuel_Solde / 1000;
	}
}
