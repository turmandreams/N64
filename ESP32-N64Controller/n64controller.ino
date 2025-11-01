
#define PIN 21

#define N64_HIGH pinMode(PIN,INPUT)
#define N64_LOW pinMode(PIN,OUTPUT)
#define N64_QUERY digitalRead(PIN)

int8_t dato=0;

void delay_500ns() {

  // You may need to adjust the number of NOPs based on your ESP32 and compiler.
  __asm__("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
          "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
          "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
          "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");
}


void delay_250ns() {

  // You may need to adjust the number of NOPs based on your ESP32 and compiler.
  __asm__("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
          "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");
}


// Send one byte of data to eeprom
void sendbyte(byte data) {
  
  pinMode(PIN,OUTPUT);
  
  for (byte i = 0; i < 8; i++) {
    // pull data line low    
     N64_LOW;

    // if current bit is 1, pull high after ~1us, 2 cycles
    if (data >> 7) {
      delayMicroseconds(1);      
      N64_HIGH;
      delayMicroseconds(3);            
    }
    // if current bit is 0 pull high after ~3us, 6 cycles
    else {
      delayMicroseconds(3);
      N64_HIGH;
      delayMicroseconds(1);      
    }

    // rotate to the next bit
    data <<= 1;
  }

  pinMode(PIN,INPUT);
  
}


void readStop() {    /// El stop bit que envia la consola dura 3 us
  //while(N64_QUERY){}
  while(!N64_QUERY){} 
}

// Send stop bit to eeprom
void sendStop() {    /// El stop bit que envia la consola dura 3 us
  N64_LOW;
  delayMicroseconds(1);  
  N64_HIGH;
  delayMicroseconds(3);  
}

// Capture 8 bytes in 64 bits into bit array tempBits
void readData() {
  
 
  dato=0;
       
  dato = dato | (N64_QUERY << 7);
  while (!N64_QUERY) { }
  
  //while(N64_QUERY){};  
  delayMicroseconds(2);
  dato = dato | (N64_QUERY << 6);
  while (!N64_QUERY) { }

  //while(N64_QUERY){};
  delay_500ns();
  delayMicroseconds(2);
  dato = dato | (N64_QUERY << 5);
  while (!N64_QUERY) { }

  //while(N64_QUERY){};
  delay_500ns();
  delayMicroseconds(2);
  dato = dato | (N64_QUERY << 4);
  while (!N64_QUERY) { }

  //while(N64_QUERY){};
  delay_500ns();
  delayMicroseconds(2);
  dato = dato | (N64_QUERY << 3);
  while (!N64_QUERY) { }

  //while(N64_QUERY){};
  delay_500ns();
  delayMicroseconds(2);
  dato = dato | (N64_QUERY << 2);
  while (!N64_QUERY) { }

  //while(N64_QUERY){};
  delay_500ns();
  delayMicroseconds(2);
  dato = dato | (N64_QUERY << 1);
  while (!N64_QUERY) { }

  //while(N64_QUERY){};
  delay_500ns();
  delayMicroseconds(2);
  dato = dato | (N64_QUERY << 0);
  while (!N64_QUERY) { }

  
  
}





void setup() {
  
  
  // S_DATA
  pinMode(PIN,INPUT);
  
  // Start Serial Monitor
  Serial.begin(115200);
  Serial.begin(1000000);
 // Serial.println("");
 // Serial.println("N64 EEPROM WRITER");
  
  
}

void loop(){
  
  sendbyte(0x01);//sendStop();

  delayMicroseconds(5);
  readData();byte dato0=dato;
  
  delayMicroseconds(2);
  readData();byte dato1=dato;

  delayMicroseconds(2);
  readData();int dato2=dato;dato2+=128;

  delayMicroseconds(2);
  readData();int dato3=dato;dato3+=128;

  readStop();

  if((dato0!=5)&&(dato0!=255)&&(dato1!=128)&&(dato1!=255)&&(dato2<255)&&(dato3<255)){
  
    Serial.print(dato0);Serial.print(",");
    Serial.print(dato1);Serial.print(",");
    Serial.print(dato2);Serial.print(",");
    Serial.println(dato3);

    delay(10);

  }  
  
  
}
