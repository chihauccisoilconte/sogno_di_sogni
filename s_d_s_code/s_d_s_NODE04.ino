/*
  Sogno_di_sogni_ == Node 04==

  by Chi ha ucciso Il Conte? (Nicolo Merendino - https://chihauccisoilconte.eu/)
  from code by Dejan, www.HowToMechatronics.com
  Libraries:
  nRF24/RF24, https://github.com/nRF24/RF24
  nRF24/RF24Network, https://github.com/nRF24/RF24Network
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include<Wire.h>
#define button 3
RF24 radio(7, 8);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 04;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node00 = 00;      // Address of the other node in Octal format
const int MPU = 0x68;
int16_t AcX, AcY, AcZ4, Tmp, GyX, GyY, GyZ;

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  pinMode(button, INPUT_PULLUP);
}
void loop() {
  network.update();
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 12, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ4 = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();

  //===== Sending =====//

  unsigned long acc4value = AcZ4;
  RF24NetworkHeader header(node00);    // (Address where the data is going)
  bool ok = network.write(header, &acc4value, sizeof(acc4value)); // Send the data


}
