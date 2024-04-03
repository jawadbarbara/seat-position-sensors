
#include "parameters.h"



#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        22 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 3 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup(){
  Serial.begin(115200);
  delay(5000);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);

  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(sensor3Pin, INPUT);
  pinMode(sensor4Pin, INPUT);
  pinMode(sensor5Pin, INPUT);   
}

void loop() {
  delay(1000);

  int s1_prev =0,s2_prev =0,s3_prev=0,s4_prev=0,s5_prev=0;  
  int s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0;
  int times=0;
  int sensor1_values_sum=0, sensor2_values_sum=0, sensor3_values_sum=0, sensor4_values_sum=0, sensor5_values_sum=0;
  
  bool FlagPositionChangeDetected = false;
  bool FlagDoneTrainingMode = false;
  bool FlagPredictModeDone = false;
  int switchState;

  //do not start the system until the button is pressed
  buttonState = digitalRead(buttonPin); 
  
  pixels.setPixelColor(0, pixels.Color(0, 0,0));
  pixels.setPixelColor(1, pixels.Color(0, 0,0));
  pixels.setPixelColor(2, pixels.Color(0, 0,0));
  while(first_time && buttonState == HIGH)
  {
    buttonState = digitalRead(buttonPin);
  }
  if(first_time)
  {
    first_time = false;
    delay(500);
  }
  buttonState = digitalRead(buttonPin);
  switchState = digitalRead(switchPin);

  //if we are in TRAINING MODE
  if(switchState == HIGH)
  {
    Serial.println("Training Mode");
    pixels.setPixelColor(0, pixels.Color(100, 254,3));
    pixels.setPixelColor(1, pixels.Color(100, 254,3));
    pixels.setPixelColor(2, pixels.Color(100, 254,3));
    pixels.show();

    FlagDoneTrainingMode = false;
    //sampleing the sensor values in order to update them in the dataset
    while(1)
    {    
      buttonState = digitalRead(buttonPin);
      //waiting for the button to be pressed in order to start sampling
      while(buttonState == HIGH)
      {
        buttonState = digitalRead(buttonPin);
        switchState = digitalRead(switchPin);
        if(switchState==LOW)
        {
          FlagDoneTrainingMode = true;
          break;
        }
      }

      if(FlagDoneTrainingMode)
      {
        break;
      }

      times =0;
      sensor1_values_sum=0, sensor2_values_sum=0, sensor3_values_sum=0, sensor4_values_sum=0, sensor5_values_sum=0;
      
      //sampling for 2 secs
      while (times<20)
      {
        sensor1Value = analogRead(sensor1Pin);
        sensor2Value = analogRead(sensor2Pin);
        sensor3Value = analogRead(sensor3Pin);
        sensor4Value = analogRead(sensor4Pin);
        sensor5Value = analogRead(sensor5Pin);
        sensor1_values_sum +=sensor1Value;
        sensor2_values_sum +=sensor2Value;
        sensor3_values_sum +=sensor3Value;
        sensor4_values_sum +=sensor4Value;
        sensor5_values_sum +=sensor5Value;
        delay(100);
        times++;
      }
      //Normalizing the sensor values
      s1 = ((4096-(sensor1_values_sum)/20)/100);
      s2 = ((4096-(sensor2_values_sum)/20)/100);
      s3 = ((4096-(sensor3_values_sum)/20)/100);
      s4 = ((4096-(sensor4_values_sum)/20)/100);
      s4 = ((4096-(sensor4_values_sum)/20)/100);
      s5 = ((4096-(sensor5_values_sum)/20)/100);

      //sending the data to python
      Serial.print("the average of the sensor 1 values is  = ");
      Serial.println((double)(s1));
      Serial.print("the average of the sensor 2 values is  = ");
      Serial.println(((double)s2));  
      Serial.print("the average of the sensor 3 values is  = ");
      Serial.println((double)s3);
      Serial.print("the average of the sensor 4 values is  = ");
      Serial.println((double)s4); 
      Serial.print("the average of the sensor 5 values is  = ");
      Serial.println((double)s5);
    }
  }

  //if we are in PREDICTING MODE
  else 
  {
    
    pixels.setPixelColor(0, pixels.Color(0, 0,0));
    pixels.setPixelColor(1, pixels.Color(0, 0,0));
    pixels.setPixelColor(2, pixels.Color(0, 0,0));
    pixels.show();

    FlagPredictModeDone = false;
    Serial.println("Predict Mode");

    while(1)
    {
      //waiting for the button to be pressed to start predicting
      while(buttonState == HIGH)
      {
        buttonState = digitalRead(buttonPin);
        switchState = digitalRead(switchPin);
        if(switchState==HIGH)
        {
          FlagPredictModeDone = true;
          break;
        }
      }
      if(FlagPredictModeDone)
      {
        break;
      }
      was_high = 0;
      FlagPredictModeDone = false;
      
      //Predicting loop
      while(1)
      {
        switchState = digitalRead(switchPin);
        //switched to training state
        if(switchState == HIGH)
        {
          FlagPredictModeDone = true;
          break;
        }

        times = 0;
        sensor1_values_sum=0, sensor2_values_sum=0, sensor3_values_sum=0, sensor4_values_sum=0, sensor5_values_sum=0;

        while (times<20)
        {    
          sensor1Value = analogRead(sensor1Pin);
          sensor2Value = analogRead(sensor2Pin);
          sensor3Value = analogRead(sensor3Pin);
          sensor4Value = analogRead(sensor4Pin);
          sensor5Value = analogRead(sensor5Pin);
          //non valid setting position (no person setting)
          if (!((sensor1Value < 3500 && sensor3Value < 3500) || ( sensor2Value < 3500  && sensor4Value < 3500 )))
          {
            pixels.setPixelColor(0, pixels.Color(0, 0,0));
            pixels.setPixelColor(1, pixels.Color(0, 0,0));
            pixels.setPixelColor(2, pixels.Color(0, 0,0));
            pixels.show();
            was_high =1;
            break;
          }
  
          Serial.print("sensor 1 = ");
          Serial.print((4096-(sensor1Value))/100);
          Serial.print("  ");
          sensor1_values_sum +=sensor1Value;

          Serial.print("sensor 2 = ");
          Serial.print((4096-(sensor2Value))/100);
          Serial.print("  ");
          sensor2_values_sum +=sensor2Value;
          
          Serial.print("sensor 3 = ");
          Serial.print((4096-(sensor3Value))/100);
          Serial.print("  ");
          sensor3_values_sum +=sensor3Value;

          Serial.print("sensor 4 = ");
          Serial.print((4096-(sensor4Value))/100);
          Serial.print("  ");
          sensor4_values_sum +=sensor4Value;

          Serial.print("sensor 5 = ");
          Serial.println((4096-(sensor5Value))/100);
          sensor5_values_sum +=sensor5Value;

          delay(100);
          times++;
        }

        if(was_high){
          break;
        }

        s1 = ((4096-(sensor1_values_sum)/20)/100);
        s2 = ((4096-(sensor2_values_sum)/20)/100);
        s3 = ((4096-(sensor3_values_sum)/20)/100);
        s4 = ((4096-(sensor4_values_sum)/20)/100);
        s4 = ((4096-(sensor4_values_sum)/20)/100);
        s5 = ((4096-(sensor5_values_sum)/20)/100);
        
        if(FlagPositionChangeDetected)
        {
          FlagPositionChangeDetected =false;
          break;
        }  
        //detecting that the sensors chaged significantly 
        if((s1-s1_prev)>3 || (s1_prev-s1)>3 || (s2-s2_prev)>3 || (s2_prev-s2) >3 || (s3-s3_prev)>3 ||(s3_prev-s3) >3|| (s4-s4_prev)>3 || (s4_prev-s4) >3|| (s5-s5_prev)>=1 || (s5_prev-s5) >=1)
        {
          FlagPositionChangeDetected = true;
          delay(500);
        }
      }

      if(FlagPredictModeDone)
      {
        break;
      }

      if(was_high)
      {
        was_high = 0;
        continue;
      }
      Serial.println("*************************************************starting sample*****************************************************");
      Serial.print("the average of the sensor 1 values is  = ");
      Serial.println((double)(s1));
      Serial.print("the average of the sensor 2 values is  = ");
      Serial.println(((double)s2));  
      Serial.print("the average of the sensor 3 values is  = ");
      Serial.println((double)s3);
      Serial.print("the average of the sensor 4 values is  = ");
      Serial.println((double)s4);    //appendFilebreak(SD,"/sensors");
      Serial.print("the average of the sensor 5 values is  = ");
      Serial.println((double)s5);
      int Prediction;
      while(!Serial.available()){}
        Prediction = Serial.readString().toInt();
      
      if(Prediction == Position::NORMAL || Prediction == Position::RELAXED ){
        pixels.setPixelColor(0, pixels.Color(200, 0,0));
        pixels.setPixelColor(1, pixels.Color(200, 0,0));
        pixels.setPixelColor(2, pixels.Color(200, 0,0));
    }
      else 
      {
        if(Prediction==Position::LEFT_LEG_UP)
        {
                  pixels.setPixelColor(0, pixels.Color(0, 200,0));
                  pixels.setPixelColor(1, pixels.Color(200, 0,0));
                  pixels.setPixelColor(2, pixels.Color(200, 0,0));
        }
        if(Prediction==Position::RIGHT_LEG_UP)
        {
                  pixels.setPixelColor(0, pixels.Color(200, 0,0));
                  pixels.setPixelColor(1, pixels.Color(0, 200,0));
                  pixels.setPixelColor(2, pixels.Color(200, 0,0));
        }
        if(Prediction==Position::LAID_DOWN)
        {
                  pixels.setPixelColor(0, pixels.Color(0, 200,0));
                  pixels.setPixelColor(1, pixels.Color(0, 200,0));
                  pixels.setPixelColor(2, pixels.Color(0, 200,0));
        }
        if(Prediction==Position::CURLED_BACK)
        {
                  pixels.setPixelColor(0, pixels.Color(200, 0,0));
                  pixels.setPixelColor(1, pixels.Color(200, 0,0));
                  pixels.setPixelColor(2, pixels.Color(0, 200,0));
        }
        if(Prediction==Position::OTHER)
        {
                  pixels.setPixelColor(0, pixels.Color(190, 137, 30));
                  pixels.setPixelColor(1, pixels.Color(190, 137, 30));
                  pixels.setPixelColor(2, pixels.Color(190, 137, 30));
        }
      }
      pixels.show();   // Send the updated pixel colors to the hardware.
      s1_prev = s1;
      s2_prev = s2;
      s3_prev = s3;
      s4_prev = s4;
      s5_prev = s5;
    }
  }
}
