bool cspSolver(CSP &problem) {
    if (problem.isSolved()) {
        return true;
    }

    auto &currentVariable = problem.getNextUnassignedVariable();
    for (auto value : currentVariable.domain) {
        currentVariable.assign(value);
        if (!obtainArcConsistency(problem)) {
            currentVariable.removeAssignment();
            continue;
        }

        if (cspSolver(problem)) {
            return true;
        }

        currentVariable.removeAssignment();
    }

    return false;
}