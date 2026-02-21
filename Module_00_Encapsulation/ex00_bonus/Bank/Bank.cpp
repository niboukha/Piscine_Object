#include "Bank.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm> // for std::find_if

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

// Functor for finding account by ID
struct Bank::AccountIDMatch
{
    int id;
    AccountIDMatch(int _id) : id(_id) {}
    bool operator()(Account* acc) const { return acc->get_id() == id; }
};

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
    for (std::vector<Account*>::iterator it = clientAccounts.begin(); it != clientAccounts.end(); ++it)
        delete *it;
    clientAccounts.clear();
    std::cout << "Bank destroyed" << std::endl;
}

Bank::Account::Account(int p_id, int p_value) : id(p_id), value(p_value)
{
    std::cout << "Account created with id : " << id << " and value : " << format_cents(value) << std::endl;
}

Bank::Account::~Account() {}

const int&  Bank::Account::get_id() const { return id; }
const int& Bank::Account::get_value() const { return value; }

std::ostream& operator<<(std::ostream& p_os, const Bank::Account& p_account)
{
    p_os << "[" << p_account.get_id() << "] - [" << format_cents(p_account.get_value()) << "]";
    return p_os;
}

const int& Bank::get_liquidity() const { return liquidity; }
void Bank::set_clientAccount(Account* p_account) { clientAccounts.push_back(p_account); }

// Refactored findAccountByID using operator[]
Bank::Account* Bank::findAccountByID(int id)
{
    try {
        return &((*this)[id]);
    } catch (const std::exception&) {
        return NULL;
    }
}

int Bank::computeDepositFee(int amount) { return amount * 5 / 100; }
bool Bank::isAmountValid(int amount) { return amount > 0; }

void Bank::createAccount(int id, int amount)
{
    if (!isAmountValid(amount))
        throw std::invalid_argument("The initial amount must be positive");

    if (findAccountByID(id) != NULL)
        throw std::invalid_argument("Account with ID already exists");

    int fee = computeDepositFee(amount);
    liquidity += fee;

    Account* account = new Account(id, amount - fee);
    set_clientAccount(account);
}

void Bank::removeAccount(int id)
{
    Account* account = findAccountByID(id);
    if (!account)
        throw std::invalid_argument("Account with ID not found");

    std::vector<Account*>::iterator it = std::find(clientAccounts.begin(), clientAccounts.end(), account);
    if (it != clientAccounts.end())
    {
        delete *it;
        clientAccounts.erase(it);
        std::cout << "The client account with id : " << id << " is removed" << std::endl;
    }
}

void Bank::depositToAccount(int id, int amount)
{
    if (!isAmountValid(amount))
        throw std::invalid_argument("The deposit amount must be positive");

    Account* account = findAccountByID(id);
    if (!account)
        throw std::invalid_argument("Account with ID not found");

    int fee = computeDepositFee(amount);
    int netDeposit = amount - fee;
    liquidity += fee;
    std::cout << "Balance of account with id : " << account->id << " increased from " 
              << format_cents(account->value) << " to " << format_cents(account->value + netDeposit) << std::endl;
    account->value += netDeposit;

    std::cout << "Deposit of " << format_cents(amount) << " to account with id : " << id << " is successful" << std::endl;
}

void Bank::withdrawFromAccount(int id, int amount)
{
    if (!isAmountValid(amount))
        throw std::invalid_argument("The withdrawal amount must be positive");

    Account* account = findAccountByID(id);
    if (!account)
        throw std::invalid_argument("Account with ID not found");

    if (account->get_value() < amount)
        throw std::invalid_argument("Account has insufficient balance");

    std::cout << "Balance of account with id : " << account->id << " decreased from "
              << format_cents(account->value) << " to " << format_cents(account->value - amount) << std::endl;
    account->value -= amount;
    std::cout << "Withdrawal of " << format_cents(amount) << " from account with id : " << id << " is successful" << std::endl;
}

void Bank::giveLoan(int accountID, int amount)
{
    if (!isAmountValid(amount))
        throw std::invalid_argument("The loan amount must be positive");

    if (liquidity < amount)
        throw std::invalid_argument("The bank has insufficient liquidity");

    Account* account = findAccountByID(accountID);
    if (!account)
        throw std::invalid_argument("Account with ID not found");

    std::cout << "Balance of account with id : " << account->id << " increased from " 
              << format_cents(account->value) << " to " << format_cents(account->value + amount) << std::endl;
    account->value += amount;
    liquidity -= amount;
    std::cout << "Loan of " << format_cents(amount) << " to account with id : " << accountID << " is successful" << std::endl;
}

void Bank::printAccount(int id, std::ostream& os) const
{
    for (std::vector<Account*>::const_iterator it = clientAccounts.begin(); it != clientAccounts.end(); ++it) {
        if ((*it)->get_id() == id) {
            os << **it;
            return;
        }
    }
    throw std::invalid_argument("Account with ID not found");
}

Bank::Account& Bank::operator[](int id)
{
    std::vector<Account*>::iterator it = std::find_if(
        clientAccounts.begin(),
        clientAccounts.end(),
        AccountIDMatch(id)
    );

    if (it != clientAccounts.end())
        return **it;

    throw std::invalid_argument("Account with ID not found");
}

std::ostream& operator<<(std::ostream& p_os, const Bank& p_bank)
{
    p_os << "Bank informations : " << std::endl;
    p_os << "Liquidity : " << format_cents(p_bank.get_liquidity()) << std::endl;
    const std::vector<Bank::Account*>& accounts = p_bank.clientAccounts;
    for (std::vector<Bank::Account*>::const_iterator it = accounts.begin(); it != accounts.end(); ++it)
        p_os << **it << std::endl;
    return p_os;
}
