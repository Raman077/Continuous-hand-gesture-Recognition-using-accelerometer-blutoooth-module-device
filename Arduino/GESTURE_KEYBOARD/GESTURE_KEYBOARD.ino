
#include <Wire.h>
#include <SoftwareSerial.h>


boolean isBluetoothEnabled = true;
int rxPin = 10;
int txPin = 16;
SoftwareSerial bluetooth(rxPin, txPin);

int btnPin1 = 9;
int btnPin2 = 8;
int yellowLedPin = 6;
int redLedPin = 5;

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;


int precBtn1 = HIGH;
int precBtn2 = HIGH;


void setup(){
  pinMode(btnPin1, INPUT_PULLUP);
  pinMode(btnPin2, INPUT_PULLUP);

  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  // Start the serial communications
  Serial.begin(38400);
  bluetooth.begin(9600);
}
void loop(){
  // Read the values of the buttons
  int resultBtn1 = digitalRead(btnPin1);
  int resultBtn2 = digitalRead(btnPin2);

  if (precBtn1 == HIGH && resultBtn1 == LOW)
  {
    digitalWrite(yellowLedPin, HIGH);
    startBatch();
  }

  if (precBtn2 == HIGH && resultBtn2 == LOW)
  {
    isBluetoothEnabled=!isBluetoothEnabled;
  }

  if (isBluetoothEnabled)
  {
    digitalWrite(redLedPin, HIGH);
  }else{
    digitalWrite(redLedPin, LOW);
  }

  if (resultBtn1==LOW)
  {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true); 
    // Reads the data from the sensor
    AcX=Wire.read()<<8|Wire.read();     
    AcY=Wire.read()<<8|Wire.read(); 
    AcZ=Wire.read()<<8|Wire.read(); 
    Tmp=Wire.read()<<8|Wire.read();  
    GyX=Wire.read()<<8|Wire.read();  
    GyY=Wire.read()<<8|Wire.read();  
    GyZ=Wire.read()<<8|Wire.read(); 
    
    if (isBluetoothEnabled)
    {
      bluetooth.print("START");
      bluetooth.print(" "); bluetooth.print(AcX);
      bluetooth.print(" "); bluetooth.print(AcY);
      bluetooth.print(" "); bluetooth.print(AcZ);
      bluetooth.print(" "); bluetooth.print(GyX);
      bluetooth.print(" "); bluetooth.print(GyY);
      bluetooth.print(" "); bluetooth.print(GyZ);
      bluetooth.println(" END");
    }else{
      Serial.print("START");
      Serial.print(" "); Serial.print(AcX);
      Serial.print(" "); Serial.print(AcY);
      Serial.print(" "); Serial.print(AcZ);
      Serial.print(" "); Serial.print(GyX);
      Serial.print(" "); Serial.print(GyY);
      Serial.print(" "); Serial.print(GyZ);
      Serial.println(" END");
    }
    
  }

  // Closes the batch when the button is released
  if (precBtn1 == LOW && resultBtn1 == HIGH)
  {
    digitalWrite(yellowLedPin, LOW);
    closeBatch();
  }

  // Saves the button states
  precBtn1 = resultBtn1;
  precBtn2 = resultBtn2;
}


void startBatch()
{
  if (isBluetoothEnabled)
  {
    bluetooth.println("STARTING BATCH");
  }else{
    Serial.println("STARTING BATCH");
  }
}

// Sends the closed batch signal
void closeBatch()
{
 if (isBluetoothEnabled)
  {
    bluetooth.println("CLOSING BATCH");
  }else{
    Serial.println("CLOSING BATCH");
  }
}


