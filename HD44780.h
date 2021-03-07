#ifndef HD44780_h
#define HD44780_h

#define TOTAL_CHARACTERS_OF_LCD 32
#define LCD_LINE_LENGHT 16

#define IR 0
#define DR 1

// Display commands
#define DISPLAY_CLEAR_CMD               0b00000001  //  Clears display and set DDRAM address 0
#define RETURN_HOME_CMD                 0b00000010  //  DDRAM address 0, display in original position

#define ENTRY_MODE_DEC_NO_SHIFT_CMD     0b00000100  //  Entry mode decrement, no display shift
#define ENTRY_MODE_DEC_SHIFT_CMD        0b00000101  //  Entry mode decrement, display shift
#define ENTRY_MODE_INC_NO_SHIFT_CMD     0b00000110  //  Entry mode increment, no display shift
#define ENTRY_MODE_INC_SHIFT_CMD        0b00000111  //  Entry mode increment, display shift

#define DISP_OFF_CUR_OFF_BLINK_OFF_CMD  0b00001000  //  Display off, cursor off, blinking off
#define DISP_OFF_CUR_OFF_BLINK_ON_CMD   0b00001001  //  Display off, cursor off, blinking on
#define DISP_OFF_CUR_ON_BLINK_OFF_CMD   0b00001010  //  Display off, cursor on, blinking off
#define DISP_OFF_CUR_ON_BLINK_ON_CMD    0b00001011  //  Display off, cursor on, blinking on
#define DISP_ON_CUR_OFF_BLINK_OFF_CMD   0b00001100  //  Display on, cursor off, blinking off
#define DISP_ON_CUR_OFF_BLINK_ON_CMD    0b00001101  //  Display on, cursor off, blinking on
#define DISP_ON_CUR_ON_BLINK_OFF_CMD    0b00001110  //  Display on, cursor on, blinking off
#define DISP_ON_CUR_ON_BLINK_ON_CMD     0b00001111  //  Display on, cursor on, blinking on
  
#define CURSOR_MOVE_SHIFT_LEFT_CMD      0b00010000  //  Cursor move, shift to the left
#define CURSOR_MOVE_SHIFT_RIGHT_CMD     0b00011100  //  Cursor move, shift to the right
#define DISPLAY_MOVE_SHIFT_LEFT_CMD     0b00011000  //  Display move, shift to the left
#define DISPLAY_MOVE_SHIFT_RIGHT_CMD    0b00011100  //  Display move, shift to the right

#define FOUR_BIT_ONE_LINE_5x8_CMD       0b00100000  //  4 bit, 1 line, 5x8 font size
#define FOUR_BIT_ONE_LINE_5x10_CMD      0b00100100  //  4 bit, 1 line, 5x10 font size
#define FOUR_BIT_TWO_LINE_5x8_CMD       0b00101000  //  4 bit, 2 line, 5x8 font size
#define FOUR_BIT_TWO_LINE_5x10_CMD      0b00101100  //  4 bit, 2 line, 5x10 font size
#define EIGHT_BIT_ONE_LINE_5x8_CMD      0b00110000  //  8 bit, 1 line, 5x8 font size
#define EIGHT_BIT_ONE_LINE_5x10_CMD     0b00110100  //  8 bit, 1 line, 5x10 font size
#define EIGHT_BIT_TWO_LINE_5x8_CMD      0b00111000  //  8 bit, 2 line, 5x8 font size
#define EIGHT_BIT_TWO_LINE_5x10_CMD     0b00111100  //  8 bit, 2 line, 5x10 font size

#define CGRAM_ADDRESS(addr) (addr|=0x40)            //  CGRAM addressing
#define DDRAM_ADDRESS(addr) (addr|=0x80)            //  DDRAM addressing

#define NUM_TO_CODE(num)(num+0x30)                  //  0-9 ROM codes
#define CODE_TO_NUM(code)(code-0x30)                //  0-9 ROM num

class LCD_HD44780 {
    public:
        //attributes
        short int RegisterSelect;
        short int ReadWrite;
        short int Enable;
        short int DataLine4;
        short int DataLine5;
        short int DataLine6;
        short int DataLine7;

        //methods
        LCD_HD44780(short int,short int,short int,short int,short int,short int,short int); //costructor
        void begin(void);
        void printString(unsigned char*);
        void printNumber(int);
        void setCursor(short int,short int);
        void putCommand(short int);
        
    private:
        //methods
        void printChar(unsigned char);               
        void writeByte(short int,short int);
        void putCommand_hf(short int);
        void toggle();
        void sendUpperByte(short int);
        void sendLowerByte(short int);
        void clearDataLine(void);
        
};

LCD_HD44780::LCD_HD44780(short int rs, short int rw, short int en, short int d4, short int d5, short int d6, short int d7) {
/*   
 *     @description: This function allows you to set the attributes for an object in this class
 */
    RegisterSelect = rs;
    ReadWrite = rw;
    Enable = en;
    DataLine4 = d4;
    DataLine5 = d5;
    DataLine6 = d6;
    DataLine7 = d7;
}

void LCD_HD44780::begin(void) {
    pinMode(RegisterSelect, OUTPUT);
    pinMode(ReadWrite, OUTPUT);
    pinMode(Enable, OUTPUT);
    pinMode(DataLine4, OUTPUT);
    pinMode(DataLine5, OUTPUT);
    pinMode(DataLine6, OUTPUT);
    pinMode(DataLine7, OUTPUT);

    putCommand_hf(0x30); // sequence for intialization
    putCommand_hf(0x30); // ----
    putCommand_hf(0x20); // ----
    putCommand(FOUR_BIT_TWO_LINE_5x8_CMD);
    putCommand(DISP_ON_CUR_OFF_BLINK_OFF_CMD);
    putCommand(DISPLAY_CLEAR_CMD);
    putCommand(ENTRY_MODE_INC_NO_SHIFT_CMD);   
}

void LCD_HD44780::printString(unsigned char lineOfCharacters[TOTAL_CHARACTERS_OF_LCD]) {
/*    
 *     @description: This function allows you to print a string on the display
 */
    short int i = 0;
    while(lineOfCharacters[i] && i<TOTAL_CHARACTERS_OF_LCD) {
        printChar(lineOfCharacters[i]);
        i++;
    }
}

void LCD_HD44780::printNumber(int number) {
/*    
 *     @description: This function allows you to print a number on the display
 */
    short int index = 0, MAX_NUM_LEN = 10, temp;
    int digit[MAX_NUM_LEN];

    while(number != 0) {
        digit[index] = number%10;
        number/=10;
        index++;
    }

    if(index == 0) {
        printChar(NUM_TO_CODE(0)); //----print nuber zero
    }else {
        /*array inversion algorithm*/
        short int cycle;
        short int comodo = index-1;
        for(cycle = 0; cycle < index-index/2; cycle++) {
            temp = digit[comodo];
            digit[comodo] = digit[cycle];
            digit[cycle] = temp;
            comodo--;
        }

        for(cycle = 0; cycle < index; cycle++) {
            printChar(NUM_TO_CODE(digit[cycle]));
        }
    }
}

void LCD_HD44780::setCursor(short int line, short int col) {
/*    
 *     @description: This function allows you to set cursor position on the display
 */    
    short int address;
    switch(line) {
        case 0:
            address = 0x00;
            break;
        case 1:
            address = 0x40;
            break;
        case 2:
            address = 0x10;
            break;
        case 3:
            address = 0x50;
            break;
        default:
            address = 0x00;
            break;
    }
    if(col >= 0 && col >= LCD_LINE_LENGHT)
        address += col;

    putCommand(DDRAM_ADDRESS(address));
}

void LCD_HD44780::putCommand(short int command) {
/*    
 *     @description: this is a subroutine to send a single command instruction on the display
 */
    delay(1); // wait until LCD is free
    writeByte(IR, command);
}

void LCD_HD44780::printChar(unsigned char character) {
/*
 *    @description: this is a subroutine to print a single alphanumeric character on the display
 */
    delay(1); // wait until LCD is free
    writeByte(DR, character);
}

void LCD_HD44780::writeByte(short int rs ,short int data_to_LCD) {   
/*    
 *     @description: this is a subroutine to send a single byte on the display
 */    
    if(rs)
        digitalWrite(RegisterSelect, HIGH); 
    else
        digitalWrite(RegisterSelect, LOW);  //----Selecting register as Command register
    digitalWrite(ReadWrite, LOW);           //----Selecting write mode
    
    clearDataLine();
    sendUpperByte(data_to_LCD);

    toggle();

    clearDataLine();
    sendLowerByte(data_to_LCD);

    toggle();  
}

void LCD_HD44780::putCommand_hf(short int data_to_LCD) {
/*    
 *     @description: this is a subroutine to send upper command instruction on the display
 */  
    digitalWrite(RegisterSelect, LOW);      //----Selecting register as Command register
    digitalWrite(ReadWrite, LOW);           //----Selecting write mode
    clearDataLine();                        //----clearing the 4 bits data line
    sendUpperByte(data_to_LCD);
    toggle();
}

void LCD_HD44780::toggle() {
/*    
 *     @description: this is a subroutine to send an High to Low signal on Enable pin
 */  
    delay(1);
    digitalWrite(Enable, HIGH);            
    delay(1);
    digitalWrite(Enable, LOW);
    delay(1); 
}

void LCD_HD44780::sendUpperByte(short int data_to_LCD) {
/*    
 *     @description: this is a subroutine to send last four bit of a byte on the display
 */
    if(data_to_LCD & 0b00010000)
        digitalWrite(DataLine4, HIGH);
    else
        digitalWrite(DataLine4, LOW);
        
    if(data_to_LCD & 0b00100000)
        digitalWrite(DataLine5, HIGH);
    else
        digitalWrite(DataLine5, LOW);
        
    if(data_to_LCD & 0b01000000)
        digitalWrite(DataLine6, HIGH);
    else
        digitalWrite(DataLine6, LOW);
        
    if(data_to_LCD & 0b10000000)   
        digitalWrite(DataLine7, HIGH);
    else
        digitalWrite(DataLine7, LOW); 
}

void LCD_HD44780::sendLowerByte(short int data_to_LCD) {
/*    
 *     @description: this is a subroutine to send first four bit of a byte on the display
 */
    if(data_to_LCD & 0b00000001)
        digitalWrite(DataLine4, HIGH);
    else
        digitalWrite(DataLine4, LOW);
        
    if(data_to_LCD & 0b00000010)
        digitalWrite(DataLine5, HIGH);
    else
        digitalWrite(DataLine5, LOW);
        
    if(data_to_LCD & 0b00000100)
        digitalWrite(DataLine6, HIGH);
    else
        digitalWrite(DataLine6, LOW);
        
    if(data_to_LCD & 0b00001000)   
        digitalWrite(DataLine7, HIGH);
    else
        digitalWrite(DataLine7, LOW); 
}

void LCD_HD44780::clearDataLine(void) {
/*    
 *     @description: this is a subroutine to put Low the data pin
 */
    int dataLine[4] = {DataLine4, DataLine5, DataLine6, DataLine7};
    for(int i=0; i<=3; i++) {
        digitalWrite(dataLine[i], LOW);
    } 
}

#endif
