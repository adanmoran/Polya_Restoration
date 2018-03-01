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
	, sigmaEdge_(new SlidersGroup(Qt::Horizontal, tr("Sigma * 10")))
	, threshold_(new SlidersGroup(Qt::Horizontal, tr("Threshold (%)")))
	, useEdge_(new QCheckBox(tr("Use Edge Map"), this))
{
	// Add the elements to the toolbar in order
//	initializeComboLabel();
	initializeCheckBox();
	initializeSliders();
	Connections();

	// Set the view based on the default choice
	useEdgeMap(useEdge_->isChecked());
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
	// Only have half the screen
	auto remainingSize = width/2 - (edgeLabel_->size().width());

	// There are 5 sliders,but only 4 are visible at once.
	// so each takes 25% of the remaining space
	// we make it 22% to account for separators
	auto sliderScaling = remainingSize * 0.22;

	sigmaEdge_->setMinimumWidth(sliderScaling);
	threshold_->setMinimumWidth(sliderScaling);

	// emit the signal
	emit widthChanged(width);
}

auto EdgeToolbar::useEdgeMap(int useEdge) -> void
{
	if (useEdge == Qt::Unchecked)
	{
		sigmaEdgeAction_->setEnabled(false);
		thresholdAction_->setEnabled(false);
		emit boxChecked(false);
	}
	else
	{
		sigmaEdgeAction_->setEnabled(true);
		thresholdAction_->setEnabled(true);
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
	addWidget(useEdge_);
	addSeparator();
}

auto EdgeToolbar::initializeSliders() -> void
{
	initializeSigmaSlider();
	initializeThresholdSlider();
}

auto EdgeToolbar::initializeSigmaSlider() -> void
{
	// Edge map has sigma between 0 and 5.0
	sigmaEdge_->setMinimum(0);
	sigmaEdge_->setMaximum(50);
	// Default values is 3.0, can be changed by user
	sigmaEdge_->setValue(30);
	sigmaEdgeAction_ = addWidget(sigmaEdge_);
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
	connect(useEdge_, SIGNAL(stateChanged(int)), SLOT(useEdgeMap(int)));

}

auto EdgeToolbar::sliderConnections() -> void
{
	// Emit the slider values when they change
	connect(sigmaEdge_, SIGNAL(valueChanged(int)), SIGNAL(sigmaEdgeChanged(int)));
	connect(threshold_, SIGNAL(valueChanged(int)), SIGNAL(thresholdChanged(int)));
}

/* vim: set ts=4 sw=4 et : */
