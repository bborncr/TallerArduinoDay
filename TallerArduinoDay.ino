/*Ejemplo de codigo para controlar un Arduino atraves del internet
*  
* Bentley Born @crcibernetica.com 2014 
*/

#include <SPI.h>        
#include <Ethernet.h>

#define LED1 2
#define LED2 3
#define MICROSWITCH 5

String commandString = "null";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 };//asegurar que el MAC no tiene conflicto con nada en su red local
byte ip[] = { 192, 168, 1, 2 };

EthernetServer server = EthernetServer(80);

void setup()
{
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(MICROSWITCH,INPUT);
  digitalWrite(MICROSWITCH,HIGH);
  
  Serial.begin(9600);
  Serial.println("Conexion serial lista...");
 
  Ethernet.begin(mac,ip);
  
  server.begin();
  delay(1000);
  Serial.print("IP: ");
  Serial.print((Ethernet.localIP()));
  Serial.println("...lista");
  
}

void loop()
{
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        if( client.find("GET /") ) {  // search for 'GET'
          // find tokens starting with "cmd" and stop on the first blank line
          while(client.findUntil("cmd", "\n\r")){  // search to the end of line for 'cmd'
            int cmd = client.parseInt(); // receive command number

            switch (cmd) {
            case 0 : 
              digitalWrite(LED1,HIGH); 
              commandString = ("LED1 PRENDIDO"); 
              break;
            case 1 : 
              digitalWrite(LED1,LOW); 
              commandString = ("LED1 APAGADO"); 
              break;
            case 2 : 
              digitalWrite(LED2,HIGH);
              commandString = ("LED2 PRENDIDO"); 
              break;
            case 3 : 
              digitalWrite(LED2,LOW);
              commandString = ("LED2 APAGADO"); 
              break;
            case 10 : 
              commandString = ("Taller Arduino con Ethernet v1.0"); 
              break;
            default: 
              commandString = ("Desconocido"); 
              Serial.println("Comando desconocido"); 
              break;
            }
          }
        }

        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();

        // Respond with the handled request
        client.print("Comando recibido: "); 
        client.print(commandString);
        client.println("<br />");
        client.println("<br />");
        
        client.print("Estatus del microswitch: ");
        client.print(digitalRead(MICROSWITCH));
        client.println("<br />");
        client.println("<br />");
        break;
      }
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
    commandString="null";
  }
}




