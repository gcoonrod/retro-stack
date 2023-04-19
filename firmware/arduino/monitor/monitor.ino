// Copyright 2023 Greg Coonrod
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "RetroStackDebug.h"

volatile uint8_t dataDirection = INPUT;

void setup() {
    // Set up the address pins
    setAddressDirection(INPUT);

    // Set up the data pins
    setDataDirection(dataDirection);

    // Set up the control pins
    setControlDirections(0xff, INPUT);

    // Set up the interrupt
    attachInterrupt(digitalPinToInterrupt(PHI2), onClockRise, RISING);

    // Set up the serial port
    Serial.begin(115200);
    Serial.println("Setup complete.");
}

void loop() {

}

void onClockRise() {
    sei();
    // Default data direction is input
    if (dataDirection == OUTPUT) {
        dataDirection = INPUT;
        setDataDirection(dataDirection);
    }

    // Read address and data
    uint8_t data;
    uint16_t address = readAddress();
    // If data is in ROM, set data direction to output
    // and write the data to the bus
    if (inROM(address)) {
        data = readROM(address);
        dataDirection = OUTPUT;
        setDataDirection(dataDirection);
        writeData(data);
    } else {
        data = readData();
    }


    char output[15];
    Serial.print("   ");
    sprintf(output, "   %04x  %c %02x", address, digitalRead(RWB) ? 'r' : 'W', data);
    Serial.println(output);
    cli();
}



