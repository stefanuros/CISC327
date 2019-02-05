//The back end for the QASIC app
//It takes transaction summaries and returns an updated master accounts file
//
// Assignment 4
//
// Stefan Urosevic   10146785
// Zane Little       10179568
// Meryl Gamboa      10173192

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//A function for inserting an account into the master accounts file in order
vector<string> insertInOrder(vector<string> master, string line)
{
	int i = 0;

	while (i < master.size())
	{
		//If the line is less than the master
		if (line.substr(0, 7) < master[i].substr(0, 7))
		{
			//Insert it where it belongs
			master.insert(master.begin() + i, line);
			return master;
		}
		i++;
	}

	//If the function gets to here, the account number goes at the end
	master.push_back(line);

	return master;
}

//A function that takes an error message to write to a file
void writeError(string msg, bool fatal)
{

	string fileName = "errorLog.txt";
	//open file
	ofstream file;
	file.open(fileName.c_str(), ios::app);

	if (file.is_open())
	{
		file << msg << '\n';

		//Closing the file
		file.close();
	}
	else printf("Unable to open file.\n");

	if (fatal)
	{
		exit(EXIT_FAILURE);
	}
}


// Finds the position of an account in a vector of strings
//  using the account number
// Takes a vector to search
// And an element (account number) to search for
// Returns position of that account
int findPos(vector<string> vec, string element)
{
	int pos = 0;
	while (pos < vec.size())
	{
		if (stoi(element) == stoi(vec[pos].substr(0, 7)))
		{
			return pos;
		}
		pos++;
	}
	return -1; // ERROR - element not in vector;
}

// Find the account name from an entry of the master accounts file
// Takes a string
// And where to start
// Returns a string containing the name
string findAcctName(string str, int i)
{
	string name;
	while (str[i] != ' ') // find the start of the name
	{
		i++;
	}
	i++;
	while (i < str.length()) // retrieve the name
	{
		name += str[i];
		i++;
	}
	return name;
}

//Takes a vector and turns it into a file
//Takes a vector of strings 
//Takes a string which is the file name to write to
//Takes the end of file code
//Writes the list to a file
int writeFile(vector<string> vec, string fileName, string EOFCode)
{
	//open file
	ofstream file;
	file.open(fileName.c_str());

	if (file.is_open())
	{
		//write the list to the file
		for (int i = 0; i < vec.size(); i++)
		{
			file << (vec[i].c_str()) << '\n';
		}
		//Writing EOF code
		file << EOFCode.c_str();

		//Closing the file
		file.close();
	}
	else printf("Unable to open file.\n");

	return 0;
}

// upDEP takes accounts vector and a transaction string
// It returns accounts vector updated with the deposit action performed
vector<string> upDEP(vector<string> accounts, string transaction, int acctPos)
{
	string transAmt = "";
	int newAmt = 0;
	string oldAmt = "";
	int x = 12; // index for the start of the $

				//get transaction amount
	while (transaction[x] != ' ')
	{
		transAmt += transaction[x];
		x += 1;
	}

	//get previous amount
	x = 8; // the first index of the amount in account string
	while (accounts[acctPos][x] != ' ')
	{
		oldAmt += accounts[acctPos][x];
		x += 1;
	}

	//calculate new ammount
	newAmt = stoi(oldAmt) + stoi(transAmt);
	accounts[acctPos] = accounts[acctPos].substr(0, 7) + " " + to_string(newAmt) + " " + findAcctName(accounts[acctPos], 8);

	return accounts;
}


// upWDR takes accounts vector and a transaction string
// It returns accounts vector updated for withdraw
vector<string> upWDR_XFR(vector<string> accounts, string transaction, int acctPos)
{
	int x = 12;
	string transAmt = "";
	string oldAmt = "";
	int newAmt = 0;
	int acctPos2;


	// Find account position for
	if (transaction[0] == 'X')
	{
		acctPos2 = findPos(accounts, transaction.substr(4, 7));
		if (acctPos2 == -1)
		{
			writeError("ERROR WITH XFR : ACCOUNT NUMBER " + transaction.substr(4,7) + " NOT FOUND IN ACCOUNTS FILE.ACCOUNTS MANIFEST NOT UPDATED.", false);
			return accounts;
		}
	}

	//get transaction amount
	while (transaction[x] != ' ')
	{
		transAmt += transaction[x];
		x++;
	}

	//get previous amount
	x = 8;
	while (accounts[acctPos][x] != ' ')
	{
		oldAmt += accounts[acctPos][x];
		x++;
	}

	string oldAcct = accounts[acctPos];
	//calculate new ammount
	newAmt = stoi(oldAmt) - stoi(transAmt);
	accounts[acctPos] = oldAcct.substr(0, 7) + " " + to_string(newAmt) + " " + findAcctName(oldAcct, 8);
	// deal with transfer further by updating the 'to' account
	if (transaction[0] == 'X')
	{
		accounts = upDEP(accounts, transaction, acctPos2);
	}
	return accounts;
}


// upNEW takes accounts vector and a transaction string
// It returns accounts vector updated for withdraw
vector<string> upNEW(vector<string> accounts, string transaction, int acctPos)
{
	string newAcct = transaction.substr(4, 7);
	newAcct += " 000 ";
	newAcct += transaction.substr(24, transaction.length() - 24);
	accounts = insertInOrder(accounts, newAcct);
	return accounts;
}


vector<string> upDEL(vector<string> accounts, string transaction, int acctPos)
{
	// Check to see if the balance is 0 (the 8th charater is the first character in the 
	// balance, if it is 0, the balance is 0.

	if (accounts[acctPos][8] != '0')
	{
		writeError("ERROR WITH DEL: ACCOUNT NUMBER " + transaction.substr(4, 7) + " DOES NOT HAVE 0 BALANCE. ACCOUNTS MANIFEST NOT UPDATED.", false);
	}
	else
	{
		// account balance is 0, delete account
		accounts.erase(accounts.begin() + acctPos);
	}
	return accounts;
}


// Takes a string vector that represents the accounts file 
//  and and transaction string.
// This function decides what action to performn based on the 
//  transaction string
// Returns accounts vector.
vector<string> updateAccounts(vector<string> accounts, string transaction)
{
	int acctPos = 0;
	int x;
	if (transaction.substr(0, 3) == "DEP")
	{
		acctPos = findPos(accounts, transaction.substr(4, 7)); // Get account position
		if (acctPos == -1)
		{
			writeError("ERROR WITH DEP: ACCOUNT NUMBER " + transaction.substr(4,7) + " NOT FOUND IN ACCOUNTS FILE. ACCOUNTS MANIFEST NOT UPDATED.", false);
			return accounts;
		}
		accounts = upDEP(accounts, transaction, acctPos); //Handle Deposite and Transfer functionality	
	}
	else if ((transaction[0] == 'W') || (transaction[0] == 'X'))
	{
		if (transaction[0] == 'W')
		{
			acctPos = findPos(accounts, transaction.substr(4, 7)); // Get account position
			if (acctPos == -1)
			{
				writeError("ERROR WITH WDR: ACCOUNT NUMBER " + transaction.substr(4, 7) + " NOT FOUND IN ACCOUNTS FILE. ACCOUNTS MANIFEST NOT UPDATED.", false);
				return accounts;
			}
		}
		else // transfer
		{
			// find the start of the 'to' account
			x = 12;
			while (transaction[x] != ' ')
			{
				x++;
			}
			x++;
			acctPos = findPos(accounts, transaction.substr(x, 7));
			if (acctPos == -1)
			{
				writeError("ERROR WITH XFR: ACCOUNT NUMBER " + transaction.substr(x, 7) + " NOT FOUND IN ACCOUNTS FILE. ACCOUNTS MANIFEST NOT UPDATED.", false);
				return accounts;
			}
		}
		accounts = upWDR_XFR(accounts, transaction, acctPos);	// Handle Withdraw functionality
	}
	else if (transaction[0] == 'N')
	{
		accounts = upNEW(accounts, transaction, acctPos);	// Handle creating a new account
	}
	else if (transaction.substr(0, 3) == "DEL")
	{
		acctPos = findPos(accounts, transaction.substr(4, 7)); // Get account position
		if (acctPos == -1)
		{
			writeError("ERROR WITH DEL: ACCOUNT NUMBER " + transaction.substr(4, 7) + " NOT FOUND IN ACCOUNTS FILE. ACCOUNTS MANIFEST NOT UPDATED.", false);
			return accounts;
		}
		accounts = upDEL(accounts, transaction, acctPos);	// Handle deleting an account
	}
	else
	{
		writeError("ERROR: TRANSACTION CODE NOT RECOGNIZED. ACCOUNTS MANIFEST NOT UPDATED.", false);
	}

	return accounts;
}


//Takes a fileName and creates a vector of strings with the contents
vector<string> createVector(string fileName, string EOFCode)
{
	vector<string> vec;

	string line;

	//Opening the file
	ifstream file(fileName.c_str());
	if (file.is_open())
	{
		while (getline(file, line) && line.c_str() != EOFCode)
		{
			vec.push_back(line);
		}

		file.close();
	}
	else writeError("Unable to open file: " + fileName, true);

	return vec;
}

//Takes a number as a string and checks if it is actually a number
bool isNum(string num)
{
	int i;
	for (i = 0; i < num.length(); i++)
	{
		if (!isdigit(num[i])) return false;
	}
	return true;
}

//Validate Account Name
//Takes a string for a name
bool validateName(string name, string acct)
{
	string errorMsg = "FATAL ERROR with account " + acct + " name: ";
	errorMsg += name + " ";

	//Check that it doesn't start with a space
	if (name[0] == ' ')
	{
		errorMsg += "starts with space";
		writeError(errorMsg, true);
		return false;
	}

	//Check thatit doesnt end with a space
	if (name[name.length() - 1] == ' ')
	{
		errorMsg += "ends with space";
		writeError(errorMsg, true);
		return false;
	}

	//Check that it is not too long or too short
	if (name.length() > 30 || name.length() < 3)
	{
		errorMsg += "invalid Length";
		writeError(errorMsg, true);
		return false;
	}

	//Check if there are non-valid symbols in the name
	int i;
	for (i = 0; i < name.length(); i++)
	{
		if (!isalnum(name[i]) && name[i] != ' ')
		{
			errorMsg += "contains symbols";
			writeError(errorMsg, true);
			return false;
		}
	}

	return true;
}

//Validate Account Number
//Takes a string for account number and checks if its valid
bool validateAccount(string account)
{
	string errorMsg = "FATAL ERROR with account " + account + " number: ";
	errorMsg += account + " ";

	//Check if it has symbols
	if (!isNum(account))
	{
		errorMsg = +"contains symbols";
		writeError(errorMsg, true);
		return false;
	}

	//Check that it doesnt start with a 0
	if (account[0] == '0')
	{
		errorMsg = +"starts with 0";
		writeError(errorMsg, true);
		return false;
	}

	//check that it is not too short or too long
	if (account.length() != 7)
	{
		errorMsg = +"not 7 digits";
		writeError(errorMsg, true);
		return false;
	}

	return true;
}

bool checkMaster(vector<string> master)
{
	string acctNum = ""; //The number of 1 account
	string acctName = ""; // The name of 1 account
	string acctBal = ""; // The balance of 1 account

	string errorMsg;

	//Check the master accounts file and log a fatal error if something
	//is wrong
	for (int i = 0; i < master.size(); i++)
	{

		//check if its sorted
		//If its not sorted, fatal error
		if (i < master.size() - 1)
		{
			if (master[i].substr(0, 7) > master[i + 1].substr(0, 7))
			{
				//fatal error
				errorMsg = "FATAL ERROR: Master accounts file not sorted";
				writeError(errorMsg, true);
				return false;
			}
		}

		//Get account number
		acctNum = master[i].substr(0, 7);
		//Index for the start of balance
		int x = 8;
		//Get account blance
		while (master[i][x] != ' ')
		{
			acctBal += master[i][x];
			x++;
		}

		x++;

		//Get account name
		while (x < master[i].length())
		{
			acctName += master[i][x];
			x++;
		}

		//VaLidate the 3 strings I just got
		validateAccount(acctNum);
		validateName(acctName, acctNum);
		if (!isNum(acctBal))
		{
			errorMsg = "FATAL ERROR: Balance is not a number";
			writeError(errorMsg, true);
			return false;
		}
	}

	//If there is a problem, the program is ended earlier
	return true;
}

int main(int argc, char* argv[])
{
	//Creating a new error log file
	string fileName = "errorLog.txt";
	ofstream file;
	file.open(fileName.c_str());
	file.close();

	//Get the name of the Merged Transaction Summary File
	string mergedTransSumName = argv[1];
	vector<string> mergedTransSum = createVector(mergedTransSumName, "EOS");
	//Get the name of Old Master Accounts File
	string oldMasterAcctName = argv[2];
	vector<string> oldMasterAcct = createVector(oldMasterAcctName, "EOF");
	//Get the name of New Master Accounts File
	string newMasterAcctName = argv[3];
	//Get the name of Valid Accounts File
	string validAcctName = argv[4];

	//Check that the lines in the master accounts file are valid
	checkMaster(oldMasterAcct);

	vector<string> newMasterAcct = oldMasterAcct;
	//Call Update Accounts
	for (int i = 0; i < mergedTransSum.size(); i++)
	{
		newMasterAcct = updateAccounts(newMasterAcct, mergedTransSum[i]);
	}

	//Make the valid accounts list
	vector<string> validAcct;

	for (int i = 0; i < newMasterAcct.size(); i++)
	{
		validAcct.push_back(newMasterAcct[i].substr(0, 7));
	}

	//Create files for newMasterAcct and validAcct
	writeFile(newMasterAcct, newMasterAcctName, "EOF");
	writeFile(validAcct, validAcctName, "0000000");

	//End Program
	system("PAUSE");
	return 0;
}
