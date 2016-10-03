#include "Software\SonarDriver.h"
#include <LiquidCrystal_I2C.h>
#include "Sonar.h"

//SCL: Pin A5
//SDA: Pin A4

#define I2C_ADDR 0x27
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);
SonarDriver sonarDriver(new Sonar());

void setup()
{
	lcd.begin(16, 2);

	lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
	lcd.setBacklight(HIGH);
}

void loop()
{
	lcd.home();
	lcd.print("Distance: ");

	sonarDriver.updateDist();

	if (sonarDriver.getDist() < 10)
		lcd.print(0);

	lcd.print(sonarDriver.getDist());
	lcd.print(" cm.");

	lcd.setCursor(0, 15);
	if (sonarDriver.isObstacle())
	{
		lcd.print("STOP!!!");
	}
	else
	{
		lcd.print("       ");
	}
}