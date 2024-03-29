[![unit tests](https://github.com/Timmifixedit/CSP-Solver/actions/workflows/unit_tests.yml/badge.svg)](https://github.com/Timmifixedit/CSP-solver/actions/workflows/unit_tests.yml)
# CSP-Solver
Project for the lecture "Objektorientierte Programmierung mit C++" at Ulm University. Solves arbitrary binary constraint
satisfaction problems (CSP) using the [AC-3 algorithm](https://en.wikipedia.org/wiki/AC-3_algorithm) and backtracking
search. The seminar paper (German) for this project can be found [here](https://timmifixedit.github.io/CSP-Solver/Report/report.pdf).

## Doxygen Documentation
* [HTML](https://timmifixedit.github.io/CSP-Solver/docs/html/index.html)
* [PDF](https://timmifixedit.github.io/CSP-Solver/docs/CSPSolverDocs.pdf)

## How To
A CPS consists of a set of variables, represented by `csp::Variable` and a set of constraints (dependencies between
pairs of variables) represented by `csp::Constraint` or `csp::Arc`. You can use your own variable type by deriving from
`csp::Variable`.

### Creating Variables
To create a variable, a domain of values has to be given, representing all possible
values the variable might take:
```cpp
#include "csp_solver.h"
csp::Variable a{1, 2, 17, 24};
```
A variable can be defined on any domain type. Also, you can change the underlying container used for the domain:
```cpp
#include "csp_solver.h"
#include <vector>
using MyVar = csp::Variable<double, std::vector>;
MyVar a{0.2, 4, 1.7};
```
The default container used as domain storage is `std::list`. This is due to the fact that during solving, values need to
be erased from the domain. When your variables have domains with many values, a data storage that supports efficient
removal from any position might be advantageous. When your domains only contain a few values (and / or when your values
can be copied / moved efficiently), a container like `std::vector` or `std::deque` which supports efficient iteration
might be the better choice.

Furthermore, you can create your own variable type. This allows you to add functionality to your variable type. For
example, you can add a variable name:
```cpp
#include <string>
#include "csp_solver.h"
class MyVar : public csp::Variable<int> {
public:
  explicit MyVar(std::string name) : csp::Variable<int>{1, 2, 3, 4}, name(std::move(name)) {}
  const std::string name;
};
```
### Specifying Constraints
`csp::Constraint` or `csp::Arc` specify dependencies between pairs of variables. They contain a pointer type to each
variable and a binary predicate specifying the constraint. You can use arbitrary pointer types that support the
dereference-operator as well as -> operator. Example using the custom variable type above and shared_ptr (which I
recommend over raw pointers):
```cpp
#include <memory>
#include "csp_solver.h"
auto varA = std::make_shared<MyVar>("A");
auto varB = std::make_shared<MyVar>("B");
csp::Constraint aLessB(varA, varB, std::less<>());
```
You can also use `csp::Arc` to specify the relation between variables. The difference is, that an arc describes a
directed constraint. Even if A < B is equivalent to B > A, two arcs describing both relations respectively are not. When
defining your CSP using arcs, make sure that always both directions are specified explicitly. When using
`csp::Constraint` only one direction suffices. In some situations, it is easier to specify the CSP using arcs than using
constraints or vice versa. You cannot mix arcs and constraints when creating a CSP but you can convert a
`csp::Constraint` to two equivalent `csp::Arc`.

### Creating the CSP
Once you specified all variables and the respective constraints, create your CSP using:
```cpp
csp::Csp myCsp = csp::make_csp(std::array{varA, varB}, std::array{aLessB});
```
You can use arbitrary containers that support iteration.

### Solving the CSP
An instance of `csp::Csp` can be solved using:
```cpp
bool success = csp::solve(myCsp);
```
If solving the CSP is possible all domains of all variables will be reduced to exactly one value. The algorithm will
find a solution if one exists (given engough time). If multiple exist, it is unspecified which exact solution is found.
If the CSP cannot be solved, the function returns `false` but might still modify the variables' value domains.
#### Specifying a Solving Strategy
By default, `csp::solve` uses the minimum remaining values strategy, meaning that the algorithm chooses the variable
with the fewest remaining values in its domain to be assigned next. You can also use a different (even custom) strategy
e.g.:
```cpp
auto strat = [](const auto & problem) {
  // Your code here -> return the desired unassigned variable from the CSP
  return theNextVar;
};

bool success = csp::solve(myCsp, strat);
```
Examples on how to create your own strategy can be found in the file `src/strategies.h`.

## Solving Sudoku Puzzles
The `src/main.cpp` contains a program that can solve Sudoku puzzles. A Sudoku is defined by a grid of numbers where a 0
indicates, that the respective field is yet to be assigned. Some examples are provided in the `res` directory.
