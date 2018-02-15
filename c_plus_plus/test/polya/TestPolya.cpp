/////////////////////////////////////////
// Copyright Adan Moran-MacDonald 2018 //
/////////////////////////////////////////

#include "gtest/gtest.h"
#include "common/EigenTypes.h"
#include "polya/polya.h"
#include "common/EigenHelpers.h"

using Tripleti = Triplet<int>;

class PolyaTest : public ::testing::Test
{
protected:
    // Code that runs in every test goes here
    virtual void SetUp()
    {}

    // Code that tears down every test goes here
    virtual void TearDown()
    {}

};

TEST_F(PolyaTest, Identity)
{
    /* Build the following Delta matrix:
     1  0
     0  1
    */
    DeltaMatrix delta = eye(2);

    /* Build the following adjacency matrix:
     1 0 0 0 0 0 0 0 0
     0 1 0 0 0 0 0 0 0
     0 0 1 0 0 0 0 0 0
     0 0 0 1 0 0 0 0 0
     0 0 0 0 1 0 0 0 0
     0 0 0 0 0 1 0 0 0
     0 0 0 0 0 0 1 0 0
     0 0 0 0 0 0 0 1 0
     0 0 0 0 0 0 0 0 1

     which is the adjacency matrix for a 3x3 image with no
     connections.
     */
    AdjacencyMatrix adj = eye(9);
        
    /* Build the following urn matrix in black and white:
      1 0
      0 1
      1 0
      0 1
      0 1
      1 0
      0 1
      1 0
      1 0
    */
    UrnMatrix urn = createSparseMatrix(9,2, Triplets<int>(
    {
        {0,0,1},
        {1,1,1},
        {2,0,1},
        {3,1,1},
        {4,1,1},
        {5,0,1},
        {6,1,1},
        {7,0,1},
        {8,0,1},
    }));

    auto expected = Eigen::MatrixXi(urn);
    expected *= 2;
    auto actual = Eigen::MatrixXi(polya(urn, adj, delta, SamplingType::MEDIAN));
         
    // Our urns should be the same
    EXPECT_EQ(expected,actual) <<
        "Expected \n" << expected << "\n, got \n" << actual;
}

TEST_F(PolyaTest, SmallMatrix)
{
    using Eigen::MatrixXi;

    // 3 ball types
    DeltaMatrix delta = eye(3);

    // 2x2 image with 1-norm and 1-radius
    // Adjacency = 
    // 1 1 1 0
    // 1 1 0 1
    // 1 0 1 1
    // 0 1 1 1
    
    MatrixXi preAdj(4,4);
    preAdj << 
        1,1,1,0,
        1,1,0,1,
        1,0,1,1,
        0,1,1,1;
    AdjacencyMatrix adj = Eigen::SparseMatrix<int>(preAdj.sparseView());

    // UrnMatrix to start:
    // 1 0 0 
    // 0 1 0
    // 0 0 1
    // 0 1 0
    UrnMatrix urn = createSparseMatrix(4,3,Triplets<int>(
    {
        {0,0,1},
        {1,1,1},
        {2,2,1},
        {3,1,1}
    }
    ));
    
    // Expected superurn for first run:
    // 1 1 1
    // 1 2 0
    // 1 1 1
    // 0 2 1
    // so the expected output after one run is
    MatrixXi expected1(4,3);
    expected1 <<
       1, 1, 0,
       0, 2, 0,
       0, 1, 1,
       0, 2, 0;

    auto actual1 = Eigen::MatrixXi(polya(urn, adj, delta, SamplingType::MEDIAN));
    EXPECT_EQ(expected1, actual1) <<
        "Expected \n" << expected1 << "\n and got \n" << actual1;
       
    // Expected superurn for second run:
    // 1 4 1
    // 1 5 1
    // 1 4 1
    // 0 5 1
    // again, the expected output is to add to index 2
    // 1 2 0
    // 0 3 0
    // 0 2 1
    // 0 3 0
    MatrixXi expected2(4,3);
    expected2 <<
        1,2,0,
        0,3,0,
        0,2,1,
        0,3,0;

    UrnMatrix urn1 = Eigen::SparseMatrix<int>(actual1.sparseView());

    auto actual2 = Eigen::MatrixXi(polya(urn1, adj, delta, SamplingType::MEDIAN));
    EXPECT_EQ(expected2, actual2) <<
        "Expected \n" << expected2 << "\n and got \n" << actual2;
    
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/* vim: set ts=4 sw=4 et :*/
