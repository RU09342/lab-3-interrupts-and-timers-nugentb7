# Timer A Blink
## Brendan Nugent
## Compatible Microcontrollers
* MSP430G2553
* MSP430F5529
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989
##Description
While the previous exercise used a button-triggered interrupt, the focus of this exercise is to configure the internal timer module to trigger the LED toggle.

Within the "timerstart(int f)" function, the internal timer is configured in its 'up' mode and set to use the SMCLK, which has a base frequency of approximately 1MHz.
This is then divided, using the internal divider, by 4 to create a new frequency of 250KHz. The 'f' variable passed to the function indicates the desired frequency. To set this frequency,
the timer's capture compare register is set to 250KHz/f. This creates an interrupt each time the timer crosses the CCR threshold, set to 250KHz/f. 

As an example, and one that is used in the program, the function call timerstart(10) will lead to the following calculations. (10^6 Hz/4) = 250KHz --> 250KHz / 10 = 25000
Then, the CCR value is set to this value, held in a variable 'n'. Thus, 250KHz/25000 = 10. This indicates that the timer will cross the CCR value ten times a second, or at a frequency of 10Hz.

To further divide the LED's toggle frequency, within the timer interrupt, a counter variable is used. The count is incremented at each interrupt. However, the LED is only toggled each time
the count reaches the value of 50. This causes the LED to stay on for 5s and off for 5s --> 0.1s * 50 interrupts = 5s. Then, each time the LED is toggled, the count is also reset.

The difference between boards, aside from the pinouts, lies in the timer modules. While the rest of the boards have a Timer A module that I chose to utilize, the MSP430FR2311 must utilize
its Timer B module. Still, the timer is configured the same way.