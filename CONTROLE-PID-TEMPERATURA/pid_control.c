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

#include <stdio.h>
#include <stdlib.h>
#include "pic18f4520.h"
#include "pid_control.h"
#include "lcd.h"
#include "itoa.h"
#include "atraso.h"
#include <math.h>

int pid(int ideal, int atual, int kp, int ki, int kd) {
    
    int PID;
    int proporcional;
    int derivativo;
    int integrativo;
    int ultima = 0;
    int erroaux = 0, erro = 0;
    
    erroaux = (ideal - atual);
    
    if(erroaux<0) {
        erro = erroaux*(-1);
    }
    else {
        erro = erroaux;
    }
    proporcional = erro * kp;
    integrativo += (erro*ki);
    derivativo = (ultima - atual) * kd;
    ultima = atual;
    PID = proporcional + integrativo + derivativo;
    //PID = PID/4;
    //pwm = (PID + 128)*100/256;
    
    return PID;
}

