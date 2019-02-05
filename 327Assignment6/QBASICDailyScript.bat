rem Zoopdeeboop Co.
rem Meryl Gamboa - 10173192
rem Zane Little - 10179568
rem Stefan Urosevic - 10146785
rem Assignment 6 Daily Script


@Echo off

set /A numRuns = 3

rem Making the transaction summary folder
mkdir TSFILES\

echo Looping through front end
rem loop numRuns times

for /L %%i in (1,1,%numRuns%) do (

	echo Front End run %%i
	rem for each loop run front end and get transaction file
	QBASICFrontEnd.exe AccountsFile.txt TSFILES/TS%%i.txt

	echo Stripping end of file code from transaction summary %%i
	rem call cpp file to strip EOF from transaction summary file
	stripEOS.exe TSFILES/TS%%i.txt
	)


echo Merging summary files
rem merge the transaction summaries
copy TSFILES\*.txt MergedTS.txt


rem add EOS to merged transaction file
echo EOS>>MergedTS.txt

echo Running back end
rem run back end using merged transaction file
QBASICBackEnd.exe MergedTS.txt MA.txt MA.txt AccountsFile.txt

copy MA.txt %1

pause