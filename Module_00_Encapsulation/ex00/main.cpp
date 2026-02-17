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
	// Bank creates accounts (IDs are unique)
	Account *accountA = bank.createAccount(0, 10000);
	Account *accountB = bank.createAccount(1, 1700);
	
	// Try to create duplicate ID (should fail)
	Account *duplicate = bank.createAccount(0, 5000);
	if (duplicate != NULL)
		std::cout << "Duplicate account created (unexpected)" << std::endl;

	std::cout << "\n=== Deposit Money (5% fee to bank) ===" << std::endl;
	// Deposit through bank (bank gets 5% fee)
	bank.depositToAccount(0, 10000);
	std::cout << "Bank liquidity after deposit: " << format_cents(bank.get_liquidity()) << std::endl;

	std::cout << "\n=== Withdraw Money ===" << std::endl;
	// Withdraw from account
	bank.withdrawFromAccount(1, 5000);

	std::cout << "\n=== Give Loan ===" << std::endl;
	// Bank gives loan (within liquidity)
	bank.giveLoan(0, 20000);
	std::cout << "Bank liquidity after loan: " << format_cents(bank.get_liquidity()) << std::endl;

	std::cout << "\n=== Final State ===" << std::endl;
	if (accountA != NULL)
		std::cout << "Account A: " << *accountA << std::endl;
	if (accountB != NULL)
		std::cout << "Account B: " << *accountB << std::endl;
	std::cout << "\nBank:\n" << bank << std::endl;

	std::cout << "\n=== Remove Account ===" << std::endl;
	bank.removeAccount(1);

	std::cout << "\n=== Final Bank State ===" << std::endl;
	std::cout << bank << std::endl;

	return (0);
}
