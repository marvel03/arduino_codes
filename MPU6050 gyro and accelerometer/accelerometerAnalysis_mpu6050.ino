#include<Wire.h>
#define MPU 0b1101000
float accelX,accelY,accelZ;
float Vxz[]={0,0};
float Vyz[]={0,0};
float Axpack[]={0,0,0};// {rawValue, oldFilteredValue, newFilteredValue}
float Aypack[]={0,0,0};// {rawValue, oldFilteredValue, newFilteredValue}
float Azpack[]={0,0,0};// {rawValue, oldFilteredValue, newFilteredValue}

void setup() {
 Serial.begin(9600);
 Wire.begin();
 configureMPU();
}

void loop() {
 getAccelData();
 lowPassFilter(Axpack);
 lowPassFilter(Aypack);
 lowPassFilter(Azpack);
//  Serial.print(Axpack[2]);
//  Serial.print(",");
//  Serial.print(Aypack[2]);
//  Serial.print(",");
//  Serial.println(Azpack[2]);
 normalizeVector(Vxz[0],Vxz[1],Axpack[2],Azpack[2]);
 normalizeVector(Vyz[0],Vyz[1],Aypack[2],Azpack[2]);
 //Serial.println("Vxz: "+(String)Vxz[0]+","+(String)Vxz[1]);
 Serial.print((String)Vxz[0]+","+(String)Vxz[1]+","); 
 Serial.println((String)Vyz[0]+","+(String)Vyz[1]); 
}
void configureMPU(){
  Wire.beginTransmission(MPU); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission(); 
  Wire.beginTransmission(MPU);
  Wire.write(0x1b);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0x08);
  Wire.endTransmission();
}
void normalizeVector(float &a,float &b,float v1,float v2){
  a=v1/sqrt(sq(v1)+sq(v2));
  b=v2/sqrt(sq(v1)+sq(v2));
  
}
void getAccelData(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU,6);
  while(Wire.available()<6);
  accelX=Wire.read()<<8|Wire.read();
  accelY=Wire.read()<<8|Wire.read();
  accelZ=Wire.read()<<8|Wire.read();
  processAccelData();
}
void processAccelData(){
  Aypack[0]=accelY/8192.0;
  Azpack[0]=accelZ/8192.0;
  Axpack[0]=accelX/8192.0;
}
void lowPassFilter(float(&datapack)[3]){
  //dataFiltered=0.9*dataOld+0.1*data;
  datapack[2]=0.8*datapack[1]+0.2*datapack[0];
  datapack[1]=datapack[2];
}
