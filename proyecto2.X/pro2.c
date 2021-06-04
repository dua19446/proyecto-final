/*
 * File:   pro2.c
 * Author: duart
 *
 * Created on 20 de mayo de 2021, 10:17 AM
 */
#include <xc.h>
#include <stdint.h>
#include <stdio.h>// Libreria para poder usar printf junto a la funcion putch.

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
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
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

char M = 0;
char M1 = 0;
char M2 = 0;
char M3 = 0;
char M4 = 0;
char M5 = 0;
char M6 = 0;
char M7 = 0;
char sentido;
char sentido2;
int uart;
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
// Para la ventana terminal 
void menu(void);
void putch(char data);
void receptar(void);
// para leer y escribir EEPROM
char leer_eeprom(char direccion);
void escribir_eeprom(char data, char direccion);
void nuevo_bit_banging(void);

// Se establece el vector de interrupcion
void __interrupt() isr(void){
    
    if (PIR1bits.ADIF == 1)//Interrupcion del ADC 
    {
        if (ADCON0bits.CHS == 0)//si se esta en este canal que haga lo siguiente
        {
            ADCON0bits.CHS = 1;
            if (PORTDbits.RD0 == 0)
            { 
                sentido = 0b01; 
                CCPR1L = 0;
                CCPR2L = ADRESH/4;
            }                     
            else if (PORTDbits.RD1 == 0)
            {
                sentido = 0b10; 
                CCPR1L = ADRESH/4;
                CCPR2L = 0;
            }  
            else if(PORTDbits.RD2 == 0)
            {
                sentido = 0b00; 
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
            M1 = ADRESH;
            ADCON0bits.CHS = 3;
            if(M1<=50)
            {
               servo2();
            }
            if((M1<=101)&&(M1>=51))
            {
               servo2_2();
            }
            if((M1<=152)&&(M1>=102))
            {
               servo2_3();
            }
            if((M1<=203)&&(M1>=153))
            {
               servo2_4();
            }
            if(M1>=204)
            {
               servo2_5();
            }
        }
        else if (ADCON0bits.CHS == 3)
        {
            M2 = ADRESH;
            ADCON0bits.CHS = 4;
            if(M2<=50)
            {
               servo3();
            }
            if((M2<=101)&&(M2>=51))
            {
               servo3_2();
            }
            if((M2<=152)&&(M2>=102))
            {
               servo3_3();
            }
            if((M2<=203)&&(M2>=153))
            {
               servo3_4();
            }
            if(M2>=204)
            {
               servo3_5();
            }
        }
        else if (ADCON0bits.CHS == 4)
        {
            M3 = ADRESH;
            ADCON0bits.CHS = 0;
            if(M3<=50)
            {
               servo4();
            }
            if((M3<=101)&&(M3>=51))
            {
               servo4_2();
            }
            if((M3<=152)&&(M3>=102))
            {
               servo4_3();
            }
            if((M3<=203)&&(M3>=153))
            {
               servo4_4();
            }
            if(M3>=204)
            {
               servo4_5();
            }
        }
        __delay_us(50);//tiempo necesario para el cambio de canal 
        PIR1bits.ADIF = 0;//Se apaga el valor de la bandera de interrupcion ADC
    }
    if (RBIF == 1)// Interrupcion por la bandera del puerto B
    {
        if (PORTBbits.RB4 == 0)
        {
            RE0 = 1;
            escribir_eeprom(M, 0x14);
            escribir_eeprom(M1, 0x15);
            escribir_eeprom(M2, 0x16);
            escribir_eeprom(M3, 0x17);
            escribir_eeprom(sentido, 0x18);
            __delay_ms(1000);
            RE0 = 0;
        }                     
        if (PORTBbits.RB5 == 0)
        {
            RE1 = 1;
            M4 = leer_eeprom(0x14);
            M5 = leer_eeprom(0x15);
            M6 = leer_eeprom(0x16);
            M7 = leer_eeprom(0x17);
            sentido2 = leer_eeprom(0x18);
            nuevo_bit_banging();
            __delay_ms(1000);
            RE1 = 0;
        }                     
        if (PORTBbits.RB6 == 0)
        {
            RE2 = 1;
            uart = 1;
            while (uart == 1)
            {
            menu();//Se llama a la funcion menu para desplegar el menu de opciones
            }
            RE2 = 0;
        }                     
        INTCONbits.RBIF = 0;// Se limpia la bandera de la interrupcion del 
    }                       // puerto B
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
    TRISBbits.TRISB3 = 0;// para potenciometros
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 1;
    TRISBbits.TRISB6 = 1;// Se ponen como entradas para botones para estados.
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD1 = 1;
    TRISDbits.TRISD2 = 1;//Se ponen como entradas los primeros pines del puertoB
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    
    PORTA = 0X00;
    PORTD = 0X00;
    PORTB = 0X00;
    PORTE = 0X00;
    PORTC = 0X00;//Se limpian los puertos utilizados
    
    // configuracion del oscilador 
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1; //Se configura el oscilador a una frecuencia de 8 MHz
    OSCCONbits.SCS = 1;
    
    // configuracion del pull-up internos

    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b01110000;
    IOCBbits.IOCB4 = 1;
    IOCBbits.IOCB5 = 1;
    IOCBbits.IOCB6 = 1;
   
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
    
    //Configuracion TX y RX 
    TXSTAbits.BRGH = 1;  // Para alta velocidad.
    BAUDCTLbits.BRG16 = 1; // Se usan los 16 bits
    
    TXSTAbits.SYNC = 0; // transmision asincrona
    RCSTAbits.SPEN = 1; // Se enciende el modulo 
    RCSTAbits.CREN = 1; // Se abilita la recepcion 
    
    TXSTAbits.TXEN = 1; // Se abilita la transmision 
    
    RCSTAbits.RX9 = 0; // Se determina que no se quieren 9 bits
    
    SPBRG = 207; //BAUD RATE de 9600
    SPBRGH = 0;
    
    // configuracion de interrupciones 
    INTCONbits.GIE = 1;
    PIR1bits.ADIF = 0; // BANDERA de interrupcion del ADC
    PIE1bits.ADIE = 1; // Habilita la interrupcion del ADC
    INTCONbits.PEIE = 1; // Interrupcion de los perifericos
    INTCONbits.RBIF = 1;
    INTCONbits.RBIE = 1;
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
            __delay_ms(18);
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
            __delay_ms(18);
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
            __delay_ms(18);
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
            __delay_ms(18);
}
char leer_eeprom(char direccion)
{
    EEADR = direccion;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    char data = EEDATA;
    return data;
}
void escribir_eeprom(char data, char direccion)
{
    EEADR = direccion;
    EEDAT = data;
    
    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;
    
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    
    EECON1bits.WR = 1;
    
    while(PIR2bits.EEIF == 0);
    PIR2bits.EEIF = 0;
    
    EECON1bits.WREN = 0;
    INTCONbits.GIE = 0;
}
void nuevo_bit_banging(void)
{
    if(M4<=50)
    {
     servo1();
    }
    if((M4<=101)&&(M4>=51))
    {
     servo1_2();
    }
    if((M4<=152)&&(M4>=102))
    {
     servo1_3();
    }
    if((M4<=203)&&(M4>=153))
    {
     servo1_4();
    }
    if(M4>=204)
    {
     servo1_5();
    }
    
    
    if(M5<=50)
    {
     servo2();
    }
    if((M5<=101)&&(M5>=51))
    {
     servo2_2();
    }
    if((M5<=152)&&(M5>=102))
    {
     servo2_3();
    }
    if((M5<=203)&&(M5>=153))
    {
     servo2_4();
    }
    if(M5>=204)
    {
     servo2_5();
    }
    
    if(M6<=50)
    {
     servo3();
    }
    if((M6<=101)&&(M6>=51))
    {
     servo3_2();
    }
    if((M6<=152)&&(M6>=102))
    {
     servo3_3();
    }
    if((M6<=203)&&(M6>=153))
    {
     servo3_4();
    }
    if(M6>=204)
    {
     servo3_5();
    }
    
    
    if(M7<=50)
    {
     servo4();
    }
    if((M7<=101)&&(M7>=51))
    {
     servo4_2();
    }
    if((M7<=152)&&(M7>=102))
    {
     servo4_3();
    }
    if((M7<=203)&&(M7>=153))
    {
     servo4_4();
    }
    if(M7>=204)
    {
     servo4_5();
    }
    
    
    if(sentido2 == 0b01)
    {
     CCPR1L = 0;
     CCPR2L = ADRESH;   
    }
    if(sentido2 == 0b10)
    {
     CCPR1L = ADRESH;
     CCPR2L = 0;   
    }
    if(sentido2 == 0b10)
    {
     CCPR1L = 0;
     CCPR2L = 0;   
    }
}
      
void menu(void){
     __delay_ms(50);
     printf("\rQue accion desea ejecutar? \r");
     __delay_ms(50);
     printf("\r(1) MOVER OJOS \r");
     __delay_ms(50);
     printf("(2) MOVER CEJAS \r");
     __delay_ms(50);
     printf("(3) MOVER LENGUA \r");
     __delay_ms(50);
     printf("(4) Salir de UART \r");
     while(RCIF == 0); //Se espera algo que recibir (CARACTER).
//     char entregado = RCREG;//Se crea un variable local que equivale al registro 
//                           // de recepcion para usarlo en las condicionales if.
     if (RCREG == '1'){//si la opcion que se recibe es 1 se hace lo siguiente
        __delay_ms(50);
        printf("\r(a) Mover ojos a la izquierda. \r");//Se despliega la fila de
                                                   //caracteres.
        __delay_ms(50);
        printf("(b) Mover ojos a la derecha. \r");
        __delay_ms(50);
        printf("(c) Mover ojos para arriba. \r");
        __delay_ms(50);
        printf("(d) Mover ojos para abajo. \r");
        while(RCIF == 0);
        if (RCREG == 'a')
        {
            servo2();
        }
        else if (RCREG == 'b')
        {
            servo2_5();
        }
        else if (RCREG == 'c')
        {
            servo1();
        }
        else if (RCREG == 'd')
        {
            servo1_5();
        }
    }                                             
    if (RCREG == '2'){//si la opcion que se recibe es 2 se hace lo siguiente
        __delay_ms(50);
        printf("\r(e) Mover ceja 1. \r");
        __delay_ms(50);
        printf("(f) Mover ceja 2. \r");
        while(RCIF == 0);//Se espera algo que recibir (CARACTER)elegido por 
                         // el usuario.
        if (RCREG == 'e')
        {
        __delay_ms(50);
        printf("\r(6) Mover ceja para arriba. \r");
        __delay_ms(50);
        printf("(7) Mover ceja para abajo. \r");
        while(RCIF == 0);
        if(RCREG == '6')
        {
            servo3();
        }
        if(RCREG == '7')
        {
            servo3_5();
        }
        }
        else if (RCREG == 'f')
        {
            __delay_ms(50);
        printf("\r(8) Mover ceja para arriba. \r");
        __delay_ms(50);
        printf("(9) Mover ceja para abajo. \r");
        while(RCIF == 0);
        if(RCREG == '8')
        {
            servo4();
        }
        if(RCREG == '9')
        {
            servo4_5();
        }
        }
    }
    if (RCREG == '3'){//si la opcion que se recibe es 3 se hace lo siguiente
        __delay_ms(50);
        printf("\r(g) Desenrrollar. \r");
        __delay_ms(50);
        printf("(h) Enrrollar. \r");
        __delay_ms(50);
        printf("(i) Deter. \r");
        while(RCIF == 0);//Se espera algo que recibir (CARACTER)elegido por 
                         // el usuario.
        if (RCREG == 'g')
        {
            CCPR1L = 0;
            CCPR2L = ADRESH;
        }
        else if (RCREG == 'h')
        {
            CCPR1L = ADRESH;
            CCPR2L = 0;
        }
        else if (RCREG == 'i')
        {
            CCPR1L = 0;
            CCPR2L = 0;
        }
    }
    if (RCREG == '4'){//si la opcion que se recibe es 3 se hace lo siguiente
        uart = 0;
        printf("\rHa salido del UART \r");
        __delay_ms(50);
    }
}
void putch(char info){//Se transmite la cadena de caracteres a esta funcion 
                      // por el printf
    while (TXIF == 0);// Se espera algo que haya que transmitir
    TXREG = info;// lo que hay en data se pasa al registro de transmision para 
                 // para que se depliegue en la terminal virtual.
}
