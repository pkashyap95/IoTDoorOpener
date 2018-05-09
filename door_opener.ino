#include <SoftwareSerial.h>

String inputString="";
String wifi_response="";
String chk_str="";

String AP = "";       // change access point name
String PASS = "";     //change access point password

String USER_ID = "";    //Username for Webpage
String USER_PASS = "";  //Password for Webpage
String PORT= "80";
String HOST=""; //Change host name

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 

SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
//Physical pin connection, RX is on the TX Pin abd TX is on the RX pin

String response = "";
String str,data;
bool open = false;
const int door0 = 12;
const int door1 = 13;
const int led = 5;
int door_status,reset,first;

void setup()
{
  pinMode(door0,OUTPUT);
  pinMode(door1,OUTPUT);
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  Serial.begin(9600); 
  Serial.println("    IOT Based Door Opener   ");
  digitalWrite(door0,HIGH);
  digitalWrite(door1,LOW);
  delay(2000);
  digitalWrite(door0,LOW);
  digitalWrite(door1,LOW);
  Serial.print("Connecting Wifi"+ HOST);
  esp8266.begin(9600); // your esp's baud rate might be different
  sendData("AT","OK",5,1); 
  sendData("AT+CWMODE=1","OK",5,1); // configure as access point
  sendData("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"","OK",10000,1);   
  Serial.print("Connecting to iotgecko.com");
  delay(2000);
  sendData("AT+CIPSTART=\"TCP\",\""+ HOST +"\","+ PORT,"OK",15,1);
  Serial.println("Connected");
  delay(1500);
  Serial.println("   Door close   ");
  reset=0;
}

void loop()
{    
  if(reset==1)
  {
    Serial.println("connection lost");
    delay(1000);
    Serial.println("Reconnecting...");
    sendData("AT+CIPCLOSE=0","OK",1000,1);
    sendData("AT+CWMODE=1","OK",5,1); // configure as access point
    sendData("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"","OK",10000,1);   
    sendData("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,"OK",15,1);
    reset=0;
    Serial.print("Connected");
    delay(2000);
    
  }
  else
  {        
    String getData = "GET http://"+ HOST +"/IOTHit.aspx?id="+ USER_ID +"&Pass"+ USER_PASS;
   
    String len=String(str.length()) + "\r\n" ;
    sendData("AT+CIPSEND=0,"+String(getData.length()+4),">",100,0);
    esp8266.println(getData);
    int i=0;
    long looptime = millis();
    while(data!=NULL)
    {       
     // if(response[i++] == 'd')
        if(data[i++] == 'p')
          if(data[i++] == 'i')
              //if(inputString[i++] == 'n')
              //if(inputString[i++] == '=')
              {
                i = i+2;
                door_status = (data[i++] - 48);
                break;
              }     
       if(millis() - looptime > 5000)
       {
        break;
       }      
    }
    if((door_status==1) && open == false)    
    {
      Serial.print("   Door open   ");
      digitalWrite(led,HIGH);
      digitalWrite(door0,LOW);
      digitalWrite(door1,HIGH);
      delay(2000);
      digitalWrite(door0,LOW);
      digitalWrite(door1,LOW);
      open = true;     
    }
    else if((door_status==0) && open == true)
    {
      Serial.print("   Door close   ");
      digitalWrite(led,LOW);
      digitalWrite(door0,HIGH);
      digitalWrite(door1,LOW);
      delay(2000);
      digitalWrite(door0,LOW);
      digitalWrite(door1,LOW);
      open = false; 
    }
    delay(1000);
  }
      
}

void sendData(String cmd,char response[],int timeout,int debug) 
{
  Serial.print(countTrueCommand); 
  Serial.print("at command => ");
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
  }
  
  found = false;
} 
