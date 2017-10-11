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
unsigned int i = 1;

void timerstart(int f);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;           // Disable the GPIO power-on default high-impedance mode


    P1DIR |= BIT0; // P1.0 as output
    P1OUT &= ~BIT0; // clear LED

    P5DIR &=~(BIT6); //set P5.6 as input
    P5REN|=BIT6;//enable pull-up resistor on
    P5OUT|=BIT6;
    P5IE |=BIT6;//enable the interrupt on P5.6
    P5IES |=BIT6;//set to look for falling edge
    P5IFG &=~(BIT6);//clear interrupt flag


    timerstart(20);    // initialize timer to 10Hz

    __enable_interrupt();
}

void timerstart(int f) // call function with desired frequency to initialize timer module
{
    int n;
    TA0CCTL0 = CCIE; //Enable interrupt in compare mode
    TA0CTL = TASSEL_2 + MC_1 + ID_2; //SMCLK, up mode
    n = 250000 / f; //250000 / 10 = 12000
    TA0CCR0 = n; // [( 10^6 / 4) / (12500) = 20Hz)]
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER_A(void)
{
    if (enabled != 0)
    {
        if (P5IN & BIT6)         //then reset both count and enabled when the button is released
        {
            buttonPressed = 0; // state the button is no longer pressed
            enabled = 0; //reset our variables
            count = 0;
        }
        //let us "loop" and increment i while the button is pressed on each timer interrupt

        else
        {
            P1OUT |= BIT0; //hold LED on
            i ++; //increment our comparative counter
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

#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void)
{
    enabled = 1;//enable will be set when button is pressed and reset when it's released in the TIMERA interrupt
    i = 0; //reset our timing counter
    buttonPressed = 1; //state that the button is pressed
    __delay_cycles(100000); // prevent bouncing from interfering
    P5IFG &=~(BIT6);    //clear interrupt flag
}
