//////////////////////////////////////////
// Copyright Adan Moran-MacDonald, 2018 //
//////////////////////////////////////////

#include <Eigen/Sparse>

typedef Eigen::SparseMatrix<double> SparseMatrix; // declares a column-major sparse matrix type of double
int main()
{
    // build a nxm sparse matrix
    int n = 5;
    int m = 5;
    SparseMatrix A(n, m);
	return 0;
}

/* vim: set ts=4 sw=4 et :*/
