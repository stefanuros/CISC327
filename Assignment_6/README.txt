The daily script is called QBASICDailyScript.bat. It will run one day with 3 sessions per day.
The weekly script is called QBASICWeeklyScript.bat. It will run the daily script 5 times and for each day create a copy of the master accounts file.

The master accounts file is called MA.txt. The TSFILES folder holds the individual transaction files for each day. The errorLog logs any errors with the back end. MergedTS.txt is the merged transaction file. AccountsFile.txt is the list of accounts for the front end.
stripEOS.exe is the program that we use in our daily script to strip end of file codes from the transaction summary files for easier merging.