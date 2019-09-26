@echo off

IF NOT EXIST %AWKENV%\*.awk GOTO noscript

IF "%1"=="" GOTO help

IF EXIST %TEMP%\gasm?.tmp del %TEMP%\gasm?.tmp

%AWKENV%\gawk_emx -f %AWKENV%\comment.awk %1 > %TEMP%\gasm0.tmp
IF ERRORLEVEL 1 GOTO error
%AWKENV%\gawk_emx -f %AWKENV%\keyword.awk %TEMP%\gasm0.tmp > %TEMP%\gasm1.tmp
IF ERRORLEVEL 1 GOTO error
%AWKENV%\gawk_emx -f %AWKENV%\section.awk %TEMP%\gasm1.tmp > %TEMP%\gasm2.tmp
IF ERRORLEVEL 1 GOTO error
gasp -u %TEMP%\gasm2.tmp > %TEMP%\gasm3.tmp
IF ERRORLEVEL 1 GOTO error
%AWKENV%\gawk_emx -f %AWKENV%\gasp.awk %TEMP%\gasm3.tmp > %TEMP%\gasm4.tmp
IF ERRORLEVEL 1 GOTO error
gasp -u %TEMP%\gasm4.tmp > %TEMP%\gasm5.tmp
IF ERRORLEVEL 1 GOTO error
%AWKENV%\gawk_emx -f %AWKENV%\pool.awk %TEMP%\gasm5.tmp > %TEMP%\gasm6.tmp
IF ERRORLEVEL 1 GOTO error
as %TEMP%\gasm6.tmp %2 %3 %4 %5 %6 %7 %8 %9
IF ERRORLEVEL 1 GOTO error
GOTO end

:noscript
ECHO �X�N���v�g�t�@�C���̂���f�B���N�g�������ϐ� AWKENV �Ɏw�肵�Ă�������.
ECHO\
GOTO end

:help
ECHO"C:\> gasm source-file [as-options][30m"[m
ECHO\
ECHO [as-options]
ECHO     as �ɑ΂���I�v�V����
ECHO\
ECHO �g�p������ϐ�
ECHO   AWKENV  awk �̃X�N���v�g�t�@�C���̂���f�B���N�g�����w�肷��
ECHO           �w��̂Ȃ��Ƃ��̓��[�g�f�B���N�g�����w�肳�ꂽ���̂Ƃ݂Ȃ��܂�
ECHO   TEMP    �e���|�����t�@�C�����쐬����f�B���N�g�����w�肷��
ECHO           �w��̂Ȃ��Ƃ��̓��[�g�f�B���N�g�����w�肳�ꂽ���̂Ƃ݂Ȃ��܂�
ECHO\
GOTO end

:error
echo error!!
:end
