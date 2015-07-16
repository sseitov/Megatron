@echo off
set TEMP=P
set TEMP=%TEMP%CAN

copy wrapper1.dll %TEMP%_USB.dll
copy wrapper1.dll %TEMP%_2USB.dll
copy wrapper1.dll %TEMP%_PCI.dll
copy wrapper1.dll %TEMP%_2PCI.dll
copy wrapper1.dll %TEMP%_PCC.dll
copy wrapper1.dll %TEMP%_2PCC.dll

copy wrapper1_nopnp.dll %TEMP%_ISA.dll
copy wrapper1_nopnp.dll %TEMP%_2ISA.dll
copy wrapper1_nopnp.dll %TEMP%_DNB.dll
copy wrapper1_nopnp.dll %TEMP%_DNG.dll

