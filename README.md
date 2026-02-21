# Piscine Object - Advanced Object-Oriented Programming

A 42 School specialization covering object-oriented design principles, from encapsulation to design patterns.

## Overview

This course progresses through six modules, each building on the previous one, to master software architecture and design:

1. **Module 00** - Encapsulation
2. **Module 01** - Relationships
3. **Module 02** - UML Diagrams
4. **Module 03** - SOLID Principles
5. **Module 04** - Design Patterns
6. **Module 05** - Practical Integration

## Project Structure

```
encapsulation/
├── Module_00_Encapsulation/
│   ├── ex00/                  ✅ Bank system with strict encapsulation
│   │   ├── Account/
│   │   ├── Bank/
│   │   ├── main.cpp
│   │   ├── Makefile
│   │   └── README.md
│   ├── ex00_bonus/            ✅ Advanced bonus requirements
│   │   ├── Account/
│   │   ├── Bank/
│   │   ├── main.cpp
│   │   ├── Makefile
│   │   └── README_BONUS.md
│   └── ex01/                  ⏳ Relationships exercise
├── .gitignore
└── README.md
```

## Module 00: Encapsulation ✅

Implement a properly encapsulated banking system demonstrating:
- Private attributes and controlled access through getters
- Friend classes for selective internal access
- Validation and business logic enforcement
- Proper memory management and cleanup

📖 **[Exercise Documentation](Module_00_Encapsulation/ex00/README.md)**
📖 **[Bonus Documentation](Module_00_Encapsulation/ex00_bonus/README_BONUS.md)**

**Build & Run (ex00):**
```bash
cd Module_00_Encapsulation/ex00/
make              # Compile
./a.out          # Run
make fclean      # Clean
```

**Build & Run (ex00_bonus):**
```bash
cd Module_00_Encapsulation/ex00_bonus/
make              # Compile
./a.out          # Run
make fclean      # Clean
```

## Course Learning Path

### Phase 1: Fundamentals (Modules 00-02)
Learn the core concepts and how to express them:
1. **Encapsulation** → Hide complexity, control access
2. **Relationships** → Connect objects meaningfully
3. **UML** → Visualize software architecture

### Phase 2: Best Practices (Modules 03-04)
Apply industry standards and proven patterns:
4. **SOLID** → Write maintainable, scalable code
5. **Design Patterns** → Solve common problems elegantly

### Phase 3: Integration (Module 05)
Put it all together:
6. **Practical Work** → Real-world application of all concepts

## Language & Standards

- **Language**: C++
- **Standard**: C++98 (with strict compliance)
- **Compiler Flags**: `-Wall -Wextra -Werror -std=c++98`
- **No External Libraries**: Only STL allowed
- **No Boost Libraries**: Pure Standard Library only

## Prerequisites

- Basic C++ knowledge (variables, functions, classes)
- Understanding of pointers and dynamic memory
- Familiarity with Makefiles
- Git version control basics

## Getting Started

```bash
# Navigate to an exercise
cd Module_00_Encapsulation/ex00/

# Build
make

# Run
./a.out

# Clean
make fclean
```


## Notes

- Build artifacts placed in `objects/` directory
- Strict C++98 compliance for maximum portability
- Memory-safe implementations (no leaks)
- Each exercise has its own Makefile and documentation

---

**Repository**: [github.com/niboukha/Piscine_Object](https://github.com/niboukha/Piscine_Object)
