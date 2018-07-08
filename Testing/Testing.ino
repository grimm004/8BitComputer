#define CLK     2
#define CLK_SPD 13
#define PIN_1   10
#define PIN_2   9
#define PIN_4   8
#define PIN_8   7
#define PIN_16  6
#define PIN_32  5
#define PIN_64  4
#define PIN_128 3

uint16_t current = 0x00;
bool clk = false;

void setup() {
	pinMode(CLK, OUTPUT);
	pinMode(CLK_SPD, INPUT);

	pinMode(PIN_1, OUTPUT);
	pinMode(PIN_2, OUTPUT);
	pinMode(PIN_4, OUTPUT);
	pinMode(PIN_8, OUTPUT);
	pinMode(PIN_16, OUTPUT);
	pinMode(PIN_32, OUTPUT);
	pinMode(PIN_64, OUTPUT);
	pinMode(PIN_128, OUTPUT);

	digitalWrite(PIN_1, LOW);
	digitalWrite(PIN_2, LOW);
	digitalWrite(PIN_4, LOW);
	digitalWrite(PIN_8, LOW);
	digitalWrite(PIN_16, LOW);
	digitalWrite(PIN_32, LOW);
	digitalWrite(PIN_64, LOW);
	digitalWrite(PIN_128, LOW);
}

void loop() {
	digitalWrite(CLK, (clk ^= 0x1));

	digitalWrite(PIN_1, (current >> 0) & 0x01);
	digitalWrite(PIN_2, (current >> 1) & 0x01);
	digitalWrite(PIN_4, (current >> 2) & 0x01);
	digitalWrite(PIN_8, (current >> 3) & 0x01);

	digitalWrite(PIN_16, (current >> 4) & 0x01);
	digitalWrite(PIN_32, (current >> 5) & 0x01);
	digitalWrite(PIN_64, (current >> 6) & 0x01);
	digitalWrite(PIN_128, (current >> 7) & 0x01);

	if (++current > 0xFF) current = 0x0;
	delay(digitalRead(CLK_SPD) ? 500 : 100);
}
