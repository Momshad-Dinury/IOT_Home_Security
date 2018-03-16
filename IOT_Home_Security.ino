//Including Library:
#include <ESP8266WiFi.h> //For WiFi Connectivity
#include <PubSubClient.h> //For MQTT


//WiFi ceredential:
const char* ssid = "STELLAR"; //chnage here
const char* password = "stellarBD"; //change here
WiFiClient Tasnim;
//MQTT Server:
const char* mqtt_server = "www.stellarbd.com"; //change here
PubSubClient client(Tasnim);

//Pir Sensor and LED
#define pirPin D1
#define ledPin D2

void setup() {
	Serial.begin(115200);
	Serial.println("System Initialized");
  	pinMode(pirPin, INPUT);
  	pinMode(pirPin, OUTPUT);
  	pinMode(ledPin, OUTPUT);

	//WiFi setup:
	delay(10);
  	Serial.println();
  	Serial.print("Connecting to ");
  	Serial.println(ssid);

  	WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  	Serial.println("");
  	Serial.println("WiFi connected");
  	Serial.println("IP address: ");
  	Serial.println(WiFi.localIP());
	Serial.println("WiFi Configured Successfully");

	//Mqtt server:
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);

}

void loop() {
	if (!client.connected()) { //This loop checks if mqtt is connected
    reconnect(); //If not then calls reconnect function
  }
  client.loop(); //This has to be run frequently to ensure MQTT is connected to server
  
  if(digitalRead(pirPin) == HIGH) {
    client.publish("Alert", "Motion Detected");
    digitalWrite(pirPin, LOW);
  }
}

//Callback function is used for MQTT Subscriber function:
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if((char)payload[0] == '1') {
    digitalWrite(ledPin, HIGH);
  }else if ((char)payload[0] == '0') {
    digitalWrite(ledPin, LOW);
  }

 }

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Tasnim")) {
      	Serial.println("Connected");
  
      	//Once connected, publish an announcement...
      	client.publish("Dashboard", "System started"); //EDIT HERE
        //... and resubscribe
      	client.subscribe("Action"); //EDIT HERE
    } else {
      	Serial.print("failed, rc=");
      	Serial.print(client.state());
      	Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      	delay(5000);
    }
  }
}
