@echo off
cd %~dp0

:: Hiển thị ASCII Art logo
call :print_ascii "logo"

rem Compile peer
echo peer is compiling
g++ -o src/peer/peer.exe src/peer/peer.cpp src/peer/include/feature.cpp src/peer/include/request.cpp src/peer/include/command.cpp -lws2_32
if %errorlevel% neq 0 (
    echo Compilation of peer failed!
    pause
    exit /b
)
echo Peer compiled successfully.

rem Compile peer
echo peerB is compiling
g++ -o src/peerB/peer.exe src/peerB/peer.cpp src/peerB/include/feature.cpp src/peerB/include/request.cpp src/peerB/include/command.cpp -lws2_32
if %errorlevel% neq 0 (
    echo Compilation of peer failed!
    pause
    exit /b
)
echo PeerB compiled successfully.

rem Compile peer
echo peerC is compiling
g++ -o src/peerC/peer.exe src/peerC/peer.cpp src/peerC/include/feature.cpp src/peerC/include/request.cpp src/peerC/include/command.cpp -lws2_32
if %errorlevel% neq 0 (
    echo Compilation of peer failed!
    pause
    exit /b
)
echo PeerC compiled successfully.

rem Compile peer
echo peerD is compiling
g++ -o src/peerD/peer.exe src/peerD/peer.cpp src/peerD/include/feature.cpp src/peerD/include/request.cpp src/peerD/include/command.cpp -lws2_32
if %errorlevel% neq 0 (
    echo Compilation of peer failed!
    pause
    exit /b
)
echo PeerD compiled successfully.

rem Compile tracker
echo tracker is compiling
g++ -o src/tracker/tracker.exe src/tracker/tracker.cpp src/tracker/include/command.cpp src/tracker/include/feature.cpp src/tracker/include/request.cpp -lws2_32
if %errorlevel% neq 0 (
    echo Compilation of tracker failed!
    pause
    exit /b
)
echo Tracker compiled successfully.

rem Open new CMD for peer
start cmd /k "src\peer\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "src\peerB\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "src\peerC\peer.exe"
echo Peer started.

rem Open new CMD for tracker
start cmd /k "src\peerD\tracker.exe"
echo Tracker started.

rem Open new CMD for tracker
start cmd /k "src\tracker\tracker.exe"
echo Tracker started.



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

:::logo.    _____ ____  __  __ _____  _    _ _______ ______ _____    _   _ ______ _________          ______  _____  _  __
:::logo.   / ____/ __ \|  \/  |  __ \| |  | |__   __|  ____|  __ \  | \ | |  ____|__   __\ \        / / __ \|  __ \| |/ /
:::logo.  | |   | |  | | \  / | |__) | |  | |  | |  | |__  | |__) | |  \| | |__     | |   \ \  /\  / | |  | | |__) | ' / 
:::logo.  | |   | |  | | |\/| |  ___/| |  | |  | |  |  __| |  _  /  | . ` |  __|    | |    \ \/  \/ /| |  | |  _  /|  <  
:::logo.  | |___| |__| | |  | | |    | |__| |  | |  | |____| | \ \  | |\  | |____   | |     \  /\  / | |__| | | \ \| . \ 
:::logo.   \_____\____/|_|  |_|_|     \____/   |_|  |______|_|  \_\ |_| \_|______|  |_|      \/  \/   \____/|_|  \_|_|\_\
:::logo.
:::logo.