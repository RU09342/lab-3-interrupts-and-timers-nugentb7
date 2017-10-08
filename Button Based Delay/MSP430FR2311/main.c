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
    PM5CTL0 &= ~LOCKLPM5;           // Disable the GPIO power-on default high-impedance mode



    P1DIR |= BIT0; // set P1.0 as output
    P1OUT &= ~BIT0; // clear LED

    P1DIR &=~(BIT1); //set P1.1 as input
    P1REN|=BIT1;//enable pull-up resistor on
    P1OUT|=BIT1;
    P1IE |=BIT1;//enable the interrupt on P1.1
    P1IES |=BIT1;//set to look for falling edge
    P1IFG &=~(BIT1);//clear interrupt flag


    timerstart(10);    // initialize timer to 10Hz

    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

void timerstart(int f) // call function with desired frequency to initialize timer module
{
    int n;
    TB0CCTL0 = CCIE; //Enable interrupt in compare mode
    TB0CTL = TBSSEL_2 + MC_1 + ID_2; //SMCLK, up mode
    n = 250000 / f; //250000 / 10 = 25000
    TB0CCR0 = n; // [( 10^6 / 4) / (25000) = 10Hz)]
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TIMER_B(void)
{
    if (enabled != 0)
    {
        if (P1IN & BIT1)         //then reset both count and enabled when the button is released
        {
            buttonPressed = 0;
            enabled = 0;
            count = 0;
        }
        //let us "loop" and increment i while the button is pressed on each timer interrupt

        else
        {
            P1OUT |= BIT0;
            i ++;
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
    __delay_cycles(50000); // prevent bouncing from interfering with the enable bit
    P1IFG &=~(BIT1);    //clear interrupt flag
}
