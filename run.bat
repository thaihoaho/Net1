@echo off
cd %~dp0

:: Hiển thị ASCII Art logo
call :print_ascii "logo"

rem Open new CMD for peer
start cmd /k "src\peer\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "src\peerB\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "src\peerC\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "src\peerD\peer.exe"
echo Peer started.

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