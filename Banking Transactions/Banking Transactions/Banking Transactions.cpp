// This application is designed to process banking transactions
// James Pinner
//***********************************************************************************

#include<fstream>
#include<iostream>
#include<string>
#include<iomanip>
using namespace std;


struct account {
	int accountNumber;
	string Fname;
	string Lname;
	float balance;
	float credits;
	float debits;
	// These were added to keep track of all transactions associated with the account.
	int numOftransactions;
	int transElement[10];
};

struct transaction {
	int transactionId;
	int accountnumber;
	float amount;
	char type;
	char processed;
};

// returns the number of entries in a file minus the headers
int numberOfentries(ifstream& fin);

// these read files into an array one for accounts and one for transactions
void readAccounts(ifstream& fin, int entries, account accounts[]);
void readTransactions(ifstream& fin, int entries, transaction transactions[]);

// This function is called from the function main to begin processing transactions
void processTransactions(account accounts[], int numberOfAccounts, transaction transactions[], int numberOfTransactions);

// This function is called from the processTransactions() function 
void process(transaction transactions[], account accounts[], int accountIdentifier, int transactionIdentifer);

// This function is called from the processTransactions() it returns the element of the arry with a matching account number
int findAccount(int accountnumber, account accounts[]);

// This function outputs the transactions and account information to a file
void output(ofstream& fout, transaction transactions[], account accounts[], int numberOfAccounts, int numberOfTransactions);

// Values for the accounts and Transactions arrays
const int NUM_ACCOUNTS = 10;
const int NUM_TRANSACTIONS = 100;


int main()
{


	// These will hold the actual number of values contained within the accounts and transactions arrays
	int numberOfAccounts = 0;
	int numberOfTransactions = 0;
	// File reading identifiers for the accounts and transactions files
	ifstream accountsTxt;
	ifstream transactionsTxt;
	// File to output results
	ofstream summaryOfAccounts;
	// Arrays to hold account and transaction informaiton
	account accounts[NUM_ACCOUNTS] = { 0 };
	transaction transactions[NUM_TRANSACTIONS]{ 0 };

	// determine the number of entries and input into the arrys
	accountsTxt.open("accounts.txt");
	numberOfAccounts = numberOfentries(accountsTxt);
	accountsTxt.close();
	accountsTxt.open("accounts.txt");
	readAccounts(accountsTxt, numberOfAccounts, accounts);
	accountsTxt.close();

	transactionsTxt.open("transactions.txt");
	numberOfTransactions = numberOfentries(transactionsTxt);
	transactionsTxt.close();
	transactionsTxt.open("transactions.txt");
	readTransactions(transactionsTxt, numberOfTransactions, transactions);
	transactionsTxt.close();

	// Process transactions

	processTransactions(accounts, numberOfAccounts, transactions, numberOfTransactions);

	// Output results
	summaryOfAccounts.open("summaryOfAccounts.txt");
	output(summaryOfAccounts, transactions, accounts, numberOfAccounts, numberOfTransactions);
	summaryOfAccounts.close();

	cout << "Transactions have been processed." << endl
		<< "A file summaryOfAccounts.txt has been created" << endl
		<< "Press Enter to exit the program";
	cin.ignore(100, '\n');

	return 0;
}

// ***********************************************Number of entries************************************************
int numberOfentries(ifstream& fin)
{
	int retVal = -1;
	fin.ignore(100, '\n');
	while (fin) {
		fin.ignore(100, '\n');
		retVal++;
	}
	return retVal;
}
//*********************************************Read Accounts*********************************************************
void readAccounts(ifstream& fin, int entries, account accounts[])
{
	fin.ignore(100, '\n');
	for (int i = 0; i < entries; i++) {
		fin >> accounts[i].accountNumber;
		fin >> accounts[i].Fname;
		fin >> accounts[i].Lname;
		fin.ignore(100, '\n');

	}

}
//**********************************************Read Transactions *******************************************************
void readTransactions(ifstream& fin, int entries, transaction transactions[])
{
	fin.ignore(100, '\n');
	for (int i = 0; i < entries; i++) {
		fin >> transactions[i].accountnumber;
		fin >> transactions[i].type;
		fin >> transactions[i].amount;
		fin.ignore(100, '\n');
	}

}
//**************************************************Process Transactions**************************************************
void processTransactions(account accounts[], int numberOfaccounts, transaction transactions[], int numberOfTransactions)
{
	//Variables to identify the transaction and associated account
	int accountIdentifier = -1;
	int transactionIdentifer = -1;

	for (int i = 0; i < numberOfTransactions; i++)
	{
		transactionIdentifer = i;
		// Creat a transaction Id for all transactions beginning with 100
		transactions[i].transactionId = i + 100;
		// Call functions to Match transaction to an account
		accountIdentifier = findAccount(transactions[i].accountnumber, accounts);
		// Process the transaction
		process(transactions, accounts, accountIdentifier, transactionIdentifer);


	}
}
//***************************************************Match transactions to Accounts****************************************

int findAccount(int accountNumber, account accounts[])
{
	int retVal = -1;
	do
	{
		retVal++;
		if (accountNumber == accounts[retVal].accountNumber)
		{
			//When a match is found the element of the accounts array is returned
			return retVal;
		}

	} while (accountNumber != accounts[retVal].accountNumber);
	// return -1 if no match is found
	return -1;
}

//****************************************************Credit or Debit accounts*********************************************
void process(transaction transactions[], account accounts[], int accountIdentifier, int transactionIdentifer)
{
	//Process Credits
	if (transactions[transactionIdentifer].type == 'C')
	{
		accounts[accountIdentifier].balance += transactions[transactionIdentifer].amount;
		accounts[accountIdentifier].credits += transactions[transactionIdentifer].amount;
		transactions[transactionIdentifer].processed = 'Y';
		accounts[accountIdentifier].transElement[accounts[accountIdentifier].numOftransactions] = transactionIdentifer;
		accounts[accountIdentifier].numOftransactions++;


	} //Process Debits
	else if (transactions[transactionIdentifer].type == 'D')
	{
		if (accounts[accountIdentifier].balance > transactions[transactionIdentifer].amount)
		{
			accounts[accountIdentifier].balance -= transactions[transactionIdentifer].amount;
			accounts[accountIdentifier].debits += transactions[transactionIdentifer].amount;
			transactions[transactionIdentifer].processed = 'Y';
			accounts[accountIdentifier].transElement[accounts[accountIdentifier].numOftransactions] = transactionIdentifer;
			accounts[accountIdentifier].numOftransactions++;
		}
		else
		{	//Insufficient funds Do Not Process
			transactions[transactionIdentifer].processed = 'N';
			accounts[accountIdentifier].transElement[accounts[accountIdentifier].numOftransactions] = transactionIdentifer;
			accounts[accountIdentifier].numOftransactions++;
		}
	}
}

//*****************************************************OUTPUT****************************************************************************

void output(ofstream& fout, transaction transactions[], account accounts[], int numberOfAccounts, int numberOfTransactions)
{

	for (int i = 0; i < numberOfAccounts; i++)
	{
		//output account information credits debits and balance
		fout << setw(14) << "Account Number" << setw(20) << "Client Name" << setw(20) << "Credits" << setw(20) << "Debits" << setw(20) << "Balance" << endl;
		fout << setw(14) << accounts[i].accountNumber << setw(20) << accounts[i].Lname + ", " + accounts[i].Fname << setw(20) << fixed << setprecision(2)
			<< accounts[i].credits << setw(20) << fixed << setprecision(2) << accounts[i].debits << setw(20) << fixed << setprecision(2) << accounts[i].balance << endl << endl;

		//Output all transactions associated with the account
		fout << setw(14) << "Transaction ID" << setw(20) << "Type" << setw(20) << "Amount" << setw(20) << "Processed" << endl;
		for (int j = 0; j < accounts[i].numOftransactions; j++)
		{
			int k = accounts[i].transElement[j];
			fout << setw(14) << transactions[k].transactionId << setw(20) << transactions[k].type << setw(20) << fixed << setprecision(2) << transactions[k].amount << setw(20) << transactions[k].processed << endl;
		}
		fout << endl << "**********************************************************************************************" << endl << endl;

	}
}