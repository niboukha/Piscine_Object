# Exercise 00: Divide and Conquer - Bank Encapsulation

## Overview

This exercise implements a properly encapsulated banking system with two classes: `Bank` and `Account`. The goal is to enforce logical constraints through object-oriented design, preventing unauthorized or illogical operations.

## Requirements Met

✅ **Encapsulation**
- Account attributes are private with no direct public setters
- All money operations flow through the Bank class
- Used `friend class Bank` to grant controlled access to private Account methods

✅ **5% Bank Fee**
- Bank receives 5% of all money inflows on client accounts
- Fee applied on:
  - Account creation (initial deposit)
  - Account deposits via `depositToAccount()`
- Uses integer arithmetic (cents) for accurate calculations without floating-point errors

✅ **Unique Account IDs**
- `createAccount()` checks for duplicate IDs and rejects duplicates
- Each account has a unique identifier

✅ **Money Flow Control**
- Impossible to add/modify account balance directly
- All operations go through Bank methods:
  - `createAccount(id, amount)` - Creates account with fee deducted
  - `depositToAccount(id, amount)` - Deposits with 5% fee to bank
  - `withdrawFromAccount(id, amount)` - Withdraws from account
  - `giveLoan(id, amount)` - Loan within bank's liquidity limits

✅ **Loan Restrictions**
- Bank only grants loans within available liquidity
- Returns `false` if insufficient funds

✅ **Getters**
- Public const getters: `get_id()`, `get_value()` (return by value for primitives)
- Public const getter: `get_liquidity()` (Account/Bank read-only access)
- `get_clientAccounts()` returns const reference (no copy)

✅ **C++98 Compliance**
- No C++11 features (no lambdas, no `auto`, no range-based for loops)
- Uses `NULL` instead of `nullptr`
- Compiled with `-std=c++98 -Wall -Wextra -Werror`
- Uses STL containers (`std::vector`)

✅ **Memory Management**
- Bank owns Account objects (created with `new`)
- Bank destructor properly deletes all accounts
- `removeAccount()` deletes account before removal from vector
- No memory leaks

✅ **Monetary Values**
- All amounts stored as cents (integers) for precision
- Formatted output as `$xx.yy` (e.g., `$100.50`)
- 5% fee calculation: `amount * 5 / 100` (exact integer arithmetic)

## Building

```bash
$ make              # Compile
$ make clean        # Remove object files
$ make fclean       # Remove objects and executable
$ make re           # Rebuild from scratch
```

Build artifacts go to `objects/` directory.

## Running

```bash
$ ./a.out
```

### Sample Output

```
Bank created with liquidity : $1000.00

=== Creating Accounts ===
Account created with id : 0 and value : $95.00
Account created with id : 1 and value : $16.15
The client account with id : 0 already exists

=== Deposit Money (5% fee to bank) ===
Deposit of $100.00 to account with id : 0 is successful
Bank liquidity after deposit: $1009.95

=== Withdraw Money ===
Withdrawal of $50.00 from account with id : 1 is successful

=== Give Loan ===
Loan of $200.00 to account with id : 0 is successful
Bank liquidity after loan: $809.95

=== Final State ===
Account A: [0] - [$295.00]
Account B: [1] - [$19.15]

Bank:
Bank informations : 
Liquidity : $809.95
[0] - [$295.00]
[1] - [$19.15]

=== Remove Account ===
The client account with id : 1 is removed

=== Final Bank State ===
Bank informations : 
Liquidity : $809.95
[0] - [$295.00]
```

## File Structure

```
ex00/
├── Account/
│   ├── Account.hpp
│   └── Account.cpp
├── Bank/
│   ├── Bank.hpp
│   └── Bank.cpp
├── main.cpp
├── Makefile
└── README.md
```

## Design Decisions

### 1. Friend Class Pattern
Used `friend class Bank` in Account instead of public setters to:
- Maintain strict encapsulation
- Allow Bank to call private Account methods
- Prevent any other code from modifying accounts
- Enforce that all operations go through Bank logic

### 2. Cents Over Dollars
Storing amounts as cents (integers) instead of dollars (floats) because:
- No floating-point precision errors
- 5% fee calculation is exact and fast: `amount * 5 / 100`
- All money calculations are integer-safe
- Display format `$xx.yy` is applied in formatting functions

### 3. Account Ownership
Bank owns Account objects because:
- Accounts are created by the bank (`createAccount()`)
- Bank manages account lifecycle (creation, removal)
- Destructor cleanup is centralized in Bank
- Prevents orphaned accounts or double-deletion

### 4. Public Const Getters
Made `get_id()` and `get_value()` public to:
- Allow external code to read account information
- Maintain read-only access (no modification possible)
- Enable the `operator<<` overload for printing
- Comply with "Getter creation is mandatory if it makes sense"

### 5. C++98 Strict Compliance
Avoided C++11 features to:
- Support legacy systems
- Follow the exercise constraint
- Use traditional patterns (iterator loops, pointer management)

## Class Responsibilities

**Account**
- Stores id and value (balance in cents)
- Provides read-only access via getters
- Only Bank can modify balance

**Bank**
- Manages account collection
- Enforces business rules (5% fee, unique IDs, loan limits)
- Controls all money flow
- Owns and deletes accounts

## Testing Scenarios

The `main.cpp` demonstrates:
- ✅ Creating unique accounts
- ✅ Rejecting duplicate IDs
- ✅ 5% fee on initial creation (1900 cents → $16.15 after 5% fee)
- ✅ 5% fee on deposits
- ✅ Withdrawals reducing account balance
- ✅ Loans within liquidity limits
- ✅ Account removal with proper cleanup
- ✅ Displaying account/bank state with formatted currency

## Compiler Output

```
c++ -Wall -Wextra -Werror -std=c++98 -g -c main.cpp -o objects/main.o
c++ -Wall -Wextra -Werror -std=c++98 -g -c Account/Account.cpp -o objects/Account/Account.o
c++ -Wall -Wextra -Werror -std=c++98 -g -c Bank/Bank.cpp -o objects/Bank/Bank.o
c++ -Wall -Wextra -Werror -std=c++98 -g -o a.out objects/main.o objects/Account/Account.o objects/Bank/Bank.o
```

Zero warnings, zero errors. ✅

## Key Takeaway

This exercise demonstrates that proper encapsulation and friend classes can enforce complex business logic at compile-time, preventing illogical operations from even being possible, rather than catching them at runtime.
