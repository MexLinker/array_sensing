// soft_spi.h
#ifndef SOFT_SPI_H
#define SOFT_SPI_H

// should i do this?

#include "ch32v20x.h"

//#include "_stdint.h"



// #include "stm32f10x.h"

void Soft_SPI_Init(void);
void Soft_SPI_Start(void);
void Soft_SPI_Stop(void);
void Soft_SPI_Write(uint16_t data);
void writeToADG714(uint8_t chip1Data, uint8_t chip2Data);

void turn_off_Nth_sw(uint16_t n);

void MX_Init(void);

uint8_t re_arrange_MX(uint8_t n);

void MX_turn_n_on(uint8_t n);

void MOAA_GPIO_Init();

void MOAA_GPIO_turn_n_on(uint16_t n);

#endif // SOFT_SPI_H
