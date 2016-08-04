LIBRARY IEEE;
USE IEEE.std_logic_1164.all;

ENTITY SCHEMATIC1 IS 

END SCHEMATIC1;



ARCHITECTURE STRUCTURE OF SCHEMATIC1 IS

-- COMPONENTS

COMPONENT \100nF\
	PORT (
	A : INOUT std_logic;
	B : INOUT std_logic
	); END COMPONENT;

COMPONENT TL072
	PORT (
	\+IN_A\ : IN std_logic;
	\-IN_A\ : IN std_logic;
	OUT_A : OUT std_logic;
	\+9v\ : IN std_logic;
	GND : IN std_logic;
	\+IN_B\ : IN std_logic;
	\-IN_B\ : IN std_logic;
	OUT_B : OUT std_logic
	); END COMPONENT;

COMPONENT \LM2940 / SOT223  LDO\
	PORT (
	\IN\ : IN std_logic;
	G1 : INOUT std_logic;
	\+9V\ : IN std_logic;
	G2 : INOUT std_logic
	); END COMPONENT;

COMPONENT \51k\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \74HC08\
	PORT (
	A_A : IN std_logic;
	B_A : IN std_logic;
	Y_A : OUT std_logic;
	\+3.3V\ : IN std_logic;
	GND : IN std_logic;
	A_B : IN std_logic;
	B_B : IN std_logic;
	Y_B : OUT std_logic;
	A_C : IN std_logic;
	B_C : IN std_logic;
	Y_C : OUT std_logic;
	A_D : IN std_logic;
	B_D : IN std_logic;
	Y_D : OUT std_logic
	); END COMPONENT;

COMPONENT \74HC04\
	PORT (
	A_A : IN std_logic;
	Y_A : OUT std_logic;
	\+3.3V\ : IN std_logic;
	GND : IN std_logic;
	A_B : IN std_logic;
	Y_B : OUT std_logic;
	A_C : IN std_logic;
	Y_C : OUT std_logic;
	A_D : IN std_logic;
	Y_D : OUT std_logic;
	A_E : IN std_logic;
	Y_E : OUT std_logic;
	A_F : IN std_logic;
	Y_F : OUT std_logic
	); END COMPONENT;

COMPONENT \LM2936/SO_5\
	PORT (
	\+3.3V\ : IN std_logic;
	\+9V\ : IN std_logic;
	GND : IN std_logic
	); END COMPONENT;

COMPONENT \10k\
	PORT (
	A : INOUT std_logic;
	B : INOUT std_logic;
	WIPER : INOUT std_logic
	); END COMPONENT;

COMPONENT \HEADER 3\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic;
	\3\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \10uF\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \0.156 VH\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \10uF\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \HEADER 20X2\
	PORT (
	\2\ : INOUT std_logic;
	\4\ : INOUT std_logic;
	\6\ : INOUT std_logic;
	\8\ : INOUT std_logic;
	\10\ : INOUT std_logic;
	\12\ : INOUT std_logic;
	\14\ : INOUT std_logic;
	\16\ : INOUT std_logic;
	\18\ : INOUT std_logic;
	\20\ : INOUT std_logic;
	\22\ : INOUT std_logic;
	\24\ : INOUT std_logic;
	\26\ : INOUT std_logic;
	\28\ : INOUT std_logic;
	\30\ : INOUT std_logic;
	\32\ : INOUT std_logic;
	\34\ : INOUT std_logic;
	\36\ : INOUT std_logic;
	\38\ : INOUT std_logic;
	\40\ : INOUT std_logic;
	\1\ : INOUT std_logic;
	\3\ : INOUT std_logic;
	\5\ : INOUT std_logic;
	\7\ : INOUT std_logic;
	\9\ : INOUT std_logic;
	\11\ : INOUT std_logic;
	\13\ : INOUT std_logic;
	\15\ : INOUT std_logic;
	\17\ : INOUT std_logic;
	\19\ : INOUT std_logic;
	\21\ : INOUT std_logic;
	\23\ : INOUT std_logic;
	\25\ : INOUT std_logic;
	\27\ : INOUT std_logic;
	\29\ : INOUT std_logic;
	\31\ : INOUT std_logic;
	\33\ : INOUT std_logic;
	\35\ : INOUT std_logic;
	\37\ : INOUT std_logic;
	\39\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \HEADER 16\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic;
	\3\ : INOUT std_logic;
	\4\ : INOUT std_logic;
	\5\ : INOUT std_logic;
	\6\ : INOUT std_logic;
	\7\ : INOUT std_logic;
	\8\ : INOUT std_logic;
	\9\ : INOUT std_logic;
	\10\ : INOUT std_logic;
	\11\ : INOUT std_logic;
	\12\ : INOUT std_logic;
	\13\ : INOUT std_logic;
	\14\ : INOUT std_logic;
	\15\ : INOUT std_logic;
	\16\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \HEADER 9\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic;
	\3\ : INOUT std_logic;
	\4\ : INOUT std_logic;
	\5\ : INOUT std_logic;
	\6\ : INOUT std_logic;
	\7\ : INOUT std_logic;
	\8\ : INOUT std_logic;
	\9\ : INOUT std_logic
	); END COMPONENT;

COMPONENT TX_Echo
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic;
	\3\ : INOUT std_logic
	); END COMPONENT;

COMPONENT SB1245
	PORT (
	ANODE : INOUT std_logic;
	CATHODE : INOUT std_logic
	); END COMPONENT;

COMPONENT \10 Pin Header to DTMF Ctrl\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic;
	\3\ : INOUT std_logic;
	\4\ : INOUT std_logic;
	\5\ : INOUT std_logic;
	\6\ : INOUT std_logic;
	\7\ : INOUT std_logic;
	\8\ : INOUT std_logic;
	\9\ : INOUT std_logic
	); END COMPONENT;

COMPONENT \1N4148\
	PORT (
	\1\ : INOUT std_logic;
	\2\ : INOUT std_logic
	); END COMPONENT;

COMPONENT RED
	PORT (
	ANODE : INOUT std_logic;
	CATHODE : INOUT std_logic
	); END COMPONENT;

COMPONENT \22uF/Tant\
	PORT (
	P : INOUT std_logic;
	N : INOUT std_logic
	); END COMPONENT;

COMPONENT \2N2222\
	PORT (
	COLLECTOR : INOUT std_logic;
	EMITTER : INOUT std_logic;
	BASE : IN std_logic
	); END COMPONENT;

-- SIGNALS

SIGNAL \+3.3V\ : std_logic;
SIGNAL \+4.5V\ : std_logic;
SIGNAL \+9V\ : std_logic;
SIGNAL AUDIOOUT : std_logic;
SIGNAL AUDIO_AUX : std_logic;
SIGNAL COS : std_logic;
SIGNAL COS_ALLSTAR : std_logic;
SIGNAL DTMF_AUDIO : std_logic;
SIGNAL DTMF_TONE_CONFIRMATION : std_logic;
SIGNAL GND : std_logic;
SIGNAL GND_SIGNAL : std_logic;
SIGNAL INHIBIT : std_logic;
SIGNAL N134507 : std_logic;
SIGNAL N134567 : std_logic;
SIGNAL N134575 : std_logic;
SIGNAL N134810 : std_logic;
SIGNAL N14521945 : std_logic;
SIGNAL N14527017 : std_logic;
SIGNAL N14538770 : std_logic;
SIGNAL N14545130 : std_logic;
SIGNAL N14551658 : std_logic;
SIGNAL N145714160 : std_logic;
SIGNAL N14592870 : std_logic;
SIGNAL N14597991 : std_logic;
SIGNAL N14601768 : std_logic;
SIGNAL N14603309 : std_logic;
SIGNAL N14603313 : std_logic;
SIGNAL N14619662 : std_logic;
SIGNAL N146226602 : std_logic;
SIGNAL N14629991 : std_logic;
SIGNAL N196050 : std_logic;
SIGNAL N88559 : std_logic;
SIGNAL N88575 : std_logic;
SIGNAL N89923 : std_logic;
SIGNAL N90145 : std_logic;
SIGNAL N90149 : std_logic;
SIGNAL N904010 : std_logic;
SIGNAL N90849 : std_logic;
SIGNAL N908810 : std_logic;
SIGNAL N96497 : std_logic;
SIGNAL PTT : std_logic;
SIGNAL PTT_E : std_logic;
SIGNAL RX_A : std_logic;
SIGNAL RX_E : std_logic;
SIGNAL TX_A : std_logic;

-- INSTANCE ATTRIBUTES

ATTRIBUTE PSPICETEMPLATE:string;
ATTRIBUTE PSPICETEMPLATE of D2 : label is "D^@REFDES %1 %2 @MODEL ?AREA/@AREA/";
ATTRIBUTE \COMPONENT\:string;
ATTRIBUTE \COMPONENT\ of D2 : label is "1N4148";
ATTRIBUTE PSPICETEMPLATE of D4 : label is "D^@REFDES %1 %2 @MODEL ?AREA/@AREA/";
ATTRIBUTE \COMPONENT\ of D4 : label is "1N4148";


-- GATE INSTANCES

BEGIN
GND_SIGNAL<=GND;
C20 : \100nF\	PORT MAP(
	A => GND, 
	B => \+9V\
);
U1 : TL072	PORT MAP(
	\+IN_A\ => \+4.5V\, 
	\-IN_A\ => N88559, 
	OUT_A => N88575, 
	\+9v\ => \+9V\, 
	GND => GND, 
	\+IN_B\ => \+4.5V\, 
	\-IN_B\ => N90849, 
	OUT_B => N89923
);
U3 : \LM2940 / SOT223  LDO\	PORT MAP(
	\IN\ => N14521945, 
	G1 => GND, 
	\+9V\ => \+9V\, 
	G2 => GND
);
R1 : \51k\	PORT MAP(
	\1\ => N90145, 
	\2\ => N196050
);
R2 : \51k\	PORT MAP(
	\1\ => N88575, 
	\2\ => N88559
);
U5 : \74HC08\	PORT MAP(
	A_A => N14527017, 
	B_A => PTT_E, 
	Y_A => N14629991, 
	\+3.3V\ => \+3.3V\, 
	GND => GND, 
	A_B => PTT_E, 
	B_B => N14597991, 
	Y_B => N96497, 
	A_C => 'Z', 
	B_C => 'Z', 
	Y_C => OPEN, 
	A_D => 'Z', 
	B_D => 'Z', 
	Y_D => OPEN
);
R3 : \51k\	PORT MAP(
	\1\ => N90149, 
	\2\ => TX_A
);
U6 : \74HC04\	PORT MAP(
	A_A => N14527017, 
	Y_A => N14597991, 
	\+3.3V\ => \+3.3V\, 
	GND => GND, 
	A_B => PTT_E, 
	Y_B => N14545130, 
	A_C => N14619662, 
	Y_C => N14592870, 
	A_D => 'Z', 
	Y_D => OPEN, 
	A_E => 'Z', 
	Y_E => OPEN, 
	A_F => 'Z', 
	Y_F => OPEN
);
R4 : \51k\	PORT MAP(
	\1\ => N90849, 
	\2\ => N89923
);
U7 : \LM2936/SO_5\	PORT MAP(
	\+3.3V\ => \+3.3V\, 
	\+9V\ => \+9V\, 
	GND => GND
);
R5 : \7.5k\	PORT MAP(
	\1\ => N134507, 
	\2\ => N134810
);
R6 : \10k\	PORT MAP(
	A => N134810, 
	B => GND, 
	WIPER => RX_E
);
R7 : \7.5k\	PORT MAP(
	\1\ => N134575, 
	\2\ => N904010
);
R8 : \51k\	PORT MAP(
	\1\ => N908810, 
	\2\ => N90849
);
R9 : \10k\	PORT MAP(
	A => N904010, 
	B => GND, 
	WIPER => RX_A
);
J11 : \HEADER 3\	PORT MAP(
	\1\ => N14601768, 
	\2\ => GND, 
	\3\ => N14603309
);
C1 : \10uF\	PORT MAP(
	\1\ => N88559, 
	\2\ => N90145
);
J12 : \HEADER 3\	PORT MAP(
	\1\ => N14601768, 
	\2\ => GND, 
	\3\ => N14603313
);
C2 : \100pF\	PORT MAP(
	A => N88575, 
	B => N88559
);
J13 : \0.156 VH\	PORT MAP(
	\1\ => GND, 
	\2\ => N14603313
);
C3 : \10uF\	PORT MAP(
	\1\ => N88559, 
	\2\ => N90149
);
J14 : \0.156 VH\	PORT MAP(
	\1\ => GND, 
	\2\ => N14603309
);
C4 : \10uF\	PORT MAP(
	\1\ => N88575, 
	\2\ => AUDIOOUT
);
J15 : \HEADER 20X2\	PORT MAP(
	\2\ => GND, 
	\4\ => OPEN, 
	\6\ => OPEN, 
	\8\ => OPEN, 
	\10\ => OPEN, 
	\12\ => OPEN, 
	\14\ => OPEN, 
	\16\ => OPEN, 
	\18\ => OPEN, 
	\20\ => OPEN, 
	\22\ => OPEN, 
	\24\ => OPEN, 
	\26\ => OPEN, 
	\28\ => N14592870, 
	\30\ => PTT_E, 
	\32\ => OPEN, 
	\34\ => N96497, 
	\36\ => OPEN, 
	\38\ => OPEN, 
	\40\ => OPEN, 
	\1\ => OPEN, 
	\3\ => OPEN, 
	\5\ => OPEN, 
	\7\ => OPEN, 
	\9\ => OPEN, 
	\11\ => OPEN, 
	\13\ => OPEN, 
	\15\ => OPEN, 
	\17\ => OPEN, 
	\19\ => OPEN, 
	\21\ => OPEN, 
	\23\ => OPEN, 
	\25\ => OPEN, 
	\27\ => OPEN, 
	\29\ => OPEN, 
	\31\ => OPEN, 
	\33\ => OPEN, 
	\35\ => OPEN, 
	\37\ => OPEN, 
	\39\ => OPEN
);
C5 : \10uF\	PORT MAP(
	\1\ => N89923, 
	\2\ => N134507
);
J16 : \HEADER 16\	PORT MAP(
	\1\ => N14527017, 
	\2\ => COS_ALLSTAR, 
	\3\ => N14545130, 
	\4\ => OPEN, 
	\5\ => OPEN, 
	\6\ => OPEN, 
	\7\ => OPEN, 
	\8\ => OPEN, 
	\9\ => OPEN, 
	\10\ => OPEN, 
	\11\ => OPEN, 
	\12\ => OPEN, 
	\13\ => OPEN, 
	\14\ => OPEN, 
	\15\ => OPEN, 
	\16\ => GND
);
C6 : \100nF\	PORT MAP(
	A => GND, 
	B => \+9V\
);
J17 : \HEADER 9\	PORT MAP(
	\1\ => GND, 
	\2\ => AUDIOOUT, 
	\3\ => AUDIO_AUX, 
	\4\ => GND, 
	\5\ => COS, 
	\6\ => GND, 
	\7\ => GND, 
	\8\ => INHIBIT, 
	\9\ => PTT
);
C7 : \1uF\	PORT MAP(
	\1\ => N908810, 
	\2\ => AUDIO_AUX
);
J18 : \0.156 VH\	PORT MAP(
	\1\ => GND, 
	\2\ => N14603309
);
C8 : \10uF\	PORT MAP(
	\1\ => N89923, 
	\2\ => N134575
);
C9 : \4.7uF\	PORT MAP(
	\1\ => \+4.5V\, 
	\2\ => GND
);
R10 : \7.5k\	PORT MAP(
	\1\ => N134567, 
	\2\ => DTMF_AUDIO
);
R11 : \10k\	PORT MAP(
	\1\ => GND, 
	\2\ => DTMF_AUDIO
);
R12 : \51k\	PORT MAP(
	\1\ => N14551658, 
	\2\ => DTMF_TONE_CONFIRMATION
);
R13 : \51k\	PORT MAP(
	\1\ => \+4.5V\, 
	\2\ => \+9V\
);
R14 : \10k\	PORT MAP(
	\1\ => N14619662, 
	\2\ => \+3.3V\
);
J1 : TX_Echo	PORT MAP(
	\1\ => GND, 
	\2\ => N196050, 
	\3\ => N196050
);
R15 : \10k\	PORT MAP(
	\1\ => \+3.3V\, 
	\2\ => N146226602
);
J2 : TX_Allstar	PORT MAP(
	\1\ => GND, 
	\2\ => TX_A, 
	\3\ => TX_A
);
R16 : \51k\	PORT MAP(
	\1\ => GND, 
	\2\ => \+4.5V\
);
J4 : RX_Echo	PORT MAP(
	\1\ => GND, 
	\2\ => RX_E, 
	\3\ => RX_E
);
R17 : \330\	PORT MAP(
	\1\ => N145714160, 
	\2\ => \+3.3V\
);
R18 : \10k\	PORT MAP(
	\1\ => PTT_E, 
	\2\ => \+3.3V\
);
J5 : Rx_Allstar	PORT MAP(
	\1\ => GND, 
	\2\ => RX_A, 
	\3\ => RX_A
);
D1 : SB1245	PORT MAP(
	ANODE => N14538770, 
	CATHODE => N14521945
);
R19 : \10k\	PORT MAP(
	\1\ => N14527017, 
	\2\ => \+3.3V\
);
J6 : \10 Pin Header to DTMF Ctrl\	PORT MAP(
	\1\ => DTMF_AUDIO, 
	\2\ => GND, 
	\3\ => DTMF_TONE_CONFIRMATION, 
	\4\ => GND, 
	\5\ => INHIBIT, 
	\6\ => GND, 
	\7\ => GND, 
	\8\ => GND, 
	\9\ => GND
);
D2 : \1N4148\	PORT MAP(
	\1\ => N14619662, 
	\2\ => COS
);
D3 : RED	PORT MAP(
	ANODE => N145714160, 
	CATHODE => GND
);
J7 : \0.156 VH\	PORT MAP(
	\1\ => N14538770, 
	\2\ => GND
);
D4 : \1N4148\	PORT MAP(
	\1\ => COS_ALLSTAR, 
	\2\ => COS
);
D5 : B320	PORT MAP(
	ANODE => N14538770, 
	CATHODE => N14601768
);
C10 : \10uF\	PORT MAP(
	\1\ => N89923, 
	\2\ => N134567
);
C11 : \10uF\	PORT MAP(
	\1\ => N88559, 
	\2\ => N14551658
);
C12 : \470nF\	PORT MAP(
	A => N14521945, 
	B => GND
);
C13 : \22uF/Tant\	PORT MAP(
	P => \+9V\, 
	N => GND
);
C14 : \100nF\	PORT MAP(
	A => GND, 
	B => \+9V\
);
C15 : \10uF\	PORT MAP(
	\1\ => \+3.3V\, 
	\2\ => GND
);
Q1 : \2N2222\	PORT MAP(
	COLLECTOR => PTT, 
	EMITTER => N14629991, 
	BASE => N146226602
);
C16 : \10uF\	PORT MAP(
	\1\ => \+3.3V\, 
	\2\ => GND
);
C17 : \100nF\	PORT MAP(
	A => GND, 
	B => \+3.3V\
);
C18 : \100nF\	PORT MAP(
	A => GND, 
	B => \+3.3V\
);
C19 : \100nF\	PORT MAP(
	A => \+3.3V\, 
	B => GND
);
END STRUCTURE;

