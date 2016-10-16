#include "widget.h"
#include "pic.h"

#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QFileDialog>

#include <iostream>
#include <ctime>

widget::widget(QWidget *parent)
    : QWidget(parent)
{
    initButtons();

    picture = new pic(this);

    layout();

    setConnections();
}

//TODO use filename
widget::widget(const QString &filename, QWidget *parent)
    : QWidget(parent)
{
    initButtons();

    picture = new pic(filename, this);

    layout();

    setConnections();
}

void widget::initButtons()
{
    openButton = new QPushButton(tr("Open..."));
    originalButton = new QPushButton(tr("Original"));
    redButton = new QPushButton(tr("Red"));
    greenButton = new QPushButton(tr("Green"));
    blueButton = new QPushButton(tr("Blue"));
    inverseButton = new QPushButton(tr("Inverse"));
    compressButton = new QPushButton(tr("Compress"));
	cannyButton = new QPushButton(tr("Canny"));
	halvingButton = new QPushButton(tr("Halving"));
	rotateRightButton = new QPushButton(tr("Rotate Right"));
	rotateLeftButton = new QPushButton(tr("Rotate Left"));
	hadamardButton = new QPushButton(tr("Hadamard"));

    brSlider = new QSlider(Qt::Horizontal, this);
    brSlider->setMinimum(0);
    brSlider->setMaximum(255);
}

void widget::layout()
{
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(openButton);
    rightLayout->addWidget(originalButton);
    rightLayout->addWidget(redButton);
    rightLayout->addWidget(greenButton);
    rightLayout->addWidget(blueButton);
    rightLayout->addWidget(inverseButton);
    rightLayout->addWidget(compressButton);
	rightLayout->addWidget(cannyButton);
	rightLayout->addWidget(halvingButton);
    rightLayout->addWidget(brSlider);
	rightLayout->addWidget(rotateRightButton);
	rightLayout->addWidget(rotateLeftButton);
	rightLayout->addWidget(hadamardButton);
    rightLayout->setAlignment(Qt::AlignTop);
    rightLayout->setSizeConstraint(QLayout::SetFixedSize);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(picture);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);
    setLayout(layout);
}

void widget::setConnections()
{
    // connect all buttons to their slots
    connect(openButton, SIGNAL(clicked()),
            this, SLOT(onOpen()));
    connect(originalButton, SIGNAL(clicked()),
            this, SLOT(onOriginal()));
    connect(redButton, SIGNAL(clicked()),
            this, SLOT(onRed()));
    connect(greenButton, SIGNAL(clicked()),
            this, SLOT(onGreen()));
    connect(blueButton, SIGNAL(clicked()),
            this, SLOT(onBlue()));
    connect(inverseButton, SIGNAL(clicked()),
            this, SLOT(onInverse()));
    connect(compressButton, SIGNAL(clicked()),
            this, SLOT(onCompress()));
    connect(cannyButton, SIGNAL(clicked()),
            this, SLOT(onCanny()));
    connect(brSlider, SIGNAL(valueChanged(int)),
            this, SLOT(onBrightnessChanged(int)));
    connect(halvingButton, SIGNAL(clicked()),
            this, SLOT(onHalving()));
    connect(rotateRightButton, SIGNAL(clicked()),
            this, SLOT(onRotateRight()));
    connect(rotateLeftButton, SIGNAL(clicked()),
            this, SLOT(onRotateLeft()));
    connect(hadamardButton, SIGNAL(clicked()),
            this, SLOT(onHadamard()));
}
//TODO 
// implementation of onOpen() slot
// this slot is called when user presses on Open... button
// 
void widget::onOpen()
{
    QString dirname("../");
    if( ! mFilename.isEmpty())
    {
        dirname = QFileInfo(mFilename).absolutePath();
    }

    QString filename = QFileDialog::getOpenFileName(this, 
                                    tr("Open File"), dirname, tr("Images *.bmp"));
    if (filename.isEmpty())
        return;

    mFilename = filename;

    brSlider->blockSignals(true);
    brSlider->setValue(0);
    brSlider->blockSignals(false);
    //Q_ASSERT(picture);
    //delete picture;
    //picture = new pic(filename, this);
    //Q_ASSERT(picture);
    picture->init(filename);
    resize(sizeHint());
    update();
}

void widget::update()
{
    picture->repaint();
    //QWidget::update();
    resize(sizeHint());
}

void widget::onOriginal()
{
    if(mFilename.isEmpty())
        return;

    brSlider->blockSignals(true);
    brSlider->setValue(0);
    brSlider->blockSignals(false);
    picture->init(mFilename);
    resize(sizeHint());
    update();
    // repaint();
}

void widget::onRed()
{
    picture->paintRed();
    repaint();
}

void widget::onGreen()
{
    picture->paintGreen();
    repaint();
}

void widget::onBlue()
{
    picture->paintBlue();
    repaint();
}

void widget::onInverse()
{
    picture->inverse();
    update();
}

void widget::onCompress()
{
    picture->compress();
    update();
}

void widget::onCanny()
{
	picture->canny();
	update();
}

void widget::onBrightnessChanged(int br)
{
    picture->addBrightness(br - brightnessValue);
    brightnessValue = br;
    update();
}

void widget::onHalving()
{
	picture->halving();
	repaint();
}

void widget::onRotateRight()
{
	picture->rotateRight();
	update();
}

void widget::onRotateLeft()
{
	picture->rotateLeft();
	update();
}

void widget::onHadamard()
{
    std::clock_t begin_time = std::clock();
	picture->doHadamard();
    std::cout << "time elapsed " << double(clock() - begin_time) / CLOCKS_PER_SEC << std::endl;
	update();
}
