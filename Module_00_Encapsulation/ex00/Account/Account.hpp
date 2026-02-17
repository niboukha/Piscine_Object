#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
#include <iostream>
#include <vector>

class Bank;

class Account
{
    public:
        friend std::ostream& operator << (std::ostream& p_os, const Account& p_account);

        int get_id() const;
        int get_value() const;

    private:
        Account(int p_id, int p_value);
        ~Account();

        friend class Bank;
        int id;
        int value;
        
        void add_to_balance(int amount);
        void subtract_from_balance(int amount);
};


#endif /* ACCOUNT_HPP */
