EESchema Schematic File Version 2  date 29/08/2010 20:37:44
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
LIBS:Micropendous1-Base-cache
EELAYER 24  0
EELAYER END
$Descr A4 11700 8267
Sheet 1 1
Title "Micropendous1-Base"
Date "30 aug 2010"
Rev "1.0"
Comp ""
Comment1 "http://creativecommons.org/licenses/by/3.0/"
Comment2 "Copyright Under the Creative Commons Attribution License"
Comment3 "By Opendous Inc."
Comment4 "www.Micropendous.org"
$EndDescr
$Comp
L VCC #PWR01
U 1 1 4905873C
P 9150 3450
F 0 "#PWR01" H 9150 3550 30  0001 C CNN
F 1 "VCC" H 9150 3550 30  0000 C CNN
	1    9150 3450
	1    0    0    -1  
$EndComp
Text Notes 9450 3750 0    30   ~ 0
when board powered externally
Text Notes 9450 3700 0    30   ~ 0
Polarity Protection
Wire Wire Line
	9150 3850 9150 3950
Wire Wire Line
	8675 4175 8350 4175
Connection ~ 4675 3475
Wire Wire Line
	4975 3475 4400 3475
Connection ~ 7375 3600
Wire Wire Line
	7200 3600 7425 3600
Wire Bus Line
	5050 3725 5050 5175
Wire Bus Line
	5050 3725 4175 3725
Wire Wire Line
	4275 4575 4275 4550
Wire Wire Line
	4275 4550 4075 4550
Wire Wire Line
	3275 3800 3275 3725
Connection ~ 4325 2025
Wire Wire Line
	4275 2025 4400 2025
Wire Wire Line
	4400 2025 4400 2175
Wire Wire Line
	4400 2175 4575 2175
Wire Wire Line
	4575 2175 4575 2350
Wire Wire Line
	4575 2350 4850 2350
Wire Wire Line
	4850 2350 4850 2525
Wire Wire Line
	4850 2525 5075 2525
Wire Wire Line
	5075 2525 5075 2625
Wire Wire Line
	5075 2625 5150 2625
Connection ~ 4075 2325
Wire Wire Line
	4025 2325 4075 2325
Connection ~ 7350 4175
Connection ~ 7450 4275
Wire Wire Line
	7175 4275 8050 4275
Connection ~ 7700 4275
Connection ~ 4825 3475
Wire Wire Line
	8150 4275 8150 4175
Connection ~ 4900 3475
Connection ~ 4900 3525
Wire Wire Line
	4900 3625 4900 3475
Connection ~ 4975 3225
Wire Wire Line
	4975 3225 4975 3275
Wire Wire Line
	8675 4175 8675 4325
Connection ~ 5150 3525
Wire Wire Line
	5150 3475 5150 3575
Connection ~ 7675 3600
Wire Wire Line
	7625 3600 7675 3600
Wire Wire Line
	6750 4275 6975 4275
Wire Wire Line
	7300 4875 7300 4950
Wire Wire Line
	7675 2850 7675 2950
Wire Wire Line
	7275 3075 7400 3075
Wire Wire Line
	7075 2850 7075 3075
Wire Wire Line
	8250 4325 8250 4175
Connection ~ 4750 2825
Wire Wire Line
	4750 2775 4750 3125
Connection ~ 4925 2925
Wire Wire Line
	4550 1925 4550 2025
Wire Wire Line
	4950 2150 4825 2150
Wire Wire Line
	4750 3125 5150 3125
Wire Wire Line
	4500 3025 5150 3025
Wire Wire Line
	4500 2825 4750 2825
Wire Wire Line
	5150 2925 4500 2925
Connection ~ 5150 2150
Wire Wire Line
	8050 4275 8050 4175
Wire Wire Line
	7075 3075 6750 3075
Wire Bus Line
	6850 2575 6850 5175
Wire Bus Line
	6850 5175 5050 5175
Wire Wire Line
	7700 4875 7700 4950
Wire Wire Line
	7700 4375 7700 4275
Wire Wire Line
	6975 4175 6750 4175
Wire Wire Line
	7375 3550 7375 3600
Wire Wire Line
	7375 3850 7375 3800
Wire Wire Line
	5150 3525 4900 3525
Wire Wire Line
	8350 4175 8350 4950
Wire Wire Line
	8350 4950 7300 4950
Connection ~ 7700 4950
Wire Wire Line
	5150 3225 4925 3225
Wire Wire Line
	4925 3225 4925 2775
Wire Wire Line
	7675 3600 7675 3550
Wire Wire Line
	4975 3275 4825 3275
Wire Wire Line
	4675 3275 4675 3025
Connection ~ 4675 3025
Wire Wire Line
	7300 4175 7300 4375
Connection ~ 7650 4275
Connection ~ 7300 4175
Wire Wire Line
	7950 4175 7175 4175
Connection ~ 7550 4175
Wire Wire Line
	4075 2625 4075 2025
Wire Wire Line
	4275 2625 5025 2625
Wire Wire Line
	5025 2625 5025 2725
Wire Wire Line
	5025 2725 5150 2725
Connection ~ 4325 2625
Wire Wire Line
	3275 4475 3275 4550
Wire Wire Line
	3275 5350 3275 5275
Wire Wire Line
	4075 3800 4275 3800
Wire Wire Line
	4275 3800 4275 3825
Wire Wire Line
	4075 5350 4275 5350
Wire Wire Line
	4275 5350 4275 5375
Wire Bus Line
	3175 4000 3175 6000
Wire Bus Line
	3175 6000 4175 6000
Wire Bus Line
	4175 6000 4175 3725
Wire Wire Line
	7200 3800 7675 3800
Connection ~ 7375 3800
Wire Wire Line
	4400 3275 4400 3225
Wire Wire Line
	5150 2475 5150 1850
Connection ~ 5150 1925
Wire Wire Line
	4925 2775 4975 2775
Wire Wire Line
	9150 3550 9150 3450
$Comp
L D_RECTIFIER D1
U 1 1 4901E150
P 9150 3700
F 0 "D1" V 9060 3650 35  0000 C CNN
F 1 "D_RECTIFIER" V 9230 3620 30  0000 C CNN
	1    9150 3700
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR02
U 1 1 4901E13D
P 9150 3950
F 0 "#PWR02" H 9150 3950 30  0001 C CNN
F 1 "GND" H 9150 3880 30  0001 C CNN
	1    9150 3950
	1    0    0    -1  
$EndComp
$Comp
L VIA U1
U 1 1 4901D8AB
P 4975 2775
F 0 "U1" V 5000 2875 20  0000 C CNN
F 1 "VIA" H 4975 2975 60  0001 C CNN
	1    4975 2775
	0    1    1    0   
$EndComp
$Comp
L CONN_1 P5
U 1 1 48BBAB5F
P 5150 1700
F 0 "P5" V 5200 1700 40  0000 L CNN
F 1 "CONN_1" H 5150 1755 30  0001 C CNN
	1    5150 1700
	0    -1   -1   0   
$EndComp
NoConn ~ 3275 5750
$Comp
L VCC #PWR03
U 1 1 48BB918B
P 4400 3225
F 0 "#PWR03" H 4400 3325 30  0001 C CNN
F 1 "VCC" H 4400 3325 30  0000 C CNN
	1    4400 3225
	1    0    0    -1  
$EndComp
$Comp
L C_MINI C3
U 1 1 48BB90E5
P 4400 3375
F 0 "C3" V 4350 3415 30  0000 C CNN
F 1 "1uF" V 4450 3425 25  0000 C CNN
	1    4400 3375
	0    1    1    0   
$EndComp
$Comp
L C_MINI C5
U 1 1 48BB8F00
P 7200 3700
F 0 "C5" V 7150 3740 30  0000 C CNN
F 1 "1uF" V 7250 3750 25  0000 C CNN
	1    7200 3700
	0    1    1    0   
$EndComp
$Comp
L CP_TANTALUM_MINI C7
U 1 1 489B85E8
P 7675 3700
F 0 "C7" V 7625 3740 30  0000 C CNN
F 1 "4.7uF" V 7725 3770 25  0000 C CNN
	1    7675 3700
	0    1    1    0   
$EndComp
NoConn ~ 4075 5750
NoConn ~ 3275 5650
$Comp
L CONN_5X2 P8
U 1 1 48BB5927
P 3675 5550
F 0 "P8" H 3675 5850 60  0000 C CNN
F 1 "PortC" V 3675 5550 50  0000 C CNN
	1    3675 5550
	1    0    0    1   
$EndComp
Entry Wire Line
	3175 5650 3275 5550
Entry Wire Line
	3175 5550 3275 5450
$Comp
L VCC #PWR04
U 1 1 48BB590B
P 3275 5275
F 0 "#PWR04" H 3275 5375 30  0001 C CNN
F 1 "VCC" H 3275 5375 30  0000 C CNN
	1    3275 5275
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 48BB5909
P 4275 5375
F 0 "#PWR05" H 4275 5375 30  0001 C CNN
F 1 "GND" H 4275 5305 30  0001 C CNN
	1    4275 5375
	1    0    0    -1  
$EndComp
Entry Wire Line
	4075 5650 4175 5750
Entry Wire Line
	4075 5550 4175 5650
Entry Wire Line
	4075 5450 4175 5550
Entry Wire Line
	3175 4300 3275 4200
Entry Wire Line
	3175 4200 3275 4100
Entry Wire Line
	3175 4100 3275 4000
Entry Wire Line
	3175 4000 3275 3900
$Comp
L VCC #PWR06
U 1 1 48BB58B9
P 3275 3725
F 0 "#PWR06" H 3275 3825 30  0001 C CNN
F 1 "VCC" H 3275 3825 30  0000 C CNN
	1    3275 3725
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 P6
U 1 1 48BB58B8
P 3675 4000
F 0 "P6" H 3675 4300 60  0000 C CNN
F 1 "PortB" V 3675 4000 50  0000 C CNN
	1    3675 4000
	1    0    0    1   
$EndComp
$Comp
L GND #PWR07
U 1 1 48BB58B7
P 4275 3825
F 0 "#PWR07" H 4275 3825 30  0001 C CNN
F 1 "GND" H 4275 3755 30  0001 C CNN
	1    4275 3825
	1    0    0    -1  
$EndComp
Entry Wire Line
	4075 4100 4175 4200
Entry Wire Line
	4075 4000 4175 4100
Entry Wire Line
	4075 4200 4175 4300
Entry Wire Line
	4075 3900 4175 4000
Entry Wire Line
	3175 5050 3275 4950
Entry Wire Line
	3175 4950 3275 4850
Entry Wire Line
	3175 4850 3275 4750
Entry Wire Line
	3175 4750 3275 4650
$Comp
L VCC #PWR08
U 1 1 48BB588A
P 3275 4475
F 0 "#PWR08" H 3275 4575 30  0001 C CNN
F 1 "VCC" H 3275 4575 30  0000 C CNN
	1    3275 4475
	1    0    0    -1  
$EndComp
$Comp
L CONN_5X2 P7
U 1 1 48BB5813
P 3675 4750
F 0 "P7" H 3675 5050 60  0000 C CNN
F 1 "PortD" V 3675 4750 50  0000 C CNN
	1    3675 4750
	1    0    0    1   
$EndComp
$Comp
L CRYSTAL X1
U 1 1 48B9EF2F
P 4325 2325
F 0 "X1" V 4350 2475 60  0000 C CNN
F 1 "CRYSTAL" V 4500 2575 60  0000 C CNN
	1    4325 2325
	0    1    1    0   
$EndComp
$Comp
L CONN_1 via4
U 1 1 48B9CB05
P 7650 4125
F 0 "via4" V 7700 4125 40  0000 L CNN
F 1 "CONN_1" H 7650 4180 30  0001 C CNN
	1    7650 4125
	0    -1   -1   0   
$EndComp
$Comp
L CONN_1 via3
U 1 1 48B9CB04
P 7550 4025
F 0 "via3" V 7600 4025 40  0000 L CNN
F 1 "CONN_1" H 7550 4080 30  0001 C CNN
	1    7550 4025
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR09
U 1 1 489EDCA1
P 4025 2325
F 0 "#PWR09" H 4025 2325 30  0001 C CNN
F 1 "GND" H 4025 2255 30  0001 C CNN
	1    4025 2325
	0    1    1    0   
$EndComp
Text Notes 4900 3390 2    15   ~ 0
Poly-Film
$Comp
L VPP #PWR19
U 1 1 489BA0D5
P 8150 4275
F 0 "#PWR19" H 8150 4475 40  0001 C CNN
F 1 "VPP" H 8150 4425 40  0000 C CNN
	1    8150 4275
	-1   0    0    1   
$EndComp
$Comp
L VPP #PWR18
U 1 1 489BA0D2
P 7675 3550
F 0 "#PWR18" H 7675 3750 40  0001 C CNN
F 1 "VPP" H 7675 3700 40  0000 C CNN
	1    7675 3550
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 via2
U 1 1 489B9080
P 7450 4125
F 0 "via2" V 7500 4125 40  0000 L CNN
F 1 "CONN_1" H 7450 4180 30  0001 C CNN
	1    7450 4125
	0    -1   -1   0   
$EndComp
$Comp
L CONN_1 via1
U 1 1 489B907F
P 7350 4025
F 0 "via1" V 7400 4025 40  0000 L CNN
F 1 "CONN_1" H 7350 4080 30  0001 C CNN
	1    7350 4025
	0    -1   -1   0   
$EndComp
$Comp
L C_MINI C6
U 1 1 489B8692
P 4825 3375
F 0 "C6" V 4775 3425 30  0000 C CNN
F 1 "0.1uF" V 4875 3445 25  0000 C CNN
	1    4825 3375
	0    1    1    0   
$EndComp
Text Notes 7735 3705 2    15   ~ 0
Tantalum
$Comp
L L_MINI L3
U 1 1 489B83F5
P 7525 3600
F 0 "L3" H 7450 3550 30  0000 C CNN
F 1 "47nH" H 7575 3550 22  0000 C CNN
	1    7525 3600
	-1   0    0    1   
$EndComp
$Comp
L VDD #PWR010
U 1 1 4893E23A
P 7375 3550
F 0 "#PWR010" H 7375 3650 30  0001 C CNN
F 1 "VDD" H 7375 3660 30  0000 C CNN
	1    7375 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 4893E231
P 7375 3850
F 0 "#PWR011" H 7375 3850 30  0001 C CNN
F 1 "GND" H 7375 3780 30  0001 C CNN
	1    7375 3850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 4893DE36
P 4275 4575
F 0 "#PWR012" H 4275 4575 30  0001 C CNN
F 1 "GND" H 4275 4505 30  0001 C CNN
	1    4275 4575
	1    0    0    -1  
$EndComp
Text Label 6750 3475 2    20   ~ 0
PD3
Text Label 3275 4850 0    20   ~ 0
PD3
Text Label 6750 3775 2    20   ~ 0
PD0
Text Label 6750 3675 2    20   ~ 0
PD1
Text Label 6750 3575 2    20   ~ 0
PD2
Text Label 6750 3375 2    20   ~ 0
PD4
Text Label 6750 3275 2    20   ~ 0
PD5
Text Label 6750 3175 2    20   ~ 0
PD6
Text Label 6750 3075 2    20   ~ 0
PD7
Text Label 5150 4475 0    20   ~ 0
PB0
Text Label 5150 4375 0    20   ~ 0
PB1
Text Label 5150 4275 0    20   ~ 0
PB2
Text Label 5150 4175 0    20   ~ 0
PB3
Text Label 5150 4075 0    20   ~ 0
PB4
Text Label 5150 3975 0    20   ~ 0
PB5
Text Label 5150 3875 0    20   ~ 0
PB6
Text Label 5150 3775 0    20   ~ 0
PB7
Text Label 6750 2875 2    20   ~ 0
PC7
Text Label 6750 2775 2    20   ~ 0
PC6
Text Label 6750 2675 2    20   ~ 0
PC5
Text Label 6750 2575 2    20   ~ 0
PC4
Text Label 6750 2475 2    20   ~ 0
PC2
Text Label 4075 4950 2    20   ~ 0
PD0
Text Label 3275 4950 0    20   ~ 0
PD1
Text Label 4075 4850 2    20   ~ 0
PD2
Text Label 4075 4750 2    20   ~ 0
PD4
Text Label 3275 4750 0    20   ~ 0
PD5
Text Label 4075 4650 2    20   ~ 0
PD6
Text Label 3275 4650 0    20   ~ 0
PD7
Text Label 4075 4200 2    20   ~ 0
PB0
Text Label 3275 4200 0    20   ~ 0
PB1
Text Label 4075 4100 2    20   ~ 0
PB2
Text Label 3275 4100 0    20   ~ 0
PB3
Text Label 4075 4000 2    20   ~ 0
PB4
Text Label 3275 4000 0    20   ~ 0
PB5
Text Label 4075 3900 2    20   ~ 0
PB6
Text Label 3275 3900 0    20   ~ 0
PB7
Text Label 3275 5450 0    20   ~ 0
PC7
Text Label 4075 5450 2    20   ~ 0
PC6
Text Label 3275 5550 0    20   ~ 0
PC5
Text Label 4075 5550 2    20   ~ 0
PC4
Text Label 4075 5650 2    20   ~ 0
PC2
Entry Wire Line
	4075 4850 4175 4950
Entry Wire Line
	4075 4750 4175 4850
Entry Wire Line
	4075 4950 4175 5050
Entry Wire Line
	4075 4650 4175 4750
Entry Wire Line
	5050 4575 5150 4475
Entry Wire Line
	5050 4475 5150 4375
Entry Wire Line
	5050 4375 5150 4275
Entry Wire Line
	5050 4275 5150 4175
Entry Wire Line
	5050 4175 5150 4075
Entry Wire Line
	5050 4075 5150 3975
Entry Wire Line
	5050 3975 5150 3875
Entry Wire Line
	5050 3875 5150 3775
Entry Wire Line
	6750 3775 6850 3875
Entry Wire Line
	6750 3675 6850 3775
Entry Wire Line
	6750 3575 6850 3675
Entry Wire Line
	6750 3475 6850 3575
Entry Wire Line
	6750 3375 6850 3475
Entry Wire Line
	6750 3275 6850 3375
Entry Wire Line
	6750 3175 6850 3275
Entry Wire Line
	6750 3075 6850 3175
Entry Wire Line
	6750 2875 6850 2975
Entry Wire Line
	6750 2775 6850 2875
Entry Wire Line
	6750 2675 6850 2775
Entry Wire Line
	6750 2575 6850 2675
Entry Wire Line
	6750 2475 6850 2575
$Comp
L R_MINI R1
U 1 1 4893DBAC
P 7075 4275
F 0 "R1" H 7005 4325 25  0000 C CNN
F 1 "22" H 7145 4325 20  0000 C CNN
	1    7075 4275
	1    0    0    -1  
$EndComp
$Comp
L R_MINI R2
U 1 1 4893DBA9
P 7075 4175
F 0 "R2" H 7005 4225 25  0000 C CNN
F 1 "22" H 7145 4225 20  0000 C CNN
	1    7075 4175
	1    0    0    -1  
$EndComp
$Comp
L C_MINI C9
U 1 1 4893D99C
P 7375 3700
F 0 "C9" V 7425 3650 30  0000 C CNN
F 1 "0.1uF" V 7325 3600 25  0000 C CNN
	1    7375 3700
	0    -1   -1   0   
$EndComp
$Comp
L C_MINI C8
U 1 1 4893D94F
P 4975 3375
F 0 "C8" V 4925 3415 30  0000 C CNN
F 1 "1uF" V 5025 3425 25  0000 C CNN
	1    4975 3375
	0    1    1    0   
$EndComp
$Comp
L R_MINI R4
U 1 1 4893D735
P 7175 3075
F 0 "R4" H 7235 3120 25  0000 C CNN
F 1 "47k" H 7105 3120 20  0000 C CNN
	1    7175 3075
	-1   0    0    -1  
$EndComp
$Comp
L VCC #PWR013
U 1 1 4893D733
P 7400 3075
F 0 "#PWR013" H 7400 3175 30  0001 C CNN
F 1 "VCC" H 7400 3175 30  0000 C CNN
	1    7400 3075
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 4893D731
P 7675 2950
F 0 "#PWR014" H 7675 2950 30  0001 C CNN
F 1 "GND" H 7675 2880 30  0001 C CNN
	1    7675 2950
	-1   0    0    -1  
$EndComp
$Comp
L SW_PUSH SW2
U 1 1 4893D730
P 7375 2850
F 0 "SW2" H 7245 2960 50  0000 C CNN
F 1 "HWB" H 7555 2960 50  0000 C CNN
	1    7375 2850
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 4893D6FB
P 8675 4325
F 0 "#PWR015" H 8675 4325 30  0001 C CNN
F 1 "GND" H 8675 4255 30  0001 C CNN
	1    8675 4325
	1    0    0    -1  
$EndComp
$Comp
L C_MINI C4
U 1 1 4893D53C
P 4675 3375
F 0 "C4" V 4625 3415 30  0000 C CNN
F 1 "1uF" V 4725 3425 25  0000 C CNN
	1    4675 3375
	0    1    1    0   
$EndComp
$Comp
L C_MINI C2
U 1 1 4893D446
P 4175 2625
F 0 "C2" H 4115 2590 30  0000 C CNN
F 1 "18pF" H 4255 2590 25  0000 C CNN
	1    4175 2625
	1    0    0    -1  
$EndComp
$Comp
L C_MINI C1
U 1 1 4893D443
P 4175 2025
F 0 "C1" H 4115 1990 30  0000 C CNN
F 1 "18pF" H 4255 1990 25  0000 C CNN
	1    4175 2025
	1    0    0    -1  
$EndComp
$Comp
L R_MINI R3
U 1 1 4893D306
P 5050 2150
F 0 "R3" H 4980 2200 25  0000 C CNN
F 1 "47k" H 5120 2200 20  0000 C CNN
	1    5050 2150
	1    0    0    -1  
$EndComp
$Comp
L USBCONN J1
U 1 1 484E10D0
P 8150 3925
F 0 "J1" H 8200 3625 60  0000 C CNN
F 1 "USBCONN" H 8350 4225 40  0000 C CNN
	1    8150 3925
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR016
U 1 1 480FE811
P 4825 2150
F 0 "#PWR016" H 4825 2250 30  0001 C CNN
F 1 "VCC" H 4825 2250 30  0000 C CNN
	1    4825 2150
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR017
U 1 1 480FE7D9
P 4925 2775
F 0 "#PWR017" H 4925 2875 30  0001 C CNN
F 1 "VCC" H 4925 2875 30  0000 C CNN
	1    4925 2775
	1    0    0    -1  
$EndComp
$Comp
L AT90USB16 IC1
U 1 1 480FE7A8
P 5950 3475
F 0 "IC1" H 6400 2325 50  0000 L BNN
F 1 "AT90USB16" H 5350 2325 50  0000 L BNN
F 2 "TQFP32" H 5950 3625 50  0001 C CNN
	1    5950 3475
	1    0    0    -1  
$EndComp
$Comp
L ESD_ZENER CR2
U 1 1 47FBC1D2
P 7700 4625
F 0 "CR2" H 7700 4500 60  0000 C CNN
F 1 "ESD_ZENER" H 7700 4775 60  0000 C CNN
	1    7700 4625
	0    1    1    0   
$EndComp
$Comp
L ESD_ZENER CR1
U 1 1 47FBC085
P 7300 4625
F 0 "CR1" H 7300 4500 60  0000 C CNN
F 1 "ESD_ZENER" H 7300 4775 60  0000 C CNN
	1    7300 4625
	0    1    1    0   
$EndComp
$Comp
L GND #PWR018
U 1 1 47FBB68B
P 4550 2025
F 0 "#PWR018" H 4550 2025 30  0001 C CNN
F 1 "GND" H 4550 1955 30  0001 C CNN
	1    4550 2025
	1    0    0    -1  
$EndComp
$Comp
L VDD #PWR019
U 1 1 47FBB4C5
P 4750 2775
F 0 "#PWR019" H 4750 2875 30  0001 C CNN
F 1 "VDD" H 4750 2885 30  0000 C CNN
	1    4750 2775
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 47FBB47F
P 4900 3625
F 0 "#PWR020" H 4900 3625 30  0001 C CNN
F 1 "GND" H 4900 3555 30  0001 C CNN
	1    4900 3625
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSH SW1
U 1 1 47FBB3F1
P 4850 1925
F 0 "SW1" H 5000 2035 50  0000 C CNN
F 1 "RESET" H 4675 2025 50  0000 C CNN
	1    4850 1925
	1    0    0    -1  
$EndComp
Text Notes 2475 2975 0    40   ~ 0
or UVCC(5V-450mA) is used for Vcc
Text Notes 2475 2875 0    40   ~ 0
Jumper K1 selects whether UCAP(3.3V-50mA)
$Comp
L CONN_3 K1
U 1 1 47FBB096
P 4150 2925
F 0 "K1" V 4100 2925 50  0000 C CNN
F 1 "Vcc_Sel" V 4200 2925 40  0000 C CNN
	1    4150 2925
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR021
U 1 1 47FBAEAB
P 8250 4325
F 0 "#PWR021" H 8250 4325 30  0001 C CNN
F 1 "GND" H 8250 4255 30  0001 C CNN
	1    8250 4325
	1    0    0    -1  
$EndComp
$EndSCHEMATC
