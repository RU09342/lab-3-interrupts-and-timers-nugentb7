/**
 * main.c
 Blink LED with TimerA interrupt
 MSP430FR6989
Brendan Nugent
 */

#include <msp430.h>

unsigned int count = 0;
void timerstart(int f);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;           // Disable high-impedance


    P1DIR |= BIT0; // set P1.0 as output
    P1OUT &= ~BIT0; // clear LED

    timerstart(10);    // initialize timer to 10Hz

    __enable_interrupt();

   __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

void timerstart(int f) // call function with desired frequency to initialize timer module
{
    int n;
    TA0CCTL0 = CCIE; //Enable interrupt in compare mode
    TA0CTL = TASSEL_2 + MC_1 + ID_2; //SMCLK, up mode
    n = 250000 / f; //250000 / 10 = 25000
    TA0CCR0 = n; // [( 10^6 / 4) / (25000) = 10Hz)]
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER_A(void)
{

    if (count == 50) //toggle every 50 interrupts or 50 * .1 = 5 s
    {
        P1OUT ^= 0x01; //Toggle LED
        count = 0;  // reset count
    }

    else
        count++;    // increment count each interrupt until count = 50
}
