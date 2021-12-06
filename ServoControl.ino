//The low and High values have been found through experimentation;180 degree corresponds to 2200us pulse and 0 degree corresponds to 350us pulse.
//0 degree corresponds to initial position of servob which is 0 degree rotation
//190 degree corresponds to 180 degree rotation of servob
//0 correspons to gripper opened and 120 degree corresponds to gripper fully closed(servog corresponds to gripper servo)
//180 degree corrosponds to 0 of servob1 and 87 corresponds to 90 of servob1
//93 corresponds to initial position pf servob2 and 0 corresponds to maximum rotation of servob2
// 0-180(range of servob);
//(0-40)(range of servob1);
//(0-20)(range of  servob2);
//(0-120)(range of gripper);
//for reaching the source and picking the object  order of servo operation from 0-3 or bottom to up
// reaching the destination order of servo operation from 2-0 or top to bottom
// for dropping the object order of servos will be 1-3
#include<SoftwareSerial.h>
SoftwareSerial bluetooth(12,13);
int sig[4]={8,9,10,11};
long cv[4]={180,40,20,120};  
int msec;
char data[20];
int index=0;
int flag=0;
//int a=0;
int angle[5];
void Servoangle(long angle,int pin,int msec);
void Servo_Set(long angle,int pin);
void initial_pos();
void check_data();
void extract_angles();
void pick_place();

void setup() {
  // put your setup code here, to run once:
   bluetooth.begin(9600);
  for (int i=0;i<4;i++)
  {
    pinMode(sig[i],OUTPUT);
  }
  initial_pos();
}


void loop() {
 while(bluetooth.available()>0 && index!=19)
{ 
 
  data[index]=bluetooth.read();
  index=index+1;
}
if(index==19)
{
  extract_angles();
  pick_place();
  initial_pos();
}
}


 


void Servoangle(long angle,int pin,int msec)
{
  long initial;
 
  if (pin==sig[0])
{  
   angle=round((190*angle)/180);
   initial=round((190*(cv[0]))/180);
   cv[0]=round((180*angle)/190);

}
else if (pin==sig[1])
{ 
  angle=round(180 - (93*angle)/90);
  initial=round(180 - (93*(cv[1]))/90);
  cv[1]=round(((180-angle)*90)/93);
 
}
else if (pin==sig[2]){
  angle=round(93-(93*angle)/90);
  initial=round(93-(93*(cv[2]))/90);
  cv[2]=round(((93-angle)*90)/93);
  
}
else{
  initial=cv[3];
  cv[3]=angle;
}
if (initial<angle)
{
 for (int i=initial;i<=angle;i++)
 {
  Servo_Set(i,pin);
  delay(msec);
 }
}
else if (initial>angle)
{
   for (int i=initial;i>=angle;i--)
 {
  Servo_Set(i,pin);
  delay(msec);
 }
}
 

}

void Servo_Set(long angle,int pin)
{
long ptime=350 + ((1860*angle)/180.0);
  digitalWrite(pin,HIGH);
  delayMicroseconds(ptime);
  digitalWrite(pin,LOW);
  long ltime=20000-ptime;
  delayMicroseconds(ltime);
  }
void initial_pos()
{
    for (int i=3;i>=0;i--)
  { 
    Servoangle(0,sig[i],20);
    angle[i]=0;
  }
  angle[4]=0;
}

void extract_angles()
{ 
  
  char *dptr;
  char data2[3];
  int row=0;
  int col=0;

dptr=strtok(data,",");

while(dptr!=NULL)

{
  col=0;
  while(*dptr!=NULL)
{
 data2[col]=*dptr;
 dptr++;
 col++;
}
angle[row]=atoi(data2);
dptr=strtok(NULL,",");
row++;
}
index=0;
}

void pick_place()
{
Servoangle(long(angle[0]),sig[0],20);
Servoangle(long(angle[1]),sig[1],20);
Servoangle(long(angle[2]),sig[2],20);
Servoangle(long(angle[3]),sig[3],20);

Servoangle(0,sig[2],20);
Servoangle(0,sig[1],20);
Servoangle(long(angle[4]),sig[0],20);

Servoangle(long(angle[1]),sig[1],20);
Servoangle(long(angle[2]),sig[2],20);
Servoangle(0,sig[3],20);
}
