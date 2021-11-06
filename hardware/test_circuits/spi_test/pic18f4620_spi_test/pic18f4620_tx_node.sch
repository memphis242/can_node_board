EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
Title "PIC18F4620 Set Up as TX Node"
Date "2021-11-06"
Rev "1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Author: Abdullah Almosalami"
$EndDescr
$Comp
L can_node_board_hw:PIC18F4620-IP U?
U 1 1 61A4C010
P 3750 4000
AR Path="/61A4C010" Ref="U?"  Part="1" 
AR Path="/619FAD4D/61A4C010" Ref="U?"  Part="1" 
F 0 "U?" H 3725 5865 50  0000 C CNN
F 1 "PIC18F4620-IP" H 3725 5774 50  0000 C CNN
F 2 "" H 3600 4550 50  0001 C CNN
F 3 "" H 3600 4550 50  0001 C CNN
	1    3750 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61A4C016
P 2350 2200
AR Path="/61A4C016" Ref="R?"  Part="1" 
AR Path="/619FAD4D/61A4C016" Ref="R?"  Part="1" 
F 0 "R?" H 2420 2246 50  0000 L CNN
F 1 "10K" H 2420 2155 50  0000 L CNN
F 2 "" V 2280 2200 50  0001 C CNN
F 3 "~" H 2350 2200 50  0001 C CNN
	1    2350 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61A4C01C
P 2100 2500
AR Path="/61A4C01C" Ref="R?"  Part="1" 
AR Path="/619FAD4D/61A4C01C" Ref="R?"  Part="1" 
F 0 "R?" V 1893 2500 50  0000 C CNN
F 1 "51" V 1984 2500 50  0000 C CNN
F 2 "" V 2030 2500 50  0001 C CNN
F 3 "~" H 2100 2500 50  0001 C CNN
	1    2100 2500
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x05_Female J?
U 1 1 61A4C022
P 1700 1300
AR Path="/61A4C022" Ref="J?"  Part="1" 
AR Path="/619FAD4D/61A4C022" Ref="J?"  Part="1" 
F 0 "J?" H 1800 1300 50  0000 C CNN
F 1 "Conn_01x05_Female" H 1600 1700 50  0001 C CNN
F 2 "" H 1700 1300 50  0001 C CNN
F 3 "~" H 1700 1300 50  0001 C CNN
	1    1700 1300
	-1   0    0    -1  
$EndComp
Text GLabel 2000 1100 2    40   Input ~ 0
~MCLR
Text GLabel 2000 1400 2    40   Input ~ 0
PGD
Text GLabel 2000 1500 2    40   Input ~ 0
PGC
$Comp
L power:+5V #PWR?
U 1 1 61A4C02B
P 2350 1150
AR Path="/61A4C02B" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C02B" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2350 1000 50  0001 C CNN
F 1 "+5V" H 2365 1323 50  0000 C CNN
F 2 "" H 2350 1150 50  0001 C CNN
F 3 "" H 2350 1150 50  0001 C CNN
	1    2350 1150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A4C031
P 2350 1350
AR Path="/61A4C031" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C031" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2350 1100 50  0001 C CNN
F 1 "GND" H 2355 1177 50  0000 C CNN
F 2 "" H 2350 1350 50  0001 C CNN
F 3 "" H 2350 1350 50  0001 C CNN
	1    2350 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 1200 1900 1200
Wire Wire Line
	1900 1300 2350 1300
Wire Wire Line
	1900 1100 2000 1100
Wire Wire Line
	1900 1400 2000 1400
Wire Wire Line
	1900 1500 2000 1500
Wire Wire Line
	2350 1200 2350 1150
Wire Wire Line
	2350 1300 2350 1350
Text GLabel 1800 2500 0    40   Input ~ 0
~MCLR
Wire Wire Line
	1800 2500 1950 2500
Wire Wire Line
	2250 2500 2350 2500
$Comp
L power:+5V #PWR?
U 1 1 61A4C041
P 2350 1950
AR Path="/61A4C041" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C041" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2350 1800 50  0001 C CNN
F 1 "+5V" H 2365 2123 50  0000 C CNN
F 2 "" H 2350 1950 50  0001 C CNN
F 3 "" H 2350 1950 50  0001 C CNN
	1    2350 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 2500 2350 2350
Connection ~ 2350 2500
Wire Wire Line
	2350 2500 2500 2500
Wire Wire Line
	2350 2050 2350 1950
Text Notes 1500 2250 0    25   ~ 0
51 ohm resistor in series /w MCLR\nimportant! Seen videos where PICKit 4\nbricks uCs because of voltage spikes\non ~MCLR.
NoConn ~ 2500 2650
NoConn ~ 2500 2800
Wire Wire Line
	5100 2800 5100 2850
Wire Wire Line
	5100 3200 5100 3150
$Comp
L power:GND #PWR?
U 1 1 61A4C07C
P 5100 3200
AR Path="/61A4C07C" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C07C" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5100 2950 50  0001 C CNN
F 1 "GND" H 5105 3027 50  0000 C CNN
F 2 "" H 5100 3200 50  0001 C CNN
F 3 "" H 5100 3200 50  0001 C CNN
	1    5100 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2800 5100 2800
$Comp
L Device:R R?
U 1 1 61A4C083
P 5100 3000
AR Path="/61A4C083" Ref="R?"  Part="1" 
AR Path="/619FAD4D/61A4C083" Ref="R?"  Part="1" 
F 0 "R?" H 5170 3046 50  0000 L CNN
F 1 "R" H 5170 2955 50  0000 L CNN
F 2 "" V 5030 3000 50  0001 C CNN
F 3 "~" H 5100 3000 50  0001 C CNN
	1    5100 3000
	1    0    0    -1  
$EndComp
NoConn ~ 4950 2950
NoConn ~ 4950 3100
NoConn ~ 4950 3250
NoConn ~ 4950 3400
NoConn ~ 4950 3550
Text GLabel 4950 2650 2    40   Input ~ 0
PGC
Text GLabel 4950 2500 2    40   Input ~ 0
PGD
$Comp
L Device:L_Small L?
U 1 1 61A4C090
P 1900 2900
AR Path="/61A4C090" Ref="L?"  Part="1" 
AR Path="/619FAD4D/61A4C090" Ref="L?"  Part="1" 
F 0 "L?" V 2085 2900 50  0000 C CNN
F 1 "100nH" V 1994 2900 50  0000 C CNN
F 2 "" H 1900 2900 50  0001 C CNN
F 3 "~" H 1900 2900 50  0001 C CNN
	1    1900 2900
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 61A4C096
P 1650 2850
AR Path="/61A4C096" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C096" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1650 2700 50  0001 C CNN
F 1 "+5V" H 1665 3023 50  0000 C CNN
F 2 "" H 1650 2850 50  0001 C CNN
F 3 "" H 1650 2850 50  0001 C CNN
	1    1650 2850
	1    0    0    -1  
$EndComp
$Comp
L power:+5VA #PWR?
U 1 1 61A4C09C
P 2100 2850
AR Path="/61A4C09C" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C09C" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2100 2700 50  0001 C CNN
F 1 "+5VA" H 2115 3023 50  0000 C CNN
F 2 "" H 2100 2850 50  0001 C CNN
F 3 "" H 2100 2850 50  0001 C CNN
	1    2100 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 2850 1650 2900
Wire Wire Line
	1650 2900 1800 2900
Wire Wire Line
	2100 2900 2100 2850
Wire Wire Line
	2000 2900 2100 2900
Connection ~ 1650 2900
Wire Wire Line
	1650 2900 1650 2950
Wire Wire Line
	2100 2950 2100 2900
Connection ~ 2100 2900
Wire Wire Line
	2500 3150 2500 3100
Wire Wire Line
	2100 2900 2500 2900
Wire Wire Line
	2500 2900 2500 2950
Wire Wire Line
	2100 3150 2500 3150
Wire Wire Line
	1900 3250 1900 3200
$Comp
L power:GND #PWR?
U 1 1 61A4C0AF
P 1900 3250
AR Path="/61A4C0AF" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C0AF" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1900 3000 50  0001 C CNN
F 1 "GND" H 1905 3077 50  0000 C CNN
F 2 "" H 1900 3250 50  0001 C CNN
F 3 "" H 1900 3250 50  0001 C CNN
	1    1900 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A4C0B5
P 1650 3050
AR Path="/61A4C0B5" Ref="C?"  Part="1" 
AR Path="/619FAD4D/61A4C0B5" Ref="C?"  Part="1" 
F 0 "C?" H 1742 3096 50  0000 L CNN
F 1 "1uF" H 1742 3005 50  0000 L CNN
F 2 "" H 1650 3050 50  0001 C CNN
F 3 "~" H 1650 3050 50  0001 C CNN
	1    1650 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A4C0BB
P 2100 3050
AR Path="/61A4C0BB" Ref="C?"  Part="1" 
AR Path="/619FAD4D/61A4C0BB" Ref="C?"  Part="1" 
F 0 "C?" H 2192 3096 50  0000 L CNN
F 1 "10nF" H 2192 3005 50  0000 L CNN
F 2 "" H 2100 3050 50  0001 C CNN
F 3 "~" H 2100 3050 50  0001 C CNN
	1    2100 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 3150 1650 3200
Wire Wire Line
	1650 3200 1900 3200
Wire Wire Line
	1900 3200 2100 3200
Wire Wire Line
	2100 3200 2100 3150
Connection ~ 1900 3200
Connection ~ 2100 3150
NoConn ~ 2500 3250
NoConn ~ 2500 3400
Text GLabel 2500 3550 0    50   Input ~ 0
SW1
Text GLabel 2500 3700 0    50   Input ~ 0
SW2
$Comp
L power:+5V #PWR?
U 1 1 61A4C0CB
P 5750 6350
AR Path="/61A4C0CB" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C0CB" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5750 6200 50  0001 C CNN
F 1 "+5V" H 5765 6523 50  0000 C CNN
F 2 "" H 5750 6350 50  0001 C CNN
F 3 "" H 5750 6350 50  0001 C CNN
	1    5750 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 6450 5750 6350
Wire Wire Line
	5650 6450 5750 6450
Wire Wire Line
	5150 6850 5150 6800
Wire Wire Line
	5150 6450 5250 6450
Wire Wire Line
	5150 6500 5150 6450
Connection ~ 5150 6450
Wire Wire Line
	4950 6450 5150 6450
$Comp
L Switch:SW_Push SW?
U 1 1 61A4C0D8
P 5450 6450
AR Path="/61A4C0D8" Ref="SW?"  Part="1" 
AR Path="/619FAD4D/61A4C0D8" Ref="SW?"  Part="1" 
F 0 "SW?" H 5450 6735 50  0000 C CNN
F 1 "SW_Push" H 5450 6644 50  0000 C CNN
F 2 "" H 5450 6650 50  0001 C CNN
F 3 "~" H 5450 6650 50  0001 C CNN
	1    5450 6450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A4C0DE
P 5150 6850
AR Path="/61A4C0DE" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C0DE" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5150 6600 50  0001 C CNN
F 1 "GND" H 5155 6677 50  0000 C CNN
F 2 "" H 5150 6850 50  0001 C CNN
F 3 "" H 5150 6850 50  0001 C CNN
	1    5150 6850
	1    0    0    -1  
$EndComp
Text GLabel 4950 6450 0    50   Input ~ 0
SW2
$Comp
L Device:R R?
U 1 1 61A4C0E5
P 5150 6650
AR Path="/61A4C0E5" Ref="R?"  Part="1" 
AR Path="/619FAD4D/61A4C0E5" Ref="R?"  Part="1" 
F 0 "R?" H 5220 6696 50  0000 L CNN
F 1 "10k" H 5220 6605 50  0000 L CNN
F 2 "" V 5080 6650 50  0001 C CNN
F 3 "~" H 5150 6650 50  0001 C CNN
	1    5150 6650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 61A4C0EB
P 4600 6350
AR Path="/61A4C0EB" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C0EB" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4600 6200 50  0001 C CNN
F 1 "+5V" H 4615 6523 50  0000 C CNN
F 2 "" H 4600 6350 50  0001 C CNN
F 3 "" H 4600 6350 50  0001 C CNN
	1    4600 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 6450 4600 6350
Wire Wire Line
	4500 6450 4600 6450
Wire Wire Line
	4000 6850 4000 6800
Wire Wire Line
	4000 6450 4100 6450
Wire Wire Line
	4000 6500 4000 6450
Connection ~ 4000 6450
Wire Wire Line
	3800 6450 4000 6450
$Comp
L Switch:SW_Push SW?
U 1 1 61A4C0F8
P 4300 6450
AR Path="/61A4C0F8" Ref="SW?"  Part="1" 
AR Path="/619FAD4D/61A4C0F8" Ref="SW?"  Part="1" 
F 0 "SW?" H 4300 6735 50  0000 C CNN
F 1 "SW_Push" H 4300 6644 50  0000 C CNN
F 2 "" H 4300 6650 50  0001 C CNN
F 3 "~" H 4300 6650 50  0001 C CNN
	1    4300 6450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A4C0FE
P 4000 6850
AR Path="/61A4C0FE" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C0FE" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4000 6600 50  0001 C CNN
F 1 "GND" H 4005 6677 50  0000 C CNN
F 2 "" H 4000 6850 50  0001 C CNN
F 3 "" H 4000 6850 50  0001 C CNN
	1    4000 6850
	1    0    0    -1  
$EndComp
Text GLabel 3800 6450 0    50   Input ~ 0
SW1
$Comp
L Device:R R?
U 1 1 61A4C105
P 4000 6650
AR Path="/61A4C105" Ref="R?"  Part="1" 
AR Path="/619FAD4D/61A4C105" Ref="R?"  Part="1" 
F 0 "R?" H 4070 6696 50  0000 L CNN
F 1 "10k" H 4070 6605 50  0000 L CNN
F 2 "" V 3930 6650 50  0001 C CNN
F 3 "~" H 4000 6650 50  0001 C CNN
	1    4000 6650
	1    0    0    -1  
$EndComp
Text Notes 3550 6050 0    40   ~ 0
Push buttons whose state is read in by RE0 and RE1\nand then sent over SPI.
NoConn ~ 2500 3850
$Comp
L power:+5V #PWR?
U 1 1 61A4C10D
P 1650 3850
AR Path="/61A4C10D" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C10D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1650 3700 50  0001 C CNN
F 1 "+5V" H 1665 4023 50  0000 C CNN
F 2 "" H 1650 3850 50  0001 C CNN
F 3 "" H 1650 3850 50  0001 C CNN
	1    1650 3850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A4C113
P 1650 4300
AR Path="/61A4C113" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C113" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1650 4050 50  0001 C CNN
F 1 "GND" H 1655 4127 50  0000 C CNN
F 2 "" H 1650 4300 50  0001 C CNN
F 3 "" H 1650 4300 50  0001 C CNN
	1    1650 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A4C119
P 1650 4100
AR Path="/61A4C119" Ref="C?"  Part="1" 
AR Path="/619FAD4D/61A4C119" Ref="C?"  Part="1" 
F 0 "C?" H 1450 4150 50  0000 L CNN
F 1 "1uF" H 1450 4000 50  0000 L CNN
F 2 "" H 1650 4100 50  0001 C CNN
F 3 "~" H 1650 4100 50  0001 C CNN
	1    1650 4100
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A4C11F
P 1850 4100
AR Path="/61A4C11F" Ref="C?"  Part="1" 
AR Path="/619FAD4D/61A4C11F" Ref="C?"  Part="1" 
F 0 "C?" H 1950 4200 50  0000 L CNN
F 1 "10nF" H 1950 4100 50  0000 L CNN
F 2 "" H 1850 4100 50  0001 C CNN
F 3 "~" H 1850 4100 50  0001 C CNN
	1    1850 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 3900 2350 4000
Wire Wire Line
	2350 4000 2500 4000
Wire Wire Line
	5100 3600 5100 3700
Wire Wire Line
	5100 3700 4950 3700
Wire Wire Line
	4950 3850 5100 3850
Wire Wire Line
	5100 3850 5100 3950
Wire Wire Line
	1650 4300 1650 4250
Wire Wire Line
	1650 4000 1650 3900
Wire Wire Line
	1650 3900 1850 3900
Connection ~ 1650 3900
Wire Wire Line
	1650 3900 1650 3850
Wire Wire Line
	1650 4250 1850 4250
Connection ~ 1650 4250
Wire Wire Line
	1650 4250 1650 4200
Wire Wire Line
	1850 4250 1850 4200
Wire Wire Line
	1850 4000 1850 3900
Connection ~ 1850 3900
Wire Wire Line
	1850 3900 2350 3900
$Comp
L power:+5V #PWR?
U 1 1 61A4C137
P 5600 3550
AR Path="/61A4C137" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C137" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5600 3400 50  0001 C CNN
F 1 "+5V" H 5615 3723 50  0000 C CNN
F 2 "" H 5600 3550 50  0001 C CNN
F 3 "" H 5600 3550 50  0001 C CNN
	1    5600 3550
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A4C13D
P 5600 4000
AR Path="/61A4C13D" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C13D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5600 3750 50  0001 C CNN
F 1 "GND" H 5605 3827 50  0000 C CNN
F 2 "" H 5600 4000 50  0001 C CNN
F 3 "" H 5600 4000 50  0001 C CNN
	1    5600 4000
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A4C143
P 5600 3800
AR Path="/61A4C143" Ref="C?"  Part="1" 
AR Path="/619FAD4D/61A4C143" Ref="C?"  Part="1" 
F 0 "C?" H 5400 3850 50  0000 L CNN
F 1 "1uF" H 5400 3700 50  0000 L CNN
F 2 "" H 5600 3800 50  0001 C CNN
F 3 "~" H 5600 3800 50  0001 C CNN
	1    5600 3800
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A4C149
P 5400 3800
AR Path="/61A4C149" Ref="C?"  Part="1" 
AR Path="/619FAD4D/61A4C149" Ref="C?"  Part="1" 
F 0 "C?" H 5492 3846 50  0000 L CNN
F 1 "10nF" H 5492 3755 50  0000 L CNN
F 2 "" H 5400 3800 50  0001 C CNN
F 3 "~" H 5400 3800 50  0001 C CNN
	1    5400 3800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5600 4000 5600 3950
Wire Wire Line
	5600 3700 5600 3600
Wire Wire Line
	5600 3600 5400 3600
Connection ~ 5600 3600
Wire Wire Line
	5600 3600 5600 3550
Wire Wire Line
	5600 3950 5400 3950
Connection ~ 5600 3950
Wire Wire Line
	5600 3950 5600 3900
Wire Wire Line
	5400 3950 5400 3900
Connection ~ 5400 3950
Wire Wire Line
	5400 3950 5100 3950
Wire Wire Line
	5400 3700 5400 3600
Connection ~ 5400 3600
Wire Wire Line
	5400 3600 5100 3600
$Comp
L Device:Crystal_Small Y?
U 1 1 61A4C15D
P 1900 4750
AR Path="/61A4C15D" Ref="Y?"  Part="1" 
AR Path="/619FAD4D/61A4C15D" Ref="Y?"  Part="1" 
F 0 "Y?" H 1900 4975 50  0000 C CNN
F 1 "10MHz" H 1900 4884 50  0000 C CNN
F 2 "" H 1900 4750 50  0001 C CNN
F 3 "~" H 1900 4750 50  0001 C CNN
	1    1900 4750
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A4C163
P 1800 4950
AR Path="/61A4C163" Ref="C?"  Part="1" 
AR Path="/619FAD4D/61A4C163" Ref="C?"  Part="1" 
F 0 "C?" H 1600 5000 50  0000 L CNN
F 1 "20pF" H 1550 4850 50  0000 L CNN
F 2 "" H 1800 4950 50  0001 C CNN
F 3 "~" H 1800 4950 50  0001 C CNN
	1    1800 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A4C169
P 2000 4950
AR Path="/61A4C169" Ref="C?"  Part="1" 
AR Path="/619FAD4D/61A4C169" Ref="C?"  Part="1" 
F 0 "C?" H 2100 5000 50  0000 L CNN
F 1 "20pF" H 2050 4850 50  0000 L CNN
F 2 "" H 2000 4950 50  0001 C CNN
F 3 "~" H 2000 4950 50  0001 C CNN
	1    2000 4950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A4C16F
P 1900 5100
AR Path="/61A4C16F" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C16F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1900 4850 50  0001 C CNN
F 1 "GND" H 1905 4927 50  0000 C CNN
F 2 "" H 1900 5100 50  0001 C CNN
F 3 "" H 1900 5100 50  0001 C CNN
	1    1900 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 5100 1800 5100
Wire Wire Line
	1800 5100 1800 5050
Wire Wire Line
	1800 4850 1800 4750
Wire Wire Line
	2000 4750 2000 4850
Wire Wire Line
	2000 5100 1900 5100
Wire Wire Line
	2000 5050 2000 5100
Connection ~ 1900 5100
Text GLabel 1750 4750 0    40   Input ~ 0
OSC1
Text GLabel 2050 4750 2    40   Input ~ 0
OSC2
Wire Wire Line
	1750 4750 1800 4750
Connection ~ 1800 4750
Wire Wire Line
	2000 4750 2050 4750
Connection ~ 2000 4750
Wire Wire Line
	2100 4150 2100 4250
Wire Wire Line
	2100 4250 1850 4250
Wire Wire Line
	2100 4150 2500 4150
Connection ~ 1850 4250
Text GLabel 2500 4300 0    40   Input ~ 0
OSC1
NoConn ~ 2500 4600
NoConn ~ 2500 4750
NoConn ~ 2500 4900
NoConn ~ 2500 5200
NoConn ~ 2500 5350
Text GLabel 2500 5050 0    50   Input ~ 0
SCK
Text GLabel 4950 5050 2    50   Input ~ 0
MISO
Text GLabel 4950 4900 2    50   Input ~ 0
MOSI
NoConn ~ 4950 4750
NoConn ~ 4950 4600
NoConn ~ 4950 4450
NoConn ~ 4950 4300
NoConn ~ 4950 4150
NoConn ~ 4950 4000
Text Notes 2200 900  2    50   ~ 0
ICSP Header
Text Notes 1500 1550 1    30   ~ 0
Use with something like\na PICkit4.
Text Notes 1200 6900 0    150  ~ 30
TX NODE
$Comp
L power:GND #PWR?
U 1 1 61A4C1B2
P 5750 5650
AR Path="/61A4C1B2" Ref="#PWR?"  Part="1" 
AR Path="/619FAD4D/61A4C1B2" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5750 5400 50  0001 C CNN
F 1 "GND" H 5755 5477 50  0000 C CNN
F 2 "" H 5750 5650 50  0001 C CNN
F 3 "" H 5750 5650 50  0001 C CNN
	1    5750 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 61A4C1B8
P 5750 5450
AR Path="/61A4C1B8" Ref="D?"  Part="1" 
AR Path="/619FAD4D/61A4C1B8" Ref="D?"  Part="1" 
F 0 "D?" V 5789 5332 50  0000 R CNN
F 1 "LED" V 5698 5332 50  0000 R CNN
F 2 "" H 5750 5450 50  0001 C CNN
F 3 "~" H 5750 5450 50  0001 C CNN
	1    5750 5450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5750 5650 5750 5600
Wire Wire Line
	5750 5300 5750 5200
Text Notes 5200 5500 0    50   ~ 10
Debug LED
Wire Wire Line
	4950 5200 5350 5200
Wire Wire Line
	5650 5200 5750 5200
$Comp
L Device:R R?
U 1 1 61A4C1C3
P 5500 5200
AR Path="/61A4C1C3" Ref="R?"  Part="1" 
AR Path="/619FAD4D/61A4C1C3" Ref="R?"  Part="1" 
F 0 "R?" V 5293 5200 50  0000 C CNN
F 1 "4.7k" V 5384 5200 50  0000 C CNN
F 2 "" V 5430 5200 50  0001 C CNN
F 3 "~" H 5500 5200 50  0001 C CNN
	1    5500 5200
	0    1    1    0   
$EndComp
Text Notes 1250 7200 0    70   ~ 14
Read button states.\nSend state over SPI.
Text GLabel 4950 5350 2    50   Input ~ 0
~SS
Text GLabel 2500 4450 0    40   Input ~ 0
OSC2
$EndSCHEMATC
