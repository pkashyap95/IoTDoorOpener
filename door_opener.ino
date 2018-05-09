#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

String AP = "";       // CHANGE ME
String PASS = ""; // CHANGE ME
String USER_ID = ""; //Username for IOTGecko
String USER_PASS = ""; //Password for IOTGecko
String PORT= "";
String HOST="";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int pinVal=0;

SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
//Physical pin connection, RX is on the TX Pin abd TX is on the RX pin
LiquidCrystal lcd(6,7,8,9,10,11);

bool open = false;
const int door0 = 12;
const int door1 = 13;
const int led = 5;
int door_status,first;
String data="";
int reset;
int counter=0;

void setup()
{
  Serial.begin(9600);
  //Door setup to close
  pinMode(door0,OUTPUT);
  pinMode(door1,OUTPUT);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW); 
  lcd.begin(16, 2); 
  lcd.clear();
  lcd.print("IOT Based Door Opener");
  digitalWrite(door0,HIGH);
  digitalWrite(door1,LOW);
  delay(2000);
  digitalWrite(door0,LOW);
  digitalWrite(door1,LOW);  
  //WiFi Setup
  lcd.clear();
  lcd.print("Connecting Wifi");
  esp8266.begin(9600); // your esp's baud rate might be different
  sendData("AT","OK",5,1); 
  sendData("AT+CWMODE=1","OK",5,1); // configure as access point
  sendData("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"","OK",10000,1);   
  lcd.clear();
  lcd.print("Connecting to iotgecko.com");
  delay(2000);
  sendData("AT+CIPSTART=\"TCP\",\""+ HOST +"\","+ PORT,"OK",15,1);
  lcd.clear();
  lcd.println("Connected");
  delay(1500);
  lcd.clear();
  lcd.print("   Door close   ");
  reset=0;
}

void loop()
{    
  if(reset==1)
  {
    lcd.clear();
    lcd.print("connection lost");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Reconnecting...");
    sendData("AT+CIPCLOSE","OK",5,1);
    sendData("AT+CIPSTART=\"TCP\",\""+ HOST +"\","+ PORT,"OK",15,1);
    reset=0;
    lcd.clear();
    lcd.print("Connected");
    delay(2000);
    //sendReq();
  }
  else
  {        
  int test=sendReq();
  counter++;
  if(counter%2==0){
    if((test==1) && open == false)    
    {
      lcd.clear();
      lcd.print("   Door open   ");
      Serial.println("door open");
      digitalWrite(led,HIGH);
      digitalWrite(door0,LOW);
      digitalWrite(door1,HIGH);
      delay(2000);
      digitalWrite(door0,LOW);
      digitalWrite(door1,LOW);
      open = true;     
    }
    else if((test==0) && open == true)
    {
      lcd.clear();
      lcd.print("   Door close   ");
      Serial.println("door close");
      digitalWrite(led,LOW);
      digitalWrite(door0,HIGH);
      digitalWrite(door1,LOW);
      delay(2000);
      digitalWrite(door0,LOW);
      digitalWrite(door1,LOW);
      open = false; 
    }
    reset=1;
  }
  sendData("AT+CIPCLOSE","OK",5,0);
  }
}

int sendReq(){
  sendData("AT+CIPSEND=75",">",100,0); //Change 75 to length of the request being sent
  esp8266.println(""); //Put GET request for the site with 
  countTrueCommand++;
  if(esp8266.find("pin=0")) //Compare the ESP response to expected response
  {
    pinVal = 0;
    return pinVal;
  }
  else //Compare the ESP response to expected response
  {
    pinVal = 1;
    return pinVal;
  }
}

void sendData(String cmd,char response[],int timeout,int debug) 
{
  Serial.print(countTrueCommand); 
  Serial.print(". at command => ");
  Serial.println(cmd); //What was put in the loop
  while(countTimeCommand < (timeout*1))
  {
    esp8266.println(cmd);//Sends AT command to the WiFi module
    if(esp8266.find(response)) //Compare the ESP response to expected response
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
    reset=1;
  }
}
