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
#include "common/EigenHelpers.h"
#include "polya/polya.h"

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

    using tp = high_resolution_clock::time_point;
    using millis = duration<double, std::milli>;


    int n = 1048576;

    tp t1 = high_resolution_clock::now();
    AdjacencyMatrix A = eye(n);
    std::cout << "Adj creation Time: " << millis(high_resolution_clock::now() - t1).count() << std::endl;

    Triplets<int> ints;
    ints.reserve(n);
    for(int i =0; i < n; ++i)
    {
        // cannot have all-zero terms!
        ints.push_back(Triplet<int>(i, i % 256, i % 256+1));
    }

    // create an by 256 urn matrix
    std::cout << "Creating urn matrix" << std::endl;
    tp t2 = high_resolution_clock::now();
    UrnMatrix V = createSparseMatrix(n,256,ints);
    std::cout << "Urn creation Time: " << millis(high_resolution_clock::now() - t2).count() << std::endl;

    // create a 256*256 delta matrix
    DeltaMatrix delta = eye(256);

    duration<double, std::milli> time_span = t2 - t1;

    double average = 0.0;

    // run the polya process however many times and measure the average time
    for(int i = 0; i < 10; ++i)
    {
        std::cout << "iteration " << i << std::endl;
        t2 = high_resolution_clock::now();
        V = polya(V, A, delta, SamplingType::MEDIAN);
        t1 = high_resolution_clock::now();
        time_span = t1 - t2;
        average += time_span.count() ;
    }
    average = average / 10.0;
    std::cout << "Multiplication Time: " << average << std::endl;

}

/* vim: set ts=4 sw=4 et :*/
