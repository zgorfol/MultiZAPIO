#ifndef i2ckeypad_h
#define i2ckeypad_h

//#include <inttypes.h>

#define KEYPADD_ADDR (0x21 << 1)
#define KEYPAD_NO_PRESSED 0x00

class i2ckeypad {
public:
  i2ckeypad(uint8_t);
  i2ckeypad(uint8_t, uint8_t, uint8_t);
  char get_key();
  char getKeyP();
  void init();
  
private:
  HAL_StatusTypeDef pcf8574_write(uint8_t, uint8_t);
  uint8_t pcf8574_byte_read(uint8_t);
};

#endif

