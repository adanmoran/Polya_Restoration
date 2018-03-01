//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#include "qt/PolyaToolbar.h"
#include <QDebug>

////////////
// Public //
////////////

PolyaToolbar::PolyaToolbar(const QString& title, QWidget* parent)
	: QToolBar(title, parent)
	, polyaLabel_(new QLabel(this))
	, radius1_(new QRadioButton(tr("1"), this))
	, radius2_(new QRadioButton(tr("2"), this))
	, radiusSlider_(new SlidersGroup(Qt::Horizontal, tr("Radius")))
	, norm1_(new QRadioButton(tr("1"), this))
	, norm2_(new QRadioButton(tr("2"), this))
	, normInf_(new QRadioButton(tr("inf"), this))
	, radiusBox_(new QGroupBox(tr("Radius"), this))
	, normBox_(new QGroupBox(tr("Norm"), this))
	, radiusHBox_(new QHBoxLayout(this))
	, normHBox_(new QHBoxLayout(this))
	, iterations_(new QSpinBox)
	, addBalls_(new QSpinBox)
	, iterationsBox_(new QGroupBox(tr("Number of Iterations"), this))
	, addBallsBox_(new QGroupBox(tr("Balls to Add"), this))
	, iterationsHBox_(new QHBoxLayout(this))
	, addBallsHBox_(new QHBoxLayout(this))
	, startingBalls_(new QSpinBox)
	, startingBallsBox_(new QGroupBox(tr("Starting Balls"), this))
	, startingBallsHBox_(new QHBoxLayout(this))

	, play_(new QPushButton(tr("Start"),this))
	, reset_(new QPushButton(tr("Reset Values"), this))
{
	// Add the elements to the toolbar in order
	initializeComboLabel();
	initializeStartingBallInput();
	initializeIterationsInput();
	initializeAddBallsInput();
	initializeRadiusSlider();
	initializeRadioButtons();
	initializePushButtons();
	edgeMapSelected(false);
}

///////////
// Slots //
///////////

auto PolyaToolbar::scaleToWidth(const QSize& size) -> void
{
	scaleToWidth(size.width());
}

auto PolyaToolbar::scaleToWidth(int width) -> void
{
	// TODO Change the width of all the elements to fit inside the given width
	auto remainingSize = width - (polyaLabel_->size().width());

	// There are 5 sliders,but only 4 are visible at once.
	// so each takes 25% of the remaining space
	// we make it 22% to account for separators
	auto sliderScaling = remainingSize * 0.22;

	radiusSlider_->setMinimumWidth(sliderScaling);

	// emit the signal
	emit widthChanged(width);
}

auto PolyaToolbar::edgeMapSelected(bool useEdge) -> void
{
	if (useEdge)
	{
		radiusButtonAction_->setVisible(true);
		radiusSliderAction_->setVisible(false);
	}
	else
	{
		radiusButtonAction_->setVisible(false);
		radiusSliderAction_->setVisible(true);
	}
}

////////////
// Private //
////////////

auto PolyaToolbar::initializeComboLabel() -> void
{
	polyaLabel_->setText(COMBO_LABEL);
	addWidget(polyaLabel_);
	addSeparator();
}


auto PolyaToolbar::initializeStartingBallInput() -> void
{
	// Probability is between 0 - 100
	startingBalls_->setMinimum(100);
	startingBalls_->setMaximum(1000);
	// Default values is 95%, can be changed by user
	startingBalls_->setValue(479);
	startingBallsHBox_->addWidget(startingBalls_);
	startingBallsBox_->setLayout(startingBallsHBox_);
	addWidget(startingBallsBox_);
	addSeparator();
}

auto PolyaToolbar::sliderConnections() -> void
{
	// Emit the slider values when they change
	connect(startingBalls_, SIGNAL(valueChanged(int)), SIGNAL(startBallChanged(int)));
}

auto PolyaToolbar::initializeRadioButtons() -> void
{
	initializeRadiusButtons();
	initializeNormButtons();
}

auto PolyaToolbar::initializeRadiusButtons() -> void
{
	radiusHBox_->addWidget(radius1_);
	radiusHBox_->addWidget(radius2_);
	radiusBox_->setLayout(radiusHBox_);
	radiusButtonAction_ = addWidget(radiusBox_);
	addSeparator();
}

auto PolyaToolbar::initializeNormButtons() -> void
{
	normHBox_->addWidget(norm1_);
	normHBox_->addWidget(norm2_);
	normHBox_->addWidget(normInf_);
	normBox_->setLayout(normHBox_);
	addWidget(normBox_);
	addSeparator();
}

auto PolyaToolbar::initializeRadiusSlider() -> void
{
	// Probability is between 0 - 100
	radiusSlider_->setMinimum(1);
	radiusSlider_->setMaximum(10);
	// Default values is 95%, can be changed by user
	radiusSlider_->setValue(3);
	radiusSliderAction_ = addWidget(radiusSlider_);
	addSeparator();
}

auto PolyaToolbar::initializeAddBallsInput() -> void
{
	addBalls_->setMinimum(0);
	addBalls_->setMaximum(3000);
	addBalls_->setValue(1642);
	addBallsHBox_->addWidget(addBalls_);
	addBallsBox_->setLayout(addBallsHBox_);
	addWidget(addBallsBox_);
	addSeparator();
}

auto PolyaToolbar::initializeIterationsInput() -> void
{
	iterations_->setMinimum(0);
	iterations_->setMaximum(500);
	iterations_->setValue(8);
	iterationsHBox_->addWidget(iterations_);
	iterationsBox_->setLayout(iterationsHBox_);
	addWidget(iterationsBox_);	
	addSeparator();
}

auto PolyaToolbar::initializePushButtons() -> void
{
	initializePlayButton();
	initializeResetButton();
}

auto PolyaToolbar::initializePlayButton() -> void
{
	addWidget(play_);
	addSeparator();
}

auto PolyaToolbar::initializeResetButton() -> void
{
	addWidget(reset_);
	addSeparator();
}