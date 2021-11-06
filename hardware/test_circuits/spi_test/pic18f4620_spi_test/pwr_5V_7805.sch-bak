EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
Title "5V Power L7805"
Date "2021-11-06"
Rev "1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 "Author: Abdullah Almosalami"
$EndDescr
$Comp
L Regulator_Linear:L7805 U?
U 1 1 61A28F55
P 6150 3600
F 0 "U?" H 6150 3842 50  0000 C CNN
F 1 "L7805" H 6150 3751 50  0000 C CNN
F 2 "" H 6175 3450 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 6150 3550 50  0001 C CNN
	1    6150 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A28F5B
P 6150 4050
F 0 "#PWR?" H 6150 3800 50  0001 C CNN
F 1 "GND" H 6155 3877 50  0000 C CNN
F 2 "" H 6150 4050 50  0001 C CNN
F 3 "" H 6150 4050 50  0001 C CNN
	1    6150 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 4050 6150 4000
$Comp
L Device:C_Small C?
U 1 1 61A28F62
P 5700 3850
F 0 "C?" H 5500 3900 50  0000 L CNN
F 1 "0.1uF" H 5400 3800 50  0000 L CNN
F 2 "" H 5700 3850 50  0001 C CNN
F 3 "~" H 5700 3850 50  0001 C CNN
	1    5700 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A28F68
P 6600 3850
F 0 "C?" H 6692 3896 50  0000 L CNN
F 1 "10uF" H 6692 3805 50  0000 L CNN
F 2 "" H 6600 3850 50  0001 C CNN
F 3 "~" H 6600 3850 50  0001 C CNN
	1    6600 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 3600 5700 3600
Wire Wire Line
	5700 3950 5700 4000
Wire Wire Line
	5700 4000 6150 4000
Connection ~ 6150 4000
Wire Wire Line
	6150 4000 6150 3900
Wire Wire Line
	6600 3950 6600 4000
Wire Wire Line
	6600 4000 6150 4000
Wire Wire Line
	5700 3750 5700 3600
Connection ~ 5700 3600
Wire Wire Line
	5700 3600 5850 3600
Wire Wire Line
	6450 3600 6600 3600
Wire Wire Line
	6600 3600 6600 3750
$Comp
L Device:D_Schottky D?
U 1 1 61A28F7A
P 5500 3600
F 0 "D?" H 5500 3383 50  0000 C CNN
F 1 "D_Schottky" H 5500 3474 50  0000 C CNN
F 2 "" H 5500 3600 50  0001 C CNN
F 3 "~" H 5500 3600 50  0001 C CNN
	1    5500 3600
	-1   0    0    1   
$EndComp
Text Notes 5050 3300 0    20   ~ 0
This is minimal input voltage protection (reverse polarity).\nWhen breadboarding, it's easy to make mistakes, so ideally,\nyou'd use more protection - clamp diodes to ground,\np-channel mosfet reverse-polarity protection, a fuse, etc.
$Comp
L Device:LED D?
U 1 1 61A28F81
P 7000 4150
F 0 "D?" V 7039 4032 50  0000 R CNN
F 1 "LED" V 6948 4032 50  0000 R CNN
F 2 "" H 7000 4150 50  0001 C CNN
F 3 "~" H 7000 4150 50  0001 C CNN
	1    7000 4150
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 61A28F87
P 7000 3800
F 0 "R?" H 6930 3754 50  0000 R CNN
F 1 "4.7k" H 6930 3845 50  0000 R CNN
F 2 "" V 6930 3800 50  0001 C CNN
F 3 "~" H 7000 3800 50  0001 C CNN
	1    7000 3800
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A28F8D
P 7000 4350
F 0 "#PWR?" H 7000 4100 50  0001 C CNN
F 1 "GND" H 7005 4177 50  0000 C CNN
F 2 "" H 7000 4350 50  0001 C CNN
F 3 "" H 7000 4350 50  0001 C CNN
	1    7000 4350
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 61A28F93
P 7000 3550
F 0 "#PWR?" H 7000 3400 50  0001 C CNN
F 1 "+5V" H 7015 3723 50  0000 C CNN
F 2 "" H 7000 3550 50  0001 C CNN
F 3 "" H 7000 3550 50  0001 C CNN
	1    7000 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 4300 7000 4350
Wire Wire Line
	7000 3950 7000 4000
Wire Notes Line
	6150 4650 6150 4350
Wire Notes Line
	6150 4350 4450 4350
Wire Notes Line
	4450 4350 4450 3050
Text Notes 4500 4300 0    100  ~ 20
5V POWER
Wire Wire Line
	7000 3550 7000 3650
Wire Wire Line
	4600 3750 4700 3750
Wire Wire Line
	4600 3550 4600 3750
Wire Wire Line
	5100 3850 5250 3850
Wire Wire Line
	5250 3850 5250 3600
Wire Wire Line
	5250 3600 5350 3600
$Comp
L power:+5V #PWR?
U 1 1 61A28FA5
P 6600 3550
F 0 "#PWR?" H 6600 3400 50  0001 C CNN
F 1 "+5V" H 6615 3723 50  0000 C CNN
F 2 "" H 6600 3550 50  0001 C CNN
F 3 "" H 6600 3550 50  0001 C CNN
	1    6600 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 3600 6600 3550
Connection ~ 6600 3600
Wire Notes Line
	7350 4650 7350 3050
Wire Notes Line
	6150 4650 7350 4650
Wire Notes Line
	4450 3050 7350 3050
NoConn ~ 5100 3650
$Comp
L Switch:SW_SPDT SW?
U 1 1 61A28FB1
P 4900 3750
F 0 "SW?" H 4850 3900 50  0000 C CNN
F 1 "SW_SPDT" H 4750 3600 50  0000 C CNN
F 2 "" H 4900 3750 50  0001 C CNN
F 3 "~" H 4900 3750 50  0001 C CNN
	1    4900 3750
	1    0    0    -1  
$EndComp
$Comp
L power:VPP #PWR?
U 1 1 61A28FB7
P 4600 3550
F 0 "#PWR?" H 4600 3400 50  0001 C CNN
F 1 "VPP" H 4615 3723 50  0000 C CNN
F 2 "" H 4600 3550 50  0001 C CNN
F 3 "" H 4600 3550 50  0001 C CNN
	1    4600 3550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
