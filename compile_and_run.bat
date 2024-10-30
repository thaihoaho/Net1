@echo off
cd %~dp0
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
echo Peer compiled successfully.

rem Compile peer
echo peerC is compiling
g++ -o src/peerC/peer.exe src/peerC/peer.cpp src/peerC/include/feature.cpp src/peerC/include/request.cpp src/peerC/include/command.cpp -lws2_32
if %errorlevel% neq 0 (
    echo Compilation of peer failed!
    pause
    exit /b
)
echo Peer compiled successfully.

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
start cmd /k "src\tracker\tracker.exe"
echo Tracker started.
