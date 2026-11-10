#include <SPI.h>
#include <Adafruit_PN532.h>

// Pin definitions for PN532 modules
#define PN5321_SCK  (18)
#define PN5321_MISO (19)
#define PN5321_MOSI (23)
#define PN5321_SS   (25)

#define PN5322_SCK  (18)
#define PN5322_MISO (19)
#define PN5322_MOSI (23)
#define PN5322_SS   (27)

#define PN5323_SCK  (18)
#define PN5323_MISO (19)
#define PN5323_MOSI (23)
#define PN5323_SS   (26)

// Buzzer pin
#define BUZZER_PIN   (2)
#define BUTTON_PIN 33

// PN532 objects
Adafruit_PN532 nfc1(PN5321_SCK, PN5321_MISO, PN5321_MOSI, PN5321_SS);
Adafruit_PN532 nfc2(PN5322_SCK, PN5322_MISO, PN5322_MOSI, PN5322_SS);
Adafruit_PN532 nfc3(PN5323_SCK, PN5323_MISO, PN5323_MOSI, PN5323_SS);

// Item database (card UID as key)
struct Item {
  String uid;
  bool registered;
  bool sold;
};

Item items[10]; // Array for up to 10 items
int itemCount = 0; // Current number of items

// Stored card UIDs
String storedCards[3];
int registeredCardCount = 0;

// Array to track sold items
String soldItems[10];
int soldItemCount = 0;
bool pressflag = true;

void setup() {
  Serial.begin(9600);
  delay(1000);

  // Initialize pins
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Set button pin as INPUT
  pinMode(BUTTON_PIN, INPUT);

  // Set SS pins as OUTPUT and initialize HIGH
  pinMode(PN5321_SS, OUTPUT);
  pinMode(PN5322_SS, OUTPUT);
  pinMode(PN5323_SS, OUTPUT);

  digitalWrite(PN5321_SS, HIGH);
  digitalWrite(PN5322_SS, HIGH);
  digitalWrite(PN5323_SS, HIGH);

  // Initialize PN532 modules
  initPN532(nfc1, "Module 1", PN5321_SS);
  initPN532(nfc2, "Module 2", PN5322_SS);
  initPN532(nfc3, "Module 3", PN5323_SS);

  Serial.println("Registering three cards using Module 1...");

  // Register exactly three cards
  String registeredCards[3];
  int registeredCount = 0;

  while (registeredCount < 3) {
    digitalWrite(PN5321_SS, LOW);
    delay(10);

    uint8_t success;
    uint8_t uid[7]; // Buffer to store UID
    uint8_t uidLength;

    success = nfc1.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
    if (success) {
      String cardUID = "";
      for (byte i = 0; i < uidLength; i++) {
        cardUID += String(uid[i], HEX);
      }
      cardUID.toUpperCase();

      // Check if card is already registered
      bool alreadyRegistered = false;
      for (int i = 0; i < registeredCount; i++) {
        if (registeredCards[i] == cardUID) {
          alreadyRegistered = true;
          break;
        }
      }

      if (!alreadyRegistered) {
        registeredCards[registeredCount] = cardUID;
        registeredCount++;
        Serial.println("Card " + String(registeredCount) + " registered: " + cardUID);

        // Beep to indicate successful registration
        digitalWrite(BUZZER_PIN, HIGH);
        delay(50);
        digitalWrite(BUZZER_PIN, LOW);
        delay(50);

      } else {
        Serial.println("Card already registered: " + cardUID);
      }
    }
    digitalWrite(PN5321_SS, HIGH);
    delay(500); // Prevent rapid polling
  }

  Serial.println("Three cards have been registered. Proceeding with the system...");
  Serial.println("Stored Cards:");
  for (int i = 0; i < 3; i++) {
    Serial.println("Card " + String(i + 1) + ": " + registeredCards[i]);
  }

  Serial.println("System ready.");
}

void loop() 
{
  // Check if the button is pressed (active high)
  if (digitalRead(BUTTON_PIN) == HIGH && pressflag==true)
  {
    Serial.println("Button pressed! Switching from Module 3 to Module 2...");
    processModule2();
    pressflag=false;
    delay(500); // Debounce delay to avoid multiple triggers
    
  }
  else
  {
    Serial.println("Processing Module 3...");
    pressflag=true;
    processModule3();
  }

delay(500); // Small delay to prevent constant polling
}


// Function to scan and store a card
void scanAndStoreCard(Adafruit_PN532 &nfc, const char *moduleName, int ssPin) {
  // Activate the SS pin to low for the selected module to allow communication
  digitalWrite(ssPin, LOW);
  delay(10);

  uint8_t success;
  uint8_t uid[7]; // Buffer to store UID
  uint8_t uidLength;

  // Read card UID from the module
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    String cardUID = "";
    for (byte i = 0; i < uidLength; i++) {
      cardUID += String(uid[i], HEX);
    }
    cardUID.toUpperCase();

    // Check if the card is already stored
    for (int i = 0; i < registeredCardCount; i++) {
      if (storedCards[i] == cardUID) {
        Serial.println("Card already registered: " + cardUID);
        digitalWrite(ssPin, HIGH);  // Deactivate the SS pin
        return;
      }
    }

    // Store the new card
    storedCards[registeredCardCount] = cardUID;
    registeredCardCount++;
    Serial.println("Card registered: " + cardUID);

    // Beep to indicate successful registration
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
  }

  // Deactivate the SS pin after processing
  digitalWrite(ssPin, HIGH);
}

// Function to initialize the PN532 modules
void initPN532(Adafruit_PN532 &nfc, const char *moduleName, int ssPin) {
  digitalWrite(ssPin, LOW);
  delay(10);

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print(moduleName);
    Serial.println(": Didn't find PN53x board");
    digitalWrite(ssPin, HIGH);
    return;
  }

  nfc.SAMConfig();
  digitalWrite(ssPin, HIGH);
}


// Function specifically for processing Module 2 (mark as sold)
void processModule2() {
  Serial.println("Activating Module 2...");
  digitalWrite(PN5322_SS, LOW); // Activate SS pin for Module 2
  delay(10);

  uint8_t success;
  uint8_t uid[7];
  uint8_t uidLength;

  // Attempt to read a card UID
  success = nfc2.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    String cardUID = "";
    for (byte i = 0; i < uidLength; i++) {
      cardUID += String(uid[i], HEX);
    }
    cardUID.toUpperCase();

    Serial.println("Module 2 detected card UID: " + cardUID);

    // Mark the item as sold
    markItemSold(cardUID);

  } else {
    Serial.println("Module 2 no card detected");
  }

  digitalWrite(PN5322_SS, HIGH); // Deactivate SS pin for Module 2
  Serial.println("Deactivating Module 2...");
}

// Function specifically for processing Module 3 (verification)
void processModule3() {
  Serial.println("Activating Module 3...");
  digitalWrite(PN5323_SS, LOW); // Activate SS pin for Module 3
  delay(10);

  uint8_t success;
  uint8_t uid[7];
  uint8_t uidLength;

  // Attempt to read a card UID
  success = nfc3.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    String cardUID = "";
    for (byte i = 0; i < uidLength; i++) {
      cardUID += String(uid[i], HEX);
    }
    cardUID.toUpperCase();

    Serial.println("Module 3 detected card UID: " + cardUID);

    // Verify the item
    verifyItem(cardUID);

  } else {
    Serial.println("Module 3 no card detected");
  }

  digitalWrite(PN5323_SS, HIGH); // Deactivate SS pin for Module 3
  Serial.println("Deactivating Module 3...");
}



// Function to process checkpoints
void processCheckpoint(Adafruit_PN532 &nfc, const char *moduleName, int ssPin, int checkpoint) {
  digitalWrite(ssPin, LOW);
  delay(10);

  uint8_t success;
  uint8_t uid[7];
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    String cardUID = "";
    for (byte i = 0; i < uidLength; i++) {
      cardUID += String(uid[i], HEX);
    }
    cardUID.toUpperCase();

    Serial.println(moduleName + String(" detected card UID: ") + cardUID);

    if (checkpoint == 1) {
      registerItem(cardUID);
    } else if (checkpoint == 2) {
      markItemSold(cardUID);
    } else if (checkpoint == 3) {
      verifyItem(cardUID);
    }
  } else {
    Serial.println(moduleName + String(" no card detected"));
  }

  digitalWrite(ssPin, HIGH); // Deactivate the SS pin
}

void registerItem(String cardUID) {
  Serial.println("Item Registered: " + cardUID);
}

void markItemSold(String cardUID) {
  // Mark the card UID as sold
  for (int i = 0; i < 10; i++) {
    if (soldItems[i] == "") {
      soldItems[i] = cardUID;
      soldItemCount++;
      Serial.println("Item Sold: " + cardUID);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(50);
      digitalWrite(BUZZER_PIN, LOW);
      delay(50);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(50);
      digitalWrite(BUZZER_PIN, LOW);
      delay(50);
      return;
    }
  }
  Serial.println("Sold item array is full.");
}

void verifyItem(String cardUID) {
  // Check if the item has been marked as sold
  bool found = false;
  for (int i = 0; i < soldItemCount; i++) {
    if (soldItems[i] == cardUID) {
      found = true;
      break;
    }
  }

  if (!found) {
    // If item is not sold, trigger the buzzer for 2 seconds
    Serial.println("Item not paid, triggering buzzer!");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(2000); // Beep for 2 seconds
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    // Item is already sold, beep 3 times
    Serial.println("Item already paid for.");

    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(50);
  }

}
