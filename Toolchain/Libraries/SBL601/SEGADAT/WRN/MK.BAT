:	mk.bat
:	Copyright(c) SEGAENTERPRISES, LTD. 1995
:	Written by eMuKei on 1995-05-22 Ver.1.00
@echo off
if "%1"=="p" goto jobpre
if "%1"=="b" goto jobbuild
if "%1"=="e" goto jobemu
if "%1"=="w" goto jobcdw
if "%1"=="clean" goto jobclean
:usage
    echo Usage: %0 [[p/b/e/w]/[clean [all]]]
    goto quit
:jobpre
    timestmp "[vcdpre start]--------------------------\" >>tags
    vcdpre wrn
    timestmp "[vcdpre complete]-----------------------\" >>tags
    echo 
    pause
    goto jobemu
:jobbuild
    timestmp "[vcdbuild start]------------------------\" >>tags
    vcdpre wrn -b -n
    timestmp "[vcdbuild complete]---------------------\" >>tags
    echo 
    pause
    goto jobemu
:jobemu
    timestmp "[vcdemu start]--------------------------\" >>tags
    vcdemu wrn
    timestmp "[vcdemu complete]-----------------------\" >>tags
    goto quit
:jobcdw
    timestmp "[vcdmktoc start]------------------------\" >>tags
    vcdmktoc wrn
    timestmp "[vcdmktoc complete]---------------------\" >>tags
    pause
    tocchk wrn >wrn.tck
    type wrn.tck | more
    pause
    timestmp "[segacdw start]-------------------------\" >>tags
    segacdw wrn
    timestmp "[segacdw complete]----------------------\" >>tags
    goto quit
:jobclean
    if exist *.bak del *.bak
    if exist *.qsb del *.qsb
    if exist *.pvd del *.pvd
    if exist *.pat del *.pat
if not "%2"=="all" goto quit
    if exist *.dsk del *.dsk
    if exist *.rti del *.rti
    if exist *.toc del *.toc
    if exist *.tck del *.tck
    if exist tags  del tags
    goto quit
:quit
