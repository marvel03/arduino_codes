#include <Wire.h>
#include <Math.h>
//////
float theta=0;
float phi=0;
//////gyro related variables
long  gyroX,gyroY,gyroZ;
float rotX,rotY,rotZ;
float GthetaX=0,GthetaY=0;

////////////accelerometer related variables
long accelX=0,accelY=0,accelZ=0;
float gforceX=0,gforceY=0,gforceZ=0;
float AthetaX=0,AthetaY=0;

/////////////timer variables
unsigned long millisOld=0;
float dt=0 ;

//////////////////////////////filtering variables
float AthetaXOld=0;
float AthetaXFiltered=0;
float AthetaYOld=0;
float AthetaYFiltered=0;
float GthetaXOld=0;
float GthetaXFiltered=0;
float GthetaYOld=0;
float GthetaYFiltered=0;
float rotXFiltered=0;
float rotXOld=0;
float rotYFiltered=0;
float rotYOld=0;
float rotZFiltered=0;
float rotZOld=0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  configureMPU();
  millisOld=millis();  
}

  
void configureMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00010000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}
void getAccelData(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);
  while(Wire.available()<6);
  accelX=Wire.read()<<8|Wire.read();
  accelY=Wire.read()<<8|Wire.read();
  accelZ=Wire.read()<<8|Wire.read();
  processAccelData();
}
void processAccelData(){
  gforceX=accelX/16384.0;
  gforceY=accelY/16384.0;
  gforceZ=accelZ/16384.0;
  AthetaX=atan2(gforceY,gforceZ)/2/3.141592*360;
  AthetaY=atan2(gforceX,gforceZ)/2/3.141592*360;
  AthetaXFiltered=0.9*AthetaXOld+0.1*AthetaX;
  AthetaYFiltered=0.9*AthetaYOld+0.1*AthetaY;
  AthetaYOld=AthetaYFiltered;
  AthetaXOld=AthetaXFiltered;
}

void getGyroData(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);
  while(Wire.available()<6);
  gyroX=Wire.read()<<8|Wire.read();
  gyroY=Wire.read()<<8|Wire.read();
  gyroZ=Wire.read()<<8|Wire.read();
  processGyroData();
}
void processGyroData(){
  rotX=(gyroX/131.0);
  rotY=(gyroY/131.0);
  rotZ=(gyroZ/131.0);
  dt=(millis()-millisOld)/1000.;
    millisOld=millis();

  theta=(theta+rotY*dt)*(0.95)+(AthetaYFiltered)*(0.05);
  phi=(phi+rotX*dt)*(0.95)+(AthetaXFiltered)*0.05; 
  
}

void printData(){
Serial.print((int)theta);Serial.print(',');Serial.println((int)phi);

}

void loop() {
 getAccelData();
 getGyroData();
 
 printData();
}
