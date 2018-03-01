//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#include "qt/EdgeToolbar.h"
#include <QDebug>

////////////
// Public //
////////////

EdgeToolbar::EdgeToolbar(const QString& title, QWidget* parent)
	: QToolBar(title, parent)
	, edgeLabel_(new QLabel(this))
	, sigmaedge_(new SlidersGroup(Qt::Horizontal, tr("Sigma * 10")))
	, threshold_(new SlidersGroup(Qt::Horizontal, tr("Threshold (%)")))
	, useedge_(new QCheckBox(tr("Use Edge Map"), this))
{
	// Add the elements to the toolbar in order
	initializeComboLabel();
	initializeCheckBox();
	initializeSliders();

	// Set the view based on the default choice
	useEdgeMap(useedge_->isChecked());
}

///////////
// Slots //
///////////

auto EdgeToolbar::scaleToWidth(const QSize& size) -> void
{
	scaleToWidth(size.width());
}

auto EdgeToolbar::scaleToWidth(int width) -> void
{
	// TODO Change the width of all the elements to fit inside the given width
	auto remainingSize = width - (edgeLabel_->size().width());

	// There are 5 sliders,but only 4 are visible at once.
	// so each takes 25% of the remaining space
	// we make it 22% to account for separators
	auto sliderScaling = remainingSize * 0.22;

	sigmaedge_->setMinimumWidth(sliderScaling);
	threshold_->setMinimumWidth(sliderScaling);

	// emit the signal
	emit widthChanged(width);
}

auto EdgeToolbar::useEdgeMap(int useedge) -> void
{
	if (useedge == Qt::Unchecked)
	{
		sigmaedgeAction_->setVisible(false);
		thresholdAction_->setVisible(false);
		emit boxChecked(false);
	}
	else
	{
		sigmaedgeAction_->setVisible(true);
		thresholdAction_->setVisible(true);
		emit boxChecked(true);
	}
}

////////////
// Private //
////////////

auto EdgeToolbar::initializeComboLabel() -> void
{
	edgeLabel_->setText(COMBO_LABEL);
	addWidget(edgeLabel_);
	addSeparator();
}

auto EdgeToolbar::initializeCheckBox() -> void
{
	addWidget(useedge_);
	addSeparator();
}

auto EdgeToolbar::initializeSliders() -> void
{
	initializeSigmaSlider();
	initializeThresholdSlider();
}

auto EdgeToolbar::initializeSigmaSlider() -> void
{
	// Probability is between 0 - 100
	sigmaedge_->setMinimum(0);
	sigmaedge_->setMaximum(5);
	// Default values is 95%, can be changed by user
	sigmaedge_->setValue(3);
	sigmaedgeAction_ = addWidget(sigmaedge_);
	addSeparator();
}

auto EdgeToolbar::initializeThresholdSlider() -> void
{
	// Probability is between 0 - 100
	threshold_->setMinimum(0);
	threshold_->setMaximum(100);
	// Default values is 95%, can be changed by user
	threshold_->setValue(40);
	thresholdAction_ = addWidget(threshold_);
	addSeparator();
}

auto EdgeToolbar::Connections() -> void
{
	// Connect the combo box to the sliders
	connect(useedge_, &QCheckBox::stateChanged, this, &EdgeToolbar::useEdgeMap);

}

auto EdgeToolbar::sliderConnections() -> void
{
	// Emit the slider values when they change
	connect(sigmaedge_, SIGNAL(valueChanged(int)), SIGNAL(sigmaedgeChanged(int)));
	connect(threshold_, SIGNAL(valueChanged(int)), SIGNAL(thresholdChanged(int)));
}

