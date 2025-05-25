#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RED     5     // LED rosu pe D5
#define GREEN   6     // LED verde pe D6
#define BLUE    9     // LED albastru pe D9
#define BUTTON  2     // Buton pentru incrementare zile
#define BUZZER  3     // Buzzer pasiv pe D3
#define SENSOR  A0    // Senzor MQ135 pe A0

LiquidCrystal_I2C lcd(0x27, 16, 2);

int zile = 0;
bool lastButtonState = HIGH;

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  // Detectare apasare buton (flank cazator)
  bool currentButtonState = digitalRead(BUTTON);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    zile++;
    if (zile > 14) {
      zile = 0; // reset după 15 apăsări
      Serial.println("Reset zile!");
    } else {
      Serial.print("Zile incrementate: ");
      Serial.println(zile);
    }
    delay(300); // debounce
  }
  lastButtonState = currentButtonState;

  // Feedback LED RGB în funcție de numărul de zile
  if (zile < 10) {
    setColor(0, 255, 0); // verde
    noTone(BUZZER);
  } else if (zile < 14) {
    setColor(255, 165, 0); // portocaliu
    noTone(BUZZER);
  } else {
    setColor(255, 0, 0);   // roșu
    tone(BUZZER, 2000);
  }

  // Afișare pe LCD
  int airQuality = analogRead(SENSOR);
  lcd.setCursor(0, 0);
  lcd.print("Zile: ");
  lcd.print(zile);
  lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd.print("Aer: ");
  lcd.print(airQuality);
  lcd.print("   ");

  Serial.print("Zile: ");
  Serial.print(zile);
  Serial.print(" | Aer: ");
  Serial.println(airQuality);

  delay(300);
}

void setColor(int r, int g, int b) {
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}
