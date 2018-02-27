/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtWidgets>

#include "qt/slidersgroup.h"

SlidersGroup::SlidersGroup(Qt::Orientation orientation, const QString &title,
	QWidget *parent)
	: QGroupBox(title, parent)
{
	slider = new QSlider(orientation);
	slider->setFocusPolicy(Qt::StrongFocus);
	slider->setTickPosition(QSlider::TicksBothSides);
	slider->setTickInterval(10);
	slider->setSingleStep(1);

	scrollBar = new QScrollBar(orientation);
	scrollBar->setFocusPolicy(Qt::StrongFocus);

	dial = new QDial;
	dial->setFocusPolicy(Qt::StrongFocus);

	connect(slider, SIGNAL(valueChanged(int)), scrollBar, SLOT(setValue(int)));
	connect(scrollBar, SIGNAL(valueChanged(int)), dial, SLOT(setValue(int)));
	connect(dial, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
	connect(dial, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));

	QBoxLayout::Direction direction;

	if (orientation == Qt::Horizontal)
		direction = QBoxLayout::TopToBottom;
	else
		direction = QBoxLayout::LeftToRight;

	QBoxLayout *slidersLayout = new QBoxLayout(direction);
	slidersLayout->addWidget(slider);
	slidersLayout->addWidget(scrollBar);
//	slidersLayout->addWidget(dial);
	setLayout(slidersLayout);
}

void SlidersGroup::setValue(int value)
{
	slider->setValue(value);
}

void SlidersGroup::setMinimum(int value)
{
	slider->setMinimum(value);
	scrollBar->setMinimum(value);
	dial->setMinimum(value);
}

void SlidersGroup::setMaximum(int value)
{
	slider->setMaximum(value);
	scrollBar->setMaximum(value);
	dial->setMaximum(value);
}

void SlidersGroup::invertAppearance(bool invert)
{
	slider->setInvertedAppearance(invert);
	scrollBar->setInvertedAppearance(invert);
	dial->setInvertedAppearance(invert);
}

void SlidersGroup::invertKeyBindings(bool invert)
{
	slider->setInvertedControls(invert);
	scrollBar->setInvertedControls(invert);
	dial->setInvertedControls(invert);
}