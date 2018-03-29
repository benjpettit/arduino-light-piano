# arduino-light-piano
MIDI instrument using light sensors and arcade buttons

This runs on an Arduino Leonardo or Micro, with 12 photoresistors wired to the 12 analog input pins. When the light on a photoresistor exceeds a threshold, it sends a MIDI message to turn on a particular note, with an intensity/volume that depends on the light level. In other words, you play notes by shining lights on the sensors. I also gave it 3 arcade buttons that send notes on a separate MIDI channel, to use for triggering drums or samples. Here's a video of the exciting moment when I first got it going! https://youtu.be/qySMCh-tCoI

When the Arduino starts up or resets, it calibrates the light sensitivity by setting a threshold based on the maximum reading across all of the photoresistors. I added an extra arcade button to ground the reset pin, so that you can recalibrate it in the middle of a performance and make it more or less sensitive to light. I'm happy to add a wiring diagram if anyone's interested.
