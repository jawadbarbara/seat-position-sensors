# The Smart Chair
# About The Project 
We are the students : Jawad Barbara, Nataly Armaly, Bishara Abuzalaf.
Our smart chair is developed in order to enhance your sitting habits.
Attached a photo with our supported positions, in the folder of positons.

The chair have two modes :

1. Predicting mode : used for predicting your current position, and notfiying with 3 leds.
Each led specific body part that need to be adjusted.
we have one led for back,and two for each leg.
Our prediction relies on dataSet, that you must have in your computer before running the system, and if you would to expand our dataSet use training mode.

2. Training mode : you will get instruction with the number of the poition to sit in, and you have to complete the 6 positions ,
in order to add your own sample to our data set, and if you don't, you will get notfied and you must change ypur position accordingly.



# Using Instructions 

- To start the system, choose your desired mode, press the button, and follow the instruction that will get displayed into the screen.
- After choosing one mode, you can switch to another, by changning the state of the switch.




# Used Libraries
-Adafruit_NeoPixel.h
 



# Folder Description 
In this git you have the following folders:

1. Code:
- smartSeatSensor.ino: The main code that is apploaded into the esp. This code interacts with the hardware, and read the values of the button, switch,sensors.
- parameters.h: contains all parametres that we use in our project that is  hard coded.
- smartSeatSensor.py: in order to run the system, you have to run this file . This is the code that contain user-iterface model, and runs the main algorithm and interacts with the data set in order to calculate a prediction of your current position.


2. DataSet:
sensors_samples.xsl: containts the dataSet we have created.


3. Positons : 
positions.png: contains a pdf file with our supported positions. 

4. documentation:
work documentation.docs: contain the work proccess.
   




# Hardware
Hardware Components:
- 5 Fsr Sensors.
- 5 300 [ohm] resistors.
- 1 Button.
- 1 Switch.
- 3 leds.

# Project Diagram 
![WhatsApp Image 2024-04-03 at 22 08 21](https://github.com/jawadbarbara/seat-position-sensors/assets/159052487/cdf443c3-8920-4210-b2c1-94042277409b)




