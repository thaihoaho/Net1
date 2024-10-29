@echo off
cd %~dp0
rem Open new CMD for peer
start cmd /k "src\peer\peer.exe"
echo Peer started.

rem Open new CMD for tracker
start cmd /k "src\tracker\tracker.exe"
echo Tracker started.
