#include <  .h>   

#define SENSOR_PIN 2
#define DISPLAY_PIN 3
#define SPEAKER_PIN 4
#define LED_PIN 5

#define UPDATE_PERIOD 1000
#define MIN_FLOW 2.5
#define MAX_FLOW 20

#define LED_FREQ 35
#define SPEAKER_FREQ 880
#define ALARM_TIMEOUT 100
#define ALARM_REP 5

FlowMeter sensor = FlowMeter(SENSOR_PIN);

void sensorISR() {
  sensor.count();
}

void setup() {
  Serial.begin(115200);
  
  attachInterrupt(INT0, sensorISR, RISING);
  sensor.reset();

  pinMode(DISPLAY_PIN, OUTPUT); 
  pinMode(SPEAKER_PIN, OUTPUT); 
  pinMode(LED_PIN, OUTPUT); 

  alarmTest();
  
}

bool isAlarmOn = false;

void alarmTest() {
  isAlarmOn = true;
  displayValue(MAX_FLOW);
  handleAlarm();
  isAlarmOn = false;
  displayValue(0);
}

void handleAlarm() {
  if (isAlarmOn) {
    for(int i = 0; i < ALARM_REP; i++) {        
      tone(SPEAKER_PIN, SPEAKER_FREQ);
      digitalWrite(LED_PIN, HIGH);  
      
      delay(ALARM_TIMEOUT);
        
      noTone(SPEAKER_PIN);
      digitalWrite(LED_PIN, LOW);  
      
      delay(ALARM_TIMEOUT);
    }
  } else {
    noTone(SPEAKER_PIN);
    digitalWrite(LED_PIN, LOW);  
  }
}

void displayValue(double value) {
  int displayValue = map(value, 0, MAX_FLOW, 0, 255);
  analogWrite(DISPLAY_PIN, displayValue);
}

void loop() {
  sensor.tick(UPDATE_PERIOD);
  delay(UPDATE_PERIOD);
  sensor.tick(UPDATE_PERIOD);

  Serial.println("Flow: " + String(sensor.getCurrentFlowrate()) + " l/min, " + String(sensor.getTotalVolume())+ " l total.");

  displayValue(sensor.getCurrentFlowrate());

  isAlarmOn = sensor.getCurrentFlowrate() < MIN_FLOW;
  //handleAlarm();
}
