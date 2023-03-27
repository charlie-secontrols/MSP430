#include <msp430.h>

int analogRead(int pin){
    ADC12CTL0 = ADC12SHT0_2 + ADC12ON; // Set sample time and turn on ADC12
    ADC12CTL1 = ADC12SHP; // Use sampling timer

    if (pin == 0){
    ADC12MCTL0 = ADC12INCH_0;} // Set analog input channel A0
    else if (pin == 1){
    ADC12MCTL0 = ADC12INCH_1;} // Set analog input channel A1
    else if (pin == 2){
    ADC12MCTL0 = ADC12INCH_2;} // Set analog input channel A2

    ADC12CTL0 |= ADC12ENC; // Enable ADC12

    ADC12CTL0 |= ADC12SC; // Start conversion
    while (!(ADC12IFG & BIT0)); // Wait for conversion to complete
    return ADC12MEM0; // Read conversion result
}
void analogWrite(int pin, int duty_cycle){

    if (pin == 0){
    ADC12MCTL0 = ADC12INCH_0;} // Set analog input channel A0
    else if (pin == 1){
    ADC12MCTL0 = ADC12INCH_1;} // Set analog input channel A1
    else if (pin == 2){
    ADC12MCTL0 = ADC12INCH_2;} // Set analog input channel A2

    P1DIR |= BIT2;              // P1.2 as output
    P1SEL |= BIT2;              // P1.2 as TA0.1 output

    TA0CCR0 = 1000 - 1;         // PWM period (1000 cycles)
    TA0CCTL1 = OUTMOD_7;        // TA0CCR1 output mode = reset/set
    TA0CCR1 = duty_cycle;              // Duty cycle = PWM
    TA0CTL = TASSEL_2 + MC_1;   // SMCLK, Up mode
}

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    P2DIR |= BIT5;              // P2.5 as output
    P1DIR |= BIT2;              // P1.2 as output
    P1DIR |= BIT4;              // P1.4 as output

    P1OUT = ~BIT2;          // Relay OFF
    P1OUT = BIT4;          // Relay OFF
    P2OUT = BIT5;


    while (1)
    {
        P1OUT ^= BIT2;          // Toggle Relay
        P1OUT ^= ~BIT4;          // Toggle Relay
        __delay_cycles(1000000);    // Delay 1s
        P1OUT ^= BIT2;          // Toggle Relay
        P1OUT ^= BIT4;          // Toggle Relay
        __delay_cycles(1000000);    // Delay 1s

    }

}
