@ECHO Off
SET IDE_DIR=%~dp0..\..\IDE
start "" %IDE_DIR%\Netbeans\bin\netbeans64.exe --jdkhome %IDE_DIR%\jdk-12.0.2 .