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

#include <stdint.h>

#ifndef RETROSTACKDEBUG_H
#define RETROSTACKDEBUG_H

// Define the memory ranges
#define RAM_START 0x0000
#define RAM_END   0x3FFF
#define RAM_SIZE  (RAM_END - RAM_START + 1)
#define inRAM(address) (address >= RAM_START && address <= RAM_END)

#define ROM_START 0x8000
#define ROM_END   0xFFFF
#define ROM_SIZE  (ROM_END - ROM_START + 1)
#define inROM(address) (address >= ROM_START && address <= ROM_END)

// Define the address and data pins

// reverse the order of the address pins
const char ADDR2[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char ADDR[] = {52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22};

const char DATA2[] = {37, 35, 33, 31, 29, 27, 25, 23};
const char DATA[] = {23, 25, 27, 29, 31, 33, 35, 37};


// Define the control pins
#define BE      39
#define RDY     41
#define SYNC    43
#define NMIB    45
#define IRQB    47
#define RWB     49
#define PHI2    2
#define RESB    53

// Op Codes
#define NOP 0xEA

// Define functions
uint8_t readROM(uint16_t address);
uint8_t readRAM(uint16_t address);
uint16_t readAddress();
void setDataDirection(int direction);
void setAddressDirection(int direction);
void setControlDirections(uint8_t mask, uint8_t direction);
uint8_t readData();
void writeData(uint8_t data);


#endif
