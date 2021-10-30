# can_node_board
### Summary
A board to send J1939 messages to wheel loader controllers over CAN (2.0B Extended Frame) for testing purposes using the MCP2515 CAN Controller + MCP2562 CAN Transceiver + PIC18F4620 microcontroller.

### What Problem This Solves
I intend to integrate this board with a test bench I built at work for a particular controller, along with any necessary switches/knobs, in order to make it more intuititve to test and have everything together as a "package". The test bench tests the controller outside of the machine it would normally be in, so when it is turned on, if it doesn't receive certain messages over CAN that it normally expects from other controllers, then it throws errors and this prevents the controller from being fully tested. I have set up CANalyzer to fulfill this purpose at work, but I thought that having a separate board that I would just put on the test bench would make the testing process more efficient than having to look for an available Vector Bus module and setting up CANalyzer.

### Structure of This Repo


### Motivation & Reasoning


### What Makes This Project Unique
