//////////////////////////////////////////
// Copyright Adan Moran-MacDonald, 2018 //
//////////////////////////////////////////

// Std
#include <vector>
#include <iostream>
#include <chrono>
// Eigen
#include <Eigen/Sparse>
#include <Eigen/Dense>
// Qt
#include <QCoreApplication>
#include <QDebug>
// Ours
#include "common/EigenTypes.h"

using SparseMatrix = Eigen::SparseMatrix<int>;
using Dynamic1D = Eigen::VectorXf;
auto helloEigen() -> void
{
    // build a nxm sparse matrix
    int n = 5;
    int m = 5;
    SparseMatrix A(n, m);

    // build a set of triplets
    Triplets<int> elements;
    elements.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        elements.push_back(Triplet<int>(i,i,2));
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
    SquareSparseMatrix<int> d(3);
    std::cout << Eigen::MatrixXi(d) << std::endl;
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
    /*
    helloEigen();
    std::cout << "::::::::::::::" << std::endl;
    std::cout << "Qt------------" << std::endl;
    return helloQt(argc, argv);
    */
    using namespace std::chrono;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    int n = 1048576;
    SparseMatrix A(n,n);
    Eigen::MatrixXi V(n,256);

    Triplets<int> ints;
    ints.reserve(n);
    for(int i =0; i < n; ++i)
    {
        ints.push_back(Triplet<int>(i,i,1));
        for (int j = 0; j < 256; ++j)
        {
            V(i,j) = j;
        }
    }
    A.setFromTriplets(ints.begin(), ints.end());

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double, std::milli> time_span = t2 - t1;
    std::cout << "Creation Time: " << time_span.count() << std::endl;

    double average;
    for(int i = 0; i < 10; ++i)
    {
        t2 = high_resolution_clock::now();
        auto B = A * V;
        t1 = high_resolution_clock::now();
        time_span = t1 - t2;
        average += time_span.count() ;
    }
    average = average / 10.0;
    std::cout << "Multiplication Time: " << average << std::endl;

}

/* vim: set ts=4 sw=4 et :*/
