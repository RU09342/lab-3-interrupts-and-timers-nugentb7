# Button Interrupt
## Brendan Nugent
## Compatible Microcontrollers
* MSP430G2553
* MSP430F5529
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989
## Description
The CCS projects included create an LED-blinking microcontroller which is controlled by the on-board buttons. The state of the LED is changed only with a press of the button.
The program has been implemented with the optional capability of changing the speed at which the LED is blinking with each press of the button. When the speed reaches its maximum value
and the button is pressed once more, the speed loops back to the "off" mode. 

Each button press triggers an interrupt set up to look for the falling edge on each pin that is associated with the on-board buttons. Though this varies with each board, the algorithm remains constant.
When the interrupt is triggered, the speed is toggled. To change the frequency, the value inside of the __delay_cycles(x) code is changed inside of a case statement.

Additionally, a while loop that continues while the button is held, followed by a delay inside of the interrupt service routine, reduces the effect that bouncing has on the end-result.

