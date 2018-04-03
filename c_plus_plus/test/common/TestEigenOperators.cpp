//////////////////////////////////////////
// Copyright Adan Moran-MacDonald, 2018 //
//////////////////////////////////////////

#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <Eigen/Sparse>
#include "common/EigenTypes.h"
#include "common/EigenOperators.h"

using Eigen::SparseMatrix;
using Eigen::MatrixXi;
using Eigen::VectorXd;
using Eigen::Triplet;

class EigenOperatorTest : public ::testing::Test
{
protected:

    EigenOperatorTest()
    : mat_(4,4)
    {}

    virtual void SetUp()
    {}

    virtual void TearDown()
    {}

    // A matrix for comparing against
    SparseMatrix<int> mat_;
};

TEST_F(EigenOperatorTest, TestGT)
{

	// Test cumsum for square non-identity matrix
	MatrixXi Mat(3,3);
	Mat << 
		1,2,3,
		4,5,6,
		7,8,9;

	VectorXd vals(3);
	vals << 2, 4, 6;

	MatrixXi trueMat(3,3);
	trueMat <<
		0,0,1,
		0,1,1,
		1,1,1;

	SparseMatrix<int> spMat = Mat.sparseView();
	auto gtMat = MatrixXi(spMat > vals);
	EXPECT_EQ(gtMat, trueMat ) <<
		"result:\n" << gtMat << "\n actual: \n" << trueMat;
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/* vim: set ts=4 sw=4 et : */
