//#include<EEPROM.h>
 
 // IR Sensors
int sensor1 = 5;      // right most sensor
int sensor2 = 4;
int sensor3 = 3;
int sensor4 = 2;      
int sensor5 = A1;      // left most sensor

int sensor[5] = {0, 0, 0, 0, 0};

// Motor Variables
int ENB = 6;
int motorInput1 = 7; //LEFT MOTOR 
int motorInput2 = 8;
int motorInput3 = 9;
int motorInput4 = 10; // RIGHT MOTOR
int ENA = 11;
char way;
int initial_motor_speed =140;

// PID Constants
float Kp = 25;
float Ki = 0;
float Kd = 11;

char path[30] = {'\0'};
int pathLength=0;
int readLength=0;
/*int add=0;
int add2=0;
int i=0;
int j=2;*/
float error = 0,p=0,I=0,D=0,PID_value =0; 
float previous_error = 0 ,previous_I=0;
int flag=0;

void setup() {
  // put your setup code here, to run once:

pinMode(sensor1,INPUT);
pinMode(sensor2,INPUT);
pinMode(sensor3,INPUT);
pinMode(sensor4,INPUT);
pinMode(sensor5,INPUT);
pinMode(13,OUTPUT);

  pinMode(motorInput1, OUTPUT); //LEFT MOTOR
  pinMode(motorInput2, OUTPUT);
  pinMode(motorInput3, OUTPUT);//RIGHT MOTOR
  pinMode(motorInput4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  Serial.begin(9600);                     //setting serial monitor at a default baund rate of 9600
  delay(500);
  Serial.println("Started !!");
  delay(500);
  /*while((j)--){
  way=EEPROM.read(add2);
  Serial.print(way);
  add2++;
  }
  EEPROM.write(0,0);*/
  

}

void forward()
{
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}
void reverse()
{
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
}
void right()
{
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
  
  
}
void left()
{
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}
void sharpRightTurn() {
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, HIGH);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, HIGH);
  
}
void sharpLeftTurn() {
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
  
}
void stop_bot()
{
  /*The pin numbers and high, low values might be different depending on your connections */
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}




void read_sensor_values()
{
  sensor[0] = digitalRead(sensor1);
  sensor[1] = digitalRead(sensor2);
  sensor[2] = digitalRead(sensor3);
  sensor[3] = digitalRead(sensor4);
  sensor[4] = digitalRead(sensor5);

Serial.print("  ");
    /*Serial.print(sensor[0]);
    Serial.print("/");
    Serial.print(sensor[1]);
    Serial.print("/");
    Serial.print(sensor[2]);
    Serial.print("/");
    Serial.print(sensor[3]);
    Serial.print("/"); 
    Serial.print(sensor[4]);
    Serial.print("/");
    Serial.print("/");*/



  if((sensor[0]==0) && (sensor[1]==0) && (sensor[2]==1) && (sensor[3]==0) && (sensor[4]==0))
  error = 0;
  else if((sensor[0]==0) && (sensor[1]==0) && (sensor[2]==1) && (sensor[3]==1) && (sensor[4]==0))
  error = 1;
  else if((sensor[0]==0) && (sensor[1]==0) && (sensor[2]==0) && (sensor[3]==1) && (sensor[4]==0))
  error = 2;
  else if((sensor[0]==0) && (sensor[1]==0) && (sensor[2]==0) && (sensor[3]==1) && (sensor[4]==1))
  error = 3;
  else if((sensor[0]==0) && (sensor[1]==0) && (sensor[2]==0) && (sensor[3]==0) && (sensor[4]==1))
  error = 4;
  else if((sensor[0]==0) && (sensor[1]==1) && (sensor[2]==1) && (sensor[3]==0) && (sensor[4]==0))
  error = -1;
  else if((sensor[0]==0) && (sensor[1]==1) && (sensor[2]==0) && (sensor[3]==0) && (sensor[4]==0))
  error = -2;
  else if((sensor[0]==1) && (sensor[1]==1) && (sensor[2]==0) && (sensor[3]==0) && (sensor[4]==0))
  error = -3;
  else if((sensor[0]==1) && (sensor[1]==0) && (sensor[2]==0) && (sensor[3]==0) && (sensor[4]==0))
  error = -4;
    
    
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 0) && (sensor[4]==0)) // Turn robot right side
    error = 101;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4]==1)) // Turn robot left side
    error = 100;
  else if ((sensor[0] == 0) && (sensor[1] == 0) && (sensor[2] == 0) && (sensor[3] == 0) && (sensor[4]==0)) // Make U turn
    error = 102;
  else if ((sensor[0] == 1) && (sensor[1] == 1) && (sensor[2] == 1) && (sensor[3] == 1) && (sensor[4]==1)) // Turn left side or stop
    error = 103;

//Serial.print(error);

}   
void calculate_pid()
{
  p = error;
  I = I + previous_I;
  D = error - previous_error;

  PID_value = (Kp * p) + (Ki * I) + (Kd * D);

  previous_I = I;
  previous_error = error;
} 
void motor_control()
{
  // Calculating the effective motor speed:
  int left_motor_speed = initial_motor_speed - PID_value;
  int right_motor_speed = initial_motor_speed + PID_value;

  // The motor speed should not exceed the max PWM value
  left_motor_speed = constrain(left_motor_speed, 0, 255);
  right_motor_speed = constrain(right_motor_speed, 0, 255);
  right_motor_speed=right_motor_speed;
  /*Serial.print(PID_value);
    Serial.print("\t");
    Serial.print(left_motor_speed);
    Serial.print("\t");
    Serial.println(right_motor_speed);*/

  analogWrite(ENB, left_motor_speed); //Left Motor Speed
  analogWrite(ENA, right_motor_speed); //Right Motor Speed

  //following lines of code are to make the bot move forward
  forward();
}

void shortPath(){
 int shortDone=0;
  if(path[pathLength-3]=='L' && path[pathLength-1]=='R'){
    pathLength-=2;
    path[pathLength-1]='B';
    //Serial.println("test1");
    shortDone=1;
  }
   
  if(path[pathLength-3]=='L' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=2;
    path[pathLength-1]='R';
    //Serial.println("test2");
    shortDone=1;
  }
   
  if(path[pathLength-3]=='R' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=2;
    path[pathLength-1]='B';
    //Serial.println("test3");
    shortDone=1;
  }
  
   
  if(path[pathLength-3]=='S' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=2;
    path[pathLength-1]='R';
    //Serial.println("test4");
    shortDone=1;
  }
     
  if(path[pathLength-3]=='S' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=2;
    path[pathLength-1]='B';
    //Serial.println("test5");
    shortDone=1;
  }
    if(path[pathLength-3]=='L' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=2;
    path[pathLength-1]='S';
    //Serial.println("test6");
    shortDone=1;
  }
}




void loop() {
  read_sensor_values();
 
  if (error == 100) { 
 stop_bot();
 delay(150);
 analogWrite(ENA,140); //right Motor Speed
 analogWrite(ENB,140); //left Motor Speed
 sharpLeftTurn();
 delay(400);
do {
      read_sensor_values();
      sharpLeftTurn();
    } while (error != 0);
      

  path[pathLength]='L';
   // Serial.println("l");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
    if(path[pathLength-2]=='B'){
    //Serial.println("shortening path");
    shortPath();
     }
}
        
  
  else if (error == 101) {          // Make right turn in case of it detects only right path (it will go into forward direction in case of staright and right "|--")
         
                                      // untill it detects straigHT                              
    analogWrite(ENA, 62); //Left Motor Speed
    analogWrite(ENB, 62); //Right Motor Speed
    forward();
    delay(300);
    stop_bot();
    read_sensor_values();
    if (error == 102) {
      
       path[pathLength]='R';
   // Serial.println("r");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
    if(path[pathLength-2]=='B'){
    //Serial.println("shortening path");
    shortPath();
     }
      
      do {
        analogWrite(ENA, 140); //right Motor Speed
        analogWrite(ENB, 140); //left Motor Speed
        sharpRightTurn();
        read_sensor_values();
      }while (error != 0);
    }
    else{
      path[pathLength]='S';
    //Serial.println("s");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
      if(path[pathLength-2]=='B'){
        //Serial.println("shortening path");
        shortPath();
      }
      
      }
    }
  else if (error == 102) {        // Make left turn untill it detects straight path
       analogWrite(ENA, 80); //right Motor Speed
      analogWrite(ENB, 80); //left Motor Speed

    do {
      sharpLeftTurn();
      read_sensor_values();
    } while (error != 0);
 
     path[pathLength]='B';
    //Serial.println("b");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
      if(path[pathLength-2]=='B'){
        //Serial.println("shortening path");
        shortPath();
      }
   
    
  } else if (error == 103) {        // Make left turn untill it detects straight path or stop if dead end reached.
    
        
 
     analogWrite(ENA, 60); //right Motor Speed   // move 2 inches to confirm finish
      analogWrite(ENB, 60); //left Motor Speed
      forward();
      delay(300);
      stop_bot();
      read_sensor_values();
     
      if (error == 103) { 
        path[pathLength]='D';
    //Serial.println("l");
    pathLength++;/**** Dead End Reached, Stop! ****/
        stop_bot();
      digitalWrite(13,HIGH);
      delay(10000);
      digitalWrite(13,LOW);
     /* while(pathLength--)
      {
      EEPROM.put(add,path[i]);  
      add=add+sizeof(char);
      i++;
      }*/
      replay();
      } 
      
      else
      {
       stop_bot();
       delay(150);
       analogWrite(ENA,140); //right Motor Speed
       analogWrite(ENB,140); //left Motor Speed
       sharpLeftTurn();
       delay(400);
       do {
       read_sensor_values();
       sharpLeftTurn();
       } while (error != 0);
 
         
         path[pathLength]='L';
    //Serial.println("l");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
      if(path[pathLength-2]=='B'){
        //Serial.println("shortening path");
        shortPath();
      }
       
      }
    }
   
  
  else {
    calculate_pid();
    motor_control();
  }

}



void replay(){
  while(1)
  {
  read_sensor_values();
  if((error == 101) ||( error == 100) || (error==103) || (error==102)){
    if(path[readLength]=='D'){
     stop_bot();
    digitalWrite(13,HIGH);
    delay(5000);}
    
    else if(path[readLength]=='R'){
      do {
      read_sensor_values();
       analogWrite(ENA, 140); //right Motor Speed
      analogWrite(ENB, 140); //left Motor Speed
      sharpRightTurn();
       } while (error != 0);
       readLength++;
       //i=1;
       }

else if(path[readLength]=='B'){
      do {
      read_sensor_values();
       analogWrite(ENA, 140); //right Motor Speed
      analogWrite(ENB, 140); //left Motor Speed
      sharpRightTurn();
       } while (error != 0);
       readLength++;
       
       }
    
    else if(path[readLength]=='L'){
      stop_bot();
    delay(220);
    analogWrite(ENA,140); //right Motor Speed
    analogWrite(ENB,140); //left Motor Speed
    sharpLeftTurn();
    delay(400);
    do {
      read_sensor_values();
      sharpLeftTurn();
       } while (error != 0);
     readLength++;
       }
    
   else if(path[readLength]=='S'){
      analogWrite(ENA, 140); //right Motor Speed
      analogWrite(ENB, 140); //left Motor Speed
      forward();
      delay(300);
      readLength++;
      }
    
    }
  
 else{
   read_sensor_values();
    calculate_pid();
    motor_control();
  }
  }
}

