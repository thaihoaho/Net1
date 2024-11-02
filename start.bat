@echo off
cd %~dp0
cls


:: Hiển thị thông báo chào mừng
echo ========================================================================================================================
call :print_ascii "logo"
echo ========================================================================================================================
echo.

:: Hỏi người dùng có muốn khởi động hệ thống không
set /p startSystem="Do you want to start? (y/n): "
if /i "%startSystem%" neq "y" (
    echo Exiting system. Goodbye!
    exit /b
)
start cmd /k tracker.bat
start cmd /k "call peer.bat peer"
start cmd /k "call peer.bat peerB"
start cmd /k "call peer.bat peerC"
@REM start cmd /k "call peer.bat peerD"

                
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