@echo off
title Tracker
call :print_ascii "Tracker"
echo.
echo TRACKER compiling...
g++ -o src/tracker/tracker.exe src/tracker/tracker.cpp src/tracker/include/feature.cpp src/tracker/include/request.cpp src/tracker/include/command.cpp -lws2_32
echo Compile successfully!
cd src\tracker
.\tracker.exe


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



:::Tracker.   ______  ____    ____    __  __  _    ___  ____  
:::Tracker.  |      ||    \  /    |  /  ]|  |/ ]  /  _]|    \ 
:::Tracker.  |      ||  D  )|  o  | /  / |  ' /  /  [_ |  D  )
:::Tracker.  |_|  |_||    / |     |/  /  |    \ |    _]|    / 
:::Tracker.    |  |  |    \ |  _  /   \_ |     \|   [_ |    \ 
:::Tracker.    |  |  |  .  \|  |  \     ||  .  ||     ||  .  \
:::Tracker.    |__|  |__|\_||__|__|\____||__|\_||_____||__|\_|
:::Tracker.                                                   
