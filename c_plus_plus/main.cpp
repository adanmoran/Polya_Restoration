//////////////////////////////////////////
// Copyright Adan Moran-MacDonald, 2018 //
//////////////////////////////////////////

#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <vector>
#include <iostream>

typedef Eigen::SparseMatrix<int> SparseMatrix; // declares a column-major sparse matrix type of double
typedef Eigen::Triplet<int> Triplet; // declarse a triplet type for doubles
typedef std::vector<Triplet> Triplets;
int main()
{
    // build a nxm sparse matrix
    int n = 5;
    int m = 5;
    SparseMatrix A(n, m);

    // build a set of triplets
    Triplets elements;
    elements.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        elements.push_back(Triplet(i,i,1));
    }

    A.setFromTriplets(elements.begin(), elements.end());

    std::cout << Eigen::MatrixXi(A) << std::endl;

	return 0;
}

/* vim: set ts=4 sw=4 et :*/
