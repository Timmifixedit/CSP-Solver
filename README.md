# CSP-Solver
Project for the lecture "Objektorientierte Programmierung mit C++" at Ulm University. Solves arbitrary constraint satisfaction problems (CSP) using the AC3 algorithm
and backtracking search.

## How To
A CPS consists of a set of Variables, represented by `csp::Variable` and a set of constraints (dependencies between pairs of varaibles) represented by `csp::Constraint`
or `csp::Arc`. You can use your own variable type by deriving from `csp::Variable`.

### Creating Variables
To create a varaible, a domain of values has to be given, representing all possible
values the variable might take:
```cpp
#include "Variable.h"
using MyVar = csp::Varialbe<int>;
MyVar a({1, 2, 17, 24});
```
Also possible: Create your own type:
```cpp
#include <string>
#include "Variable.h"
class MyVar : public csp::Varaible<int> {
public:
  explicit MyVar(std::string name) : csp::Variable<int>({1, 2, 3, 4}), name(std::move(name)) {}
  const std::string name;
}
```
### Specifying constraints
`csp::Constraint` or `csp::Arc` specify dependencies between pairs of variables. They contain a pointer type to each varaible and a binary predicate specifying the
constraint. Example using the custom varaiable type above:
```cpp
#include <memory>
#include "Arc.h"
auto varA = std::make_shared<MyVar>("A");
auto varB = std::make_shared<MyVar>("B");
csp::Constraint aLessB(varA, varB, std::less<>());
```
You can also use `csp::Arc` to specify the relation between variables. The difference is, that an arc describes a directed constraint. Even if A < B is equivalent
to B > A, two arcs describing both relations respectively are not. When defining your CPS using arcs, make sure that always both directions are specified explicitly.
When using `csp::Constraint` only one direction suffices. In some situations, it is easier to specify the CSP using arcs than using constraints or vice versa.





