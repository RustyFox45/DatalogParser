# Datalog Parser & Relational Database Engine

A C++ implementation of a lexical analyzer, recursive-descent parser, relational database engine, and fixed-point Datalog rule evaluator. Designed to ingest raw Datalog source files, validate syntax against a context-free grammar, materialize relational schemas and facts, evaluate recursive inference rules, and execute relational algebra queries.

## Architecture & Execution Pipeline

The engine processes Datalog programs through a four-stage compilation and execution pipeline:

```
┌──────────────┐     ┌──────────────┐     ┌────────────────────┐     ┌────────────────────┐
│  Input File  │ ──► │   Scanner    │ ──► │       Parser       │ ──► │  Database Engine   │
│  (.txt/.dl)  │     │ (Lexer/DFA)  │     │ (Recursive Descent)│     │ (Fixed-Point Eval) │
└──────────────┘     └──────────────┘     └────────────────────┘     └────────────────────┘
                            │                       │                          │
                            ▼                       ▼                          ▼
                     Token Stream           DatalogProgram AST         Relational Algebra
                                         (Schemes, Facts, Rules,     (Select, Project, Join,
                                                Queries)              Rename, Union, Eval)
```

### 1. Lexical Analysis (`Scanner.h`, `Token.h`)
* **Tokenization:** Scans input character streams into typed lexical tokens (`SCHEMES`, `FACTS`, `RULES`, `QUERIES`, `ID`, `STRING`, `COLON_DASH`, `COMMA`, `PERIOD`, `Q_MARK`, `LEFT_PAREN`, `RIGHT_PAREN`, `ADD`, `MULTIPLY`, `COMMENT`, `UNDEFINED`, and `EOF_TYPE`).
* **Deterministic Finite Automata (DFA):** Recognizes multi-line block comments (`#| ... |#`), single-line comments (`# ...`), escaped single-quoted string literals (`'...'`), and alphanumeric identifiers while tracking exact line numbers for syntax error reporting.

### 2. Syntactic Analysis (`Parser.h`, `DatalogProgram.h`)
* **Recursive-Descent Parsing:** Validates token sequences against the Datalog Context-Free Grammar (CFG). Upon encountering an unexpected token, throws the offending `Token` to report exact line-level syntax failures.
* **AST & Domain Construction:** Populates a `DatalogProgram` structure containing:
  * **Schemes:** Relation headers (`Predicate` definitions).
  * **Facts:** Grounded atomic tuples (`Predicate` instances) and an extracted set of domain constants (`std::set<std::string>`).
  * **Rules:** Horn clauses ($H \leftarrow B_1, B_2, \dots, B_n$) represented via `Rule` objects pairing a head `Predicate` with a body `std::vector<Predicate>`.
  * **Queries:** Target predicates (`Query`) to be evaluated against the derived database state.

### 3. Relational Database & Algebra (`Database.h`, `Relation.h`, `Scheme.h`, `Tuple.h`)
* **Set-Based Storage:** Models each `Relation` as a `Scheme` (ordered attribute vector) paired with a `std::set<Tuple>` to enforce relational set semantics (automatic deduplication and deterministic lexicographical ordering).
* **Relational Algebra Operators:**
  * **Selection ($\sigma$):** Supports constant-value matching (`select(index, value)`) and column-to-column equality constraints (`select(index1, index2)`) for repeated variables within a single predicate.
  * **Projection ($\pi$):** Extracts and reorders specified attribute columns (`project(indices)`).
  * **Renaming ($\rho$):** Rebinds relation attributes to query/rule variable names (`rename(newScheme)`).
  * **Natural Join ($\bowtie$):** Computes common join attributes across two relations (`join(other)`), filters compatible tuples, and concatenates non-duplicate attributes.
  * **Union ($\cup$):** Merges newly inferred tuples into existing relations while tracking cardinality changes (`unionWith(other)`).

### 4. Fixed-Point Rule & Query Evaluation (`Database.h`)
* **Naive Fixed-Point Iteration:** Evaluates all Datalog `Rules` iteratively until a convergence pass adds zero new tuples across all relations in the `Database`, reporting the total number of passes required to reach the least fixed point.
* **Query Execution:** Evaluates each `Query` by applying selections, projections, and renamings to the target relation, outputting either `No` or `Yes(n)` followed by distinct variable bindings.

## Project Structure

```text
DatalogParser-master/
├── CMakeLists.txt          # CMake build configuration (C++14 / C++17)
├── makefile                # Alternative GNU Make build targets
├── main.cpp                # CLI entry point orchestrating Scanner, Parser, and Database
├── Token.h                 # TokenType enum and Token representation
├── Scanner.h               # Lexical analyzer and state-machine automata
├── Parser.h                # Recursive-descent grammar parser
├── DatalogProgram.h        # Container for Schemes, Facts, Rules, Queries, and Domain
├── Predicate.h             # Predicate structure (name + parameter list)
├── Parameter.h             # Atomic parameter (constant string or variable ID)
├── Rule.h                  # Horn clause representation (Head :- Body)
├── Query.h                 # Query wrapper around Predicate
├── Scheme.h                # Relation attribute header definition
├── Tuple.h                 # Tuple value vector with strict weak ordering
├── Relation.h              # Relation container and relational algebra operations
├── Database.h              # Schema instantiation, fixed-point rule evaluator, and query runner
├── StringUtil.h            # String formatting helpers
├── Utils.h                 # General utility functions
└── project4-passoff/       # Automated test suite (inputs & expected outputs across 20%–100% tiers)
```

## Build & Execution

### Prerequisites
* **C++ Compiler:** `g++` or `clang++` supporting C++14 or newer
* **Build System:** `CMake` (v3.10+) or `make`

### Building with CMake
```bash
mkdir -p build && cd build
cmake ..
cmake --build .
```

### Building with Make
```bash
make
```

### Running the Engine
Pass a Datalog source file as a command-line argument:
```bash
./DatalogParser path/to/input.txt
```

### Running the Passoff Test Suite
The `project4-passoff/` directory includes categorized test cases (`20/`, `40/`, `60/`, `80/`, `100/`) containing `input*.txt` and expected `answer*.txt` files:
```bash
./DatalogParser project4-passoff/100/input101.txt | diff - project4-passoff/100/answer101.txt
```
