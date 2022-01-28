/*
* Lower triangular solver Lx=b
* L is stored in the compressed column storage format
* Inputs are:
* n : the matrix dimension
* Lp : the column pointer of L
* Li : the row index of L
* Lx : the values of L
* In/Out:
* x : the right hand-side b at start and the solution x at the end.
*/
int lsolve (int n, int* Lp, int* Li, double* Lx, double *x){
    int p, j;
    if (!Lp || !Li || !x) return (0);
    /* check inputs */
    for (j = 0; j < n; j++)
    {
        // solve the current col
        x [j] /= Lx [Lp [j]];

        if(j != (n - 1))
        {
        // for the next row subtract from b all of the values of the cols that we already know
        // for (p = Lp [j]+1; p < Lp [j+1]; p++)
        for (p = Lp[j] + 1; p < Lp [j+1]; p++) 
        {
            // x [Li [p]] -= Lx [p] * x [j];
            x [j + 1] -= Lx [p] * x [p - j - 2];
        }
        } 
    }
    return (1);
}
/*
* Sparse matrix-vector multiply: y = A*x
* A is stored in the compressed column storage format
* Inputs:
* Ap : the column pointer of A
* Ai : the row index of A
* Ax : the values of A
* x : is a dense vector
* Output:
* y : is a dense vector that stores the result of multiplication
*/
int spmv_csc (int n, int size_t, int *Ap, int *Ai, double *Ax, double *x, double *y)
{
    int p, j;
    if (!Ap || !x || !y) return (0);
    /* check inputs */
    for (j = 0; j < n; j++){
        for (p = Ap [j]; p < Ap [j+1]; p++)
        {
            y [Ai [p]] += Ax [p] * x [j];
        }
    }
    return (1);
}
