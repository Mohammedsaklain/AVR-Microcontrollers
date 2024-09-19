#include <Arduino.h>

#define DD *((volatile byte *)0x2A) // Data Direction for PORTD
#define DB *((volatile byte *)0x24) // Data Direction for PORTB
#define PD *((volatile byte *)0x2B) // PORTD
#define PB *((volatile byte *)0x25) // PORTB

volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;
volatile uint8_t hours = 0;

void displayNumber(uint8_t num, byte activeDigit);
void activateSegments(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g);


// Timer configuration
void timer_init() {
    TCCR1A = 0;                     // Normal mode
    TCCR1B = (1 << WGM12);          // CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
    OCR1A = 15624;                  // Set value for 1 second at 16 MHz
    TIMSK1 |= (1 << OCIE1A);        // Enable Timer 1 Compare A interrupt
}

// Display a number on specific digit
void displayNumber(uint8_t num, byte activeDigit) {
    PB = 0; // Clear PORTB to deactivate all digits

    // Activate corresponding segments for the number
    switch (num) {
        case 0: activateSegments(1, 1, 1, 1, 1, 1, 0); break;
        case 1: activateSegments(0, 1, 1, 0, 0, 0, 0); break;
        case 2: activateSegments(1, 1, 0, 1, 1, 0, 1); break;
        case 3: activateSegments(1, 1, 1, 1, 0, 0, 1); break;
        case 4: activateSegments(0, 1, 1, 0, 0, 1, 1); break;
        case 5: activateSegments(1, 0, 1, 1, 0, 1, 1); break;
        case 6: activateSegments(1, 0, 1, 1, 1, 1, 1); break;
        case 7: activateSegments(1, 1, 1, 0, 0, 0, 0); break;
        case 8: activateSegments(1, 1, 1, 1, 1, 1, 1); break;
        case 9: activateSegments(1, 1, 1, 1, 0, 1, 1); break;
    }

    // Activate the chosen digit
    PB |= (1 << activeDigit);
}

// Function to activate segments
void activateSegments(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g) {
    PD = 0; // Clear previous segments

    PD = (a << PORTD0) | (b << PORTD1) | (c << PORTD2) | 
         (d << PORTD3) | (e << PORTD4) | (f << PORTD5) | 
         (g << PORTD6); // Set required segments
}

// Timer interrupt service routine
ISR(TIMER1_COMPA_vect) {
    seconds++;
    if (seconds == 60) {
        seconds = 0;
        minutes++;
        if (minutes == 60) {
            minutes = 0;
            hours++;
            if (hours == 24) {
                hours = 0;
            }
        }
    }
}

// Setup function
void setup() {
    DD = 0xFF; // Set PORTD as output for segments
    DB = 0x0F; // Set PORTB lower nibble as output (for activating digits)
    timer_init(); // Initialize timer
    sei(); // Enable global interrupts
}

// Main loop
void loop() {
    // Display time dynamically
    displayNumber(hours % 10, PORTB1); // Display units of hours
    delay(5);
    displayNumber(minutes % 10, PORTB2); // Display units of minutes
    delay(5);
    displayNumber(seconds % 10, PORTB3); // Display units of seconds
    delay(5);
    
    // Optionally, you can implement a more advanced display method to show all digits
}
