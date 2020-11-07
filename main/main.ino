/*
Sensor is to create Digital (Lo Pulse) output to Particulate Matters (PM). Lo Pulse Occupancy time (LPO time) is in proportion
to PM concentration. The output is for PM whose size is around 1 micro meter or larger. We can use the sensor to detect the dust in clean room.
Minimum detect particle: 1um
http://www.seeedstudio.com/wiki/Grove_-_Dust_Sensor
 Grove - Dust Sensor Demo v1.0
 Interface to Shinyei Model PPD42NS Particle Sensor
 Program by Christopher Nafis 
 Written April 2012
 
 http://www.seeedstudio.com/depot/grove-dust-sensor-p-1050.html
 http://www.sca-shinyei.com/pdf/PPD42NS.pdf
 */

int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 1000;//sampe 1s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
int gLed = 7;
int yLed = 6;
int rLed = 5;
const int buzzer = 9;

void setup() {
  Serial.begin(9600);
  pinMode(pin,INPUT);
  pinMode(gLed,OUTPUT);
  pinMode(yLed,OUTPUT);
  pinMode(rLed,OUTPUT);
  pinMode(buzzer, OUTPUT);
  starttime = millis();//get the current time;
}

void loop() {
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;

  if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.print("concentration = ");
    Serial.print(concentration);
    Serial.print(" pcs/0.01cf  -  ");
    if (concentration < 1.0) {
     Serial.println("It's a smokeless and dustless environment"); 
     digitalWrite(gLed, HIGH);
     digitalWrite(yLed, LOW);
     digitalWrite(rLed, LOW);
     noTone(buzzer);
  }
    if (concentration > 1.0 && concentration < 20000) {
     Serial.println("It's probably only you blowing air to the sensor :)"); 
     digitalWrite(gLed, HIGH);
     digitalWrite(yLed, LOW);
     digitalWrite(rLed, LOW);
     noTone(buzzer);
    }
    
    if (concentration > 20000 && concentration < 315000) {
     Serial.println("Smokes from matches detected!"); 
     digitalWrite(gLed, LOW);
     digitalWrite(yLed, HIGH);
     digitalWrite(rLed, LOW);
     noTone(buzzer);
    }
      if (concentration > 315000) {
     Serial.println("Smokes from cigarettes detected! Or It might be a huge fire! Beware!"); 
     digitalWrite(gLed, LOW);
     digitalWrite(yLed, LOW);
     digitalWrite(rLed, HIGH);
     tone(buzzer, 250);
  }
    
    lowpulseoccupancy = 0;
    starttime = millis();
  }
}

