// @File pic.cpp
// definition of class pic

#include "pic.h"
#include "hadamard.h"

#include <iostream>
#include <QPainter>
#include <QFile>

pic::pic(QWidget *parent)
    : QWidget(parent)
{
    mFilename = QString();
    mWidth = 0;
    mHeight = 0;
    red.reserve(1000);
    green.reserve(1000);
    blue.reserve(1000);
    setMinimumSize(200, 300);
    show();
}

pic::pic(const QString& filename, QWidget *parent)
    :   QWidget(parent), mFilename(filename)
{
    init();
    show();
}

void pic::init(const QString &filename)
{
    if ( filename.isEmpty() && mFilename.isEmpty() )
        return;

    if (! filename.isEmpty() )
        mFilename = filename;

    QFile *pFile = new QFile(mFilename);
    Q_ASSERT(pFile);
    if(! pFile->open(QIODevice::ReadOnly))
    {
        std::cerr << "Could not open " << mFilename.toStdString()
                  << "for reading\n";
        return;
    }


    QDataStream stream(pFile);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.skipRawData(18);
    stream >> mWidth;
    stream >> mHeight;
    stream.skipRawData(28);
    int pad = 4 - (mWidth * 3) % 4;
    if (4 == pad)
        pad = 0;

    {
        red = QVector<int>(mWidth * mHeight, 0);
        green = QVector<int>(mWidth * mHeight, 0);
        blue = QVector<int>(mWidth * mHeight, 0);
    }
    // red.clear();
    // green.clear();
    // blue.clear();
    // red.resize(mWidth * mHeight);
    // green.resize(mWidth * mHeight);
    // blue.resize(mWidth * mHeight);

    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    int line = 0;
    for(int i = mHeight; i > 0; --i)
    {
        line = (i - 1) * mWidth;
        for(int j = 0; j < mWidth; ++j)
        {
            stream >> b;
            stream >> g;
            stream >> r;
            red[line + j] = r;
            green[line + j] = g;
            blue[line + j] = b;
        }
        stream.skipRawData(pad);
    }
    setMinimumSize(mWidth, mHeight);
}

QSize pic::sizeHint()
{
    return QSize(mWidth, mHeight);
}

void pic::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int line = 0;
    QImage image(mWidth, mHeight, QImage::Format_ARGB32);
    for (int i = 0; i <  mHeight; ++i)
    {
        line = i * mWidth;
        for( int j = 0; j < mWidth; ++j)
        {
            QRgb c = qRgb(red[line + j], green[line + j], blue[line + j]);
            image.setPixel(j, i, c);
        }
    }
    painter.drawImage(0, 0, image);
	setMinimumSize(mWidth, mHeight);
}

void pic::paintRed()
{
    green = QVector<int>(mWidth * mHeight, 0);
    blue = green;
    repaint();
}

void pic::paintGreen()
{
    red = QVector<int>(mWidth * mHeight, 0);
    blue = red;
    repaint();
}

void pic::paintBlue()
{
    red = QVector<int>(mWidth * mHeight, 0);
    green = red;
    repaint();
}

void pic::compressByRow()
{
    int line = 0;
    int top = 0;
    int bottom = 0;
    int middle = mWidth * mHeight/2;
    QVector<int> r(mWidth * mHeight);
    QVector<int> g(mWidth * mHeight);
    QVector<int> b(mWidth * mHeight);
    for (int i = 0; i < mHeight; i += 2)
    {
        line = i * mWidth;
        top = mWidth *  i/2;
        bottom = top + middle;
        for(int j = 0; j < mWidth; ++j)
        {
            r[top + j] = red[line + j] + red[line + j + mWidth];
            r[bottom + j] = qAbs(red[line + j] - red[line + j + mWidth]);
            g[top + j] = green[line + j] + green[line + j + mWidth];
            g[bottom + j] = qAbs(green[line + j] - green[line + j + mWidth]);
            b[top + j] = blue[line + j] + blue[line + j + mWidth];
            b[bottom + j] = qAbs(blue[line + j] - blue[line + j + mWidth]);
        }
    }
    red = r;
    green = g;
    blue = b;
}
void pic::compressByColumn()
{
    int column = 0;
    int left = 0;
    int right = 0;
    int middle = mWidth/2;
    QVector<int> r(mWidth * mHeight);
    QVector<int> g(mWidth * mHeight);
    QVector<int> b(mWidth * mHeight);
    for (int i = 0; i < mWidth; i += 2)
    {
        for(int j = 0; j < mHeight; ++j)
        {
            column = j * mWidth;
            left = column + i/2;
            right = left + middle;
            r[left] = red[column + i] + red[column + i + 1];
            r[right] = qAbs(red[column + i] - red[column + i + 1]);
            g[left] = green[column + i] + green[column + i + 1];
            g[right] = qAbs(green[column + i] - green[column + i + 1]);
            b[left] = blue[column + i] + blue[column + i + 1];
            b[right] = qAbs(blue[column + i] - blue[column + i + 1]);
        }
    }
    red = r;
    green = g;
    blue = b;
}

void pic::compress()
{
    compressByRow();
    compressByColumn();
}

void pic::inverse()
{
    int line = 0;
    for (int i = 0; i < mHeight; ++i)
    {
        line = i * mWidth;
        for(int j = 0; j < mWidth; ++j)
        {
            red[line + j] = 255 - red[line + j];
            green[line + j] = 255 - green[line + j];
            blue[line + j] = 255 - blue[line + j];
        }
    }
}

void pic::addBrightness(const int scale)
{
    int line = 0;
    for (int i = 0; i < mHeight; ++i)
    {
        line = i * mWidth;
        for(int j = 0; j < mWidth; ++j)
        {
            red[line + j] += scale ;
            green[line + j] += scale;
            blue[line + j] += scale;
        }
    }
    repaint();
}

void pic::minMax()
{
    unsigned char rMin = 255;
    unsigned char gMin = 255;
    unsigned char bMin = 255;
    unsigned char rMax = 0;
    unsigned char gMax = 0;
    unsigned char bMax = 0;

    int line = 0;
    for (int i = 0; i < mHeight; ++i)
    {
        line = i * mWidth;
        for(int j = 0; j < mWidth; ++j)
        {
            if(red[line + j] < rMin)
                rMin = red[line + j];
            if(red[line + j] > rMax)
                rMax = red[line + j];
            if(green[line + j] < gMin)
                gMin = green[line + j];
            if(green[line + j] > gMax)
                gMax = green[line + j];
            if(red[line + j] < bMin)
                bMin = blue[line + j];
            if(blue[line + j] > bMax)
                bMax = blue[line + j];
        }
    }
    unsigned char rDiff = rMax - rMin;
    unsigned char gDiff = gMax - gMin;
    unsigned char bDiff = bMax - bMin;
    for (int i = 0; i < mHeight; ++i)
    {
        line = i * mWidth;
        for(int j = 0; j < mWidth; ++j)
        {
            red[line + j] = 255 * (red[line + j] - rMin) / rDiff;
            green[line + j] = 255 * (green[line + j] - gMin) / gDiff;
            blue[line + j] = 255 * (blue[line + j] - bMin) / bDiff;
        }
    }

}

void pic::middle()
{
    long rSum = 0;
    long gSum = 0;
    long bSum = 0;

    int line = 0;
    for (int i = 0; i < mHeight; ++i)
    {
        line = i * mWidth;
        for(int j = 0; j < mWidth; ++j)
        {
            rSum += red[line + j];
            gSum += green[line + j];
            bSum += blue[line + j];
        }
    }

    unsigned char rMid = rSum / (mWidth * mHeight);
    unsigned char gMid = gSum / (mWidth * mHeight);
    unsigned char bMid = bSum / (mWidth * mHeight);

    for (int i = 0; i < mHeight; ++i)
    {
        line = i * mWidth;
        for(int j = 0; j < mWidth; ++j)
        {
            red[line + j] *= red[line + j] / rMid;
            green[line + j] *= green[line + j] / gMid;
            blue[line + j] *= blue[line + j] / bMid;
        }
    }
}

void pic::canny()
{
	QVector<int> r(red.size());
	QVector<int> g(green.size());
	QVector<int> b(blue.size());

	int prev = 0;
	int cur = 0;
	int next = 0;

	for (int i = 1; i < mHeight - 1; ++i)
	{
		for(int j = 1; j < mWidth - 1; ++j)
		{
			prev = (i-1) * mWidth + j;
			cur = i * mWidth + j;
			next = (i+1) * mWidth + j;

			r[cur] = (int)red[prev - 1] + (int)2*red[prev] + (int)red[prev + 1] -
				(int)((int)red[next - 1] + (int)2*red[next] + (int)red[next + 1]);
			g[cur] = (int)green[prev - 1] + (int)2*green[prev] + (int)green[prev + 1] -
				((int)green[next - 1] + (int)2*green[next] + (int)green[next + 1]);
			b[cur] = (int)blue[prev - 1] + (int)2*blue[prev] + (int)blue[prev + 1] -
				((int)blue[next - 1] + (int)2*blue[next] + (int)blue[next + 1]);
		}
	}
	red = r;
	green = g;
	blue = b;
}

void pic::halving()
{
	QVector<int> r;
	QVector<int> g;
	QVector<int> b;

    int line = 0;
    for (int i = 0; i < mHeight; i+=2)
    {
        line = i * mWidth;
        for(int j = 0; j < mWidth/2; ++j)
        {
			r.push_back(red[line + 2*j]);
			g.push_back(green[line + 2*j]);
			b.push_back(blue[line + 2*j]);
        }
    }

	mWidth /= 2;
	mHeight /= 2;

	red = r;
	green = g;
	blue = b;
}

void pic::rotateRight()
{
	//TODO reserve memory for these vectors
	QVector<int> r;
	QVector<int> g;
	QVector<int> b;

    int ind = 0;
    for (int i = mWidth - 1; i >= 0; --i)
    {
        for(int j = 0; j < mHeight; ++j)
        {
			ind = j * mWidth + i - 1;
			r.push_back(red[ind]);
			g.push_back(green[ind]);
			b.push_back(blue[ind]);
        }
    }

	int t = mHeight;
	mHeight = mWidth;
	mWidth = t;


	red = r;
	green = g;
	blue = b;
}

void pic::rotateLeft()
{
	//TODO reserve memory for these vectors
	QVector<int> r;
	QVector<int> g;
	QVector<int> b;

    int ind = 0;
    for (int i = 0; i < mWidth; ++i)
    {
        for(int j = mHeight - 1; j >= 0; --j)
        {
			ind = j * mWidth + i;
			r.push_back(red[ind]);
			g.push_back(green[ind]);
			b.push_back(blue[ind]);
        }
    }

	int t = mHeight;
	mHeight = mWidth;
	mWidth = t;


	red = r;
	green = g;
	blue = b;
}

void pic::doHadamard()
{
	// compute the n for Hadamard function
	int n = 0;
	int mask = 1;
	while(! (mWidth & mask))
	{
		mask = mask << 1;
		++n;
	}
	std::cout << "degree of hadamard is " << n << std::endl;

	std::unique_ptr<QVector<int> > h = hadamard(n);
	std::unique_ptr<QVector<int> > h_tr = traverse(*h, mask);

	QVector<int> temp(red.size());
	// red
	mult(*h, red, temp, mask);
	mult(temp, *h_tr, red, mask);

	// green
	mult(*h, green, temp, mask);
	mult(temp, *h_tr, green, mask);

	//blue
	mult(*h, blue, temp, mask);
	mult(temp, *h_tr, blue, mask);
}
