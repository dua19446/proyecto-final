/*
 * File:   pro2.c
 * Author: duart
 *
 * Created on 20 de mayo de 2021, 10:17 AM
 */
#include <xc.h>
#include <stdint.h>

//------------------------------------------------------------------------------
//                         BITS DE CONFIGURACION
//------------------------------------------------------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO 
                                   //oscillator: I/O function on RA6/OSC2/CLKOUT
                                   //pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR 
                                //pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
                                //protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code
                                //protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
                                //(Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor Enabled bit 
                                //(Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3/PGM 
                                //pin has PGM function, low voltage programming 
                                //enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out 
                                //Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
                                //(Write protection off)

#define _XTAL_FREQ 8000000//Para usar la funcion de 'delay'

//------------------------------------------------------------------------------
//                                VARIABLES
//------------------------------------------------------------------------------

int M = 0;

//------------------------------------------------------------------------------
//                          PROTOTIPOS FUNCIONES 
//------------------------------------------------------------------------------
void setup(void); //configuraciones generales 
// Servo para el movimiento de ojos para arriba y abajo
void servo1(void);
void servo1_2(void);
void servo1_3(void);
void servo1_4(void);
void servo1_5(void);
// Servo para el movimiento de ojos de izquierda a derecha
void servo2(void);
void servo2_2(void);
void servo2_3(void);
void servo2_4(void);
void servo2_5(void);
// Servo para para movimiento de la ceja 1
void servo3(void);
void servo3_2(void);
void servo3_3(void);
void servo3_4(void);
void servo3_5(void);
// Servo para para movimiento de la ceja 2
void servo4(void);
void servo4_2(void);
void servo4_3(void);
void servo4_4(void);
void servo4_5(void);

// Se establece el vector de interrupcion
void __interrupt() isr(void){
    
    if (PIR1bits.ADIF == 1)//Interrupcion del ADC 
    {
        if (ADCON0bits.CHS == 0)//si se esta en este canal que haga lo siguiente
        {
            ADCON0bits.CHS = 1;
            if (PORTDbits.RD0 == 0)
            {
                CCPR1L = 0;
                CCPR2L = ADRESH;
            }                     
            else if (PORTDbits.RD1 == 0)
            {
                CCPR1L = ADRESH;
                CCPR2L = 0;
            }  
            else if(PORTDbits.RD2 == 0)
            {
                CCPR1L = 0;
                CCPR2L = 0;
            }    
        }
        else if (ADCON0bits.CHS == 1)
        {
            M = ADRESH;
            ADCON0bits.CHS = 2;
            if(M<=50)
            {
               servo1();
            }
            if((M<=101)&&(M>=51))
            {
               servo1_2();
            }
            if((M<=152)&&(M>=102))
            {
               servo1_3();
            }
            if((M<=203)&&(M>=153))
            {
               servo1_4();
            }
            if(M>=204)
            {
               servo1_5();
            }
        }
        else if (ADCON0bits.CHS == 2)
        {
            M = ADRESH;
            ADCON0bits.CHS = 3;
            if(M<=50)
            {
               servo2();
            }
            if((M<=101)&&(M>=51))
            {
               servo2_2();
            }
            if((M<=152)&&(M>=102))
            {
               servo2_3();
            }
            if((M<=203)&&(M>=153))
            {
               servo2_4();
            }
            if(M>=204)
            {
               servo2_5();
            }
        }
        else if (ADCON0bits.CHS == 3)
        {
            M = ADRESH;
            ADCON0bits.CHS = 4;
            if(M<=50)
            {
               servo3();
            }
            if((M<=101)&&(M>=51))
            {
               servo3_2();
            }
            if((M<=152)&&(M>=102))
            {
               servo3_3();
            }
            if((M<=203)&&(M>=153))
            {
               servo3_4();
            }
            if(M>=204)
            {
               servo3_5();
            }
        }
        else if (ADCON0bits.CHS == 4)
        {
            M = ADRESH;
            ADCON0bits.CHS = 0;
            if(M<=50)
            {
               servo4();
            }
            if((M<=101)&&(M>=51))
            {
               servo4_2();
            }
            if((M<=152)&&(M>=102))
            {
               servo4_3();
            }
            if((M<=203)&&(M>=153))
            {
               servo4_4();
            }
            if(M>=204)
            {
               servo4_5();
            }
        }
        __delay_us(50);//tiempo necesario para el cambio de canal 
        PIR1bits.ADIF = 0;//Se apaga el valor de la bandera de interrupcion ADC
    }
}

//------------------------------------------------------------------------------
//                             CICLO PRINCIPAL 
//------------------------------------------------------------------------------
void main(void) {
    
    setup();// Se llama a la funcion setup para configuracion de I/O
            
    while (1) // Se implemta el loop
    {
            ADCON0bits.GO = 1; //para empezar de nuevo la ejecucion del ADC
}
}
//------------------------------------------------------------------------------
//                             CONFIGURACIONES
//------------------------------------------------------------------------------
void setup(void){
    // configuracion de puertos 
    ANSEL = 0b00011111; //setea AN0 y AN1
    ANSELH = 0X00;//se establecen los pines como entras y salidas digitales
    
    TRISA = 0xff; // Se pone todo el puerto A como entrada.
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD1 = 1;
    TRISDbits.TRISD2 = 1;//Se ponen como entradas los primeros pines del puertoB
    
    PORTA = 0X00;
    PORTD = 0X00;
    PORTB = 0X00;
    PORTC = 0X00;//Se limpian los puertos utilizados
    
    // configuracion del oscilador 
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1; //Se configura el oscilador a una frecuencia de 8 MHz
    OSCCONbits.SCS = 1;
    
    // configuracion del timer 0 y pull-up internos
//
//    OPTION_REGbits.nRBPU = 0;
//    WPUB = 0b00000111;
//    IOCBbits.IOCB0 = 1;
//    IOCBbits.IOCB1 = 1;
//    IOCBbits.IOCB2 = 1;
   
    // configuracion del ADC
  
    ADCON0bits.CHS = 0; // CANAL AN0
    
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS0 = 0; //Fosc/32
    
    ADCON0bits.ADON = 1; //Activa el modulo ADC
    
    ADCON1bits.ADFM = 0; // justificacion a la izquierda.
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;  //Vss y Vcc
    
    // configuracion del PWM
    TRISCbits.TRISC2 = 1; //CCP1 se establece como entrada
    TRISCbits.TRISC1 = 1; //CCP2 se establece como entrada 
    PR2 = 250; // periodo de 2 ms para el timer 2
    CCP1CONbits.P1M = 0; // se configura como single output
    CCP1CONbits.CCP1M = 0b00001100; // Modo PWM
    CCP2CONbits.DC2B0 = 0;
    CCP2CONbits.DC2B1 = 0; // Modo PWM
    CCP2CONbits.CCP2M = 0b1111;// Modo PWM
    
    CCPR1L = 0x0f;  //Ciclo de trabajo inicial 
    CCP1CONbits.DC1B = 0; //bits menos significativos
    CCPR2L = 0x0f; //Ciclo de trabajo inicial
    
    PIR1bits.TMR2IF = 0; 
    T2CONbits.T2CKPS = 0b11; //Pre-scaler de 1:16
    T2CONbits.TMR2ON = 1; //Se prende timer 2
    
    while (PIR1bits.TMR2IF == 0);// Se espera ciclo del timer2
    PIR1bits.TMR2IF = 0;
    
    TRISCbits.TRISC2 = 0; //se pone como salida CCP1
    TRISCbits.TRISC1 = 0; //se pone como salida CCP2
    
    // configuracion de interrupciones 
    INTCONbits.GIE = 1;
    PIR1bits.ADIF = 0; // BANDERA de interrupcion del ADC
    PIE1bits.ADIE = 1; // Habilita la interrupcion del ADC
    INTCONbits.PEIE = 1; // Interrupcion de los perifericos
}

void servo1(void)
{
            PORTBbits.RB0 = 1;
            __delay_ms(0.7);
            PORTBbits.RB0 = 0;
            __delay_ms(19.3);
}
void servo1_2(void)
{
            PORTBbits.RB0 = 1;
            __delay_ms(1.25);
            PORTBbits.RB0 = 0;
            __delay_ms(18.75);
}
void servo1_3(void)
{
            PORTBbits.RB0 = 1;
            __delay_ms(1.5);
            PORTBbits.RB0 = 0;
            __delay_ms(18.5);
}
void servo1_4(void)
{
            PORTBbits.RB0 = 1;
            __delay_ms(1.75);
            PORTBbits.RB0 = 0;
            __delay_ms(18.25);
}
void servo1_5(void)
{
            PORTBbits.RB0 = 1;
            __delay_ms(2);
            PORTBbits.RB0 = 0;
            __delay_ms(17);
}

void servo2(void)
{
            PORTBbits.RB1 = 1;
            __delay_ms(0.7);
            PORTBbits.RB1 = 0;
            __delay_ms(19.3);
}
void servo2_2(void)
{
            PORTBbits.RB1 = 1;
            __delay_ms(1.25);
            PORTBbits.RB1 = 0;
            __delay_ms(18.75);
}
void servo2_3(void)
{
            PORTBbits.RB1 = 1;
            __delay_ms(1.5);
            PORTBbits.RB1 = 0;
            __delay_ms(18.5);
}
void servo2_4(void)
{
            PORTBbits.RB1 = 1;
            __delay_ms(1.75);
            PORTBbits.RB1 = 0;
            __delay_ms(18.25);
}
void servo2_5(void)
{
            PORTBbits.RB1 = 1;
            __delay_ms(2);
            PORTBbits.RB1 = 0;
            __delay_ms(17);
}

void servo3(void)
{
            PORTBbits.RB2 = 1;
            __delay_ms(0.7);
            PORTBbits.RB2 = 0;
            __delay_ms(19.3);
}
void servo3_2(void)
{
            PORTBbits.RB2 = 1;
            __delay_ms(1.25);
            PORTBbits.RB2 = 0;
            __delay_ms(18.75);
}
void servo3_3(void)
{
            PORTBbits.RB2 = 1;
            __delay_ms(1.5);
            PORTBbits.RB2 = 0;
            __delay_ms(18.5);
}
void servo3_4(void)
{
            PORTBbits.RB2 = 1;
            __delay_ms(1.75);
            PORTBbits.RB2 = 0;
            __delay_ms(18.25);
}
void servo3_5(void)
{
            PORTBbits.RB2 = 1;
            __delay_ms(2);
            PORTBbits.RB2 = 0;
            __delay_ms(17);
}

void servo4(void)
{
            PORTBbits.RB3 = 1;
            __delay_ms(0.7);
            PORTBbits.RB3 = 0;
            __delay_ms(19.3);
}
void servo4_2(void)
{
            PORTBbits.RB3 = 1;
            __delay_ms(1.25);
            PORTBbits.RB3 = 0;
            __delay_ms(18.75);
}
void servo4_3(void)
{
            PORTBbits.RB3 = 1;
            __delay_ms(1.5);
            PORTBbits.RB3 = 0;
            __delay_ms(18.5);
}
void servo4_4(void)
{
            PORTBbits.RB3 = 1;
            __delay_ms(1.75);
            PORTBbits.RB3 = 0;
            __delay_ms(18.25);
}
void servo4_5(void)
{
            PORTBbits.RB3 = 1;
            __delay_ms(2);
            PORTBbits.RB3 = 0;
            __delay_ms(17);
}
//void ant1(void)
//{
//    while(PORTBbits.RB0 == 0);
//    __delay_ms(100){}
//    return;
//}
//void ant2(void)
//{
//    while(PORTBbits.RB1 == 0);
//    __delay_ms(100){}
//    return;
//}
//void ant3(void)
//{
//    while(PORTBbits.RB2 == 0);
//    __delay_ms(100){}
//    return;
//}