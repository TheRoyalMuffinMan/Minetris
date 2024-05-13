# Minetris
<p align="center">
  <img src="images/Box.jpg" alt="arcade box" height="400"/>
</p>
Ever wondered how difficult it would be to build an arcade machine? I pondered the same, to the extent that I decided that for my final project, I would build my own arcade machine where you could play the game [Tetris](https://en.wikipedia.org/wiki/Tetris) on. I chose Tetris since it is a game commonly found in many arcades, and it's a classic I've always enjoyed playing in my free time. My implementation of the game attempts to replicate the [NES](https://en.wikipedia.org/wiki/Tetris_(NES_video_game)) version of Tetris, which is known as *Classic Tetris*. The goal of the game is to handle different types of falling shapes by trying to fit them into gaps to create solid rows. Completing these solid rows gives points, which increases the user's score. The game ends when pieces are stacked to the top of the screen and the next piece can't fit on the screen. \

Outside of the fun factor of making a video game, I felt the project brought on equal levels of difficulty on the hardware and software sides. Such as working with new peripherals, learning how to solder parts, managing my own basic graphics library, and finally building the case for it. These aspects all motivated me to make this my final project for the course. The project also tackles an interesting problem that comes with working with microcontrollers. Which is the restriction brought on by having a limited amount of available RAM. This unique issue comes up heavily when working with displays, requiring you to be very frugal with what you can and cannot do memory-wise. I believe the results achieved in my project helped me learn how to tackle this issue and could be used as a basis for other screen-based projects on the ATmega3208 microcontroller.

My project utilizes key aspects of embedded systems, which are all managed on the AVR-BLE board. As detailed below:

- User control is managed by the analog thumbstick, which allows the user to input three different options. Two of these options include vertical and horizontal analog inputs, which are handled by ADC. These actions allow the user to navigate different screens within my system and provide movement of pieces when playing the game. The last option is a digital input, which is read from an interrupt ISR when the thumbstick is pressed down. This action allows the user to perform selections while in menus and rotate the falling pieces. This input is also debounced to improve the user experience.

- The I²C protocol is utilized on two major peripherals within the system. The first is the 128x64 OLED display, which reads a pixel-based buffer to display different images on the screen. And the 7-segment display, which is sent over a number that represents the current score achieved by the player.

- Another set of displays that are shown to the user are three LEDs (red, green, and blue). Over digital I/O, the LEDs light up in different orders to let the user know the next piece.

- Korobeiniki (A-Type) is played while the game is running. This occurs by using the speaker with the TCA peripheral in frequency wave generation mode.
