/*
 * mk_paj7620.h
 *
 *  Created on: 19 maj 2021
 *      Author: Miros�aw Karda�
 *
 *		www.atnel.pl           www.sklep.atnel.pl
 *      Gesture Library for PAJ7620 (Reliable LIB)
 *      ver: 1.0a
 *
 *      - basic 9 gestures with gesture filtering feature
 *      - we can add more than 810 own custom gestures!
 *      - we can use independant circle-continous cw & ccw gesture (FINGER as ROTARY ENCODER)
 *      - we can use INIT-GESTURE with user defined timeout to start recognize other basic and custom gestures
 *      - (callback with the last three gestures)
 *      - work with TWI/I2C, soft I2C and USI I2C (using MK_I2C Library)
 *      - work with INTx and PCINTx interrupt on ATmega/ATtiny
 *      - easy library and custom gestures configuration
 */

#ifndef MK_GESTURE_PAJ7620_MK_PAJ7620_H_
#define MK_GESTURE_PAJ7620_MK_PAJ7620_H_


//********************* KONFIGURACJA *************************************


#define CUSTOM_GS_CLEAR_TIME	800			// ms (Timeout dla zdefiniowanych przez u�ytkownika gest�w z�o�onych)

#define USE_CIRCLE_CONTINOUS	1			// za��czenie opcji niezale�nego kr�cenia k�ek CW i CCW z ci�g�ym wywo�ywaniem zdarze�
#define USE_INIT_GESTURE		1			// za��czenie opcji pracy w trybie odblokowania/zablokowania czujnika przez gest inicjalizacyjny

/* 0 - przerwania typu INTx
 * 1 - przerwania typu PCINTx  */
#define PAJ_INT_TYPE			1


#if PAJ_INT_TYPE == 0								/* je�li wybra�e� przerwania typu INTx */

	#define PAJ_INT_NR			0			// Numer przerwania: 0,1,2,3 ... 7(max)
	#define PAJ_INT_PORT		PORTD		// PORTx dla pinu przerwania
	#define PAJ_INT_PIN_BIT		(1<<PD2)	// pin INTx

	/* wyb�r zbocza falling/opadaj�ce, rising/narastaj�ce, any/dowolne, low_lewel/stan niski */
	/* 0 - FALLING/opadaj�ce
	 * 1 - RISING/narastaj�ce
	 * 2 - ANY/ka�de zbocze
	 * 3 - LOW_LEVEL/stan niski */
	#define PAJ_INT_EDGE		0			// 0-opadaj�ce, 1-narastaj�ce, 2-ka�de zbocze, 3-poziom niski

#endif

#if PAJ_INT_TYPE == 1								/* je�li wybra�e� przerwania typu PCINTx */

	#define PAJ_IRQ_VECTOR		PCINT3_vect	// nazwa wektora przerwa�a PCINT
	#define PAJ_INT_PORT		PORTD		// rejestr wyj�ciowy pinu przerwania PORTx
	#define PAJ_INT_PIN			PIND		// rejestr wej�ciowy pinu przerwania PINx
	#define PAJ_INT_PIN_BIT		(1<<PD2)	// bit pinu przerwania (odpowiednik pinu INT0)

	#define	PAJ_INT_EN_REG		PCICR 		// rejest zezwalaj�cy na przerwania zewn�trzne np INTx / PCINT
	#define PAJ_INT_EN_BIT		PCIE3		// bit odpowiadaj�cy za przerwanie lub grup� przerwa� PCINT w rejestrze wy�ej
	#define PAJ_INT_MASK_REG	PCMSK3		// rejestr maskuj�cy grup� przerwa� PCINT
	#define PAJ_INT_MASK_BIT	PCINT26		// bit w rejestrze maskuj�cym - wyb�r konkretnego pinu np PCINT2, PCINT14 itp

#endif


#define ADD_ISR_NOBLOCK			0			// 0-bez opcji ISR_NOBLOCK, 1-z opcj� ISR_NOBLOCK (ISR_NOBLOCK mo�e by� no b.potrzebne przy korzystaniu z V-USB)


//******************** koniec konfiguracji *******************************



//.........................
#define PAJ_IRQ_INTx				0
#define PAJ_IRQ_PCINT				1


#if PAJ_INT_TYPE == PAJ_IRQ_INTx				// IRQ_INTx
	// m644(P)/1284(P) m328(P)
	#ifdef EICRA
		#define PAJ_INT_EDGE_REG				EICRA			// dla m32 to np MCUCR
	#endif

	#ifdef EIMSK
		#define PAJ_INT_EN_REG					EIMSK			// dla m32 to np GICR
	#endif


	// m16/m32
	#ifdef MCUCR
		#ifndef EICRA
			#define PAJ_INT_EDGE_REG			MCUCR
		#endif
	#endif

	#ifdef GICR
		#define PAJ_INT_EN_REG					GICR
	#endif

	#ifndef PAJ_INT_EN_REG
		#ifdef GIMSK
			#define PAJ_INT_EN_REG				GIMSK
		#endif
	#endif

	// makra do okre�lania reakcji przerwania na zbocze/poziom itp
	#define PAJ_ISC_BIT0(x)			PAJ_IRQ_BIT0A(x)
	#define PAJ_IRQ_BIT0A(x)		(ISC##x##0)

	#define PAJ_ISC_BIT1(x)			PAJ_IRQ_BIT1A(x)
	#define PAJ_IRQ_BIT1A(x)		(ISC##x##1)

	#define PAJ_SET_FALLING_EDGE	PAJ_INT_EDGE_REG |= ( 1<<PAJ_ISC_BIT1(PAJ_INT_NR) )
	#define PAJ_SET_RISING_EDGE		PAJ_INT_EDGE_REG |= ( 1<<PAJ_ISC_BIT1(PAJ_INT_NR) ); PAJ_INT_EDGE_REG |= ( 1<<PAJ_ISC_BIT0(PAJ_INT_NR) )
	#define PAJ_SET_ANY_EDGE		PAJ_INT_EDGE_REG |= ( 1<<PAJ_ISC_BIT0(PAJ_INT_NR) )

	// makra do wyznaczania w�a�ciwej nazwy wektora przerwania
	#define PAJ_INT_BIT0(x)			PAJ_INT_BIT1(x)
	#define PAJ_INT_BIT1(x)			(INT##x)

	#define PAJ_IRQ_V0(x)		 	PAJ_IRQ_V1(x)
	#define PAJ_IRQ_V1(x) 			(INT##x##_vect)

	#define PAJ_IRQ_VECTOR 			PAJ_IRQ_V0(PAJ_INT_NR)

#endif


//---------------------------------------------------------------------------- koniec makr roboczych na potrzeby ustalania przerwa�














#define NORMAL_MODE_120_FPS		0
#define FAST_MODE_240_FPS		1




#define PAJ7620_ADDR	  		0xE6



#define CHIP_ID					0x7620


// REGISTER BANK SELECT
#define PAJ7620_REGITER_BANK_SEL		0xEF	//W

// REGISTER BANK 0
#define PAJ7620_ADDR_PART_ID_LOW		0x00	//R
#define PAJ7620_ADDR_PART_ID_HIGH		0x01	//R
#define PAJ7620_ADDR_VERSION_ID			0x01	//R

#define PAJ7620_ADDR_SUSPEND_CMD		0x03	//W
#define PAJ7620_ADDR_GES_PS_DET_MASK_0	0x41	//RW
#define PAJ7620_ADDR_GES_PS_DET_MASK_1	0x42	//RW
#define PAJ7620_ADDR_GES_PS_DET_FLAG_0	0x43	//R
#define PAJ7620_ADDR_GES_PS_DET_FLAG_1	0x44	//R
#define PAJ7620_ADDR_STATE_INDICATOR	0x45	//R
#define PAJ7620_ADDR_PS_HIGH_THRESHOLD	0x69	//RW
#define PAJ7620_ADDR_PS_LOW_THRESHOLD	0x6A	//RW
#define PAJ7620_ADDR_PS_APPROACH_STATE	0x6B	//R
#define PAJ7620_ADDR_PS_RAW_DATA		0x6C	//R

// REGISTER BANK 1
#define PAJ7620_ADDR_PS_GAIN			0x44	//RW
#define PAJ7620_ADDR_IDLE_S1_STEP_0		0x67	//RW
#define PAJ7620_ADDR_IDLE_S1_STEP_1		0x68	//RW
#define PAJ7620_ADDR_IDLE_S2_STEP_0		0x69	//RW
#define PAJ7620_ADDR_IDLE_S2_STEP_1		0x6A	//RW
#define PAJ7620_ADDR_OP_TO_S1_STEP_0	0x6B	//RW
#define PAJ7620_ADDR_OP_TO_S1_STEP_1	0x6C	//RW
#define PAJ7620_ADDR_OP_TO_S2_STEP_0	0x6D	//RW
#define PAJ7620_ADDR_OP_TO_S2_STEP_1	0x6E	//RW
#define PAJ7620_ADDR_OPERATION_ENABLE	0x72	//RW

// PAJ7620_REGITER_BANK_SEL
#define PAJ7620_BANK0					0x00
#define PAJ7620_BANK1					0x01

// PAJ7620_ADDR_SUSPEND_CMD
#define PAJ7620_I2C_WAKEUP				0x01
#define PAJ7620_I2C_SUSPEND				0x00

// PAJ7620_ADDR_OPERATION_ENABLE
#define PAJ7620_ENABLE					0x01
#define PAJ7620_DISABLE					0x00


#define BANK0							0
#define BANK1							1




#define INIT_REG_ARRAY_SIZE 			(sizeof(initRegisterArray)/sizeof(initRegisterArray[0]))


// czasy reakcji czujnika, domy�lnie fps_120
typedef enum { fps_120, fps_240 } TFPS;

// numery 9 gest�w wykrywanych sprz�towo
typedef enum { gs_none=0, gs_right=1, gs_left=2, gs_up=3, gs_down=4, gs_forward=5, gs_backward=6, gs_cw=7, gs_ccw=8, gs_wave=9 } TGSNR;

// domy�lne filtry gest�w w bibliotece
typedef enum {
	gf_none 			= 0b0000000000000000,	// filters OFF
	gf_right			= 0b0000000000000001,	// only RIGHT
	gf_left				= 0b0000000000000010,	// only LEFT
	gf_up				= 0b0000000000000100,	// only UP
	gf_down				= 0b0000000000001000,	// only DOWN
	gf_forward			= 0b0000000000010000,	// only FORWARD
	gf_backward			= 0b0000000000100000,	// only BACKWARD
	gf_cw				= 0b0000000001000000,	// only CW
	gf_ccw				= 0b0000000010000000,	// only CCW
	gf_wave				= (1<<(gs_wave-1)),	// only WAVE
} TGESTUREFILTER;


/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
/* ----------------------------------------------- CALLBACKI i ich ARGUMENTY - do w�asnych funkcji u�ytkownika ---------- */
/* ...................................................................................................................... */

// g��wny callback do rozpoznawania gest�w
/* zwraca u�ytkownikowi 3 ostatnie gesty je�li wyst�pi�y
 * 		first_gs - to najstarszy gest
 * 		second_gs - to kolejny, kt�ry wyst�pi� po najstarszym
 * 		last_gs - to ostanio wykonany gest czyli zawsze bie��cy
 */
typedef void (*TGESTURE)( TGSNR last_gs, TGSNR second_gs, TGSNR first_gs );	/* !!!!!!!!! G��WNY (PODSTAWOWY) CALLBACK dla u�ytkownika !!!!!!!! */


#if USE_CIRCLE_CONTINOUS == 1
	// callback do funkcjonalno�ci ala "palec as enkoder" ;)
	/* zwraca u�ytkownikowi numereg gestu CW albo CCW oraz licznik/counter w postaci wska�nika
	 * dzi�ki czemu u�ytkownik mo�e wp�ywa� na jego warto�� "w locie" */
	typedef void (*TCIRCLECONTINOUS)( TGSNR gs_cc, int * cnt );
#endif


#if USE_INIT_GESTURE == 1
	// callback po wykryciu gestu odblokowuj�cego rozpoznawanie innych gest�w
	/* zwraca u�ytkownikowi wska�nik na Timeout wy��czenia rozpoznawania gest�w, u�ytkownik mo�e go modyfikowa� "w locie"
	 * drugi argument to wska�nik na filtr gest�w po inicjalizacji rozpoznawania - u�ytkownik mo�e w��czy� rozpoznawanie
	 * tylko cz�ci wybranych przez siebie gest�w. Domy�lnie rozpoznawane s� wszystkie podstawowe */
	typedef void (*TINITGESTURE_START)( uint16_t * ms, TGESTUREFILTER * gf_start_filter );


	// callback po up�yni�ciu czasu (timeout) i przej�cie w tryb gestu inicjuj�cego/odblokowuj�cego
	/* zwraca u�ytkownikowi wska�nik na filtr gest�w po zablokowaniu. Domy�lnie filtr wy��cza wszystkie gesty - ale
	 * u�ytkownik mo�e w wyj�tkowych okazjach, gdy to potrzebne - odblokowa� sobie filtrem niekt�re z nich */
	typedef void (*TINITGESTURE_END)( TGESTUREFILTER * gf_end_filter );
#endif

/* ...................................................................................................................... */










//**************************** Nag��wki funkcji dost�pnych dla u�ytkownika biblioteki **************************************


/* inicjalizacja pracy czujnika gest�w */
extern uint8_t paj7620_init( TFPS fps );

/* Rejestracja w�asnego callbacka wywo�ywanego po ka�dym rozpoznaniu jednego z podstawowych 9 gest�w
 *
 * 		Argumenty:
 * 		1. w�asny callback - w�asna funckja zwrotna
 * 		2. wska�nik na timer programowy o podstawie czasu 10ms
 */
extern void register_gesture_callback( TGESTURE callback, volatile uint16_t * gs_timer );

/* g��wne zdarzenie (EVENT) kt�re nale�y zawsze wywo�ywa� w p�tli g��wnej programu */
extern void PAJ7620_EVENT( void );

/* funkcja do zmiany filtr�w gest�w */
extern void gs_set_filter( TGESTUREFILTER filter );


#if USE_CIRCLE_CONTINOUS == 1
	/* je�li chcemy korzysta� ze zdarzenia ci�g�ego kr�cenia k�ka w prawo lub w lewo. WYMAGANY oddzielny timer programowy
	 *
	 * 		Argumenty:
	 * 		1. w�asny callback - wywo�ywany niezale�nie od pojedynczych gest�w CW i CCW, kt�re dzia�aj� na przerwaniach
	 * 		2. wska�nik na timer programowy o podstawie czasu 10ms
	 * 		3. warto�� inicjuj�ca licznik, kt�ry jest p�niej przekazywany do callbacka (daj�cy funkcjonalno�� enkodera)
	 */
	extern void register_circle_continous_callback( TCIRCLECONTINOUS callback, volatile uint16_t * cc_timer, int cnt_start_val );

	/* je�li chcemy programowo wy��czy� na jaki� czas niezale�ne zdarzenia ci�g�ego kr�cenia k�ka, lub je w��czy�
	 *
	 * 		Argumenty:
	 * 		0 - zablokowanie zdarze� od kr�cenia k�ek CW i CCW
	 * 		1 - odblokowanie zdarze� kr�cenia k�ek CW i CCW (domy�lnie odblokowane po inicjalizacji)
	 */
	void enable_gs_circle_continous_mode( uint8_t enable );
#endif

#if USE_INIT_GESTURE == 1
	/* inicjalizacja - tzw GESTU INICJUJ�CEGO prac� rozpoznawania kolejnych gest�w (rejestracja callbacka_
	 *
	 * 		Argumenty:
	 * 		1. w�asny callback - wywo�ywany w momencie wykryca gestu inicjuj�cego, mo�na pomin�� podaj�c warto�� 0 (NULL)
	 * 		2. filtr gestu np gf_wave (mo�na ��czy� kilka filtr�w)
	 * 		3. wska�nik na timer programowy o podstawie czasu 10ms
	 * 		4. czas w ms na Timeout po kt�rym ko�czy si� wykrywanie gest�w i nast�uje powr�t do stanu nas�uchiwania gestu inicjuj�cego
	 *
	 * 		(callback mo�e by� przydatny np do �wietlnej albo d�wi�kowej sygnalizacji startu rozpoznawania gest�w)
	 */
	extern void register_init_gs_start_callback( TINITGESTURE_START callback, TGESTUREFILTER gf_init, volatile uint16_t * init_tmr, uint16_t ms_interval );

	/* rejestracja callbacka - wywo�ywanego w momencie gdy ko�czy si� czas rozpoznawania gest�w i nast�uje powr�t do stanu nas�uchiwania gestu inicjuj�cego
	 *
	 * 		Argumwenty:
	 * 		1. w�asny callback
	 *
	 * 		rejestracj� mo�na zupe�nie pomin�� je�li nie chcemy nic robi� w momencie ko�ca czasu rozpoznawania gest�w
	 * 		(mo�e by� przydatny np do �wietlnej albo d�wi�kowej sygnalizacji ko�ca rozpoznawania gest�w)
	 */
	extern void register_init_gs_end_callback( TINITGESTURE_END callback );
#endif


#endif /* MK_GESTURE_PAJ7620_MK_PAJ7620_H_ */
