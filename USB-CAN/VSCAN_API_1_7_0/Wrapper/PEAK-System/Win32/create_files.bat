@echo off
set TARG=P
set TARG=%TARG%CAN

copy wrapper1.dll %TARG%_USB.dll
copy wrapper1.dll %TARG%_2USB.dll
copy wrapper1.dll %TARG%_PCI.dll
copy wrapper1.dll %TARG%_2PCI.dll
copy wrapper1.dll %TARG%_PCC.dll
copy wrapper1.dll %TARG%_2PCC.dll

copy wrapper1_nopnp.dll %TARG%_ISA.dll
copy wrapper1_nopnp.dll %TARG%_2ISA.dll
copy wrapper1_nopnp.dll %TARG%_DNB.dll
copy wrapper1_nopnp.dll %TARG%_DNG.dll


ECHO [%TARG%_USB.dll]>vscan.ini
ECHO Port = "COM5"   ; Com port (COMx) or Ip address (192.168.254.254:2001) >vscan.ini
ECHO                 ; from the VSCAN device which should be used for the   >vscan.ini
ECHO                 ; mapping to the ECHO Peak library                     >vscan.ini
ECHO debug = 0       ; prints debug infos into a file (vs_can_api.log)      >vscan.ini

