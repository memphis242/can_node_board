EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title "MCP2515 CAN TEST: Node 1"
Date "2021-11-06"
Rev "1.0"
Comp ""
Comment1 "and update the debug LED state through CAN messages."
Comment2 "This CAN Node will send the state of the two button switches"
Comment3 ""
Comment4 "Author: Abdullah Almosalami"
$EndDescr
$Comp
L can_node_board_hw:PIC18F4620-IP U104
U 1 1 6184BAE3
P 3400 4250
F 0 "U104" H 3375 6115 50  0000 C CNN
F 1 "PIC18F4620-IP" H 3375 6024 50  0000 C CNN
F 2 "" H 3250 4800 50  0001 C CNN
F 3 "" H 3250 4800 50  0001 C CNN
	1    3400 4250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R102
U 1 1 6184FCEC
P 2000 2450
F 0 "R102" H 2070 2496 50  0000 L CNN
F 1 "10K" H 2070 2405 50  0000 L CNN
F 2 "" V 1930 2450 50  0001 C CNN
F 3 "~" H 2000 2450 50  0001 C CNN
	1    2000 2450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R103
U 1 1 6184FE85
P 1750 2750
F 0 "R103" V 1543 2750 50  0000 C CNN
F 1 "51" V 1634 2750 50  0000 C CNN
F 2 "" V 1680 2750 50  0001 C CNN
F 3 "~" H 1750 2750 50  0001 C CNN
	1    1750 2750
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x05_Female J101
U 1 1 61851AFC
P 1350 1550
F 0 "J101" H 1450 1550 50  0000 C CNN
F 1 "Conn_01x05_Female" H 1250 1950 50  0001 C CNN
F 2 "" H 1350 1550 50  0001 C CNN
F 3 "~" H 1350 1550 50  0001 C CNN
	1    1350 1550
	-1   0    0    -1  
$EndComp
Text GLabel 1650 1350 2    40   Input ~ 0
PIC1_~MCLR
Text GLabel 1650 1650 2    40   Input ~ 0
PIC1_PGD
Text GLabel 1650 1750 2    40   Input ~ 0
PIC1_PGC
$Comp
L power:+5V #PWR0108
U 1 1 6185330E
P 2150 1400
F 0 "#PWR0108" H 2150 1250 50  0001 C CNN
F 1 "+5V" H 2165 1573 50  0000 C CNN
F 2 "" H 2150 1400 50  0001 C CNN
F 3 "" H 2150 1400 50  0001 C CNN
	1    2150 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 61854162
P 2150 1600
F 0 "#PWR0110" H 2150 1350 50  0001 C CNN
F 1 "GND" H 2155 1427 50  0000 C CNN
F 2 "" H 2150 1600 50  0001 C CNN
F 3 "" H 2150 1600 50  0001 C CNN
	1    2150 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 1450 1550 1450
Wire Wire Line
	1550 1550 2150 1550
Wire Wire Line
	1550 1350 1650 1350
Wire Wire Line
	1550 1650 1650 1650
Wire Wire Line
	1550 1750 1650 1750
Wire Wire Line
	2150 1450 2150 1400
Wire Wire Line
	2150 1550 2150 1600
Text GLabel 1450 2750 0    40   Input ~ 0
PIC1_~MCLR
Wire Wire Line
	1450 2750 1600 2750
Wire Wire Line
	1900 2750 2000 2750
$Comp
L power:+5V #PWR0115
U 1 1 61856454
P 2000 2200
F 0 "#PWR0115" H 2000 2050 50  0001 C CNN
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
L Regulator_Linear:L7805 U101
U 1 1 61857303
P 4550 1250
F 0 "U101" H 4550 1492 50  0000 C CNN
F 1 "L7805" H 4550 1401 50  0000 C CNN
F 2 "" H 4575 1100 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 4550 1200 50  0001 C CNN
	1    4550 1250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 61857A6A
P 4550 1700
F 0 "#PWR0112" H 4550 1450 50  0001 C CNN
F 1 "GND" H 4555 1527 50  0000 C CNN
F 2 "" H 4550 1700 50  0001 C CNN
F 3 "" H 4550 1700 50  0001 C CNN
	1    4550 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1700 4550 1650
$Comp
L Device:C_Small C103
U 1 1 6185829F
P 4100 1500
F 0 "C103" H 3800 1550 50  0000 L CNN
F 1 "0.1uF" H 3800 1450 50  0000 L CNN
F 2 "" H 4100 1500 50  0001 C CNN
F 3 "~" H 4100 1500 50  0001 C CNN
	1    4100 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C104
U 1 1 61858CED
P 5000 1500
F 0 "C104" H 5092 1546 50  0000 L CNN
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
L Device:D_Schottky D101
U 1 1 6185D69A
P 3900 1250
F 0 "D101" H 3900 1033 50  0000 C CNN
F 1 "D_Schottky" H 3900 1124 50  0000 C CNN
F 2 "" H 3900 1250 50  0001 C CNN
F 3 "~" H 3900 1250 50  0001 C CNN
	1    3900 1250
	-1   0    0    1   
$EndComp
Text Notes 3450 950  0    20   ~ 0
This is minimal input voltage protection (reverse polarity).\nWhen breadboarding, it's easy to make mistakes, so ideally,\nyou'd use more protection - clamp diodes to ground,\np-channel mosfet reverse-polarity protection, a fuse, etc.
$Comp
L power:GND #PWR0121
U 1 1 618654D5
P 5150 3100
F 0 "#PWR0121" H 5150 2850 50  0001 C CNN
F 1 "GND" H 5155 2927 50  0000 C CNN
F 2 "" H 5150 3100 50  0001 C CNN
F 3 "" H 5150 3100 50  0001 C CNN
	1    5150 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3050 4750 3050
$Comp
L Device:R R104
U 1 1 6184F921
P 4900 3050
F 0 "R104" V 5000 3000 50  0000 L CNN
F 1 "10k" V 4850 3200 50  0000 L CNN
F 2 "" V 4830 3050 50  0001 C CNN
F 3 "~" H 4900 3050 50  0001 C CNN
	1    4900 3050
	0    1    1    0   
$EndComp
NoConn ~ 4600 3200
NoConn ~ 4600 3350
Text GLabel 4600 2900 2    40   Input ~ 0
PIC1_PGC
Text GLabel 4600 2750 2    40   Input ~ 0
PIC1_PGD
$Comp
L Device:L_Small L101
U 1 1 6186AB9D
P 1550 3150
F 0 "L101" V 1735 3150 50  0000 C CNN
F 1 "100nH" V 1644 3150 50  0000 C CNN
F 2 "" H 1550 3150 50  0001 C CNN
F 3 "~" H 1550 3150 50  0001 C CNN
	1    1550 3150
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR0119
U 1 1 6186CAD1
P 1300 3100
F 0 "#PWR0119" H 1300 2950 50  0001 C CNN
F 1 "+5V" H 1315 3273 50  0000 C CNN
F 2 "" H 1300 3100 50  0001 C CNN
F 3 "" H 1300 3100 50  0001 C CNN
	1    1300 3100
	1    0    0    -1  
$EndComp
$Comp
L power:+5VA #PWR0120
U 1 1 6186D0A3
P 1750 3100
F 0 "#PWR0120" H 1750 2950 50  0001 C CNN
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
L power:GND #PWR0124
U 1 1 6186D3F7
P 1550 3500
F 0 "#PWR0124" H 1550 3250 50  0001 C CNN
F 1 "GND" H 1555 3327 50  0000 C CNN
F 2 "" H 1550 3500 50  0001 C CNN
F 3 "" H 1550 3500 50  0001 C CNN
	1    1550 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C107
U 1 1 6186C438
P 1300 3300
F 0 "C107" H 1392 3346 50  0000 L CNN
F 1 "1uF" H 1392 3255 50  0000 L CNN
F 2 "" H 1300 3300 50  0001 C CNN
F 3 "~" H 1300 3300 50  0001 C CNN
	1    1300 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C108
U 1 1 6186BFE0
P 1750 3300
F 0 "C108" H 1842 3346 50  0000 L CNN
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
L power:+5V #PWR0133
U 1 1 6187E0DD
P 5400 6600
F 0 "#PWR0133" H 5400 6450 50  0001 C CNN
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
L Switch:SW_Push SW104
U 1 1 6187D56B
P 5100 6700
F 0 "SW104" H 5100 6985 50  0000 C CNN
F 1 "SW_Push" H 5100 6894 50  0000 C CNN
F 2 "" H 5100 6900 50  0001 C CNN
F 3 "~" H 5100 6900 50  0001 C CNN
	1    5100 6700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0135
U 1 1 6187CD54
P 4800 7100
F 0 "#PWR0135" H 4800 6850 50  0001 C CNN
F 1 "GND" H 4805 6927 50  0000 C CNN
F 2 "" H 4800 7100 50  0001 C CNN
F 3 "" H 4800 7100 50  0001 C CNN
	1    4800 7100
	1    0    0    -1  
$EndComp
Text GLabel 4600 6700 0    50   Input ~ 0
SW2
$Comp
L Device:R R108
U 1 1 618502FD
P 4800 6900
F 0 "R108" H 4870 6946 50  0000 L CNN
F 1 "10k" H 4870 6855 50  0000 L CNN
F 2 "" V 4730 6900 50  0001 C CNN
F 3 "~" H 4800 6900 50  0001 C CNN
	1    4800 6900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0132
U 1 1 618892F2
P 4250 6600
F 0 "#PWR0132" H 4250 6450 50  0001 C CNN
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
L Switch:SW_Push SW103
U 1 1 61889585
P 3950 6700
F 0 "SW103" H 3950 6985 50  0000 C CNN
F 1 "SW_Push" H 3950 6894 50  0000 C CNN
F 2 "" H 3950 6900 50  0001 C CNN
F 3 "~" H 3950 6900 50  0001 C CNN
	1    3950 6700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0134
U 1 1 6188958F
P 3650 7100
F 0 "#PWR0134" H 3650 6850 50  0001 C CNN
F 1 "GND" H 3655 6927 50  0000 C CNN
F 2 "" H 3650 7100 50  0001 C CNN
F 3 "" H 3650 7100 50  0001 C CNN
	1    3650 7100
	1    0    0    -1  
$EndComp
Text GLabel 3450 6700 0    50   Input ~ 0
SW1
$Comp
L Device:R R107
U 1 1 6188959A
P 3650 6900
F 0 "R107" H 3720 6946 50  0000 L CNN
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
L power:+5V #PWR0127
U 1 1 61893890
P 1300 4100
F 0 "#PWR0127" H 1300 3950 50  0001 C CNN
F 1 "+5V" H 1315 4273 50  0000 C CNN
F 2 "" H 1300 4100 50  0001 C CNN
F 3 "" H 1300 4100 50  0001 C CNN
	1    1300 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0129
U 1 1 618938AF
P 1300 4550
F 0 "#PWR0129" H 1300 4300 50  0001 C CNN
F 1 "GND" H 1305 4377 50  0000 C CNN
F 2 "" H 1300 4550 50  0001 C CNN
F 3 "" H 1300 4550 50  0001 C CNN
	1    1300 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C111
U 1 1 618938B9
P 1300 4350
F 0 "C111" H 1100 4400 50  0000 L CNN
F 1 "1uF" H 1100 4250 50  0000 L CNN
F 2 "" H 1300 4350 50  0001 C CNN
F 3 "~" H 1300 4350 50  0001 C CNN
	1    1300 4350
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C112
U 1 1 618938C3
P 1500 4350
F 0 "C112" H 1600 4450 50  0000 L CNN
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
L power:+5V #PWR0125
U 1 1 618C2577
P 5250 3800
F 0 "#PWR0125" H 5250 3650 50  0001 C CNN
F 1 "+5V" H 5265 3973 50  0000 C CNN
F 2 "" H 5250 3800 50  0001 C CNN
F 3 "" H 5250 3800 50  0001 C CNN
	1    5250 3800
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0128
U 1 1 618C257D
P 5250 4250
F 0 "#PWR0128" H 5250 4000 50  0001 C CNN
F 1 "GND" H 5255 4077 50  0000 C CNN
F 2 "" H 5250 4250 50  0001 C CNN
F 3 "" H 5250 4250 50  0001 C CNN
	1    5250 4250
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C110
U 1 1 618C2583
P 5250 4050
F 0 "C110" H 5000 4100 50  0000 L CNN
F 1 "1uF" H 5050 3950 50  0000 L CNN
F 2 "" H 5250 4050 50  0001 C CNN
F 3 "~" H 5250 4050 50  0001 C CNN
	1    5250 4050
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C109
U 1 1 618C2589
P 5050 4050
F 0 "C109" H 5142 4096 50  0000 L CNN
F 1 "10nF" H 5142 4005 50  0000 L CNN
F 2 "" H 5050 4050 50  0001 C CNN
F 3 "~" H 5050 4050 50  0001 C CNN
	1    5050 4050
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5250 4250 5250 4200
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
$Comp
L Device:Crystal_Small Y102
U 1 1 618C5189
P 1150 5100
F 0 "Y102" H 1150 5325 50  0000 C CNN
F 1 "10MHz" H 1150 5234 50  0000 C CNN
F 2 "" H 1150 5100 50  0001 C CNN
F 3 "~" H 1150 5100 50  0001 C CNN
	1    1150 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C113
U 1 1 618C60AE
P 1050 5300
F 0 "C113" H 800 5350 50  0000 L CNN
F 1 "20pF" H 800 5200 50  0000 L CNN
F 2 "" H 1050 5300 50  0001 C CNN
F 3 "~" H 1050 5300 50  0001 C CNN
	1    1050 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C114
U 1 1 618C633B
P 1250 5300
F 0 "C114" H 1350 5350 50  0000 L CNN
F 1 "20pF" H 1300 5200 50  0000 L CNN
F 2 "" H 1250 5300 50  0001 C CNN
F 3 "~" H 1250 5300 50  0001 C CNN
	1    1250 5300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0130
U 1 1 618C693A
P 1150 5450
F 0 "#PWR0130" H 1150 5200 50  0001 C CNN
F 1 "GND" H 1155 5277 50  0000 C CNN
F 2 "" H 1150 5450 50  0001 C CNN
F 3 "" H 1150 5450 50  0001 C CNN
	1    1150 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 5450 1050 5450
Wire Wire Line
	1050 5450 1050 5400
Wire Wire Line
	1050 5200 1050 5100
Wire Wire Line
	1250 5100 1250 5200
Wire Wire Line
	1250 5450 1150 5450
Wire Wire Line
	1250 5400 1250 5450
Connection ~ 1150 5450
Text GLabel 1000 5100 0    40   Input ~ 0
PIC1_OSC1
Text GLabel 1300 5100 2    40   Input ~ 0
PIC2_OSC2
Wire Wire Line
	1000 5100 1050 5100
Connection ~ 1050 5100
Wire Wire Line
	1250 5100 1300 5100
Connection ~ 1250 5100
Wire Wire Line
	1750 4400 1750 4500
Wire Wire Line
	1750 4500 1500 4500
Wire Wire Line
	1750 4400 2150 4400
Connection ~ 1500 4500
Text GLabel 2150 4550 0    40   Input ~ 0
PIC1_OSC1
NoConn ~ 2150 4850
NoConn ~ 2150 5000
NoConn ~ 2150 5150
NoConn ~ 2150 5450
NoConn ~ 2150 5600
Text GLabel 2150 5300 0    50   Input ~ 0
PIC1_SCK
Text GLabel 4600 5300 2    50   Input ~ 0
PIC1_MISO
Text GLabel 4600 5150 2    50   Input ~ 0
PIC1_MOSI
NoConn ~ 4600 5000
NoConn ~ 4600 4850
NoConn ~ 4600 4700
NoConn ~ 4600 4550
NoConn ~ 4600 4400
NoConn ~ 4600 4250
$Comp
L Device:LED D102
U 1 1 6191A014
P 5400 1800
F 0 "D102" V 5439 1682 50  0000 R CNN
F 1 "LED" V 5348 1682 50  0000 R CNN
F 2 "" H 5400 1800 50  0001 C CNN
F 3 "~" H 5400 1800 50  0001 C CNN
	1    5400 1800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R101
U 1 1 6191A3E2
P 5400 1450
F 0 "R101" H 5330 1404 50  0000 R CNN
F 1 "4.7k" H 5330 1495 50  0000 R CNN
F 2 "" V 5330 1450 50  0001 C CNN
F 3 "~" H 5400 1450 50  0001 C CNN
	1    5400 1450
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 6191A3EC
P 5400 2000
F 0 "#PWR0113" H 5400 1750 50  0001 C CNN
F 1 "GND" H 5405 1827 50  0000 C CNN
F 2 "" H 5400 2000 50  0001 C CNN
F 3 "" H 5400 2000 50  0001 C CNN
	1    5400 2000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0105
U 1 1 6191FF81
P 5400 1200
F 0 "#PWR0105" H 5400 1050 50  0001 C CNN
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
Text Notes 750  7050 0    150  ~ 30
NODE 1
$Comp
L power:GND #PWR0131
U 1 1 61A2CC88
P 5550 5900
F 0 "#PWR0131" H 5550 5650 50  0001 C CNN
F 1 "GND" H 5555 5727 50  0000 C CNN
F 2 "" H 5550 5900 50  0001 C CNN
F 3 "" H 5550 5900 50  0001 C CNN
	1    5550 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D103
U 1 1 61A2D217
P 5550 5700
F 0 "D103" V 5589 5582 50  0000 R CNN
F 1 "LED" V 5498 5582 50  0000 R CNN
F 2 "" H 5550 5700 50  0001 C CNN
F 3 "~" H 5550 5700 50  0001 C CNN
	1    5550 5700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5550 5900 5550 5850
Wire Wire Line
	5550 5550 5550 5450
Text Notes 5500 5400 0    50   ~ 10
Debug LED
Wire Wire Line
	4600 5450 5150 5450
Wire Wire Line
	5450 5450 5550 5450
$Comp
L Device:R R106
U 1 1 61A2D221
P 5300 5450
F 0 "R106" V 5093 5450 50  0000 C CNN
F 1 "4.7k" V 5184 5450 50  0000 C CNN
F 2 "" V 5230 5450 50  0001 C CNN
F 3 "~" H 5300 5450 50  0001 C CNN
	1    5300 5450
	0    1    1    0   
$EndComp
Wire Notes Line
	4550 2300 4550 2000
Wire Notes Line
	4550 2000 2850 2000
Wire Notes Line
	2850 2000 2850 700 
Text Notes 2900 1950 0    100  ~ 20
5V POWER
Text Notes 800  7450 0    70   ~ 14
Read button states.\nSend state over CAN.\nReceive command for Debug LED.
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
L power:+5V #PWR0104
U 1 1 61D9D7C9
P 5000 1200
F 0 "#PWR0104" H 5000 1050 50  0001 C CNN
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
NoConn ~ 3500 1300
Text GLabel 4600 5600 2    50   Input ~ 0
PIC1_~SS
Text GLabel 2150 4700 0    40   Input ~ 0
PIC2_OSC2
$Comp
L Switch:SW_SPDT SW101
U 1 1 61D41444
P 3300 1400
F 0 "SW101" H 3250 1550 50  0000 C CNN
F 1 "SW_SPDT" H 3150 1250 50  0000 C CNN
F 2 "" H 3300 1400 50  0001 C CNN
F 3 "~" H 3300 1400 50  0001 C CNN
	1    3300 1400
	1    0    0    -1  
$EndComp
$Comp
L power:VPP #PWR0103
U 1 1 6185940E
P 3000 1200
F 0 "#PWR0103" H 3000 1050 50  0001 C CNN
F 1 "VPP" H 3015 1373 50  0000 C CNN
F 2 "" H 3000 1200 50  0001 C CNN
F 3 "" H 3000 1200 50  0001 C CNN
	1    3000 1200
	1    0    0    -1  
$EndComp
Text GLabel 4600 3800 2    30   Input ~ 0
CAN1_~RX0BF
Text GLabel 4600 3650 2    30   Input ~ 0
CAN1_~RX1BF
Wire Wire Line
	5250 3800 5250 3850
Wire Wire Line
	4600 3950 4950 3950
Wire Wire Line
	5050 3950 5050 3850
Wire Wire Line
	5050 3850 5250 3850
Connection ~ 5250 3850
Wire Wire Line
	5250 3850 5250 3950
Wire Wire Line
	4950 3950 4950 3850
Wire Wire Line
	4950 3850 5050 3850
Connection ~ 5050 3850
Wire Wire Line
	5050 3050 5150 3050
Wire Wire Line
	5150 3050 5150 3100
Text GLabel 4600 3500 2    30   Input ~ 0
CAN1_~INT
$Comp
L Interface_CAN_LIN:MCP2562-E-P U102
U 1 1 61872B82
P 10050 2050
F 0 "U102" H 9700 2400 50  0000 C CNN
F 1 "MCP2562-E-P" H 10400 2400 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 10050 1550 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/25167A.pdf" H 10050 2050 50  0001 C CNN
	1    10050 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R105
U 1 1 6187DD36
P 9400 3200
F 0 "R105" H 9470 3246 50  0000 L CNN
F 1 "10k" H 9470 3155 50  0000 L CNN
F 2 "" V 9330 3200 50  0001 C CNN
F 3 "~" H 9400 3200 50  0001 C CNN
	1    9400 3200
	1    0    0    -1  
$EndComp
Text GLabel 7750 1800 0    50   Input ~ 0
PIC1_MOSI
Text GLabel 7750 1900 0    50   Input ~ 0
PIC1_MISO
Text GLabel 7750 2000 0    35   Input ~ 0
PIC1_~SS
Text GLabel 7750 2100 0    50   Input ~ 0
PIC1_SCK
$Comp
L Device:Crystal_Small Y101
U 1 1 618821D9
P 7100 2900
F 0 "Y101" H 7100 3125 50  0000 C CNN
F 1 "40MHz" H 7100 3034 50  0000 C CNN
F 2 "" H 7100 2900 50  0001 C CNN
F 3 "~" H 7100 2900 50  0001 C CNN
	1    7100 2900
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C105
U 1 1 61882631
P 7000 3100
F 0 "C105" H 6750 3150 50  0000 L CNN
F 1 "20pF" H 6750 3000 50  0000 L CNN
F 2 "" H 7000 3100 50  0001 C CNN
F 3 "~" H 7000 3100 50  0001 C CNN
	1    7000 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C106
U 1 1 6188263B
P 7200 3100
F 0 "C106" H 7300 3150 50  0000 L CNN
F 1 "20pF" H 7250 3000 50  0000 L CNN
F 2 "" H 7200 3100 50  0001 C CNN
F 3 "~" H 7200 3100 50  0001 C CNN
	1    7200 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0122
U 1 1 61882645
P 7100 3250
F 0 "#PWR0122" H 7100 3000 50  0001 C CNN
F 1 "GND" H 7105 3077 50  0000 C CNN
F 2 "" H 7100 3250 50  0001 C CNN
F 3 "" H 7100 3250 50  0001 C CNN
	1    7100 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 3250 7000 3250
Wire Wire Line
	7000 3250 7000 3200
Wire Wire Line
	7000 3000 7000 2900
Wire Wire Line
	7200 2900 7200 3000
Wire Wire Line
	7200 3250 7100 3250
Wire Wire Line
	7200 3200 7200 3250
Connection ~ 7100 3250
Text GLabel 6950 2900 0    40   Input ~ 0
CAN1_OSC1
Text GLabel 7250 2900 2    40   Input ~ 0
CAN1_OSC2
Wire Wire Line
	6950 2900 7000 2900
Connection ~ 7000 2900
Wire Wire Line
	7200 2900 7250 2900
Connection ~ 7200 2900
Text GLabel 7750 2700 0    40   Input ~ 0
CAN1_OSC1
Text GLabel 7750 2600 0    40   Input ~ 0
CAN1_OSC2
NoConn ~ 7750 2800
$Comp
L power:GND #PWR0123
U 1 1 618B461A
P 8350 3250
F 0 "#PWR0123" H 8350 3000 50  0001 C CNN
F 1 "GND" H 8355 3077 50  0000 C CNN
F 2 "" H 8350 3250 50  0001 C CNN
F 3 "" H 8350 3250 50  0001 C CNN
	1    8350 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 3250 8350 3200
$Comp
L power:+5V #PWR0109
U 1 1 618B7F2B
P 8350 1550
F 0 "#PWR0109" H 8350 1400 50  0001 C CNN
F 1 "+5V" H 8350 1700 50  0000 C CNN
F 2 "" H 8350 1550 50  0001 C CNN
F 3 "" H 8350 1550 50  0001 C CNN
	1    8350 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 1600 8350 1550
$Comp
L power:+5V #PWR0111
U 1 1 618BBB78
P 10050 1600
F 0 "#PWR0111" H 10050 1450 50  0001 C CNN
F 1 "+5V" H 10065 1773 50  0000 C CNN
F 2 "" H 10050 1600 50  0001 C CNN
F 3 "" H 10050 1600 50  0001 C CNN
	1    10050 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 618BBEFD
P 10050 2500
F 0 "#PWR0117" H 10050 2250 50  0001 C CNN
F 1 "GND" H 10055 2327 50  0000 C CNN
F 2 "" H 10050 2500 50  0001 C CNN
F 3 "" H 10050 2500 50  0001 C CNN
	1    10050 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 1650 10050 1600
Wire Wire Line
	10050 2450 10050 2500
$Comp
L Interface_CAN_LIN_mine:MCP2515-xSO U103
U 1 1 618D619D
P 8350 2400
F 0 "U103" H 7900 3150 50  0000 C CNN
F 1 "MCP2515-xSO" H 8700 3150 50  0000 C CNN
F 2 "Package_SO:SOIC-18W_7.5x11.6mm_P1.27mm" H 8350 1500 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21801e.pdf" H 8450 1600 50  0001 C CNN
	1    8350 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 1850 9550 1850
Wire Wire Line
	8950 1950 9550 1950
Text GLabel 8950 2500 2    30   Input ~ 0
CAN1_~RX0BF
Text GLabel 8950 2600 2    30   Input ~ 0
CAN1_~RX1BF
$Comp
L Switch:SW_Push SW102
U 1 1 6190D3F8
P 9400 3650
F 0 "SW102" V 9400 3550 50  0000 C CNN
F 1 "CAN1_RST" V 9400 3950 50  0000 C CNN
F 2 "" H 9400 3850 50  0001 C CNN
F 3 "~" H 9400 3850 50  0001 C CNN
	1    9400 3650
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0118
U 1 1 619123F1
P 9400 2700
F 0 "#PWR0118" H 9400 2550 50  0001 C CNN
F 1 "+5V" H 9400 2850 50  0000 C CNN
F 2 "" H 9400 2700 50  0001 C CNN
F 3 "" H 9400 2700 50  0001 C CNN
	1    9400 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8950 2700 9400 2700
Wire Wire Line
	8950 2800 9400 2800
Wire Wire Line
	9400 2800 9400 2700
Connection ~ 9400 2700
Wire Wire Line
	8950 2900 9400 2900
Wire Wire Line
	9400 2900 9400 2800
Connection ~ 9400 2800
Wire Wire Line
	9400 3050 9400 2900
Connection ~ 9400 2900
Wire Wire Line
	9400 3350 9400 3400
Wire Wire Line
	9400 3400 9150 3400
Wire Wire Line
	9150 3400 9150 3000
Wire Wire Line
	9150 3000 8950 3000
Connection ~ 9400 3400
Wire Wire Line
	9400 3400 9400 3450
$Comp
L power:GND #PWR0126
U 1 1 619373CD
P 9400 3900
F 0 "#PWR0126" H 9400 3650 50  0001 C CNN
F 1 "GND" H 9405 3727 50  0000 C CNN
F 2 "" H 9400 3900 50  0001 C CNN
F 3 "" H 9400 3900 50  0001 C CNN
	1    9400 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 3900 9400 3850
Text GLabel 8950 2400 2    30   Input ~ 0
CAN1_~INT
$Comp
L power:+5V #PWR0101
U 1 1 619412FD
P 8650 1000
F 0 "#PWR0101" H 8650 850 50  0001 C CNN
F 1 "+5V" H 8550 1100 50  0000 C CNN
F 2 "" H 8650 1000 50  0001 C CNN
F 3 "" H 8650 1000 50  0001 C CNN
	1    8650 1000
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 619417CF
P 8650 1300
F 0 "#PWR0106" H 8650 1050 50  0001 C CNN
F 1 "GND" H 8500 1250 50  0000 C CNN
F 2 "" H 8650 1300 50  0001 C CNN
F 3 "" H 8650 1300 50  0001 C CNN
	1    8650 1300
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C101
U 1 1 619417D9
P 8650 1150
F 0 "C101" H 8350 1150 50  0000 L CNN
F 1 "0.1uF" H 8750 1150 50  0000 L CNN
F 2 "" H 8650 1150 50  0001 C CNN
F 3 "~" H 8650 1150 50  0001 C CNN
	1    8650 1150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8650 1000 8650 1050
Wire Wire Line
	8650 1250 8650 1300
$Comp
L power:+5V #PWR0102
U 1 1 6195BCFE
P 10350 1050
F 0 "#PWR0102" H 10350 900 50  0001 C CNN
F 1 "+5V" H 10250 1150 50  0000 C CNN
F 2 "" H 10350 1050 50  0001 C CNN
F 3 "" H 10350 1050 50  0001 C CNN
	1    10350 1050
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 6195C1FE
P 10350 1350
F 0 "#PWR0107" H 10350 1100 50  0001 C CNN
F 1 "GND" H 10200 1300 50  0000 C CNN
F 2 "" H 10350 1350 50  0001 C CNN
F 3 "" H 10350 1350 50  0001 C CNN
	1    10350 1350
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C102
U 1 1 6195C208
P 10350 1200
F 0 "C102" H 10050 1200 50  0000 L CNN
F 1 "0.1uF" H 10450 1200 50  0000 L CNN
F 2 "" H 10350 1200 50  0001 C CNN
F 3 "~" H 10350 1200 50  0001 C CNN
	1    10350 1200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10350 1050 10350 1100
Wire Wire Line
	10350 1300 10350 1350
$Comp
L power:+5V #PWR0114
U 1 1 61963C38
P 9450 2150
F 0 "#PWR0114" H 9450 2000 50  0001 C CNN
F 1 "+5V" H 9350 2150 50  0000 C CNN
F 2 "" H 9450 2150 50  0001 C CNN
F 3 "" H 9450 2150 50  0001 C CNN
	1    9450 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 2150 9550 2150
$Comp
L power:GND #PWR0116
U 1 1 61968BE9
P 9450 2250
F 0 "#PWR0116" H 9450 2000 50  0001 C CNN
F 1 "GND" H 9300 2200 50  0000 C CNN
F 2 "" H 9450 2250 50  0001 C CNN
F 3 "" H 9450 2250 50  0001 C CNN
	1    9450 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 2250 9550 2250
Text Notes 9700 3000 0    25   ~ 0
Vio is for what voltage level the logic\ncircuitry is at, which is 5V here.\n\nSTBY is for putting the transceiver in\nstandby mode or normal mode. 0V for normal.
Text GLabel 10550 1950 2    50   Input ~ 0
CANH
Text GLabel 10550 2150 2    50   Input ~ 0
CANL
Wire Notes Line
	6350 650  10950 650 
Wire Notes Line
	10950 650  10950 4300
Wire Notes Line
	10950 4300 6350 4300
Wire Notes Line
	6350 650  6350 4300
Text Notes 6450 1050 0    100  ~ 20
CAN CONTROLLER +\nTRANSCEIVER
Text Notes 6500 4150 0    50   ~ 0
CAN Bus Format: CAN 2.0B Extended Frame\nCAN Bus Baud Rate: 250kbps
$Sheet
S 8950 5100 1850 1000
U 619B39CA
F0 "CAN Node 2" 50
F1 "can_node_2sch.sch" 50
$EndSheet
Text Notes 1600 -2400 0    100  ~ 20
5V POWER
Text Notes 2900 950  0    40   ~ 0
External Power
$EndSCHEMATC
