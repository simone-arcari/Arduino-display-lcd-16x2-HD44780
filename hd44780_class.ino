#include "HD44780.h"

#define rs 7
#define rw 8
#define en 9
#define d4 10
#define d5 11
#define d6 12
#define d7 13

LCD_HD44780 lcd(rs,rw,en,d4,d5,d6,d7);

void setup() {
  // put your setup code here, to run once:
    lcd.begin();
    lcd.printString("hello Arduino");
    lcd.putCommand(DISP_ON_CUR_ON_BLINK_ON_CMD);
    delay(3000);
    lcd.putCommand(DISP_ON_CUR_OFF_BLINK_OFF_CMD);
    lcd.putCommand(DISPLAY_CLEAR_CMD);
}

void loop() {
  // put your main code here, to run repeatedly:
    static int number = 101;
    if(number > 100) {
        lcd.putCommand(DISPLAY_CLEAR_CMD);
        lcd.putCommand(RETURN_HOME_CMD);
        lcd.printString("count example:");
        number = 0;
    }
    
    lcd.setCursor(1,0);
    lcd.printNumber(number);
    number++;
    delay(500);
}
