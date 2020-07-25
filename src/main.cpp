#include <iostream>
#include <memory>
#include <cassert>
#include <utility>
#include <fstream>
#include <iomanip>
#include <chrono>

#include "Variable.h"
#include "Arc.h"
#include "Csp.h"
#include "solver.h"

template <unsigned int N>
class SudokuNode : public csp::Variable<unsigned int> {
public:
    SudokuNode(unsigned int val, unsigned int x, unsigned int y) : csp::Variable<unsigned int>(
            val == 0 ? std::list<unsigned int>{1, 2, 3, 4, 5, 6, 7 ,8 ,9} : std::list{val}), x(x), y(y) {
        assert(val <= N);
    }

    const unsigned int x, y;
};

template<unsigned int N, unsigned int SQRT>
class Sudoku {
public:
    explicit Sudoku(std::istream &input) {
        int index = 0;
        unsigned int val = 0;
        while (input >> val) {
            assert(val <= N);
            auto [x, y] = linearToCoordinates(index);
            fields[index] = std::make_shared<SudokuNode<N>>(val, x, y);
            ++index;
        }
    }

    static auto linearToCoordinates(unsigned int index) -> std::pair<unsigned int, unsigned int >{
        assert(index < N * N);
        return {index % N, index / N};
    }

    static auto coordinatesToLinear(unsigned int x, unsigned int y) -> unsigned int {
        assert(x < N && y < N);
        return N * y + x;
    }

    void print(std::ostream &out) const {
        bool horLine = true;
        for (std::size_t index = 0; index < fields.size(); ++index) {
            const auto &field = fields[index];
            auto [x, y] = linearToCoordinates(index);
            out << std::setw(2);
            if (horLine && y % SQRT == 0) {
                for (unsigned int i = 0; i < N; ++i) {
                    out << std::setw(3) <<  "-";
                }

                out << std::endl;
                horLine = false;
            } else if (y % SQRT != 0) {
                horLine = true;
            }

            if(x % SQRT == 0) {
                out << "| ";
            }

            if (field->isAssigned()) {
                out << field->valueDomain().front();
            } else {
                out << "?";
//                for (auto val : field->valueDomain()) {
//                    out << val;
//                }
//                out << ")";
            }

            if(x == N - 1) {
                out << " |" << std::endl;
            } else {
                out << " ";
            }
        }

        for (unsigned int i = 0; i < N; ++i) {
            out << std::setw(3) <<  "-";
        }

        out << std::endl;
    }

    bool solve() {
        auto inequal = [](unsigned int lhs, unsigned int rhs) {return lhs != rhs;};
        std::list<csp::Arc<unsigned int>> arcs;
        for (std::size_t index = 0; index < fields.size(); ++index) {
            auto neighbours = getNeighbours(index);
            for (const auto &nb : neighbours) {
                arcs.emplace_back(fields[index], nb, inequal);
            }
        }

        auto sudokuProblem = csp::make_csp(fields, arcs);
//        csp::util::ac3(sudokuProblem);
//        print(std::cout);
        return csp::solve(sudokuProblem);
    }

private:
    [[nodiscard]] auto getNeighbours(unsigned int index) const -> std::array<std::shared_ptr<SudokuNode<N>>, 2 * (N - 1) + N - (2 * SQRT - 1)> {
        auto [col, row] = linearToCoordinates(index);
        std::array<std::shared_ptr<SudokuNode<N>>, 2 * (N - 1) + N - (2 * SQRT - 1)> ret;
        auto it = ret.begin();
        for (unsigned int i = 0; i < N; ++i) {
            if (i != col) {
                *it = fields[coordinatesToLinear(i, row)];
                ++it;
            }

            if (i != row) {
                *it = fields[coordinatesToLinear(col, i)];
                ++it;
            }
        }

        unsigned int blockNumber = SQRT * (row / SQRT) + col / SQRT;
        unsigned int colStart = (blockNumber % SQRT) * SQRT;
        unsigned int rowStart = (blockNumber / SQRT) * SQRT;
        for(unsigned int y = rowStart; y < rowStart + SQRT; y++){
            for(unsigned int x = colStart; x < colStart + SQRT; x++){
                if(x != col && y != row){
                    *it = fields[coordinatesToLinear(x, y)];
                    ++it;
                }
            }
        }

        assert(it == ret.end());
        return ret;
    }
    std::array<std::shared_ptr<SudokuNode<N>>, N * N> fields;
};


int main() {
    std::ifstream sudokuFile("../res/VeryHardSudoku.txt");
    if (!sudokuFile) {
        std::cerr << "Failed to open file" << std::endl;
        std::exit(1);
    }

    Sudoku<9, 3> sudoku(sudokuFile);
    sudoku.print(std::cout);

    auto start = std::chrono::high_resolution_clock::now();
    if (sudoku.solve()) {
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Successfully solved sudoku in " << duration.count() << "ms" << std::endl;
        sudoku.print(std::cout);
    } else {
        std::cout << "Sudoku cannot be solved" << std::endl;
    }
}
