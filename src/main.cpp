#include <iostream>

#include "Variable.h"
#include "solver.h"


class SudokuNode : public csp::Variable<int, SudokuNode> {
public:
    SudokuNode(int x, int y) : csp::Variable<int, SudokuNode>({1, 2, 3, 4, 5, 6, 7, 8, 9}), x(x), y(y) {}

    [[nodiscard]] bool assignmentValid(const int &val) const {
        return val != 4;
    }

private:
    int x, y;
};


int main() {
    SudokuNode s1(0, 2);
    SudokuNode s2(1, 1);
    csp::Variable<int, SudokuNode> &tmp = s2;
    std::cout << tmp.isAssigned();
    tmp.assign(2);
    std::cout << tmp.isAssigned();
    std::cout << s2.isAssigned();
    //csp::removeInconsistent(s1, s2);
    return 0;
}
