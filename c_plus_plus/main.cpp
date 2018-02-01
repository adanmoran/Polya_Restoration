//////////////////////////////////////////
// Copyright Adan Moran-MacDonald, 2018 //
//////////////////////////////////////////

// Std
#include <vector>
#include <iostream>
// Eigen
#include <Eigen/Sparse>
#include <Eigen/Dense>
// Qt
#include <QCoreApplication>
#include <QDebug>

using SparseMatrix = Eigen::SparseMatrix<int>; // declares a column-major sparse matrix type of double
using Triplet = Eigen::Triplet<int>; // declarse a triplet type for doubles
using Triplets = std::vector<Triplet>;
using Dynamic1D = Eigen::VectorXf;

auto helloEigen() -> void
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
        elements.push_back(Triplet(i,i,2));
    }

    A.setFromTriplets(elements.begin(), elements.end());

    Eigen::VectorXi v(n);
    for (int i = 0; i < n; ++i)
    {
        v(i) = i * n;
    }

    std::cout << Eigen::MatrixXi(A) << std::endl;
    Dynamic1D x = Dynamic1D::Ones(A.rows());
    std::cout << x << std::endl;
    std::cout << v << std::endl;
    std::cout << A * v << std::endl;
}

auto helloQt(int argc, char** argv) -> int
{

    QCoreApplication a( argc, argv );

    qDebug() << "Hello World";
    return a.exec();
}

int main( int argc, char **argv )
{
    std::cout << "Eigen:::::" << std::endl;
    helloEigen();
    std::cout << "::::::::::::::" << std::endl;
    std::cout << "Qt------------" << std::endl;
    return helloQt(argc, argv);
}

/* vim: set ts=4 sw=4 et :*/
