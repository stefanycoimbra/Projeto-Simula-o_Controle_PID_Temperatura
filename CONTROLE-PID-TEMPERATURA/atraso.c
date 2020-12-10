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

#include "pic18f4520.h"

void atraso_ms(unsigned int valor)
{
    unsigned int  i;
    unsigned char j;

     for (i =0; i< valor; i++)
     {

      for (j =0 ; j < 200; j++)
       {
        //_asm
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        asm("NOP");
        //_endasm
       }
     }
}
