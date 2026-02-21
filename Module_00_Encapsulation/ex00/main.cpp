#include "Account/Account.hpp"
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
	// Create a bank with initial liquidity
	Bank bank = Bank(100000);

	std::cout << "\n=== Creating Accounts ==="  << std::endl;
	
	// Try to create Account A
	try {
		bank.createAccount(0, 10000);
	} catch (const std::exception& e) {
		std::cerr << "Error creating Account A: " << e.what() << std::endl;
	}
	
	// Try to create Account B
	try {
		bank.createAccount(1, 1700);
	} catch (const std::exception& e) {
		std::cerr << "Error creating Account B: " << e.what() << std::endl;
	}
	
	// Try to create duplicate ID (should fail)
	try {
		bank.createAccount(0, 5000);
	} catch (const std::exception& e) {
		std::cerr << "Expected error for duplicate: " << e.what() << std::endl;
	}

	std::cout << "\n=== Deposit Money (5% fee to bank) ===" << std::endl;
	
	// Try to deposit to Account A
	try {
		bank.depositToAccount(0, 10000);
		std::cout << "Bank liquidity after deposit: " << format_cents(bank.get_liquidity()) << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error depositing to Account A: " << e.what() << std::endl;
	}

	std::cout << "\n=== Withdraw Money ===" << std::endl;
	
	// Try to withdraw from Account B
	try {
		bank.withdrawFromAccount(1, 50);
		bank.withdrawFromAccount(1, 5000);
	} catch (const std::exception& e) {
		std::cerr << "Error withdrawing from Account B: " << e.what() << std::endl;
	}

	std::cout << "\n=== Give Loan ===" << std::endl;
	
	// Try to give a loan to Account A
	try {
		bank.giveLoan(0, 200);
		bank.giveLoan(1, 200000); // Should fail due to insufficient liquidity
		std::cout << "Bank liquidity after loan: " << format_cents(bank.get_liquidity()) << std::endl;
	} catch (const std::exception& e) {
		std::cerr << "Error giving loan: " << e.what() << std::endl;
	}

	std::cout << "\n=== Final State ===" << std::endl;
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
	std::cout << "\n" << bank << std::endl;

	std::cout << "\n=== Remove Account ===" << std::endl;
	
	// Try to remove Account B
	try {
		bank.removeAccount(1);
		bank.removeAccount(1);
	} catch (const std::exception& e) {
		std::cerr << "Error removing Account B: " << e.what() << std::endl;
	}

	std::cout << "\n=== Final Bank State ===" << std::endl;
	std::cout << bank << std::endl;

	return (0);
}
