#include <Arduino.h>
#include "display7seg_manager.h"
#include "LCD_manager.h"
#include "rfid_manager.h"

#define TEST_DELAY 500
#define DL_1 200

// Function declaration
void dig7seg_test();
void crystal_Test();
void I2C_Scanner(int address);
void RFID_test();
void ledStatus(String LED_COLOR, bool LED_STAT);
void Freq_Meter();
void application_SW();

static int counter = 0;
bool tagPresence = 0, recheck_new_tag = 1;
int error_counter = 0;
// bool startButton = false;
// bool stopButton = false;
String RFID_ID = "";
int Fuel_Solde = 0;
bool user_access = 0, Vann_state = 0;
int Liquid_Flow = 0;
int saving_counter = 0;
int error_saving_counter = 1;

int startPin = 4;
int buzzer_Pin = A2, Red_LED_PIN = 6, Green_LED_PIN = 3, VANN_Relay_Pin = 7;
int stopPin = 5;
const byte FlowMeterPin = 2;
int count = 0;
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
	delay(150);
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
	application_SW();
	delay(DL_1);
	Serial.println("Counter = " + String(count));
	// if (count > 200)
	// 	count = 0;

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

		Serial.println("Tag is present ");
		Serial.println(getID_RFID());
		// Serial.println(getData_RFID());
		Serial.print("the returned value is = ");
		Serial.println(getValue_RFID());

		delay(DL_1);
		// setData_RFID(10999);
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

void Freq_Meter()
{
	count += 1;
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

void application_SW()
{
	Serial.println("-----");
	Serial.println("-----");
	Serial.println("-----");

	if (!tagPresence)
	{
		error_counter++;
		if(error_counter>3){
			showTextLine1("Put your Tag");
			clear7seg();
		}
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

	// 	if (digitalRead(stopPin))
	// {
	// Serial.println("pesent forced");
	// 	tagPresence = 1;
	// }

	if (tagPresence == 1)
	{
		// digitalWrite(buzzer_Pin, HIGH);
		// delay(50);
		// digitalWrite(buzzer_Pin, LOW);
		// showTextLine1("Tag detected");
		Serial.println("tag detected");
		RFID_ID = getID_RFID();

		int value_of_read = getValue_RFID();
		if (value_of_read == 99)
		{
			// error state
			
			Serial.println("99 Error state");
			//error_counter++;

			// halt_RFID();
			// tagPresence = check_new_Card_Presence();
			// if (tagPresence)
			// 	Serial.println("recovred");
			// else
			{
				tagPresence = 0;
				recheck_new_tag = 1;
				user_access = 0;
				if(!recheck_new_tag)
					showValue7seg(0.00f);
				return;
			}
		}
		else
		{
			error_counter=0;
			Serial.println("GOOD VALUE");
			Fuel_Solde = value_of_read;
		}

		showValue7seg((float)Fuel_Solde / 1000);
		if (Fuel_Solde > 0)
		{
			user_access = 1;
			Serial.println("- Get access");
		}
		else
		{
			Serial.println("- Block access");
			user_access = 0;
		}
	}

	if (user_access == 1)
	{
		showTextLine1("Sold = " + String(Fuel_Solde)+"mL");

		// change the state of electrovanne to high
		if (digitalRead(startPin))
		{
			Serial.println("vanne state High");
			Vann_state = HIGH;
		}

		// change the state of electrovanne to low
		if (digitalRead(stopPin))
		{
			Serial.println("vanne state Low");
			Vann_state = LOW;
		}

		if (Vann_state == HIGH)
		{
			digitalWrite(Red_LED_PIN, LOW);
			digitalWrite(Green_LED_PIN, HIGH);
		}
		else
		{
			digitalWrite(Red_LED_PIN, HIGH);
			digitalWrite(Green_LED_PIN, LOW);
		}

		while (Fuel_Solde > 0 && Vann_state == HIGH)
		{
			digitalWrite(VANN_Relay_Pin, HIGH);
			Liquid_Flow = count * 2.08;

			if (Fuel_Solde < Liquid_Flow)
			{
				Fuel_Solde = 0;
				Vann_state = LOW;
				digitalWrite(Red_LED_PIN, HIGH);
				digitalWrite(Green_LED_PIN, LOW);
			}
			else
				Fuel_Solde = Fuel_Solde - Liquid_Flow;

			showTextLine1("debit : " + String(Liquid_Flow));
			count = 0;
			showValue7seg(((float)Fuel_Solde) / 1000);
			saving_counter++;
			if (saving_counter > 10|| Fuel_Solde == 0 )
			{
				saving_counter = 0;
				error_saving_counter = setData_RFID(Fuel_Solde);
				Serial.println("data sync to tag/error: "+String(error_saving_counter));
				digitalWrite(buzzer_Pin, HIGH);
				delay(50);
				digitalWrite(buzzer_Pin, LOW);
			}


			if (digitalRead(stopPin)|| (!error_saving_counter))
			{
				error_saving_counter = 1;
				Serial.println("vanne state Low");
				Vann_state = LOW;
				digitalWrite(Red_LED_PIN, HIGH);
				digitalWrite(Green_LED_PIN, LOW);
			}
			delay(DL_1);
		}

		if (Vann_state == LOW)
		{
			digitalWrite(VANN_Relay_Pin, LOW);
		}
	}
}
