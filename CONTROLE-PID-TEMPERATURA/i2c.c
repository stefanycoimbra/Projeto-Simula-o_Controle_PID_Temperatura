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

#ifndef _18F4550
#include "i2c.h"
#include "pic18f4520.h"
 

void delay(void)
{
  //#asm
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
  //#endasm
}

void i2c_init(void)
{
    BitSet(PORTC, 1);
    TIDAT=0;
    ICLK=1;
    IDAT=1;
}

void i2c_start(void)
{
    BitSet(PORTC, 1);
    ICLK=1;
    IDAT=1;
    delay();
    IDAT=0;
    delay();
}

void i2c_stop(void)
{
    BitSet(PORTC, 1);
    ICLK=1;
    IDAT=0;
    delay();
    IDAT=1;
    delay();
}

void i2c_wb(unsigned char val)
{
    BitSet(PORTC, 1);
    unsigned char i;
    ICLK=0;
    for(i=0;i<8;i++)
    {
      IDAT=((val>>(7-i))& 0x01);
      ICLK=1;
      delay();
      ICLK=0;
    }	
    IDAT=1;
    delay();
    ICLK=1;
    delay();
    ICLK=0;
}

unsigned char i2c_rb(unsigned char ack)
{
    BitSet(PORTC, 1);
    char i;
    unsigned char ret=0;

    ICLK=0;
    TIDAT=1;
    IDAT=1;
    for(i=0;i<8;i++)
    {
      ICLK=1;
      delay();
      ret|=(IDAT<<(7-i));
      ICLK=0;
    }
    TIDAT=0;
    if(ack)
      IDAT=0;
    else
      IDAT=1;
    delay();
    ICLK=1;
    delay();
    ICLK=0;

    return ret;
}

#endif


