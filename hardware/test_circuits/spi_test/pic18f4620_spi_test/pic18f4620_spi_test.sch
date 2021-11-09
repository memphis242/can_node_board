EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title "PIC18F4620 - MSSP SPI Test Circuit"
Date "2021-11-04"
Rev "1.0"
Comp "Personal Project"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Author: Abdullah Almosalami"
$EndDescr
$Comp
L can_node_board_hw:PIC18F4620-IP U1
U 1 1 6184BAE3
P 3400 4250
F 0 "U1" H 3375 6115 50  0000 C CNN
F 1 "PIC18F4620-IP" H 3375 6024 50  0000 C CNN
F 2 "" H 3250 4800 50  0001 C CNN
F 3 "" H 3250 4800 50  0001 C CNN
	1    3400 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 6184FCEC
P 2000 2450
F 0 "R2" H 2070 2496 50  0000 L CNN
F 1 "10K" H 2070 2405 50  0000 L CNN
F 2 "" V 1930 2450 50  0001 C CNN
F 3 "~" H 2000 2450 50  0001 C CNN
	1    2000 2450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 6184FE85
P 1750 2750
F 0 "R1" V 1543 2750 50  0000 C CNN
F 1 "51" V 1634 2750 50  0000 C CNN
F 2 "" V 1680 2750 50  0001 C CNN
F 3 "~" H 1750 2750 50  0001 C CNN
	1    1750 2750
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x05_Female J1
U 1 1 61851AFC
P 1350 1550
F 0 "J1" H 1450 1550 50  0000 C CNN
F 1 "Conn_01x05_Female" H 1250 1950 50  0001 C CNN
F 2 "" H 1350 1550 50  0001 C CNN
F 3 "~" H 1350 1550 50  0001 C CNN
	1    1350 1550
	-1   0    0    -1  
$EndComp
Text GLabel 1650 1350 2    40   Input ~ 0
~MCLR
Text GLabel 1650 1650 2    40   Input ~ 0
PGD
Text GLabel 1650 1750 2    40   Input ~ 0
PGC
$Comp
L power:+5V #PWR07
U 1 1 6185330E
P 2000 1400
F 0 "#PWR07" H 2000 1250 50  0001 C CNN
F 1 "+5V" H 2015 1573 50  0000 C CNN
F 2 "" H 2000 1400 50  0001 C CNN
F 3 "" H 2000 1400 50  0001 C CNN
	1    2000 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 61854162
P 2000 1600
F 0 "#PWR08" H 2000 1350 50  0001 C CNN
F 1 "GND" H 2005 1427 50  0000 C CNN
F 2 "" H 2000 1600 50  0001 C CNN
F 3 "" H 2000 1600 50  0001 C CNN
	1    2000 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 1450 1550 1450
Wire Wire Line
	1550 1550 2000 1550
Wire Wire Line
	1550 1350 1650 1350
Wire Wire Line
	1550 1650 1650 1650
Wire Wire Line
	1550 1750 1650 1750
Wire Wire Line
	2000 1450 2000 1400
Wire Wire Line
	2000 1550 2000 1600
Text GLabel 1450 2750 0    40   Input ~ 0
~MCLR
Wire Wire Line
	1450 2750 1600 2750
Wire Wire Line
	1900 2750 2000 2750
$Comp
L power:+5V #PWR09
U 1 1 61856454
P 2000 2200
F 0 "#PWR09" H 2000 2050 50  0001 C CNN
F 1 "+5V" H 2015 2373 50  0000 C CNN
F 2 "" H 2000 2200 50  0001 C CNN
F 3 "" H 2000 2200 50  0001 C CNN
	1    2000 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 2750 2000 2600
Connection ~ 2000 2750
Wire Wire Line
	2000 2750 2150 2750
Wire Wire Line
	2000 2300 2000 2200
Text Notes 1150 2500 0    25   ~ 0
51 ohm resistor in series /w MCLR\nimportant! Seen videos where PICKit 4\nbricks uCs because of voltage spikes\non ~MCLR.
NoConn ~ 2150 2900
NoConn ~ 2150 3050
$Comp
L Regulator_Linear:L7805 U2
U 1 1 61857303
P 4550 1250
F 0 "U2" H 4550 1492 50  0000 C CNN
F 1 "L7805" H 4550 1401 50  0000 C CNN
F 2 "" H 4575 1100 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 4550 1200 50  0001 C CNN
	1    4550 1250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 61857A6A
P 4550 1700
F 0 "#PWR013" H 4550 1450 50  0001 C CNN
F 1 "GND" H 4555 1527 50  0000 C CNN
F 2 "" H 4550 1700 50  0001 C CNN
F 3 "" H 4550 1700 50  0001 C CNN
	1    4550 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1700 4550 1650
$Comp
L Device:C_Small C7
U 1 1 6185829F
P 4100 1500
F 0 "C7" H 3900 1550 50  0000 L CNN
F 1 "0.1uF" H 3800 1450 50  0000 L CNN
F 2 "" H 4100 1500 50  0001 C CNN
F 3 "~" H 4100 1500 50  0001 C CNN
	1    4100 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C8
U 1 1 61858CED
P 5000 1500
F 0 "C8" H 5092 1546 50  0000 L CNN
F 1 "10uF" H 5092 1455 50  0000 L CNN
F 2 "" H 5000 1500 50  0001 C CNN
F 3 "~" H 5000 1500 50  0001 C CNN
	1    5000 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 1250 4100 1250
Wire Wire Line
	4100 1600 4100 1650
Wire Wire Line
	4100 1650 4550 1650
Connection ~ 4550 1650
Wire Wire Line
	4550 1650 4550 1550
Wire Wire Line
	5000 1600 5000 1650
Wire Wire Line
	5000 1650 4550 1650
Wire Wire Line
	4100 1400 4100 1250
Connection ~ 4100 1250
Wire Wire Line
	4100 1250 4250 1250
Wire Wire Line
	4850 1250 5000 1250
Wire Wire Line
	5000 1250 5000 1400
$Comp
L Device:D_Schottky D1
U 1 1 6185D69A
P 3900 1250
F 0 "D1" H 3900 1033 50  0000 C CNN
F 1 "D_Schottky" H 3900 1124 50  0000 C CNN
F 2 "" H 3900 1250 50  0001 C CNN
F 3 "~" H 3900 1250 50  0001 C CNN
	1    3900 1250
	-1   0    0    1   
$EndComp
Text Notes 3450 950  0    20   ~ 0
This is minimal input voltage protection (reverse polarity).\nWhen breadboarding, it's easy to make mistakes, so ideally,\nyou'd use more protection - clamp diodes to ground,\np-channel mosfet reverse-polarity protection, a fuse, etc.
Wire Wire Line
	4750 3050 4750 3100
Wire Wire Line
	4750 3450 4750 3400
$Comp
L power:GND #PWR014
U 1 1 618654D5
P 4750 3450
F 0 "#PWR014" H 4750 3200 50  0001 C CNN
F 1 "GND" H 4755 3277 50  0000 C CNN
F 2 "" H 4750 3450 50  0001 C CNN
F 3 "" H 4750 3450 50  0001 C CNN
	1    4750 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3050 4750 3050
$Comp
L Device:R R4
U 1 1 6184F921
P 4750 3250
F 0 "R4" H 4820 3296 50  0000 L CNN
F 1 "R" H 4820 3205 50  0000 L CNN
F 2 "" V 4680 3250 50  0001 C CNN
F 3 "~" H 4750 3250 50  0001 C CNN
	1    4750 3250
	1    0    0    -1  
$EndComp
NoConn ~ 4600 3200
NoConn ~ 4600 3350
NoConn ~ 4600 3500
NoConn ~ 4600 3650
NoConn ~ 4600 3800
Text GLabel 4600 2900 2    40   Input ~ 0
PGC
Text GLabel 4600 2750 2    40   Input ~ 0
PGD
$Comp
L Device:L_Small L1
U 1 1 6186AB9D
P 1550 3150
F 0 "L1" V 1735 3150 50  0000 C CNN
F 1 "100nH" V 1644 3150 50  0000 C CNN
F 2 "" H 1550 3150 50  0001 C CNN
F 3 "~" H 1550 3150 50  0001 C CNN
	1    1550 3150
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 6186CAD1
P 1300 3100
F 0 "#PWR01" H 1300 2950 50  0001 C CNN
F 1 "+5V" H 1315 3273 50  0000 C CNN
F 2 "" H 1300 3100 50  0001 C CNN
F 3 "" H 1300 3100 50  0001 C CNN
	1    1300 3100
	1    0    0    -1  
$EndComp
$Comp
L power:+5VA #PWR06
U 1 1 6186D0A3
P 1750 3100
F 0 "#PWR06" H 1750 2950 50  0001 C CNN
F 1 "+5VA" H 1765 3273 50  0000 C CNN
F 2 "" H 1750 3100 50  0001 C CNN
F 3 "" H 1750 3100 50  0001 C CNN
	1    1750 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 3100 1300 3150
Wire Wire Line
	1300 3150 1450 3150
Wire Wire Line
	1750 3150 1750 3100
Wire Wire Line
	1650 3150 1750 3150
Connection ~ 1300 3150
Wire Wire Line
	1300 3150 1300 3200
Wire Wire Line
	1750 3200 1750 3150
Connection ~ 1750 3150
Wire Wire Line
	2150 3400 2150 3350
Wire Wire Line
	1750 3150 2150 3150
Wire Wire Line
	2150 3150 2150 3200
Wire Wire Line
	1750 3400 2150 3400
Wire Wire Line
	1550 3500 1550 3450
$Comp
L power:GND #PWR04
U 1 1 6186D3F7
P 1550 3500
F 0 "#PWR04" H 1550 3250 50  0001 C CNN
F 1 "GND" H 1555 3327 50  0000 C CNN
F 2 "" H 1550 3500 50  0001 C CNN
F 3 "" H 1550 3500 50  0001 C CNN
	1    1550 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C1
U 1 1 6186C438
P 1300 3300
F 0 "C1" H 1392 3346 50  0000 L CNN
F 1 "1uF" H 1392 3255 50  0000 L CNN
F 2 "" H 1300 3300 50  0001 C CNN
F 3 "~" H 1300 3300 50  0001 C CNN
	1    1300 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C6
U 1 1 6186BFE0
P 1750 3300
F 0 "C6" H 1842 3346 50  0000 L CNN
F 1 "10nF" H 1842 3255 50  0000 L CNN
F 2 "" H 1750 3300 50  0001 C CNN
F 3 "~" H 1750 3300 50  0001 C CNN
	1    1750 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 3400 1300 3450
Wire Wire Line
	1300 3450 1550 3450
Wire Wire Line
	1550 3450 1750 3450
Wire Wire Line
	1750 3450 1750 3400
Connection ~ 1550 3450
Connection ~ 1750 3400
NoConn ~ 2150 3500
NoConn ~ 2150 3650
Text GLabel 2150 3800 0    50   Input ~ 0
SW1
Text GLabel 2150 3950 0    50   Input ~ 0
SW2
$Comp
L power:+5V #PWR022
U 1 1 6187E0DD
P 5400 6600
F 0 "#PWR022" H 5400 6450 50  0001 C CNN
F 1 "+5V" H 5415 6773 50  0000 C CNN
F 2 "" H 5400 6600 50  0001 C CNN
F 3 "" H 5400 6600 50  0001 C CNN
	1    5400 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 6700 5400 6600
Wire Wire Line
	5300 6700 5400 6700
Wire Wire Line
	4800 7100 4800 7050
Wire Wire Line
	4800 6700 4900 6700
Wire Wire Line
	4800 6750 4800 6700
Connection ~ 4800 6700
Wire Wire Line
	4600 6700 4800 6700
$Comp
L Switch:SW_Push SW2
U 1 1 6187D56B
P 5100 6700
F 0 "SW2" H 5100 6985 50  0000 C CNN
F 1 "SW_Push" H 5100 6894 50  0000 C CNN
F 2 "" H 5100 6900 50  0001 C CNN
F 3 "~" H 5100 6900 50  0001 C CNN
	1    5100 6700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 6187CD54
P 4800 7100
F 0 "#PWR015" H 4800 6850 50  0001 C CNN
F 1 "GND" H 4805 6927 50  0000 C CNN
F 2 "" H 4800 7100 50  0001 C CNN
F 3 "" H 4800 7100 50  0001 C CNN
	1    4800 7100
	1    0    0    -1  
$EndComp
Text GLabel 4600 6700 0    50   Input ~ 0
SW2
$Comp
L Device:R R5
U 1 1 618502FD
P 4800 6900
F 0 "R5" H 4870 6946 50  0000 L CNN
F 1 "10k" H 4870 6855 50  0000 L CNN
F 2 "" V 4730 6900 50  0001 C CNN
F 3 "~" H 4800 6900 50  0001 C CNN
	1    4800 6900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR012
U 1 1 618892F2
P 4250 6600
F 0 "#PWR012" H 4250 6450 50  0001 C CNN
F 1 "+5V" H 4265 6773 50  0000 C CNN
F 2 "" H 4250 6600 50  0001 C CNN
F 3 "" H 4250 6600 50  0001 C CNN
	1    4250 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 6700 4250 6600
Wire Wire Line
	4150 6700 4250 6700
Wire Wire Line
	3650 7100 3650 7050
Wire Wire Line
	3650 6700 3750 6700
Wire Wire Line
	3650 6750 3650 6700
Connection ~ 3650 6700
Wire Wire Line
	3450 6700 3650 6700
$Comp
L Switch:SW_Push SW1
U 1 1 61889585
P 3950 6700
F 0 "SW1" H 3950 6985 50  0000 C CNN
F 1 "SW_Push" H 3950 6894 50  0000 C CNN
F 2 "" H 3950 6900 50  0001 C CNN
F 3 "~" H 3950 6900 50  0001 C CNN
	1    3950 6700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 6188958F
P 3650 7100
F 0 "#PWR011" H 3650 6850 50  0001 C CNN
F 1 "GND" H 3655 6927 50  0000 C CNN
F 2 "" H 3650 7100 50  0001 C CNN
F 3 "" H 3650 7100 50  0001 C CNN
	1    3650 7100
	1    0    0    -1  
$EndComp
Text GLabel 3450 6700 0    50   Input ~ 0
SW1
$Comp
L Device:R R3
U 1 1 6188959A
P 3650 6900
F 0 "R3" H 3720 6946 50  0000 L CNN
F 1 "10k" H 3720 6855 50  0000 L CNN
F 2 "" V 3580 6900 50  0001 C CNN
F 3 "~" H 3650 6900 50  0001 C CNN
	1    3650 6900
	1    0    0    -1  
$EndComp
Text Notes 3200 6300 0    40   ~ 0
Push buttons whose state is read in by RE0 and RE1\nand then sent over SPI.
NoConn ~ 2150 4100
$Comp
L power:+5V #PWR02
U 1 1 61893890
P 1300 4100
F 0 "#PWR02" H 1300 3950 50  0001 C CNN
F 1 "+5V" H 1315 4273 50  0000 C CNN
F 2 "" H 1300 4100 50  0001 C CNN
F 3 "" H 1300 4100 50  0001 C CNN
	1    1300 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 618938AF
P 1300 4550
F 0 "#PWR03" H 1300 4300 50  0001 C CNN
F 1 "GND" H 1305 4377 50  0000 C CNN
F 2 "" H 1300 4550 50  0001 C CNN
F 3 "" H 1300 4550 50  0001 C CNN
	1    1300 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 618938B9
P 1300 4350
F 0 "C2" H 1100 4400 50  0000 L CNN
F 1 "1uF" H 1100 4250 50  0000 L CNN
F 2 "" H 1300 4350 50  0001 C CNN
F 3 "~" H 1300 4350 50  0001 C CNN
	1    1300 4350
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C4
U 1 1 618938C3
P 1500 4350
F 0 "C4" H 1600 4450 50  0000 L CNN
F 1 "10nF" H 1600 4350 50  0000 L CNN
F 2 "" H 1500 4350 50  0001 C CNN
F 3 "~" H 1500 4350 50  0001 C CNN
	1    1500 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 4150 2000 4250
Wire Wire Line
	2000 4250 2150 4250
Wire Wire Line
	4750 3850 4750 3950
Wire Wire Line
	4750 3950 4600 3950
Wire Wire Line
	4600 4100 4750 4100
Wire Wire Line
	4750 4100 4750 4200
Wire Wire Line
	1300 4550 1300 4500
Wire Wire Line
	1300 4250 1300 4150
Wire Wire Line
	1300 4150 1500 4150
Connection ~ 1300 4150
Wire Wire Line
	1300 4150 1300 4100
Wire Wire Line
	1300 4500 1500 4500
Connection ~ 1300 4500
Wire Wire Line
	1300 4500 1300 4450
Wire Wire Line
	1500 4500 1500 4450
Wire Wire Line
	1500 4250 1500 4150
Connection ~ 1500 4150
Wire Wire Line
	1500 4150 2000 4150
$Comp
L power:+5V #PWR017
U 1 1 618C2577
P 5250 3800
F 0 "#PWR017" H 5250 3650 50  0001 C CNN
F 1 "+5V" H 5265 3973 50  0000 C CNN
F 2 "" H 5250 3800 50  0001 C CNN
F 3 "" H 5250 3800 50  0001 C CNN
	1    5250 3800
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 618C257D
P 5250 4250
F 0 "#PWR018" H 5250 4000 50  0001 C CNN
F 1 "GND" H 5255 4077 50  0000 C CNN
F 2 "" H 5250 4250 50  0001 C CNN
F 3 "" H 5250 4250 50  0001 C CNN
	1    5250 4250
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C10
U 1 1 618C2583
P 5250 4050
F 0 "C10" H 5050 4100 50  0000 L CNN
F 1 "1uF" H 5050 3950 50  0000 L CNN
F 2 "" H 5250 4050 50  0001 C CNN
F 3 "~" H 5250 4050 50  0001 C CNN
	1    5250 4050
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C9
U 1 1 618C2589
P 5050 4050
F 0 "C9" H 5142 4096 50  0000 L CNN
F 1 "10nF" H 5142 4005 50  0000 L CNN
F 2 "" H 5050 4050 50  0001 C CNN
F 3 "~" H 5050 4050 50  0001 C CNN
	1    5050 4050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5250 4250 5250 4200
Wire Wire Line
	5250 3950 5250 3850
Wire Wire Line
	5250 3850 5050 3850
Connection ~ 5250 3850
Wire Wire Line
	5250 3850 5250 3800
Wire Wire Line
	5250 4200 5050 4200
Connection ~ 5250 4200
Wire Wire Line
	5250 4200 5250 4150
Wire Wire Line
	5050 4200 5050 4150
Connection ~ 5050 4200
Wire Wire Line
	5050 4200 4750 4200
Wire Wire Line
	5050 3950 5050 3850
Connection ~ 5050 3850
Wire Wire Line
	5050 3850 4750 3850
$Comp
L Device:Crystal_Small Y1
U 1 1 618C5189
P 1550 5000
F 0 "Y1" H 1550 5225 50  0000 C CNN
F 1 "10MHz" H 1550 5134 50  0000 C CNN
F 2 "" H 1550 5000 50  0001 C CNN
F 3 "~" H 1550 5000 50  0001 C CNN
	1    1550 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C3
U 1 1 618C60AE
P 1450 5200
F 0 "C3" H 1250 5250 50  0000 L CNN
F 1 "20pF" H 1200 5100 50  0000 L CNN
F 2 "" H 1450 5200 50  0001 C CNN
F 3 "~" H 1450 5200 50  0001 C CNN
	1    1450 5200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C5
U 1 1 618C633B
P 1650 5200
F 0 "C5" H 1750 5250 50  0000 L CNN
F 1 "20pF" H 1700 5100 50  0000 L CNN
F 2 "" H 1650 5200 50  0001 C CNN
F 3 "~" H 1650 5200 50  0001 C CNN
	1    1650 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 618C693A
P 1550 5350
F 0 "#PWR05" H 1550 5100 50  0001 C CNN
F 1 "GND" H 1555 5177 50  0000 C CNN
F 2 "" H 1550 5350 50  0001 C CNN
F 3 "" H 1550 5350 50  0001 C CNN
	1    1550 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 5350 1450 5350
Wire Wire Line
	1450 5350 1450 5300
Wire Wire Line
	1450 5100 1450 5000
Wire Wire Line
	1650 5000 1650 5100
Wire Wire Line
	1650 5350 1550 5350
Wire Wire Line
	1650 5300 1650 5350
Connection ~ 1550 5350
Text GLabel 1400 5000 0    40   Input ~ 0
OSC1
Text GLabel 1700 5000 2    40   Input ~ 0
OSC2
Wire Wire Line
	1400 5000 1450 5000
Connection ~ 1450 5000
Wire Wire Line
	1650 5000 1700 5000
Connection ~ 1650 5000
Wire Wire Line
	1750 4400 1750 4500
Wire Wire Line
	1750 4500 1500 4500
Wire Wire Line
	1750 4400 2150 4400
Connection ~ 1500 4500
Text GLabel 2150 4550 0    40   Input ~ 0
OSC1
NoConn ~ 2150 4850
NoConn ~ 2150 5000
NoConn ~ 2150 5150
NoConn ~ 2150 5450
NoConn ~ 2150 5600
Text GLabel 2150 5300 0    50   Input ~ 0
SCK
Text GLabel 4600 5300 2    50   Input ~ 0
MISO
Text GLabel 4600 5150 2    50   Input ~ 0
MOSI
NoConn ~ 4600 5000
NoConn ~ 4600 4850
NoConn ~ 4600 4700
NoConn ~ 4600 4550
NoConn ~ 4600 4400
NoConn ~ 4600 4250
$Comp
L Device:LED D2
U 1 1 6191A014
P 5400 1800
F 0 "D2" V 5439 1682 50  0000 R CNN
F 1 "LED" V 5348 1682 50  0000 R CNN
F 2 "" H 5400 1800 50  0001 C CNN
F 3 "~" H 5400 1800 50  0001 C CNN
	1    5400 1800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R7
U 1 1 6191A3E2
P 5400 1450
F 0 "R7" H 5330 1404 50  0000 R CNN
F 1 "4.7k" H 5330 1495 50  0000 R CNN
F 2 "" V 5330 1450 50  0001 C CNN
F 3 "~" H 5400 1450 50  0001 C CNN
	1    5400 1450
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR020
U 1 1 6191A3EC
P 5400 2000
F 0 "#PWR020" H 5400 1750 50  0001 C CNN
F 1 "GND" H 5405 1827 50  0000 C CNN
F 2 "" H 5400 2000 50  0001 C CNN
F 3 "" H 5400 2000 50  0001 C CNN
	1    5400 2000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR019
U 1 1 6191FF81
P 5400 1200
F 0 "#PWR019" H 5400 1050 50  0001 C CNN
F 1 "+5V" H 5415 1373 50  0000 C CNN
F 2 "" H 5400 1200 50  0001 C CNN
F 3 "" H 5400 1200 50  0001 C CNN
	1    5400 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 1950 5400 2000
Wire Wire Line
	5400 1600 5400 1650
Text Notes 1850 1150 2    50   ~ 0
ICSP Header
Text Notes 1150 1800 1    30   ~ 0
Use with something like\na PICkit4.
Wire Notes Line
	5950 550  5900 6600
$Comp
L can_node_board_hw:PIC18F4620-IP U3
U 1 1 6194E699
P 8550 3100
F 0 "U3" H 8525 4965 50  0000 C CNN
F 1 "PIC18F4620-IP" H 8525 4874 50  0000 C CNN
F 2 "" H 8400 3650 50  0001 C CNN
F 3 "" H 8400 3650 50  0001 C CNN
	1    8550 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 6194EA93
P 7150 1300
F 0 "R10" H 7220 1346 50  0000 L CNN
F 1 "10K" H 7220 1255 50  0000 L CNN
F 2 "" V 7080 1300 50  0001 C CNN
F 3 "~" H 7150 1300 50  0001 C CNN
	1    7150 1300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 6194EA9D
P 6900 1600
F 0 "R9" V 6693 1600 50  0000 C CNN
F 1 "51" V 6784 1600 50  0000 C CNN
F 2 "" V 6830 1600 50  0001 C CNN
F 3 "~" H 6900 1600 50  0001 C CNN
	1    6900 1600
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x05_Female J2
U 1 1 6194EAA7
P 10700 5750
F 0 "J2" H 10800 5750 50  0000 C CNN
F 1 "Conn_01x05_Female" H 10600 6150 50  0001 C CNN
F 2 "" H 10700 5750 50  0001 C CNN
F 3 "~" H 10700 5750 50  0001 C CNN
	1    10700 5750
	1    0    0    -1  
$EndComp
Text GLabel 10400 5550 0    40   Input ~ 0
~MCLR
Text GLabel 10400 5850 0    40   Input ~ 0
PGD
Text GLabel 10400 5950 0    40   Input ~ 0
PGC
$Comp
L power:+5V #PWR034
U 1 1 6194EAB4
P 10050 5600
F 0 "#PWR034" H 10050 5450 50  0001 C CNN
F 1 "+5V" H 10065 5773 50  0000 C CNN
F 2 "" H 10050 5600 50  0001 C CNN
F 3 "" H 10050 5600 50  0001 C CNN
	1    10050 5600
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR035
U 1 1 6194EABE
P 10050 5800
F 0 "#PWR035" H 10050 5550 50  0001 C CNN
F 1 "GND" H 10055 5627 50  0000 C CNN
F 2 "" H 10050 5800 50  0001 C CNN
F 3 "" H 10050 5800 50  0001 C CNN
	1    10050 5800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10050 5650 10500 5650
Wire Wire Line
	10500 5750 10050 5750
Wire Wire Line
	10500 5550 10400 5550
Wire Wire Line
	10500 5850 10400 5850
Wire Wire Line
	10500 5950 10400 5950
Wire Wire Line
	10050 5650 10050 5600
Wire Wire Line
	10050 5750 10050 5800
Text GLabel 6600 1600 0    40   Input ~ 0
~MCLR
Wire Wire Line
	6600 1600 6750 1600
Wire Wire Line
	7050 1600 7150 1600
$Comp
L power:+5V #PWR027
U 1 1 6194EAD2
P 7150 1050
F 0 "#PWR027" H 7150 900 50  0001 C CNN
F 1 "+5V" H 7165 1223 50  0000 C CNN
F 2 "" H 7150 1050 50  0001 C CNN
F 3 "" H 7150 1050 50  0001 C CNN
	1    7150 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 1600 7150 1450
Connection ~ 7150 1600
Wire Wire Line
	7150 1600 7300 1600
Wire Wire Line
	7150 1150 7150 1050
Text Notes 6300 1350 0    25   ~ 0
51 ohm resistor in series /w MCLR\nimportant! Seen videos where PICKit 4\nbricks uCs because of voltage spikes\non ~MCLR.
NoConn ~ 7300 1750
NoConn ~ 7300 1900
Wire Wire Line
	9900 1900 9900 1950
Wire Wire Line
	9900 2300 9900 2250
$Comp
L power:GND #PWR033
U 1 1 6194EB3E
P 9900 2300
F 0 "#PWR033" H 9900 2050 50  0001 C CNN
F 1 "GND" H 9905 2127 50  0000 C CNN
F 2 "" H 9900 2300 50  0001 C CNN
F 3 "" H 9900 2300 50  0001 C CNN
	1    9900 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 1900 9900 1900
$Comp
L Device:R R11
U 1 1 6194EB49
P 9900 2100
F 0 "R11" H 9970 2146 50  0000 L CNN
F 1 "R" H 9970 2055 50  0000 L CNN
F 2 "" V 9830 2100 50  0001 C CNN
F 3 "~" H 9900 2100 50  0001 C CNN
	1    9900 2100
	1    0    0    -1  
$EndComp
NoConn ~ 9750 2050
NoConn ~ 9750 2200
NoConn ~ 9750 2350
NoConn ~ 9750 2500
NoConn ~ 9750 2650
Text GLabel 9750 1750 2    40   Input ~ 0
PGC
Text GLabel 9750 1600 2    40   Input ~ 0
PGD
NoConn ~ 7300 2350
NoConn ~ 7300 2950
$Comp
L power:+5V #PWR024
U 1 1 6194EC13
P 6450 2950
F 0 "#PWR024" H 6450 2800 50  0001 C CNN
F 1 "+5V" H 6465 3123 50  0000 C CNN
F 2 "" H 6450 2950 50  0001 C CNN
F 3 "" H 6450 2950 50  0001 C CNN
	1    6450 2950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR025
U 1 1 6194EC1D
P 6450 3400
F 0 "#PWR025" H 6450 3150 50  0001 C CNN
F 1 "GND" H 6455 3227 50  0000 C CNN
F 2 "" H 6450 3400 50  0001 C CNN
F 3 "" H 6450 3400 50  0001 C CNN
	1    6450 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C12
U 1 1 6194EC27
P 6450 3200
F 0 "C12" H 6250 3250 50  0000 L CNN
F 1 "1uF" H 6250 3100 50  0000 L CNN
F 2 "" H 6450 3200 50  0001 C CNN
F 3 "~" H 6450 3200 50  0001 C CNN
	1    6450 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C14
U 1 1 6194EC31
P 6650 3200
F 0 "C14" H 6750 3300 50  0000 L CNN
F 1 "10nF" H 6750 3200 50  0000 L CNN
F 2 "" H 6650 3200 50  0001 C CNN
F 3 "~" H 6650 3200 50  0001 C CNN
	1    6650 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 3000 7150 3100
Wire Wire Line
	7150 3100 7300 3100
Wire Wire Line
	9900 2700 9900 2800
Wire Wire Line
	9900 2800 9750 2800
Wire Wire Line
	9750 2950 9900 2950
Wire Wire Line
	9900 2950 9900 3050
Wire Wire Line
	6450 3400 6450 3350
Wire Wire Line
	6450 3100 6450 3000
Wire Wire Line
	6450 3000 6650 3000
Connection ~ 6450 3000
Wire Wire Line
	6450 3000 6450 2950
Wire Wire Line
	6450 3350 6650 3350
Connection ~ 6450 3350
Wire Wire Line
	6450 3350 6450 3300
Wire Wire Line
	6650 3350 6650 3300
Wire Wire Line
	6650 3100 6650 3000
Connection ~ 6650 3000
Wire Wire Line
	6650 3000 7150 3000
$Comp
L power:+5V #PWR036
U 1 1 6194EC4D
P 10400 2650
F 0 "#PWR036" H 10400 2500 50  0001 C CNN
F 1 "+5V" H 10415 2823 50  0000 C CNN
F 2 "" H 10400 2650 50  0001 C CNN
F 3 "" H 10400 2650 50  0001 C CNN
	1    10400 2650
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR037
U 1 1 6194EC57
P 10400 3100
F 0 "#PWR037" H 10400 2850 50  0001 C CNN
F 1 "GND" H 10405 2927 50  0000 C CNN
F 2 "" H 10400 3100 50  0001 C CNN
F 3 "" H 10400 3100 50  0001 C CNN
	1    10400 3100
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C16
U 1 1 6194EC61
P 10400 2900
F 0 "C16" H 10200 2950 50  0000 L CNN
F 1 "1uF" H 10200 2800 50  0000 L CNN
F 2 "" H 10400 2900 50  0001 C CNN
F 3 "~" H 10400 2900 50  0001 C CNN
	1    10400 2900
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C15
U 1 1 6194EC6B
P 10200 2900
F 0 "C15" H 10292 2946 50  0000 L CNN
F 1 "10nF" H 10292 2855 50  0000 L CNN
F 2 "" H 10200 2900 50  0001 C CNN
F 3 "~" H 10200 2900 50  0001 C CNN
	1    10200 2900
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10400 3100 10400 3050
Wire Wire Line
	10400 2800 10400 2700
Wire Wire Line
	10400 2700 10200 2700
Connection ~ 10400 2700
Wire Wire Line
	10400 2700 10400 2650
Wire Wire Line
	10400 3050 10200 3050
Connection ~ 10400 3050
Wire Wire Line
	10400 3050 10400 3000
Wire Wire Line
	10200 3050 10200 3000
Connection ~ 10200 3050
Wire Wire Line
	10200 3050 9900 3050
Wire Wire Line
	10200 2800 10200 2700
Connection ~ 10200 2700
Wire Wire Line
	10200 2700 9900 2700
$Comp
L Device:Crystal_Small Y2
U 1 1 6194EC83
P 6500 4000
F 0 "Y2" H 6500 4225 50  0000 C CNN
F 1 "10MHz" H 6500 4134 50  0000 C CNN
F 2 "" H 6500 4000 50  0001 C CNN
F 3 "~" H 6500 4000 50  0001 C CNN
	1    6500 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C11
U 1 1 6194EC8D
P 6400 4200
F 0 "C11" H 6200 4250 50  0000 L CNN
F 1 "20pF" H 6150 4100 50  0000 L CNN
F 2 "" H 6400 4200 50  0001 C CNN
F 3 "~" H 6400 4200 50  0001 C CNN
	1    6400 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C13
U 1 1 6194EC97
P 6600 4200
F 0 "C13" H 6700 4250 50  0000 L CNN
F 1 "20pF" H 6650 4100 50  0000 L CNN
F 2 "" H 6600 4200 50  0001 C CNN
F 3 "~" H 6600 4200 50  0001 C CNN
	1    6600 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR026
U 1 1 6194ECA1
P 6500 4350
F 0 "#PWR026" H 6500 4100 50  0001 C CNN
F 1 "GND" H 6505 4177 50  0000 C CNN
F 2 "" H 6500 4350 50  0001 C CNN
F 3 "" H 6500 4350 50  0001 C CNN
	1    6500 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4350 6400 4350
Wire Wire Line
	6400 4350 6400 4300
Wire Wire Line
	6400 4100 6400 4000
Wire Wire Line
	6600 4000 6600 4100
Wire Wire Line
	6600 4350 6500 4350
Wire Wire Line
	6600 4300 6600 4350
Connection ~ 6500 4350
Text GLabel 6350 4000 0    40   Input ~ 0
OSC1
Text GLabel 6650 4000 2    40   Input ~ 0
OSC2
Wire Wire Line
	6350 4000 6400 4000
Connection ~ 6400 4000
Wire Wire Line
	6600 4000 6650 4000
Connection ~ 6600 4000
Wire Wire Line
	6900 3250 6900 3350
Wire Wire Line
	6900 3350 6650 3350
Wire Wire Line
	6900 3250 7300 3250
Connection ~ 6650 3350
NoConn ~ 7300 4300
Text GLabel 7300 4150 0    50   Input ~ 0
SCK
$Comp
L power:GND #PWR023
U 1 1 6194ECDA
P 6350 5600
F 0 "#PWR023" H 6350 5350 50  0001 C CNN
F 1 "GND" H 6355 5427 50  0000 C CNN
F 2 "" H 6350 5600 50  0001 C CNN
F 3 "" H 6350 5600 50  0001 C CNN
	1    6350 5600
	1    0    0    -1  
$EndComp
NoConn ~ 9750 4450
Text GLabel 9750 4000 2    50   Input ~ 0
MISO
Text GLabel 9750 4150 2    50   Input ~ 0
MOSI
NoConn ~ 9750 3850
NoConn ~ 9750 3700
Text Notes 10200 5350 0    50   ~ 0
ICSP Header
Text Notes 850  7150 0    150  ~ 30
TX NODE
Text Notes 9550 900  0    150  ~ 30
RX NODE
Text GLabel 7300 2500 0    50   Input ~ 0
~SS
Text GLabel 7300 3400 0    50   Input ~ 0
OSC1
Text GLabel 7300 3550 0    50   Input ~ 0
OSC2
NoConn ~ 7300 2050
NoConn ~ 7300 2200
$Comp
L Device:LED D4
U 1 1 6194ECC6
P 6350 5400
F 0 "D4" V 6400 5550 50  0000 R CNN
F 1 "LED" V 6300 5600 50  0000 R CNN
F 2 "" H 6350 5400 50  0001 C CNN
F 3 "~" H 6350 5400 50  0001 C CNN
	1    6350 5400
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R8
U 1 1 6194ECD0
P 6500 5200
F 0 "R8" V 6600 5200 50  0000 C CNN
F 1 "4.7k" V 6400 5150 50  0000 C CNN
F 2 "" V 6430 5200 50  0001 C CNN
F 3 "~" H 6500 5200 50  0001 C CNN
	1    6500 5200
	0    -1   -1   0   
$EndComp
Text Notes 6500 5550 0    50   ~ 10
Debug LED
$Comp
L power:GND #PWR021
U 1 1 61A2CC88
P 5400 5900
F 0 "#PWR021" H 5400 5650 50  0001 C CNN
F 1 "GND" H 5405 5727 50  0000 C CNN
F 2 "" H 5400 5900 50  0001 C CNN
F 3 "" H 5400 5900 50  0001 C CNN
	1    5400 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D3
U 1 1 61A2D217
P 5400 5700
F 0 "D3" V 5439 5582 50  0000 R CNN
F 1 "LED" V 5348 5582 50  0000 R CNN
F 2 "" H 5400 5700 50  0001 C CNN
F 3 "~" H 5400 5700 50  0001 C CNN
	1    5400 5700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5400 5900 5400 5850
Wire Wire Line
	5400 5550 5400 5450
Text Notes 4850 5750 0    50   ~ 10
Debug LED
Wire Wire Line
	4600 5450 5000 5450
Wire Wire Line
	5300 5450 5400 5450
$Comp
L Device:R R6
U 1 1 61A2D221
P 5150 5450
F 0 "R6" V 4943 5450 50  0000 C CNN
F 1 "4.7k" V 5034 5450 50  0000 C CNN
F 2 "" V 5080 5450 50  0001 C CNN
F 3 "~" H 5150 5450 50  0001 C CNN
	1    5150 5450
	0    1    1    0   
$EndComp
Text Notes 10300 4100 0    25   ~ 5
Careful to remember SDO of master goes\nto SDI of slave, and vice versa!
$Comp
L Display_Character:HY1602E DS1
U 1 1 61A72C23
P 8550 5500
F 0 "DS1" V 8150 6350 50  0000 C CNN
F 1 "HY1602E" V 8950 4900 50  0000 C CNN
F 2 "Display:HY1602E" H 8550 4600 50  0001 C CIN
F 3 "http://www.icbank.com/data/ICBShop/board/HY1602E.pdf" H 8750 5600 50  0001 C CNN
	1    8550 5500
	0    -1   1    0   
$EndComp
$Comp
L power:GND #PWR028
U 1 1 61A79470
P 7400 6000
F 0 "#PWR028" H 7400 5750 50  0001 C CNN
F 1 "GND" H 7405 5827 50  0000 C CNN
F 2 "" H 7400 6000 50  0001 C CNN
F 3 "" H 7400 6000 50  0001 C CNN
	1    7400 6000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR029
U 1 1 61A7988A
P 7700 5450
F 0 "#PWR029" H 7700 5300 50  0001 C CNN
F 1 "+5V" H 7715 5623 50  0000 C CNN
F 2 "" H 7700 5450 50  0001 C CNN
F 3 "" H 7700 5450 50  0001 C CNN
	1    7700 5450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR032
U 1 1 61A79D01
P 9400 5550
F 0 "#PWR032" H 9400 5300 50  0001 C CNN
F 1 "GND" H 9405 5377 50  0000 C CNN
F 2 "" H 9400 5550 50  0001 C CNN
F 3 "" H 9400 5550 50  0001 C CNN
	1    9400 5550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9400 5550 9400 5500
Wire Wire Line
	9400 5500 9350 5500
Wire Wire Line
	7750 5500 7700 5500
Wire Wire Line
	7700 5500 7700 5450
$Comp
L Device:R_POT RV1
U 1 1 61A98DFF
P 7650 5950
F 0 "RV1" H 7581 5996 50  0000 R CNN
F 1 "10K" H 7581 5905 50  0000 R CNN
F 2 "" H 7650 5950 50  0001 C CNN
F 3 "~" H 7650 5950 50  0001 C CNN
	1    7650 5950
	0    1    1    0   
$EndComp
Wire Wire Line
	7650 6100 7800 6100
Wire Wire Line
	7800 6100 7800 5950
Connection ~ 7800 5950
$Comp
L power:+5V #PWR030
U 1 1 61AE105E
P 8400 6000
F 0 "#PWR030" H 8400 5850 50  0001 C CNN
F 1 "+5V" H 8350 6150 50  0000 C CNN
F 2 "" H 8400 6000 50  0001 C CNN
F 3 "" H 8400 6000 50  0001 C CNN
	1    8400 6000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR031
U 1 1 61AE144C
P 8650 5950
F 0 "#PWR031" H 8650 5700 50  0001 C CNN
F 1 "GND" H 8500 5850 50  0000 C CNN
F 2 "" H 8650 5950 50  0001 C CNN
F 3 "" H 8650 5950 50  0001 C CNN
	1    8650 5950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8650 5950 8650 5900
Wire Wire Line
	8400 6000 8400 6050
Wire Wire Line
	8400 6050 8550 6050
Wire Wire Line
	8550 6050 8550 5900
Wire Wire Line
	7300 4450 7250 4450
Wire Wire Line
	6350 5550 6350 5600
Wire Wire Line
	6350 5200 6350 5250
NoConn ~ 8450 5100
NoConn ~ 8550 5100
NoConn ~ 8650 5100
NoConn ~ 8750 5100
Wire Wire Line
	7400 6000 7400 5950
Wire Wire Line
	7400 5950 7500 5950
Wire Wire Line
	7950 5950 7950 5900
Wire Wire Line
	7800 5950 7950 5950
Entry Wire Line
	9850 3100 9950 3200
Entry Wire Line
	9850 3250 9950 3350
Entry Wire Line
	9850 3400 9950 3500
Entry Wire Line
	9850 3550 9950 3650
Entry Wire Line
	9250 4900 9150 5000
Entry Wire Line
	9150 4900 9050 5000
Entry Wire Line
	9050 4900 8950 5000
Entry Wire Line
	8950 4900 8850 5000
Wire Bus Line
	10200 4900 10200 3850
Wire Bus Line
	10200 3850 9950 3850
Wire Wire Line
	9150 5100 9150 5000
Wire Wire Line
	9050 5100 9050 5000
Wire Wire Line
	8950 5100 8950 5000
Wire Wire Line
	8850 5100 8850 5000
Wire Wire Line
	9750 3550 9850 3550
Wire Wire Line
	9850 3400 9750 3400
Wire Wire Line
	9850 3250 9750 3250
Wire Wire Line
	9850 3100 9750 3100
Text Label 9750 3100 0    30   ~ 6
DB7
Text Label 9750 3250 0    30   ~ 6
DB6
Text Label 9750 3400 0    30   ~ 6
DB5
Text Label 9750 3550 0    30   ~ 6
DB4
Text Label 9150 5100 1    30   ~ 6
DB7
Text Label 9050 5100 1    30   ~ 6
DB6
Text Label 8950 5100 1    30   ~ 6
DB5
Text Label 8850 5100 1    30   ~ 6
DB4
Entry Wire Line
	7050 3850 6950 3950
Entry Wire Line
	7050 4000 6950 4100
Entry Wire Line
	7050 3700 6950 3800
Entry Wire Line
	7850 4900 7950 5000
Entry Wire Line
	8050 4900 8150 5000
Entry Wire Line
	8150 4900 8250 5000
Wire Wire Line
	7050 3700 7300 3700
Wire Wire Line
	7050 3850 7300 3850
Wire Wire Line
	7050 4000 7300 4000
Wire Wire Line
	7950 5100 7950 5000
Wire Wire Line
	8150 5100 8150 5000
Wire Wire Line
	8250 5000 8250 5100
Text Label 7200 3700 0    30   ~ 6
RS
Text Label 7200 3850 0    30   ~ 6
R~W
Text Label 7200 4000 0    30   ~ 6
E
Text Label 7950 5100 1    30   ~ 6
E
Text Label 8150 5100 1    30   ~ 6
R~W
Text Label 8250 5100 1    30   ~ 6
RS
Text GLabel 7250 4450 3    40   Input ~ 0
DEBUG_LED
Text GLabel 6700 5200 2    40   Input ~ 0
DEBUG_LED
Wire Wire Line
	6700 5200 6650 5200
Wire Notes Line
	4550 2300 4550 2000
Wire Notes Line
	4550 2000 2850 2000
Wire Notes Line
	2850 2000 2850 700 
Text Notes 2900 1950 0    100  ~ 20
5V POWER
Text Notes 9600 1150 0    70   ~ 14
Get button states over SPI.\nDisplay state.
Text Notes 900  7450 0    70   ~ 14
Read button states.\nSend state over SPI.
Text Notes 8050 6350 0    50   ~ 10
HD44780 Driven LCD Display\n4-bit Mode Used
Text Notes 8350 5050 0    15   Italic 0
*Might be good idea to pulldown/\npulldown unused data lines...
Wire Wire Line
	5400 1200 5400 1300
Wire Wire Line
	3000 1400 3100 1400
Wire Wire Line
	3000 1200 3000 1400
Wire Wire Line
	3500 1500 3650 1500
Wire Wire Line
	3650 1500 3650 1250
Wire Wire Line
	3650 1250 3750 1250
$Comp
L power:+5V #PWR0101
U 1 1 61D9D7C9
P 5000 1200
F 0 "#PWR0101" H 5000 1050 50  0001 C CNN
F 1 "+5V" H 5015 1373 50  0000 C CNN
F 2 "" H 5000 1200 50  0001 C CNN
F 3 "" H 5000 1200 50  0001 C CNN
	1    5000 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 1250 5000 1200
Connection ~ 5000 1250
Wire Notes Line
	5750 2300 5750 700 
Wire Notes Line
	4550 2300 5750 2300
Wire Notes Line
	2850 700  5750 700 
NoConn ~ 7300 2650
NoConn ~ 7300 2800
NoConn ~ 9750 4300
NoConn ~ 3500 1300
Text GLabel 4600 5600 2    50   Input ~ 0
~SS
Text GLabel 2150 4700 0    40   Input ~ 0
OSC2
$Comp
L Switch:SW_SPDT SW3
U 1 1 61D41444
P 3300 1400
F 0 "SW3" H 3250 1550 50  0000 C CNN
F 1 "SW_SPDT" H 3150 1250 50  0000 C CNN
F 2 "" H 3300 1400 50  0001 C CNN
F 3 "~" H 3300 1400 50  0001 C CNN
	1    3300 1400
	1    0    0    -1  
$EndComp
$Comp
L power:VPP #PWR010
U 1 1 6185940E
P 3000 1200
F 0 "#PWR010" H 3000 1050 50  0001 C CNN
F 1 "VPP" H 3015 1373 50  0000 C CNN
F 2 "" H 3000 1200 50  0001 C CNN
F 3 "" H 3000 1200 50  0001 C CNN
	1    3000 1200
	1    0    0    -1  
$EndComp
$Sheet
S 5550 7450 500  150 
U 619FAD4D
F0 "PIC18F4620 TX Node Setup" 50
F1 "pic18f4620_tx_node.sch" 50
$EndSheet
$Sheet
S 5550 7100 500  150 
U 61A0C441
F0 "PIC18F4620 RX Node Setup" 50
F1 "pic18f4620_rx_node.sch" 50
$EndSheet
$Sheet
S 5550 6750 500  150 
U 61A1D4A2
F0 "Power 5V 7805" 50
F1 "pwr_5V_7805.sch" 50
$EndSheet
Wire Bus Line
	6950 3800 6950 4900
Wire Bus Line
	6950 4900 8150 4900
Wire Bus Line
	8950 4900 10200 4900
Wire Bus Line
	9950 3200 9950 3850
Text Notes 11100 3850 1    40   ~ 0
Also note that in slave mode, once CS goes high (i.e., disabled),\nthen SDO goes floating. So it MAY be desired to have pull-up\nor pull-down resistors! This behavior seems common among\nSPI slave devices in general.
$EndSCHEMATC
