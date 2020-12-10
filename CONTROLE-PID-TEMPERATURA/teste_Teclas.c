#include "config_452.h"
#include "pic18f4520.h"
#include "atraso.h"
#include "lcd.h"
#include "teclado.h"

unsigned int atrasoMin = 20;
unsigned int atrasoMed = 500;
unsigned int atrasoMax = 1000;

void main() {
    unsigned char i;
    unsigned char tmp;

    ADCON1 = 0x06;
    TRISB = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;

    lcd_init();

    //Teclado numérico
    TRISB = 0xF8;

    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Teclado Numerico");
    lcd_cmd(L_L2);

    while (tmp < 0x3C) {
        TRISD = 0xF0;
        tmp = tc_tecla(0) + 0x30;
        TRISD = 0x00;
        lcd_dat(tmp);
        if (tmp == 0x3A) {
            lcd_cmd(L_L2);
            for (i = 0; i < 16; i++) {
                lcd_dat(i % 2);
                atraso_ms(atrasoMin);
            }
            lcd_cmd(L_L2);
        }
    }

    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Teclas extras");

    TRISA = 0x20;
    TRISB = 0x3F;

    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Teclas extras");

    TRISA = 0x20;
    TRISB = 0x3F;

    lcd_cmd(L_L2);
    lcd_str(" RB0          ");
    while (BitTst(PORTB, 0)); //PORTBbits.RB0

    lcd_cmd(L_L2 + 6);
    lcd_str("Ok");
    atraso_ms(atrasoMed);


    lcd_cmd(L_L2);
    lcd_str(" RB1          ");
    while (BitTst(PORTB, 1)); //PORTBbits.RB1

    lcd_cmd(L_L2 + 6);
    lcd_str("Ok");
    atraso_ms(atrasoMed);


    lcd_cmd(L_L2);
    lcd_str(" RB3          ");
    while (BitTst(PORTB, 3)); //PORTBbits.RB3

    lcd_cmd(L_L2 + 6);
    lcd_str("Ok");
    atraso_ms(atrasoMed);


    lcd_cmd(L_L2);
    lcd_str(" RB4          ");
    while (BitTst(PORTB, 4)); //PORTBbits.RB4

    lcd_cmd(L_L2 + 6);
    lcd_str("Ok");
    atraso_ms(atrasoMed);


    lcd_cmd(L_L2);
    lcd_str(" RB5          ");
    while (BitTst(PORTB, 5)); //PORTBbits.RB5

    lcd_cmd(L_L2 + 6);
    lcd_str("Ok");
    atraso_ms(atrasoMed);
    
    
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("FIM");
    while(1);
}