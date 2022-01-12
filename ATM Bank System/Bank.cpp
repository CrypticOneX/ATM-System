#include <iostream>
#include <stdlib.h>
#include <string>

#include "mysql_connection.h"

#include "cppconn/exception.h"
#include "cppconn/statement.h"
#include "cppconn/driver.h" 

using namespace std;
using namespace sql;

const string host = "tcp://127.0.0.1:3306";
const string uid = "root";
const string password = "ashutosh";
const string dbName = "atm";


void addCustomer() {
	string name, email, mobNumber;
	int accountType;
	cout << "Enter customer name : ";
	cin >> name;
	cout << "Enter customer email : ";
	cin >> email;
	cout << "Enter customer contact number : ";
	cin >> mobNumber;
	cout << "Select account type : \n Enter 1 for Saving Account & 2 for current account." << endl;
	cin >> accountType;

	try {
		Driver* driver;
		Connection* conn;
		ResultSet* rs;
		Statement* smt;

		driver = get_driver_instance();
		conn = driver->connect(host, uid, password);
		conn->setSchema(dbName);
		smt = conn->createStatement();
		rs = smt->executeQuery("INSERT INTO user (name, email, mob_no, balance, pin, account_type) VALUES ('" + name + "', '" + email + "', " + mobNumber + ", 0, 1234, "+ to_string(accountType) +")");

		delete rs;
		delete smt;
		delete conn;
	}
	catch (SQLException exception) {
		// cout << "Error in connection!" << endl;
		// cout << exception.what() << endl;
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
		// cout << "Unable to communicate with database" << endl;
		// cout << exception.what() << endl;
	}

	return balance;
}


void depositMoney() {
	int balance;
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

			if (currentBalance > balance) {
				cout << "Amount deposited successfully" << endl;
				cout << "Your current account balance is now Rs. " << currentBalance << endl;
			}
		}
		else {
			cout << "Invalid account number!!!" << endl;
		}

		delete rs;
		delete smt;
		delete conn;
	}
	catch (SQLException e) {
		// cout << "Unable to communicate with database" << endl;
		cout << endl;
	}
}

int main() {
	while (1) {
		int options;
		int accountNo;
		cout << "***********************" << endl;
		cout << "	Ayush Bank of India	" << endl;
		cout << "***********************" << endl;

		cout << "1. Add Customer" << endl;
		cout << "2. Check Balance" << endl;
		cout << "3. Deposite Money" << endl;
		cout << "4. Exit" << endl;

		cout << "Please choose your option from the following menus : " << endl;
		cin >> options;

		switch (options) {
		case 1:
			addCustomer();
			break;
		case 2:
			cout << "Enter account number : ";
			cin >> accountNo;
			cout << getBalance(accountNo) << endl;
			break;
		case 3:
			depositMoney();
			break;
		case 4:
			exit(0);
			break;
		default:
			cout << "Invalid options!!!" << endl;
		}
	}
	return 0;
}