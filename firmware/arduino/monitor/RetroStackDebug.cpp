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
#include <Arduino.h>

uint16_t readAddress() {
    uint16_t address = 0;
    for (int n = 0; n < 16; n += 1) {
        int bit = digitalRead(ADDR[n]) ? 1 : 0;
        address = (address << 1) + bit;
    }
    return address;
}

uint8_t readData() {
    uint8_t data = 0;
    for (int n = 0; n < 8; n += 1) {
        int bit = digitalRead(DATA[n]) ? 1 : 0;
        data = (data << 1) + bit;
    }
    return data;
}

void writeData(uint8_t data) {
    for (int n = 0; n < 8; n += 1) {
        int bit = (data >> (7 - n)) & 1;
        digitalWrite(DATA[n], bit);
    }
}

void setDataDirection(int direction) {
    for (int n = 0; n < 8; n += 1) {
        pinMode(DATA[n], direction);
    }
}

void setAddressDirection(int direction) {
    for (int n = 0; n < 16; n += 1) {
        pinMode(ADDR[n], direction);
    }
}

void setControlDirections(uint8_t mask, uint8_t direction) {
    if (mask & 0x01) {
        pinMode(BE, direction);
    }
    if (mask & 0x02) {
        pinMode(RDY, direction);
    }
    if (mask & 0x04) {
        pinMode(SYNC, direction);
    }
    if (mask & 0x08) {
        pinMode(NMIB, direction);
    }
    if (mask & 0x10) {
        pinMode(IRQB, direction);
    }
    if (mask & 0x20) {
        pinMode(RWB, direction);
    }
    if (mask & 0x40) {
        pinMode(PHI2, direction);
    }
    if (mask & 0x80) {
        pinMode(RESB, direction);
    }
}

uint8_t readROM(uint16_t address) {
    switch(address) {
        case 0x8000:
        case 0x8001:
        case 0x8002:
        case 0x8003:
        case 0x8004:
        case 0x8005:
        case 0x8006:
        case 0x8007:
        case 0x8008:
        case 0x8009:
        case 0x800A:
        case 0x800B:
        case 0x800C:
            return NOP;
        case 0x800D:
            return 0x4c;
        case 0x800E:
            return 0x00;
        case 0x800F:
            return 0x80;
        case 0xFFFC:
            return 0x00;
        case 0xFFFD:
            return 0x80;
        default:
            return NOP;
    }
}