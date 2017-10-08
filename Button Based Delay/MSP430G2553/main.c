/**
 * main.c
 Blink LED with TimerA interrupt -- button based delay
 MSP430G2553
Brendan Nugent
 */

#include <msp430.h>

unsigned int buttonPressed;
unsigned int enabled = 0x00;
unsigned int count = 0;
unsigned int i = 0;

void timerstart(int f);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer



    P1DIR |= BIT0; // P1.0 as output
    P1OUT &= ~BIT0; // clear LED

    P1DIR &=~(BIT3); //set P1.3 as input
    P1REN|=BIT3;//enable pull-up resistor on
    P1OUT|=BIT3;
    P1IE |=BIT3;//enable interrupt on P1.3
    P1IES |=BIT3;//set to look for falling edge
    P1IFG &=~(BIT3);//clear interrupt flag


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
    if (enabled != 0)
    {
        if (P1IN & BIT3)         //then reset both count and enabled when the button is released
        {
            buttonPressed = 0;  //state the button is no longer pressed
            enabled = 0;    //reset our variables
            count = 0;
        }
        //let us "loop" and increment i while the button is pressed on each timer interrupt

        else
        {
            P1OUT |= BIT0;  // otherwise keep the LED on to indicate that we recognize the hold
            i ++;   // and increment i
        }
    }

    else //otherwise increment count until it hits i, then it will toggle the LED
    {
        if (count == i) //toggle every i interrupts or i * .1 = y s
        {
            P1OUT ^= 0x01; //Toggle LED
            count = 0;  // reset count
        }

        else
            count++;    // increment count each interrupt until count = i
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    enabled = 1;//enable will be set when button is pressed and reset when it's released in the TIMERA interrupt
    i = 0; //reset our timing counter
    buttonPressed = 1;
    __delay_cycles(50000); // prevent bouncing from interfering
    P1IFG &=~(BIT3);    //clear interrupt flag
}