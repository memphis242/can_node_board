EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title "CAN Node Board - CAN Interface"
Date "2021-11-04"
Rev "Rev. 0.1"
Comp "Personal Project"
Comment1 ""
Comment2 "creativecommons.org/licenses/by/4.0/"
Comment3 "License: CC BY 4.0"
Comment4 "Author: Abdullah Almosalami"
$EndDescr
$Comp
L can_node_board_hw:PIC18F4620-IP U?
U 1 1 61843E26
P 2550 3450
F 0 "U?" H 2525 5315 50  0000 C CNN
F 1 "PIC18F4620-IP" H 2525 5224 50  0000 C CNN
F 2 "" H 2400 4000 50  0001 C CNN
F 3 "" H 2400 4000 50  0001 C CNN
	1    2550 3450
	1    0    0    -1  
$EndComp
$Sheet
S 8700 800  2150 1300
U 61844E89
F0 "can_node_board_user_interface" 50
F1 "can_node_board_user_interface.sch" 50
$EndSheet
$Sheet
S 8700 2500 2150 1300
U 61844F22
F0 "can_node_board_pwr" 50
F1 "can_node_board_pwr.sch" 50
$EndSheet
$Comp
L Interface_CAN_LIN:MCP2562-E-P U?
U 1 1 61845795
P 8000 5050
F 0 "U?" H 8000 5631 50  0000 C CNN
F 1 "MCP2562-E-P" H 8000 5540 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 8000 4550 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/25167A.pdf" H 8000 5050 50  0001 C CNN
	1    8000 5050
	1    0    0    -1  
$EndComp
$Comp
L Interface_CAN_LIN:MCP2515-xSO U?
U 1 1 618466CF
P 6300 5050
F 0 "U?" H 6300 6031 50  0000 C CNN
F 1 "MCP2515-xSO" H 6300 5940 50  0000 C CNN
F 2 "Package_SO:SOIC-18W_7.5x11.6mm_P1.27mm" H 6300 4150 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21801e.pdf" H 6400 4250 50  0001 C CNN
	1    6300 5050
	1    0    0    -1  
$EndComp
$EndSCHEMATC
