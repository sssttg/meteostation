#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Порядок строго с воскресенья: dayOfTheWeek() возвращает 0 = Sunday
const char* const days[] = {
  "Sun", "Mon", "Tue", "Wed",
  "Thu", "Fri", "Sat"
};

char buf[17];          // 16 символов строки + завершающий ноль
int8_t lastSecond = -1;

// функция для вывода стартого экрана
void home_screen() {
  lcd.setCursor(1, 0);
  lcd.print("Weather Station");
  lcd.setCursor(12, 1);
  lcd.print("v1.0");
}
void clear_screen() {
  lcd.clear();
}

void time_screen() {
  DateTime now = rtc.now();
  // перерисовка времени при смене секунды
  if (now.second() != lastSecond) {
    lastSecond = now.second();

  // строка 1: дата и время
  sprintf(buf, "%02d.%02d.%04d",
          now.day(), now.month(), now.year());
  lcd.setCursor(0, 0);
  lcd.print(buf);

  // строка 2: день недели и секунды
  sprintf(buf, "%-3s     %02d:%02d:%02d",
          days[now.dayOfTheWeek()], now.hour(), 
          now.minute(), now.second());
  lcd.setCursor(0, 1);
  lcd.print(buf);
  }
}

void main_screen() {

}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  // 2 пин для кнопки
  pinMode(2, INPUT_PULLUP);
  // проверка на то есть ли часы
  if (!rtc.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("not found");
    while(1);
  }
  // проверка на потеря связи с часами
  if (rtc.lostPower()) {
    lcd.setCursor(0, 0);
    lcd.print(F("Time was reset"));
    // Установка времени. После первой прошивки закомментируйте
    // строку ниже и залейте скетч заново.
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    delay(1500);
    lcd.clear();
  } 
  rtc.disable32K();
  rtc.writeSqwPinMode(DS3231_OFF);
  home_screen();
  delay(2000);
  clear_screen();
}

void loop() {
  // put your main code here, to run repeatedly:
  time_screen();
}
