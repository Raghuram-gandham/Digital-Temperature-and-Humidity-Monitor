#include <LiquidCrystal.h>
#include <DHT.h>

// LCD Pins
const int RS = 7;
const int EN = 6;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;

// DHT11
const int DHTPIN = 8;
#define DHTTYPE DHT11

// Pushbutton
const int BUTTON_PIN = 9;


LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
DHT dht(DHTPIN, DHTTYPE);


bool showFahrenheit = false;
bool lastButtonState = HIGH;
bool currentButtonState;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

unsigned long lastUpdate = 0;
unsigned long updateInterval = 2000;

void setup()
{
    lcd.begin(16,2);
    dht.begin();

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    lcd.clear();
    lcd.print("Temp Humidity");
    lcd.setCursor(0,1);
    lcd.print("Monitor Start");
    delay(2000);
}

void loop()
{
    handleButton();

    if(millis() - lastUpdate >= updateInterval)
    {
        lastUpdate = millis();
        updateDisplay();
    }
}

void handleButton()
{
    static bool previousState = HIGH;

    bool currentState = digitalRead(BUTTON_PIN);

    // detect new press
    if(previousState == HIGH && currentState == LOW)
    {
        showFahrenheit = !showFahrenheit;
        delay(250); // simple debounce
    }

    previousState = currentState;
}

void updateDisplay()
{
    float humidity = dht.readHumidity();
    float tempC = dht.readTemperature();

    if (isnan(humidity) || isnan(tempC))
    {
        lcd.clear();
        lcd.print("Sensor Error");
        return;
    }

    lcd.clear();

    // First Line: Temperature
    lcd.setCursor(0,0);

    if(showFahrenheit)
    {
        float tempF = tempC * 9.0 / 5.0 + 32.0;

        lcd.print("Temp:");
        lcd.print(tempF,1);
        lcd.print((char)223);
        lcd.print("F");
    }
    else
    {
        lcd.print("Temp:");
        lcd.print(tempC,1);
        lcd.print((char)223);
        lcd.print("C");
    }

    // Second Line: Humidity
    lcd.setCursor(0,1);
    lcd.print("Hum:");
    lcd.print(humidity,1);
    lcd.print("%");
}