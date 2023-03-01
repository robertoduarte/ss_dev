@ECHO Off
SET COMMON_DIR=..\..\Common

if not exist *.cue (
    echo "ISO missing, please build first."
) else (
    @REM Finding first cue file and running it on mednafen
    FOR %%F IN (*.cue) DO (
        "%COMMON_DIR%\mednafen\mednafen.exe" %%F
        exit /b
    )
)
