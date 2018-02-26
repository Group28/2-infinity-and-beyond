

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <SimpleTimer.h>

const char* ssid     = "Your Mum!";
const char* password = "nPHFPSfDRRfedqaYQyZcdXJLaJa1uuNAkwtQQDBntOQ=";


// END ***********************************************************************

ESP8266WebServer server(80);
WebSocketsServer webSocket=WebSocketsServer(88);

SimpleTimer timer;

String webSite =R"(<!DOCTYPE HTML><html>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <head></head>
  <body>
  <h1 style='float: left; font-family:monospace; font-size: 30px;'>2 &infin; and beyond - Command Center</h1>
  <img style='height:150px; float:right;' src='https://secure.img1-fg.wfcdn.com/im/49483365/resize-h800%5Ecompr-r85/3557/3557499/Toy+Story+Buzz+Giant+Wall+Decal.jpg'></img><BR style='clear:both;'>
  Runtime: <a id='runtime'></a><br>

  <input placeholder='Command' onkeydown='search(this)' id='tx' style='width:90%; font-family:monospace;'>
  <button id='button' onclick='button()'>Send</button>
  <pre id='rx' style='padding: 10px 0 5px 5px; background: rgb(228,228,228); max-height:30em; overflow: auto'></pre>
  
  <script>
  function InitWebSocket(){
    websock=new WebSocket('ws://'+window.location.hostname+':88/');
    websock.onmessage=function(evt){
      JSONobj=JSON.parse(evt.data);
      document.getElementById('runtime').innerHTML=JSONobj.runtime;
      if(JSONobj.rx){
       var element = document.getElementById('rx');
       element.innerHTML+=JSONobj.rx;
       element.scrollTop = element.scrollHeight;
      }
    }
  }
  function button(){
   websock.send('COMMAND=' + document.getElementById('tx').value + '\n');
   document.getElementById('tx').value = '';
  }
  var keys = [];
  function checkKeys(){
   if(keys[38]) {websock.send('COMMAND=w\n');}
   if(keys[40]) {websock.send('COMMAND=s\n');}
   if(keys[37]) {websock.send('COMMAND=a\n');}
   if(keys[39]) {websock.send('COMMAND=d\n');}
   if(!keys[38] && !keys[40] && !keys[37] && !keys[39]){websock.send('COMMAND=f\n');}
  }
  document.addEventListener('keydown', function(event){
    if(event.repeat){return;}
   if(event.keyCode == 37 || event.keyCode == 38 || event.keyCode == 39 || event.keyCode == 40){
    keys[event.keyCode] = true;
    checkKeys();
   }
  });
  document.addEventListener('keyup', function(event){
    if(event.keyCode == 37 || event.keyCode == 38 || event.keyCode == 39 || event.keyCode == 40){
      keys[event.keyCode] = false;
      checkKeys();
    }
  });
  function search(){
   if(event.keyCode == 13) {button()};
  }
  InitWebSocket();
  </script>
  
  </body>
 </html>)";
  


String JSONtxt;
unsigned long websockCount=0;


void intervalUpdate() {
    websockCount++;
    JSONtxt="{\"runtime\":\""+millis2time()+"\","+              // JSON requires double quotes
             "\"websockCount\":\""+(String)websockCount+"\"}";
    webSocket.broadcastTXT(JSONtxt);
}

String millis2time(){
  String Time="";
  unsigned long ss;
  byte mm,hh;
  ss=millis()/1000;
  hh=ss/3600;
  mm=(ss-hh*3600)/60;
  ss=(ss-hh*3600)-mm*60;
  if(hh<10)Time+="0";
  Time+=(String)hh+":";
  if(mm<10)Time+="0";
  Time+=(String)mm+":";
  if(ss<10)Time+="0";
  Time+=(String)ss;
  return Time;
}

void handleWebsite(){
  server.send(200,"text/html",webSite);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t wslength){
  String payloadString=(const char *)payload;
  if(payloadString.startsWith("COMMAND="))
    Serial.print(payloadString.substring(8));

}

void setup() {
  Serial.begin(115200); 
  //pinMode(LED,OUTPUT);
  WiFi.hostname("buggy28");
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    //Serial.print(".");
    delay(500);
  }
  WiFi.mode(WIFI_STA);
  //Serial.print("\nConnected to ");
  //Serial.println(ssid);
  //Serial.print("Station IP address = ");
  //Serial.println(WiFi.localIP());

  
  
  server.on("/",handleWebsite);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  timer.setInterval(500, intervalUpdate);

}

void loop() {
  char inChar;
  String inputString = "";
  
  webSocket.loop();
  timer.run();
  server.handleClient();
  while (Serial.available()) {
    // get the new byte:
    inChar = (char)Serial.read();
   
     
    if(inChar == '"') {
      inputString += "\\\"";
    } else if (inChar == '\r'){
      inputString += "\\r";
    } else if (inChar == '\n'){
      inputString += "\\n";
    } else {
      inputString += inChar;
    }  
  }
  if(inputString != ""){
    websockCount++;
    JSONtxt="{\"runtime\":\""+millis2time()+"\","+              // JSON requires double quotes
             "\"websockCount\":\""+(String)websockCount+"\","+
             "\"rx\":\""+inputString+"\"}";
    webSocket.broadcastTXT(JSONtxt);
  }
}



