EESchema Schematic File Version 2  date 29/08/2010 20:33:50
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
Title "Micropendous1-Base-to-JTAG-Adaptor"
Date "22 aug 2010"
Rev "1.0"
Comp ""
Comment1 "http://creativecommons.org/licenses/by/3.0/"
Comment2 "Copyright Under the Creative Commons Attribution License"
Comment3 "By Opendous Inc."
Comment4 "www.Micropendous.org"
$EndDescr
Connection ~ 4950 6050
Wire Wire Line
	4950 6000 4950 6100
Wire Wire Line
	4950 6100 4700 6100
Connection ~ 4950 5750
Connection ~ 4950 5650
Wire Wire Line
	4950 5700 4950 5650
Connection ~ 4700 5950
Connection ~ 4700 5900
Connection ~ 4700 5850
Connection ~ 4700 5800
Connection ~ 4700 5750
Connection ~ 4700 5700
Wire Wire Line
	6750 5000 6750 4950
Wire Wire Line
	6750 4950 6650 4950
Wire Wire Line
	6650 4950 6650 5050
Wire Wire Line
	6650 5050 6600 5050
Wire Wire Line
	6650 2250 6550 2250
Connection ~ 6750 2600
Wire Wire Line
	6750 2550 6750 2600
Wire Wire Line
	7250 2800 7250 2600
Wire Wire Line
	5650 3650 5650 3600
Wire Wire Line
	5650 3600 5500 3600
Wire Wire Line
	4400 4900 4500 4900
Wire Wire Line
	4800 4900 4800 5300
Connection ~ 4800 5200
Wire Wire Line
	5500 4500 5650 4500
Wire Wire Line
	6250 2800 6250 2600
Wire Wire Line
	6250 2600 7250 2600
Wire Wire Line
	7250 2900 7250 3850
Connection ~ 7250 3000
Connection ~ 7250 3100
Connection ~ 7250 3200
Connection ~ 7250 3300
Connection ~ 7250 3400
Connection ~ 7250 3500
Connection ~ 7250 3600
Connection ~ 7250 3700
Wire Wire Line
	6850 2250 6950 2250
Wire Wire Line
	6600 5150 6650 5150
Wire Wire Line
	6650 5150 6650 5250
Wire Wire Line
	6650 5250 6750 5250
Wire Wire Line
	6750 5250 6750 5200
Wire Wire Line
	7550 5200 7550 5250
Wire Wire Line
	7550 5250 7450 5250
Wire Wire Line
	7450 5250 7450 5150
Wire Wire Line
	7450 5150 7400 5150
Wire Wire Line
	7400 5050 7450 5050
Wire Wire Line
	7450 5050 7450 4950
Wire Wire Line
	7450 4950 7550 4950
Wire Wire Line
	7550 4950 7550 5000
Wire Wire Line
	4950 5650 5000 5650
Wire Wire Line
	5050 5750 4950 5750
Wire Wire Line
	5050 5750 5050 5800
Wire Wire Line
	4950 5750 4950 5850
Connection ~ 4950 5800
Connection ~ 4700 6000
Connection ~ 4700 6050
Wire Wire Line
	4700 6150 4700 5650
Connection ~ 4700 6100
$Comp
L VIA V18
U 1 1 4C70B6F7
P 4950 6050
F 0 "V18" V 4975 6150 20  0000 C CNN
F 1 "VIA" H 4950 6250 60  0001 C CNN
	1    4950 6050
	0    -1   -1   0   
$EndComp
$Comp
L VIA V17
U 1 1 4C70B6F3
P 4700 6050
F 0 "V17" V 4725 6150 20  0000 C CNN
F 1 "VIA" H 4700 6250 60  0001 C CNN
	1    4700 6050
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR01
U 1 1 4C70B574
P 5050 5800
F 0 "#PWR01" H 5050 5800 30  0001 C CNN
F 1 "GND" H 5050 5730 30  0001 C CNN
	1    5050 5800
	1    0    0    -1  
$EndComp
Text Label 4950 5950 0    20   ~ 0
PC7
Text Label 4950 5900 0    20   ~ 0
PC7
$Comp
L VIA V13
U 1 1 4C70AC09
P 4950 5850
F 0 "V13" V 4975 5950 20  0000 C CNN
F 1 "VIA" H 4950 6050 60  0001 C CNN
	1    4950 5850
	0    -1   -1   0   
$EndComp
$Comp
L VIA V12
U 1 1 4C70AC08
P 4950 5800
F 0 "V12" V 4975 5900 20  0000 C CNN
F 1 "VIA" H 4950 6000 60  0001 C CNN
	1    4950 5800
	0    -1   -1   0   
$EndComp
$Comp
L VIA V10
U 1 1 4C70AC07
P 4950 5700
F 0 "V10" V 4975 5800 20  0000 C CNN
F 1 "VIA" H 4950 5900 60  0001 C CNN
	1    4950 5700
	0    -1   -1   0   
$EndComp
$Comp
L VIA V11
U 1 1 4C70AC06
P 4950 5750
F 0 "V11" V 4975 5850 20  0000 C CNN
F 1 "VIA" H 4950 5950 60  0001 C CNN
	1    4950 5750
	0    -1   -1   0   
$EndComp
$Comp
L VIA V15
U 1 1 4C70AC01
P 4950 5950
F 0 "V15" V 4975 6050 20  0000 C CNN
F 1 "VIA" H 4950 6150 60  0001 C CNN
	1    4950 5950
	0    -1   -1   0   
$EndComp
$Comp
L VIA V14
U 1 1 4C70AC00
P 4950 5900
F 0 "V14" V 4975 6000 20  0000 C CNN
F 1 "VIA" H 4950 6100 60  0001 C CNN
	1    4950 5900
	0    -1   -1   0   
$EndComp
$Comp
L VIA V16
U 1 1 4C70ABFD
P 4950 6000
F 0 "V16" V 4975 6100 20  0000 C CNN
F 1 "VIA" H 4950 6200 60  0001 C CNN
	1    4950 6000
	0    -1   -1   0   
$EndComp
$Comp
L VIA V9
U 1 1 4C6E737F
P 4950 5650
F 0 "V9" V 4975 5750 20  0000 C CNN
F 1 "VIA" H 4950 5850 60  0001 C CNN
	1    4950 5650
	0    -1   -1   0   
$EndComp
$Comp
L VIA V5
U 1 1 4C6E701A
P 4700 5850
F 0 "V5" V 4725 5950 20  0000 C CNN
F 1 "VIA" H 4700 6050 60  0001 C CNN
	1    4700 5850
	0    -1   -1   0   
$EndComp
$Comp
L VIA V6
U 1 1 4C6E7019
P 4700 5900
F 0 "V6" V 4725 6000 20  0000 C CNN
F 1 "VIA" H 4700 6100 60  0001 C CNN
	1    4700 5900
	0    -1   -1   0   
$EndComp
$Comp
L VIA V8
U 1 1 4C6E7018
P 4700 6000
F 0 "V8" V 4725 6100 20  0000 C CNN
F 1 "VIA" H 4700 6200 60  0001 C CNN
	1    4700 6000
	0    -1   -1   0   
$EndComp
$Comp
L VIA V7
U 1 1 4C6E7017
P 4700 5950
F 0 "V7" V 4725 6050 20  0000 C CNN
F 1 "VIA" H 4700 6150 60  0001 C CNN
	1    4700 5950
	0    -1   -1   0   
$EndComp
$Comp
L VIA V3
U 1 1 4C6E7015
P 4700 5750
F 0 "V3" V 4725 5850 20  0000 C CNN
F 1 "VIA" H 4700 5950 60  0001 C CNN
	1    4700 5750
	0    -1   -1   0   
$EndComp
$Comp
L VIA V4
U 1 1 4C6E7014
P 4700 5800
F 0 "V4" V 4725 5900 20  0000 C CNN
F 1 "VIA" H 4700 6000 60  0001 C CNN
	1    4700 5800
	0    -1   -1   0   
$EndComp
$Comp
L VIA V2
U 1 1 4C6E7012
P 4700 5700
F 0 "V2" V 4725 5800 20  0000 C CNN
F 1 "VIA" H 4700 5900 60  0001 C CNN
	1    4700 5700
	0    -1   -1   0   
$EndComp
$Comp
L VIA V1
U 1 1 4C6E7004
P 4700 5650
F 0 "V1" V 4725 5750 20  0000 C CNN
F 1 "VIA" H 4700 5850 60  0001 C CNN
	1    4700 5650
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR02
U 1 1 4C6E69AC
P 7550 5200
F 0 "#PWR02" H 7550 5300 30  0001 C CNN
F 1 "VCC" H 7550 5300 30  0000 C CNN
	1    7550 5200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 4C6E69AB
P 7550 5000
F 0 "#PWR03" H 7550 5000 30  0001 C CNN
F 1 "GND" H 7550 4930 30  0001 C CNN
	1    7550 5000
	1    0    0    -1  
$EndComp
$Comp
L VREF #PWR04
U 1 1 4C6E6953
P 6950 2250
F 0 "#PWR04" H 6950 2350 30  0001 C CNN
F 1 "VREF" H 6950 2350 30  0000 C CNN
	1    6950 2250
	1    0    0    -1  
$EndComp
$Comp
L JUMPER-2 JP1
U 1 1 4C6E6947
P 6750 2250
F 0 "JP1" H 6750 2175 30  0000 C CNN
F 1 "JUMPER-2" H 6750 2250 30  0001 C CNN
	1    6750 2250
	-1   0    0    1   
$EndComp
$Comp
L VREF #PWR05
U 1 1 4C6E6935
P 6750 2550
F 0 "#PWR05" H 6750 2650 30  0001 C CNN
F 1 "VREF" H 6750 2650 30  0000 C CNN
	1    6750 2550
	1    0    0    -1  
$EndComp
Text Label 6250 3700 2    40   ~ 0
PC7
$Comp
L GND #PWR06
U 1 1 4C6E67F2
P 7250 3850
F 0 "#PWR06" H 7250 3850 30  0001 C CNN
F 1 "GND" H 7250 3780 30  0001 C CNN
	1    7250 3850
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR07
U 1 1 4C6E67F1
P 6550 2250
F 0 "#PWR07" H 6550 2350 30  0001 C CNN
F 1 "VCC" H 6550 2350 30  0000 C CNN
	1    6550 2250
	1    0    0    -1  
$EndComp
Text Label 6250 3300 2    40   ~ 0
PB7
Text Label 6250 3500 2    40   ~ 0
PB6
Text Label 6250 2900 2    40   ~ 0
PB5
Text Label 6250 3600 2    40   ~ 0
PB4
Text Label 6250 3400 2    40   ~ 0
PB3
Text Label 6250 3000 2    40   ~ 0
PB2
Text Label 6250 3200 2    40   ~ 0
PB1
Text Label 6250 3100 2    40   ~ 0
PB0
$Comp
L CONN_JTAG_ARM U2
U 1 1 4C6E6790
P 6750 3250
F 0 "U2" H 6550 2700 50  0000 C CNN
F 1 "ARM_JTAG" H 6750 3800 50  0000 C CNN
	1    6750 3250
	1    0    0    -1  
$EndComp
NoConn ~ 7400 4250
NoConn ~ 7400 4350
NoConn ~ 7400 4550
$Comp
L GND #PWR08
U 1 1 4C6E673D
P 5650 3650
F 0 "#PWR08" H 5650 3650 30  0001 C CNN
F 1 "GND" H 5650 3580 30  0001 C CNN
	1    5650 3650
	1    0    0    -1  
$EndComp
$Comp
L MICROPENDOUS1-BASE U1
U 1 1 4C6E8395
P 4700 3400
F 0 "U1" H 4200 4650 50  0000 C CNN
F 1 "MICROPENDOUS1-BASE" H 5100 4650 50  0000 C CNN
	1    4700 3400
	-1   0    0    -1  
$EndComp
$Comp
L VCC #PWR09
U 1 1 49F0E950
P 5000 5650
F 0 "#PWR09" H 5000 5750 30  0001 C CNN
F 1 "VCC" V 5000 5775 30  0000 C CNN
	1    5000 5650
	0    1    1    0   
$EndComp
$Comp
L GND #PWR010
U 1 1 49ED4009
P 4700 6150
F 0 "#PWR010" H 4700 6150 30  0001 C CNN
F 1 "GND" H 4700 6080 30  0001 C CNN
	1    4700 6150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 49ED2D94
P 4800 5300
F 0 "#PWR011" H 4800 5300 30  0001 C CNN
F 1 "GND" H 4800 5230 30  0001 C CNN
	1    4800 5300
	1    0    0    -1  
$EndComp
Text Label 4500 5200 2    20   ~ 0
PD7
$Comp
L SWITCH_PUSHBUTTON SW2
U 1 1 49ED2D4B
P 4650 5200
F 0 "SW2" H 4650 5150 30  0000 C CNN
F 1 "HWB" H 4650 5275 25  0000 C CNN
	1    4650 5200
	1    0    0    -1  
$EndComp
$Comp
L SWITCH_PUSHBUTTON SW1
U 1 1 49ED2D47
P 4650 4900
F 0 "SW1" H 4650 4850 30  0000 C CNN
F 1 "RESET" H 4650 4975 25  0000 C CNN
	1    4650 4900
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P1
U 1 1 49ED29BD
P 4250 4900
F 0 "P1" H 4300 4900 40  0000 L CNN
F 1 "CONN_1" H 4250 4955 30  0001 C CNN
	1    4250 4900
	-1   0    0    1   
$EndComp
Text Label 4500 4900 2    20   ~ 0
RESET
Text Label 7400 4650 2    20   ~ 0
PC4
Text Label 7400 4950 2    20   ~ 0
PC7
Text Label 7400 4850 2    20   ~ 0
PC6
Text Label 7400 4750 2    20   ~ 0
PC5
$Comp
L CONN_10 P3
U 1 1 494F6FAF
P 7050 4700
F 0 "P3" V 7000 4700 60  0000 C CNN
F 1 "PortC" V 7100 4700 60  0000 C CNN
	1    7050 4700
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 494F6F8D
P 6750 5000
F 0 "#PWR012" H 6750 5000 30  0001 C CNN
F 1 "GND" H 6750 4930 30  0001 C CNN
	1    6750 5000
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR013
U 1 1 494F6F8C
P 6750 5200
F 0 "#PWR013" H 6750 5300 30  0001 C CNN
F 1 "VCC" H 6750 5300 30  0000 C CNN
	1    6750 5200
	1    0    0    -1  
$EndComp
$Comp
L CONN_10 P2
U 1 1 494F6EF8
P 6250 4700
F 0 "P2" V 6200 4700 60  0000 C CNN
F 1 "PortD" V 6300 4700 60  0000 C CNN
	1    6250 4700
	-1   0    0    -1  
$EndComp
$Comp
L VCC #PWR014
U 1 1 494F6417
P 5650 4500
F 0 "#PWR014" H 5650 4600 30  0001 C CNN
F 1 "VCC" H 5650 4600 30  0000 C CNN
	1    5650 4500
	1    0    0    -1  
$EndComp
Text Label 5500 4100 2    20   ~ 0
PD3
Text Label 5500 4400 2    20   ~ 0
PD0
Text Label 5500 4300 2    20   ~ 0
PD1
Text Label 5500 4200 2    20   ~ 0
PD2
Text Label 5500 4000 2    20   ~ 0
PD4
Text Label 5500 3900 2    20   ~ 0
PD5
Text Label 5500 3800 2    20   ~ 0
PD6
Text Label 5500 2700 2    20   ~ 0
PC7
Text Label 5500 2600 2    20   ~ 0
PC6
Text Label 5500 2500 2    20   ~ 0
PC5
Text Label 5500 2400 2    20   ~ 0
PC4
Text Label 5500 2300 2    20   ~ 0
PC2
Text Label 5500 3500 2    20   ~ 0
PB0
Text Label 5500 3400 2    20   ~ 0
PB1
Text Label 5500 3300 2    20   ~ 0
PB2
Text Label 5500 3200 2    20   ~ 0
PB3
Text Label 5500 3100 2    20   ~ 0
PB4
Text Label 5500 3000 2    20   ~ 0
PB5
Text Label 5500 2900 2    20   ~ 0
PB6
Text Label 5500 2800 2    20   ~ 0
PB7
Text Label 5500 3700 2    20   ~ 0
PD7
Text Label 6600 4550 2    20   ~ 0
PD3
Text Label 6600 4250 2    20   ~ 0
PD0
Text Label 6600 4350 2    20   ~ 0
PD1
Text Label 6600 4450 2    20   ~ 0
PD2
Text Label 6600 4650 2    20   ~ 0
PD4
Text Label 6600 4750 2    20   ~ 0
PD5
Text Label 6600 4850 2    20   ~ 0
PD6
Text Label 7400 4450 2    20   ~ 0
PC2
Text Label 6600 4950 2    20   ~ 0
PD7
$EndSCHEMATC
