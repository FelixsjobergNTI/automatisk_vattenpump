const int buttonPin = 3;      // Port för knappen
const int pumpPin = 2;        // Port för vattenpumpen
const int redLampPin = 4;     // Port för den röda lampan
const int greenLampPin = 5;   // Port för den gröna lampan
const int moistureSensorPin = A0; // Port för fuktsensorn


bool pumpActive = false; // Flagga för att hålla koll på om pumpen är aktiv eller inte
bool greenLampActive = false; // Flagga för att hålla koll på om den gröna lampan har lyst
bool redLampBlinking = false; // Flagga för att hålla koll på om den röda lampan blinkar


void setup() {
  pinMode(buttonPin, INPUT_PULLUP);  // Sätt knappen som en ingång med pull-up motstånd
  pinMode(pumpPin, OUTPUT);          // Sätt pumpporten som en utgång
  pinMode(redLampPin, OUTPUT);       // Sätt den röda lamppinnen som en utgång
  pinMode(greenLampPin, OUTPUT);     // Sätt den gröna lamppinnen som en utgång
  pinMode(moistureSensorPin, INPUT); // Sätt fuktsensorn som en ingång
  


  Serial.begin(9600); // Starta kommunikationen med Serial Monitor
}


void loop() {
  int moistureLevel = analogRead(moistureSensorPin); // Läs av fuktsensorn


  // Skicka fuktsensorns värde till Serial Monitor
  Serial.print("Fuktnivå: ");
  Serial.println(moistureLevel);


  // Kontrollera om knappen är tryckt och den röda lampan blinkar
  if (digitalRead(buttonPin) == LOW && redLampBlinking) {
    // Om knappen är tryckt och den röda lampan blinkar, sätt igång pumpen
    pumpActive = true;
    digitalWrite(pumpPin, HIGH); // Tänd pumpen
  }


  if (moistureLevel < 500) {
    // Om fuktsensorn är torr (analogRead ger en låg nivå)
    digitalWrite(redLampPin, LOW); // Släck den röda lampan
    if (!greenLampActive) {
      digitalWrite(greenLampPin, HIGH); // Tänd den gröna lampan om den inte har lyst tidigare
      delay(5000); // Håll den gröna lampan tänd i 3 sekunder
      digitalWrite(greenLampPin, LOW); // Släck den gröna lampan
      greenLampActive = true; // Markera att den gröna lampan har lyst
    }


    // Stoppa pumpen om den är aktiv och den gröna lampan har lyst
    if (pumpActive && greenLampActive) {
      digitalWrite(pumpPin, LOW); // Släck pumpen
      pumpActive = false; // Återställ flaggan för pumpen
    }
  } else {
    // Om fuktsensorn är blöt, sluta blinka den röda lampan och släck den gröna lampan
    digitalWrite(greenLampPin, LOW); // Släck den gröna lampan


    // Blinka den röda lampan
    for (int i = 0; i < 5; ++i) {
      digitalWrite(redLampPin, HIGH); // Tänd den röda lampan
      delay(500); // Vänta i 0,5 sekund
      digitalWrite(redLampPin, LOW); // Släck den röda lampan
      delay(500); // Vänta i 0,5 sekund
    }


    greenLampActive = false; // Återställ flaggan för den gröna lampan
    redLampBlinking = true; // Markera att den röda lampan blinkar
  }


  delay(1000); // Vänta i 1 sekund innan nästa avläsning
}
