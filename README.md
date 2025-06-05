# AutomaticFishFeeder
Program an arduino mega with two stepper motors and LEDs to feed multiple fishtanks from the console. 

This program can feed two separate fishtanks simultaneously. The two tanks are called `JEFF` and `BOB`. 
The steps for the stepper motors are calibrated to match the distance in the 3D printable feeder files included. 

- It allows you to send a command to feed yourself, or will feed automatically if more than 24h have gone by since the last time the tank has been fed. 

- It will not feed automatically or allow you to feed after 10pm

- You can ask how many times youve fed either tank in the past 24h. It will show confirmation messages on the console when it runs any task and completes it. 

- You can ask for the time to make sure the clock is still synchonized. 
