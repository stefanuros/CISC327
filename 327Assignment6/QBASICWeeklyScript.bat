rem Zoopdeeboop Co.
rem Meryl Gamboa - 10173192
rem Zane Little - 10179568
rem Stefan Urosevic - 10146785
rem Assignment 6 Weekly Script

@Echo off

rem days indicates the number of times
rem  the daily script will run
set /A days = 5

rem loop for days times, running 
rem  the daily script.
for /L %%i in (1,1,%days%) do (
	echo Running Day %%i
	call QBASICDailyScript.bat MADays%%i.txt
	)

echo Finished running for the week
echo Shutting down
echo Goodbye Dave
pause