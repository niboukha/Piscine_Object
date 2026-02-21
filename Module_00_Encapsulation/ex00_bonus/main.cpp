#include "Bank/Bank.hpp"
#include <iomanip>
#include <sstream>

static std::string format_cents(int cents)
{
	int abs_cents = cents < 0 ? -cents : cents;
	int dollars = abs_cents / 100;
	int rem = abs_cents % 100;
	std::ostringstream oss;

	if (cents < 0)
		oss << "-";
	oss << "$" << dollars << "." << std::setw(2) << std::setfill('0') << rem;
	return oss.str();
}

int main()
{
	Bank bank = Bank(100000);

	std::cout << "\n=== Creating Accounts ===" << std::endl;
	
	try {
		bank.createAccount(0, 10000);
		std::cout << "Successfully created Account A (ID: 0)" << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error creating Account A: " << e.what() << std::endl;
	}
	
	try {
		bank.createAccount(1, 1700);
		std::cout << "Successfully created Account B (ID: 1)" << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error creating Account B: " << e.what() << std::endl;
	}
	
	try {
		bank.createAccount(0, 5000);
		std::cout << "ERROR: Duplicate account created (unexpected)" << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Expected error (duplicate ID): " << e.what() << std::endl;
	}

	std::cout << "\n=== Deposit Money to Account A ===" << std::endl;
	
	try {
		bank.depositToAccount(0, 10000);
		std::cout << "Bank liquidity: " << format_cents(bank.get_liquidity()) << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Withdraw from Account B ===" << std::endl;
	
	try {
		bank.withdrawFromAccount(1, 50);
		std::cout << "Withdrawal successful" << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	try {
		bank.withdrawFromAccount(1, 5000);
		std::cout << "ERROR: Withdraw should have failed (insufficient balance)" << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Expected error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Give Loan to Account A ===" << std::endl;
	
	try {
		bank.giveLoan(0, 200);
		std::cout << "Loan successful" << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	try {
		bank.giveLoan(1, 200000);
		std::cout << "ERROR: Loan should have failed (insufficient liquidity)" << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Expected error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Current State ===" << std::endl;
	try {
		std::cout << "Account A: ";
		bank.printAccount(0, std::cout);
		std::cout << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error printing Account A: " << e.what() << std::endl;
	}
	try {
		std::cout << "Account B: ";
		bank.printAccount(1, std::cout);
		std::cout << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error printing Account B: " << e.what() << std::endl;
	}
	std::cout << "Bank Status:" << std::endl << bank << std::endl;

	std::cout << "\n=== Remove Account B ===" << std::endl;
	
	try {
		bank.removeAccount(1);
		std::cout << "Account removed successfully" << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	try {
		bank.removeAccount(1);
		std::cout << "ERROR: Should not remove account twice" << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Expected error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Final Bank State ===" << std::endl;
	std::cout << bank << std::endl;

	return 0;
}
