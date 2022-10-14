// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include <time.h>

#include "lcd.h"
void fibonacci(int num);

int main(void) {
    // initiate port and LCD
    Port_Init();
    LCD_Init();

    // print the first 78 Fibonacci numbers
    fibonacci(78);
}

void fibonacci(int num) {
    LCD_Print_Str("Fibonacci:");
    uint64_t n1 = 0;
    uint64_t n2 = 1;
    uint64_t temp;
    int count = 0;
    while (count < num) {
       count += 1;
       LCD_Cursor(cursor_line_2_front);
       temp = n2;
       n2 = n1 + n2;
       n1 = temp;
       LCD_Print_Int(n2);
       delay_ms(1000);
    }
}

