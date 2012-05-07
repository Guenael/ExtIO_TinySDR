#ifndef EXTIO_TINYSDR_H
#define EXTIO_TINYSDR_H

#include <QWidget>


QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QUdpSocket;
class QAction;
QT_END_NAMESPACE


class ExtIO : public QWidget
{
    Q_OBJECT

public:
    ExtIO(QWidget *parent = 0);
    void (* WinradCallBack)(int, int, float, void *);

    long GetSR();
    long GetLO();
    int  GetPS();
    void SetLO(long newLO);

private slots:
    void processPendingDatagrams();

private:
    QLabel *statusLabel, *labelLO;
    QPushButton *hideButton;
    QUdpSocket *udpSocket;

    char *datagram;

    long SR;
    long LO;
    //int  PS;
};

#endif
