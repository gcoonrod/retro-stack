<!--
 Copyright 2023 Greg Coonrod
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
     http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
-->

# RetroStack Firmware

The RetroStack computer includes a W65C02S processor, PLDs, and Arduinos. Each of these components need software or firmware in order to operate. This folder contains that software broken up by logical "programs" or use-cases.

## Development and Debugging Utilities

- [Monitor](./arduino/monitor/) - Arduino Mega2560 based bus monitor and ROM emulator.
- [PLD Tester](./arduino/pld-tester/) - Arduino Nano Every based tool for testing an AT16V8B PLD.

## Programmable Logic

- [Address Decode](./pld/address-decode/) - CUPL files for the various PLDs.

## 6502 Applications

- [Memory Tester](./6502/memory-test/) - Verify current RAM/ROM address map.
