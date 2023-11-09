const byte PHOTORESISTOR_PIN = A0;  // photoresistor analog pin A0

const byte Switch1 = 2;  //HERO board pins 0-2 are switch INPUTs
const byte Switch2 = 3;
const byte Switch3 = 4;

const byte CABIN_LIGHT_PIN = 5;    // Digital pin for red LED
const byte STORAGE_LIGHT_PIN = 6;  // Digital pin for blue LED
const byte COCKPIT_LIGHT_PIN = 7;  // Digital pin for green LED
const byte HIGH_LIGHT_PIN = 8;     // Digital pin for white LED

const byte RED_PIN = 11;    // PWM pin for three-color red LED
const byte GREEN_PIN = 10;  // PWM pin for three-color green LED
const byte BLUE_PIN = 9;    // PWM pin for three-color blue LED

const byte OFF = 0;
const byte DIM = 64;
const byte BRIGHTER = DIM + 64;
const byte BRIGHT = BRIGHTER + 64;
const byte BRIGHTEST = 255;

const unsigned long BATTERY_CAPACITY = 50000;  // Maximum battery capacity
const byte BATTERY_DRAIN = 75; // amount the battery drains on each iteration
const float MINIMUM_BATTERY_PERCENTAGE = 25.0;

void setup() {
  // set each input/output pins
  pinMode(PHOTORESISTOR_PIN, INPUT);
  pinMode(Switch1, INPUT);
  pinMode(Switch2, INPUT);
  pinMode(Switch3, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(CABIN_LIGHT_PIN, OUTPUT);
  pinMode(STORAGE_LIGHT_PIN, OUTPUT);
  pinMode(COCKPIT_LIGHT_PIN, OUTPUT);
  pinMode(HIGH_LIGHT_PIN, OUTPUT);

  // start the serial monitor
  Serial.begin(9600);
}

void loop() {
  // charge the battery and display info in the serial monitor

  // current battery charge level. Start with 0% battery
  // Static maintains value from across multiple loop iterations
  static unsigned long battery_level = 0;

  // if the battery is not fully charged
  if (battery_level < BATTERY_CAPACITY) {
    // charge the battery
    battery_level += analogRead(PHOTORESISTOR_PIN);

    // don't allow the battery to overcharge
    if (battery_level > BATTERY_CAPACITY) {
      battery_level = BATTERY_CAPACITY;
    }
  }

  // calculate battery charge percentage cast to float so we get the correct precision
  float percentage = ((float)battery_level / (float)BATTERY_CAPACITY) * 100;

  if (percentage == 100.00) {
    // white
    displayColor(BRIGHTER, BRIGHTER, BRIGHTER);
  } else if (percentage >= 50.0) {
    //green
    displayColor(OFF, BRIGHTER, OFF);
  } else if (percentage >= MINIMUM_BATTERY_PERCENTAGE && percentage < 50.0) {
    //yellow
    displayColor(BRIGHTER, DIM, OFF);
  } else {
    //red pulsing
    displayColor(DIM, OFF, OFF);
    delay(40);
    displayColor(BRIGHTER, OFF, OFF);
  }

  // must have at least minimum battery level battery to use the lights
  if (percentage >= MINIMUM_BATTERY_PERCENTAGE) {
    if (digitalRead(Switch1) == HIGH) {     // check switch1
      digitalWrite(CABIN_LIGHT_PIN, HIGH);  // turn LED1 on
      battery_level -= BATTERY_DRAIN;
    } else {
      digitalWrite(CABIN_LIGHT_PIN, LOW);  // turn LED1 off
    }

    if (digitalRead(Switch2) == HIGH) {       // check switch2
      digitalWrite(STORAGE_LIGHT_PIN, HIGH);  // turn LED2 on
      battery_level -= BATTERY_DRAIN;
    } else {
      digitalWrite(STORAGE_LIGHT_PIN, LOW);  // turn LED2 off
    }

    if (digitalRead(Switch3) == HIGH) {       // check switch3
      digitalWrite(COCKPIT_LIGHT_PIN, HIGH);  // turn LED3 on
      battery_level -= BATTERY_DRAIN;
    } else {
      digitalWrite(COCKPIT_LIGHT_PIN, LOW);  // turn LED3 off
    }

    // check if all three switches are on
    if (digitalRead(Switch1) == HIGH
        && digitalRead(Switch2) == HIGH
        && digitalRead(Switch3) == HIGH) {
      digitalWrite(HIGH_LIGHT_PIN, HIGH);  // turn LED4 on
      battery_level -= BATTERY_DRAIN;
    } else {
      digitalWrite(HIGH_LIGHT_PIN, LOW);  // turn LED4 off
    }
  } else {
    // not enough battery power to use the lights
    digitalWrite(CABIN_LIGHT_PIN, LOW);    // turn LED1 off
    digitalWrite(STORAGE_LIGHT_PIN, LOW);  // turn LED2 off
    digitalWrite(COCKPIT_LIGHT_PIN, LOW);  // turn LED3 off
    digitalWrite(HIGH_LIGHT_PIN, LOW);     // turn LED4 off
  }

  Serial.print(percentage);
  Serial.println("%");
  delay(200);
}

/** 
 displayColor() is a function that accepts three parameters representing the desired intensity for each of the LEDs in the RGB LED.
 @param red_intensity red LED intensity (0-255)
 @param green_intensity green LED intensity (0-255)
 @param blue_intensity blue LED intensity (0-255) 
*/
void displayColor(byte red_intensity, byte green_intensity, byte blue_intensity) {
  analogWrite(RED_PIN, red_intensity);
  analogWrite(GREEN_PIN, green_intensity);
  analogWrite(BLUE_PIN, blue_intensity);
}