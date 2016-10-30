#ifndef WIDGET_H
#define WIDGET_H

#include "pic.h"

#include <QString>

class QPushButton;
class QSlider;

class pic;

class widget : public QWidget
{
    Q_OBJECT
public:
    explicit widget(QWidget *parent = 0);
    widget(const QString& filename, QWidget *parent = 0);
    ~widget();
protected:
    void update();
private slots:
    void onOpen();
    void onOriginal();
    void onRed();
    void onGreen();
    void onBlue();
    void onInverse();
    void onCompress();
	void onCanny();
	void onHalving();
	void onRotateRight();
	void onRotateLeft();
    void onBrightnessChanged(int);
	void onHadamard();
    void onSave();
private:
    void initButtons();
    void layout();
    void setConnections();

    pic *picture;

    QPushButton *openButton;
    QPushButton *originalButton;
    QPushButton *redButton;
    QPushButton *greenButton;
    QPushButton *blueButton;
    QPushButton *inverseButton;
    QPushButton *compressButton;
	QPushButton *cannyButton;
	QPushButton *halvingButton;
	QPushButton *rotateRightButton;
	QPushButton *rotateLeftButton;
	QPushButton *hadamardButton;
    QPushButton *saveButton;
    QSlider *brSlider;

    QString mFilename;

    int brightnessValue;
};

#endif // WIDGET_H
