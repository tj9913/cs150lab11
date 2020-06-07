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

unsigned char a;
unsigned char b;

enum press_states{press};
int pressTick(int state){
	switch(state){
		case press:
			state = press;
			break;
		default:	
			state = press;
			break;
	}
	switch(state){
		case press:
			a = GetKeypadKey();
			break;
		default:
			break;
	}
	return state;
}

enum display_state{display};
int displayTick(int state){
	switch(state){
		case display:
			state = display;
			break;
		default:
			state = display;
			break;
	}
	switch(state){
		case display:
			if(a != '\0'){
				LCD_Cursor(1);
				LCD_WriteData(a);
			}
		default:
		break;
	}
	return state;
}
int main(){
	DDRD = 0xFF; PORTD = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	static task task1, task2;
	task *tasks[] = {&task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	const char start = -1;

	task1.state = start;
	task1.period = 1;
	task1.elapsedTime = 1;
	task1.TickFct = &pressTick;
	
	task2.state = start;
	task2.period = 1;
	task2.elapsedTime = 1;
	task2.TickFct = &displayTick;

	unsigned long int GCD = 10;//half a second

	TimerSet(GCD);
	TimerOn();
	LCD_init();
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












