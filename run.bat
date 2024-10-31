@echo off
cd %~dp0

rem Open new CMD for peer
start cmd /k "echo PEER is running && cd src\peer && .\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "echo PEEC B is running && cd src\peerB && .\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "echo PEEC C is running && cd src\peerC && .\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "echo PEEC D is running && cd src\peerD && .\peer.exe"
echo Peer started.

rem Open new CMD for peer
start cmd /k "echo TRACKER is running && cd src\tracker && .\tracker.exe"
echo Peer started.