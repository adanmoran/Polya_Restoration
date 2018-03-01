//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#include "qt/OutputsToolbar.h"
#include <QDebug>

////////////
// Public //
////////////

OutputsToolbar::OutputsToolbar(const QString& title, QWidget* parent)
	: QToolBar(title, parent)
	, outputsLabel_(new QLabel(this))
	, mseLabel_(new QLabel(this))
	, psnrLabel_(new QLabel(this))
	, ssimLabel_(new QLabel(this))
	, save_(new QPushButton(tr("Save"), this))
	, log_(new QPushButton(tr("Log"), this))
{
	// Add the elements to the toolbar in order
	initializeLabels();
	initializePushButtons();
}

///////////
// Slots //
///////////

auto OutputsToolbar::scaleToWidth(const QSize& size) -> void
{
	scaleToWidth(size.width());
}

auto OutputsToolbar::scaleToWidth(int width) -> void
{
	// TODO Change the width of all the elements to fit inside the given width
	auto remainingSize = width - (outputsLabel_->size().width());

	// There are 5 sliders,but only 4 are visible at once.
	// so each takes 25% of the remaining space
	// we make it 22% to account for separators
	auto Scaling = remainingSize * 0.1;


	// emit the signal
	emit widthChanged(width);
}


////////////
// Private //
////////////

auto OutputsToolbar::initializeLabels() -> void
{
	initializeComboLabel();
	initializeMSELabel();
	initializePSNRLabel();
	initializeSSIMLabel();
}

auto OutputsToolbar::initializeComboLabel() -> void
{
	outputsLabel_->setText(COMBO_LABEL);
	addWidget(outputsLabel_);
	addSeparator();
}

auto OutputsToolbar::initializeMSELabel() -> void
{
	mseLabel_->setText("MSE: TO DO");
	addWidget(mseLabel_);
	addSeparator();
}

auto OutputsToolbar::initializePSNRLabel() -> void
{
	psnrLabel_->setText("PSNR: TO DO");
	addWidget(psnrLabel_);
	addSeparator();
}

auto OutputsToolbar::initializeSSIMLabel() -> void
{
	ssimLabel_->setText("SSIM: TO DO");
	addWidget(ssimLabel_);
	addSeparator();
}

auto OutputsToolbar::initializePushButtons() -> void
{
	initializeSaveButton();
	initializeLogButton();
}

auto OutputsToolbar::initializeSaveButton() -> void
{
	addWidget(save_);
	addSeparator();
}

auto OutputsToolbar::initializeLogButton() -> void
{
	addWidget(log_);
	addSeparator();
}