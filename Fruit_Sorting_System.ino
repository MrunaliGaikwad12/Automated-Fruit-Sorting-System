#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// TCS3200 Color Sensor Pins
#define S0 18
#define S1 19
#define S2 25
#define S3 26
#define sensorOut 23

// IR Sensor
#define IR_SENSOR 4

// Servo Motor
#define SERVO_PIN 13

Servo sorter;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

int fruitCount = 0;

void setup()
{
  Serial.begin(115200);

  lcd.begin();      
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Fruit Sorting");
  lcd.setCursor(0,1);
  lcd.print("System Ready");

  delay(2000);
  lcd.clear();

  // Color sensor pins
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // IR sensor
  pinMode(IR_SENSOR, INPUT);

  // Set color sensor frequency scaling (20%)
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Servo setup
  sorter.setPeriodHertz(50);
  sorter.attach(SERVO_PIN, 500, 2400);

  sorter.write(90);
}

// Function to read color
int readColor(int s2, int s3)
{
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);

  delay(100);

  return pulseIn(sensorOut, LOW);
}

void loop()
{
  int irValue = digitalRead(IR_SENSOR);

  if(irValue == HIGH)
  {
    fruitCount++;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Fruit Detected");
    lcd.setCursor(0,1);
    lcd.print("Count:");
    lcd.print(fruitCount);

    Serial.println("Fruit Detected");

    // Read colors
    redValue = readColor(LOW, LOW);
    greenValue = readColor(HIGH, HIGH);
    blueValue = readColor(LOW, HIGH);

    Serial.print("Red: ");
    Serial.print(redValue);
    Serial.print(" Green: ");
    Serial.print(greenValue);
    Serial.print(" Blue: ");
    Serial.println(blueValue);

    delay(1000);

    if(redValue < greenValue && redValue < blueValue)
    {
      Serial.println("Red Fruit -> Bin 1");

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Color: RED");
      lcd.setCursor(0,1);
      lcd.print("Bin 1");

      sorter.write(180);
    }

    else if(greenValue < redValue && greenValue < blueValue)
    {
      Serial.println("Green Fruit -> Bin 2");

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Color: GREEN");
      lcd.setCursor(0,1);
      lcd.print("Bin 2");

      sorter.write(90);
    }

    else
    {
      Serial.println("Other Fruit -> Bin 3");

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Color: OTHER");
      lcd.setCursor(0,1);
      lcd.print("Bin 3");

      sorter.write(30);
    }

    delay(2000);

    sorter.write(90);

    delay(1000);
  }
} is it 