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
/*SG#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif
#endif SG*/
//SG
#include "qt/imageviewer.h"
#include "qt/slidersgroup.h"
//SG
ImageViewer::ImageViewer()
   : imageLabel(new QLabel)
   , scrollArea(new QScrollArea)
   , scaleFactor(1)
{
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);
    setCentralWidget(scrollArea);

    createActions();

    resize(QGuiApplication::primaryScreen()->availableSize() );

	//SG Noise Toolbar

	int toolbarWidth = size().width();

	QToolBar *noiseToolBar = addToolBar(tr("&Noise"));

	QComboBox *noisetype = new QComboBox;
	QLabel *noiselabel = new QLabel(this);
	noiselabel->setText("Noise Type");
	noiseToolBar->addWidget(noiselabel);
	noiseToolBar->addSeparator();

	noiseToolBar->addWidget(noisetype);
	noisetype->addItem("None");
	noisetype->addItem("Gaussian");
	noisetype->addItem("Binary Burst");
	noisetype->addItem("Gaussian Markov");
	noisetype->addItem("Gaussian Burst");
	noisetype->addItem("Gaussian + Binary Burst");

	noiseToolBar->addSeparator();

	SlidersGroup *transition = new SlidersGroup(Qt::Horizontal, tr("Transition (%)"));
	noiseToolBar->addWidget(transition);
	//set initial values
	transition->setMinimum(0);
	transition->setMaximum(100);
	transition->setValue(95);
	//set width
	int transitionw = toolbarWidth *0.2;
	transition->setMinimumWidth(transitionw);
	connect(this, QOverload<const QSize&>::of(&ImageViewer::resized), [=](const QSize& size) {transition->setMinimumWidth(size.width()*.2);});

	noiseToolBar->addSeparator();

	SlidersGroup *error = new SlidersGroup(Qt::Horizontal, tr("Error (%)"));
	noiseToolBar->addWidget(error);
	//set initial values
	error->setMinimum(0);
	error->setMaximum(100);
	error->setValue(90);
	//set width
	int errorw = toolbarWidth *0.2;
	error->setFixedWidth(errorw);
	connect(this, QOverload<const QSize&>::of(&ImageViewer::resized), [=](const QSize& size) {error->setMinimumWidth(size.width()*.2);});

	noiseToolBar->addSeparator();

	SlidersGroup *confidence = new SlidersGroup(Qt::Horizontal, tr("Confidence Interval (%)"));
	noiseToolBar->addWidget(confidence);
	//set initial values
	confidence->setMinimum(0);
	confidence->setMaximum(100);
	confidence->setValue(80);
	//set width
	int confidencew = toolbarWidth *0.2;
	confidence->setFixedWidth(confidencew);
	connect(this, QOverload<const QSize&>::of(&ImageViewer::resized), [=](const QSize& size) {confidence->setMinimumWidth(size.width()*.2);});

	noiseToolBar->addSeparator();

	SlidersGroup *gaussiansigma = new SlidersGroup(Qt::Horizontal, tr("Gaussian Sigma"));
	gsaction = noiseToolBar->addWidget(gaussiansigma);
	gsaction->setVisible(false);
	//set initial values
	gaussiansigma->setMinimum(0);
	gaussiansigma->setMaximum(100);
	gaussiansigma->setValue(1);
	//set width
	int gsw = toolbarWidth *0.2;
	gaussiansigma->setFixedWidth(gsw);
	connect(this, QOverload<const QSize&>::of(&ImageViewer::resized), [=](const QSize& size) {gaussiansigma->setMinimumWidth(size.width()*.2);});

	SlidersGroup *burstsigma = new SlidersGroup(Qt::Horizontal, tr("Burst Sigma"));
	bsaction = noiseToolBar->addWidget(burstsigma);
	bsaction->setVisible(false);
	//set initial values
	burstsigma->setMinimum(0);
	burstsigma->setMaximum(100);
	burstsigma->setValue(1);
	//set width
	int bsw = toolbarWidth *0.2;
	burstsigma->setFixedWidth(bsw);
	connect(this, QOverload<const QSize&>::of(&ImageViewer::resized), [=](const QSize& size) {burstsigma->setMinimumWidth(size.width()*.2);});

	noiseToolBar->addSeparator();

	//Connect signals and slots for noise type options
	//connect(noisetype, QOverload<const QString&>::of(&QComboBox::activated), [=](const QString& str) {chooseNoise(str);});
	connect(noisetype, SIGNAL(activated(const QString&)), SLOT(chooseNoise(const QString&)));
	//	verticalSliders, SLOT(setValue(int)));
	//SG
}


bool ImageViewer::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    setImage(newImage);

    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}

void ImageViewer::setImage(const QImage &newImage)
{
    image = newImage;
    imageLabel->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;

    scrollArea->setVisible(true);
    printAct->setEnabled(true);
    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}


bool ImageViewer::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}


static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void ImageViewer::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void ImageViewer::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void ImageViewer::print()
{
}
/*SG
    Q_ASSERT(imageLabel->pixmap());
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
#endif
}SG*/

void ImageViewer::copy()
{
#ifndef QT_NO_CLIPBOARD
    QGuiApplication::clipboard()->setImage(image);
#endif // !QT_NO_CLIPBOARD
}

#ifndef QT_NO_CLIPBOARD
static QImage clipboardImage()
{
    if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
        if (mimeData->hasImage()) {
            const QImage image = qvariant_cast<QImage>(mimeData->imageData());
            if (!image.isNull())
                return image;
        }
    }
    return QImage();
}
#endif // !QT_NO_CLIPBOARD

void ImageViewer::paste()
{
#ifndef QT_NO_CLIPBOARD
    const QImage newImage = clipboardImage();
    if (newImage.isNull()) {
        statusBar()->showMessage(tr("No image in clipboard"));
    } else {
        setImage(newImage);
        setWindowFilePath(QString());
        const QString message = tr("Obtained image from clipboard, %1x%2, Depth: %3")
            .arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
        statusBar()->showMessage(message);
    }
#endif // !QT_NO_CLIPBOARD
}

void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
    updateActions();
}

void ImageViewer::about()
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
               "and QScrollArea to display an image. QLabel is typically used "
               "for displaying a text, but it can also display an image. "
               "QScrollArea provides a scrolling view around another widget. "
               "If the child widget exceeds the size of the frame, QScrollArea "
               "automatically provides scroll bars. </p><p>The example "
               "demonstrates how QLabel's ability to scale its contents "
               "(QLabel::scaledContents), and QScrollArea's ability to "
               "automatically resize its contents "
               "(QScrollArea::widgetResizable), can be used to implement "
               "zooming and scaling features. </p><p>In addition the example "
               "shows how to use QPainter to print an image.</p>"));
}

void ImageViewer::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &ImageViewer::open);
    openAct->setShortcut(QKeySequence::Open);

    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &ImageViewer::saveAs);
    saveAsAct->setEnabled(false);

    printAct = fileMenu->addAction(tr("&Print..."), this, &ImageViewer::print);
    printAct->setShortcut(QKeySequence::Print);
    printAct->setEnabled(false);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    copyAct = editMenu->addAction(tr("&Copy"), this, &ImageViewer::copy);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setEnabled(false);

    QAction *pasteAct = editMenu->addAction(tr("&Paste"), this, &ImageViewer::paste);
    pasteAct->setShortcut(QKeySequence::Paste);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &ImageViewer::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);

    zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &ImageViewer::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);

    normalSizeAct = viewMenu->addAction(tr("&Normal Size"), this, &ImageViewer::normalSize);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);

    viewMenu->addSeparator();

    fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &ImageViewer::fitToWindow);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &ImageViewer::about);
    helpMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);


}

void ImageViewer::updateActions()
{
    saveAsAct->setEnabled(!image.isNull());
    copyAct->setEnabled(!image.isNull());
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

//SG
void ImageViewer::createControls(const QString &title)
{
	controlsGroup = new QGroupBox(title);

	minimumLabel = new QLabel(tr("Minimum value:"));
	maximumLabel = new QLabel(tr("Maximum value:"));
	valueLabel = new QLabel(tr("Current value:"));

	invertedAppearance = new QCheckBox(tr("Inverted appearance"));
	invertedKeyBindings = new QCheckBox(tr("Inverted key bindings"));

	minimumSpinBox = new QSpinBox;
	minimumSpinBox->setRange(-100, 100);
	minimumSpinBox->setSingleStep(1);

	maximumSpinBox = new QSpinBox;
	maximumSpinBox->setRange(-100, 100);
	maximumSpinBox->setSingleStep(1);

	valueSpinBox = new QSpinBox;
	valueSpinBox->setRange(-100, 100);
	valueSpinBox->setSingleStep(1);

	orientationCombo = new QComboBox;
	orientationCombo->addItem(tr("Horizontal slider-like widgets"));
	orientationCombo->addItem(tr("Vertical slider-like widgets"));

	connect(orientationCombo, SIGNAL(activated(int)),
		stackedWidget, SLOT(setCurrentIndex(int)));
	connect(minimumSpinBox, SIGNAL(valueChanged(int)),
		horizontalSliders, SLOT(setMinimum(int)));
	connect(minimumSpinBox, SIGNAL(valueChanged(int)),
		verticalSliders, SLOT(setMinimum(int)));
	connect(maximumSpinBox, SIGNAL(valueChanged(int)),
		horizontalSliders, SLOT(setMaximum(int)));
	connect(maximumSpinBox, SIGNAL(valueChanged(int)),
		verticalSliders, SLOT(setMaximum(int)));
	connect(invertedAppearance, SIGNAL(toggled(bool)),
		horizontalSliders, SLOT(invertAppearance(bool)));
	connect(invertedAppearance, SIGNAL(toggled(bool)),
		verticalSliders, SLOT(invertAppearance(bool)));
	connect(invertedKeyBindings, SIGNAL(toggled(bool)),
		horizontalSliders, SLOT(invertKeyBindings(bool)));
	connect(invertedKeyBindings, SIGNAL(toggled(bool)),
		verticalSliders, SLOT(invertKeyBindings(bool)));

	QGridLayout *controlsLayout = new QGridLayout;
	controlsLayout->addWidget(minimumLabel, 0, 0);
	controlsLayout->addWidget(maximumLabel, 1, 0);
	controlsLayout->addWidget(valueLabel, 2, 0);
	controlsLayout->addWidget(minimumSpinBox, 0, 1);
	controlsLayout->addWidget(maximumSpinBox, 1, 1);
	controlsLayout->addWidget(valueSpinBox, 2, 1);
	controlsLayout->addWidget(invertedAppearance, 0, 2);
	controlsLayout->addWidget(invertedKeyBindings, 1, 2);
	controlsLayout->addWidget(orientationCombo, 3, 0, 1, 3);
	controlsGroup->setLayout(controlsLayout);
}
//SG