// Define Outputs
#define RWB 2
#define CLK 3
#define A9  4
#define A10 5
#define A11 6
#define A12 7
#define A13 8
#define A14 9
#define A15 10
#define RS0 15
#define RS1 16
#define RS2 17
#define OEB 20

// Define Inputs
#define RESULT 14

#define NUM_TESTS 4

// Define Test Case Struct
struct TestCase {
    uint16_t address;
    bool rwb;
    bool clk;
    bool oeb;
    uint8_t result;
};

// Define Test Cases
const TestCase testCases[NUM_TESTS] = {
    // address, rwb, clk, oeb, result
    {0x0000, false, false, false, 0x00},
    // Read from 0xFFFC
    {0xFFFC, true, false, false, 0x00},
    // Read from 0x8000
    {0x8000, true, false, false, 0x00},
    // Write to 0x0100
    {0x0100, false, false, false, 0x00},
};

void setup()
{
	// Set Outputs
    pinMode(RWB, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(A9, OUTPUT);
    pinMode(A10, OUTPUT);
    pinMode(A11, OUTPUT);
    pinMode(A12, OUTPUT);
    pinMode(A13, OUTPUT);
    pinMode(A14, OUTPUT);
    pinMode(A15, OUTPUT);
    pinMode(RS0, OUTPUT);
    pinMode(RS1, OUTPUT);
    pinMode(RS2, OUTPUT);
    pinMode(OEB, OUTPUT);
    
    // Set Inputs
    pinMode(RESULT, INPUT);
    
    // Set Outputs to LOW
    digitalWrite(RWB, LOW);
    digitalWrite(CLK, LOW);
    digitalWrite(A9, LOW);
    digitalWrite(A10, LOW);
    digitalWrite(A11, LOW);
    digitalWrite(A12, LOW);
    digitalWrite(A13, LOW);
    digitalWrite(A14, LOW);
    digitalWrite(A15, LOW);
    digitalWrite(RS0, LOW);
    digitalWrite(RS1, LOW);
    digitalWrite(RS2, LOW);
    digitalWrite(OEB, LOW);

    // Setup Serial at 115200 baud
    Serial.begin(115200);
    Serial.println("PLD Tester");

    // Run Test Cases
    for (int i = 0; i < NUM_TESTS; i++) {
        // Print test case with test number
        Serial.print(F("Test "));
        Serial.print(i + 1);
        Serial.println(F(": "));



        // Update State
        updateState(testCases[i].address, testCases[i].rwb, testCases[i].clk, testCases[i].oeb);
        
        // Read Result
        uint8_t result = readResult();
        
        // Print Result
        Serial.print(F("\tAddress: 0x"));
        Serial.print(testCases[i].address, HEX);
        Serial.print(F(", RWB: "));
        Serial.print(testCases[i].rwb);
        Serial.print(F(", CLK: "));
        Serial.print(testCases[i].clk);
        Serial.print(F(", OEB: "));
        Serial.print(testCases[i].oeb);
        Serial.print(F(", Result: 0b"));
        Serial.print(result, BIN);
        Serial.print(F(", Expected: 0b"));
        Serial.println(testCases[i].result, BIN);
        decodeResult(result);
    }
}

void loop()
{
	
}

uint8_t readResult() {
    // Read result from 74HC151, update RS0, RS1, RS2 between each bit
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        // Set RS0, RS1, RS2
        digitalWrite(RS0, i & 0x01);
        digitalWrite(RS1, i & 0x02);
        digitalWrite(RS2, i & 0x04);
        
        // Read result
        result |= (digitalRead(RESULT) << i);
    }
    return result;
}

void updateState(uint16_t address, bool rwb, bool clk, bool oeb) {
    // Set RWB, CLK, OEB
    digitalWrite(RWB, rwb);
    digitalWrite(CLK, clk);
    digitalWrite(OEB, oeb);
    
    // Set A9, A10, A11, A12, A13, A14, A15 to cover a 16-bit address
    digitalWrite(A15, (address & 0x8000) ? HIGH : LOW);
    digitalWrite(A14, (address & 0x4000) ? HIGH : LOW);
    digitalWrite(A13, (address & 0x2000) ? HIGH : LOW);
    digitalWrite(A12, (address & 0x1000) ? HIGH : LOW);
    digitalWrite(A11, (address & 0x0800) ? HIGH : LOW);
    digitalWrite(A10, (address & 0x0400) ? HIGH : LOW);
    digitalWrite(A9, (address & 0x0200) ? HIGH : LOW);
}

void decodeResult(uint8_t result) {
    // Decode result from 74HC151
    // Print result
    Serial.print(F("\tRAM_CS: "));
    Serial.print((result & 0x01) ? "HIGH" : "LOW");
    Serial.print(F(", RAM_OE: "));
    Serial.print((result & 0x02) ? "HIGH" : "LOW");
    Serial.print(F(", RAM_WE: "));
    Serial.print((result & 0x04) ? "HIGH" : "LOW");
    Serial.print(F(", ROM_CS: "));
    Serial.print((result & 0x08) ? "HIGH" : "LOW");
    Serial.print(F(", ROM_OE: "));
    Serial.println((result & 0x10) ? "HIGH" : "LOW");

}
