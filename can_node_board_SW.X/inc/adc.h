/* 
 * File: ccp.h
 * Author: Abdullah Almosalami
 * Comments: This, along with the accompanying .c file, is meant to be my
 *           implementation of driving the ADC peripheral.
 * 
 * TODO:
 *      1- 
 * 
 * Revision history: 0.1
 * Started: November 15, 2021 9:33PM
 */

/* ----------------------------------------------------------------------------
 * Overview Description of the ADC module in the PIC18F4620
 * ----------------------------------------------------------------------------
 * The ADC on the PIC18F4620 has 10-bit resolution and utilizes successive approximation.
 * It's transfer function includes 0 (i.e., rounding-symmetric).
 * 
 * There are 13 possible channels
 * to read from:
 * ----AN0 : RA0----AN1 : RA1----AN2 : RA2----AN3 : RA3----AN4 : RA4----AN5 : RE0
 * ----AN6 : RE1----AN7 : RE2----AN8 : RB2----AN9 : -  ----AN10 : RB1----AN11 : RB4
 * ----AN12 : RB0
 * 
 * There are five registers for this module:
 *      1 & 2- A/D Result High Byte (ADRESH) and Low Byte (ADRESL)
 *      3- ADCON0 --> Controls operation of ADC
 *      4- ADCON1 --> Configures pins for ADC
 *      5- ADCON2 --> Configure ADC clock source, acquisition time, and justification
 * See below in the "REGISTERS & MACROS" section for outline of the registers.
 * 
 * The ADC has the unique feature of being able to operate while in Sleep mode;
 * however, it needs to be using the A/D's internal RC oscillator.
 * 
 * The following are the general steps to perform an A/D conversion:
 *      1- Configure the A/D module
 *          - Configure analog pins, voltage reference, and I/O (ADCON1)
 *          - Select A/D input channel (ADCON0)
 *          - Select A/D acquisition time (ADCON2)
 *          - Select A/D conversion clock (ADCON2)
 *          - Turn on A/D module (ADCON0)
 *
 *      2- Configure A/D interrupt
 *          - Clear ADIF bit
 *          - Set ADIE bit
 *          - Set GIE bit
 * 
 *      3- Wait the required acquisition time (letting Chold charge up)
 *      4- Start conversion by setting the GO/~DONE bit (ADCON0)
 *      5- Await A/D conversion by either polling the GO/~DONE bit or handling in
 *         A/D ISR (when ADIF gets set at the end of a conversion)
 *      6- Read A/D result registers and clear ADIF
 *      7- Before starting another conversion, a minimum of 2 TAD is required before
 *         the next acquisition
 * 
 * In order for the A/D to convert correctly, you need to wait enough time for the
 * charge holding capacitor to fully charge. Depending on the source impedance of
 * the analog source Rs (as well as the internal sampling switch impedance Rss).
 * this amount of time may differ. Furthermore, the source impedance Rs affects
 * the voltage offset at the input since there is input leakage current through
 * the A/D pin (approx +-100nA). Accordingly, the maximum recommended source
 * impedance for the ADC is 2.5k.
 * 
 * This is why "acquisition time" is defined - this is the time given for the
 * charging capacitor to charge up! Once you start a conversion by setting the
 * GO/~DONE bit, the sample-hold switch is disconnected and whatever voltage
 * the hold capacitor has will be converted into the 10-bit code. Refer to page
 * 230 for the necessary calculations of acquisition time. For example, with
 * source impedance of 2.5k, max conversion error of 1/2LSB, VDD = 5V --> Rss = 2k,
 * temperature = 85C max, the capacitor charging time is ~1.2us and temperature
 * coefficient is 1.05us, so overall, minimum TACQ = 2.4us. NOTE that this is just
 * the acquisition time. The conversion time comes on top of this, which is 11TAD.
 * When you set an acquisition time within ADCON2 with the ACQT bits, then you
 * won't need to actually manually wait for the acquisition time as this will be
 * done automatically when the GO/~DONE bit is set. If you do not set ACQT, then
 * you need to manually ensure the correct amount of acquisition time was done.
 * 
 * For selecting the clock source, you ideally would like the fastest possible
 * acquisition time (so smallest TAD) but you will need to ensure that TAD
 * is at least as large as the minimum acquisition time (--> what about in manual
 * mode ...?). For example, with Fosc at 40 MHz (which I am using), then TAD
 * needs to be either 32 Tosc or 64 Tosc (i.e., 800ns or 1600ns). Overall, assuming
 * a minimum acquisition time of 2.4us and a TAD of 800ns, the maximum sample rate
 * of the ADC is ~89kHz (11.2us period).
 * 
 * Finally, you can configure the ADC to convert with a CCP2 Special Trigger
 * Event (in Compare mode). This requires CCP2CON CCP2M bits to be 0xB. Under such
 * operation, you will have a CCP2 interrupt clear the CCP2IF and ANOTHER interrupt
 * for the ADC to handle when it has completed a conversion, as the overall
 * sample time may be quite long compared to the instruction clock rate.
 * 
 */


#ifndef ADC_HEADER
#define	ADC_HEADER

#include <xc.h>
#include <stdint.h>

#ifndef ENABLE_PERIPHERAL_INTERRUPTS
#define ENABLE_PERIPHERAL_INTERRUPTS    (INTCONbits.PEIE = 1u)
#define DISABLE_PERIPHERAL_INTERRUPTS   (INTCONbits.PEIE = 0u)
#endif


// <editor-fold defaultstate="collapsed" desc="DEFINITIONS: REGISTERS & MACROS">

// REGISTER OUTLINE
/* ADCON0 - A/D Control Register 0 -  Register 19-1 in Datasheet
 * Default/POR: 00 0000 0 0
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |..........undef..........|.....................CHS.......................|.GO/~DONE..|...ADON....|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains:
 *      (R/W) CHS : Analog Channel Select bits (0x0 to 0xC and beyond that unimplemented)
 *      (R/W) GO/~DONE : When ADON = 1 (ADC active), this shows whether a ADC conversion is
 *                       done or not (0 for done, 1 for in-progress)
 *      (R/W) ADON : Enable/Disable for ADC
 * 
 */
#define ADCON0_CHS(x)           (ADCON0bits.CHS = (x))
#define ADCON0_CHS_DEFAULT      (ADCON0bits.CHS = 0x0u)     // AN0 = RA0
#define ADC_TRIS_DEFAULT        (TRISAbits.RA0 = 1u)        // RA0 as input
#define ADC_START_CONVERSION    (ADCON0bits.GO = 1u)
#define ADC_IN_PROGRESS         (ADCON0bits.GO_NOT_DONE)
#define ADC_ENABLE              (ADCON0bits.ADON = 1u)
#define ADC_DISABLE             (ADCON0bits.ADON = 0u)

/* ADCON1 - A/D Control Register 1 -  Register 19-2 in Datasheet
 * Default/POR: 00 00 0qqq --> q as in, determined by configuration bits --> PCFG of PBADEN Config reg
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |..........undef..........|.........VCFG..........|.....................PCFG......................|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains:
 *      (R/W) VCFG : Voltage reference configuration bits
 *                  - VCFG1 : 1 = VREF-, 0 = VSS
 *                  - VCFG0 : 1 = VREF+, 0 = VDD
 *      (R/W) PCFG : A/D Port Configuration bits (see datasheet for table)
 * 
 */
#define ADC_USE_VREF        (ADCON1bits.VCFG = 0x3)
#define ADC_DONT_USE_VREF   (ADCON1bits.VCFG = 0x0)
#define ADC_PCFG_DEFAULT    (ADCON1bits.PCFG = 0xA)
// TODO: Convenient PCFG macros

/* ADCON2 - A/D Control Register 2 -  Register 19-3 in Datasheet
 * Default/POR: 0 0 000 000
 * +-------------------------------------------------------------------------------------------------+
 * |    bit 7   |    bit 6   |   bit 5   |   bit 4   |   bit 3   |   bit 2   |   bit 1   |   bit 0   |
 * +-------------------------------------------------------------------------------------------------+
 * |...ADFM.....|...undef....|...............ACQT................|...............ADCS................|
 * +-------------------------------------------------------------------------------------------------+
 * This register contains:
 *      (R/W) ADFM : A/D Result format select bit
 *              1 = right justified, 0 = left justified
 *      (R/W) ACQT : A/D Acquisition Time Select bits
 *              111 = 20 TAD
 *              110 = 16 TAD
 *              101 = 12 TAD
 *              100 = 8 TAD
 *              011 = 6 TAD
 *              010 = 4 TAD
 *              001 = 2 TAD
 *              000 = 0 TAD --> If the A/D FRC clock source is used, a delay of one TCY (instruction cycle)
 *                              is added before the A/D clock starts
 *      (R/W) ADCS : A/D Conversion Clock Select bits
 *              111 = FRC
 *              110 = FOSC/64
 *              101 = FOSC/16
 *              100 = FOSC/4
 *              011 = FRC
 *              010 = FOSC/32
 *              001 = FOSC/8
 *              000 = FOSC/2
 * 
 */
#define ADC_RESULT_RIGHT_JUSTIFY        (ADCON2bits.ADFM = 1u)
#define ADC_RESULT_LEFT_JUSTIFY         (ADCON2bits.ADFM = 0u)
#define ADC_ACQ_TIME(x)                 (ADCON2bits.ACQT = (x))     // Use with adc_acqt_t enum
// TODO
#define ADC_ACQ_TIME_DEFAULT            (ADCON2bits.ACQT = ADC_ACQT_4TAD)    // 4 TAD
#define ADC_CLOCK_SOURCE(x)             (ADCON2bits.ADCS = (x))     // Use with adc_clk_t enum
// TODO
#define ADC_CLOCK_SOURCE_DEFAULT        (ADCON2bits.ADCS = (ADC_CLK_FOSC64))     // FOSC/64

// Using the ADC with CCP2...
#define ADC_WITH_CCP2                   (CCP2CONbits.CCP2M = 0xB)   // 1011 for CCP2 special trigger event

// Interrupt related
#define ADC_INTERRUPT_ENABLE            (PIE1bits.ADIE = 1u)
#define ADC_INTERRUPT_DISABLE           (PIE1bits.ADIE = 0u)
#define ADC_CLEAR_IF                    (PIR1bits.ADIF = 0u)
// TODO: Priority stuff...


// </editor-fold>


// Enumerations
typedef enum { ADC_ACQT_0TAD, ADC_ACQT_2TAD, ADC_ACQT_4TAD, ADC_ACQT_6TAD, ADC_ACQT_8TAD, ADC_ACQT_12TAD, ADC_ACQT_16TAD, ADC_ACQT_20TAD } adc_acqt_t;
typedef enum { ADC_CLK_FOSC2=0u, ADC_CLK_FOSC4=4u, ADC_CLK_FOSC8=1u, ADC_CLK_FOSC16=5u, ADC_CLK_FOSC32=2u, ADC_CLK_FOSC64=6u, ADC_CLK_FRC=3u } adc_clk_t;
typedef enum { ADC_AN0, ADC_AN1, ADC_AN2, ADC_AN3, ADC_AN4, ADC_AN5, ADC_AN6, ADC_AN7, ADC_AN8, ADC_AN9, ADC_AN10, ADC_AN11, ADC_AN12 } adc_pin_t;
typedef enum { ADC_VREF, ADC_NOT_VREF } adc_vref_t;

// Function declarations
void adc_init(adc_pin_t adc_pin, adc_vref_t adc_vref, adc_acqt_t adc_acqt, adc_clk_t adc_clk);
void adc_init_default(void);
void adc_use_ccp2(void);

#endif	/* ADC_HEADER */

