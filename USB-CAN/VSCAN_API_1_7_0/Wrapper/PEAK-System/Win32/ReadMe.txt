VSCAN Wrapper for the Light DLLs from PEAK-System Technik GmbH
--------------------------------------------------------------

This wrapper DLL gives you the possibility to use any VSCAN product for the
replacement of PEAK-System Technik GmbH products which where implemented over
the Light API.

Installation:

Run the batch file create_files.bat.
Copy the desired wrapper DLL over the original Light API in your program
directory. Also copy the sample file vscan.ini to the same location.
You must also copy the latest VSCAN-API (vs_can_api.dll) into the same directory.

Configuration:

If you want to specify a special configuration for the mapping of a VSCAN
product, you can do this over a configuration file (vscan.ini).
When there is no configuration file available, the wrapper API uses the first 
VSCAN product which will be found in the PC.
You can also get extra debug informations when you configure the debug option for
the CAN channel in the configuration file. The debug output will be saved in a 
file called vs_can_api.log in the program directory.

Configuration File Example:
.----------------------------------------------------------------------------------------.
| [CAN_1.dll]                                                                            |
| Port = "COM5"                 ; VSCAN device over a (virtual) COM port                 |
| debug = 0                                                                              |
|                                                                                        |
| [CAN_2.dll]                                                                            |
| Port = "192.168.254.254:2001" ; mapping to VS-NET-CAN over IP and TCP port (raw mode)  |
| debug = 1                     ; debug output is switched on (vs_can_api.log)           |
'----------------------------------------------------------------------------------------'
Note: In this example CAN_1.dll and CAN_2.dll are sample text. Replace the text
in the brackets with the name of the used wrapper DLL. So if CAN_1.dll in reality
is XCAN_USB.dll, use that file name. If CAN_2.dll in reality is yCAN_PCI.dll, use
that name for the second section title.

