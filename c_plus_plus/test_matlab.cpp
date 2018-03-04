#include "matlab/MatlabProxy.h"
#include "common/EigenHelpers.h"
#include <iostream>
#include <QApplication>
#include <QLabel>
#include <QImage>
#include <QImageReader>
#include <QMatrix>

int run_main(int argc, char** argv)
{
	MatlabProxy mp;
	mp.Initialize();
	auto a = mp.getSparseAdj({ 3,3 }, 1, MatlabProxy::PNorm::ONE);

	std::cout << "The adjacency matrix for a 3x3 is \n" << full(a) << std::endl;

	QApplication app(argc, argv);

	QImage lena;
	QImageReader reader("lena512.bmp");
	if (!reader.read(&lena))
	{
		std::cout << "Failed to read lena" << std::endl;
		return -1;
	}
	if (lena.isGrayscale())
	{
		std::cout << "Lena is a grayscale image!" << std::endl;
	}

	QLabel label;
	label.setPixmap(QPixmap::fromImage(lena));
	label.show();

	QImage noisyImg = label.pixmap()->toImage();

	// Get the noise on the image by calling MATLAB commands
	Prefs prefs;
	prefs.image.type = Prefs::ImageType::GRAY;

	Noise noise;
	noise.type = Noise::Type::GAUSSIAN;
	noise.gaussian.sigma = 0.01;

	std::cout << "noisyImg has format " << noisyImg.format() << std::endl;

	if (!mp.addNoise(&noisyImg, prefs, noise))
	{
		std::cerr << "Failed to add noise to the image " << std::endl;

		return -2;
	}
	QLabel noiseLabel;
	noiseLabel.setPixmap(QPixmap::fromImage(noisyImg));
	noiseLabel.show();

	return app.exec();
}

int main()
{
	// Initialize the MATLAB Runtime
	mclmcrInitialize();

	return mclRunMain(reinterpret_cast<mclMainFcnType>(run_main), 0, NULL);
}

/* vim: set ts=4 sw=4 et : */