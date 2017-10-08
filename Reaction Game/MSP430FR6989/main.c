#include <msp430.h> 


/**
 * main.c
 * Reaction Game
 * Author: Brendan Nugent
 * To play :
 * Allow LEDs to blink three times --> The game will begin when the LEDs stop blinking and hold on
 * Press your respective button as fast as you can! The first to press their button 20 times wins!
 * The winner is indicated by the LED blinking without the need to press the button
 * To reset the game, press any button and again, allow the LEDs to blink and hold.
 * Remember, if you press the button before the game begins, the other player wins!
 */

unsigned int reset = 0;
unsigned int count = 0;
unsigned int playerAscore = 0;
unsigned int playerBscore = 0;

unsigned int awon = 0;
unsigned int bwon = 0;

void timerstart(int f);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;           // Disable the GPIO power-on default high-impedance mode

	P1DIR |=BIT0; //set Port 1.0 output --LED
	P9DIR |=BIT7; //set Port 9.7 output --LED
	P1OUT &= ~BIT0;
	P9OUT &= ~BIT7;
	P1DIR &=~(BIT1|BIT2); //set Port 1.1/1.2 input --- pushbutton
	P1REN|=(BIT1|BIT2);//enable pull-up resistor on buttons
	P1OUT|=(BIT1|BIT2); //set as pullup resistors
	P1IE |=(BIT1|BIT2);//enable the interrupt on P1.1 and P1.2
	P1IES |= (BIT1|BIT2);//set as falling edge
	P1IFG &=~(BIT1|BIT2);//clear interrupt flag
	
	timerstart(100);    //initialize 100Hz clock

	__enable_interrupt();

	while (1)
	{}
}

void timerstart(int f) // call function with desired frequency to initialize timer module
{
    int n;
    TA0CCTL0 = CCIE; //Enable interrupt in compare mode
    TA0CTL = TASSEL_2 + MC_1 + ID_2; //SMCLK, up mode
    //Calculations for TA0CCRO
    n = 250000 / f; //250000 / 10 = 25000
    TA0CCR0 = n; // [( 10^6 / 4) / (25000) = 10Hz)]
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{

    if (!(P1IN & BIT1) && reset == 1)
    {
        if (playerAscore < 20) //as long as the score is less than 10, increment
        {
            playerAscore++;
            P1OUT ^= BIT0;
            while(!(P1IN & BIT1)) {} //hold until button is released
            __delay_cycles(10000); //additional delay to avoid bouncing toggling LED
        }

        else // once the score is equal to 10, player A has won
        {
            count = 0;
            awon = 1;
        }
        P1IFG &=~(BIT1);    //clear interrupt flag
    }

    if (!(P1IN & BIT2) && reset ==1 )
    {

        if (playerBscore < 20)
        {
            playerBscore++; //increment score
            P9OUT ^= BIT7; //toggle LED to indicate button press
            while(!(P1IN & BIT2)) {} //hold until button is released
            __delay_cycles(10000); //additional delay to avoid bouncing toggling LED
        }

        else //once the score has reached 10, player B has won
        {
            count = 0;
            bwon = 1;
        }

        P1IFG &=~(BIT2);    //clear interrupt flag

    }
}

// TimerISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
    if (reset == 0) //starting game
    {
        if (count == 1) // if this is initial reset
        {
            P1OUT &= ~BIT0; //clear the LED's
            P9OUT &= ~BIT7;
        }
        if (count % 25 == 0)
        {
            P1OUT ^= BIT0; //toggle to indicate start
            P9OUT ^= BIT7;
        }

        if (count == 200)
        {
            P1OUT ^= BIT0; //toggle once more --> the LEDs will both be on and held at start
            P9OUT ^= BIT7;
            count = 0;
            reset = 1; // set reset to indicate the game is in progress
        }
        count++;

    }
    if (awon == 1) //if player A has won
    {
        P9OUT &= ~BIT7; //clear loser's LED
        P1IE &= ~(BIT1 + BIT2); //disable interrupts temp
        if (count % 25 == 0) //toggle every .01 s * 25 = 0.025 s
        {
            P1OUT ^= BIT0; //toggle winner's LED
        }

        if (count == 200) // after so many toggles
        {
            while ((P1IN & BIT1) && (P1IN & BIT2)) //speed will increase to indicate the possibility of resetting game
            {                                      // will hold until a button is pressed
                __delay_cycles(100000);
                P1OUT ^= BIT0; //toggle winner's LED
            }
            //then we reset the variables necessary for a new game
            awon = 0; //reset winning marker
            count = 0;
            reset = 0; //let's restart the game
            playerBscore = 0;
            playerAscore = 0;
            P1IE |= (BIT1 + BIT2); //renable interrupts
        }
        count++; //increment count
    }

    else if (bwon == 1) //same process if player B has won
    {
         P1OUT &= ~BIT0;
         P1IE &= ~(BIT1 + BIT2); //disable interrupts temp
         if (count % 25 == 0)  //toggle every .01 s * 25 = 0.025 s
         {
             P9OUT ^= BIT7; //toggle winner's LED
         }

         if (count == 200)
         {
             while ((P1IN & BIT1) && (P1IN & BIT2))
             {
                 __delay_cycles(100000);
                 P9OUT ^= BIT7; //toggle winner's LED
             }
             bwon = 0; //reset winning marker
             count = 0;
             reset = 0; //let's restart the game
             playerBscore = 0;
             playerAscore = 0;
             P1IE |= (BIT1 + BIT2); //renable interrupts
         }
         count++; //increment count
     }
}
