#include "matlab/MatlabProxy.h"
#include "common/EigenHelpers.h"
#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QImage>
#include <QImageReader>

int run_main(int argc, char** argv)
{
	MatlabProxy mp;
	mp.Initialize();
	auto a = mp.getSparseAdj({ 3,3 }, 1, MatlabProxy::PNorm::ONE);

	std::cout << "The adjacency matrix for a 3x3 is \n" << Eigen::MatrixXd(a) << std::endl;

	QApplication app(argc, argv);

	QImage lena;
	QImageReader reader("lena512.bmp");
	if (!reader.read(&lena))
	{
		std::cout << "Failed to read lena" << std::endl;
		return -1;
	}
	QLabel label;
	label.setPixmap(QPixmap::fromImage(lena));
	label.show();

	return app.exec();
}

int main()
{
	// Initialize the MATLAB Runtime
	mclmcrInitialize();

	return mclRunMain(reinterpret_cast<mclMainFcnType>(run_main), 0, NULL);
}

/* vim: set ts=4 sw=4 et : */