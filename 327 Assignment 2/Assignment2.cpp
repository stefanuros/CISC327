/*

CISC327 Assignment 2

Stefan Urosevic   10146785
Zane Little       10179568
Meryl Gamboa      10173192

*/

//#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

// bool isNum(string num);
// bool validateName(string name);
// bool validateAccount(string account, vector<int> acctList);
// bool validateMoney(string amount, bool atm);

// Finds an elements position in a vector of strings
// Takes a vector to search
// And an element to search for
// Returns position of that element
int findPos(vector<string> vec, string element)
{
	int pos = 0;
	while (pos < vec.size())
	{
		if (element == vec[pos])
		{
			return pos;
		}
		pos++;
	}
	return -1; // ERROR - element not in vector;
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
bool validateName(string name)
{
	//Check that it doesn't start with a space
	if (name[0] == ' ')
	{
		printf("Invalid Account Name. Cannot start with a space. ");
		return false;
	}

	//Check thatit doesnt end with a space
	if (name[name.length() - 1] == ' ')
	{
		printf("Invalid Account Name. Cannot end with a space. ");
		return false;
	}

	//Check that it is not too long or too short
	if (name.length() > 30 || name.length() < 3)
	{
		printf("Invalid Account Name. Must be at least 3 and at most");
		printf(" 30 characters. ");
		return false;
	}

	//Check if there are non-valid symbols in the name
	int i;
	for (i = 0; i < name.length(); i++)
	{
		if (!isalnum(name[i]))
		{
			printf("Invalid Account Name. Cannot contain symbols. ");
			return false;
		}
	}

	return true;
}

//Validate Account Number
//Takes a string for account number
//and a vector that includes a valid list of accounts
bool validateAccount(string account, vector<string> acctList)
{
	//Check if it has symbols
	if (!isNum(account))
	{
		printf("Invalid Account Number. Cannot contain symbols. ");
		return false;
	}

	//Convert it to an int
	int num = atoi(account.c_str());

	//Check that it doesnt start with a 0
	if (account[0] == '0')
	{
		printf("Invalid Account Number. Cannot start with 0. ");
		return false;
	}

	//check that it is not too short or too long
	if (account.length() != 7)
	{
		printf("Invalid Account Number. Needs to be exactly 7 digits. ");
		return false;
	}

	//Check that it is in the Account List
	int i;
	bool contains = false;
	for (i = 0; i < acctList.size(); i++)
	{
		if (num == atoi(acctList[i].c_str())) contains = true;
	}

	if (!contains)
	{
		printf("Invalid Account Number. Not a valid account. ");
		return false;
	}

	return true;
}

//Validate Account Number
//Takes a string for account number
//and a vector that includes a valid list of accounts
bool validateAccountCA(string account, vector<string> acctList)
{
	//Check if it has symbols
	if (!isNum(account))
	{
		printf("Invalid Account Number. Cannot contain symbols. ");
		return false;
	}

	//Convert it to an int
	int num = atoi(account.c_str());

	//Check that it doesnt start with a 0
	if (account[0] == '0')
	{
		printf("Invalid Account Number. Cannot start with 0. ");
		return false;
	}

	//check that it is not too short or too long
	if (account.length() != 7)
	{
		printf("Invalid Account Number. Needs to be exactly 7 digits. ");
		return false;
	}

	//Check that it is in the Account List
	int i;
	bool contains = false;
	for (i = 0; i < acctList.size(); i++)
	{
		if (num == atoi(acctList[i].c_str())) contains = true;
	}

	if (contains)
	{
		printf("Invalid Account Number. Not a valid account. ");
		return false;
	}

	return true;
}

//Validate Money Amount
//Takes a string for money
// and a boolean for privileges(true machine, false agent)
bool validateMoney(string amount, bool isMachine)
{
	//Check there are no symbols in the amount
	if (!isNum(amount))
	{
		printf("Invalid Amount. Cannot contain symbols. ");
		return false;
	}

	//Convert the amount into an int
	int num = atoi(amount.c_str());

	//Check that the value is appropriate for the privileges
	if (!isMachine && num > 100000)
	{
		printf("Invalid Amount. Value too high for machine. ");
		return false;
	}
	else if (isMachine && num > 99999999)
	{
		printf("Invalid Amount. Value too high for agent. ");
		return false;
	}

	return true;
}

//createacct command
//Takes a valid account list
//Returns a string with the transaction
string createacct(vector<string> acctList, bool isAgent)
{
	string transaction;

	string inputNum;
	string inputName;

	//Check privileges
	if(!isAgent)
	{
		printf("Transaction Failed. You do no have the proper privileges.\n");
		transaction = "$FAILED";
		return transaction;
	}

	//Asking for account number
	printf("Please enter your account number:");
	cin >> inputNum;

	if (!validateAccountCA(inputNum, acctList))
	{
		printf("Transaction Failed.\n");
		transaction = "$FAILED";
		return transaction;
	}

	//Asking for account name
	printf("Please enter your account name:");
	cin >> inputName;

	if (!validateName(inputName))
	{
		printf("Transaction Failed.\n");
		transaction = "$FAILED";
		return transaction;
	}

	//Returning transaction
	printf("Transaction Successful.\n");
	transaction = "NEW " + inputNum + " 000 " + "0000000 " + inputName;
	return transaction;
}

//deleteacct command
//Takes a valid account list
//Returns a string with the transaction
string deleteacct(vector<string> acctList, bool isAgent)
{
	string inputNum;
	string inputName;

	string transaction;

	//Asking for account number
	printf("Please enter your account number:");
	cin >> inputNum;

	if (!validateAccount(inputNum, acctList))
	{
		printf("Transaction Failed.\n");
		transaction = "$FAILED";
		printf("%s", transaction.c_str());
		return transaction;
	}

	//Asking for account name
	printf("Please enter your account name:");
	printf("blah");
	cin >> inputName;

	printf("TEST");
	if (!validateName(inputName))
	{
		printf("Transaction Failed.\n");
		transaction = "$FAILED";
		return transaction;
	};

	//Returning transaction
	printf("Transaction Successful.\n");
	transaction = "DEL " + inputNum + " 000 " + "0000000 " + inputName;
	return transaction;

}

//deposit command
//Takes a valid account list
//Returns a string with the transaction
string deposit(vector<string> acctList, bool isAgent)
{
	string inputNum;
	string inputAmt;

	string transaction("");

	//Asking for account number
	printf("Please enter your account number:");
	cin >> inputNum;

	if (!validateAccount(inputNum, acctList))
	{
		printf("Transaction Failed.\n");
		transaction = "$FAILED";
		return transaction;
	}

	//ASking for money amount
	printf("Please enter amount to deposit IN CENTS:");
	cin >> inputAmt;

	if (!validateMoney(inputAmt, isAgent))
	{
		printf("Transaction Failed.\n");
		transaction = "$FAILED";
		return transaction;
	}

	//Returning transaction
	printf("Transaction Successful.\n");
	transaction = "DEP " + inputNum + " " + inputAmt + " 0000000 " + "***";
	return transaction;

}

//withdraw command
//Takes a valid account list
//Returns a string with the transaction
string withdraw(vector<string> acctList, bool isAgent, vector<string> transSum)
{
	string inputNum;
	string inputMoney;

	string transaction;

	//ASking for account number
	printf("Please enter your account number:");
	cin  >> inputNum;

	if (!validateAccount(inputNum, acctList))
	{
		printf("Transaction Failed.\n");
		transaction = "$FAILED";
		return transaction;
	}
	
	//ASking for money amount
	printf("Please enter the amount to withdraw (IN CENTS):");
	cin >> inputMoney;

	if (!validateMoney(inputMoney, isAgent))
	{
		printf("Transaction Failed.\n");
		transaction = "$FAILED";
		return transaction;
	}

	//Check if the total amount withdrawn is greater than 1000
	if(!isAgent)
	{
		int sum = atoi(inputMoney.c_str());
		//Search through the transaction summary to find the total withdrawn
		//from this account number
		int i;
		for(i = 0; i < transSum.size(); i++)
		{
			//Search through the withdraws
			if(transSum[i].substr(0,3) == "WDR")
			{
				//If they are the same account number
				if(transSum[i].substr(5,7) == inputNum)
				{
					//Find the length of the money int
					int lenMon = transSum[i].length()-24;
					//Get the sum
					sum += atoi(transSum[i].substr(12,lenMon).c_str());
				}
			}
		}

		//Checking if it is valid
		if(sum > 100000)
		{
			printf("Transaction Failed. Cannot transfer that much money.\n");
			transaction = "$FAILED";
			return transaction;
		}
	}

	//Returning transaction
	printf("Transaction Successful.\n");
	transaction = "WDR " + inputNum + " " + inputMoney + " 0000000" + " ***";
	return transaction;

}

//transfer command
//Takes a valid account list
//Returns a string with the transaction
string transfer(vector<string> acctList, bool isAgent)
{
	string transaction = "XFR ";
	string act1; // to account number
	string act2; // from account number
	string amt; // amount transferring
	
	// Retreive to account number 
	cout << "Please enter the account number to transfer to:  ";
	cin >> act1;
	if (!validateAccount(act1, acctList)) // validate to account number
	{
		cout << "Transaction Failed." << endl;
		return "$FAILED";
	}
	transaction += act1 + " "; // account number is valid, add it to transaction statement

	//retreive amount
	cout << "Please enter amount to transfer (in cents):  ";
	cin >> amt;
	if (!validateMoney(amt, isAgent)) //validate amount
	{
		cout << "Transaction Failed." << endl;
		return "$FAILED";
	}
	transaction += amt + " "; // amount is valid, add it to transaction statement

	// Retreive from account number 
	cout << "Please enter the account number to transfer to:  ";
	cin >> act2;
	if (!validateAccount(act2, acctList)) // validate to account number
	{
		cout << "Transaction Failed." << endl;
		return "$FAILED";
	}

	transaction += act2 + " ***"; // account number is valid, add it to transaction statement
	cout << "Transaction successful." << endl;
	return transaction;
}


//Function for getting input from a file
//Used to get the valid accounts information
vector<string> readAccounts(string fileName)
{
	//Creating the vector
	vector<string> accountNums;

	string line;

	//Opening the file
	ifstream file(fileName.c_str());
	if(file.is_open())
	{
		while(getline(file,line) && atoi(line.c_str()) != 0000000)
		{
			accountNums.push_back(line);
		}

		file.close();
	}
	else printf("Unable to open file.\n");

	return accountNums;
}

//Function for writing output to a file
//Used for creating the transaction summary file
void writeTransactions(vector<string> transSum, string fileName)
{
	//open file
	ofstream file;
	file.open(fileName.c_str());

	if(file.is_open())
	{
		//write the list to the file
		int i;
		for(i = 0; i < transSum.size(); i++)
		{
			file << (transSum[i].c_str()) << '\n';
		}
		//Writing EOS
		file << "EOS";

		//Closing the file
		file.close();
	}
	else printf("Unable to open file.\n");
}

//The intention of the program is to allow the user to use commands
//in the QBASIC banking application. The front end handles all of the
//Commands that a user can call.
//The program inputs 1 valid accounts file
//The program outputs 1 transaction summary file
//The program is intended to be run by a user or an agent
int main(int argc, char* argv[])
{
	//Getting the file names from the command line
	string transFileName(argv[2]); 
	string accountsFileName(argv[1]);

	string inStr;				 //to hold the input from the user regarding commands
	bool priv;					 //to hold the mode that the user has entered
	vector<string> transactions; //to hold the transactions that have taken place throughout the session
	vector<string> accounts;	 //to hold the master accounts file
	string transaction;			 //to hold the transaction that has just taken place
	int acctPos;



	/*		read in valid accounts file to accounts vector 	*/
	accounts = readAccounts(accountsFileName);

	//Initial Login
	cout << "Enter a command:  ";
	cin >> inStr;
	while (inStr != "login")
	{
		cout << "That is not a valid command.\nTry again: ";
		cin >> inStr;
		cout << endl;
	}

	//Choose mode:
	cout << "Enter transaction mode:  ";
	while (true)
	{
		cin >> inStr;
		if (inStr == "machine") // entering program in machine mode 
		{
			priv = false;
			break;
		}
		else if (inStr == "agent") // entering program in agent mode
		{
			priv = true;
			break;
		}
		else // user has not entered a valid mode, do not break
		{
			cout << "That is not a valid mode.\nPlease try agian:  ";
		}
	}
	// User has successfully logged in
	cout << "\n\n";
	// Loop through to 
	while (true)
	{
		cout << " _______________________" << endl
			<< "|                       |" << endl
			<< "|  >  deposit           |" << endl
			<< "|  >  withdraw          |" << endl
			<< "|  >  transfer          |" << endl
			<< "|  >  createacct        |" << endl
			<< "|  >  deleteacct        |" << endl
			<< "|  >  logout            |" << endl
			<< "|_______________________|" << endl
			<< "Please enter a command:  ";
      	cin >> inStr;
      	cout << endl;

		//Check what the user entered
		if (inStr == "deposit")
		{
			transaction = deposit(accounts, priv);
			if (transaction[0] != '$')
			{
				transactions.push_back(transaction);
			}
		}
		else if (inStr == "withdraw")
		{
			transaction = withdraw(accounts, priv, transactions);
			if (transaction[0] != '$')
			{
				transactions.push_back(transaction);
			}
		}
		else if (inStr == "transfer")
		{
			transaction = transfer(accounts, priv);
			if (transaction[0] != '$')
			{
				transactions.push_back(transaction);
			}
		}
		else if (inStr == "createacct")
		{
			transaction = createacct(accounts, priv);
			if (transaction[0] != '$')
			{
				transactions.push_back(transaction);
			}
		}
		else if (inStr == "deleteacct")
		{
			transaction = deleteacct(accounts, priv);
			if (transaction[0] != '$')
			{
				transactions.push_back(transaction);
			}
			//delete account from accounts vector
			acctPos = findPos(accounts, transaction.substr(4, 7));
			accounts.erase(accounts.begin() + acctPos);
		}
		else if (inStr == "logout")
		{
			break;
		}
		else //command was not recognized
		{
			cout << "'" << inStr << "' is not a recognized command. Please enter one of the following commands:";
		}
	}

	// Logout Functionality

	// create transaction summary file
	writeTransactions(transactions, transFileName);
	cout << "Logout successful.\n";
	system("pause");
	return 0;
}