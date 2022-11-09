import numpy as np 
import scipy as sp 
from scipy.sparse import diags
import gurobipy as gp
from gurobipy import GRB

def ilp(B, alpha, gamma):
    
    m = np.shape(B)[0]

    n = np.shape(B)[1]

    one_row = np.ones(m, dtype=int) # row vector of all ones

    one_column = np.ones((n, 1), dtype=int) # column vector of all ones

    alpha_vec = alpha * one_row

    gamma_vec = gamma * one__column

    # -------------------------------- #
    #      The model and variables     #
    # -------------------------------- #
    M = gp.Model("SOLVE-ILP")

    # Variables: {0, 1}
    Y = M.addMVar(shape = (n, n), vtype = GRB.BINARY, name = "Y")

    # ------------------ #
    #      Objective     #
    # ------------------ #
    M.setObjective(Y.sum(), GRB.MINIMIZE)

    # ------------------- #
    #     Constraints     #
    # ------------------- #

    # Pairwise constratins is a little tricky

    M.addConstr(Y @ one_column <= gamma_vec) 

    M.addConstr(one_row @ Y <= alpha_vec) 

    # -------------------- #
    #      Time limit      #
    # -------------------- #
    #M.Params.timeLimit = 100.0

    # -------------------- #
    #     Optimization     #
    # -------------------- #
    M.optimize()

    # ---------------- #
    #      Output      #
    # ---------------- #
    print("The objective: {}".format(M.objVal))
    return(x.X)
