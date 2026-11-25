# STM32 Pac-Man Duel

A two-player Pac-Man vs. Ghost game running on two STM32 microcontrollers. Each player has their own board and screen. You move by tilting the board, the two boards talk to each other over UART, and the final battle is played by moving your hand above an ultrasonic sensor.

Final project for ELE3312 (Microcontrollers and Applications) at Polytechnique Montréal, fall 2024.

## How the game works

1. **Menu.** Each player tilts their board to choose a character: Pac-Man or Ghost.
2. **Maze.** Both characters appear in a maze on a TFT screen. Tilting the board moves your character. Wall collisions are checked tile by tile, and each board sends its position to the other so both screens stay in sync.
3. **Battle.** When the two players meet, the battle starts. A target moves up and down the screen. Each player raises or lowers their hand above an ultrasonic sensor to keep their marker on the target. While you are on target, the board plays a tone and your power bar fills up. The first player to fill their bar wins.

## Hardware

- 2 x STM32 Nucleo-F446RE boards
- 2 x ILI9341 TFT screens (SPI)
- 2 x MPU-6050 accelerometers (I2C, with interrupt pin)
- 2 x ultrasonic distance sensors
- A speaker on the DAC output
- UART link between the two boards

## Peripherals used

| Feature | Peripheral |
|---|---|
| Character movement | MPU-6050 accelerometer over I2C (PB8 SCL, PB9 SDA), interrupt on PC11 |
| Distance measurement | Ultrasonic sensor: Timer 2 in PWM mode for Trig (PA1), Timer 5 in input capture for Echo (PA0) |
| Sound | DAC on PA4, fed by DMA |
| Board-to-board link | UART with DMA (PC12 TX, PD2 RX) |
| Display | ILI9341 over SPI |

## Code structure

```
Core/Src/main.c            setup and main loop
Core/Src/Menu.c            character selection
Core/Src/Game.c            maze drawing, collisions, player positions
Core/Src/Battle.c          battle stage, target, power bars, winner check
Core/Src/Accelerometre.c   MPU-6050 setup and tilt direction
Core/Src/Communication.c   UART messages between the two boards
Core/Inc/maze.h            maze layout
Core/Inc/sprite_data.h     character sprites
MDK-ARM/                   Keil uVision project
ELE3312_Project.ioc        STM32CubeMX configuration
```

## Build and flash

1. Open `MDK-ARM/ELE3312_Project.uvprojx` in Keil uVision 5.
2. Build the project (F7).
3. Flash each Nucleo board (F8).
4. Wire the two boards together (TX to RX, RX to TX, shared ground) and power both.

The pin setup can be viewed or regenerated with STM32CubeMX from `ELE3312_Project.ioc`.

## Built with

C, STM32 HAL, Keil uVision 5, STM32CubeMX

## What I learned

- Using timers, interrupts and DMA together on one microcontroller
- Measuring distance with input capture
- Generating sound with a DAC
- Keeping two devices in sync over a serial link
- Fitting graphics and game logic into the memory and speed limits of an embedded board

## Team

Built in a team of four (Team 5): Rina Aladad, Mahdi Moussa, Moïra Lamarre and Kamer Ammour.

The project started from a reference template provided by the course. The team built the gameplay, sensor handling, sound and board-to-board communication on top of it.

## Author

Moïra Lamarre, [github.com/MEL2002](https://github.com/MEL2002)
