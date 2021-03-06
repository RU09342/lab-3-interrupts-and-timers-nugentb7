# Button Based Delay
## Brendan Nugent
## Compatible Microcontrollers
* MSP430G2553
* MSP430F5529
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989
## Description
This application will allow microcontroller users to determine the speed at which the LED blinks by holding down the on-board button. The duration that the button is held down will become the new interval at which
the LED is toggled. Another way to state this is, the longer the button is held down, the longer the LED will remain in its high and low state.

To accomplish this, an enable variable is set within the button's interrupt service routine. This variable will allow the timer to increment a counter within the timer's interrupt service routine.
The enable bit is set low once again when the button is released. Next, within the timer's interrupt routine, the count is used in a comparative statement. The LED is then toggled each time a based
count is equal to the button-set count.

To avoid confusion between each count, allow the variable that is set by the duration of which the button is held down to be 'i'. Allow the base counter to be called 'count'.
Then, in the timer's interrupt service routine, we include the following code snippet:

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
        if (count == i) //toggle every i interrupts or i * .1 = 5 s
        {
            P1OUT ^= 0x01; //Toggle LED
            count = 0;  // reset count
        }

        else
            count++;    // increment count each interrupt until count = i
    }

As can be seen, the LED is toggled only when count is equal to i. Additionally, each time the LED is toggled, count is reset.

The biggest difference between each board, aside from the pinouts, is the timer module that must be used with the MSP430FR2311. While the rest of the development boards have a Timer A module,
the MSP430FR2311 must utilize its Timer B module. In addition, with the FR family, its programmer must turn off the high-impedance mode.