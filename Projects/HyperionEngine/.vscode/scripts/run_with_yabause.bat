@ECHO Off
SET EMULATOR_DIR=..\..\Emulators

if exist HyperionEngine.cue (
"%EMULATOR_DIR%\yabause\yabause.exe" -a -i HyperionEngine.cue
) else (
echo Please compile first !
)
