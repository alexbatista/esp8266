/* mbed freetronicsLCDShield Library, written by Koen J.F. Kempeneers
 * koen.kempeneers@damiaaninstituut.be
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#ifndef FREETRONICSLCDSHIELD_H
#define FREETRONICSLCDSHIELD_H
 
#define LEFT  0
#define RIGHT 1
 
/** 
  * Provides full LCD support for the HD44780 compatible LCD on the arduino shaped shield.
  * http://www.freetronics.com/products/lcd-keypad-shield#.UnIr6_nkq0M
  *
  */
class freetronicsLCDShield : public Stream {
    private:
        // Functions
        void writeByte (int byte);
        void writeCommand (int command);
        void writeData (int data);
        void character(int line, int col, int value);
        
        // Hardware         
        DigitalOut _rs, _e;
        BusOut _d;
        PwmOut _bl;
        AnalogIn _a0;
        
    public:
        /** 
         * The constructor creates an freeTronics LCD Shield object, the pins are to be provided by the user. In sequence, RegisterSelect, Enable, Data0 
         * to Data3. Bl is the backlight and a0 is to be provided for button support. 
         * Bl should be a pin with PWM capabilities and a0 should be an analogue input.
         * 
         * The class inherits from stream, therfore writing to the display is as easy as calling printf() to display text or putc() to display a custom character  
         * 
         * Example:
         * @code
         * <instanceName>.printf("Hello World");
         * @endcode         */ 
        freetronicsLCDShield (PinName rs /*= PTA13*/, 
                              PinName e  /*= PTD5*/, 
                              PinName d0 /*= PTA4*/, 
                              PinName d1 /*= PTA5*/, 
                              PinName d2 /*= PTC8*/, 
                              PinName d3 /*= PTC9*/,
                              PinName bl /*= PTA12*/,
                              PinName a0 /*= PTB0*/);                               
        /** Creates custom characters 
         *
         * Characters that aren't included in the LCD controllers character map which includes typically all ASCII characters 
         * can be generated by writing bitmaps to the character generator ram memory space. For instance the degree sign '�' is an 
         * extended ASCII character not included in the character map.
         * It can however be generated using the writeCGRAM member function. Each line of the 5x7 dot matrix is represented by a byte in which
         * the lower 5 bits correspond to the pixel on the display. In total 8 bytes make up one custom character (the 8th byte represents the
         * cursor space)
         *
         * Example:
         * @code
         * CGRAM_DATA[] = {0xC0,    //0b00001100
         *                 0x12,    //0b00010010 
         *                 0x12,    //0b00010010 
         *                 0xC0,    //0b00001100
         *                 0x00,    //0b00000000 
         *                 0x00,    //0b00000000 
         *                 0x00,    //0b00000000 
         *                 0x00};   //0b00000000
         * 
         * <instanceName>.writeCGRAM (0x00, &CGRAM_DATA[0], 8);
         * @endcode
         *
         * The '�' can hereafter be displayed by calling:
         * @code
         * <instanceName>.putc (0);
         * @endcode
         *
         */                        
        void writeCGRAM (char address, const char *ptr, char nbytes);
        
        /** Sets the current cursor position.
         *
         * To place the cursor at a specific location on the display call the setCursorPosition member function, the first argument is the line either 0 
         * or 1, the second argument is the column 0 .. 15.
         *
         */
        void setCursorPosition (int line, int col);
        
        /** Sets the backlight.
         * 
         * The backlight is turned on (argument true) or off (false) 
         */  
        void setBackLight (bool blStatus);
 
        /** Sets the backlight.
         * 
         * The backlight intensity is specified by the normalized float argument 0 .. 1 
         */  
        void setBackLight (float blIntensity);
        
        /** Sets cursor appearance.
         * 
         * The cursor is set visible (1st argument true) or invisible (false). When the second argument is set when the cStatus is set the cursor blinks. 
         */  
        void setCursor (bool cStatus = true, bool blink = false);
        
        /** Shifts text.
         * 
         * Text on the display is shifted left.
         */  
        void shiftLeft (void);
        
        /** Shifts text.
         * 
         * Text on the display is shifted right.
         */  
        void shiftRight (void);
        
 
        /** Shifts text.
         * 
         * Text on the display is shifted left if direction is set (true) or right is direction is reset (false)
         */  
        void shift (bool direction);
 
        /** Clears the display, the cursor returns to its home position (0,0).
         * 
         * The user should preserve caution when clearing the display in the main program loop, this very quickly results in flickering. A better approach is to
         * overwrite the display. 
         */  
        void cls (void);
 
        /** Returns the cursor to positition (0,0). The display is NOT cleared. 
         * 
         * This function differs from setCursorPosition(0,0) in the way that home() undoes all preceding shift operations. i.e. If the display is shifted
         * one position right, the setCursorPosition(0,0) function call would place the cursor physically at the second character of the first row while
         * home() places it at the first character of the first row. 
         */  
        void home(void);
        
        /** Reads the status of the buttons
         * 
         * 
         */  
        float readButton(void);
             
    protected:
        // Stream implementation functions
        virtual int _putc(int value);
        virtual int _getc();
};
 
#endif
       