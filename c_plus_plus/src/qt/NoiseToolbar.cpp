//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#include "qt/NoiseToolbar.h"
#include <QDebug>

////////////
// Public //
////////////

NoiseToolbar::NoiseToolbar(const QString& title, QWidget* parent)
: QToolBar(title, parent)
, noiseType_(new QComboBox(this))
, noiseLabel_(new QLabel(this))
, transitionProb_(new SlidersGroup(Qt::Horizontal, tr("Transition (%)")))
,errorProb_(new SlidersGroup(Qt::Horizontal, tr("Error (%)")))
, confidence_(new SlidersGroup(Qt::Horizontal, tr("Confidence Interval (%)")))
, gaussianSigma_(new SlidersGroup(Qt::Horizontal, tr("Gaussian Sigma * 10")))
, burstSigma_(new SlidersGroup(Qt::Horizontal, tr("Burst Sigma * 10")))
{
    // Add the elements to the toolbar in order
    initializeComboLabel();
    initializeComboBox();
    initializeSliders();
}

///////////
// Slots //
///////////

auto NoiseToolbar::scaleToWidth(const QSize& size) -> void
{
    scaleToWidth(size.width());
}

auto NoiseToolbar::scaleToWidth(int width) -> void
{
    // TODO Change the width of all the elements to fit inside the given width
    auto remainingSize = width - (noiseLabel_->size().width() + noiseType_->size().width());

    // There are 5 sliders,but only 4 are visible at once.
    // so each takes 25% of the remaining space
    // we make it 23% to account for separators
    auto sliderScaling = remainingSize * 0.23;

    transitionProb_->setMinimumWidth(sliderScaling);
    errorProb_->setMinimumWidth(sliderScaling);
    confidence_->setMinimumWidth(sliderScaling);
    gaussianSigma_->setMinimumWidth(sliderScaling);
    burstSigma_->setMinimumWidth(sliderScaling);

    // emit the signal
    emit widthChanged(width);
}

/////////////
// Private //
/////////////

auto NoiseToolbar::initializeComboLabel() -> void
{
    noiseLabel_->setText(COMBO_LABEL);
    addWidget(noiseLabel_);
    addSeparator();
}

auto NoiseToolbar::initializeComboBox() -> void
{
	noiseType_->addItem(NO_NOISE);
	noiseType_->addItem(GAUSSIAN_NOISE);
	noiseType_->addItem(BINARY_BURST_NOISE);
	noiseType_->addItem(GAUSSIAN_MARKOV_NOISE);
	noiseType_->addItem(GAUSSIAN_BURST_NOISE);
	noiseType_->addItem(GAUSSIAN_AND_BURST_NOISE);
    addWidget(noiseType_);
    addSeparator();
}

auto NoiseToolbar::initializeSliders() -> void
{
    initializeTransitionSlider();
    initializeErrorSlider();
    initializeConfidenceSlider();
    initializeGaussianSigmaSlider();
    initializeBurstSigmaSlider();
}

auto NoiseToolbar::initializeTransitionSlider() -> void
{
    // Probability is between 0 - 100
    transitionProb_->setMinimum(0);
    transitionProb_->setMaximum(100);
    // Default values is 95%, can be changed by user
    transitionProb_->setValue(95);
    addWidget(transitionProb_);
    addSeparator();
}

auto NoiseToolbar::initializeErrorSlider() -> void
{
    // Probability is between 0 - 100
    errorProb_->setMinimum(0);
    errorProb_->setMaximum(100);
    // Default values is 90%, can be changed by user
    errorProb_->setValue(90);
    addWidget(errorProb_);
    addSeparator();
}

auto NoiseToolbar::initializeConfidenceSlider() -> void
{
    // Probability is between 0 - 100
    confidence_->setMinimum(0);
    confidence_->setMaximum(100);
    // Default values is 80%, can be changed by user
    confidence_->setValue(80);
    addWidget(confidence_);
    addSeparator();
}

auto NoiseToolbar::initializeGaussianSigmaSlider() -> void
{
    // Sigma is between 0.0 and 5.0
    gaussianSigma_->setMinimum(0);
    gaussianSigma_->setMaximum(50);
    // Default values is 90%, can be changed by user
    gaussianSigma_->setValue(30);
    addWidget(gaussianSigma_);
    addSeparator();
}

auto NoiseToolbar::initializeBurstSigmaSlider() -> void
{
    // Sigma is between 0.0 and 5.0
    burstSigma_->setMinimum(0);
    burstSigma_->setMaximum(50);
    // Default values is 3.0, can be changed by user
    burstSigma_->setValue(30);
    addWidget(burstSigma_);
    addSeparator();
}
/* vim: set ts=4 sw=4 et : */
