import numpy as np 
import scipy as sp 
from scipy.sparse import diags
import gurobipy as gp
from gurobipy import GRB

def ip_IOA(A, k):
    """
    Decription
    ----------
    Solve the IOA problem. Formulation see paper.

    Input
    -----
    A: scipy sparse matrix
       The adjacency matrix of the underlying graph G. MUST HAVE ALL ONES ON DIAGOANL

    Output
    ------
    C: numpy binary array
       The types of vertices
    """

    n = np.shape(A)[0] # number of nodes

    deg = np.array([0] * n) # degree of vertices
    for u in range(n):
        deg[u] = A[u].count_nonzero()

    one_row = np.ones(n, dtype=int) # row vector of all ones
    one_column = np.ones((n, 1), dtype=int) # column vector of all ones

    zero_row = np.zeros(n, dtype=int) # row vector of all zeros
    zero_column = np.zeros((n, 1), dtype=int) # column vector of all zeros

    # -------------------------------- #
    #      The model and variables     #
    # -------------------------------- #
    M = gp.Model("IOA")

    # Variables: {0, 1}, column vector
    x = M.addMVar(shape = n, vtype = GRB.BINARY, name = "x")
    y = M.addMVar(shape = n, vtype = GRB.BINARY, name = "y")
    z = M.addMVar(shape = (n, n), vtype = GRB.BINARY, name = "z")

    # ------------------ #
    #      Objective     #
    # ------------------ #
    # max sum(y_i)
    M.setObjective(one_row @ y, GRB.MAXIMIZE)

    # ------------------- #
    #     Constraints     #
    # ------------------- #
    # Constraint 1 & 2: z_{ij} <= 2 - x_i - x_j, z_{ij} <= x_i + x_j
    for i in range(n):
        for j in range(n):
            M.addConstr(z[i,j] <= 2 - x[i] - x[j])
            M.addConstr(z[i,j] <= x[i] + x[j])

    # Constraint 3: y_i = max_{j \in N(i)} {z_{ij}}
    for i in range(n):
        M.addConstr(y[i] <= sum(z[i,j] for j in range(n) if A[i, j] == 1))
        for j in range(n):
            if A[i,j] == 1:
                M.addConstr(y[i] >= z[i,j])

    # Constraint 4: sum{x_i} = n-k
    M.addConstr(one_row @ x == n-k)

    # -------------------- #
    #      Time limit      #
    # -------------------- #
    # M.Params.timeLimit = 200.0

    # -------------------- #
    #     Optimization     #
    # -------------------- #
    M.optimize()

    # ---------------- #
    #      Output      #
    # ---------------- #
    print("Optimal IOA: {}".format(M.objVal))
    return M.objVal
