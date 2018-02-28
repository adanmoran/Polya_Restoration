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

    // There are 5 sliders, so each takes 20% of the remainig space
    auto sliderScaling = remainingSize * 0.2;

    transitionProb_->setMinimumWidth(sliderScaling);
    errorProb_->setMinimumWidth(sliderScaling);

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
/* vim: set ts=4 sw=4 et : */
