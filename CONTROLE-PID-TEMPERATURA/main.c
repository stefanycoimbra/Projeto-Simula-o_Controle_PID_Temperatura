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

//*****************************************************************************************************************

//INCLUDES DE BIBLIOTECAS

//*****************************************************************************************************************

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <xc.h>
#include "config_452.h"
#include "atraso.h"
#include "lcd.h"
#include "pic18f4520.h"
#include "itoa.h"
#include "adc.h"
#include "teclado.h"
#include "displaySeteSegmentos.h"
#include "pwm.h"
#include "temperatura.h"
#include "rtc.h"

//*****************************************************************************************************************
//VARIÁVEIS GERAIS
//*****************************************************************************************************************

//VARIÁVEL PARA RTC
int rtcLoop = 0; //variável para loop relacionado à data e hora mostrados

//VARIÁVEIS DE ATRASO PARA TECLAS 
unsigned int atrasoMin = 20; //variável para gerar um atraso de 20 milissegundos no programa
unsigned int atrasoMed = 500; //variável para gerar um atraso de 500 milissegundos no programa
unsigned int atrasoMax = 1000; //variável para gerar um atraso de 1000 milissegundos no programa
    
//VARIÁVEIS PARA SETPOINT
unsigned char tmpSetpoint; //variável usada para apresentar tecla apertada no momento
int auxSetpoint; //variável auxiliar para loop de inicialização do vetor setpoint 
int setpoint[16]; //vetor que guarda valor do setpoint para o processo
int contSetpoint = 0; //variável para contar o número de teclas pressionadas para setpoint 
int posSetpoint; //variável usada para percorrer loop
int setpointInt = 0; //variável inteira que guarda valor do setpoint
int setpointNeg = 0; //flag para ver estado do setpoint(se é negativo 1, se é positivo 0)
    
//VARIÁVEIS PARA TEMPERATURA
int temperaturaInt = 0; //variável inteira que guarda valor da temperatura
    
//VARIÁVEIS PARA Kp
unsigned char tmpKp; //variável usada para apresentar tecla apertada no momento
int auxKp; //variável auxiliar para loop de inicialização do vetor Kp
int Kp[16]; //vetor que guarda valor do Kp para o processo
int contKp = 0; //variável para contar o número de teclas pressionadas para Kp 
int posKp; //variável usada para percorrer loop
int KpInt = 1; //variável inteira que guarda valor de Kp 
    
//VARIÁVEIS PARA Kd
unsigned char tmpKd; //variável usada para apresentar tecla apertada no momento
int auxKd; //variável auxiliar para loop de inicialização do vetor Kd
int Kd[16]; //vetor que guarda valor do Kd para o processo
int contKd = 0; //variável para contar o número de teclas pressionadas para Kd
int posKd; //variável usada para percorrer loop
int KdInt = 0; //variável inteira que guarda valor de Kd
    
//VARIÁVEIS PARA Ki
unsigned char tmpKi; //variável usada para apresentar tecla apertada no momento
int auxKi; //variável auxiliar para loop de inicialização do vetor Ki
int Ki[16]; //vetor que guarda valor do Ki para o processo
int contKi = 0; //variável para contar o número de teclas pressionadas para Ki
int posKi; //variável usada para percorrer loop
int KiInt = 0; //variável inteira que guarda valor de Ki
    
//VARIÁVEIS PARA PID
int T0_baseTime = 0; //variável de base de tempo para pwm
int pwm = 0; //variável para guardar cálculo do pwm do processo
int PID; //variável para guardar o valor do cálculo PID do proesso

//VARIÁVEL ADC
unsigned char tmpADCPot1;


//*****************************************************************************************************************  
//PROTÓTIPOS DAS FUNÇÕES
//*****************************************************************************************************************
void menuGeral(); //menu geral do programa
void menuPrincipal(); //menu principal do programa
void menuSetpoint(); //setpoint
void menuTemperatura(); //temperatura
void menuConstantes(); //constantes Kp, Kd, Ki do PID
void menuProcesso(); //processo

//*****************************************************************************************************************
//*****************************************************************************************************************
//FUNÇÕES
//*****************************************************************************************************************
//*****************************************************************************************************************

//SETPOINT
void menuSetpoint() {
    
    setpointNeg = 0; //inicializa com setpoint considerado positivo
  
    TRISB = 0x3F; //bits RB0 a RB5 da PORTB configurados como entrada
            
    lcd_cmd(L_CLR); //apaga todo o display LCD
    lcd_cmd(L_L1); //coloca cursor na primeira linha, coluna 0 do Display LCD
    lcd_str(" DIGITE O VALOR "); //escreve mensagem na primeira linha do LCD
    lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD
    lcd_str("DO SETPOINT PARA"); //escreve mensagem na segunda linha do LCD
    lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD
    lcd_str("   O PROCESSO:  "); //escreve mensagem na terceira linha do LCD
    lcd_cmd(L_L4); //coloca cursor na quarta linha, coluna 0 do Display LCD

    TRISB = 0xF8; //configura os cinco bits mais significativos de PORTB como entrada
    tmpSetpoint = 0x00; //inicializa tmpSetpoint
    setpointInt = 0; //inicializa setpointInt
         
    //inicializa vetor setpoint
    for(auxSetpoint = 0; auxSetpoint < 16; auxSetpoint++) {
        setpoint[auxSetpoint] = 0; //cada posição do vetor recebe 0
    }
    //enquanto a tecla fizer parte do teclado numérico
    while (tmpSetpoint < 0x3C) {
        
        TRISD = 0x0F; //bits RD0 a RD4 da PORTD configurados como entrada
        
        tmpSetpoint = (tc_tecla(0) + 0x30); //guarda na variável o número pressionado como char
        
        if(tmpSetpoint != 0x3A && tmpSetpoint != 0x2D) {
            setpoint[posSetpoint] = tc_tecla(0); //guarda número correspondente na posição do vetor
            posSetpoint++; //incrementa a variável da posição do vetor setpoint
        }
        
        if(tmpSetpoint == 0x3A) {
            tmpSetpoint = 0x2D;
            setpointNeg = 1;
        }
        
        if(tmpSetpoint < 0x3C && tmpSetpoint != 0x3A && tmpSetpoint != 0x2D) //verifica se a tecla pressionada foi do teclado matricial 4x3
            contSetpoint++; //conta o número de teclas pressionadas para setpoint
        
        TRISD = 0x00; //configura todos os pinos do PORTD como saída
        lcd_dat(tmpSetpoint); //mostra no display LCD a tecla pressionada
    }   

    //*****************************************************************************************************************
    //CONVERTE SETPOINT, EM STRING, PARA INT

    int fatorSetpoint = contSetpoint-1; //fator de cada potência quando o setpoint, em string, é convertido para int
    //para a posição do vetor setpoint de 0 a contSetpoint
    for (posSetpoint = 0; posSetpoint < contSetpoint; posSetpoint++){ 

        setpointInt += setpoint[posSetpoint] * pow(10, fatorSetpoint); //cada posição é multiplicada por uma potência de 10 (unidade, dezena, centena)
        fatorSetpoint--; //diminui uma unidade no fatorSetpoint
    }
    //se setpoint é negativo
    if(setpointNeg == 1) {
        setpointInt = setpointInt*(-1);
    }

    //*****************************************************************************************************************
    //MOSTRA VALOR ESCOLHIDO PARA SETPOINT
    TRISB = 0x3F; //bits RB0 a RB5 da PORTB configurados como entrada
           
    lcd_cmd(L_CLR); //apaga todo o display LCD
    lcd_str("VALOR SETPOINT: "); //escreve mensagem na primeira linha do LCD
    lcd_cmd(L_L2+6); //coloca cursor na segunda linha, coluna 6 do Display LCD

    //para a posição do vetor setpoint de 0 a contSetpoint
    for (posSetpoint = 0; posSetpoint < contSetpoint; posSetpoint++) {
        
        //se setpoint é negativo
        if(setpointNeg == 1) {
            lcd_dat(0x2D); //mostra sinal negativo no display LCD
            setpointNeg = 0;
        }
        else {
            lcd_dat(setpoint[posSetpoint]+0x30); //mostra no display LCD o valor da posição do vetor setpoint (passado como parâmetro char)
        }
        atraso_ms(atrasoMin); //gera um atraso
    }
    lcd_cmd(L_L2+6+contSetpoint); //coloca cursor na segunda linha, coluna 11 do Display LCD
    lcd_str(" C"); //escreve mensagem na segunda linha do LCD
    lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD
    lcd_str("PRESS RB0   "); //escreve mensagem na terceira linha do LCD
    while(BitTst(PORTB, 0)); //Espera RB0 ser pressionado
    lcd_str("Ok"); //escreve mensagem na terceira linha do LCD
    atraso_ms(1000); //gera um atraso

    //*****************************************************************************************************************
}
//*****************************************************************************************************************
//TEMPERATURA
void menuTemperatura() {
    
    TRISB = 0x3F; //bits RB0 a RB5 da PORTB configurados como entrada
    lcd_cmd(L_CLR); //apaga todo o display LCD
    lcd_str("Temperatura : "); //escreve mensagem na primeira linha do LCD

    //*****************************************************************************************************************
    //VALOR DA TEMPERATURA

    //DISPLAYS DE SETE SEGMENTOS
    configura(); //configura registradores e ports para usar os displays de sete segmentos
    iniciaDisplays7Seg(); //inicializa os displays de sete segmentos

    //loop infinito
    for(;;) {
                
        TRISA = 0x20; //configura para botão RA5
        TRISB = 0x3F; //bits RB0 a RB5 da PORTB configurados como entrada
                
        lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD
        lcd_str("RB0:T+"); //escreve mensagem na terceira linha do LCD
        lcd_cmd(L_L3+10); //coloca cursor na terceira linha, coluna 10 do Display LCD
        lcd_str("RB1:T-"); //escreve mensagem na terceira linha do LCD
        lcd_cmd(L_L4); //coloca cursor na quarta linha, coluna 0 do Display LCD
        lcd_str("   RB3: STOP  "); //escreve mensagem na quarta linha do LCD

        while (BitTst(PORTB, 0) && BitTst(PORTB, 1) && BitTst(PORTB, 3)); //Espera um dos botões (RB0, RB1 ou RB3) serem pressionados

        //se o botão pressionado foi RB0
        if(BitTst(PORTB, 0) == 0) {
            mostraTemperaturaAlterada(1, temperaturaInt); 
            temperaturaInt++; //incrementa o valor da temperatura
        }

        //se o botão pressionado foi RB1
        else if(BitTst(PORTB, 1) == 0) {
            mostraTemperaturaAlterada(2, temperaturaInt);
            temperaturaInt--; //decrementa o valor da temperatura
        }

        //se o botão pressionado foi RB3
        else if(BitTst(PORTB, 3) == 0) {
            break; //interrompe loop
        }
    }
    iniciaDisplays7Seg(); //reinicializa os displays de sete segmentos
    //*****************************************************************************************************************
}

//*****************************************************************************************************************
//CONSTANTES Kp, Kd, Ki

void menuConstantes() {
   
    //loop infinito
    for(;;) {
    
        TRISB = 0x3F; //bits RB0 a RB5 da PORTB configurados como entrada

        lcd_cmd(L_CLR); //apaga todo o display LCD
        lcd_cmd(L_L1); //coloca cursor na primeira linha, coluna 0 do Display LCD
        lcd_str("   RB3: Kp  "); //escreve mensagem na primeira linha do LCD
        lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD
        lcd_str("   RB4: Kd  "); //escreve mensagem na segunda linha do LCD
        lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD
        lcd_str("   RB5: Ki  "); //escreve mensagem na terceira linha do LCD
        lcd_cmd(L_L4); //coloca cursor na quarta linha, coluna 0 do Display LCD
        lcd_str("    RB0: STOP   "); //escreve mensagem na quarta linha do LCD
        while (BitTst(PORTB, 3) && BitTst(PORTB, 4) && BitTst(PORTB, 5) && BitTst(PORTB, 0)); //Espera um dos botões (RB3, RB4, RB5 ou RB0) serem pressionados

        //*****************************************************************************************************************
        //ALTERAR Kp

        //se o botão pressionado foi RB3
        if(BitTst(PORTB, 3) == 0) { 

            lcd_cmd(L_CLR); //apaga todo o display LCD
            lcd_cmd(L_L1); //coloca cursor na primeira linha, coluna 0 do Display LCD
            lcd_str(" DIGITE O VALOR "); //escreve mensagem na primeira linha do LCD
            lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD
            lcd_str("    PARA Kp:    "); //escreve mensagem na segunda linha do LCD
            lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD

            TRISB = 0xF8; //configura os cinco bits mais significativos de PORTB como entrada
            tmpKp = 0x00; //inicializa tmpKp
            KpInt = 0; //inicializa Kp

            //inicializa vetor Kp
            for(auxKp = 0; auxKp < 16; auxKp++) {
                Kp[auxKp] = 0; //cada posição do vetor recebe 0
            }

            //enquanto a tecla fizer parte do teclado numérico
            while (tmpKp < 0x3C) {
                TRISD = 0X0F; //bits RD0 a RD4 da PORTD configurados como entrada
                tmpKp = (tc_tecla(0) + 0x30); //guarda na variável o número pressionado como char
                Kp[posKp] = tc_tecla(0); //guarda número correspondente na posição do vetor
                posKp++; //incrementa a variável da posição do vetor Kp
                if(tmpKp < 0x3C) //verifica se a tecla pressionada foi do teclado matricial 4x3
                    contKp++; //conta o número de teclas pressionadas para Kp
                TRISD = 0x00; //configura todos os pinos do PORTD como saída
                lcd_dat(tmpKp); //mostra no display LCD a tecla pressionada
            }   

            //*****************************************************************************************************************
            //CONVERTE Kp, EM STRING, PARA INT

            int fatorKp = contKp-1; //fator de cada potência quando o Kp, em string, é convertido para int
            //para a posição do vetor Kp de 0 a contKp
            for (posKp = 0; posKp < contKp; posKp++){ 
                KpInt += Kp[posKp] * pow(10, fatorKp); //cada posição é multiplicada por uma potência de 10 (unidade, dezena, centena)
                fatorKp--; //diminui uma unidade no fatorKp
            }

            //*****************************************************************************************************************
            //MOSTRA VALOR ESCOLHIDO PARA Kp

            TRISB = 0x3F; //bits RB0 a RB5 da PORTB configurados como entrada

            lcd_cmd(L_CLR); //apaga todo o display LCD
            lcd_str("VALOR Kp: "); //escreve mensagem na primeira linha do LCD
            lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD

            //para a posição do vetor Kp de 0 a contKp
            for (posKp = 0; posKp < contKp; posKp++) {
                lcd_dat(Kp[posKp]+0x30); //mostra no display LCD o valor da posição do vetor Kp (passado como parâmetro char)
                atraso_ms(atrasoMin); //gera um atraso
            }

            lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD
            lcd_str("PRESS RB0   "); //escreve mensagem na terceira linha do LCD
            while(BitTst(PORTB, 0)); //Espera RB0 ser pressionado
            lcd_str("Ok"); //escreve mensagem na terceira linha do LCD
            atraso_ms(1000); //gera um atraso

            //*****************************************************************************************************************
        }

        //*****************************************************************************************************************
        //ALTERAR Kd

        //se o botão pressionado foi RB4
        else if(BitTst(PORTB, 4) == 0) { 

            lcd_cmd(L_CLR); //apaga todo o display LCD
            lcd_cmd(L_L1); //coloca cursor na primeira linha, coluna 0 do Display LCD
            lcd_str(" DIGITE O VALOR "); //escreve mensagem na primeira linha do LCD
            lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD
            lcd_str("    PARA Kd:    "); //escreve mensagem na segunda linha do LCD
            lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD

            TRISB = 0xF8; //configura os cinco bits mais significativos de PORTB como entrada
            tmpKd = 0x00; //inicializa tmpKd
            KdInt = 0; //inicializa Kd

            //inicializa vetor Kd
            for(auxKd = 0; auxKd < 16; auxKd++) {
                Kd[auxKd] = 0; //cada posição do vetor recebe 0
            }

            //enquanto a tecla fizer parte do teclado numérico
            while (tmpKd < 0x3C) {
                TRISD = 0X0F; //bits RD0 a RD4 da PORTD configurados como entrada
                tmpKd = (tc_tecla(0) + 0x30); //guarda na variável o número pressionado como char
                Kd[posKd] = tc_tecla(0); //guarda número correspondente na posição do vetor
                posKd++; //incrementa a variável da posição do vetor Kd
                if(tmpKd < 0x3C) //verifica se a tecla pressionada foi do teclado matricial 4x3
                    contKd++; //conta o número de teclas pressionadas para Kd
                TRISD = 0x00; //configura todos os pinos do PORTD como saída
                lcd_dat(tmpKd); //mostra no display LCD a tecla pressionada
            }   

            //*****************************************************************************************************************
            //CONVERTE Kd, EM STRING, PARA INT

            int fatorKd = contKd-1; //fator de cada potência quando o Kd, em string, é convertido para int
            //para a posição do vetor Kd de 0 a contKd
            for (posKd = 0; posKd < contKd; posKd++){ 
                KdInt += Kd[posKd] * pow(10, fatorKd); //cada posição é multiplicada por uma potência de 10 (unidade, dezena, centena)
                fatorKd--; //diminui uma unidade no fatorKd
            }

            //*****************************************************************************************************************
            //MOSTRA VALOR ESCOLHIDO PARA Kd

            TRISB = 0x3F; //bits RB0 a RB5 da PORTB configurados como entrada

            lcd_cmd(L_CLR); //apaga todo o display LCD
            lcd_str("VALOR Kd: "); //escreve mensagem na primeira linha do LCD
            lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD

            //para a posição do vetor Kd de 0 a contKd
            for (posKd = 0; posKd < contKd; posKd++) {
                lcd_dat(Kd[posKd]+0x30); //mostra no display LCD o valor da posição do vetor Kd (passado como parâmetro char)
                atraso_ms(atrasoMin); //gera um atraso
            }

            lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD
            lcd_str("PRESS RB0   "); //escreve mensagem na terceira linha do LCD
            while(BitTst(PORTB, 0)); //Espera RB0 ser pressionado
            lcd_str("Ok"); //escreve mensagem na terceira linha do LCD
            atraso_ms(1000); //gera um atraso

            //*****************************************************************************************************************
        }

        //*****************************************************************************************************************
        //ALTERAR Ki

        //se o botão pressionado foi RB5
        else if(BitTst(PORTB, 5) == 0) { 

            lcd_cmd(L_CLR); //apaga todo o display LCD
            lcd_cmd(L_L1); //coloca cursor na primeira linha, coluna 0 do Display LCD
            lcd_str(" DIGITE O VALOR "); //escreve mensagem na primeira linha do LCD
            lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD
            lcd_str("    PARA Ki:    "); //escreve mensagem na segunda linha do LCD
            lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD

            TRISB = 0xF8; //configura os cinco bits mais significativos de PORTB como entrada
            tmpKi = 0x00; //inicializa tmpKi
            KiInt = 0; //inicializa Ki

            //inicializa vetor Ki
            for(auxKi = 0; auxKi < 16; auxKi++) {
                Ki[auxKi] = 0; //cada posição do vetor recebe 0
            }

            //enquanto a tecla fizer parte do teclado numérico
            while (tmpKi < 0x3C) {
                TRISD = 0X0F; //bits RD0 a RD4 da PORTD configurados como entrada
                tmpKi = (tc_tecla(0) + 0x30); //guarda na variável o número pressionado como char
                Ki[posKi] = tc_tecla(0); //guarda número correspondente na posição do vetor
                posKi++; //incrementa a variável da posição do vetor Ki
                if(tmpKi < 0x3C) //verifica se a tecla pressionada foi do teclado matricial 4x3
                    contKi++; //conta o número de teclas pressionadas para Ki
                TRISD = 0x00; //configura todos os pinos do PORTD como saída
                lcd_dat(tmpKi); //mostra no display LCD a tecla pressionada
            }   

            //*****************************************************************************************************************
            //CONVERTE Ki, EM STRING, PARA INT

            int fatorKi = contKi-1; //fator de cada potência quando o Ki, em string, é convertido para int
            //para a posição do vetor Ki de 0 a contKi
            for (posKi = 0; posKi < contKi; posKi++){ 
                KdInt += Kd[posKd] * pow(10, fatorKi); //cada posição é multiplicada por uma potência de 10 (unidade, dezena, centena)
                fatorKi--; //diminui uma unidade no fatorKd
            }

            //*****************************************************************************************************************
            //MOSTRA VALOR ESCOLHIDO PARA Ki
            TRISB = 0x3F; //bits RB0 a RB5 da PORTB configurados como entrada

            lcd_cmd(L_CLR); //apaga todo o display LCD
            lcd_str("VALOR Ki: "); //escreve mensagem na primeira linha do LCD
            lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD

            //para a posição do vetor Ki de 0 a contKd
            for (posKi = 0; posKi < contKi; posKi++) {
                lcd_dat(Ki[posKi]+0x30); //mostra no display LCD o valor da posição do vetor Ki (passado como parâmetro char)
                atraso_ms(atrasoMin); //gera um atraso
            }

            lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD
            lcd_str("PRESS RB0   "); //escreve mensagem na terceira linha do LCD
            while(BitTst(PORTB, 0)); //Espera RB0 ser pressionado
            lcd_str("Ok"); //escreve mensagem na terceira linha do LCD
            atraso_ms(1000); //gera um atraso

            //*****************************************************************************************************************
        }
        
        //se o botão pressionado foi RB0 
        else if(BitTst(PORTB, 0) == 0) {
            break; //interrompe o loop
        }
    }
}

//*****************************************************************************************************************
//PROCESSO
void menuProcesso() {
   
    lcd_cmd(L_CLR); //apaga todo o display LCD
        
    //espera até que RB1 seja pressionado
    while(BitTst(PORTB, 1)) { 
        
        atraso_ms(1000);
        
        //*****************************************************************************************************************
        //PWM E PID

        pwm = 0; //inicializa pwm
        PID = 0; //inicializa PID
        
        PID = pid(setpointInt, temperaturaInt, KpInt, KdInt, KiInt); //guarda cálculo do PID
        
        pwm = ((PID)+128)*100/256; //converte valor de pwm para porcentagem
        
        if(pwm == 256)
            pwm = 255;
        
        if(pwm<0) {
            pwm = pwm*(-1);
        }
        
        //MOSTRA PWM
        
        char auxPwm[16]; //variável auxiliar para guardar pwm como string
        itoa(pwm, auxPwm); //converte pwm de int para string
        
        lcd_cmd(L_L1); //coloca cursor na primeira linha, coluna 0 do Display LCD
        lcd_str("PWM: "); //escreve mensagem na primeira linha do LCD
        lcd_cmd(L_L1+6); //coloca cursor na primeira linha, coluna 6 do Display LCD
        lcd_str(auxPwm); //escreve valor na segunda linha do LCD
        
        //MOSTRA TEMPERATURA
        
        char auxTemperatura[16]; //vetor que guarda valor da temperatura atual, em string
        itoa(temperaturaInt, auxTemperatura); //converte valor de temperatura, em int, para string
        
        lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD
        lcd_str("TEMP: "); //escreve mensagem na primeira linha do LCD
        lcd_cmd(L_L2+7); //coloca cursor na segunda linha, coluna 7 do Display LCD
        lcd_str(auxTemperatura); //mostra valor da temperatura no display LCD
        lcd_cmd(L_L2+5); //coloca cursor na segunda linha, coluna 7 do Display LCD
        lcd_str("C"); //escreve mensagem na segunda linha do Display LCD
        
         //MOSTRA SETPOINT
        
        char auxSetpointMostrar[16]; //vetor que guarda valor da temperatura atual, em string
        itoa(setpointInt, auxSetpointMostrar); //converte valor de temperatura, em int, para string
        
        lcd_cmd(L_L3); //coloca cursor na segunda linha, coluna 0 do Display LCD
        lcd_str("SETPOINT: "); //escreve mensagem na primeira linha do LCD
        lcd_cmd(L_L3+11); //coloca cursor na segunda linha, coluna 7 do Display LCD
        lcd_str(auxSetpointMostrar); //mostra valor da temperatura no display LCD
        lcd_cmd(L_L3+14); //coloca cursor na segunda linha, coluna 7 do Display LCD
        lcd_str("C"); //escreve mensagem na segunda linha do Display LCD
        
        TRISB = 0x3F; //bits RB0 a RB5 da PORTB configurados como entrada
        
        lcd_cmd(L_L4); //coloca cursor na terceira linha, coluna 0 do Display LCD
        lcd_str("RB1:MENU"); //escreve mensagem na quarta linha do LCD
        
        //se foi pressionado o botão RB1
        if(BitTst(PORTB, 1) == 0) { 
            
            lcd_cmd(L_L4+10); //coloca cursor na terceira linha, coluna 7 do Display LCD
            lcd_str("Ok"); //escreve mensagem na quarta linha do LCD
            atraso_ms(1000); //gera um atraso
            break; //interrompe o loop
        }
        
        //*****************************************************************************************************************
        //VALOR DA TEMPERATURA
        
        //DISPLAYS DE SETE SEGMENTOS
        configura(); //configura registradores e ports para usar os displays de sete segmentos
        iniciaDisplays7Seg(); //inicializa os displays de sete segmentos
        
        //se a temperatura atual é igual ao setpoint, emite um som por meio do buzzer
        if(temperaturaInt == setpointInt) {
           
            BitClr(PORTE, 0); //desliga relé 2
            BitClr(PORTC, 0); //desliga relé 1
            BitClr(PORTC, 5); //desliga heater
            BitClr(PORTC, 2); //desliga cooler
            
            BitClr(T2CON, 2); //desliga o TMR2ON (bit 2) do registrador T2CON
            BitClr(PORTC, 1); //liga buzzer
            atraso_ms(2000); //gera um atraso
            BitSet(PORTC, 1); //desliga buzzer
            
            TRISB = 0x00; //configura todos os bits como saída
            BitSet(PORTB, 5); //aciona LED RB2;
            BitClr(PORTB, 6); //desaciona LED RB3;
            BitClr(PORTB, 7); //desaciona LED RB4;
        }
        
        //se a temperatura atual é maior que ao setpoint, liga o cooler
        else if(temperaturaInt > setpointInt) {
            
            temperaturaInt--; //decrementa a temperatura
            
            BitClr(PORTE, 0); //desliga relé 2
            BitClr(PORTC, 5); //desliga heater
            BitClr(PORTC, 0); //desliga relé 1
            
            pwmFrequency(1000); //frequência 2000 do pwm
            pwmSet1(pwm); //atualiza duty cycle do pwm
            
            BitSet(PORTC, 2); //liga cooler
            
            TRISB = 0x00; //configura todos os bits como saída
            BitClr(PORTB, 5); //desaciona LED RB2;
            BitSet(PORTB, 6); //aciona LED RB3;
            BitClr(PORTB, 7); //desaciona LED RB4;     
        }
        
        //se a temperatura atual é menor que ao setpoint, liga o relé e o heater
        else if(temperaturaInt < setpointInt) {
            
            temperaturaInt++; //incrementa a temperatura
            
            BitClr(T2CON, 2); //desliga o TMR2ON (bit 2) do registrador T2CON
            BitSet(PORTC, 1); //desliga buzzer
            BitClr(PORTC, 0); //desliga relé 2
            BitClr(PORTC, 2); //desliga cooler
            
            BitSet(PORTE, 0); //liga relé 1
            BitSet(PORTC, 5); //liga heater
            
            TRISB = 0x00; //configura todos os bits como saída
            BitClr(PORTB, 5); //desaciona LED RB2;
            BitClr(PORTB, 6); //desaciona LED RB3;
            BitSet(PORTB, 7); //aciona LED RB4;
                    
        }

        //*****************************************************************************************************************
    }
}

//*****************************************************************************************************************
//MENU GERAL
void menuGeral() {
    
    BitClr(T2CON, 2); //desliga o TMR2ON (bit 2) do registrador T2CON 
    BitSet(PORTC, 1); //desliga o buzzer
    BitClr(PORTC, 2); //desliga o cooler
    BitClr(PORTC, 0); //desliga o relé 1
    BitClr(PORTE, 0); //desliga o relé 2
    BitClr(PORTC, 5); //desliga o heater
    
    lcd_cmd(L_CLR); //apaga todo o display LCD
    lcd_str("SELECIONE:"); //escreve mensagem na primeira linha do LCD, centralizada
    lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD
    lcd_str("RB3-PRINCIPAL"); //escreve mensagem na segunda linha do LCD, centralizada
    lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD
    lcd_str("RB4-FINALIZAR"); //escreve mensagem na terceira linha do LCD
    lcd_cmd(L_L4); //coloca cursor na quarta linha, coluna 0 do Display LCD
    
    //atraso_ms(1000); //gera tempo de atraso 1000 milisegundos
    TRISB = 0x3F; //bits RB0 a RB5 da PORTB configurados como entrada
    while((BitTst(PORTB, 3)) && (BitTst(PORTB, 4))); //Espera um dos botões (RB3, RB4, RB5 ou RB0) serem pressionados
    
     //se o botão pressionado foi RB3
    if(BitTst(PORTB, 3) == 0) {
        
        BitClr(T2CON, 2); //desliga o TMR2ON (bit 2) do registrador T2CON 
        BitSet(PORTC, 1); //desliga o buzzer
        lcd_str("Menu Principal.."); //escreve mensagem na quarta linha do LCD
        atraso_ms(1000); //gera um atraso
        menuPrincipal(); //vai para menu principal
    }
    
    //se o botão pressionado foi RB4
    else if(BitTst(PORTB, 4) == 0) {
        
        lcd_str(" Finalizando... "); //escreve mensagem na quarta linha do LCD
        atraso_ms(1000); //gera um atraso
        lcd_cmd(L_OFF); //desliga display LCD
        
        //desliga todos os componentes da placa
        TRISB = 0x00;
        PORTB=0x00;
        TRISD = 0x00;
        PORTD=0x00;
        TRISE = 0x00;
        PORTE = 0x00; //desligando relés 
        atraso_ms(3000000000); //gera um atraso muito grande
    }
}

//*****************************************************************************************************************
//MENU DE SELEÇÃO PRINCIPAL

void menuPrincipal() {
    
    BitClr(T2CON, 2); //desliga o TMR2ON (bit 2) do registrador T2CON 
    BitSet(PORTC, 1); //desliga o buzzer
    BitClr(PORTC, 2); //desliga o cooler
    BitClr(PORTC, 0); //desliga o relé 1
    BitClr(PORTE, 0); //desliga o relé 2
    BitClr(PORTC, 5); //desliga o heater
    
    TRISB=0x00; //define todos os bits como saída
    
    lcd_cmd(L_CLR); //apaga todo o display LCD
    lcd_str("RB3-SETPOINT"); //escreve mensagem na primeira linha do LCD, centralizada
    atraso_ms(10); //gera tempo de atraso 10 milisegundos
    lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD
    lcd_str("RB4-TEMPERATURA"); //escreve mensagem na segunda linha do LCD
    lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD
    lcd_str("RB5-Kp, Ki, Kd"); //escreve mensagem na terceira linha do LCD
    lcd_cmd(L_L4); //coloca cursor na quarta linha, coluna 0 do Display LCD
    lcd_str("RB0-PROCESSO"); //escreve mensagem na quarta linha do LCD
    //atraso_ms(1000); //gera tempo de atraso 1000 milisegundos
    TRISB = 0x3F; //bits RB0 a RB5 da PORTB configurados como entrada
    while((BitTst(PORTB, 3)) && (BitTst(PORTB, 4)) && (BitTst(PORTB, 5)) && (BitTst(PORTB, 0))); //Espera um dos botões (RB3, RB4, RB5 ou RB0) serem pressionados
    
    //se o botão pressionado foi RB3
    if(BitTst(PORTB, 3) == 0) {
        BitClr(T2CON, 2); //desliga o TMR2ON (bit 2) do registrador T2CON 
        BitSet(PORTC, 1); //desliga o buzzer
        menuSetpoint(); //vai para menu de setpoint
    }
    
    //se o botão pressionado foi RB4
    else if(BitTst(PORTB, 4) == 0) {
        BitClr(T2CON, 2); //desliga o TMR2ON (bit 2) do registrador T2CON 
        BitSet(PORTC, 1); //desliga o buzzer
        menuTemperatura(); //vai para menu da temperatura
    }
    
    //se o botão pressionado foi RB5
    else if(BitTst(PORTB, 5) == 0) {
        BitClr(T2CON, 2); //desliga o TMR2ON (bit 2) do registrador T2CON 
        BitSet(PORTC, 1); //desliga o buzzer
        menuConstantes(); //vai para menu das constantes Kp, Kd, Ki
    }
       
    //se o botão pressionado foi RB0
    else if(BitTst(PORTB, 0) == 0) {
        BitClr(T2CON, 2); //desliga o TMR2ON (bit 2) do registrador T2CON 
        BitSet(PORTC, 1); //desliga o buzzer
        menuProcesso(); //vai para menu do processo
    }
}

//*****************************************************************************************************************

//*****************************************************************************************************************
//MAIN

void main() {
    
    //*****************************************************************************************************************
    //LEITURA INICIAL NO LCD
    
    TRISD = 0x00; //configura todos os pinos do PORTD como saída
    TRISE = 0x00; //configura todos os pinos do PORTE como saida
    TRISC = 0x01; //configura o pino 1 do PORTC como saida
    ADCON1 = 0x06; //configura todos os pinos de A/D como I/O de uso geral
    
    BitSet(PORTC, 1); //desliga buzzer
    
    lcd_init(); //inicializa o display LCD
    
    //*****************************************************************************************************************
    //RTC
    
    lcd_cmd(L_CLR); //apaga todo o display LCD
    lcd_cmd(L_L1); //coloca cursor na primeira linha, coluna 0 do Display LCD
    lcd_str("   BEM-VINDO!   "); //escreve mensagem na primeira linha do LCD
    lcd_cmd(L_L2); //coloca cursor na segunda linha, coluna 0 do Display LCD
    lcd_str("DATA E HORA: "); //escreve mensagem na segunda linha do LCD
    rtc_r(); //chama função RTC
    lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD
    lcd_str((const char *)date); //imprime data na tela do display LCD
    
    //mostra variação da data e horário atuais
    for(rtcLoop=0;rtcLoop<10;rtcLoop++)
    {
        rtc_r(); //chama função RTC
        lcd_cmd(L_L4); //coloca cursor na quarta linha, coluna 0 do Display LCD
        lcd_str((const char *)time); //imprime hora : minuto : segundo na tela do display LCD
        atraso_ms(500);  //gera um atraso
    }
    
    //*****************************************************************************************************************
    //INICIALIZAÇÃO 
    
    lcd_cmd(L_CLR); //apaga todo o display LCD
    
    lcd_str("  CONTROLE PID  "); //escreve mensagem na primeira linha do LCD, centralizada
    atraso_ms(10); //gera tempo de atraso 10 milisegundos
    lcd_cmd(L_L3); //coloca cursor na terceira linha, coluna 0 do Display LCD
    lcd_str("GRANDEZA:"); //escreve mensagem na terceira linha do LCD
    lcd_cmd(L_L4); //coloca cursor na quarta linha, coluna 0 do Display LCD
    lcd_str("TEMPERATURA"); //escreve mensagem na quarta linha do LCD
    atraso_ms(3000); //gera tempo de atraso 1000 milisegundos
    
    for(;;) {
        //*****************************************************************************************************************
        //MENU GERAL
        menuGeral(); //vai para menu principal do programa
        //*****************************************************************************************************************
    }
}
