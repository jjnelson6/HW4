/* 
 * File:   SPI.c
 * Author: Jovan Nelson
 *
 * Created on April 16, 2019, 6:17 PM
 */

//#include "NU32.h"       // constants, funcs for startup and UART
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h> 	//for sine wave plotting
// Demonstrates spi by accessing external ram
// PIC is the master, DAC is the device

// 
// 
//
#define CS LATAbits.LATA0       // chip select pin

// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
  SPI1BUF = o;

  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}

// initialize spi4 and the ram module
void init_spi() {
  // set up the chip select pin as an output
  // the chip select pin is used by the DAC to indicate
  // when a command is beginning (clear CS to low) and when it
  // is ending (set CS high)
  
  TRISAbits.TRISA0 = 0;
  CS = 1;

  // since the pic is just starting, we know that spi is off. We rely on defaults here
 
  // setup spi1 , all bits must be changed for SPI1 from SPI4
  SPI1CON = 0;              // turn off the spi module and reset it
  SPI1BUF;                  // clear the rx buffer by reading from it
  SPI1BRG = 0x1;            // baud rate to 50 MHz [SPI4BRG = (50000000/(2*desired))-1]
  SPI1STATbits.SPIROV = 0;  // clear the overflow bit
  SPI1CONbits.CKE = 1;      // data changes when clock goes from hi to lo (since CKP is 0)
  SPI1CONbits.MSTEN = 1;    // master operation
  SPI1CONbits.ON = 1;       // turn on spi 1
  SP1CONbits.MODE32 = 0;
  SP1CONbits.MODE16 = 0
       

                            // send a ram set status command.
  CS = 0;                   // enable the ram
  spi_io(0x01);             // DAC write status
  spi_io(0x41);             // sequential mode (mode = 0b01), hold disabled (hold = 0)
  CS = 1;                   // finish the command
}



int main(void) {
  unsigned short addr1 = 0x1234;                  // the address for writing the ram
  char data[] = "Help, I'm stuck in the RAM!";    // the test message
  char read[] = "***************************";    // buffer for reading from ram
  char buf[100];                                  // buffer for comm. with the user
  unsigned char status;                           // used to verify we set the status 
 

  // check the DAC status
  CS = 0;
  spi_io(0x5);                                      //  read status command
  status = spi_io(0);                               // the actual status
  CS = 1;


	init_spi();

  while(1) {
	_CPO_SET_COUNT(0);
	float f = 512 +512*sin(i*2*3.1415/1000*10);  //should make a 10Hz sin wave)
	i++;



	setVoltage(0,255);		//test
	setVoltage(1,255);		//test

	while(_CPO_GET_COUNT() < 2400000000/1000) {}  //check this is 24Million
    ;
  }
  return 0;
}


void setVoltage(char a, int v) {
	unsigned short t = 0;
	t= a << 15; //a is at the very end of the data transfer
	t = t | 0b01110000000000000;
	t = t | ((v&0b1111111111) <<2); //rejecting excessive bits (above 10)
	
	CS = 0;
	spi_io(t>>8);
	spi_
	
}

