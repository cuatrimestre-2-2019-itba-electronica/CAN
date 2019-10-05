/*
 * SPI_driver.h
 *
 *  Created on: Sep 20, 2019
 *      Author: gonzalosilva
 */

#ifndef SPI_H_
#define SPI_H_

#include<stdint.h>
#include<stdbool.h>
void SPI_driver_init (void);
uint8_t SPI_driver_sendReceive(char * data2end, char size,char * recivedData);
bool SPI_driver_dataSended(void);
bool SPI_driver_availableDataRecived(void);
uint8_t SPI_driver_getData(uint8_t * dataRecived);


#endif /* SPI_DRIVER_H_ */
