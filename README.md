# Valentine's Robot

Simple robot based on the OTTO project.


## About the project

Using another page up on GitHub pages to store cute little sentences, an esp32 microcontroler sends a GET request and aquire 50 of those said sentences.

With the aid of a HC-SR04, a 16x2 I2C LCD lights up everytime someone comes in front of the robot's "eyes" (the ultrassonic sensor). Every hour, the sentence shown changes automatically.
If you get your hand very close to the sensor, it also changes the sentence for you.


## Optimizations to be made

Currently, the robot takes 50 sentences from the page, and only that. One more or one less might make it run in an unpredictable way.

Also, loading 50 sentences into the esp's RAM, and doing all the processing to split the strings is very heavy on processing and uses a fair bit of power (assuming I'm running on batteries).

The best IMO way would be to make a custom server or database, so the esp would get a single, random sentence from the service instead of a bunch of raw data that it still has to process itself.
