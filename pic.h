// @file pic.h
// declares class pic

#ifndef PIC_H
#define PIC_H

#include <QString>
#include <QImage>
#include <QtWidgets/QWidget>
#include <QVector>

class QPaintEvent;

class pic : public QWidget
{
    Q_OBJECT
public:
    // constructors
    pic(QWidget *parent = 0);
    pic(const QString& filename, QWidget * parent = 0);

    // @function setFilename
    // sets the filename
    // @param name name of file to work with
    void setFilename(const QString &name)
    {
        mFilename = name;
    
    }

    // @function init()
    // initiatiate pic, read from file
    // construct image
    void init(const QString &filename = QString());

    // @function height
    // gives the height of image
    // @return height of image
    int height() const
    {
        return mHeight;
    }

    // @function width
    // gives the width of image
    // @return width of image
    int width() const
    {
        return mWidth;
    }

    // @function compress
    // compresses by row and column
    void compress();

    // @function inverse
    // inverses all colors
    void inverse();


    void paintRed();
    void paintGreen();
    void paintBlue();

    void minMax();
    void middle();

	void halving();

	void rotateRight();
	void rotateLeft();

	void doHadamard();

    // @function canny
	// multiply every 3x3 range with 
	// [ 1   2  1 ]    [ 1  0 -1 ]
	// | 0   0  0 | or | 2  0 -2 | matrix
	// [ -1 -2 -1 ]    [ 1  0 -1 ]
    void canny();
public slots:
    void addBrightness(const int scale);

protected:
    // @function overloaded function paintEvent(QPaintEvent *)
    // draws widget
    void paintEvent(QPaintEvent *);

    QSize sizeHint();
private:
    // @function compressByColumn()
    // compresses image by column
    void compressByColumn();

    // @function compressByRow()
    // compresses image by row
    void compressByRow();

    QString mFilename;
    int mWidth;
    int mHeight;
    QVector<int> red;
    QVector<int> green;
    QVector<int> blue;
};

#endif // pic.h
