bool cspSolver(CSP &problem) {
    if (problem.isSolved()) {
        return true;
    }

    auto &currentVariable = problem.getNextUnassignedVariable();
    for (auto value : currentVariable.domain) {
        currentVariable.assign(value);
        bool success = false;
        if (currentVariable.isConsistent()) {
            success = cspSolver(problem);
        }

        if (success) {
            return true;
        }

        currentVariable.removeAssignment();
    }

    return false;
}