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

Dataflow framework and KillGen class diagrams:

<p float="left">
        <img src="https://user-images.githubusercontent.com/18232502/78570355-dfdfd280-77f2-11ea-9e8d-2c86871e5ff0.png"         width="300">
        <img src="https://user-images.githubusercontent.com/18232502/78570595-30573000-77f3-11ea-94db-1e7ad850ddfe.png"         width="300">
</p>

DataflowFramework is a template class and can work on any domain. Each framework instantiation need only operate on one domain type at a time, hence the use of compile-time polymorphism. Currently two meet operators are supported - intersection and union meet. These implement the IMeetOp interface. The BaseTransferFunction is a basic implementation of Gen U (IN/OUT - Kill), subclass and override if more functionality is required. Each analysis pass will also need to implement their own versions of the KillGen class to generate and kill their respective domain sets.

[Full Doxygen Documentation Here](assignment2/SourceCode/Documentation/refman.pdf)

## Assignment 3:
### Dead Code Elimination (DCE) and Loop Invariant Code Motion (LICM):
We will need to utilize the DataflowFramework from the previous assignment to implement a DCE pass using Faint Analysis, a backwards dataflow analysis pass. This will require DepKill and DepGen, we need to figure out how to integrate this into the existing framework.

For LICM, we also need to create a forwards pass for Dominators.
