#ifndef BANK_HPP
#define BANK_HPP

#include <iostream>
#include <vector>
#include <algorithm> 
#include <iterator>

class Bank
{
    private:

        class Account
        {
            public:
            
            const int& get_id() const;
            const int& get_value() const;

            private:
                Account(int p_id, int p_value);
                ~Account();
                
                int id;
                int value;
                
                friend class Bank;
        };
        
        int liquidity;
        std::vector<Account *> clientAccounts;
        struct AccountIDMatch;
        
        void set_clientAccount(Account *p_account);
        
        Account& operator[](int id);

        //helper functions
        Account *findAccountByID(int id);
        int computeDepositFee(int amount);
        bool isAmountValid(int amount);
        
        
        friend std::ostream& operator << (std::ostream& p_os, const Account& p_account);

    public:
        Bank();
        Bank(int p_liquidity);

        ~Bank();

        const int& get_liquidity() const;

        //bank operations
        void createAccount(int id, int amount);
        void removeAccount(int id);
        void depositToAccount(int id, int amount);
        void withdrawFromAccount(int id, int amount);
        
        //loan operation
        void giveLoan(int accountID, int amount);
        
        void printAccount(int id, std::ostream& os) const;
        friend std::ostream& operator << (std::ostream& p_os, const Bank& p_bank);
};

#endif /* BANK_HPP */