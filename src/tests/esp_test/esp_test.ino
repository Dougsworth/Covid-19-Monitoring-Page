#define DEBUG true

#define NETWORK_NAME "networkName"
#define NETWORK_PASSWORD "networkPassword"

void espSetup(){
  String networkName = NETWORK_NAME;
  String networkPassword = NETWORK_PASSWORD;
  
  // Reset the esp in case of power outage
  sendData("AT+RST\r\n", 10000, DEBUG);
  
  // Configure ESP to operate as client
  sendData("AT+CWMODE=3\r\n", 10000, DEBUG);

  // List access points
//  sendData("AT+CWLAP\r\n", 10000, DEBUG);

  // Join an access point
  sendData("AT+CWJAP=\"" +networkName+ "\",\""+networkPassword+"\"\r\n", 5000, DEBUG);

  // Verify that access point has been joined
  //sendData("AT+CIFSR\r\n", 3000, DEBUG);  
}

String getMacAddress(){
  String response = "";
  response = sendData("AT+CIPSTAMAC?\r\n\r\n", 3000, false);  
  return response.substring(42, 59);
}

String sendData(String command, const int timeout, boolean debug) {
    String response = "";
    
    Serial.print(command); // send the read character to the esp8266
    
    unsigned long time = millis();
    
    while( (time+timeout) > millis())
    {
      while(Serial.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = Serial.read(); // read the next character.
        response += c;
      }  
    }
    
//    if(debug)
//    {
//      Serial.print(response);
//    }
    
    return response;
}

String generatePostRequest(String route, String portNumber, int cLength, String pData) {
  String requestType = "POST /" + route + " HTTP/1.1\r\n";
  String hostInfo = "Host: 192.168.1.12:" + portNumber + "\r\n";
  String contentType = "Content-Type: application/json\r\n";
  String contentLength = "Content-Length: " + String(cLength) + "\r\n\r\n";
  String postData = pData + "\r\n\r\n";

  return requestType + hostInfo + contentType + contentLength + postData;
}

String generateCIPSend(int requestLength){
  String cipSend = "AT+CIPSEND=" + String(requestLength) + "\r\n";
  
  return cipSend;
}

String generatePost(int temp, int y, int m, int d, int h, int mn, int s){
  String post = "{\"temperature\":\""+ String(temp)+ "\", \"year\":\"" +String(y)+ "\", \"month\":\""+String(m)+ "\", \"day\":\""+String(d)+"\", \"hour\":\"" +String(hr)+ "\", \"min\":\""+String(mn)+ "\", \"sec\":\""+String(s)+"\"}\r\n\r\n";
  Serial.println(post);
  return post;
}

void postToServer(){
  // Get relevant data from the sensors
  int temp = 69;
  int year = 2021;
  int month = 8;
  int day = 30;
  int hour = 6;
  int min = 30;
  int sec = 29;

  // Prepare the structure of the POST request
  sendData("AT+CIPSTART=\"TCP\",\"192.168.1.12\",5000\r\n", 1500, DEBUG);
  String postData = generatePost(temp, year, month, day, hour, min, sec);
  String postRequest = generatePostRequest("test", "5000", postData.length(), postData);  
  String cipSend = generateCIPSend(postRequest.length());

  // Make the POST request
  sendData(cipSend, 1000, DEBUG);
  Serial.println(postRequest);
  sendData(postRequest, 5000, DEBUG);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial1.begin(115200);
  espSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  postToServer();
}
