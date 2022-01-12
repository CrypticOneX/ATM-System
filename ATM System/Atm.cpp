#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <string>

#include "mysql_connection.h" 
#include "cppconn/exception.h"
#include "cppconn/driver.h" 
#include "cppconn/statement.h" 

using namespace std;
using namespace sql; 

const string host = "tcp://127.0.0.1:3306";
const string uid = "root";
const string password = "ashutosh";
const string dbName = "atm";

void withdrawMoney(int accountNumber) {
	int pin;
	try {
		Driver* driver;
		Connection* conn;
		ResultSet* rs;
		Statement* smt;

		driver = get_driver_instance();
		conn = driver->connect(host, uid, password);
		conn->setSchema(dbName);

		smt = conn->createStatement();
		rs = smt->executeQuery("SELECT * FROM user WHERE account_number = " + to_string(accountNumber));

		if (rs->rowsCount() > 0) {
			while (rs->next()) {
				cout << "Enter your ATM Pin" << endl;
				cin >> pin;

				if (pin == rs->getInt("pin")) {
					int accountType;
					cout << "Choose your account type : " << endl;
					cout << "1.	Saving account" << endl;
					cout << "2.	Current account" << endl;

					cin >> accountType;
					if (accountType == rs->getInt("account_type")) {
						int balance = rs->getInt("balance");
						int amount;
						cout << "Enter amount to withdraw : " << endl;
						cin >> amount;

						if (amount <= balance) {
							balance -= amount;
							delete rs;
							delete smt;

							cout << "Transaction is in progress...." << endl;
							Sleep(6000);
							cout << "Please collect your money!" << endl;
							Sleep(4000);
							cout << "Transaction completed." << endl;

							smt = conn->createStatement();
							rs = smt->executeQuery("UPDATE user SET balance = " + to_string(balance) + " WHERE account_number = " + to_string(accountNumber));
						}
						else {
							cout << "Transaction is in progress..." << endl;
							Sleep(6000);
							cout << "Bhikhari sala :) Insufficient Balance" << endl;
							Sleep(4000);
							cout << "Transaction failed!!!" << endl;
						}
					}
					else {
						cout << "Please select correct bank account type!" << endl;
						Sleep(4000);
						cout << "Transaction failed!" << endl;
					}
				}
				else {
					Sleep(4000);
					cout << "Invalid pin entered" << endl;
					Sleep(4000);
					cout << "Transaction failed" << endl;
				}
			}
		}

		delete rs;
		delete smt;
		delete conn;
	}
	catch (SQLException except) {
		cout << endl;
		// cout << except.what() << endl;
	}
}

int getBalance(int accountNumber) {
	int balance = 0;
	try {
		Driver* driver;
		Connection* conn;
		ResultSet* rs;
		Statement* smt;

		driver = get_driver_instance();
		conn = driver->connect(host, uid, password);
		conn->setSchema(dbName);
		smt = conn->createStatement();
		rs = smt->executeQuery("SELECT balance FROM user WHERE account_number = " + to_string(accountNumber));
		while (rs->next()) {
			balance = rs->getInt("balance");
		}

		delete rs;
		delete smt;
		delete conn;
	}
	catch (SQLException exception) {
		cout << "Unable to communicate with database" << endl;
		cout << exception.what() << endl;
	}

	return balance;
}


void depositMoney() {
	int balance = 0;
	int accountNumber;
	int currentBalance = 0;

	cout << "Enter customer account number : ";
	cin >> accountNumber;

	currentBalance = getBalance(accountNumber);

	try {
		Driver* driver;
		Connection* conn;
		ResultSet* rs;
		Statement* smt;

		driver = get_driver_instance();
		conn = driver->connect(host, uid, password);
		conn->setSchema(dbName);
		smt = conn->createStatement();
		rs = smt->executeQuery("SELECT * FROM user WHERE account_number = " + to_string(accountNumber));

		if (rs->rowsCount() > 0) {
			cout << "Enter amount to deposit : ";
			cin >> balance;

			currentBalance += balance;

			delete rs;
			delete smt;

			smt = conn->createStatement();
			rs = smt->executeQuery("UPDATE user SET balance = " + to_string(currentBalance) + " WHERE account_number = " + to_string(accountNumber));
		}
		else {
			cout << "Invalid account number!!!" << endl;
		}

		delete rs;
		delete smt;
		delete conn;
	}
	catch (SQLException e) {
		cout << endl;
		// cout << e.what() << endl;
	}

	if (currentBalance > balance) {
		cout << "Transaction completed!!!" << endl;
		Sleep(6000);
		cout << "Amount deposited successfully" << endl;
		Sleep(3000);
		cout << "Your current account balance is now Rs. " << currentBalance << endl;
	}
}


void changePin(int accountNumber) {
	int pin;

	cout << "Please set new pin : ";
	cin >> pin;
	try {
		Driver* driver;
		Connection* conn;
		ResultSet* rs;
		Statement* smt;

		driver = get_driver_instance();
		conn = driver->connect(host, uid, password);
		conn->setSchema(dbName);

		smt = conn->createStatement();
		rs = smt->executeQuery("UPDATE user SET pin = " + to_string(pin) + " WHERE account_number = " + to_string(accountNumber));
	}
	catch (SQLException e) {
		cout << endl;
		// cout << e.what() << endl;
	}

	cout << "Transaction completed" << endl;
	Sleep(3000);
	cout << "Pin updated successfully!" << endl;
}

bool checkAccountNumber(int accountNumber, int pin) {
	Driver* driver;
	Connection* conn;
	ResultSet* rs;
	Statement* smt;

	driver = get_driver_instance();
	conn = driver->connect(host, uid, password);
	conn->setSchema(dbName);

	smt = conn->createStatement();
	rs = smt->executeQuery("SELECT * FROM user WHERE account_number = " + to_string(accountNumber) + " AND pin = " + to_string(pin));

	if (rs->rowsCount() > 0) {
		return true;
	} 

	return false;
}	
int main() {
	int choice;
	int accountNumber, pin;
	cout << "*****************************" << endl;
	cout << "	Ayush Bank of India ATM	  " << endl;
	cout << "*****************************" << endl;

	cout << "Enter your account number : " << endl;
	cin >> accountNumber;

	cout << "Enter your ATM Pin : " << endl;
	cin >> pin;
	
	if (!checkAccountNumber(accountNumber, pin)) {
		cout << "Invalid account number or PIN" << endl;
		Sleep(3000);
		cout << "Shutting down ATM";

		for (int i = 1; i <= 8; i++) {
			cout << ".";
			Sleep(1000);
		}
		cout << endl;
		exit(1);
	}
	

	while (1) {
		cout << "1. Withdraw Money" << endl;
		cout << "2. Deposit Money" << endl;
		cout << "3. Change PIN" << endl;
		cout << "4. Check Balance" << endl;
		cout << "5. Shutdown ATM" << endl;

		cout << "Please enter your choice from the above menu : " << endl;
		cin >> choice;

		switch (choice) {
		case 1:
			withdrawMoney(accountNumber);
			break;
		case 2:
			depositMoney();
			break;
		case 3:
			changePin(accountNumber);
			break;
		case 4:
			cout << "Your current account balance is Rs. " << getBalance(accountNumber) << endl;
			break;
		case 5:
			exit(0);
			break;
		default:
			cout << "Invalid choice!!!" << endl;
		}
	}

	return 0;
}