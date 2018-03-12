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
	// Returned matrix should be of the same number of rows as square matrix
	ASSERT_EQ(cumsum(mat_).rows(), mat_.rows());
	ASSERT_EQ(cumsum(mat_).cols(), mat_.cols());
		
	// Returned vector should be number of rows as non-square matrix
	ASSERT_EQ(cumsum(SparseMatrix<int>(3,4)).rows(), 3);
	ASSERT_EQ(cumsum(SparseMatrix<int>(3,4)).cols(), 4);
	
	// Returned vector of empty sparse matrix should be zero
	auto c = cumsum(mat_);
	for(int i = 0; i < c.rows(); ++i)
	{
		for(int j = 0; j < c.cols(); ++j)
		{
			EXPECT_EQ(c.coeff(i,j),0);
		}
	}
}

TEST_F(EigenTest, RowCumSumContents)
{

	// Test cumsum for square identity matrix
	auto c_eye = cumsum(eye(3));
	ASSERT_EQ(c_eye.rows(), 3);
	ASSERT_EQ(c_eye.cols(), 3);

	EXPECT_EQ(MatrixXi(c_eye), MatrixXi(eye(3)));

	// Test cumsum for square non-identity matrix
	MatrixXi nonId(3,3);
	nonId << 
		1,2,3,
		4,5,6,
		7,8,9;

	MatrixXi nonIdSum(3,3);
	nonIdSum << 
		1,3,6,
		4,9,15,
		7,15,24;

	Eigen::SparseMatrix<int> spNonId = nonId.sparseView();
	auto cNonId = MatrixXi(cumsum(spNonId));
	EXPECT_EQ(cNonId,nonIdSum) <<
		"cumsum:\n" << cNonId << "\n actual: \n" << nonIdSum;
	
	//TODO: Test cumsum for non-square matrix
	MatrixXi nonSquare(4,2);
	nonSquare << 
		1,2,
		3,4,
		5,6,
		7,8;
	MatrixXi nonSquareSum(4,2);
	nonSquareSum <<
		1,3,
		3,7,
		5,11,
		7,15;
	
	Eigen::SparseMatrix<int> spNonSquare = nonSquare.sparseView();
	auto cNonSquare = MatrixXi(cumsum(spNonSquare));
	EXPECT_EQ(cNonSquare,nonSquareSum) <<
		"cumsum:\n" << cNonSquare << "\n actual: \n" << nonSquareSum;
	
}

TEST_F(EigenTest, cumsumFindSizes)
{
	using Eigen::VectorXi;
	ASSERT_EQ(cumsumFind(createSquareMatrix(4,Triplets<int>()),VectorXi(4)).rows(),4);
	ASSERT_EQ(cumsumFind(createSquareMatrix(1,Triplets<int>()),VectorXi(2)).rows(),2);
	ASSERT_EQ(cumsumFind(createSparseMatrix(7,3,Triplets<int>()),VectorXi(3)).rows(),3);
}
TEST_F(EigenTest, cumsumFindInvalidValues)
{
	using Eigen::VectorXi;
	auto v1 = cumsumFind(createSquareMatrix(4,Triplets<int>()),VectorXi(3));
	ASSERT_EQ(v1.rows(),3);

	for(auto i = 0; i != v1.rows(); ++i)
	{
		EXPECT_EQ(v1[i],-1);
	}

	auto v2 = cumsumFind(createSparseMatrix(7,2,Triplets<int>()),VectorXi(6));
	ASSERT_EQ(v2.rows(),6);

	for(auto i = 0; i != v2.rows(); ++i)
	{
		EXPECT_EQ(v2[i],-1);
	}
	
}

TEST_F(EigenTest, cumsumFindValidInputs)
{
	using Eigen::MatrixXi;
	using Eigen::VectorXd;
	using Eigen::VectorXi;

	MatrixXi m1(3,3);
	m1 << 
		1,2,3, // sum = 6
		4,5,6, // sum = 15
		7,8,9; // sum = 24
	VectorX<double> T1(3);
	T1 << 3, 7.5, 12;

	VectorXi v1(3);
	v1 << 2,1,1;

	auto out1 = cumsumFind(Eigen::SparseMatrix<int>(m1.sparseView()),T1);
	EXPECT_EQ(out1, v1);

	MatrixXi m2(4,3);
	m2 << 
		7,0,1, // sum = 8
		4,2,6, // sum = 12
		7,8,0, // sum = 15
		1,9,9; // sum = 19
	VectorX<double> T2(4);
	T2 << 4,6,7.5,9.5;

	VectorXi v2(4);
	v2 << 0,2,1,1;

	auto out2 = cumsumFind(Eigen::SparseMatrix<int>(m2.sparseView()),T2);
	EXPECT_EQ(out2, v2);

	//  0 0 0 1   sum = 1
	//  0 9 0 2   sum = 11
	//  4 0 5 0   sum = 9
	//  1 0 0 1   sum = 2
	//  4 4 4 4   sum = 16
	auto m3 = createSparseMatrix(5,4,
			Triplets<int>(
				{
					{0,3,1},
					{1,1,9}, {1,3,2},
					{2,0,4}, {2,2,5},
					{3,0,1}, {3,3,1},
					{4,0,4}, {4,1,4}, {4,2,4}, {4,3,4}
				}
			)
		);

	VectorX<double> T3(5);
	T3 << 0.5, 5.5, 4.5, 1, 8;

	VectorXi v3(5);
	v3 << 3, 1, 2, 3, 2;

	auto out3 = cumsumFind(m3,T3);
	EXPECT_EQ(out3, v3);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/* vim: set ts=4 sw=4 :*/
