//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#include "qt/QuantizeToolbar.h"
#include <QDebug>

////////////
// Public //
////////////

QuantizeToolbar::QuantizeToolbar(const QString& title, QWidget* parent)
	: QToolBar(title, parent)
	, quantizeLabel_(new QLabel(this))
	, numballtypes_(new SlidersGroup(Qt::Horizontal, tr("Number of Partitions/Ball Types")))
	, useQuantize_(new QCheckBox(tr("Use Quantization"), this))
	, typeLloyd_(new QRadioButton(tr("Lloyd"),this))
	, typeUniform_(new QRadioButton(tr("Uniform"), this))
	, inverseLow_(new QRadioButton(tr("Low"), this))
	, inverseMean_(new QRadioButton(tr("Mean"), this))
	, inverseHigh_(new QRadioButton(tr("High"), this))
	, quantizeTypeBox_(new QGroupBox(tr("Type"),this))
	, inverseQuantizeBox_(new QGroupBox(tr("Inverse Quantization Value Select"), this))
	, quantizeTypeHBox_(new QHBoxLayout(this))
	, inverseQuantizeHBox_(new QHBoxLayout(this))
{
	// Add the elements to the toolbar in order
//	initializeComboLabel();
	initializeCheckBox();
	initializeSliders();
	initializeRadioButtons();
	Connections();

	// Set the view based on the default choice
	useQuantization(useQuantize_->isChecked());
}

///////////
// Slots //
///////////

auto QuantizeToolbar::scaleToWidth(const QSize& size) -> void
{
	scaleToWidth(size.width());
}

auto QuantizeToolbar::scaleToWidth(int width) -> void
{
	// TODO Change the width of all the elements to fit inside the given width
	auto remainingSize = width/2 - ((quantizeLabel_->size().width())+(quantizeTypeBox_->size().width()) + (inverseQuantizeBox_->size().width()));

	// There are 5 sliders,but only 4 are visible at once.
	// so each takes 25% of the remaining space
	// we make it 22% to account for separators
	auto sliderScaling = remainingSize * 0.22;

	numballtypes_->setMinimumWidth(sliderScaling);

	// emit the signal
	emit widthChanged(width);
}

auto QuantizeToolbar::useQuantization(int useQuantize) -> void
{
	if (useQuantize == Qt::Unchecked)
	{
		numballtypesAction_->setEnabled(false);
		quantizeTypeAction_->setEnabled(false);
		inverseQuantizeAction_->setEnabled(false);
		emit boxChecked(false);
	}
	else
	{
		numballtypesAction_->setEnabled(true);
		quantizeTypeAction_->setEnabled(true);
		inverseQuantizeAction_->setEnabled(true);
		emit boxChecked(true);
	}
}


////////////
// Private //
////////////

auto QuantizeToolbar::initializeComboLabel() -> void
{
	quantizeLabel_->setText(COMBO_LABEL);
	addWidget(quantizeLabel_);
	addSeparator();
}

auto QuantizeToolbar::initializeCheckBox() -> void
{
	addWidget(useQuantize_);
	addSeparator();
}

auto QuantizeToolbar::initializeSliders() -> void
{
	initializeNumBallSlider();
}

auto QuantizeToolbar::initializeNumBallSlider() -> void
{
	// Probability is between 0 - 100
	numballtypes_->setMinimum(2);
	numballtypes_->setMaximum(256);
	// Default values is 95%, can be changed by user
	numballtypes_->setValue(140);
	numballtypesAction_ = addWidget(numballtypes_);
	addSeparator();
}

auto QuantizeToolbar::Connections() -> void
{
	// Connect the combo box to the sliders
	connect(useQuantize_, &QCheckBox::stateChanged, this, &QuantizeToolbar::useQuantization);

}

auto QuantizeToolbar::sliderConnections() -> void
{
	// Emit the slider values when they change
	connect(numballtypes_, SIGNAL(valueChanged(int)), SIGNAL(numballChanged(int)));
}

auto QuantizeToolbar::initializeRadioButtons() -> void
{
	initializeTypeButtons();
	initializeInverseButtons();
}

auto QuantizeToolbar::initializeTypeButtons() -> void
{
	quantizeTypeHBox_->addWidget(typeLloyd_);
	quantizeTypeHBox_->addWidget(typeUniform_);
	quantizeTypeBox_->setLayout(quantizeTypeHBox_);
	quantizeTypeAction_ = addWidget(quantizeTypeBox_);
	addSeparator();
}

auto QuantizeToolbar::initializeInverseButtons() -> void
{
	inverseQuantizeHBox_->addWidget(inverseLow_);
	inverseQuantizeHBox_->addWidget(inverseMean_);
	inverseQuantizeHBox_->addWidget(inverseHigh_);
	inverseQuantizeBox_->setLayout(inverseQuantizeHBox_);
	inverseQuantizeAction_ = addWidget(inverseQuantizeBox_);
	addSeparator();
}
