#include <WiFiS3.h>
#include <ThingSpeak.h>

char ssid[] = "wifi_ssid";       
char pass[] = "wifi_password"; 

WiFiClient client;

unsigned long channelID = [channel ID];  
const char* writeAPIKey = "write API Key";

#define rainAnalog A0  // Αναλογική είδοσος του αισθητήρα βροχής
#define rainDigital 2  // Ψηφιακή είσοδος του αισθητήρα βροχής
#define sensorPin  A5  // Αναλογική είσοδος δεξαμενής
#define hygrometer  A2

int alarm=7;
void setup() {
    Serial.begin(9600);
    pinMode(rainDigital, INPUT);
    pinMode(alarm, OUTPUT);
    WiFi.begin(ssid, pass);
    Serial.print("Σύνδεση στο WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nΣυνδέθηκε στο WiFi!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Ενεργοποίηση ThingSpeak
    ThingSpeak.begin(client);
}

void loop() {
    int analogValue = analogRead(rainAnalog);
    int digitalValue = digitalRead(rainDigital);
    
    Serial.print("Βροχή-Αναλογική τιμή: ");
    Serial.println(analogValue);
    
    Serial.print("Βροχή-Ψηφιακή τιμή: ");
    Serial.println(digitalValue);

   if (digitalValue == LOW) {
        Serial.println("ΒΡΟΧΗ ΑΝΙΧΝΕΥΤΗΚΕ!");
    } else {
        Serial.println("ΧΩΡΙΣ ΒΡΟΧΗ.");
    }

    int sensorValue = analogRead(sensorPin);

  Serial.print("Τιμή Υγρασίας δεξαμενής: ");
  Serial.println(sensorValue); // 0 (στεγνό) έως ~1023 (βρεγμένο)
  if (sensorValue>300)
  {digitalWrite(alarm,HIGH);}
  else
  {digitalWrite(alarm,LOW);}

  int value = analogRead(hygrometer); 
  value = constrain(value,400,1023); 
  value = map(value,400,1023,100,0);
  Serial.print("Υγρασία εδάφους: ");
  Serial.print(value);
  Serial.println("%");

    // Αποστολή στο ThingSpeak
      ThingSpeak.setField(1, sensorValue);   // sensorPin
      ThingSpeak.setField(2, analogValue);   // rainAnalog
      ThingSpeak.setField(3, digitalValue);  // rainDigital
      ThingSpeak.setField(4, value);     // hygrometer
  
      int x = ThingSpeak.writeFields(channelID, writeAPIKey);
      if (x == 200) {
          Serial.println("Επιτυχής αποστολή στο ThingSpeak.");
      } else {
          Serial.print("Σφάλμα αποστολής. Κωδικός: ");
          Serial.println(x);
      }
     
      delay(20000);  // Αναμονή 20 δευτερόλεπτα  
}
