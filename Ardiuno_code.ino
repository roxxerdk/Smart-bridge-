#include <Servo.h>           // Include Servo library
#include <Wire.h>            // Include I2C library
#include <Adafruit_GFX.h>    // Include Adafruit GFX library
#include <Adafruit_SSD1306.h> // Include Adafruit SSD1306 OLED library

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Servo tap_servo;

// Pin definitions
const int sensor_pin = 5;    // Digital output (DO) pin of the soil moisture sensor
const int tap_servo_pin = 4; // Servo signal pin
const int sensor_vcc = 7;    // Power pin for the sensor (optional for controlled power)
const int led_pin = 6;       // LED to indicate flood alert

int val;

void setup() {
  pinMode(sensor_pin, INPUT);      // Soil sensor digital output
  pinMode(sensor_vcc, OUTPUT);    // Sensor power pin
  pinMode(led_pin, OUTPUT);       // LED pin
  digitalWrite(sensor_vcc, HIGH); // Turn on the sensor
  tap_servo.attach(tap_servo_pin);

  // Initialize the OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // I2C address 0x3C
    Serial.println("SSD1306 allocation failed");
    for (;;); // Don't proceed, loop forever
  }

  // Display startup message
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 25); // Position the text
  display.println("Safe Level"); // Display the initial status
  display.display();
  delay(2000); // Show startup message for 2 seconds
  display.clearDisplay(); // Clear the display
}

void loop() {
  val = digitalRead(sensor_pin); // Read the digital output (DO) pin of the sensor

  if (val == LOW) { // LOW means water detected
    tap_servo.write(90);         // Rotate servo to 90°
    digitalWrite(led_pin, HIGH); // Turn on LED
    displayStatus("Flood Alert");// Display flood alert message
  } else { // HIGH means no water detected
    tap_servo.write(0);          // Rotate servo back to 0°
    digitalWrite(led_pin, LOW);  // Turn off LED
    displayStatus("Safe Level"); // Display safe message
  }
  delay(500); // Short delay for stability
}

// Function to display status on OLED
void displayStatus(const char* message) {
  display.clearDisplay();          // Clear previous content
  display.setTextSize(2);          // Set text size
  display.setTextColor(WHITE);     // Set text color
  display.setCursor(10, 25);       // Position the text
  display.println(message);        // Display the message
  display.display();               // Update the OLED screen
}
