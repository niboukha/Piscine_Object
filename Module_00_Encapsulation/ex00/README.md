# Exercise 00: Divide and Conquer - Bank Encapsulation

## Overview

This exercise implements a simple banking system with two classes, `Bank` and `Account`, focused on encapsulation and controlled money flows.

The goal is to show that rules are enforced by design, not by user discipline: the bank owns the money flow, the account never exposes its internals, and every mutation goes through a single trusted interface.

## Requirements Checklist (and where they are enforced)

✅ **The bank must receive 5% of each money inflow**
- Applied in `createAccount()` and `depositToAccount()` using `amount * 5 / 100`.

✅ **Accounts must never have two identical IDs**
- `createAccount()` calls `findAccountByID()` and throws on duplicates.

✅ **Attributes must not be modifiable from the outside**
- `Account::id` and `Account::value` are private.
- Mutators (`add_to_balance`, `subtract_from_balance`) are private.
- `Bank::liquidity` and `clientAccounts` are private.

✅ **The bank can create, delete, and modify accounts**
- `createAccount()`, `removeAccount()`, `depositToAccount()`, `withdrawFromAccount()`.

✅ **The bank can give a loan within its funds**
- `giveLoan()` checks `liquidity >= amount` before crediting.

✅ **It must be impossible to add money without the bank**
- `Account` balance changes are private and only `Bank` is a friend.

✅ **Getters/Setters when they make sense**
- Read-only getters exist: `Account::get_id()`, `Account::get_value()`, `Bank::get_liquidity()`.
- No public setters: external code cannot modify values directly.
- Getters return by `const int&`, not by copy.

✅ **Const getters where it makes sense**
- All getters are `const` and return `const int&`.

✅ **Read-Only Account Access (Optional, Encapsulation-Safe)**
- `printAccount(id, std::ostream&)` outputs a single account using `operator<<`
- Does not expose `Account*` or allow modifications

✅ **Each choice can be defended**
- See Design Decisions below.

## Interaction Flow (What actually happens)

### 1) Account Creation
```
main: bank.createAccount(0, 10000)
    ↓
Bank: validate amount (> 0)
Bank: ensure ID is unique (findAccountByID)
Bank: compute fee = amount * 5 / 100
Bank: liquidity += fee
Bank: Account(id, amount - fee)
Bank: store pointer in clientAccounts
```

### 2) Deposit (also a money inflow)
```
main: bank.depositToAccount(0, 10000)
    ↓
Bank: validate amount (> 0)
Bank: find account
Bank: compute fee = amount * 5 / 100
Bank: liquidity += fee
Bank: account.add_to_balance(amount - fee)
```

### 3) Loan
```
main: bank.giveLoan(0, 200)
    ↓
Bank: validate amount (> 0)
Bank: check liquidity >= amount
Bank: account.add_to_balance(amount)
Bank: liquidity -= amount
```

### 4) Read-Only Account Output
```
main: bank.printAccount(0, std::cout)
    ↓
Bank: find account internally
    ↓
Bank: stream Account using operator<< (read-only)
```

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
Expected error for duplicate: Account with ID already exists

=== Deposit Money (5% fee to bank) ===
Balance of account with id : 0 increased from $95.00 to $190.00
Deposit of $100.00 to account with id : 0 is successful
Bank liquidity after deposit: $1010.85

=== Withdraw Money ===
Balance of account with id : 1 decreased from $16.15 to $15.65
Withdrawal of $0.50 from account with id : 1 is successful
Error withdrawing from Account B: Account has insufficient balance

=== Give Loan ===
Balance of account with id : 0 increased from $190.00 to $192.00
Loan of $2.00 to account with id : 0 is successful
Error giving loan: The bank has insufficient liquidity

=== Final State ===
Account A: [0] - [$192.00]
Account B: [1] - [$15.65]

Bank:
Bank informations : 
Liquidity : $1008.85
[0] - [$192.00]
[1] - [$15.65]

=== Remove Account ===
The client account with id : 1 is removed
Account with id : 1 and value : $15.65 is destroyed
Error removing Account B: Account with ID not found

=== Final Bank State ===
Bank informations : 
Liquidity : $1008.85
[0] - [$192.00]

Account with id : 0 and value : $192.00 is destroyed
Bank destroyed
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

## Architecture (UML Diagram)

```
+------------------+           owns           +------------------+
|      Bank        |------------------------>|     Account      |
+------------------+     1            *      +------------------+
| - liquidity      |                          | - id             |
| - accounts[]     |                          | - value          |
+------------------+                          +------------------+
| + createAccount()|                          | + get_id()       |
| + removeAccount()|                          | + get_value()    |
| + deposit()      |                          |                  |
| + withdraw()     |                          | (private)        |
| + printAccount() |                          | - add_balance()  |
| + giveLoan()     |                          | - sub_balance()  |
+------------------+                          +------------------+
                     <<friend>>
```

**Key Relationships:**
- **1 Bank owns * Accounts** - Bank manages a collection of Account objects
- **friend class** - Bank has exclusive access to Account's private methods
- **Encapsulation** - Account's internal state is completely protected
- **Operations** - All Account modifications go through Bank methods only

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
- Comply with the getter requirement without exposing setters

### 5. C++98 Strict Compliance
Avoided C++11 features to:
- Support legacy systems
- Follow the exercise constraint
- Use traditional patterns (iterator loops, pointer management)

## Encapsulation Review (ex00)

- `Account` fields and mutators are private; only `Bank` can change balances via friendship.
- `Bank` owns and deletes accounts, so external code cannot create or destroy them.
- `createAccount()` does not return an `Account*`, so external code never gets a direct handle.
- All balance changes flow through `Bank` methods, satisfying the "no direct money changes" rule.

## Q and A (Defense Ready)

**Q: Why does `createAccount()` return void?**
A: It prevents external code from holding an `Account*`, which keeps ownership and mutation strictly inside `Bank`.

**Q: How do I print a single account now?**
A: Use `printAccount(id, std::ostream&)`, which streams the account via `operator<<` without exposing the object.

**Q: Why use a friend class instead of public setters?**
A: Friendship makes `Bank` the only class allowed to mutate account state, enforcing a single point of control for validation, fees, and loan limits.

**Q: Why are getters returning `const int&`?**
A: The requirement forbids getters by copy when they make sense. Returning a const reference provides read-only access without copies.

**Q: Why store money as integer cents?**
A: It avoids floating-point precision bugs and keeps fee calculations exact.

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


