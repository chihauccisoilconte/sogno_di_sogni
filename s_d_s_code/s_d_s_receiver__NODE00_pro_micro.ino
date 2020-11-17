/*
  Sogno_di_sogni_NODE0 - == Base/ Master Node 00==

  by Chi ha ucciso Il Conte? (Nicolo Merendino - https://chihauccisoilconte.eu/)
  from code by Dejan, www.HowToMechatronics.com
  Libraries:
  nRF24/RF24, https://github.com/nRF24/RF24
  nRF24/RF24Network, https://github.com/nRF24/RF24Network
  MIDIUSB, https://github.com/arduino-libraries/MIDIUSB
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "MIDIUSB.h"

#define led 4
#define led2 5
#define led3 6
#define led4 8



RF24 radio(9, 10);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;  // Address of the other node in Octal format
const uint16_t node02 = 02;
const uint16_t node03 = 03;
const uint16_t node04 = 04;




void setup() {
  Serial.begin(115200);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {
  int mc1 = 0;
  int mc2  = 0;
  int mc3  = 0;
  int mc4  = 0;
  digitalWrite(led, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);



  network.update();
  //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    unsigned long incomingData;
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data

    // PWM output to LED 01 (dimming)
    if (header.from_node == 1) {
      if (incomingData > 2500) {
        mc1 = map(incomingData, 2500, 17000, 0, 127);

        digitalWrite(led, HIGH);
        controlChange(0, 0, mc1);
        MidiUSB.flush();

      } else {
        digitalWrite(led, LOW);
        mc1 = 0;
      }
      if (incomingData < -2500) {
        mc1 = map(incomingData, -2500, -17000, 0, 127);
        controlChange(0, 0, mc1);
        MidiUSB.flush();
        digitalWrite(led, HIGH);
      } else {
        digitalWrite(led, LOW);
        mc1 = 0;
      }
    }

    if (header.from_node == 2) {
      if (incomingData > 2500) {
        mc2 = map(incomingData, 2500, 17000, 0, 127);

        digitalWrite(led2, HIGH);
        controlChange(0, 1, mc2);
        MidiUSB.flush();
      } else {
        digitalWrite(led2, LOW);
        mc2 = 0;

      }
      if (incomingData < -2500) {
        mc2 = map(incomingData, -2500, -17000, 0, 127);
        digitalWrite(led2, HIGH);

        controlChange(0, 1, mc2);
        MidiUSB.flush();
      } else {
        digitalWrite(led2, LOW);
        mc2 = 0;

      }

    }
    if (header.from_node == 3) {
      if (incomingData > 2500) {
        mc3 = map(incomingData, 2500, 17000, 0, 127);
        digitalWrite(led3, HIGH);

        controlChange(0, 2, mc3);
        MidiUSB.flush();
      } else {
        digitalWrite(led3, LOW);
      }
      if (incomingData < -2500) {
        mc3 = map(incomingData, -2500, -17000, 0, 127);
        digitalWrite(led3, HIGH);

        controlChange(0, 2, mc3);
        MidiUSB.flush();
        digitalWrite(led3, HIGH);
      } else {
        digitalWrite(led3, LOW);
      }
    }

    if (header.from_node == 4) {
      if (incomingData > 2500) {
        mc4 = map(incomingData, 2500, 17000, 0, 127);
        controlChange(0, 3, mc4);
        MidiUSB.flush();
        digitalWrite(led4, HIGH);
      } else {
        digitalWrite(led4, LOW);
      }
      if (incomingData < -2500) {
        mc4 = map(incomingData, -2500, -17000, 0, 127);
        controlChange(0, 3, mc4);
        MidiUSB.flush();
        digitalWrite(led4, HIGH);
      } else {
        digitalWrite(led4, LOW);
      }
    }

  }
  delay (5);
}
//this machine kills fascists
