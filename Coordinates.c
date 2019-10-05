#include <stdio.h>
#include "Coordinates.h"
#include "MCP25625.h"

//
void init_coords(void)
{
	int mask=0x7F8;
	int filter=0x100;
	MCP25625_init(0,mask,filter);
}

//
bool sendCoords(coords coord2send)
{
	char temp[8];
	if(coord2send.coordType==ROLIDO)
		temp[0]='R';
	else if(coord2send.coordType==CABECEO)
		temp[0]='C';
	else if(coord2send.coordType==ORIENTACION)
		temp[0]='O';
	for(int i=0;i<coord2send.data_len;i++)
	{
		temp[1+i]=coord2send.data[i];
	}
	MCP25625_send(coord2send.ID,temp, coord2send.data_len+1,0);
	return true;
}

//
bool receiveCoord(coords * coord2receive)
{
	int tempID;
	int tempLen;
	char tempData[8];
	bool hay_algo;
	hay_algo= receiveFromCAN(&tempID,tempData,&tempLen);
	if(hay_algo)
	{
		coord2receive->ID=tempID;
		coord2receive->data_len=tempLen;
		if(tempData[0]=='C')
			coord2receive->coordType=CABECEO;
		else if(tempData[0]=='R')
			coord2receive->coordType=ROLIDO;
		else if(tempData[0]=='O')
			coord2receive->coordType=ORIENTACION;
		for(int i=0;i<tempLen-1;i++)
		{
			coord2receive->data[i]=tempData[i+1];
		}
		return true;
	}
	else
		return false;

}
