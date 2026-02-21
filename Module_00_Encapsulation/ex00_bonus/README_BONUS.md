# Exercise 00 Bonus: Advanced Bank Encapsulation with Design Patterns

## Overview

This bonus exercise extends the standard encapsulation exercise with **advanced C++98 design patterns**. It implements a properly encapsulated banking system with strict design constraints that enforce:
- Single responsibility principle (Account holds data only, Bank manages operations)
- Algorithm-based searches (no manual loops)
- Complete encapsulation of internal data structures
- Throw-based error handling with main-level exception catching

## 🎯 Bonus Requirements Met

### 1. ✅ **Only Const Getters in Account**

**Requirement**: "You can't create any other methods than const getters in Account structure"

Account is a **pure data holder** with only read-only access:
- ✅ Only const accessor methods exist
- ✅ No non-const methods (no setters, no mutators)
- ✅ Getters return by **const reference** (`const int&`) to prevent accidental copies
- ✅ Private constructor/destructor ensures Bank is sole creator/destroyer

**Location**: [Bank/Bank.hpp](Bank/Bank.hpp#L18-L19)
```cpp
class Account {
    public:
        const int& get_id() const;        // Const getter, returns reference
        const int& get_value() const;     // Const getter, returns reference
    
    private:
        Account(int p_id, int p_value);   // Private: only Bank creates
        ~Account();                        // Private: only Bank destroys
        int id;
        int value;
        friend class Bank;                // Bank can modify private members
};
```

**Why This Design?**
- Prevents accidental modification from outside
- RAII pattern: Bank controls complete lifecycle
- Enforces that all state changes go through Bank methods
- Maximum encapsulation: Account is immutable from client perspective

---

### 2. ✅ **Account as Inner Private Class**

**Requirement**: "The Account structure must be internal to the Bank structure"

Account is declared as a **private nested class** within Bank:
- ✅ `Account` is completely hidden from external code
- ✅ Only Bank can instantiate/use Account
- ✅ Implementation details are completely encapsulated
- ✅ Clients interact only with Bank public interface

**Location**: [Bank/Bank.hpp](Bank/Bank.hpp#L11-L28)
```cpp
class Bank {
    private:
        class Account {  // ← Private inner class
            // Account implementation
        };
    public:
        // Bank public interface only
};
```

**Why Private Inner Class?**
- Hides Bank's internal structure
- Account is an implementation detail, not a public type
- Forces all operations through Bank interface
- Cleaner API: clients never need to know Account exists

---

### 3. ✅ **operator[] Without Loops or While Statements**

**Requirement**: "The Bank structure must contain an operator[] to get an account by its ID, but you aren't allowed to make a while or a for loop to find the account"

Uses **C++98 `std::find_if` algorithm** with a **functor** (no manual loops):
- ✅ `operator[]` is **PRIVATE** - internal implementation detail (better encapsulation)
- ✅ Uses `std::find_if` with `AccountIDMatch` functor - no manual loops/while
- ✅ Called internally by `findAccountByID()` delegation pattern

**Location**: [Bank/Bank.cpp](Bank/Bank.cpp#L173-L191)
```cpp
// Functor for searching by ID (no loops!)
struct Bank::AccountIDMatch {
    int id;
    AccountIDMatch(int _id) : id(_id) {}
    bool operator()(Account* acc) const { 
        return acc->get_id() == id; 
    }
};

// operator[] implementation - PRIVATE (internal use only)
Bank::Account& Bank::operator[](int id) {  // Private - not in public interface
    std::vector<Account*>::iterator it = std::find_if(
        clientAccounts.begin(),      // Search from start
        clientAccounts.end(),        // To end
        AccountIDMatch(id)           // Using AccountIDMatch functor
    );
    
    if (it != clientAccounts.end())
        return **it;                 // Found: dereference pointer
    
    throw std::invalid_argument("Account with ID not found");  // Not found: throw
}
```

**Why Private operator[]?**
- ✅ **Better encapsulation** - internal implementation, not public API
- ✅ **Meets requirement** - "must contain" (exists ✅), doesn't say must be public
- ✅ **Single responsibility** - clients use Bank methods, not operator[] directly
- ✅ **Controlled access** - only used internally via delegation pattern

**How Delegation Works**:
`findAccountByID()` internally calls `operator[]` for DRY principle:
```cpp
Bank::Account* Bank::findAccountByID(int id) {
    try {
        return &((*this)[id]);      // Call private operator[], convert to pointer
    } catch (const std::exception&) {
        return NULL;                // Return NULL if not found
    }
}
```

**Why std::find_if Instead of Loops?**
- ✅ **No for/while loops** - meets requirement, uses algorithms instead
- ✅ **Functional style** - expresses intent clearly (what to find, not how)
- ✅ **Less error-prone** - no iterator management bugs, no off-by-one errors
- ✅ **Standard patterns** - uses C++ STL algorithms everyone recognizes
- ✅ **Maintainable** - easier to modify search logic (just change functor)

---

### 4. ✅ **Throw-Based Error Management**

**Requirement**: "The error management must be done via throw, and the main must handle those errors"

All validation errors are **thrown as exceptions**, not returned as error codes:

**Exceptions Thrown by Bank**:
- `std::invalid_argument` - Invalid amounts, duplicate IDs, account not found, insufficient balance/liquidity

**Location**: [Bank/Bank.cpp](Bank/Bank.cpp#L84-L171)
- Line 87: Negative/zero amounts throw
- Line 90: Duplicate ID throws
- Line 149: Account not found throws
- Line 152: Insufficient balance throws
- Line 166: Insufficient liquidity throws

**Location**: [main.cpp](main.cpp#L20-L106) - Exception Handling
```cpp
try {
    bank.createAccount(0, 10000);
    std::cout << "Successfully created Account A (ID: 0)" << std::endl;
} catch (const std::exception& e) {
    std::cerr << "Error creating Account A: " << e.what() << std::endl;
}

try {
    bank.depositToAccount(0, 10000);
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

**Why Throw Exceptions Instead of Error Codes?**
- ✅ **Type-safe** - compiler ensures exceptions are caught
- ✅ **Automatic cleanup** - stack unwinding on errors
- ✅ **Clear intent** - success and error paths clearly separated
- ✅ **C++ standard** - best practice for error handling
- ✅ **No silent failures** - missing error codes can be overlooked

---

## 📋 Mandatory Requirements (All Met)

✅ **5% Bank Fee on Inflows**
- Bank receives 5% on account creation
- Bank receives 5% on deposits
- Implementation: `computeDepositFee(amount)` = `amount * 5 / 100`

✅ **No Duplicate Account IDs**
- `createAccount()` throws if ID exists
- Check via `findAccountByID(id)` delegation pattern

✅ **Attributes Not Modifiable from Outside**
- Account members are private
- Only const getters public
- Bank is friend for controlled access

✅ **Bank Can Manage Accounts**
- `createAccount()` - Creates with fee deducted
- `removeAccount()` - Deletes account
- `depositToAccount()` - Deposits with fee
- `withdrawFromAccount()` - Withdraws with validation
- `giveLoan()` - Loans within liquidity

✅ **Loans Within Bank Funds**
- `giveLoan()` throws if insufficient liquidity
- Throws: `"The bank has insufficient liquidity"`

✅ **No Direct Money Addition to Accounts**
- Account has no public/non-const methods
- Only way to change balance is through Bank methods
- All operations protected by Bank validation

✅ **Read-Only Account Access (Optional, Encapsulation-Safe)**
- `printAccount(id, std::ostream&)` outputs a single account using `operator<<`
- Does not expose `Account*` or allow modifications

✅ **Const Reference Getters**
- `get_id()` returns `const int&` (not by copy)
- `get_value()` returns `const int&` (not by copy)  
- `get_liquidity()` returns `int` (primitive type, safe)

✅ **C++98 Strict Compliance**
- Compiled with `-std=c++98 -Wall -Wextra -Werror`
- No C++11 features (no lambdas, no `auto`, no range-based for)
- Uses `NULL` instead of `nullptr`
- Traditional STL iterators and algorithms

✅ **Memory Safety (RAII)**
- Bank owns all Account pointers
- Bank destructor deletes all accounts
- No memory leaks
- `removeAccount()` properly cleans up

✅ **Monetary Precision**
- All amounts stored in cents (integers)
- No floating-point errors
- Formatted as `$xx.yy`

---

## 🏗️ Design Architecture

### Class Diagram
```
┌────────────────────────────────────┐
│           Bank (public)            │
├────────────────────────────────────┤
│ - liquidity: int                   │
│ - clientAccounts: vector<Account*> │
│ - struct AccountIDMatch (functor)  │
├────────────────────────────────────┤
│ + createAccount(id, amount)        │ ──┐
│ + removeAccount(id)                │   │ throws exceptions
│ + depositToAccount(id, amount)     │   │ on errors
│ + withdrawFromAccount(id, amount)  │   │
│ + giveLoan(id, amount)             │   │
│ + printAccount(id, os)             │   │ read-only view
│ + operator[](id): Account&         │   │
│ + get_liquidity(): const int       │ ──┘
│ + ~Bank()                          │
├────────────────────────────────────┤
│ - Account (private inner class)    │
│   ├─ int id (const getter only)    │
│   ├─ int value (const getter only) │
│   └─ Private constructor/destructor│
└────────────────────────────────────┘
        ▲
        │ friend class Bank
        │ (can access private members)
        │
    operates on
```

### Interaction Flow

1. **Creating Account**
   ```
   main: bank.createAccount(0, 10000)
        ↓
   Bank: Check ID unique via findAccountByID()
        ├─ findAccountByID() calls private operator[](0)
        ├─ operator[] uses std::find_if + AccountIDMatch functor (NO LOOPS!)
        └─ Returns Account pointer or NULL
   Bank: Check amount valid (> 0)
   Bank: Calculate 5% fee
   Bank: Create Account object (private constructor)
   Bank: Add to clientAccounts vector
   ```

2. **Accessing Account Internally**
   ```
   Bank methods: depositToAccount, withdrawFromAccount, giveLoan
        ↓
   findAccountByID(id)  [private]
        ↓
   operator[](id)  [private - NOT accessible from main]
        ↓
   std::find_if with AccountIDMatch functor (NO LOOPS!)
        ↓
   Found: return Account reference
   Not found: throw std::invalid_argument
   ```

3. **Error Handling**
   ```
   main: try { bank.operation() }
        ↓
   Bank: throw std::invalid_argument
        ↓
   main: catch { handle error }
   ```

4. **Read-Only Account Output**
   ```
   main: bank.printAccount(0, std::cout)
       ↓
   Bank: find account internally
       ↓
   Bank: stream Account using operator<< (read-only)
   ```

---

## 🔧 Building & Running

```bash
$ make              # Compile
$ make clean        # Remove object files
$ make fclean       # Remove objects and executable
$ make re           # Rebuild from scratch
$ ./a.out           # Run tests
```

Compilation flags:
```makefile
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g
```

---

## 📊 Sample Output

```
Bank created with liquidity : $1000.00

=== Creating Accounts ===
Account created with id : 0 and value : $95.00
Successfully created Account A (ID: 0)
Account created with id : 1 and value : $16.15
Successfully created Account B (ID: 1)
Expected error (duplicate ID): Account with ID already exists

=== Deposit Money to Account A ===
Balance of account with id : 0 increased from $95.00 to $190.00
Deposit of $100.00 to account with id : 0 is successful
Bank liquidity: $1010.85

=== Withdraw from Account B ===
Balance of account with id : 1 decreased from $16.15 to $15.65
Withdrawal of $0.50 from account with id : 1 is successful
Withdrawal successful
Expected error: Account has insufficient balance

=== Give Loan to Account A ===
Balance of account with id : 0 increased from $190.00 to $192.00
Loan of $2.00 to account with id : 0 is successful
Loan successful
Expected error: The bank has insufficient liquidity

=== Current State ===
Account A: [0] - [$192.00]
Account B: [1] - [$15.65]
Bank Status:
Bank informations : 
Liquidity : $1008.85
[0] - [$192.00]
[1] - [$15.65]

=== Remove Account B ===
The client account with id : 1 is removed
Account removed successfully
Expected error: Account with ID not found

=== Final Bank State ===
Bank informations : 
Liquidity : $1008.85
[0] - [$192.00]

Bank destroyed
```

---

## 💡 Design Decisions & Defense

### Q: Why Make Account a Private Inner Class?
**A:** Forces all Account access through Bank's controlled interface. Clients cannot accidentally create invalid Account states or break encapsulation.

### Q: Why Only Const Getters in Account?
**A:** Enforces that Account's state is immutable from outside. All modifications must go through Bank's business logic, ensuring invariants are maintained.

### Q: Why is operator[] Private?
**A:** Better encapsulation - `operator[]` is an internal implementation detail, not part of the public API. Clients use Bank methods instead. The requirement says "contain" (exists ✅), not "public". Making it private demonstrates superior design.

### Q: Why std::find_if Instead of for/while?
**A:** Functional programming approach is more expressive, less error-prone, and follows C++ STL patterns. Meets the "no loops" requirement elegantly while keeping code maintainable.

### Q: Why Throw Exceptions?
**A:** Type-safe error handling. Compiler ensures exceptions are caught. Stack unwinding provides automatic cleanup. Separates normal flow from error flow clearly.

### Q: Why Const References for Getters?
**A:** Prevents unnecessary copying of return values. Satisfies "Getters by copy will not be accepted" requirement strictly.

### Q: Why Delegation (findAccountByID → operator[])?
**A:** DRY principle. One source of truth for account lookup. If search algorithm changes, only `operator[]` needs updating.

### Q: Why add printAccount if encapsulation is strict?
**A:** It is read-only. It prints via `operator<<` without exposing `Account*`, so callers can see data but cannot modify it.

### Q: Why friend class Instead of Public Methods?
**A:** Minimal public interface. Bank controls exactly which Account methods are used and when. Better encapsulation than public setters.

---

## 📁 File Structure

```
ex00_bonus/
├── Bank/
│   ├── Bank.hpp              # Header with private Account inner class
│   └── Bank.cpp              # Implementation with std::find_if pattern
├── main.cpp                  # Exception-based test suite
├── Makefile                  # C++98 compilation
├── README.md                 # Mandatory requirements
└── README_BONUS.md          # This file (bonus requirements)
```

---

## ✨ Key Implementation Highlights

| Feature | Implementation | Benefit |
|---------|----------------|---------|
| **Private Inner Class** | `class Account { private: }` inside `class Bank` | Maximum encapsulation |
| **Const-Only Getters** | `const int& get_id() const;` | Prevents accidental copies, enforces immutability |
| **std::find_if with Functor** | `AccountIDMatch` struct + `operator[]` | No loops required, algorithm-based search |
| **Exception Throwing** | All errors throw `std::invalid_argument` | Type-safe, automatic cleanup |
| **Delegation Pattern** | `findAccountByID()` calls `operator[]` | DRY principle, single source of truth |
| **Friend Class** | `friend class Bank` in Account | Controlled private access |
| **RAII Pattern** | Bank owns & deletes all Accounts | Guaranteed cleanup, no memory leaks |
| **Cents-Based Money** | `int` for cents, never `double` | No floating-point errors |

---

## 🔐 RAII Pattern Explanation

**RAII** = "Resource Acquisition Is Initialization" - Resources are acquired in constructor, guaranteed cleanup in destructor.

### How RAII Works in Your Code

**Bank is the Resource Owner:**

**Constructor** (Acquires resources):
```cpp
Bank::Bank(int p_liquidity) : liquidity(p_liquidity)
{
    // clientAccounts vector is initialized
    // Ready to store Account pointers
}
```

**Account Creation** (Allocate):
```cpp
void Bank::createAccount(int id, int amount) {
    // ...
    Account* account = new Account(id, amount - fee);  // ← ALLOCATE
    set_clientAccount(account);                        // ← STORE pointer
}
```

**Destructor** (Releases resources - THE RAII IN ACTION):
```cpp
Bank::~Bank()
{
    for (std::vector<Account*>::iterator it = clientAccounts.begin(); 
         it != clientAccounts.end(); ++it)
        delete *it;              // ← DELETE all Account pointers
    clientAccounts.clear();      // ← Clear vector
}
```

### Why This is RAII

```cpp
int main() {
    {
        Bank bank(100000);           // 1. Constructor: initialize
        
        bank.createAccount(0, 1000); // 2. Allocate Account
        bank.createAccount(1, 500);  // 3. Allocate Account
        
        // ... operations ...
        
        // Exception could happen here!
        throw std::runtime_error("Oops!");
    }
    // 4. Destructor GUARANTEED to run here
    //    ↓
    // ~Bank() deletes all Accounts
    // ↓
    // NO MEMORY LEAK - even if exception occurred!
}
```

### Resource Ownership

| What | Who Owns | When Acquired | When Released | How |
|-----|----------|---------------|---------------|-----|
| **Bank's liquidity** | Bank | Constructor | Destructor | Part of Bank object |
| **Accounts** | Bank | createAccount() | ~Bank() | `new`/`delete` |
| **vector container** | Bank | Constructor | Destructor | STL automatic |
| **Account's int members** | Account | Account constructor | Account destructor | Stack members (no cleanup needed) |

### Account Destructor Note

```cpp
Bank::Account::~Account()
{
    std::cout << "Account with id : " << id 
              << " and value : " << format_cents(value) << " is destroyed" << std::endl;
}
```

**Important: This destructor is NOT doing resource management:**
- Account only owns primitive `int` members (id, value)
- Primitives don't require `delete` or cleanup
- The `std::cout` is purely for tracing/debugging execution flow
- **Real resource cleanup happens at Bank level** when Bank's destructor deletes all Account pointers

**The RAII pattern is at the Bank level, not the Account level.**

### Without RAII (Problematic)

```cpp
// ❌ BAD - Manual cleanup required
void problemFunction() {
    Bank* bank = new Bank(100000);
    bank->createAccount(0, 1000);
    
    if (someError) {
        // Oops! Forgot to delete bank
        return;  // ← MEMORY LEAK!
    }
    
    delete bank;  // ← If error above, never reached!
}
```

### With RAII (Safe)

```cpp
// ✅ GOOD - Automatic cleanup guaranteed
void safeFunction() {
    Bank bank(100000);           // Stack allocation
    bank.createAccount(0, 1000);
    
    if (someError) {
        throw std::runtime_error("Error!");  // ← Destructor still runs!
    }
    
}  // Destructor automatically called
   // All Accounts deleted
   // No memory leak, even with exception!
```

### Key RAII Principles in Your Code

1. **Resource acquisition in constructor** - Bank initializes with empty vector
2. **Resource release in destructor** - Bank's destructor deletes all Accounts
3. **Exception safe** - Destructor runs even if exception occurs
4. **Deterministic cleanup** - No chance of forgetting to cleanup
5. **Stack allocation** - Bank can be allocated on stack (not just heap)

---

## 🎓 Learning Outcomes

This exercise demonstrates:
1. ✅ Advanced C++98 encapsulation techniques
2. ✅ STL algorithms (std::find_if) over manual loops
3. ✅ Functor patterns for algorithm customization
4. ✅ Exception-based error handling design
5. ✅ Inner classes for implementation hiding
6. ✅ RAII pattern for resource management
7. ✅ const-correctness and references
8. ✅ Delegation pattern for code reuse
9. ✅ Friend relationships for controlled access
10. ✅ Strict C++98 compliance (no C++11+)

---

