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

#include "itoa.h"
#include "pic18f4520.h"

void itoa(int val, char* str)
{
  int i=0;
    
  if(val >= 0) {
   
    if(val<10) {
        str[0]=(val%10)+0x30;
        str[1]=' ';  
        str[2]=' '; 
        str[3]=' '; 
        str[4]=' '; 
        str[5]=' '; 
        str[6]=' ';
        
    }
    else if(val>=10 && val<100) {
        str[0]=((val%100)/10)+0x30; 
        str[1]=(val%10)+0x30;
        str[2]=' '; 
        str[3]=' '; 
        str[4]=' '; 
        str[5]=' '; 
        str[6]=' '; 
        
    }
    else if(val>=100 && val<1000) {
        str[0]=((val%1000)/100)+0x30; 
        str[1]=((val%100)/10)+0x30; 
        str[2]=(val%10)+0x30;
        str[3]=' '; 
        str[4]=' '; 
        str[5]=' '; 
        str[6]=' '; 
        
    }
    
  }
  else if(val < 0) {
    int valPositivo = (val*(-1));
    //str[0]=0x2D;
    //str[1]=(valPositivo/10000)+0x30;  
    //str[2]=((valPositivo%10000)/1000)+0x30;  
    //str[3]=((valPositivo%1000)/100)+0x30;  
    //str[4]=((valPositivo%100)/10)+0x30;
    //str[5]=(valPositivo%10)+0x30;
    //str[6]=0;
    
    if(valPositivo<10) {
        str[0]=0x2D;
        str[1]=(valPositivo%10)+0x30;
        str[2]=' ';  
        str[3]=' '; 
        str[4]=' '; 
        str[5]=' '; 
        str[6]=' '; 
        
    }
    else if(valPositivo>=10 && valPositivo<100) {
        str[0]=0x2D;
        str[1]=((valPositivo%100)/10)+0x30; 
        str[2]=(valPositivo%10)+0x30;
        str[3]=' '; 
        str[4]=' '; 
        str[5]=' '; 
        str[6]=' '; 
        
    }
    else if(valPositivo>=100 && valPositivo<1000) {
        str[0]=0x2D;
        str[1]=((valPositivo%1000)/100)+0x30; 
        str[2]=((valPositivo%100)/10)+0x30; 
        str[3]=(valPositivo%10)+0x30; 
        str[4]=' '; 
        str[5]=' ';
        str[6]=' '; 
        
    }
    else if(valPositivo==1000) {
        str[0]=0x2D;
        str[1]=((valPositivo%10000)/1000)+0x30;  
        str[2]=((valPositivo%1000)/100)+0x30; 
        str[3]=((valPositivo%100)/10)+0x30; 
        str[4]=(valPositivo%10)+0x30; 
        str[5]=' ';
        str[6]=' '; 
        
    }
    
  }
      
}
