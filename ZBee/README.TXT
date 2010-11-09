Ubinet Master
Polytech'Nice INRIA

Subject: WPAN / WLAN
Year: 2010

Team members: 
-MADZUMAR, Somnath 
-JOST, Mauricio
-SEIFU, Sentayehu

To make this project run: 

Open aodv.dev project file with devcpp-4.9.9.2_setup.exe (Dev-C++ 4.9.9.2, attached). Then build and execute. 

Then it is possible to configure the message to send (data and destination) by pressing C. 
Use the command T to test how the application works. After one message sent. 
Each message must be sent in one node, and READ in the other, the application is intended to be used in an academic way. 
The command W (shoW) shows how each table is. 

If the ZigBee module is in command line mode, it will not receive messages, so it's pointless have multiple threads when the own device behaves as a sequential device. 

For each message that is received, it is necessary to perform a read procedure. Then the system will response with other messages if it is needed (for instance rebroadcasting RREQ, or routing certain package). It is for academic purposes.
