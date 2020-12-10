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

#ifndef DISPLAYSETESEGMENTOS_H
    #define	DISPLAYSETESEGMENTOS_H

    void configura(void);

    void iniciaDisplays7Seg(void);

    void ligaDisplay7Seg(int display);

    void atraso_display (int delay);

    void pararDisplay(void);

#endif	/* DISPLAYSETESEGMENTOS_H */

