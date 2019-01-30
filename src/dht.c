#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#define WAIT_USEC 100
 
int assure_state(int pin, int state) {

    for(int i = 0; i <= WAIT_USEC; i++) {
	if (digitalRead(pin) == state) {
	    return i;
        } 
        delayMicroseconds(1);
    }
    return -1;
}

int init_read_data(int pin) {  

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delayMicroseconds(100);

    /* Host reset signal */
    digitalWrite(pin, LOW);
    delay(16);

    digitalWrite(pin, HIGH);
    pinMode(pin, INPUT);
    if (assure_state(pin, HIGH) < 0) {
        return -1;
    }

    for (int i = 0; i < 2; i++) {
        if (assure_state(pin, LOW) < 0) {
	    return -1;
	}
        if (assure_state(pin, HIGH) < 0) {
	    return -1;
	}
    }
}
 
int read_dht_data(int pin) {

    if (init_read_data(pin) < 0) {
	return -2;
    }

    /* detect change and read data */
    int laststate = HIGH; 
    int data[5] = { 0, 0, 0, 0, 0 };
    for (int i = 0, j = 0; i <= (40*2); i++) {
        int counter = 0;
        while(digitalRead(pin) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == WAIT_USEC) {
                return -1;
            }
        }
        laststate = digitalRead(pin);
 
        if (i % 2 == 0) {
            data[j/8] <<= 1;
            if (counter > 40)
                data[j/8] |= 1;
            j++;
        }
    }

    /*
     * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
     * print it out if data is good
     */
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        float h = (float)((data[0] << 8) + data[1]) / 10;
        if (h > 100) {
            h = data[0];    // for DHT11
        }
        float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (c > 125) {
            c = data[2];    // for DHT11
        }
        if (data[2] & 0x80) {
            c = -c;
        }
	if (h < 20 || h > 95) {
	    // Humidity out of range
	    return 1;
	}
        printf("Humidity = %.1f %% Temperature = %.1f\n", h, c);
	return 0;
    } else  {
       return 1;
    }
}
 
int main(int argc, char *argv[]) {

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

    if (wiringPiSetup() == -1) {
        printf("Unable to initialize WiringPi library.\n");
        exit(1);
    }

    int i = 0;  
    while (i < 10) {
        int result = read_dht_data(atoi(argv[1]));
	if (result != 0) {
	   i++;
	}
	else
	   break;
        delay(800 + (rand() % 200)); 
    }
    return(0);
}
