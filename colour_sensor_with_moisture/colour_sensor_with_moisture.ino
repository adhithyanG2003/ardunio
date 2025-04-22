#include <Wire.h>  
#include <LiquidCrystal_I2C.h> 
#define soil A0 
LiquidCrystal_I2C lcd(0x27, 16, 2); 
const int s0 = 8;
const int s1 = 9;
const int s2 = 11;
const int s3 = 12;
const int out = 10;

int red = 0; 
int green = 0;
int blue = 0;
int value=0;
void setup() {
  Serial.begin(9600);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);

  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME    ");
  lcd.setCursor(0, 1);
  lcd.print("    EEE=A     ");
  delay(1900);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" SOIL HEALTH   ");
  lcd.setCursor(0, 1);
  lcd.print("  MONITORING    ");
  delay(1900);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AUTOMATED CROP");
  lcd.setCursor(0, 1);
  lcd.print("DISEASE DETECT");
  delay(1900);
  lcd.clear();
}
void loop() {
  value = analogRead(soil);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;
  Serial.println("\n moisture=");
  Serial.println(value);
  color();
  Serial.print("R =");
  Serial.print(red, DEC);
  Serial.print(" G = ");
  Serial.print(green, DEC);
  Serial.print(" B = ");
  Serial.print(blue, DEC);
  Serial.print("\t");

  if (red < blue && red < green && red < 25) {
    if (green - blue >= 10 && green - blue <= 25 && green - (2 * red) >= 8) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("LEAF IS AFFECTED:");
      lcd.setCursor(0, 1);
      lcd.print("  ADD NUTRIENTS  ");
      Serial.println(" - (Red Color)");
    } else if (green - red <= 10 && green - red >= -3 && blue >= green) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MOISTURE: ");
      lcd.print(value);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("P GROWTH:");
      lcd.print("POOR");
      Serial.println(" - (Yellow Color)");
    } else if (blue - red >= 3 && blue - red <= 10 && green - (2 * red) <= 5) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ADD SOME:");
      lcd.setCursor(0, 1);
      lcd.print("    FERTILIZER");
      Serial.println(" - (Pink Color)");
    } else if (green - blue >= -5 && green - blue <= 5 && green - (2 * red) <= 5) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MOISTURE: ");
      lcd.print(value);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("GIVE SOME");
      lcd.print("WATER");
      Serial.println(" - (Orange Color)");
    }
  } else if (green < red && green < blue && green < 25) {
    lcd.clear();
    lcd.setCursor(0, 0);
      lcd.print("MOISTURE: ");
      lcd.print(value);
      lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("P GROWTH:");
    lcd.print("NORMAL");
    Serial.println(" - (Green Color)");
  } else if ((red > green && blue < green) && blue < 25 && red > 40) {
    lcd.clear();
    lcd.setCursor(0, 0);
      lcd.print("MOISTURE: ");
      lcd.print(value);
      lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("P GROWTH:");
    lcd.print("POOR");
    Serial.println(" - (Blue Color)");
  } else if (red - (2 * blue) >= -2 && red - (2 * blue) <= 5 && green - red < 10) {
    lcd.clear();
    lcd.setCursor(0, 0);
      lcd.print("MOISTURE: ");
      lcd.print(value);
      lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("P GROWTH:");
    lcd.print("POOR");
    Serial.println(" - (Purple Color)");
  } else if (blue < red && blue < green && (blue && red && green) < 25) {
    if (red - green <= 5 && red - green >= 0 && ((green - blue) || (red - blue)) < 5 && blue < 50) {
      lcd.clear();
      lcd.setCursor(0, 0);
       lcd.print("AFFECTED by :");
      lcd.setCursor(0, 1);
      lcd.print("     MEALYBUGS ");
      Serial.println(" - (White Color)");
    }
  }

  Serial.println();

  delay(300);
}

void color() {
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  //count OUT, pRed, RED
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  //count OUT, pBLUE, BLUE
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  //count OUT, pGreen, GREEN
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}
