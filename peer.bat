@echo off
title %1
call :print_ascii "peer"
echo.

set "peer_name=%1"
echo %peer_name% compiling...
g++ -o src/%peer_name%/peer.exe src/%peer_name%/peer.cpp src/%peer_name%/include/feature.cpp src/%peer_name%/include/request.cpp src/%peer_name%/include/command.cpp -lws2_32
echo Compile successfully!
cd src\%peer_name%
.\peer.exe


:print_ascii
for /F "delims=" %%L in ('
    findstr /L /I /B /C:":::%~1." "%~f0"
') do (
    set "LINE=%%L"
    setlocal EnableDelayedExpansion
    echo(!LINE:*.=!
    endlocal
)
exit /B

:::peer.       ____   ___    ___  ____  
:::peer.      |    \ /  _]  /  _]|    \ 
:::peer.      |  o  )  [_  /  [_ |  D  )
:::peer.      |   _/    _]|    _]|    / 
:::peer.      |  | |   [_ |   [_ |    \ 
:::peer.      |  | |     ||     ||  .  \
:::peer.      |__| |_____||_____||__|\_|
:::peer.                                