
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
/*
The purpose of this software is to be able to switch between multiple antennas connected to the Amateur radio
repeater at UCSC.  One antenna is vertically polarized, while the other is circularly polarized, and as a result 
they each have different radiation emmision patterns, resulting in different signal strengths, depending on where
you are. Switching from one antenna to the other is achieved by means of wireless DTMF control from a handheld 
transceiver in the field. The software is set up to require a 5-digit DTMF code to enable antenna switching.
Default codes are (5,6,7,8,9) for enabling the circularly polarized antenna, and (9,8,7,6,5) for enabling the 
vertically polarized antenna.

Upon successfully receiving a code to enable either of the antennas, the software will check to see if that 
particular antenna is already enabled, and if it is, the software simply returns to an idle state.  Otherwise, 
before switching antennas, the software will first send a signal to disable the NHRC2 repeater controller, followed
by disabling the currently enabled antenna.  A feedback signal ( (X)POL_STATUS)  is checke to confirm that the 
antenna was correctly switched off.  One the status bit is confirmed, an enable signal is sent to the desired 
antenna, followed lastly by checking the status bit of the newly enabled antenna in order to confirm that it was 
correctly switched on.

This software is written for use with a MT8870 DTMF decoder chip. The input pin ST_CTRL_BIT acts as
a control signal letting the software know that a new DTMF tone has been detected.  Upon detecting a rising edge 
from te CTRL bit, the inputs corresponding to DTMF bits 0-3 are read as binary, and compared to a lookup table 
in order to confirm which digit has been sent. 



This software requires the Arduino LiquidCrystal library to run as is.
If you dont want to use the LiquidCrystal library, simply comment out the 
print statements that use it, and use the serial output for debugging.

Although this software was written for a somewhat specific system, the DTMF decoding portion of it is portable and can 
certainly be used for other applications requiring software that reacts to dtmf events. (with an 8870 dtmf decoder chip)

*/
#include <LiquidCrystal.h>

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) //ClearBit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))  //SetBit

#define CLR(x,y) (x&=(~(1<<y)))
#define SET(x,y) (x|=(1<<y))
#define _BV(bit)(1<<(bit))

//**********************   Port B Pins     ****************************
#define DTMF_BIT0   0
#define DTMF_BIT1   1
#define DTMF_BIT2   2
#define DTMF_BIT3   3
#define ST_CTRL_BIT 4
//Override switch for enabling antennas without using DTMF codes
#define OVERRIDE_SWITCH  5
//Output pin for confirmation tone upon successfully switching antennas
#define TONE_OUTPUT  6
//**********************   Port D Pins     ****************************
#define CPOL_ENABLE    2
#define CPOL_STATUS    3
#define VPOL_ENABLE    4
#define VPOL_STATUS    5
#define INHIBIT_NHRC2  6
#define SWITCHING_CIRCUIT_POWER_STATUS 7
//*******************     Tone Generator      *************************
#define DOT_PERIOD 100
#define DASH_PERIOD (DOT_PERIOD*3)
#define RELAXTIME (DOT_PERIOD)
#define FREQUENCY 440
//******************** Other macro constants  *************************
#define SWITCHING_CIRCUIT_ON 0x80
#define SWITCHING_CIRCUIT_OFF 0x00
#define VPOL_ACTIVE 0
#define DELAY 5
#define TIMEOUT 15
#define NHRC2_INHIBIT_DELAY 5
#define ENABLED 1
#define DISABLED 0
#define CODE_LENGTH 5
/*
 * LCD RS pin to A1 = pin 24
 * LCD Enable to A0 = pin 23
 * LCD D4 pin to A5 = pin 28
 * LCD D5 pin to A4 = pin 27
 * LCD D6 pin to A3 = pin 26
 * LCD D7 pin to A2 = pin 25
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/
//*********************************************************************
enum{
  IDLE,
  DTMF_DETECTED,
  MSG_DECODE,
  ENABLE_CPOL,
  ENABLE_VPOL,
} STATE;
//*********************************************************************
volatile unsigned long timeoutCounter = 0;
volatile byte portBStatus;
volatile byte portDStatus;
volatile byte prevPortBStatus;
volatile byte dtmfBits;
volatile bool cpolEnableBtn = false;
//*********************************************************************
//************** Port D Status Bits for Antenna States ****************
//*********************************************************************
//CPOL Antenna is active HIGH, --> when enabled = 1, CPOL Status = 0
//When CPOL is disabled CPOL_ENABLE = 0, CPOL status = 1
//*********************************************************************
//VPOL Antenna is active LOW, --> when enabled = 0, VPOL Status = 1
//When VPOL is disabled VPOL_ENABLE = 1, VPOL Status = 0
/*
PORTD:
cpolEnabled =  0bxxxx01xx;
cpolDisabled = 0bxxxx10xx;
*/
byte cpolEnabled =  0b00000100;
byte cpolDisabled = 0b00001000;
/*
PORTD:
vpolEnabled =  0bxx10xxxx;
vpolDisabled = 0bxx01xxxx;
*/
byte vpolEnabled =  0b00100000;
byte vpolDisabled = 0b00010000;
//********************    ISR Flags     *******************************
boolean timeoutFlag = false;
boolean dtmfFlag = false;
//*********************************************************************
int currDtmfIndex = 0;
char dtmf[CODE_LENGTH];
//*********************************************************************
/* These number sequences can be changed according to your preference.
*/
char cpolEnablingSequence[CODE_LENGTH] = {'6','9','6','9','6'};
char vpolEnablingSequence[CODE_LENGTH]  = {'9','6','9','6','9'};
//*********************************************************************
//LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7)
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
                   
void setup() {
  lcd.begin(16, 2);
  setupPins();
  setupTimer();
  disableCpol();
  enableVpol();
  enable_NHRC2();
  Serial.begin(9600); 
}

void loop() {

      dtmfStates();
}

//*************************    ISRs    ********************************

ISR (PCINT0_vect){
  portDStatus = PIND;
  if(portDStatus & SWITCHING_CIRCUIT_ON){
    portBStatus = PINB;
        if((portBStatus & 0x20) && (prevPortBStatus & 0x00)){
             STATE = ENABLE_CPOL; 
        }
        else if((portBStatus & 0x00) && (prevPortBStatus & 0x20)){
            STATE = ENABLE_VPOL;      
        }
        else if(portBStatus & 0x10 ){
          dtmfBits = portBStatus & 0x0F;        
          dtmfFlag = true;
    }
    prevPortBStatus = portBStatus;
  }
  else{
    STATE = ENABLE_VPOL;
  }
}

ISR (TIMER1_COMPA_vect){
        timeoutCounter++;
}

//********************     State Machine     *************************

void dtmfStates(){
    switch(STATE){
        case IDLE:       
            if(dtmfFlag){
                STATE = DTMF_DETECTED;
                break;
            }
        case DTMF_DETECTED:
            while(currDtmfIndex < CODE_LENGTH){
                if(dtmfFlag){
                    timeoutCounter = 0;
                    dtmf[currDtmfIndex] = mapDTMFTone(dtmfBits);
                    
                    Serial.print("DTMF[");Serial.print(currDtmfIndex);Serial.print("] = ");Serial.println(dtmf[currDtmfIndex]);
                    lcd.clear();
                    lcd.print("DTMF[");lcd.print(currDtmfIndex);lcd.print("] = ");lcd.print(dtmf[currDtmfIndex]);
                    currDtmfIndex++;
                    portBStatus = 0b00000000;
                    dtmfFlag = false;
                }
               dtmfFlag = false; 
               if(timeoutCounter >= 30 && currDtmfIndex > 0){
                   Serial.println("TIMEOUT");
                   lcd.clear();
                   lcd.print("TIMEOUT");
                   for(int i = 0; i < CODE_LENGTH; i++){
                       dtmf[i] = 0b00000000;
                   } 
                   currDtmfIndex = 0;
                   timeoutCounter = 0;
                   STATE = IDLE;
                   break; 
               }
               else if(currDtmfIndex == CODE_LENGTH){
                   currDtmfIndex = 0;
                   STATE = MSG_DECODE;
                   break;
               }            
            } 
        case MSG_DECODE:
              if(dtmfArrayDecode(dtmf, cpolEnablingSequence, CODE_LENGTH)){
                Serial.println("CPOL_EN!");
                lcd.clear();
                lcd.print("CPOL requested");
                STATE = ENABLE_CPOL;
                break;
            }
              else if(dtmfArrayDecode(dtmf, vpolEnablingSequence, CODE_LENGTH)){
                Serial.println("VPOL_EN!");
                lcd.clear();
                lcd.print("VPOL requested");
                STATE = ENABLE_VPOL;
                break;
            }
            else{
                Serial.println("STATE = IDLE");
                lcd.clear();
                lcd.print("Invalid Code!");
                STATE = IDLE;
                break;
            }
            
        case ENABLE_CPOL:
            if(cpolStatus() == ENABLED){
                Serial.println("back to idle");
                lcd.clear();
                lcd.print("CPOL already on");
                STATE = IDLE;
                break;
            }
            else{
                timeoutCounter = 0;
                inhibit_NHRC2();
                while(timeoutCounter < NHRC2_INHIBIT_DELAY){
                    Serial.println(timeoutCounter);
                }
                Serial.println("inhibited");
                lcd.clear();
                lcd.print("NHRC2 inhibited.");
                timeoutCounter = 0;
                while(timeoutCounter < TIMEOUT){
                     disableVpol();
                     if(vpolStatus() == DISABLED){
                         enableCpol();
                          lcd.clear();
                         lcd.print("VPOL Disabled");
                         break;
                     }
                     Serial.println(timeoutCounter); 
                }
                if(timeoutCounter == TIMEOUT){
                    Serial.println("CPOL enable unsuccessful");
                    lcd.clear();
                    lcd.print("CPOL Fail");
                    enableVpol();
                    enable_NHRC2();
                    STATE = IDLE;
                    break;
                }
                if(cpolStatus() == ENABLED){
                    Serial.println("CPOL ENABLED!");
                    lcd.setCursor(1,2);
                    lcd.print("CPOL Enabled");
                }
                enable_NHRC2();
                STATE = IDLE;               
                break;
            } 
        case ENABLE_VPOL:
            if(vpolStatus() == ENABLED){
                Serial.println("back to idle");
                lcd.clear();
                lcd.print("VPOL already on");
                STATE = IDLE;
                break;
            }
            else{
                timeoutCounter = 0;
                inhibit_NHRC2();
                while(timeoutCounter < NHRC2_INHIBIT_DELAY){
                    Serial.println(timeoutCounter);
                }
                Serial.println("inhibited");
                lcd.clear();
                lcd.print("NHRC2 inhibited.");
                timeoutCounter = 0;
                while(timeoutCounter < TIMEOUT){
                     disableCpol();
                     if(cpolStatus() == DISABLED){
                         enableVpol();
                         lcd.clear();
                         lcd.print("CPOL Disabled");
                         break;
                     }
                     Serial.println(timeoutCounter); 
                }
                if(timeoutCounter == TIMEOUT){
                    Serial.println("VPOL enable unsuccessful");
                    lcd.clear();
                    lcd.print("VPOL Fail");
                    enable_NHRC2();
                    enableCpol();
                    STATE = IDLE;
                    break;
                }
                if(vpolStatus() == ENABLED){
                    Serial.println("VPOL ENABLED!");
                    lcd.setCursor(1,2);
                    lcd.print("VPOL Enabled");
                }
                enable_NHRC2();
                STATE = IDLE;               
                break;
            } 
        
  
            
    }
}

//*********************************************************************

//******************* DTMF Decoding Function ************************

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

//********************** Setup Functions ****************************

void setupPins(){
  //declare i/o pins 
  DDRD   = DDRD | 0b01010100;
  DDRB   = DDRB | 0b11000000; 
  PCICR  |= (1 << PCIE0);    // PCICR  |= 0b00000001;  
  PCMSK0 |= (1 << PCINT0 | 1 << PCINT1 | 1 << PCINT2 | 1 << PCINT3 | 1 << PCINT4);  // PCMSK2 |= 0b01111000; //ENABLE PCINT 2 pins
  SET(PORTD,VPOL_ENABLE);
  CLR(PORTD,CPOL_ENABLE);    
  CLR(PORTD,INHIBIT_NHRC2);    //set inhibit line to low PD2 = gpio pin 4 --> digitalWrite(INHIBIT_NHRC2, LOW);
  
  
}

void setupTimer() //sets up a timed ISR every 1/10 second
{
    cli(); //disable interrupts
    //Setup Timer 1 ISR
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 =  0;
    OCR1A = 1562; // ISR @ 5Hz
    TCCR1B |= (1 << WGM12);    //turn on CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10);     //set CS10 and CS12 bits for 1024 prescalar
    TIMSK1 |= (1 << OCIE1A);     //enable timer compareinterrupt
    
    //Timer 0  ISR
    TCCR0A = 0;// set entire TCCR0A register to 0
    TCCR0B = 0;// same for TCCR0B
    TCNT0  = 0;//initialize counter value to 0
    //OCR0A = 249;// = (16*10^6) / (2000*64) - 1 (must be <256)
    OCR0A = 63;  //Configure ISR  ro run at 2KHz
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS01) | (1 << CS00);    // Set CS01 and CS00 bits for 64 prescaler
    TIMSK0 |= (1 << OCIE0A);   // enable timer compare interrupt
    sei(); //allow interrupts
}

//******************** Antenna Control Functions ***********************

void enableCpol(){
    SET(PORTD, CPOL_ENABLE);
}

void disableCpol(){
    CLR(PORTD,CPOL_ENABLE);
}

void enableVpol(){
   CLR(PORTD,VPOL_ENABLE); 
}

void disableVpol(){
    SET(PORTD,VPOL_ENABLE);
}

//******************** Antenna Status Functions ***********************

boolean cpolStatus(){
    if(PIND & cpolEnabled){
        return ENABLED;
    }
    else if(PIND & cpolDisabled){
        return DISABLED;
    }
}


boolean vpolStatus(){
    if(PIND & vpolEnabled){
        return ENABLED;
    }
    else if(PIND & vpolDisabled){
        return DISABLED;
    }
}

boolean isCpolEnabled(){
    if(cpolStatus() == ENABLED && vpolStatus == DISABLED){
        return ENABLED;
    }
    else{
        return DISABLED;
    }
    
}

boolean isVpolEnabled(){
    if(vpolStatus() == ENABLED && cpolStatus == DISABLED){
        return ENABLED;
    }
    else{
        return DISABLED;
    }
}

//*********************** NHRC2 Control Functions *********************

void inhibit_NHRC2(){
   CLR(PORTD, INHIBIT_NHRC2); 
}

void enable_NHRC2(){
   SET(PORTD, INHIBIT_NHRC2); 
}


