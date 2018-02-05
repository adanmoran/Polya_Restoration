//////////////////////////////////////////
// Copyright Adan Moran-MacDonald, 2018 //
//////////////////////////////////////////

#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <Eigen/Sparse>
#include "common/EigenTypes.h"
#include "common/EigenHelpers.h"

using Eigen::SparseMatrix;
using Eigen::MatrixXi;
using Eigen::Triplet;

class EigenTest : public ::testing::Test
{
protected:

    EigenTest()
    : mat_(4,4)
    {}

    virtual void SetUp()
    {}

    virtual void TearDown()
    {}

    // A matrix for comparing against
    SparseMatrix<int> mat_;
};

TEST_F(EigenTest, CreateEmptySquareMatrix)
{
    // Empty matrices should match
    EXPECT_EQ(
        MatrixXi(mat_),
        MatrixXi(createSquareMatrix(4,Triplets<int>()))
    );

    // Empty matrices of the wrong size cannot match because
    // eigen aborts the program if the dimensions are wrong
}

TEST_F(EigenTest, CreateIdentitySquareMatrix)
{

    // Identity generated with triplets a-priori should match
    Triplets<int> elements =
    {
        {0, 0, 1},
        {1, 1, 1},
        {2, 2, 1},
        {3, 3, 1}
    };

    mat_.setFromTriplets(elements.begin(), elements.end());

    ASSERT_TRUE(elements.size() > 0);

    EXPECT_EQ(
        MatrixXi(mat_),
        MatrixXi(createSquareMatrix(4,elements))
    );

    // Identity generated from inside
    EXPECT_EQ(
        MatrixXi(mat_),
        MatrixXi(createSquareMatrix(4,Triplets<int>(
            {
                {0,0,1},
                {1,1,1},
                {2,2,1},
                {3,3,1}
            }))
        )
    );
}

TEST_F(EigenTest, CreateNonIdentitySquareMatrix)
{
	// Create 
	// 1 2 3
	// 4 5 6
	// 7 8 9
	Triplets<int> elements =
	{
		{0, 0, 1},
		{0, 1, 2},
		{0, 2, 3},
		{1, 0, 4},
		{1, 1, 5},
		{1, 2, 6},
		{2, 0, 7},
		{2, 1, 8},
		{2, 2, 9}
	};

	auto square = createSquareMatrix(3, elements);

	Eigen::MatrixXi ans(3,3);
	ans(0,0) = 1;
	ans(0,1) = 2;
	ans(0,2) = 3;
	ans(1,0) = 4;
	ans(1,1) = 5;
	ans(1,2) = 6;
	ans(2,0) = 7;
	ans(2,1) = 8;
	ans(2,2) = 9;

	EXPECT_EQ(MatrixXi(square), ans) <<
		"Created matrix " << MatrixXi(square) << " did not match " << ans;
}

TEST_F(EigenTest, eyeNonEmpty)
{
	// matrix has non-zero elements along all diagonals
	mat_ = eye(10);
	ASSERT_GT(mat_.innerSize(),0);
	ASSERT_GT(mat_.outerSize(),0);
}

TEST_F(EigenTest, eyeDiagonalsAreOne)
{
	mat_ = eye(10);

	ASSERT_EQ(mat_.size(), mat_.rows() * mat_.cols());

	for(int i = 0; i < mat_.rows(); ++i)
	{
		EXPECT_EQ(mat_.coeffRef(i,i),1);
	}
}

TEST_F(EigenTest, eyeOnlyDiagonalsAreOne)
{
	mat_ = eye(10);

	for(int i = 0; i < mat_.rows(); ++i)
	{
		for(int j = 0; j < mat_.cols(); ++j)
		{
			if (i == j)
			{
				EXPECT_EQ(mat_.coeffRef(i,j),1);
			}
			else
			{
				EXPECT_EQ(mat_.coeffRef(i,j),0);
			}
		}
	}
}

TEST_F(EigenTest, RowCumSumBasics)
{
	// Returned vector should be of the same number of rows as square matrix
	ASSERT_EQ(cumsum(mat_).size(), mat_.rows());
		
	// Returned vector should be number of rows as non-square matrix
	ASSERT_EQ(cumsum(SparseMatrix<int>(3,4)).size(), 3);
	
	// Returned vector of empty sparse matrix should be zero
	auto vect = cumsum(mat_);
	for(int i = 0; i < vect.size(); ++i)
	{
		EXPECT_EQ(vect(i),0);
	}
}

TEST_F(EigenTest, RowCumSumContents)
{
	//TODO: Test cumsum for square identity matrix
	
	//TODO: Test cumsum for square non-identity matrix
	
	//TODO: Test cumsum for non-square matrix
	
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/* vim: set ts=4 sw=4 :*/
