//This file was generated by script mkregs.py

#include "iob_eth_swreg.h"


// Base Address
static int base;
void IOB_ETH_INIT_BASEADDR(uint32_t addr) {
	base = addr;
}

// Core Setters
void IOB_ETH_SET_SEND(uint8_t value) {
	(*( (volatile uint8_t *) ( (base) + (ETH_SEND) ) ) = (value));
}

void IOB_ETH_SET_RCVACK(uint8_t value) {
	(*( (volatile uint8_t *) ( (base) + (ETH_RCVACK) ) ) = (value));
}

void IOB_ETH_SET_SOFTRST(uint8_t value) {
	(*( (volatile uint8_t *) ( (base) + (ETH_SOFTRST) ) ) = (value));
}

void IOB_ETH_SET_DUMMY_W(uint32_t value) {
	(*( (volatile uint32_t *) ( (base) + (ETH_DUMMY_W) ) ) = (value));
}

void IOB_ETH_SET_TX_NBYTES(uint16_t value) {
	(*( (volatile uint16_t *) ( (base) + (ETH_TX_NBYTES) ) ) = (value));
}

void IOB_ETH_SET_DATA_WR(uint32_t addr, uint8_t value) {
	*(((volatile uint8_t *) (base + ETH_DATA_WR)) + addr) = value;
}


// Core Getters
uint32_t IOB_ETH_GET_STATUS() {
	return (*( (volatile uint32_t *) ( (base) + (ETH_STATUS) ) ));
}

uint32_t IOB_ETH_GET_DUMMY_R() {
	return (*( (volatile uint32_t *) ( (base) + (ETH_DUMMY_R) ) ));
}

uint32_t IOB_ETH_GET_CRC() {
	return (*( (volatile uint32_t *) ( (base) + (ETH_CRC) ) ));
}

uint16_t IOB_ETH_GET_RCV_SIZE() {
	return (*( (volatile uint16_t *) ( (base) + (ETH_RCV_SIZE) ) ));
}

uint8_t IOB_ETH_GET_DATA_RD(uint32_t addr) {
	return *(((volatile uint8_t *) (base + ETH_DATA_RD)) + addr);
}

