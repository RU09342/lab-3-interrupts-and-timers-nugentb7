# Twenty Presses
# Use the MSP430FR6989 to play this quick, competitive game!
## Explanation
This game is played by two players who use the on-board pushbuttons to increase their score. The start of the game is indicated by three LED blinks followed by the holding of the LED's in the
high state.

Thereafter, each button press will increment the player's score until one player reaches 20. The player who reaches 20 first will win, with the winner indicated by his or her LED blinking repeatedly. The loser's LED will be turned off until the game is reset.

The game includes reset capability, with either button being pressed restarting the game. The players will then, again, wait until the LEDS blink three times and hold before pressing the buttons.

## Algorithm
The TimerA interrupt is used to begin/reset the game, as well as indicate the end of the game. Using a counter, the LEDs are toggled each time the counter reaches a multiple of 25, using the modulus operator.
Until the counter reaches 200, the LEDs are toggled. Once the counter reaches 200, the LEDS are held high, indicating the players may begin playing the game.

Once the game begins, the PORT1 interrupt is used upon each button press. If the player who pressed the button has a score less than 20, the player's score is incremented. Once a score reaches 20, the reset variable is set high.

This indicates that the game is over, within the TimerA interrupt. Then, the LED of the winner is toggled and the LED of the loser is turned off. This continues until one of the buttons is pressed, resetting the game.
