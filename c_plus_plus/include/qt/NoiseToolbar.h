//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#ifndef NOISE_TOOLBAR_H_
#define NOISE_TOOLBAR_H_

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QToolBar>
#include <QString>
#include <QSize>
#include <QAction>

#include "qt/slidersgroup.h"

class NoiseToolbar : public QToolBar
{
	Q_OBJECT

public:
    /**
     * Creates a toolbar which contains the noise options allowed by 
     * the Polya model
     */
    NoiseToolbar(const QString& title, QWidget* parent = nullptr);

    /**
     * Titles of the noise types allowable
     */
    const QString NO_NOISE = "None";
    const QString GAUSSIAN_NOISE = "Gaussian";
    const QString GAUSSIAN_MARKOV_NOISE = "Gaussian Markov";
    const QString GAUSSIAN_BURST_NOISE = "Gaussian Burst";
    const QString BINARY_BURST_NOISE = "Binary Burst";
    const QString GAUSSIAN_AND_BURST_NOISE = "Gaussian + Binary Burst";

public slots:
    // Scale the sliders to fit inside this width
    void scaleToWidth(const QSize&);
    void scaleToWidth(int);
	void chooseNoise(const QString&);

signals:
    void widthChanged(int);

private:
    auto initializeComboBox() -> void;
    auto initializeComboLabel() -> void;
    auto initializeSliders() -> void;
    auto initializeTransitionSlider() -> void;
    auto initializeErrorSlider() -> void;
    auto initializeConfidenceSlider() -> void;
    auto initializeGaussianSigmaSlider() -> void;
    auto initializeBurstSigmaSlider() -> void;

    QToolBar* toolbar_;
    /**
     * Dropdown box which allows one to choose the noise type.
     */
    QComboBox* noiseType_;
    QLabel* noiseLabel_;

    SlidersGroup* transitionProb_;
    SlidersGroup* errorProb_;
    SlidersGroup* confidence_;
    SlidersGroup* gaussianSigma_;
    SlidersGroup* burstSigma_;

    // Actions, for hiding and showing sliders
    QAction* gaussianSigmaAction_;
    QAction* burstSigmaAction_;

    const double GAUSSIAN_MEAN = 0;
    const QString COMBO_LABEL = "Noise Type";

};

#endif

/* vim: set ts=4 sw=4 et : */
