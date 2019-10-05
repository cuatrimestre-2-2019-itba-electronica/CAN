#include"SPI.h"
#include <stdio.h>
#include "gpio.h"
#include "board.h"
#include "MCP25625.h"
#include "buffer.h"

#define DLC 7

static void MCP25625_RESET(void);

static void MCP25625_BIT_MODIFY(char address, char mask, char data);

static void MCP25625_WRITE(char address, char * buffer, int bufflen);

static void MCP25625_READ(char address, char * buffer, int bufflen);

static void MCP25625_READ_RX_BUFFER(int buff_num, bool full_or_half, char * buffer);

static void MCP25625_RTS(char buff2send);

static circ_bbuf_t recBuff;

void irqt(void);

void irqt(void)
{
	CANmsg tempData;

	char readf[30];
	char bufferaux[3];
	if(recBuff.head==15)
	{
		int a=3;
	}

	/*MCP25625_READ(CANINTF,bufferaux,1);
	char temp=bufferaux[2];
	if (MCP25625_isRbuffer_full(0,temp))*/
	{
		//aca hay que hacer un push
		MCP25625_READ(RXBXCTRL(0),readf, 16);
		char temp=0;
		MCP25625_WRITE(CANINTF,&temp,1);//reseteo rl flag
		tempData.id=(readf[3]<<3)+(readf[4]>>5);

		tempData.size=readf[DLC];
		for(int i=DLC+1;i<DLC+1+tempData.size;i++){
			tempData.data[i-(DLC+1)]=readf[i];
			}

		push_buffer(& recBuff, tempData);
	}
}


void MCP25625_RESET(void)
{
	char frame2send = RESET;
	SPI_driver_sendReceive(&frame2send, 1,NULL);

	//SPI_driver_sendRecive(uint8_t * data2end, uint8_t size,uint8_t * recivedData);
	//SPI_send_frame(RESET,1);
}

void MCP25625_BIT_MODIFY(char address, char mask, char data)
{
	char buffer[4];
	buffer[0]=BIT_MODIFY;
	buffer[1]=address;
	buffer[2]=mask;
	buffer[3]=data;
	SPI_driver_sendReceive(buffer,4,NULL);
}

void MCP25625_WRITE(char address, char * buffer, int bufflen)
{
	char send_buff[MAX_BUFF_LEN];
	send_buff[0]=WRITE;
	send_buff[1]=address;

	for(int i=0;(i<bufflen)&&(i<MAX_BUFF_LEN);i++)
	{
		send_buff[2+i]=buffer[i];
	}
	SPI_driver_sendReceive(send_buff,bufflen+2,NULL);
}

void MCP25625_READ(char address, char * buffer, int bufflen)
{
	char send_buff[MAX_BUFF_LEN];
	send_buff[0]=READ;
	send_buff[1]=address;

	for(int i=0;(i<bufflen-2)&&(i<MAX_BUFF_LEN);i++)
	{
		send_buff[2+i]=NO_COMMAND;
	}
	SPI_driver_sendReceive(send_buff,bufflen,buffer);
}
//buff2send debe ser un 1,2,4 dependiendo que buffer se desea transmitir. Si es 0 no se transmite nada.
void MCP25625_RTS(char buff2send)
{
	char frame2send=0;
	frame2send=RTS+buff2send;
	//SPI_send_frame(RTS+ID);
	SPI_driver_sendReceive(& frame2send,1,NULL);
}

//Oscilador de 16MHz
//Tengo 8 time quantas por bit
//BRP tiene que valer 7
void MCP25625_init(int ID)
{
	init_buffer(&recBuff,BUFFER_SIZE);
	SPI_driver_init();
	MCP25625_RESET();//reseteo el controlador y lo pongo en modo configuración
	MCP25625_BIT_MODIFY(CANCTRL,REQOP_MASK|OSM_MASK|CLKEN_MASK|CLKPRE_MASK,REQOP_DATA(CONFIGURATION)|OSM_DATA(0)|CLKEN_DATA(1)|CLKPRE_DATA(0));
	//Seteo el bitrate y los time quantas
	MCP25625_BIT_MODIFY(CNF1,SJW_MASK|BRP_MASK,SJW_DATA(SJW-1) | BRP_DATA(BAUDRATE_PRESCALER));

	MCP25625_BIT_MODIFY(CNF2,BTLMODE_MASK|SAM_MASK|PHSEG1_MASK|PRSEG_MASK,BTLMODE_DATA(BTLMODE)|SAM_DATA(SAM)|PHSEG1_DATA(PHSEG1-1)|PRSEG_DATA(PRSEG-1));

	MCP25625_BIT_MODIFY(CNF3,WAKFIL_MASK| PHSEG2_MASK,WAKFIL_DATA(WAKFIL)| PHSEG2_DATA(PHSEG2-1));

	int mask=0x7F8;
	char auxbuffer[3];
	auxbuffer[0]=(char)(0xFF &(mask>>3));
	auxbuffer[1]=(char)((mask<<5)&0xFF);

	MCP25625_WRITE(RXMXSIDH(0),auxbuffer,2);

	auxbuffer[0]=0x20;
	auxbuffer[1]=0x00;


	MCP25625_WRITE(RXF0SIDH,auxbuffer,2);

	MCP25625_BIT_MODIFY(RXBXCTRL(0),RECEIVE_MODE_MASK|BUKT_MASK,RECEIVE_MODE_MASK|BUKT_DATA(0));

	MCP25625_BIT_MODIFY(CANINTE,RX0IE_MASK,RX0IE_DATA(1));

	MCP25625_BIT_MODIFY(CANCTRL,REQOP_MASK|OSM_MASK|CLKEN_MASK|CLKPRE_MASK,REQOP_DATA(NORMAL)|OSM_DATA(0)|CLKEN_DATA(1)|CLKPRE_DATA(0));//pongo en modo normal, habilito el clock y prescaler de 1.

	gpioMode(IRQ_CAN, INPUT_PULLUP);
	gpioIRQ(IRQ_CAN, GPIO_IRQ_MODE_FALLING_EDGE, irqt);
}

void MCP25625_send(int ID,char * databuffer, int bufflen)//por ahora lo hago solo con el buffer 0
{
//setear el DLC, el RTR, el ID, los datos
/*
*PARA HACER ESTO TENGO QUE MODIFICAR LOS REGISTROS QUE ARRANCAN CON TXB0, EN TXBODN POR EJEMPLO TENGO LOS BYTES QUE QUIERO MANDAR
*/
//Me fijo si el buffer no esta mandando algo
	if(bufflen>8)
		bufflen=8;

	char bufferaux[2];
	MCP25625_READ(TXBXCTRL(0),bufferaux,1);
	{
		bufferaux[0]=(char)(0xFF &(ID>>3));
		bufferaux[1]=(char)((ID&0x07)<<5);
		MCP25625_WRITE(TXBXSIDH(0), bufferaux,2);//Seteo el ID a mandar
		bufferaux[0]=bufflen;
		MCP25625_WRITE(TXBXDLC(0), bufferaux,1);//seteo dataframe y data len

		MCP25625_WRITE(TXBXD0(0), databuffer,bufflen);
		MCP25625_RTS(1);//mando el comando RTS y le sumo 1 si quiero el primer buffer, 2 si quiero el segundo, 4 si quiero el tercero
	}
}

bool MCP25625_isTbuffer_empty(int buff_num, char rgstr)
{
	bool retVal=false;
	char bufferaux[3];
	if(buff_num==0)
	{
		if(rgstr&(char)TX0IF_MASK)
			retVal=true;
	}
	else if(buff_num==1)
	{
		if(rgstr&(char)TX1IF_MASK)
			retVal=true;

	}
	else if(buff_num==2)
	{
		if(rgstr&(char)TX2IF_MASK)
			retVal=true;

	}
	return retVal;
}

//
bool MCP25625_isRbuffer_full(int buff_num,char rgstr)
{
	bool retVal=false;
	char bufferaux[3];
	if(buff_num==0)
	{
		if(rgstr&(char)RX0IF_MASK | (rgstr==0))
			retVal=true;
	}
	else if(buff_num==1)
	{
		if(rgstr&(char)RX1IF_MASK | (rgstr==0))
			retVal=true;
	}

	return retVal;
}
