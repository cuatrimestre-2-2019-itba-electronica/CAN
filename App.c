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
#include "Coordinates.h"
#include "MCP25625.h"
#include "board.h"



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
	//init_coords();
	int mask=0x7F8;
	int filter=0x100;
	MCP25625_init(0,mask,filter);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	//Primero pruebo si funciona MCP25625_SEND, DESP SI FUNCA SEND2CAN, DESP SI FUNCA SENDCOORD
	coords temp;
	char dato[3]={'+','4','5'};
	temp.ID=0x104;
	temp.coordType=ORIENTACION;
	temp.data_len=3;
	for(int i=0;i<3;i++)
		temp.data[i]=dato[i];
	//UN SYSTICK
	sendCoords(temp);
	/*if(receiveCoord(&temp))
	{
		int a=3;
	}*/
}


/*******************************************************************************
 ******************************************************************************/
