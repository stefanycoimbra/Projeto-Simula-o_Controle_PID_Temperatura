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

#include<xc.h>

#define ICLK PORTCbits.RC3
#define IDAT PORTCbits.RC4
#define TIDAT TRISCbits.TRISC4

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_wb(unsigned char val);
unsigned char i2c_rb(unsigned char ack);
void i2c_acktst(unsigned char val);
