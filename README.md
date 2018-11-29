## Overview

Tool written in C for reading humidity and temperature from DHT21 (AM2301) sensor. It bases on WiringPi library.
It has been testet with OrnagePI Zero.

## Prerequitiies

* Build tools

```
$ sudo apt-get install build-essential
```

* WiringPi library 

I used fork for [OrangePi Zero board](https://github.com/xpertsavenue/WiringOP-Zero).

## Compiling and install

It can be perform with autotools:

```
$ ./configure
$ make
$ sudo make install
```

## Usage 

```
$ dht_2301 <pin_number>
```

Example output:

```
Humidity = 62.7 % Temperature = 8.9
```

## Links

* [Read DHT11/22 tematerature & humidity sensor from Raspberry Pi](http://www.uugear.com/portfolio/read-dht1122-temperature-humidity-sensor-from-raspberry-pi/)
* [WiringPi for OP Zero](https://github.com/xpertsavenue/WiringOP-Zero)
