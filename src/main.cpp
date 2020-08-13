#include <iostream>
#include <memory>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <optional>

#include "Variable.h"
#include "Arc.h"
#include "Csp.h"
#include "solver.h"

class SudokuNode : public csp::Variable<unsigned int> {
public:
    using Domain = csp::Variable<unsigned int>::DomainT;
    explicit SudokuNode(unsigned int val) : csp::Variable<unsigned int>(
            val == 0 ? Domain{1, 2, 3, 4, 5, 6, 7, 8, 9} : Domain{val}) {
        assert(val <= 9);
    }

    [[nodiscard]] auto getValue() const -> std::optional<unsigned int> {
        if (this->isAssigned()) {
            return this->valueDomain().front();
        }

        return {};
    }
};

class Sudoku {
public:
    explicit Sudoku(std::istream &input) {
        int index = 0;
        unsigned int val = 0;
        while (input >> val) {
            assert(val <= 9);
            fields[index] = std::make_shared<SudokuNode>(val);
            ++index;
        }
    }

    static auto linearToCoordinates(unsigned int index) -> std::pair<unsigned int, unsigned int >{
        assert(index < 81);
        return {index % 9, index / 9};
    }

    static auto coordinatesToLinear(unsigned int x, unsigned int y) -> unsigned int {
        assert(x < 9 && y < 9);
        return 9 * y + x;
    }

    void print(std::ostream &out) const {
        bool horLine = true;
        for (std::size_t index = 0; index < fields.size(); ++index) {
            const auto &field = fields[index];
            auto [x, y] = linearToCoordinates(index);
            out << std::setw(2);
            if (horLine && y % 3 == 0) {
                for (unsigned int i = 0; i < 9; ++i) {
                    out << std::setw(3) <<  "-";
                }

                out << std::endl;
                horLine = false;
            } else if (y % 3 != 0) {
                horLine = true;
            }

            if(x % 3 == 0) {
                out << "| ";
            }

            if (field->isAssigned()) {
                out << *field->getValue();
            } else {
                out << "?";
            }

            if(x == 9 - 1) {
                out << " |" << std::endl;
            } else {
                out << " ";
            }
        }

        for (unsigned int i = 0; i < 9; ++i) {
            out << std::setw(3) <<  "-";
        }

        out << std::endl;
    }

    bool solve() {
        std::vector<csp::Arc<std::shared_ptr<SudokuNode>>> arcs;
        arcs.reserve(81 * 20);
        for (std::size_t index = 0; index < fields.size(); ++index) {
            auto neighbours = getNeighbours(index);
            for (auto &nb : neighbours) {
                arcs.emplace_back(fields[index], std::move(nb), std::not_equal_to<>());
            }
        }

        auto sudokuProblem = csp::make_csp(fields, arcs);
        return csp::solve(sudokuProblem, csp::strategies::First<std::shared_ptr<SudokuNode>>());
    }

private:
    [[nodiscard]] auto getNeighbours(unsigned int index) const -> std::array<std::shared_ptr<SudokuNode>, 20> {
        auto [col, row] = linearToCoordinates(index);
        std::array<std::shared_ptr<SudokuNode>, 20> ret;
        auto it = ret.begin();
        for (unsigned int i = 0; i < 9; ++i) {
            if (i != col) {
                *it = fields[coordinatesToLinear(i, row)];
                ++it;
            }

            if (i != row) {
                *it = fields[coordinatesToLinear(col, i)];
                ++it;
            }
        }

        unsigned int blockNumber = 3 * (row / 3) + col / 3;
        unsigned int colStart = (blockNumber % 3) * 3;
        unsigned int rowStart = (blockNumber / 3) * 3;
        for(unsigned int y = rowStart; y < rowStart + 3; y++){
            for(unsigned int x = colStart; x < colStart + 3; x++){
                if(x != col && y != row){
                    *it = fields[coordinatesToLinear(x, y)];
                    ++it;
                }
            }
        }

        assert(it == ret.end());
        return ret;
    }

    std::array<std::shared_ptr<SudokuNode>, 81> fields;
};

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Please input the path to the sudoku file" << std::endl;
        std::exit(1);
    }

    std::ifstream sudokuFile(argv[1]);
    if (!sudokuFile) {
        std::cerr << "Failed to open file '" << argv[1] << "'" << std::endl;
        std::exit(1);
    }

    Sudoku sudoku(sudokuFile);
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
