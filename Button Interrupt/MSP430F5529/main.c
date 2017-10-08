/**
 *
 Blink LED with different speeds controlled by pressing onboard button
 MSP430F5529
Brendan Nugent
 */

#include <msp430.h>

volatile unsigned int speed = 1;

void main(void)
{
WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer
P1SEL = 0;
P1DIR |=BIT0; //set P1.0 as output
P1DIR &=~(BIT1); //set P1.1 as input
P1REN|=BIT1;//enable pull-up resistor
P1OUT|=BIT1;
P1IE |=BIT1;//enable the interrupt on P1.1
P1IES |=BIT1;//set to look for falling edge
P1IFG &=~(BIT1);//clear interrupt flag

__enable_interrupt();

    while(1) //loop infinitely
    {
        if (speed > 0)
        {
            P1OUT ^= BIT0;
            switch(speed) // toggle LED speeds
            {
                case 1: __delay_cycles(200000);
                case 2: __delay_cycles(100000);
                case 3: __delay_cycles(75000);
            }
        }
        else
            P1OUT &= ~BIT0; // if speed = 0 turn off
    }
}

//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{

if (speed != 3) // increment speed
    speed++;
else            // loop back to speed =0
    speed = 0;
while(P1IN & BIT1) {} //hold until button is released
__delay_cycles(100000); //additional delay to avoid bouncing toggling LED
P1IFG &=~(BIT1);    //clear interrupt flag
}
