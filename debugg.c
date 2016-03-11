
// For main set-up of uart:
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00

// For delay
#define F_CPU 16000000L

// Libraries:
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"


// Testing:
#define LED_ON		(PORTD	|= (1<< PD6))	// Toggle Teensy LED on.
#define LED_OFF		(PORTD	&= ~(1<<PD6))	// Toggle Teensy LED off.

//Function declarations:
void init(void);
void port_init(void);
void ValidateData(void);
void set_PCR(void);
int usart_putchar_printf(char var, FILE *stream);

// Global Variables:
uint8_t input;
uint8_t denTime=99;
uint8_t denTemp;
uint8_t annTime;
uint8_t annTemp;
uint8_t eloTime;
uint8_t eloTemp;
char passData[19];


int main(void)
{
    //initialize
    init();
	//char temp;
    while(1){
		
		ValidateData();
		printStr("Back in main \n");	//send
		//_delay_ms(3000);
		set_PCR();
    }
	
    return 0;
}

/**INITIALIZE**/
void init(void){

	//set up pins
	port_init();
	//set up the cpu prescale
	CPU_PRESCALE(CPU_16MHz);
	//USART
	uart_init();
}

void port_init(void){
	
	DDRB |= (1<<PB1) ; // Sets the data direction for Port B PIN 1 (Relay Control) as an output.
	DDRF |= (0<<PF0) | (0<<PF1); // Sets the data direction for port F, PINS 0 (ADC in), 1 (Low Battery Signal) as an input.
	DDRD |= (1<<6); // Sets the data direction for port D , PIN 6 (Teensy LED) as an output.
	
	PORTB = 0b00000000; // Sets all PB pin values low.
	PORTF = 0b00000000; // Sets all PF pins low.
	PORTD = 0b00000000; // Sets all PD pins low.
}

void ValidateData(void){
	char getValue = 'a';		// Receive Variable
	char newLine ='#';
	int valid = 0;			// Control Variable
	passData[18] = '\n';	// Array to pass values.
	int i = 0;			// Iterator for array.
	char istrue ='Y';
	int j;
	
	// For testing:
	printStr("Entered Validate Data Function \n"); //send
	//_delay_ms(5000);
	
	
	while(valid == 0){					// Control Structure for validating data values:
		printStr("Input is \n");	//send
		//_delay_ms(5000);
		while (getValue != newLine)		// Takes in all 18 values send and confirms that the values are correct:
		{
			getValue = uart_getchar();	// Receive 6 values with 3 char per value.
			uart_putchar(getValue);		// Transmit them back to the App.
			passData[i] = getValue;		// Put value into array.
			i++;						// Shift array.
			
		}
		uart_putchar('\n');				// After all the char are send needs a delimiter.
		//_delay_ms(5000);				// Pause for app to check
		
		
		printStr("Array is? \n"); //send
		//_delay_ms(5000);				// Pause for app to check
		for (j=0; j<19; j++)
		{
			uart_putchar(passData[j]);
		}
		uart_putchar('\n');				// After all the char are send needs a delimiter.
		//_delay_ms(5000);				// Pause for app to check
		
		
		getValue = 'Y';
		printStr("Is data correct? \n"); //send
		//_delay_ms(5000);
		//getValue = uart_getchar();		// Receive "Y" for correct, else retransmit.
		if (getValue == istrue)			// Check with app if values were valid.
		{
			valid = 1;					// Exit value for control structure.
		}
	}
}

void set_PCR(void){
	// Control Variable
	int hundreds;
	int tens;
	int ones;
	
	printStr("In set array is \n"); //send
	//_delay_ms(5000);				// Pause for app to check
	for (int j=0; j<19; j++)
	{
		uart_putchar(passData[j]);
	}
	uart_putchar('\n');				// After all the char are send needs a delimiter.
	//_delay_ms(5000);				// Pause for app to check
	
	// Set Denaturation Temperature:
	hundreds = passData[0]- '0';
	tens = passData[1]- '0';
	ones = passData[2]- '0';
	
	denTemp = hundreds * 100 + tens * 10 + ones;	// Shifts hundreds place value into time variable.
	printStr("Denaturing temp is "); //send App
	uart_putchar(denTemp);
	uart_putchar('\n');
	_delay_ms(5000);
	
	// Set Denaturation Time:
	hundreds = passData[3]- '0';
	tens = passData[4]- '0';
	ones = passData[5]- '0';
	
	denTime = hundreds * 100 +tens * 10 + ones;	// Shifts hundreds place value into time variable.
	printStr("Denaturing time is "); //send App
	uart_putchar(denTime);
	uart_putchar('\n');
	_delay_ms(5000);
	
	// Set Annealing Temperature:
	hundreds = passData[6]- '0';
	tens = passData[7]- '0';
	ones = passData[8]- '0';
	
	annTemp = hundreds * 100 +tens * 10 + ones;	// Shifts hundreds place value into time variable.
	printStr("Annealing temp is "); //send App
	uart_putchar(annTemp);
	uart_putchar('\n');
	_delay_ms(5000);
	
	// Set Annealing Time:
	hundreds = passData[9]- '0';
	tens = passData[10]- '0';
	ones = passData[11]- '0';
	
	annTime = hundreds * 100 +tens * 10 + ones;	// Shifts hundreds place value into time variable.
	printStr("Annealing time is "); //send App
	uart_putchar(annTime);
	uart_putchar('\n');
	_delay_ms(5000);
	
	// Set Elongation Temperature:
	hundreds = passData[12]- '0';
	tens = passData[13]- '0';
	ones = passData[14]- '0';
	
	eloTemp = hundreds * 100 +tens * 10 + ones;	// Shifts hundreds place value into time variable.
	printStr("Elongation temp is "); //send App
	uart_putchar(eloTemp);
	uart_putchar('\n');
	_delay_ms(5000);
	
	// Set Elongation Time:
	hundreds = passData[15]- '0';
	tens = passData[16]- '0';
	ones = passData[17]- '0';
	
	eloTime = hundreds * 100 +tens * 10 + ones;	// Shifts hundreds place value into time variable.
	printStr("Elongation time is "); //send App
	uart_putchar(eloTime);
	uart_putchar('\n');
	_delay_ms(5000);
}

