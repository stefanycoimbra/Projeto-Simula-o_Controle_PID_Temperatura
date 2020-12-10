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
#include "teclado.h"
#include "atraso.h"
#include "pic18f4520.h"

const unsigned char linha[4]= {TL1,TL2,TL3};

unsigned char tc_tecla(unsigned int timeout)
{
    unsigned int to=0;
    unsigned char i;
    unsigned char ret=0;
    unsigned char tmp=PORTB;


    while(((to < timeout)||(!timeout))&&(!ret))  
    {
      for(i=0;i<3;i++)
      {
        PORTB|=~linha[i];
        if(!TC1){atraso_ms(20);if(!TC1){while(!TC1);ret= 1+i;break;}};
        if(!TC2){atraso_ms(20);if(!TC2){while(!TC2);ret= 4+i;break;}};
        if(!TC3){atraso_ms(20);if(!TC3){while(!TC3);ret= 7+i;break;}};
        if(!TC4){atraso_ms(20);if(!TC4){while(!TC4);ret= 10+i;break;}};
        PORTB &=linha[i];
      };
      atraso_ms(5);
      to+=5;
    }

    if(!ret)ret=255;
    if(ret == 11)ret=0;
    PORTB=tmp;
    return ret;
}

/*#include "pic18f4520.h"
#include "teclado.h"
#include "atraso.h"
#include <xc.h>

// TC0 PORTDbits.RD3 BitTst(PORTD,3)
// TC1 PORTDbits.RD2 BitTst(PORTD,2)
// TC2 PORTDbits.RD1 BitTst(PORTD,1)
// TC3 PORTDbits.RD0 BitTst(PORTD,0)

const unsigned char coluna[4] = {TC1, TC2, TC3};
unsigned int atraso_min = 5;
unsigned int atraso = 15;

unsigned char tc_tecla(unsigned int timeout) {
    unsigned int to = 0;
    unsigned char i;
    unsigned char ret = 0;
    unsigned char tmp = PORTB;


    while (((to < timeout) || (!timeout))&&(!ret)) {
        for (i = 0; i < 3; i++) {
            PORTB |= ~coluna[i];
            if (!BitTst(PORTD,3)) { //TC1
                atraso_ms(atraso);
                if (!BitTst(PORTD,3)) {
                    while (!BitTst(PORTD,3));
                    ret = 1 + i;
                    break;
                }
            };
            if (!BitTst(PORTD,2)) {
                atraso_ms(atraso);
                if (!BitTst(PORTD,2)) {
                    while (!BitTst(PORTD,2));
                    ret = 4 + i;
                    break;
                }
            };
            if (!BitTst(PORTD,1)) {
                atraso_ms(atraso);
                if (!BitTst(PORTD,1)) {
                    while (!BitTst(PORTD,1));
                    ret = 7 + i;
                    break;
                }
            };
            if (!BitTst(PORTD,0)) {
                atraso_ms(atraso);
                if (!BitTst(PORTD,0)) {
                    while (!BitTst(PORTD,0));
                    ret = 10 + i;
                    break;
                }
            };
            PORTB &= coluna[i];
        };
        atraso_ms(atraso_min);
        to += 5;
    }

    if (!ret)ret = 255;
    if (ret == 11)ret = 0;
    PORTB = tmp;
    return ret;
}

const unsigned char linha[4]= {TL1,TL2,TL3,TL4};

unsigned char tc_tecla(unsigned int timeout)
{
  unsigned int to=0;
  unsigned char i;
  unsigned char ret=0;
  unsigned char tmp=PORTD;

  while(((to < timeout)||(!timeout))&&(!ret))
  {
    for(i=0;i<4;i++)
    {
      PORTD|=linha[i];
      if(TC1){atraso_ms(20);if(TC1){while(TC1);ret= 1+(i*3);break;}};
      if(TC2){atraso_ms(20);if(TC2){while(TC2);ret= 2+(i*3);break;}};
      if(TC3){atraso_ms(20);if(TC3){while(TC3);ret= 3+(i*3);break;}};
      PORTD&=~linha[i];
    };
    atraso_ms(5);
    to+=5;
  }

  if(!ret)ret=255;
  if(ret == 11)ret=0;
  PORTA=tmp;
  return ret;
}*/


