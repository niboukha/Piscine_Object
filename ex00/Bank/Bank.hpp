#ifndef BANK_HPP
#define BANK_HPP

#include <iostream>
#include <vector>
#include <algorithm> 
#include <iterator>

#include "../Account/Account.hpp"

class Bank
{
    public:
        Bank();
        Bank(int p_liquidity);

        ~Bank();

        int get_liquidity() const;

        //bank operations
        Account *createAccount(int id, int amount);
        void removeAccount(int id);
        void depositToAccount(int id, int amount);
        void withdrawFromAccount(int id, int amount);
        
        //loan operation
        bool giveLoan(int accountID, int amount);

        friend std::ostream& operator << (std::ostream& p_os, const Bank& p_bank);

    private:
        int liquidity;
        std::vector<Account *> clientAccounts;
        
        void set_clientAccount(Account *p_account);
        
        //helper functions
        Account *findAccountByID(int id);
        int computeDepositFee(int amount);
        bool isAmountValid(int amount);

};

#endif /* BANK_HPP */