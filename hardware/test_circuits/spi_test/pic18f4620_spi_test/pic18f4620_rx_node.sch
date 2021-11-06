EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title "PIC18F4620 Set Up as RX Node"
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
U 1 1 61A3F3F3
P 3750 3550
AR Path="/61A3F3F3" Ref="U?"  Part="1" 
AR Path="/61A0C441/61A3F3F3" Ref="U?"  Part="1" 
F 0 "U?" H 3725 5415 50  0000 C CNN
F 1 "PIC18F4620-IP" H 3725 5324 50  0000 C CNN
F 2 "" H 3600 4100 50  0001 C CNN
F 3 "" H 3600 4100 50  0001 C CNN
	1    3750 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61A3F3F9
P 2350 1750
AR Path="/61A3F3F9" Ref="R?"  Part="1" 
AR Path="/61A0C441/61A3F3F9" Ref="R?"  Part="1" 
F 0 "R?" H 2420 1796 50  0000 L CNN
F 1 "10K" H 2420 1705 50  0000 L CNN
F 2 "" V 2280 1750 50  0001 C CNN
F 3 "~" H 2350 1750 50  0001 C CNN
	1    2350 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61A3F3FF
P 2100 2050
AR Path="/61A3F3FF" Ref="R?"  Part="1" 
AR Path="/61A0C441/61A3F3FF" Ref="R?"  Part="1" 
F 0 "R?" V 1893 2050 50  0000 C CNN
F 1 "51" V 1984 2050 50  0000 C CNN
F 2 "" V 2030 2050 50  0001 C CNN
F 3 "~" H 2100 2050 50  0001 C CNN
	1    2100 2050
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x05_Female J?
U 1 1 61A3F405
P 5900 6200
AR Path="/61A3F405" Ref="J?"  Part="1" 
AR Path="/61A0C441/61A3F405" Ref="J?"  Part="1" 
F 0 "J?" H 6000 6200 50  0000 C CNN
F 1 "Conn_01x05_Female" H 5800 6600 50  0001 C CNN
F 2 "" H 5900 6200 50  0001 C CNN
F 3 "~" H 5900 6200 50  0001 C CNN
	1    5900 6200
	1    0    0    -1  
$EndComp
Text GLabel 5600 6000 0    40   Input ~ 0
~MCLR
Text GLabel 5600 6300 0    40   Input ~ 0
PGD
Text GLabel 5600 6400 0    40   Input ~ 0
PGC
$Comp
L power:+5V #PWR?
U 1 1 61A3F40E
P 5250 6050
AR Path="/61A3F40E" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F40E" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5250 5900 50  0001 C CNN
F 1 "+5V" H 5265 6223 50  0000 C CNN
F 2 "" H 5250 6050 50  0001 C CNN
F 3 "" H 5250 6050 50  0001 C CNN
	1    5250 6050
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A3F414
P 5250 6250
AR Path="/61A3F414" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F414" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5250 6000 50  0001 C CNN
F 1 "GND" H 5255 6077 50  0000 C CNN
F 2 "" H 5250 6250 50  0001 C CNN
F 3 "" H 5250 6250 50  0001 C CNN
	1    5250 6250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5250 6100 5700 6100
Wire Wire Line
	5700 6200 5250 6200
Wire Wire Line
	5700 6000 5600 6000
Wire Wire Line
	5700 6300 5600 6300
Wire Wire Line
	5700 6400 5600 6400
Wire Wire Line
	5250 6100 5250 6050
Wire Wire Line
	5250 6200 5250 6250
Text GLabel 1800 2050 0    40   Input ~ 0
~MCLR
Wire Wire Line
	1800 2050 1950 2050
Wire Wire Line
	2250 2050 2350 2050
$Comp
L power:+5V #PWR?
U 1 1 61A3F424
P 2350 1500
AR Path="/61A3F424" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F424" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2350 1350 50  0001 C CNN
F 1 "+5V" H 2365 1673 50  0000 C CNN
F 2 "" H 2350 1500 50  0001 C CNN
F 3 "" H 2350 1500 50  0001 C CNN
	1    2350 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 2050 2350 1900
Connection ~ 2350 2050
Wire Wire Line
	2350 2050 2500 2050
Wire Wire Line
	2350 1600 2350 1500
Text Notes 1500 1800 0    25   ~ 0
51 ohm resistor in series /w MCLR\nimportant! Seen videos where PICKit 4\nbricks uCs because of voltage spikes\non ~MCLR.
NoConn ~ 2500 2200
NoConn ~ 2500 2350
Wire Wire Line
	5100 2350 5100 2400
Wire Wire Line
	5100 2750 5100 2700
$Comp
L power:GND #PWR?
U 1 1 61A3F433
P 5100 2750
AR Path="/61A3F433" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F433" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5100 2500 50  0001 C CNN
F 1 "GND" H 5105 2577 50  0000 C CNN
F 2 "" H 5100 2750 50  0001 C CNN
F 3 "" H 5100 2750 50  0001 C CNN
	1    5100 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2350 5100 2350
$Comp
L Device:R R?
U 1 1 61A3F43A
P 5100 2550
AR Path="/61A3F43A" Ref="R?"  Part="1" 
AR Path="/61A0C441/61A3F43A" Ref="R?"  Part="1" 
F 0 "R?" H 5170 2596 50  0000 L CNN
F 1 "R" H 5170 2505 50  0000 L CNN
F 2 "" V 5030 2550 50  0001 C CNN
F 3 "~" H 5100 2550 50  0001 C CNN
	1    5100 2550
	1    0    0    -1  
$EndComp
NoConn ~ 4950 2500
NoConn ~ 4950 2650
NoConn ~ 4950 2800
NoConn ~ 4950 2950
NoConn ~ 4950 3100
Text GLabel 4950 2200 2    40   Input ~ 0
PGC
Text GLabel 4950 2050 2    40   Input ~ 0
PGD
NoConn ~ 2500 2800
NoConn ~ 2500 3400
$Comp
L power:+5V #PWR?
U 1 1 61A3F449
P 1650 3400
AR Path="/61A3F449" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F449" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1650 3250 50  0001 C CNN
F 1 "+5V" H 1665 3573 50  0000 C CNN
F 2 "" H 1650 3400 50  0001 C CNN
F 3 "" H 1650 3400 50  0001 C CNN
	1    1650 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A3F44F
P 1650 3850
AR Path="/61A3F44F" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F44F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1650 3600 50  0001 C CNN
F 1 "GND" H 1655 3677 50  0000 C CNN
F 2 "" H 1650 3850 50  0001 C CNN
F 3 "" H 1650 3850 50  0001 C CNN
	1    1650 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A3F455
P 1650 3650
AR Path="/61A3F455" Ref="C?"  Part="1" 
AR Path="/61A0C441/61A3F455" Ref="C?"  Part="1" 
F 0 "C?" H 1450 3700 50  0000 L CNN
F 1 "1uF" H 1450 3550 50  0000 L CNN
F 2 "" H 1650 3650 50  0001 C CNN
F 3 "~" H 1650 3650 50  0001 C CNN
	1    1650 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A3F45B
P 1850 3650
AR Path="/61A3F45B" Ref="C?"  Part="1" 
AR Path="/61A0C441/61A3F45B" Ref="C?"  Part="1" 
F 0 "C?" H 1950 3750 50  0000 L CNN
F 1 "10nF" H 1950 3650 50  0000 L CNN
F 2 "" H 1850 3650 50  0001 C CNN
F 3 "~" H 1850 3650 50  0001 C CNN
	1    1850 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 3450 2350 3550
Wire Wire Line
	2350 3550 2500 3550
Wire Wire Line
	5100 3150 5100 3250
Wire Wire Line
	5100 3250 4950 3250
Wire Wire Line
	4950 3400 5100 3400
Wire Wire Line
	5100 3400 5100 3500
Wire Wire Line
	1650 3850 1650 3800
Wire Wire Line
	1650 3550 1650 3450
Wire Wire Line
	1650 3450 1850 3450
Connection ~ 1650 3450
Wire Wire Line
	1650 3450 1650 3400
Wire Wire Line
	1650 3800 1850 3800
Connection ~ 1650 3800
Wire Wire Line
	1650 3800 1650 3750
Wire Wire Line
	1850 3800 1850 3750
Wire Wire Line
	1850 3550 1850 3450
Connection ~ 1850 3450
Wire Wire Line
	1850 3450 2350 3450
$Comp
L power:+5V #PWR?
U 1 1 61A3F473
P 5600 3100
AR Path="/61A3F473" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F473" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5600 2950 50  0001 C CNN
F 1 "+5V" H 5615 3273 50  0000 C CNN
F 2 "" H 5600 3100 50  0001 C CNN
F 3 "" H 5600 3100 50  0001 C CNN
	1    5600 3100
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A3F479
P 5600 3550
AR Path="/61A3F479" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F479" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5600 3300 50  0001 C CNN
F 1 "GND" H 5605 3377 50  0000 C CNN
F 2 "" H 5600 3550 50  0001 C CNN
F 3 "" H 5600 3550 50  0001 C CNN
	1    5600 3550
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A3F47F
P 5600 3350
AR Path="/61A3F47F" Ref="C?"  Part="1" 
AR Path="/61A0C441/61A3F47F" Ref="C?"  Part="1" 
F 0 "C?" H 5400 3400 50  0000 L CNN
F 1 "1uF" H 5400 3250 50  0000 L CNN
F 2 "" H 5600 3350 50  0001 C CNN
F 3 "~" H 5600 3350 50  0001 C CNN
	1    5600 3350
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A3F485
P 5400 3350
AR Path="/61A3F485" Ref="C?"  Part="1" 
AR Path="/61A0C441/61A3F485" Ref="C?"  Part="1" 
F 0 "C?" H 5492 3396 50  0000 L CNN
F 1 "10nF" H 5492 3305 50  0000 L CNN
F 2 "" H 5400 3350 50  0001 C CNN
F 3 "~" H 5400 3350 50  0001 C CNN
	1    5400 3350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5600 3550 5600 3500
Wire Wire Line
	5600 3250 5600 3150
Wire Wire Line
	5600 3150 5400 3150
Connection ~ 5600 3150
Wire Wire Line
	5600 3150 5600 3100
Wire Wire Line
	5600 3500 5400 3500
Connection ~ 5600 3500
Wire Wire Line
	5600 3500 5600 3450
Wire Wire Line
	5400 3500 5400 3450
Connection ~ 5400 3500
Wire Wire Line
	5400 3500 5100 3500
Wire Wire Line
	5400 3250 5400 3150
Connection ~ 5400 3150
Wire Wire Line
	5400 3150 5100 3150
$Comp
L Device:Crystal_Small Y?
U 1 1 61A3F499
P 1700 4450
AR Path="/61A3F499" Ref="Y?"  Part="1" 
AR Path="/61A0C441/61A3F499" Ref="Y?"  Part="1" 
F 0 "Y?" H 1700 4675 50  0000 C CNN
F 1 "10MHz" H 1700 4584 50  0000 C CNN
F 2 "" H 1700 4450 50  0001 C CNN
F 3 "~" H 1700 4450 50  0001 C CNN
	1    1700 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A3F49F
P 1600 4650
AR Path="/61A3F49F" Ref="C?"  Part="1" 
AR Path="/61A0C441/61A3F49F" Ref="C?"  Part="1" 
F 0 "C?" H 1400 4700 50  0000 L CNN
F 1 "20pF" H 1350 4550 50  0000 L CNN
F 2 "" H 1600 4650 50  0001 C CNN
F 3 "~" H 1600 4650 50  0001 C CNN
	1    1600 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 61A3F4A5
P 1800 4650
AR Path="/61A3F4A5" Ref="C?"  Part="1" 
AR Path="/61A0C441/61A3F4A5" Ref="C?"  Part="1" 
F 0 "C?" H 1900 4700 50  0000 L CNN
F 1 "20pF" H 1850 4550 50  0000 L CNN
F 2 "" H 1800 4650 50  0001 C CNN
F 3 "~" H 1800 4650 50  0001 C CNN
	1    1800 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A3F4AB
P 1700 4800
AR Path="/61A3F4AB" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F4AB" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1700 4550 50  0001 C CNN
F 1 "GND" H 1705 4627 50  0000 C CNN
F 2 "" H 1700 4800 50  0001 C CNN
F 3 "" H 1700 4800 50  0001 C CNN
	1    1700 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 4800 1600 4800
Wire Wire Line
	1600 4800 1600 4750
Wire Wire Line
	1600 4550 1600 4450
Wire Wire Line
	1800 4450 1800 4550
Wire Wire Line
	1800 4800 1700 4800
Wire Wire Line
	1800 4750 1800 4800
Connection ~ 1700 4800
Text GLabel 1550 4450 0    40   Input ~ 0
OSC1
Text GLabel 1850 4450 2    40   Input ~ 0
OSC2
Wire Wire Line
	1550 4450 1600 4450
Connection ~ 1600 4450
Wire Wire Line
	1800 4450 1850 4450
Connection ~ 1800 4450
Wire Wire Line
	2100 3700 2100 3800
Wire Wire Line
	2100 3800 1850 3800
Wire Wire Line
	2100 3700 2500 3700
Connection ~ 1850 3800
NoConn ~ 2500 4750
Text GLabel 2500 4600 0    50   Input ~ 0
SCK
$Comp
L power:GND #PWR?
U 1 1 61A3F4C4
P 1550 6050
AR Path="/61A3F4C4" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F4C4" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1550 5800 50  0001 C CNN
F 1 "GND" H 1555 5877 50  0000 C CNN
F 2 "" H 1550 6050 50  0001 C CNN
F 3 "" H 1550 6050 50  0001 C CNN
	1    1550 6050
	1    0    0    -1  
$EndComp
NoConn ~ 4950 4900
Text GLabel 4950 4450 2    50   Input ~ 0
MISO
Text GLabel 4950 4600 2    50   Input ~ 0
MOSI
NoConn ~ 4950 4300
NoConn ~ 4950 4150
Text Notes 5400 5800 0    50   ~ 0
ICSP Header
Text Notes 4750 1350 0    150  ~ 30
RX NODE
Text GLabel 2500 2950 0    50   Input ~ 0
~SS
Text GLabel 2500 3850 0    50   Input ~ 0
OSC1
Text GLabel 2500 4000 0    50   Input ~ 0
OSC2
NoConn ~ 2500 2500
NoConn ~ 2500 2650
$Comp
L Device:LED D?
U 1 1 61A3F4D6
P 1550 5850
AR Path="/61A3F4D6" Ref="D?"  Part="1" 
AR Path="/61A0C441/61A3F4D6" Ref="D?"  Part="1" 
F 0 "D?" V 1600 6000 50  0000 R CNN
F 1 "LED" V 1500 6050 50  0000 R CNN
F 2 "" H 1550 5850 50  0001 C CNN
F 3 "~" H 1550 5850 50  0001 C CNN
	1    1550 5850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 61A3F4DC
P 1700 5650
AR Path="/61A3F4DC" Ref="R?"  Part="1" 
AR Path="/61A0C441/61A3F4DC" Ref="R?"  Part="1" 
F 0 "R?" V 1800 5650 50  0000 C CNN
F 1 "4.7k" V 1600 5600 50  0000 C CNN
F 2 "" V 1630 5650 50  0001 C CNN
F 3 "~" H 1700 5650 50  0001 C CNN
	1    1700 5650
	0    -1   -1   0   
$EndComp
Text Notes 1700 6000 0    50   ~ 10
Debug LED
Text Notes 5500 4550 0    25   ~ 5
Careful to remember SDO of master goes\nto SDI of slave, and vice versa!
$Comp
L Display_Character:HY1602E DS?
U 1 1 61A3F4E4
P 3750 5950
AR Path="/61A3F4E4" Ref="DS?"  Part="1" 
AR Path="/61A0C441/61A3F4E4" Ref="DS?"  Part="1" 
F 0 "DS?" V 3350 6800 50  0000 C CNN
F 1 "HY1602E" V 4150 5350 50  0000 C CNN
F 2 "Display:HY1602E" H 3750 5050 50  0001 C CIN
F 3 "http://www.icbank.com/data/ICBShop/board/HY1602E.pdf" H 3950 6050 50  0001 C CNN
	1    3750 5950
	0    -1   1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A3F4EA
P 2600 6450
AR Path="/61A3F4EA" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F4EA" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2600 6200 50  0001 C CNN
F 1 "GND" H 2605 6277 50  0000 C CNN
F 2 "" H 2600 6450 50  0001 C CNN
F 3 "" H 2600 6450 50  0001 C CNN
	1    2600 6450
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 61A3F4F0
P 2900 5900
AR Path="/61A3F4F0" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F4F0" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2900 5750 50  0001 C CNN
F 1 "+5V" H 2915 6073 50  0000 C CNN
F 2 "" H 2900 5900 50  0001 C CNN
F 3 "" H 2900 5900 50  0001 C CNN
	1    2900 5900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A3F4F6
P 4600 6000
AR Path="/61A3F4F6" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F4F6" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4600 5750 50  0001 C CNN
F 1 "GND" H 4605 5827 50  0000 C CNN
F 2 "" H 4600 6000 50  0001 C CNN
F 3 "" H 4600 6000 50  0001 C CNN
	1    4600 6000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4600 6000 4600 5950
Wire Wire Line
	4600 5950 4550 5950
Wire Wire Line
	2950 5950 2900 5950
Wire Wire Line
	2900 5950 2900 5900
$Comp
L Device:R_POT RV?
U 1 1 61A3F500
P 2850 6400
AR Path="/61A3F500" Ref="RV?"  Part="1" 
AR Path="/61A0C441/61A3F500" Ref="RV?"  Part="1" 
F 0 "RV?" H 2781 6446 50  0000 R CNN
F 1 "10K" H 2781 6355 50  0000 R CNN
F 2 "" H 2850 6400 50  0001 C CNN
F 3 "~" H 2850 6400 50  0001 C CNN
	1    2850 6400
	0    1    1    0   
$EndComp
Wire Wire Line
	2850 6550 3000 6550
Wire Wire Line
	3000 6550 3000 6400
Connection ~ 3000 6400
$Comp
L power:+5V #PWR?
U 1 1 61A3F509
P 3600 6450
AR Path="/61A3F509" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F509" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3600 6300 50  0001 C CNN
F 1 "+5V" H 3550 6600 50  0000 C CNN
F 2 "" H 3600 6450 50  0001 C CNN
F 3 "" H 3600 6450 50  0001 C CNN
	1    3600 6450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61A3F50F
P 3850 6400
AR Path="/61A3F50F" Ref="#PWR?"  Part="1" 
AR Path="/61A0C441/61A3F50F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3850 6150 50  0001 C CNN
F 1 "GND" H 3700 6300 50  0000 C CNN
F 2 "" H 3850 6400 50  0001 C CNN
F 3 "" H 3850 6400 50  0001 C CNN
	1    3850 6400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3850 6400 3850 6350
Wire Wire Line
	3600 6450 3600 6500
Wire Wire Line
	3600 6500 3750 6500
Wire Wire Line
	3750 6500 3750 6350
Wire Wire Line
	2500 4900 2450 4900
Wire Wire Line
	1550 6000 1550 6050
Wire Wire Line
	1550 5650 1550 5700
NoConn ~ 3650 5550
NoConn ~ 3750 5550
NoConn ~ 3850 5550
NoConn ~ 3950 5550
Wire Wire Line
	2600 6450 2600 6400
Wire Wire Line
	2600 6400 2700 6400
Wire Wire Line
	3150 6400 3150 6350
Wire Wire Line
	3000 6400 3150 6400
Entry Wire Line
	5050 3550 5150 3650
Entry Wire Line
	5050 3700 5150 3800
Entry Wire Line
	5050 3850 5150 3950
Entry Wire Line
	5050 4000 5150 4100
Entry Wire Line
	4450 5350 4350 5450
Entry Wire Line
	4350 5350 4250 5450
Entry Wire Line
	4250 5350 4150 5450
Entry Wire Line
	4150 5350 4050 5450
Wire Bus Line
	5400 5350 5400 4300
Wire Bus Line
	5400 4300 5150 4300
Wire Wire Line
	4350 5550 4350 5450
Wire Wire Line
	4250 5550 4250 5450
Wire Wire Line
	4150 5550 4150 5450
Wire Wire Line
	4050 5550 4050 5450
Wire Wire Line
	4950 4000 5050 4000
Wire Wire Line
	5050 3850 4950 3850
Wire Wire Line
	5050 3700 4950 3700
Wire Wire Line
	5050 3550 4950 3550
Text Label 4950 3550 0    30   ~ 6
DB7
Text Label 4950 3700 0    30   ~ 6
DB6
Text Label 4950 3850 0    30   ~ 6
DB5
Text Label 4950 4000 0    30   ~ 6
DB4
Text Label 4350 5550 1    30   ~ 6
DB7
Text Label 4250 5550 1    30   ~ 6
DB6
Text Label 4150 5550 1    30   ~ 6
DB5
Text Label 4050 5550 1    30   ~ 6
DB4
Entry Wire Line
	2250 4300 2150 4400
Entry Wire Line
	2250 4450 2150 4550
Entry Wire Line
	2250 4150 2150 4250
Entry Wire Line
	3050 5350 3150 5450
Entry Wire Line
	3250 5350 3350 5450
Entry Wire Line
	3350 5350 3450 5450
Wire Wire Line
	2250 4150 2500 4150
Wire Wire Line
	2250 4300 2500 4300
Wire Wire Line
	2250 4450 2500 4450
Wire Wire Line
	3150 5550 3150 5450
Wire Wire Line
	3350 5550 3350 5450
Wire Wire Line
	3450 5450 3450 5550
Text Label 2400 4150 0    30   ~ 6
RS
Text Label 2400 4300 0    30   ~ 6
R~W
Text Label 2400 4450 0    30   ~ 6
E
Text Label 3150 5550 1    30   ~ 6
E
Text Label 3350 5550 1    30   ~ 6
R~W
Text Label 3450 5550 1    30   ~ 6
RS
Text GLabel 2450 4900 3    40   Input ~ 0
DEBUG_LED
Text GLabel 1900 5650 2    40   Input ~ 0
DEBUG_LED
Wire Wire Line
	1900 5650 1850 5650
Text Notes 4800 1600 0    70   ~ 14
Get button states over SPI.\nDisplay state.
Text Notes 3250 6800 0    50   ~ 10
HD44780 Driven LCD Display\n4-bit Mode Used
Text Notes 3550 5500 0    15   Italic 0
*Might be good idea to pulldown/\npulldown unused data lines...
NoConn ~ 2500 3100
NoConn ~ 2500 3250
NoConn ~ 4950 4750
Wire Bus Line
	2150 4250 2150 5350
Wire Bus Line
	2150 5350 3350 5350
Wire Bus Line
	4150 5350 5400 5350
Wire Bus Line
	5150 3650 5150 4300
$EndSCHEMATC
