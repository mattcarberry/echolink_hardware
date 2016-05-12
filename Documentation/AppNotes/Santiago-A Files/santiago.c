/* "Santiago" -- An inline adapter to add URI connectivity to
    existing full-duplex link devices.
    Jim Dixon, WB6NIL  Ver. 1.0 02/18/11
	This program is placed in the public domain, and may be used
	by anyone in any way for any legal purpose */

/*  Version 0.1, 9/17/10 -- Initial Version

    Version 0.2, 10/7/10 -- Added 'notrpt' mode (at least currently
        as a program-defined variable, with the intention of this
        becomming a hardware bit in the next hardware revision. Also,
		made it so URI does not receive keying signals when its 'disconnected'.

	Version 1.0, 02/18/11 -- Now supports Rev. A hardware, sensing
		the 'notrpt' mode from the RA7 bit (JP6 on the board).
*/

/* Port 'A' is the controller, and Port 'B' is the controled device,
	such as a link radio, or repeater system (depending on configuration) */

#include <pic.h>
#include <pic16f6x.h>

//  This __Config command sets the register config at the PIC's memory
//  address 2007h.  The bit settings 0x3FF0 correspond to:
//  - no code protection              - high voltage (standard) programming
//  - brown-out detection enabled     - Master clear enabled
//  - power-up timer enabled          - watch-dog timer disabled
//  - use of the internal oscillator, RA6 and RA7 left for I/O.
__CONFIG(0x3F70);

#define	TO_VAL 18000 /* timeout value, 18000 1/10 seconds (30 mins) */



void rst_timer(void) {
	T1CON = 0x30; /* Timer 1 internal OSC/8 + Disabled */;
	TMR1L = 0x50; /* Low byte of 0xc350 (50000) */
	TMR1H = 0xc3; /* High byte of 0xc350) (50000) */
	TMR1IF = 0; /* Reset overflow flag */
	T1CON = 0x31; /* Timer 1 internal OSC/8 + Enabled */
}

void main(void) {
	static unsigned short counta = 0,countb = 0,countu = 0;
	static bit cora,corb, coruri,cora_rpt,coruri_rpt,notrpt;

	TRISA = 0xff; /* Bits 0-7 are TS */
	TRISB = 0xc0; /* Bits 6,7 are TS */
	PORTA = 0x7f;
	PORTB = 0xc0;
	CMCON = 7;
	OSCF = 1; /* Set to 4 MHz */
	/* Set 'notrpt' to 1 for "standard" conriguration. Set it
	   to 0 for repeater-only operation (URI only keys port B,
	   port B only keys URI */
	notrpt = 0;

	rst_timer();
	while (1) {
		if (TMR1IF) {  /* if Timer 1 expireed, (10Hz) */
			rst_timer();
			if (counta && (counta < TO_VAL)) counta++;
			if (countb && (countb < TO_VAL)) countb++;
			if (countu && (countu < TO_VAL)) countu++;
			RB4 = !RB4; /* Toggle test bit for debug purposes */
		}
		/* if counter for Device A active, assert PTT */
		if (counta && (counta < TO_VAL)) {
			RB0 = 1;
			RB1 = 0;
		} else { /* otherwise, de-assert it */
			RB0 = 0;
			RB1 = 1;
		}
		/* if counter for Device B active, assert PTT */
		if (countb && (countb < TO_VAL)) {
			RB2 = 1;
			RB3 = 0;
		} else { /* otherwise, de-assert it */
			RB2 = 0;
			RB3 = 1;
		}
		/* if counter for URI active, assert PTT */
		if ((countu && (countu < TO_VAL)) && RA6)  /* qualify with URI enable */
			RB5 = 0;
		else /* otherwise, de-assert it */
			RB5 = 1;
		/* see if we are in 'rpt' mode or not */
		notrpt = RA7;
		/* set cora from RA0 and RA3 */
		cora = RA0 ^ RA3;
		/* set corb from RA1 and RA4 */
		corb = RA1 ^ RA4;
		/* set coruri from RA2 and RA6 */
		coruri = ((!RA2) && RA6);
		/* qualify cora with notrpt, so that port A
           may be isolated for 'rpt' mode */
		cora_rpt = cora && notrpt;
		/* qualify coruri with notrpt, so that port A
           may be isplated for 'rpt' mode */
		coruri_rpt = coruri && notrpt;
		/* if COR_B or (COR_URI & Enabled) let counta go,
			otherwise, set it to zero */
		if ((!corb) && (!coruri_rpt)) counta = 0; 
		else {
			if (counta == 0) counta = 1;
		}
		/* if COR_A or (COR_URI & Enabled) let countb go,
			otherwise, set it to zero */
		if ((!cora) && (!coruri)) countb = 0;
		else {
			if (countb == 0) countb = 1;
		}
		/* if COR_A or COR_B let countu go,
			otherwise, set it to zero */
		if ((!cora_rpt) && (!corb)) countu = 0;
		else {
			if (countu == 0) countu = 1;
		}
	}
}

