

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <SimpleTimer.h>


// If defned, configures the ESP in clinent mode
// Comment out for AP mode
#define CLIENT

const char* ssid     = "Your Mum!";
const char* password = "nPHFPSfDRRfedqaYQyZcdXJLaJa1uuNAkwtQQDBntOQ=";


// END ***********************************************************************
String millis2time();
ESP8266WebServer server(80);
WebSocketsServer webSocket=WebSocketsServer(88);

SimpleTimer timer;

String webSite =R"(
  <!DOCTYPE HTML><html>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <head>
      <style>
        h1 {
          float: left;
          font-family:monospace;
          font-size: 30px;
        }
        .logo{
          height:150px;
          float:right;
        }
        .container {
          width: 96%;
          height: 400px;
          margin: auto;
          padding: 0px;
          
        }
        .commands {
          padding: 10px;
          width: 30%;
          height: 100%;
          float: left;
        }
        .widgets {
          padding: 10px;
          float: left;
          height: 100%;
        }
        .sensors{
          width: 100px;
          position: relative;
          
        }
        .sensor{
          font-family: monospace;
          text-align: center;
          font-size: 1.1em;
          padding: 10px;
          width: 30px;
          height: 30px;
          position: absolute;
          border: #8BC34B solid 5px;
        }
        
        .sensor div{
          width:100%;
          height:100%;
          background: #ffffffbf;
        }
        .sensor div span {
          
          font-size:0.6em;
        }
        #sensor0 {
          top:0px;
          left:0px;
          background: #000000ff;
        }
        #sensor1 {
          top: 0px;
          left: 275px;
          background: #000000ff;
        }
        #sensor2 {
          top: 90px;
          left: 80px;
          background: #000000ff;
        }
        #sensor3 {
          top: 90px;
          left: 195px;
          background: #000000ff;
        }
        #sensor4 {
          top: 160px;
          left: 100px;
          background: #000000ff;
        }
        #sensor5 {
          top: 160px;
          left:175px;
          background: #000000ff;
        }
        
      
      </style>
    
    </head>
    <body>
    <h1>2 &infin; and beyond - Command Center</h1>
    <img class="logo" src='https://secure.img1-fg.wfcdn.com/im/49483365/resize-h800%5Ecompr-r85/3557/3557499/Toy+Story+Buzz+Giant+Wall+Decal.jpg'></img><BR style='clear:both;'>
    <div class="container">
      <div class="commands">
      Runtime: <a id='runtime'></a><br>

      <input placeholder='Command' onkeydown='search(this)' id='tx' style='width:90%; font-family:monospace;'>
      <button id='button' onclick='button()'>Send</button>
      <pre id='rx' style='padding: 10px 0 5px 5px; background: rgb(228,228,228); max-height:30em; overflow: auto'></pre>
      </div>
      <div class="widgets">
        <h3> Sensors </h3>
        <div class="sensors">
          <div class="sensor" id="sensor0"><div>0 <span id="sensor0Val">0.000</span></div></div>
          <div class="sensor" id="sensor1"><div>1 <span id="sensor1Val">0.000</span></div></div>
          <div class="sensor" id="sensor2"><div>2 <span id="sensor2Val">0.000</span></div></div>
          <div class="sensor" id="sensor3"><div>3 <span id="sensor3Val">0.000</span></div></div>
          <div class="sensor" id="sensor4"><div>4 <span id="sensor4Val">0.000</span></div></div>
          <div class="sensor" id="sensor5"><div>5 <span id="sensor5Val">0.000</span></div></div>
        </div>
      
      </div>
    </div>
    <script>
    function setSensor(sensor, value){
        var sensorSpan = document.getElementById('sensor' + sensor + 'Val');
        var sensor = document.getElementById('sensor'+sensor);
        sensorSpan.innerHTML = value.toFixed(3);
        sensor.style.backgroundColor = "rgba(0,0,0,"+ (1 - value) +')';
        
    }
    
    function processData(data){
      JSONobj=JSON.parse(data);
      document.getElementById('runtime').innerHTML=JSONobj.runtime;
      if(JSONobj.rx){
        if(JSONobj.rx.startsWith('JSON={')){
          var msg = JSON.parse(JSONobj.rx.substring(5));
          setSensor("0", msg["0"]);
          setSensor("1", msg["1"]);
          setSensor("2", msg["2"]);
          setSensor("3", msg["3"]);
          setSensor("4", msg["4"]);
          setSensor("5", msg["5"]);
          
        } else {
          var element = document.getElementById('rx');
          element.innerHTML+=JSONobj.rx;
          element.scrollTop = element.scrollHeight;
        }
      }
      
    }
    function InitWebSocket(){
      websock=new WebSocket('ws://'+window.location.hostname+':88/');
      websock.onmessage=function(evt){
        processData(evt.data);
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
   </html>

  
)";
  

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
  Serial.setTimeout(50);

  WiFi.hostname("buggy28");

#ifdef CLIENT
  
  // Set up WIFI in clinet mode
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    //Serial.print(".");
    delay(500);
  }
  WiFi.mode(WIFI_STA);

#else
  
  // Set up WIFI in AP mode  
  WiFi.softAP(ssid,password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
#endif
  
  server.on("/",handleWebsite);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  timer.setInterval(500, intervalUpdate);

}

void loop() {
  String inputString = "";
  
  webSocket.loop();
  timer.run();
  server.handleClient();
  if(Serial.available()) {
    // get the new byte:
    inputString = Serial.readString();
   
    inputString.replace("\"", "\\\"");
    inputString.replace("\r", "\\r");
    inputString.replace("\n", "\\n");

    websockCount++;
    JSONtxt="{\"runtime\":\""+millis2time()+"\","+              // JSON requires double quotes
             "\"websockCount\":\""+(String)websockCount+"\","+
             "\"rx\":\""+inputString+"\"}";
    webSocket.broadcastTXT(JSONtxt);
  }
}
