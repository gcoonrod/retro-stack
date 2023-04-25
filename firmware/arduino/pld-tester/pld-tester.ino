// Define Outputs
#define RWB 2
#define CLK 3
#define A9 4
#define A10 5
#define A11 6
#define A12 7
#define A13 8
#define A14 9
#define A15 10

// Define Inputs
#define RAM_CSB A0
#define RAM_OEB A1
#define RAM_WEB A2
#define ROM_CSB A3
#define ROM_OEB A4

// Define Address Bit Masks
#define A9_MASK 0x0200
#define A10_MASK 0x0400
#define A11_MASK 0x0800
#define A12_MASK 0x1000
#define A13_MASK 0x2000
#define A14_MASK 0x4000
#define A15_MASK 0x8000

// Define Test Result Bit Masks
#define RAM_CSB_MASK 0x01
#define RAM_OEB_MASK 0x02
#define RAM_WEB_MASK 0x04
#define ROM_CSB_MASK 0x08
#define ROM_OEB_MASK 0x10

// Define Test Case Struct
struct TestCase
{
    uint16_t address;
    uint8_t rwb;
    uint8_t clk;
    uint8_t expected;
};

// Define Test Cases
// address, rwb, clk, expected
#define NUM_TESTS 5
const TestCase testCases[NUM_TESTS] = {
    // Read from 0x0000
    {0x0000, HIGH, HIGH, 0b00011100},
    // Read from 0xFFFC
    {0xFFFC, HIGH, LOW, 0b00000111},
    // Read from 0x8000
    {0x8000, HIGH, LOW, 0b00000111},
    // Write to 0x0100
    {0x0100, LOW, HIGH, 0b00011010},
    // Read from IO
    {0x4006, HIGH, LOW, 0b00011111},
};

// Test result array initialized to zeros
uint8_t testResults[NUM_TESTS] = {0};

volatile int currentTest = 0;

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

    // Set Inputs
    pinMode(RAM_CSB, INPUT_PULLUP);
    pinMode(RAM_OEB, INPUT_PULLUP);
    pinMode(RAM_WEB, INPUT_PULLUP);
    pinMode(ROM_CSB, INPUT_PULLUP);
    pinMode(ROM_OEB, INPUT_PULLUP);

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

    // Setup Serial at 115200 baud
    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }
    Serial.println(F("PLD Tester"));
    Serial.println(F("Press 'n' to run test suite"));
}

void loop()
{
    if (Serial.available())
    {
        char c = Serial.read();
        if (c == 'n')
        {
           runAllTests();
        }
    }
}

uint8_t readResult()
{
    // Shift inputs into result
    uint8_t result = 0;
    result |= digitalRead(ROM_OEB) << 4;
    result |= digitalRead(ROM_CSB) << 3;
    result |= digitalRead(RAM_WEB) << 2;
    result |= digitalRead(RAM_OEB) << 1;
    result |= digitalRead(RAM_CSB);

    return result;
}

void storeResult(uint8_t result)
{
    testResults[currentTest] = result;
}

void runTest(const TestCase *test)
{
    // Set Address
    digitalWrite(A9, (test->address & A9_MASK) ? HIGH : LOW);
    digitalWrite(A10, (test->address & A10_MASK) ? HIGH : LOW);
    digitalWrite(A11, (test->address & A11_MASK) ? HIGH : LOW);
    digitalWrite(A12, (test->address & A12_MASK) ? HIGH : LOW);
    digitalWrite(A13, (test->address & A13_MASK) ? HIGH : LOW);
    digitalWrite(A14, (test->address & A14_MASK) ? HIGH : LOW);
    digitalWrite(A15, (test->address & A15_MASK) ? HIGH : LOW);

    // Set RWB
    digitalWrite(RWB, test->rwb);

    // Set CLK
    digitalWrite(CLK, test->clk);
}

void runAllTests()
{
    int passCount = 0;
    for (int i = 0; i < NUM_TESTS; i++)
    {
        runTest(&testCases[i]);
        uint8_t result = readResult();
        if(result == testCases[i].expected) {
            passCount++;
        }
        storeResult(result);
    }

    if(passCount == NUM_TESTS) {
        Serial.println(F("All Tests Passed!"));
    } else {
        Serial.print(F("Failed "));
        Serial.print(NUM_TESTS - passCount);
        Serial.print(F(" out of "));
        Serial.print(NUM_TESTS);
        Serial.println(F(" tests"));

        // Print failed test results
        for (int i = 0; i < NUM_TESTS; i++)
        {
            if(testResults[i] != testCases[i].expected) {
                Serial.print(F("\tTest "));
                Serial.print(i + 1);
                Serial.print(F(": "));
                Serial.print(F("\tAddress: 0x"));
                Serial.print(testCases[i].address, HEX);
                Serial.print(F(", RWB: "));
                Serial.print(testCases[i].rwb, DEC);
                Serial.print(F(", CLK: "));
                Serial.print(testCases[i].clk, DEC);
                Serial.print(F(", Result: 0b"));
                Serial.print(testResults[i], BIN);
                Serial.print(F(", Expected: 0b"));
                Serial.println(testCases[i].expected, BIN);

                // Print Result in Human Readable Format
                Serial.print(F("\tRAM_CS: "));
                Serial.print((testResults[i] & RAM_CSB_MASK) ? "HIGH" : "LOW");
                Serial.print(F(", RAM_OE: "));
                Serial.print((testResults[i] & RAM_OEB_MASK) ? "HIGH" : "LOW");
                Serial.print(F(", RAM_WE: "));
                Serial.print((testResults[i] & RAM_WEB_MASK) ? "HIGH" : "LOW");
                Serial.print(F(", ROM_CS: "));
                Serial.print((testResults[i] & ROM_CSB_MASK) ? "HIGH" : "LOW");
                Serial.print(F(", ROM_OE: "));
                Serial.println((testResults[i] & ROM_OEB_MASK) ? "HIGH" : "LOW");
            }
        }
    }
}
