/////////////////////////////////////////
// Copyright Adan Moran-MacDonald 2018 //
/////////////////////////////////////////

#include "gtest/gtest.h"
#include "common/EigenTypes.h"
#include "polya/polya.h"

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

TEST(PolyaTest, Identity)
{
    /* Build the following Delta matrix:
     1  0
     0  1
    */
    std::vector<Tripleti> deltaOnes=
    {
        {0, 0, 1},
        {1, 1, 1}
    };

    // two-by-two sparse matrix
    DeltaMatrix delta(2);
    delta.setFromTriplets(deltaOnes.begin(), deltaOnes.end());

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

    std::vector<Tripleti> adjOnes =
    {
        {0, 0, 1},
        {1, 1, 1},
        {2, 2, 1},
        {3, 3, 1},
        {4, 4, 1},
        {5, 5, 1},
        {6, 6, 1},
        {7, 7, 1},
        {8, 8, 1}
    };
    
    AdjacencyMatrix adj(9);
    adj.setFromTriplets(adjOnes.begin(), adjOnes.end());

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

    std::vector<Tripleti> urnOnes =
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
    };
    UrnMatrix urn(9,2);
    urn.setFromTriplets(urnOnes.begin(), urnOnes.end());

    // Our urns should be the same
    EXPECT_EQ(
        Eigen::MatrixXi(urn), 
        Eigen::MatrixXi(polya(urn, adj, delta, SamplingType::MEDIAN)));
}

int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/* vim: set ts=4 sw=4 et :*/
