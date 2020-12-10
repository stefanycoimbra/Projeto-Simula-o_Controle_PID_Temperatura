/* 
 * File:   main.c
 * 
 * Autor: Stéfany Coura Coimbra
 * Matrícula: 2019008562
 * Universidade Federal de Itajubá - Unifei
 * Campus Itajubá
 * Disciplina: Programação Embarcada
 * 
 * PROGRAMA: SIMULAÇÃO DO CONTROLE PID DA TEMPERATURA DE UM PROCESSO
 * 
 * Created on 11 de Junho de 2020, 16:58
 */

#include <xc.h>
#include "lcd.h"
#include "atraso.h"
#include "pic18f4520.h"

void lcd_wr(unsigned char val)
{
    LPORT=val;
}

void lcd_cmd(unsigned char val)
{
	LENA=1;
        lcd_wr(val);
        LDAT=0;
        atraso_ms(3);
        LENA=0;
        atraso_ms(3);
	LENA=1;
}
 
void lcd_dat(unsigned char val)
{
	LENA=1;
        lcd_wr(val);
        LDAT=1;
        atraso_ms(3);
        LENA=0;
        atraso_ms(3);
	LENA=1;
}

void lcd_init(void)
{
	LENA=0;
	LDAT=0;
	atraso_ms(20);
	LENA=1;
	
	lcd_cmd(L_CFG);
	atraso_ms(5);
	lcd_cmd(L_CFG);
    atraso_ms(1);
	lcd_cmd(L_CFG); //configura
	lcd_cmd(L_OFF);
	lcd_cmd(L_ON); //liga
	lcd_cmd(L_CLR); //limpa
	lcd_cmd(L_CFG); //configura
    lcd_cmd(L_L1);
}

void lcd_str(const char* str)
{
    unsigned char i=0;
 
     while(str[i] != 0)
    {
         if(str[i] == '\n')
        {
            lcd_cmd(0xC0);
        }
     
        else lcd_dat(str[i]);
     
         i++;
    }  
}






 
