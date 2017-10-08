/**
 *
 Blink LED with different speeds controlled by pressing onboard button
 MSP430FR5994
Brendan Nugent
 */

#include <msp430.h>

volatile unsigned int speed = 1;

void main(void)
{
WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer

P1DIR |=BIT0; //set P1.0 as output
P5DIR &=~(BIT6); //set P5.6 as input
P5REN|=BIT6;//enable pull-up resistor
P5OUT|=BIT6;
P5IE |=BIT6;//enable the interrupt on P5.6
P5IES |=BIT6;//set to look for falling edge
P5IFG &=~(BIT6);//clear interrupt flag

__enable_interrupt(); //_BIS_SR(LPM4_bits + GIE); //enter LPM4 mode and enable global interrupt

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
            P1OUT &= 0x00; // if speed = 0 turn off
    }
}

//Port 1 ISR
#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void)
{

P5IE &= ~BIT6; // disable interrupt to prevent another interrupt from interfering
if (speed != 3) // increment speed
    speed++;
else            // loop back to speed =0
    speed = 0;
while(P5IN & BIT6) {} //hold until button is released
__delay_cycles(100000); //additional delay to avoid bouncing toggling LED


P5IE |= BIT6;   // enable interrupt again
P5IFG &=~(BIT6);    //clear interrupt flag
}

