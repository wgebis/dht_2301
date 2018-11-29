#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
 
#define MAX_TIMINGS 85
#define DHT_PIN 7
int data[5] = { 0, 0, 0, 0, 0 };
 
int read_dht_data(int pin)
{
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j=0, i;

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;
 
    /* pull pin down for 18 milliseconds */
    pinMode(DHT_PIN, OUTPUT);
    digitalWrite(pin, LOW);
    delay(18);
 
    /* prepare to read the pin */
    pinMode(pin, INPUT);
 
    /* detect change and read data */
    for (i = 0; i < MAX_TIMINGS; i++) {
        counter = 0;
        while(digitalRead(pin) == laststate) {
            counter++;
            delayMicroseconds( 1 );
            if (counter == 255) {
                break;
            }
        }
        laststate = digitalRead(pin);
 
        if (counter == 255)
            break;
 
        /* ignore first 3 transitions */
        if ((i >= 4) && (i % 2 == 0)) {
            /* shove each bit into the storage bytes */
            data[j/8] <<= 1;
            if ( counter > 16 )
                data[j/8] |= 1;
            j++;
        }
    }
 
    /*
     * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
     * print it out if data is good
     */
 
    if ( (j >= 40) &&
         (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        float h = (float)((data[0] << 8) + data[1]) / 10;
        if ( h > 100 ) {
            h = data[0];    // for DHT11
        }
        float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (c > 125) {
            c = data[2];    // for DHT11
        }
        if (data[2] & 0x80) {
            c = -c;
        }
        printf("Humidity = %.1f %% Temperature = %.1f\n", h, c);
	return 0;
    }
    else  {
       //printf( "Data not good, skip\n" );
       return 1;
    }
}
 
int main(int argc, char *argv[])
{

    if (argc != 2) {
        printf("The argument numbers is invalid. Usage: dht_2301 <pin_number>\n");
	exit(1);
    }

    for (int i = 0; i < strlen(argv[1]); i++) {
    if (!isdigit(argv[1][i])) {
        printf("The pin number is in wrong format.\n");
        exit(1);
    }
    }

    if (wiringPiSetup() == -1)
        exit(1);

    int i = 0;  
    while (i < 6)
    {
        int result = read_dht_data(atoi(argv[1]));
	if (result != 0) 
	  i++;
	else
	  break;
        delay( 500 ); 
    }
    return(0);
}
