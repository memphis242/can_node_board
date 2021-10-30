/*
 * File:   ccp.c
 * Author: Abdullah Almosalami
 *
 * Created on October 28, 2021, 10:35 PM
 */


#include <xc.h>
#include "ccp.h"
#include "timer.h"


// Global and static variables relevant to CCP operation



// Functions

// CCP2
void CCP2_Init_Default(void){
    CCP2M_DEFAULT;
}

void CCP2_Compare_Val(uint16_t comp_val){
    CCPR2 = comp_val;    
}