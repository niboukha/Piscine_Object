#include "Bank.hpp"
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

Bank::Bank() : liquidity(1000)
{
    std::cout << "Bank created with liquidity : " << format_cents(liquidity) << std::endl;
}

Bank::Bank(int p_liquidity) : liquidity(p_liquidity)
{
    std::cout << "Bank created with liquidity : " << format_cents(liquidity) << std::endl;
}

Bank::~Bank()
{
    for (std::vector<Account *>::iterator it = clientAccounts.begin(); it != clientAccounts.end(); ++it) {
        delete *it;
    }
    clientAccounts.clear();
    std::cout << "Bank destroyed" << std::endl;
}

int Bank::get_liquidity() const
{
    return (liquidity);
}

void Bank::set_clientAccount(Account *p_account)
{
    clientAccounts.push_back(p_account);
}

Account *Bank::findAccountByID(int id){
    std::vector<Account *>::iterator it = clientAccounts.begin();
    for (; it != clientAccounts.end(); ++it) {
        if ((*it)->get_id() == id) {
            return (*it);
        }
    }
    return (NULL);
}

int Bank::computeDepositFee(int amount){
    return (amount * 5 / 100);
}

bool Bank::isAmountValid(int amount){
    return (amount > 0);
}

Account *Bank::createAccount(int id, int amount)
{
    if (!isAmountValid(amount)) {
        std::cout << "The initial amount must be positive" << std::endl;
        return (NULL);
    }

    if (findAccountByID(id) != NULL) {
        std::cout << "The client account with id : " << id << " already exists" << std::endl;
        return (NULL);
    }
    int fee = computeDepositFee(amount);
    liquidity += fee;
    Account *account = new Account(id, amount - fee);
    set_clientAccount(account);
    return (account);
}

void Bank::removeAccount(int id)
{
    Account *account = findAccountByID(id);
    if (account != NULL) {
        std::vector<Account *>::iterator it = std::find(clientAccounts.begin(), clientAccounts.end(), account);
        if (it != clientAccounts.end()) {
            delete *it;
            clientAccounts.erase(it);
            std::cout << "The client account with id : " << id << " is removed" << std::endl;
        }
    } else {
        std::cout << "The client account with id : " << id << " is not found" << std::endl;
    }
}

void Bank::depositToAccount(int id, int amount)
{
    if (!isAmountValid(amount)) {
        std::cout << "The deposit amount must be positive" << std::endl;
        return;
    }

    Account *account = findAccountByID(id);
    if (account != NULL) {
        int fee = computeDepositFee(amount);
        liquidity += fee;
        account->add_to_balance(amount - fee);
        std::cout << "Deposit of " << format_cents(amount) << " to account with id : " << id << " is successful" << std::endl;
    } else {
        std::cout << "The client account with id : " << id << " is not found" << std::endl;
    }
}

void Bank::withdrawFromAccount(int id, int amount)
{
    if (!isAmountValid(amount)) {
        std::cout << "The withdrawal amount must be positive" << std::endl;
        return;
    }

    Account *account = findAccountByID(id);
    if (account != NULL) {
        if (account->get_value() >= amount) {
            account->subtract_from_balance(amount);
            std::cout << "Withdrawal of " << format_cents(amount) << " from account with id : " << id << " is successful" << std::endl;
        } else {
            std::cout << "The client account with id : " << id << " has insufficient balance" << std::endl;
        }
    } else {
        std::cout << "The client account with id : " << id << " is not found" << std::endl;
    }
}

bool Bank::giveLoan(int accountID, int amount)
{
    if (!isAmountValid(amount)) {
        std::cout << "The loan amount must be positive" << std::endl;
        return (false);
    }

    if (liquidity >= amount) {
        Account *account = findAccountByID(accountID);
        if (account != NULL) {
            account->add_to_balance(amount);
            liquidity -= amount;
            std::cout << "Loan of " << format_cents(amount) << " to account with id : " << accountID << " is successful" << std::endl;
            return (true);
        } else {
            std::cout << "The client account with id : " << accountID << " is not found" << std::endl;
            return (false);
        }
    } else {
        std::cout << "The bank has insufficient liquidity" << std::endl;
        return (false);
    }
}


std::ostream& operator<<(std::ostream& p_os, const Bank& p_bank)
{
    p_os << "Bank informations : " << std::endl;
    p_os << "Liquidity : " << format_cents(p_bank.get_liquidity()) << std::endl;
    const std::vector<Account *> &accounts = p_bank.clientAccounts;
    for (std::vector<Account *>::const_iterator it = accounts.begin(); it != accounts.end(); ++it)
        p_os << **it << std::endl;
    return (p_os);
}
