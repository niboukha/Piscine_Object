#include "Account.hpp"
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

Account::Account(int p_id, int p_value) : id(p_id), value(p_value)
{
    std::cout << "Account created with id : " << id << " and value : " << format_cents(value) << std::endl;
}

Account::~Account()
{
    std::cout << "Account with id : " << id << " and value : " << format_cents(value) << " is destroyed" << std::endl;
}

int Account::get_id() const
{
    return (id);
}

int Account::get_value() const
{
    return (value);
}

void Account::add_to_balance(int amount)
{
    std::cout << "Balance of account with id : " << id << " increased from " << format_cents(value)
              << " to " << format_cents(value + amount) << std::endl;
    value += amount;
}

void Account::subtract_from_balance(int amount)
{
    std::cout << "Balance of account with id : " << id << " decreased from " << format_cents(value)
              << " to " << format_cents(value - amount) << std::endl;
    value -= amount;
}

std::ostream& operator << (std::ostream& p_os, const Account& p_account)
{
    p_os << "[" << p_account.get_id() << "] - [" << format_cents(p_account.get_value()) << "]";
    return (p_os);
}
