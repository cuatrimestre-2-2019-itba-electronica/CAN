/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include "MCP25625.h"
#include "board.h"
#include "gpio.h"



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

//static void delayLoop(uint32_t veces);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	MCP25625_init(0);

}

/* Función que se llama constantemente en un ciclo infinito */

void App_Run (void)
{
	/*char databuffer1[1]={1};
	MCP25625_send(0x104,databuffer1, 1);
	char databuffer2[2]={1,2};
	MCP25625_send(0x104,databuffer2, 2);
	char databuffer3[3]={1,2,3};
	MCP25625_send(0x104,databuffer3, 3);
	char databuffer4[4]={1,2,3,4};
	MCP25625_send(0x104,databuffer4, 4);
	char databuffer5[5]={1,2,3,4,5};
	MCP25625_send(0x104,databuffer5, 5);*/
}


/*******************************************************************************
 ******************************************************************************/
