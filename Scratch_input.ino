// Scratchboard emulation
// by Kevin Osborn
// http://baldwisdom.com
// Feel free to use this code in any way you wish! (Though if you want to link
// to my blog, that would be cool)
// The Scratchmote has a sharp ir distance sensor
// and a 3 axis accellerometer
//
// Modified for input with TMP36 temperature sensor for Scratch Fire Dragon

#define TempPIN 0                       // Analog Pin 0
char tempBuf[32] = {0};
float temp;
int but_press;

/*
* getVoltage() returns the voltage on the analog input defined by
* pin
*/
float getVoltage(int pin){
return (analogRead(pin) * 0.0048828125); //converting from a 0 to 1023 digital range
                                        // to 0 to 5 volts (each 1 reading equals ~ 5 millivolts)
}


// Format output for ScratchBoard emulation
// sensor=0-7, value=0-1023 (rescaled by Scratch to 0-100)
// 0="A", 1="B", 2="C", 3="D",
// 4="Slider", 5="Light", 6="Sound", 7="Button"
void ScratchBoardSensorReport(byte sensor, int value)
{
  Serial.write(( 1 << 7) | (sensor << 3) | (value >> 7));
  //delayMicroseconds(400);
  Serial.write( value & 0b01111111);
}


void setup()
{
  pinMode(12, INPUT);
  digitalWrite(12, HIGH);
  Serial.begin(38400);
}

void loop()
{
but_press=0;
if (digitalRead(12) == LOW) {
  but_press =1023;
}
if (Serial.available())
{
  if (Serial.read() == 0x01){
  ScratchBoardSensorReport(0,0);
  ScratchBoardSensorReport(1,0);
  ScratchBoardSensorReport(2,0);
  ScratchBoardSensorReport(3,0);
  ScratchBoardSensorReport(4,(getVoltage(TempPIN) - .5) * 1024);
  ScratchBoardSensorReport(5,0);
  ScratchBoardSensorReport(6,0);
  ScratchBoardSensorReport(7,but_press);
  }
  delay(30);
}

}
