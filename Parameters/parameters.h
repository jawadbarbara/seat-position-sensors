// These constants won't change. They're used to give names to the pins used:
const int sensor1Pin = 27;  // Analog input pin that the potentiometer is attached to
const int sensor2Pin = 4;  // Analog input pin that the potentiometer is attached to
const int sensor3Pin = 14;  // Analog input pin that the potentiometer is attached to
const int sensor4Pin = 26;  // Analog input pin that the potentiometer is attached to
const int sensor5Pin = 32;  // Analog input pin that the potentiometer is attached to

int sensor1Value = 0;  // value read from the pot
int sensor2Value = 0;  // value read from the pot
int sensor3Value = 0;  // value read from the pot
int sensor4Value = 0;  // value read from the pot
int sensor5Value = 0;
int outputValue = 0;  // value output to the PWM (analog out)

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 13;  // the number of the pushbutton pin
const int switchPin = 16;  // the number of the switch pin



// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status
bool button_flag = true ;

int was_high = 0;
bool empty_chair = false;
bool first_time= true;


enum Position {
    NORMAL = 1,
    LEFT_LEG_UP = 2,
    RIGHT_LEG_UP = 3,
    LAID_DOWN = 4,
    CURLED_BACK = 5,
    RELAXED = 6,
    OTHER = 7
};