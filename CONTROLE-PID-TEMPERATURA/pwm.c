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

#include "pwm.h"
#include "pic18f4520.h"

//configura duty cycle do pwm
void pwmSet1(unsigned char porcento) {
    
    //inicializa pwm
    pwmInit();
    
    //formula do duty cycle:
    //DC_porcento = V / ((PR2+1)?4;
    //V = DC/100 ? (PR2+1) ? 4
    //V = DC ? (PR2+1) /25

    unsigned int val = ((unsigned int) porcento) *(PR2 + 1);
    val = val / 25;
    //garante que tem apenas 10 bits
    val &= 0x03ff;
    //os 8 primeiros bits sao colocados no CCPR1L
    CCPR1L = val >> 2;
    //os ultimos dois são colocados na posição 5 e 4 do CCP1CON
    CCP1CON |= (val & 0x0003) << 4;
 
}

//configura frequência do pwm
void pwmFrequency(unsigned int freq) {

    //PR2 = fosc/(fpwm?4?prescaler)?1;
    //PR2 = (8000000/(freq?4?16)) ? 1;
    PR2 = (125000 / (freq)) - 1;
}

//inicializa pwm
void pwmInit(void) {
    
    T2CON |= 0b00000011; //configura o prescale do timer 2 para 1:16
    BitSet(T2CON, 2); //Liga o timer 2

    CCP1CON |= 0b00001100; //configura CCP1 como um PWM
    CCP2CON |= 0b00001100; //configura CCP2 como um PWM
}


  
