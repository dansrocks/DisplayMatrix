

#ifndef DISPLAY_MATRIX_CS
	#define DISPLAY_MATRIX_CS 10
#endif

#ifndef DISPLAY_MATRIX_CLK
	#define DISPLAY_MATRIX_CLK 11
#endif

#ifndef DISPLAY_MATRIX_DATAIN
	#define DISPLAY_MATRIX_DATAIN 12
#endif

#ifndef NUM_DISPLAYS
	#define NUM_DISPLAYS 1
#endif



class DisplayMatrix
{
	public:
		DisplayMatrix();
		void init(byte brightness);
		void write(byte address, unsigned char data);
		void nop();

	private:
		void send(byte data);
};

/*
 * Constructor
 */
DisplayMatrix::DisplayMatrix()
{
	// do nothing
}

/*
 * Initialization
 */
void DisplayMatrix::init(byte brightness=0x03)
{
	pinMode(DISPLAY_MATRIX_CS,    OUTPUT);
	pinMode(DISPLAY_MATRIX_CLK,   OUTPUT);
	pinMode(DISPLAY_MATRIX_DATAIN,OUTPUT);
	
	delay(100);
	
	write(0x09, 0x00);                  // disable BCD decoding
	write(0x0a, brightness & 0x0F);     // set brightness  (max value: 0x0F - 16)
	write(0x0b, 0x07);                  // scanlimit；8 LEDs
	write(0x0c, 0x01);                  // set normal mode
	write(0x0f, 0x00);                  // disable test display
}

/*
 * Initialization
 */
void DisplayMatrix::write(byte address, byte data)
{
        digitalWrite(DISPLAY_MATRIX_CS,LOW);
        send(address);           //address，code of LED
        send(data);               //data，figure on LED 
        digitalWrite(DISPLAY_MATRIX_CS, HIGH);
}

/*
 * Initialization
 */
void DisplayMatrix::send(byte data)
{
	digitalWrite(DISPLAY_MATRIX_CS, LOW);
	for(byte i=8;i>=1;i--) {
		digitalWrite(DISPLAY_MATRIX_CLK, LOW);
		digitalWrite(DISPLAY_MATRIX_DATAIN, data & 0x80);	// MSB as bit to send
		data = data<<1;
		digitalWrite(DISPLAY_MATRIX_CLK, HIGH);
	}
}

/*
 * Nop OP
 */
void DisplayMatrix::nop()
{
        write(0x00, 0x00);
}


 
unsigned char digitsAlphabet[10][8] = {
  {0x60, 0x90, 0x90, 0x90, 0x90, 0x90, 0x60, 0x01},  // 0
  {0x20, 0x60, 0xa0, 0x20, 0x20, 0x20, 0xf0, 0x02},  // 1
  {0x60, 0x90, 0x10, 0x10, 0x20, 0x40, 0xf0, 0x03},  // 2
  {0x60, 0x90, 0x10, 0x20, 0x10, 0x90, 0x60, 0x04},  // 3
  {0x80, 0x90, 0x90, 0xf0, 0x10, 0x10, 0x10, 0x05},  // 4
  {0xf0, 0x80, 0xe0, 0x10, 0x10, 0x10, 0xe0, 0x06},  // 5
  {0x30, 0x40, 0x80, 0xe0, 0x90, 0x90, 0x60, 0x07},  // 6
  {0xf0, 0x10, 0x10, 0x20, 0x40, 0x40, 0x40, 0x08},  // 7
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09},  // 8
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A}   // 9
};
 
 

DisplayMatrix dmatrix = DisplayMatrix();

void setup()
{
  dmatrix.init(0x01);
  for (unsigned char i=0; i<NUM_DISPLAYS * 8; i++) {
    dmatrix.write((i&0x7)+1, 0x00);
  }
  delay(5000);
}

 
unsigned char i;

 
void loop()
{
  for (i=0; i<=7; i++) {
    dmatrix.write(i+1, digitsAlphabet[k&0x07][i]);
  }
  k++;

  // i'm using a 4-chained displays, and it require 3 nop
  // to show same digit on all displays
  dmatrix.nop();
  dmatrix.nop();
  dmatrix.nop();
  
  // delay
  delay(5000);
}

