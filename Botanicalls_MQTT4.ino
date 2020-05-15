#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#define MOISTPIN 0 // Moisture input is on analog pin 0
#define PROBEPOWER 8 // Power to the moisture probes
#define COMMLED 4 // COMM Red LED
#define STATUSLED 9 // Satus Green LED
#define SWITCH 3 // The "Test" switch - ***need to make this work***

byte mac[] = { 0x02, 0xBC, 0xA1, 0x15, 0x3A, 0x68 }; // MAC address
byte ip[] = { 10, 1, 1, 69 };  // IP Address
const char* mqtt_server = "rpi44gb"; // MQTT Server 
const char* mqtt_topic = "Botanicalls"; // MQTT Subject
const char* clientID = "arduino"; // MQTT user
EthernetClient ethClient; // Ethernet
PubSubClient client(mqtt_server, 1883, ethClient);  // Set the MQTT server to the server stated above 

//Define what is 'conect_MQTT'
void connect_MQTT(){
  Ethernet.begin(mac, ip);  // Start the ethernet connection              
  delay(3000); // Ethernet takes some time to boot!
  Serial.print("Connecting to "); // Useful Text
  if (client.connect(clientID)){ Serial.println("arduino@rpi44gb:1883");} // Connect MQTT server  
  else { Serial.println("Server Connection Failed");  } // Sad Times
  }

//Define the Programme
void setup() {
  pinMode(PROBEPOWER, OUTPUT); // Set the pins
  pinMode(COMMLED, OUTPUT); // Set the pins
  pinMode(STATUSLED, OUTPUT); // Set the pins
  digitalWrite(COMMLED, HIGH); // Turn on the Red LED
  Serial.begin(9600); // Start Serial Output
  connect_MQTT(); // Run the Loop from above
  Serial.println("Starting up"); //Print "Starting Up"
  Serial.println(mqtt_topic); // Print the MQTT Topic
  digitalWrite(COMMLED, LOW); // Turn off the Red LED
  }

void loop(){
  digitalWrite(PROBEPOWER, HIGH); // Turn the Power on to the probe
  digitalWrite(STATUSLED, HIGH); // Turn on the Green LED
  float moisture = analogRead(MOISTPIN); // Read the Moisture Value
  Serial.println(moisture); // Print the moisture level to the Serial port
  digitalWrite(PROBEPOWER, LOW); // Turn off the Power for the sensor
  delay(150); // Dealy to keep the LED on
  digitalWrite(STATUSLED, LOW); // Turn off the Green LED
  if (client.connect(clientID);client.publish(mqtt_topic, String(moisture).c_str())) { // Connect to MQTT Server and Go!
    Serial.println("Moisture Sent!"); // Happy Times
    }
  else {
    Serial.println("MQTT Fail"); // if it didn't work the first time
    client.connect(clientID); //Connect to MQTT Server
    delay(10); //Give it a sec
    client.publish(mqtt_topic, String(moisture).c_str()); // Send message (second atempt)
    Serial.println(moisture); // Print Moisture
    }
  delay(300000); // Sleep Time
}
