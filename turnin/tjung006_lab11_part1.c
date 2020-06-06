/*      Author: terry
 *  Partner(s) Name:
 *      Lab Section:
 *      Assignment: Lab #  Exercise #
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "bit.h"
#include "io.h"
#include "keypad.h"
#include "scheduler.h"
#include "timer.h"
#endif

enum sm_states {press};
int smTick(int state){
	unsigned char x;
	switch(state){
		case press:
			state = press;
			break;
	}
	switch(state){
		case press:
			x = GetKeypadKey();
				switch (x) {
					case '\0': 
						PORTB = 0x1F; 
						break;
					case '1': 
						PORTB = 0x01; 
						break; 
					case '2': 
						PORTB = 0x02; 
						break;
					case '3': 
						PORTB = 0x03; 
						break;
					case '4': 
						PORTB = 0x04; 
						break;
					case '5': 
						PORTB = 0x05; 
						break;
					case '6': 
						PORTB = 0x06; 
						break;
					case '7': 
						PORTB = 0x07; 
						break;
					case '8': 
						PORTB = 0x08; 
						break;
					case '9': 
						PORTB = 0x09; 
						break;
					case 'A': 	
						PORTB = 0x0A;
						break;
					case 'B': 
						PORTB = 0x0B; 
						break;
					case 'C': 
						PORTB = 0x0C; 
						break;
					case 'D': 
						PORTB = 0x0D; 
						break;
					case '*': 
						PORTB = 0x0E; 
						break;
					case '0': 
						PORTB = 0x00; 
						break;
					case '#': 
						PORTB = 0x0F; 
						break;
					default: 
						PORTB = 0x1B; 
						break;
				}
			break;	
	}
	return state;
}

int main(){
	DDRC = 0xF0; PORTC = 0x0F;//input
	DDRB = 0xFF; PORTB = 0x00;//output

	static task task1;
	task *tasks[] = {&task1};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = 0;

	task1.state = start;
	task1.period = 10;
	task1.elapsedTime = 10;
	task1.TickFct = &smTick;

	 unsigned long int GCD = 10;

	TimerSet(GCD);
	TimerOn();

	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	// Error: Program should not exit!
	return 0;
}












