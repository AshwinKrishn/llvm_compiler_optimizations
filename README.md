# LLVM Compiler Optimizations
LLVM Compiler Optimization Course homework repo for SengMing and Ashwin

## Requirements:
LLVM and Clang 6.0.1

## Assignment 1:
### Function Information LLVM Pass:
Analysis pass using a FunctionPass. Spits out the following details about a function:
    
    - Name.
    - Number of arguments.
    - Number of Direct call sites in the same module.
    - Number of basic blocks.
    - Number of instructions.
    - Number of add, sub, mul, div, and branch instructions.

Local Optimization Pass to perform basic-block level optimizations, for example:
1) Algebraic Identities: eg. x + 0 = 0 + x => x
2) Local Binary Expression Constant Folding: eg. 2*4 => 8
3) Strength reduction: eg. 2*x => (x<<1)

## Assignment 2:
### Data flow Analysis framework:
General framework to break-down any unidirectional data-flow problem into well-defined set of components. It should consist of the following:
1) Domain the analysis operates on: eg. All subsets of variable definitions in module.
2) Direction, forwards or backwards analysis pass.
3) Transfer function
4) Meet Operator
5) Boundary Condition
6) Initial Interior Points (Initialization of IN and OUT sets)
