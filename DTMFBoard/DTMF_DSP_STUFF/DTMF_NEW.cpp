#include <Time.h>  
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) //ClearBit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))  //SetBit

#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))
#define _BV(bit)(1<<(bit))

//port B pins
#define DTMF_BIT0 0
#define DTMF_BIT1 1
#define DTMF_BIT2 2
#define DTMF_BIT3 3

//port D pins
#define CPOL_ENABLE   2
#define VPOL_ENABLE   3
#define INHIBIT_NHRC2 4  
#define DISABLE_RELAY 5
#define NHRC2_STATUS  6
#define NHRC2_INHIBIT_DELAY 5
#define DELAY 10
#define TIMEOUT 30



enum{
  IDLE,
  DTMF_DETECTED,
  MSG_DECODE,
  CPOL_ON,
  VPOL_ON,
} STATE;

volatile unsigned long timeoutCounter = 0;
volatile byte portBStatus;
volatile byte prevPortBStatus;
volatile byte dtmfBits;
boolean timeoutFlag = false;
boolean dtmfFlag = false;
int currDtmfIndex = 0;
char dtmf[5];
char cpolArray[5] = {'1','2','3','4','5'};
char vpolArray[5]  = {'5','4','3','2','1'};

void setup() {
  
  setupTimer();
  //declare i/o pins
  DDRD   |= DDRD | 0b00111100;
  DDRB   |= DDRB | 0b11110000; 
  PCICR  |= (1 << PCIE0);    // PCICR  |= 0b00000001;  
  PCMSK0 |= (1 << PCINT0 | 1 << PCINT1 | 1 << PCINT2 | 1 << PCINT3);  // PCMSK2 |= 0b01111000; //ENABLE PCINT 2 pins
  SET(PORTD,VPOL_ENABLE);
  CLR(PORTD,CPOL_ENABLE);    
  CLR(PORTD,INHIBIT_NHRC2);    //set inhibit line to low PD2 = gpio pin 4 --> digitalWrite(INHIBIT_NHRC2, LOW);
  CLR(PORTD,DISABLE_RELAY);    //set disable line to low PD3 = gpio pin 4 --> digitalWrite(DISABLE_RELAY, LOW);
  Serial.begin(9600); 
}

void loop() {
      dtmfStates(dtmfBits);
}

ISR (PCINT0_vect){
    prevPortBStatus = portBStatus;
    portBStatus = PINB;
    if(portBStatus != 0x00 && portBStatus != prevPortBStatus){
        dtmfFlag = true;
    }
}

ISR (TIMER1_COMPA_vect){
        timeoutCounter++;
}

void dtmfStates(byte newDTMFBytes){
    switch(STATE){
        case IDLE:         
            if(dtmfFlag){
                STATE = DTMF_DETECTED;
                break;
            }
        case DTMF_DETECTED:
            while(currDtmfIndex < 5){
                if(dtmfFlag){
                    timeoutCounter = 0;
                    dtmfBits = portBStatus;
                    dtmf[currDtmfIndex] = mapDTMFTone(dtmfBits);
                    Serial.print("DTMF[");Serial.print(currDtmfIndex);Serial.print("] = ");Serial.println(dtmf[currDtmfIndex]);
                    currDtmfIndex++;
                    portBStatus = 0b00000000;
                    dtmfFlag = false;
                }
               dtmfFlag = false; 
               if(timeoutCounter >= 30 && currDtmfIndex > 0){
                   Serial.println("TIMEOUT");
                   for(int i = 0; i < 5; i++){
                       dtmf[i] = 0b00000000;
                       Serial.println(dtmf[i]);
                   } 
                   currDtmfIndex = 0;
                   timeoutCounter = 0;
                   STATE = IDLE;
                   break; 
               }
               else if(currDtmfIndex == 5){
                   for(int i = 0; i < 5; i++){
                       Serial.println(dtmf[i]);
                   } 
                   currDtmfIndex = 0;
                   STATE = MSG_DECODE;
                   break;
               }            
            } 
        case MSG_DECODE:
              if(dtmfArrayDecode(dtmf, cpolArray, 5)){
                Serial.println("CPOL_EN!");
                STATE = CPOL_ON;
                break;
            }
            else if(dtmfArrayDecode(dtmf, vpolArray, 5)){
              Serial.println("VPOL_EN!");
                STATE = VPOL_ON;
                break;
            }
            else{
                STATE = IDLE;
                break;
            }
            
        case CPOL_ON:
            if(PIND & 0b00000100){
                Serial.println("back to idle");
                STATE = IDLE;
                break;
            }
            else{
                timeoutCounter = 0;
                SET(PORTD, INHIBIT_NHRC2); //set inhibit line to low PD2 = gpio pin 4 --> digitalWrite(INHIBIT_NHRC2, LOW);
                while(timeoutCounter < NHRC2_INHIBIT_DELAY){
                    Serial.println(timeoutCounter);
                }
                Serial.println("inhibited");
                timeoutCounter = 0;
                CLR(PORTD, VPOL_ENABLE);
                SET(PORTD, DISABLE_RELAY); //set disable line to low PD3 = gpio pin 4 --> digitalWrite(DISABLE_RELAY, LOW);
                while(timeoutCounter < DELAY){
                    Serial.println(timeoutCounter);
                }
                SET(PORTD, CPOL_ENABLE);
                Serial.println("CPOL ENABLED!");
                STATE = IDLE;               
                break;
            } 
        case VPOL_ON:
            if(PIND & 0b00001000){
                Serial.println("back to idle");
                STATE = IDLE;
                break;
            }
            else{
                timeoutCounter = 0;
                SET(PORTD, INHIBIT_NHRC2); //set inhibit line to low PD2 = gpio pin 4 --> digitalWrite(INHIBIT_NHRC2, LOW);
                while(timeoutCounter < NHRC2_INHIBIT_DELAY){
                    Serial.println(timeoutCounter);
                }
                Serial.println("inhibited");
                timeoutCounter = 0;
                CLR(PORTD, CPOL_ENABLE);
                SET(PORTD, DISABLE_RELAY); //set disable line to low PD3 = gpio pin 4 --> digitalWrite(DISABLE_RELAY, LOW);
                while(timeoutCounter < DELAY){
                    Serial.println(timeoutCounter);
                }
                SET(PORTD, VPOL_ENABLE);
                Serial.println("VPOL ENABLED!");
                STATE = IDLE;               
                break;
            }
        
  
            
    }
}

char mapDTMFTone(byte data) {
        //change data to int
	char key;
	// Convert the received key to understandable data for us
	if (data == 1 ) key = '1';
	if (data == 2 ) key = '2';
	if (data == 3 ) key = '3';
	if (data == 4 ) key = '4';
	if (data == 5 ) key = '5';
	if (data == 6 ) key = '6';
	if (data == 7 ) key = '7';
	if (data == 8 ) key = '8';
	if (data == 9 ) key = '9';
	if (data == 10) key = '0';
	if (data == 11) key = '*';
	if (data == 12) key = '#';
	if (data == 13) key = 'A';
	if (data == 14) key = 'B';
	if (data == 15) key = 'C';
	if (data == 0 ) key = 'D';
	return key;
}

boolean dtmfArrayDecode(char *a, char *b, int length){
      int n;
      for (n=0;n<length;n++){
          if (a[n]!=b[n]) return false;
      }
      return true;
}

void setupTimer() //sets up a timed ISR every 1/4 second
{
    cli(); //disable interrupts
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 =  0;
    OCR1A = 1562; // ISR @ 10Hz
    TCCR1B |= (1 << WGM12);    //turn on CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10);     //set CS10 and CS12 bits for 1024 prescalar
    TIMSK1 |= (1 << OCIE1A);     //enable timer compareinterrupt
    sei(); //allow interrupts
}

