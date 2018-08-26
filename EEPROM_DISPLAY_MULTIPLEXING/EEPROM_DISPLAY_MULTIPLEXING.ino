#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

/*
Set the EEPROM's target address and OE status.
*/
void setAddress(uint16_t address, bool outputEnable) {
	shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0b00000000 : 0b10000000));
	shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

	digitalWrite(SHIFT_LATCH, LOW);
	digitalWrite(SHIFT_LATCH, HIGH);
	digitalWrite(SHIFT_LATCH, LOW);
}

/*
Write a byte to the EEPROM at the specified address.
*/
void writeEEPROM(int address, byte data) {
	setAddress(address, /*outputEnable*/ false);
	for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
		pinMode(pin, OUTPUT);
	}

	for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
		digitalWrite(pin, data & 1);
		data = data >> 1;
	}
	digitalWrite(WRITE_EN, LOW);
	delayMicroseconds(1);
	digitalWrite(WRITE_EN, HIGH);
	delay(10);
}

/*
Read a byte from the EEPROM at the specified address.
*/
byte readEEPROM(int address) {
	for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1)
		pinMode(pin, INPUT);

	setAddress(address, /*outputEnable*/ true);

	byte data = 0;
	for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1)
		data = (data << 1) + digitalRead(pin);

	return data;
}

/*
Print the contents of the EEPROM.
*/
void printContents(int count) {
	for (int base = 0; base < count; base += 16) {
		byte data[16];
		for (int offset = 0; offset < 16; offset += 1) {
			data[offset] = readEEPROM(base + offset);
		}

		char buf[80];
		sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
			base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
			data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

		Serial.println(buf);
	}
}

/*
Calculate a to the power of b.
*/
int pow(int a, int b) {
	int result = 1;
	for (int i = 0; i < b; i++)
		result *= a;
	return result;
}

void setup() {
	pinMode(SHIFT_DATA, OUTPUT);
	pinMode(SHIFT_CLK, OUTPUT);
	pinMode(SHIFT_LATCH, OUTPUT);
	pinMode(WRITE_EN, OUTPUT);
	digitalWrite(WRITE_EN, HIGH);
	Serial.begin(57600);

	byte digits[] = {
		0b01110111, // 0
		0b00010100, // 1
		0b10110011, // 2
		0b10110110, // 3
		0b11010100, // 4
		0b11100110, // 5
		0b11100111, // 6
		0b00110100, // 7
		0b11110111, // 8
		0b11110100, // 9
		0b11110101, // A
		0b11000111, // B
		0b10000011, // C
		0b10010111, // D
		0b11100011, // E
		0b11100001, // F
	};

	byte negitive =
		0b10000000; // -

	//int displayOffsets[] = {
		//0b00110000000000,
		//0b00100000000000,
		//0b00010000000000,
		//0b00000000000000
	//};

  // Reversed as hardware only works if the 'wrong' way around
  int displayOffsets[] = {
    0b00000000000000,
    0b00010000000000,
    0b00100000000000,
    0b00110000000000
  };

	int offset;

	// Hexadecimal
	Serial.println("Writing hexadecimal...");
  // Switched with Decimal offset as required by hardware
	offset = 0b01000000000000;

  // Display for all four digits to enable 16 bit output
  // For 8 bits we will simply clear the registers then load the 8 bit value into the least significant register
  // This will leave the most signifcant bits as 00 achieveing the same goal as the second nested for loop
	for (int i = 0; i < 4; i++)
		for (int value = 0; value < 256; value++)
			writeEEPROM(value + offset + displayOffsets[i], digits[(value / pow(16, i)) % 16]);

	//for (int i = 0; i < 2; i++)
		//for (int value = 0; value < 256; value++)
			//writeEEPROM(value + offset + displayOffsets[i+2], 0b00000000);

	// Decimal
	Serial.println("Writing decimal...");
  // Switched with Hexadecimal as required
	offset = 0b00000000000000;

	for (int i = 0; i < 3; i++)
		for (int value = 0; value < 256; value++)
			writeEEPROM(value + offset + displayOffsets[i], digits[(value / pow(10, i)) % 10]);

	for (int i = 3; i < 4; i++)
		for (int value = 0; value < 256; value++)
			writeEEPROM(value + offset + displayOffsets[i], 0b00000000);

	// Two's Compliment
	Serial.println("Writing two's compliment...");
	offset = 0b00001000000000;

	for (int i = 0; i < 3; i++)
		for (int value = -128; value < 128; value++)
			writeEEPROM((byte)value + offset + displayOffsets[i], digits[abs(value / pow(10, i)) % 10]);

	for (int i = 3; i < 4; i++)
		for (int value = -128; value < 128; value++)
			writeEEPROM((byte)value + offset + displayOffsets[i], value < 0 ? negitive : 0b00000000);

	Serial.println("Finished writing...");

	printContents(8192);
}

void loop() { }
