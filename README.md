# can_node_board
### Summary
A board to send J1939 messages to wheel loader controllers over CAN (2.0B Extended Frame) for testing purposes using the MCP2515 CAN Controller + MCP2562 CAN Transceiver + PIC18F4620 microcontroller. This can probably also just be used in general for sending CAN messages, but you would need to edit the code as needed for your purposes. A visual demonstration of this board's operation is given in this link: 

### What Problem This Solves
I intend to integrate this board with a test bench I built at work for a particular controller, along with any necessary switches/knobs, in order to make it more intuititve to test and have everything together as a "package". The test bench tests the controller *outside of the machine it would normally be in*, so when it is turned on, if it doesn't receive certain messages over CAN (e.g., from the engine controller(s) and transmission controller), then it throws errors and this prevents the controller from being fully tested. I have set up CANalyzer to fulfill this purpose at work, but I thought that having a separate board that I would just put on the test bench would make the testing process more efficient than having to look for an available Vector Bus module and setting up CANalyzer.

**NOTE!** The actual specific J1939 messages I am sending wil NOT be pushed onto this remote directory, for obvious purposes! *However, example J1939 messages from the SAE J1939-71 standard document are included for demonstration purposes.*

### Structure of This Repo and How To Build
The "CAN WL SIM.X" folder has the '.X' extension because this project was done on the MPLAB X IDE. If you download this repo, you can open this project on MPLAB X by selecting this "CAN WL SIM.X" folder. In addition, note that the #include statements made within the various files will work only if you add the "inc" subdirectory under Project Properties > XC8 Compiler > XC8 Compiler > Include Directories > (add the "inc" relative path -- you can use the Browse button). With all that, you should be able to build the project just fine with default XC8 options within MPLAB X.  

MPLAB X IDE version: v5.45  
XC8 C Compiler version: v2.31

### Hardware Used
The choice of microcontroller - PIC18F4620 - was because I wanted to use the same microcontroller that I was using for my embedded systems class, which also satisfied my requirements for number of I/O. This microcontroller does not have an integrated CAN controller, so I chose the MCP2515 CAN Controller IC from Microchip. Note, they did have a newer version of the IC - the MCP2518 - but that wasn't available as of October 2021 due to the worldwide chip shortage... Likewise, the MCP2562 CAN Transceiver was selected based on availability and desired package.

### Motivation & Reasoning
I wanted to get more in detail with the various layers that make up a CAN node, and thought this was a great opportunity for that.

### What Makes This Project Unique
I have not exhaustively searched for similar projects, but I am sure there are plenty. The references section shows two Microchip application notes of boards built using a similar set up to me and programmed in assembly. I have seen some colleagues use a Raspberry Pi + CAN Hat and run some python scripts to do something similar. However, I think my project offers a non-operating system solution using an 8-bit Microchip PIC microcontroller using the C programming language, and also is meant to send J1939 messages, not just generic CAN messages.

### References
[Microchip Application Note AN215](https://ww1.microchip.com/downloads/en/AppNotes/00215c.pdf)  
[Microchip Application Note AN212](https://ww1.microchip.com/downloads/en/AppNotes/00212c.pdf)  
[CAN Controller MCP2515 Product Page](https://www.microchip.com/en-us/product/MCP2515)  
[CAN Transceiver MCP2562 Product Page](https://www.microchip.com/en-us/product/MCP2562)  
[Microcontroller PIC18F4620 Product Page](https://www.microchip.com/en-us/product/PIC18F4620)  
*Several application notes from Microchip and TI for explaining CAN and J1939*  
*I also had access to the SAE J1939 standard documents*
