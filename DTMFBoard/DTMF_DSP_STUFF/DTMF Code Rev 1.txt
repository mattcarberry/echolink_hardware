#include <Time.h>  
#include <avr/eeprom.h>
#include <avr/pgmspace.h>


int bin0 = A4;
int bin1 = A3;
int bin2 = A2;
int bin3 = A1;
int STD  = A0;

int Out1 =  2;
int Out2 =  3;
int Out3 =  4;
int Out4 =  5;
int Out5 =  6;
int Out6 =  7;
int Out7 =  8;
int Out8 =  9;
int relay =  10;
int Inhibit = 11;
int lin =  7;
int cpol = 6;

int i= 0;
char dtmf[5];
char cpolarray[] = "54321";
char linarray[]  = "12345";

byte irqState;

void setup() {
	Serial.begin(9600);
	pinMode(bin0,  INPUT);
	pinMode(bin1,  INPUT);
	pinMode(bin2,  INPUT);
	pinMode(bin3,  INPUT);
    pinMode(STD,   INPUT);
	pinMode(Out1,  OUTPUT);
	pinMode(Out2,  OUTPUT);
	pinMode(Out3,  OUTPUT);
	pinMode(Out4,  OUTPUT);	
	pinMode(Out5,  OUTPUT);
	pinMode(Out6,  OUTPUT);
	pinMode(Out7,  OUTPUT);
	pinMode(Out8,  OUTPUT);
	pinMode(relay,  OUTPUT);
	pinMode(Inhibit, OUTPUT);
	pinMode(cpol, OUTPUT);
	pinMode(lin, OUTPUT);
}

void loop() {
         
	irqState = digitalRead(STD); //Get the first trigger tone
	  if (irqState == 1) {
		dtmf[i++] = mapKey(); //Store the command
		//Send data to the console
		char code = mapKey(); //Get the code number
		Serial.print("Data: ");
		Serial.println(code);
		setTime(0); //Reset the timeout counter
		delay(200); //Delay to sync the STD data
                //controller();
	} 
	if ((second() > 3) ){   //&& (i > 1)) {
		i = 0;  //Resets the read counter	
		Serial.println("I've reseted the counter");
		setTime(0);
	}

	if (i == 5) { //When you reach the last digit..
            Serial.print("Data: ");
	    i = 0;  //Resets the read counter	
	    setTime(0);
	    controller();
        }
}

boolean array_cmp(char *a, char *b, int len_a, int len_b){
      int n;
      // if their lengths are different, return false
      if (len_a != len_b) return false;

      // test each element to be the same. if not, return false
      for (n=0;n<len_a;n++) if (a[n]!=b[n]) return false;

      //ok, if we have not returned yet, they are equal :)
      return true;
}


void controller () {
  Serial.println(" ");
        if (dtmf[0] == '1' && dtmf[1] == '2' && dtmf[2] == '3' && dtmf[3] == '4' && dtmf[4] == '5' )
        { //First-enabler tone
           if (array_cmp(dtmf,linarray,  5, 5) == true){
             digitalWrite(lin, HIGH);
	     Serial.println("yay");
             delay (1000);
             digitalWrite(lin, LOW);
             digitalWrite(Inhibit,LOW);
             delay(500);
             digitalWrite(relay,HIGH); //select relay for the linear antenna
             delay(2000);
             digitalWrite(Inhibit,HIGH);         
             i=0;
             setTime(0);
             for(int b; b<5;b++)
             {
             dtmf[b] = 0; 
             }
         }
        } 
         if (dtmf[0] == '5' && dtmf[1] == '4' && dtmf[2] == '3' && dtmf[3] == '2' && dtmf[4] == '1' )
        { //First-enabler tone
           if (array_cmp(dtmf,cpolarray,  5, 5) == true){
            digitalWrite(cpol, HIGH);
	     Serial.println("yay1");
             delay (1000);
             digitalWrite(cpol, LOW);
             i=0;
             setTime(0);
             digitalWrite(Inhibit,LOW);
             delay(500);
             digitalWrite(relay,LOW) ;//select relay for the linear antenna
             delay(2000);
             digitalWrite(Inhibit,HIGH);         
	     for(int b; b<5;b++)
             {
             dtmf[b] = 0; 
             }
           }
        }
     }


byte mapKey() {
	byte data;
        //change data to int
	char key;
	// Get the binary code from MT8870
	byte D0,D1,D2,D3;
	D0 = digitalRead(bin0);
	D1 = digitalRead(bin1);
	D2 = digitalRead(bin2);
	D3 = digitalRead(bin3);

	// Convert binary values to integers
    //data = (D0*1) + (D1*2) + (D2*4) + (D3*8);
	bitWrite(data,0,D0);
	bitWrite(data,1,D1);
	bitWrite(data,2,D2);
	bitWrite(data,3,D3);

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