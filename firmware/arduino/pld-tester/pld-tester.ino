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

#define NUM_TESTS 4

// Define Test Case Struct
struct TestCase
{
    uint16_t address;
    uint8_t rwb;
    uint8_t clk;
    uint8_t expected;
};

// Define Test Cases
const TestCase testCases[NUM_TESTS] = {
    // address, rwb, clk, expected
    {0x0000, HIGH, HIGH, 0b00011100},
    // Read from 0xFFFC
    {0xFFFC, HIGH, LOW, 0b00000111},
    // Read from 0x8000
    {0x8000, HIGH, LOW, 0b00000111},
    // Write to 0x0100
    {0x0100, LOW, HIGH, 0b00011010},
};

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
    Serial.println("PLD Tester");
    Serial.println("Press 'n' to run next test");
}

void loop()
{
    if (Serial.available() > 0)
    {
        int incomingByte = Serial.read();
        if (incomingByte == 'n')
        {

            Serial.print(F("Test "));
            Serial.print(currentTest + 1);
            Serial.print(F(": "));

            // Run Test
            runTest(&testCases[currentTest]);

            // Read Result
            uint8_t result = readResult();
            if (result == testCases[currentTest].expected)
            {
                Serial.println(F("\tPASS"));
                
            }
            else
            {
                Serial.println(F("\tFAIL"));
                // Print Result
                Serial.print(F("\tAddress: 0x"));
                Serial.print(testCases[currentTest].address, HEX);
                Serial.print(F(", RWB: "));
                Serial.print(testCases[currentTest].rwb, DEC);
                Serial.print(F(", CLK: "));
                Serial.print(testCases[currentTest].clk, DEC);
                Serial.print(F(", Result: 0b"));
                Serial.print(result, BIN);
                Serial.print(F(", Expected: 0b"));
                Serial.println(testCases[currentTest].expected, BIN);

                // Print Result in Human Readable Format
                Serial.print(F("\tRAM_CS: "));
                Serial.print((result & RAM_CSB_MASK) ? "HIGH" : "LOW");
                Serial.print(F(", RAM_OE: "));
                Serial.print((result & RAM_OEB_MASK) ? "HIGH" : "LOW");
                Serial.print(F(", RAM_WE: "));
                Serial.print((result & RAM_WEB_MASK) ? "HIGH" : "LOW");
                Serial.print(F(", ROM_CS: "));
                Serial.print((result & ROM_CSB_MASK) ? "HIGH" : "LOW");
                Serial.print(F(", ROM_OE: "));
                Serial.println((result & ROM_OEB_MASK) ? "HIGH" : "LOW");
            }

            // Increment Test
            currentTest++;
            if (currentTest >= NUM_TESTS)
            {
                currentTest = 0;
            }
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
