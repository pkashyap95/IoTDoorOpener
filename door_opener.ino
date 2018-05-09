#include <SoftwareSerial.h>


String AP = "";       // CHANGE ME
String PASS = ""; // CHANGE ME

String USER_ID = ""; //Username for IOTGecko
String USER_PASS = ""; //Password for IOTGecko
String PORT= "80";
String HOST="";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int pinVal=0;
SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
//Physical pin connection, RX is on the TX Pin abd TX is on the RX pin

String data="";
int reset;
int counter=0;

void setup()
{
  Serial.begin(9600); 
  Serial.println("    IOT Based Door Opener   ");
  delay(2000);
  Serial.println("Connecting Wifi"+ HOST);
  esp8266.begin(9600); // your esp's baud rate might be different
  sendData("AT","OK",5,1); 
  sendData("AT+CWMODE=1","OK",5,1); // configure as access point
  sendData("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"","OK",10000,1);   
  Serial.println("Connecting to iotgecko.com");
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
    sendData("AT+CIPCLOSE","OK",5,1);
    sendData("AT+CIPSTART=\"TCP\",\""+ HOST +"\","+ PORT,"OK",15,1);
    reset=0;
    Serial.println("Connected");
    delay(2000);
    //sendReq();
  }
  else
  {        
  int test=sendReq();
  counter++;
  if(counter%2==0){
    Serial.println("pinVal is:" + String(test));
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
