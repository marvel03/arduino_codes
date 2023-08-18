import processing.serial.*;
Serial port;
String angleX="0";
String angleY="0";
String angleArr[];
String angles="0,0";
int s;

void setup(){
size(500,500,P3D);
background(0);
//Serial.list()[0]
port=new Serial(this,"COM4",9600);
port.bufferUntil('\n');
}
void draw(){
   s=millis();
 while((millis()-s)<10);
 background(0);
 lights();
 translate(250,250);
 println(angles);
 angleArr=split(angles,',');
if(angleArr[0]==""){}
else{
  rotateX(radians(-Integer.valueOf(angleArr[0].trim())));
}
if(angleArr[1]==""){}
else{
  rotateZ(-radians(Integer.valueOf(angleArr[1].trim())));
}
box(150);

//port.clear();
}
void serialEvent(Serial port){

 angles=port.readString();
}
