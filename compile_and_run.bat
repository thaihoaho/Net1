@echo off
cd %~dp0

@REM rem Open new CMD for peer
@REM start cmd /k "echo PEER compiling && g++ -o src/peer/peer.exe src/peer/peer.cpp src/peer/include/feature.cpp src/peer/include/request.cpp src/peer/include/command.cpp -lws2_32 && cd src\peer && .\peer.exe"
@REM echo Peer started.

rem Open new CMD for peer
start cmd /k "echo PEEC B compiling && g++ -o src/peerB/peer.exe src/peerB/peer.cpp src/peerB/include/feature.cpp src/peerB/include/request.cpp src/peerB/include/command.cpp -lws2_32 && cd src\peerB && .\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "echo PEEC C compiling && g++ -o src/peerC/peer.exe src/peerC/peer.cpp src/peerC/include/feature.cpp src/peerC/include/request.cpp src/peerC/include/command.cpp -lws2_32 && cd src\peerC && .\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "echo PEEC D compiling && g++ -o src/peerD/peer.exe src/peerD/peer.cpp src/peerD/include/feature.cpp src/peerD/include/request.cpp src/peerD/include/command.cpp -lws2_32 && cd src\peerD && .\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "echo TRACKER compiling && g++ -o src/tracker/tracker.exe src/tracker/tracker.cpp src/tracker/include/feature.cpp src/tracker/include/request.cpp src/tracker/include/command.cpp -lws2_32 && cd src\tracker && .\tracker.exe"
echo Peer started.