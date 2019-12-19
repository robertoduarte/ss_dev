@ECHO Off
SET EMULATOR_DIR=..\..\Emulators

if exist build\build.iso (
"%EMULATOR_DIR%\yabause\yabause.exe" -a -i build\build.iso
) else (
echo Please compile first !
)
