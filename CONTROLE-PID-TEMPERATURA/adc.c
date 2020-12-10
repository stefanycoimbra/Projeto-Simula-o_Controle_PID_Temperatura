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
#include "adc.h"
//#include "msspi2c.h"


void adc_init(void)
{
    ADCON1=0x02;
    ADCON0=0x41; 
}

unsigned int adc_amostra(unsigned char canal)
{
        switch(canal)
        {
          case 0: 
            ADCON0=0x01;
            break;
          case 1: 
            ADCON0=0x05;
            break;
          case 2: 
            ADCON0=0x09;
            break;
        }

        ADCON0bits.GO=1;
        while(ADCON0bits.GO == 1);

       return ((((unsigned int)ADRESH)<<2)|(ADRESL>>6));
}
