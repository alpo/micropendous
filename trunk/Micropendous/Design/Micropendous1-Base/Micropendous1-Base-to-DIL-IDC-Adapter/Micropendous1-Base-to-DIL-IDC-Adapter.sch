EESchema Schematic File Version 2  date 29/08/2010 20:32:24
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:opendous
EELAYER 24  0
EELAYER END
$Descr A4 11700 8267
Sheet 1 1
Title "Micropendous1-Base-to-DIL-IDC-Adapter"
Date "30 aug 2010"
Rev "1.0"
Comp ""
Comment1 "http://creativecommons.org/licenses/by/3.0/"
Comment2 "Copyright Under the Creative Commons Attribution License"
Comment3 "By Opendous Inc."
Comment4 "www.Micropendous.org"
$EndDescr
$Comp
L MICROPENDOUS1-PICO P7
U 1 1 4C7AF8F9
P 8600 4150
F 0 "P7" H 8150 5400 50  0000 C CNN
F 1 "MICROPENDOUS1-PICO" H 8950 5400 50  0000 C CNN
	1    8600 4150
	-1   0    0    -1  
$EndComp
$Comp
L MICROPENDOUS1-BASE P11
U 1 1 4C7AF8D9
P 5950 4200
F 0 "P11" H 5450 5450 50  0000 C CNN
F 1 "MICROPENDOUS1-BASE" H 6300 5450 50  0000 C CNN
	1    5950 4200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6000 6100 6000 6000
Connection ~ 5150 5500
Wire Wire Line
	5200 5850 5150 5850
Wire Wire Line
	5150 5850 5150 5500
Wire Wire Line
	6650 5500 6750 5500
Wire Wire Line
	6750 5500 6750 5300
Wire Bus Line
	9450 5400 9450 2700
Wire Wire Line
	4650 4100 4450 4100
Wire Wire Line
	4450 4100 4450 4050
Wire Wire Line
	4650 5250 4450 5250
Wire Wire Line
	4450 5250 4450 5200
Wire Wire Line
	4800 6400 4800 6300
Wire Wire Line
	4800 6300 4450 6300
Wire Wire Line
	4450 6300 4450 6250
Wire Bus Line
	9450 2700 4550 2700
Wire Wire Line
	9600 4550 9600 4500
Wire Wire Line
	9600 4500 9350 4500
Wire Wire Line
	9350 4400 9350 4450
Wire Wire Line
	9350 4450 9600 4450
Wire Bus Line
	4550 2700 4550 6250
Wire Bus Line
	6850 2700 6850 5600
Wire Wire Line
	4450 6350 4450 6400
Wire Wire Line
	4450 6400 4650 6400
Wire Wire Line
	4450 5100 4450 5150
Wire Wire Line
	4450 5150 4800 5150
Wire Wire Line
	4800 5150 4800 5250
Wire Wire Line
	4450 3950 4450 4000
Wire Wire Line
	4450 4000 4800 4000
Wire Wire Line
	4800 4000 4800 4100
Wire Wire Line
	6750 4400 6750 4450
Wire Wire Line
	6750 4450 6950 4450
Wire Wire Line
	6950 4450 6950 4500
Wire Bus Line
	5100 6250 5100 5600
Wire Bus Line
	5100 5600 6850 5600
Wire Wire Line
	5500 6250 5500 5850
Connection ~ 5500 6150
Wire Wire Line
	6150 6000 6150 6100
Wire Wire Line
	6300 6000 6300 6100
Wire Wire Line
	6450 6000 6450 6100
$Comp
L VIA U1
U 1 1 49F0E959
P 6000 6100
F 0 "U1" V 6025 6200 20  0000 C CNN
F 1 "VIA" H 6000 6300 60  0001 C CNN
	1    6000 6100
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR01
U 1 1 49F0E950
P 6000 6000
F 0 "#PWR01" H 6000 6100 30  0001 C CNN
F 1 "VCC" H 6000 6100 30  0000 C CNN
	1    6000 6000
	1    0    0    -1  
$EndComp
$Comp
L VIA U4
U 1 1 49ED400A
P 6450 6000
F 0 "U4" V 6475 6100 20  0000 C CNN
F 1 "VIA" H 6450 6200 60  0001 C CNN
	1    6450 6000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 49ED4009
P 6450 6100
F 0 "#PWR02" H 6450 6100 30  0001 C CNN
F 1 "GND" H 6450 6030 30  0001 C CNN
	1    6450 6100
	1    0    0    -1  
$EndComp
$Comp
L VIA U3
U 1 1 49ED3ED6
P 6300 6000
F 0 "U3" V 6325 6100 20  0000 C CNN
F 1 "VIA" H 6300 6200 60  0001 C CNN
	1    6300 6000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 49ED3ED5
P 6300 6100
F 0 "#PWR03" H 6300 6100 30  0001 C CNN
F 1 "GND" H 6300 6030 30  0001 C CNN
	1    6300 6100
	1    0    0    -1  
$EndComp
$Comp
L VIA U2
U 1 1 49ED3EA6
P 6150 6000
F 0 "U2" V 6175 6100 20  0000 C CNN
F 1 "VIA" H 6150 6200 60  0001 C CNN
	1    6150 6000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 49ED3EA1
P 6150 6100
F 0 "#PWR04" H 6150 6100 30  0001 C CNN
F 1 "GND" H 6150 6030 30  0001 C CNN
	1    6150 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 49ED2D94
P 5500 6250
F 0 "#PWR05" H 5500 6250 30  0001 C CNN
F 1 "GND" H 5500 6180 30  0001 C CNN
	1    5500 6250
	1    0    0    -1  
$EndComp
Entry Wire Line
	5100 6250 5200 6150
Text Label 5200 6150 2    20   ~ 0
PD7
$Comp
L SWITCH_PUSHBUTTON SW2
U 1 1 49ED2D4B
P 5350 6150
F 0 "SW2" H 5350 6100 30  0000 C CNN
F 1 "HWB" H 5350 6225 25  0000 C CNN
	1    5350 6150
	1    0    0    -1  
$EndComp
$Comp
L SWITCH_PUSHBUTTON SW1
U 1 1 49ED2D47
P 5350 5850
F 0 "SW1" H 5350 5800 30  0000 C CNN
F 1 "RESET" H 5350 5925 25  0000 C CNN
	1    5350 5850
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P10
U 1 1 49ED29BD
P 5000 5500
F 0 "P10" H 5080 5500 40  0000 L CNN
F 1 "CONN_1" H 5000 5555 30  0001 C CNN
	1    5000 5500
	-1   0    0    1   
$EndComp
Entry Wire Line
	9350 3100 9450 3200
Entry Wire Line
	5150 5500 5250 5600
Text Label 5150 5500 2    20   ~ 0
RESET
Text Label 9350 3100 2    20   ~ 0
RESET
Entry Wire Line
	9350 3000 9450 3100
Entry Wire Line
	9350 3200 9450 3300
Entry Wire Line
	9350 3300 9450 3400
Entry Wire Line
	9350 3400 9450 3500
Entry Wire Line
	9350 3500 9450 3600
Entry Wire Line
	9350 3600 9450 3700
Entry Wire Line
	9350 3700 9450 3800
Entry Wire Line
	9350 3800 9450 3900
Entry Wire Line
	9350 3900 9450 4000
Entry Wire Line
	9350 4000 9450 4100
Entry Wire Line
	9350 4100 9450 4200
Entry Wire Line
	9350 4200 9450 4300
Entry Wire Line
	9350 4300 9450 4400
Entry Wire Line
	9350 4600 9450 4700
Entry Wire Line
	9350 4700 9450 4800
Entry Wire Line
	9350 4800 9450 4900
Entry Wire Line
	9350 4900 9450 5000
Entry Wire Line
	9350 5000 9450 5100
Entry Wire Line
	9350 5100 9450 5200
Entry Wire Line
	9350 5200 9450 5300
Entry Wire Line
	9350 5300 9450 5400
$Comp
L GND #PWR06
U 1 1 494F7272
P 9600 4550
F 0 "#PWR06" H 9600 4550 30  0001 C CNN
F 1 "GND" H 9600 4480 30  0001 C CNN
	1    9600 4550
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR07
U 1 1 494F7271
P 9600 4450
F 0 "#PWR07" H 9600 4550 30  0001 C CNN
F 1 "VCC" H 9600 4550 30  0000 C CNN
	1    9600 4450
	1    0    0    -1  
$EndComp
Text Label 9350 4000 2    20   ~ 0
PD3
Text Label 9350 4300 2    20   ~ 0
PD0
Text Label 9350 4200 2    20   ~ 0
PD1
Text Label 9350 4100 2    20   ~ 0
PD2
Text Label 9350 3900 2    20   ~ 0
PD4
Text Label 9350 3800 2    20   ~ 0
PD5
Text Label 9350 3700 2    20   ~ 0
PD6
Text Label 9350 3500 2    20   ~ 0
PC7
Text Label 9350 3400 2    20   ~ 0
PC6
Text Label 9350 3300 2    20   ~ 0
PC5
Text Label 9350 3200 2    20   ~ 0
PC4
Text Label 9350 3000 2    20   ~ 0
PC2
Text Label 9350 5300 2    20   ~ 0
PB0
Text Label 9350 5200 2    20   ~ 0
PB1
Text Label 9350 5100 2    20   ~ 0
PB2
Text Label 9350 5000 2    20   ~ 0
PB3
Text Label 9350 4900 2    20   ~ 0
PB4
Text Label 9350 4800 2    20   ~ 0
PB5
Text Label 9350 4700 2    20   ~ 0
PB6
Text Label 9350 4600 2    20   ~ 0
PB7
Text Label 9350 3600 2    20   ~ 0
PD7
Entry Wire Line
	4450 4900 4550 5000
Entry Wire Line
	4450 4800 4550 4900
Entry Wire Line
	4450 4700 4550 4800
Entry Wire Line
	4450 4600 4550 4700
Entry Wire Line
	4450 4500 4550 4600
Entry Wire Line
	4450 4400 4550 4500
Entry Wire Line
	4450 4300 4550 4400
Entry Wire Line
	4450 5000 4550 5100
Text Label 4450 5850 2    20   ~ 0
PC4
Text Label 4450 6150 2    20   ~ 0
PC7
Text Label 4450 6050 2    20   ~ 0
PC6
Text Label 4450 5950 2    20   ~ 0
PC5
$Comp
L GND #PWR08
U 1 1 494F6FB1
P 4800 6400
F 0 "#PWR08" H 4800 6400 30  0001 C CNN
F 1 "GND" H 4800 6330 30  0001 C CNN
	1    4800 6400
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR09
U 1 1 494F6FB0
P 4650 6400
F 0 "#PWR09" H 4650 6500 30  0001 C CNN
F 1 "VCC" H 4650 6500 30  0000 C CNN
	1    4650 6400
	1    0    0    -1  
$EndComp
$Comp
L CONN_10 P6
U 1 1 494F6FAF
P 4100 5900
F 0 "P6" V 4050 5900 60  0000 C CNN
F 1 "PortC" V 4150 5900 60  0000 C CNN
	1    4100 5900
	-1   0    0    1   
$EndComp
Entry Wire Line
	4450 6050 4550 6150
Entry Wire Line
	4450 5950 4550 6050
Entry Wire Line
	4450 5850 4550 5950
Entry Wire Line
	4450 5750 4550 5850
Entry Wire Line
	4450 5650 4550 5750
Entry Wire Line
	4450 5550 4550 5650
Entry Wire Line
	4450 5450 4550 5550
Entry Wire Line
	4450 6150 4550 6250
$Comp
L GND #PWR010
U 1 1 494F6F8D
P 4800 4100
F 0 "#PWR010" H 4800 4100 30  0001 C CNN
F 1 "GND" H 4800 4030 30  0001 C CNN
	1    4800 4100
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR011
U 1 1 494F6F8C
P 4650 4100
F 0 "#PWR011" H 4650 4200 30  0001 C CNN
F 1 "VCC" H 4650 4200 30  0000 C CNN
	1    4650 4100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 494F6F2F
P 4800 5250
F 0 "#PWR012" H 4800 5250 30  0001 C CNN
F 1 "GND" H 4800 5180 30  0001 C CNN
	1    4800 5250
	1    0    0    -1  
$EndComp
$Comp
L CONN_10 P4
U 1 1 494F6EF8
P 4100 3600
F 0 "P4" V 4050 3600 60  0000 C CNN
F 1 "PortD" V 4150 3600 60  0000 C CNN
	1    4100 3600
	-1   0    0    1   
$EndComp
$Comp
L CONN_10 P5
U 1 1 494F6EF5
P 4100 4750
F 0 "P5" V 4050 4750 60  0000 C CNN
F 1 "PortB" V 4150 4750 60  0000 C CNN
	1    4100 4750
	-1   0    0    1   
$EndComp
Entry Wire Line
	6750 3100 6850 3200
Entry Wire Line
	6750 3200 6850 3300
Entry Wire Line
	6750 3300 6850 3400
Entry Wire Line
	6750 3400 6850 3500
Entry Wire Line
	6750 3500 6850 3600
Entry Wire Line
	6750 4500 6850 4600
Entry Wire Line
	6750 4600 6850 4700
Entry Wire Line
	6750 4700 6850 4800
Entry Wire Line
	6750 4800 6850 4900
Entry Wire Line
	6750 4900 6850 5000
Entry Wire Line
	6750 5000 6850 5100
Entry Wire Line
	6750 5100 6850 5200
Entry Wire Line
	6750 5200 6850 5300
Entry Wire Line
	6750 3600 6850 3700
Entry Wire Line
	6750 3700 6850 3800
Entry Wire Line
	6750 3800 6850 3900
Entry Wire Line
	6750 3900 6850 4000
Entry Wire Line
	6750 4000 6850 4100
Entry Wire Line
	6750 4100 6850 4200
Entry Wire Line
	6750 4200 6850 4300
Entry Wire Line
	6750 4300 6850 4400
$Comp
L GND #PWR013
U 1 1 494F6418
P 6950 4500
F 0 "#PWR013" H 6950 4500 30  0001 C CNN
F 1 "GND" H 6950 4430 30  0001 C CNN
	1    6950 4500
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR014
U 1 1 494F6417
P 6650 5500
F 0 "#PWR014" H 6650 5600 30  0001 C CNN
F 1 "VCC" H 6650 5600 30  0000 C CNN
	1    6650 5500
	1    0    0    -1  
$EndComp
Text Label 6750 4900 2    20   ~ 0
PD3
Text Label 6750 5200 2    20   ~ 0
PD0
Text Label 6750 5100 2    20   ~ 0
PD1
Text Label 6750 5000 2    20   ~ 0
PD2
Text Label 6750 4800 2    20   ~ 0
PD4
Text Label 6750 4700 2    20   ~ 0
PD5
Text Label 6750 4600 2    20   ~ 0
PD6
Text Label 6750 3500 2    20   ~ 0
PC7
Text Label 6750 3400 2    20   ~ 0
PC6
Text Label 6750 3300 2    20   ~ 0
PC5
Text Label 6750 3200 2    20   ~ 0
PC4
Text Label 6750 3100 2    20   ~ 0
PC2
Text Label 6750 4300 2    20   ~ 0
PB0
Text Label 6750 4200 2    20   ~ 0
PB1
Text Label 6750 4100 2    20   ~ 0
PB2
Text Label 6750 4000 2    20   ~ 0
PB3
Text Label 6750 3900 2    20   ~ 0
PB4
Text Label 6750 3800 2    20   ~ 0
PB5
Text Label 6750 3700 2    20   ~ 0
PB6
Text Label 6750 3600 2    20   ~ 0
PB7
Text Label 6750 4500 2    20   ~ 0
PD7
Entry Wire Line
	4450 3750 4550 3850
$Comp
L VCC #PWR015
U 1 1 49468CA2
P 4650 5250
F 0 "#PWR015" H 4650 5350 30  0001 C CNN
F 1 "VCC" H 4650 5350 30  0000 C CNN
	1    4650 5250
	1    0    0    -1  
$EndComp
Entry Wire Line
	4450 3650 4550 3750
Entry Wire Line
	4450 3550 4550 3650
Entry Wire Line
	4450 3450 4550 3550
Entry Wire Line
	4450 3350 4550 3450
Entry Wire Line
	4450 3250 4550 3350
Entry Wire Line
	4450 3150 4550 3250
Entry Wire Line
	4450 3850 4550 3950
Text Label 4450 3450 2    20   ~ 0
PD3
Text Label 4450 3150 2    20   ~ 0
PD0
Text Label 4450 3250 2    20   ~ 0
PD1
Text Label 4450 3350 2    20   ~ 0
PD2
Text Label 4450 3550 2    20   ~ 0
PD4
Text Label 4450 3650 2    20   ~ 0
PD5
Text Label 4450 3750 2    20   ~ 0
PD6
Text Label 4450 5650 2    20   ~ 0
PC2
Text Label 4450 4300 2    20   ~ 0
PB0
Text Label 4450 4400 2    20   ~ 0
PB1
Text Label 4450 4500 2    20   ~ 0
PB2
Text Label 4450 4600 2    20   ~ 0
PB3
Text Label 4450 4700 2    20   ~ 0
PB4
Text Label 4450 4800 2    20   ~ 0
PB5
Text Label 4450 4900 2    20   ~ 0
PB6
Text Label 4450 5000 2    20   ~ 0
PB7
Text Label 4450 3850 2    20   ~ 0
PD7
$EndSCHEMATC
