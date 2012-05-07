#include <QtGui>
#include <QtNetwork>
//#include <QDebug>

#include "extio_tinysdr.h"

// Hardware buffer size
#define EXTIO_BLOCKZ 4096
#define EXTIO_BLOCKZSIZE (EXTIO_BLOCKZ*2*sizeof(qint16))


ExtIO::ExtIO(QWidget *parent)
    : QWidget(parent)
{
    // TODO : Disable close windows button top/right

    SR = 48000;         // Defaul Sampling Rate
    LO = 7024000;       // Default Local Oscillator

    datagram = new char[EXTIO_BLOCKZSIZE];

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress);
    //udpSocket->bind(QHostAddress("192.168.157.132"), 45454);

    connect(udpSocket, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));

    statusLabel = new QLabel(tr("Listening for broadcasted messages"));
    statusLabel->setWordWrap(true);

    labelLO = new QLabel(tr("LO=%1").arg(LO));
    labelLO->setWordWrap(true);

    hideButton = new QPushButton(tr("&Hide"));
    connect(hideButton, SIGNAL(clicked()), this, SLOT(hide()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(hideButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(labelLO);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Broadcast Receiver"));
}


void ExtIO::processPendingDatagrams() {
    while (udpSocket->hasPendingDatagrams() == TRUE) {
        udpSocket->readDatagram(datagram, EXTIO_BLOCKZSIZE);
        // FIXME, udpSocket return the size of data retrieve, need check!
        WinradCallBack( EXTIO_BLOCKZ, 0, 0, datagram );
    }
    //statusLabel->setText(tr("Receiving in progress"));
}


long ExtIO::GetSR() {
    return SR;
}


long ExtIO::GetLO() {
    return LO;
}


int ExtIO::GetPS() {
    return EXTIO_BLOCKZ;
}


void ExtIO::SetLO(long newLO) {
     LO = newLO;
     labelLO->setText(tr("LO=%1").arg(LO));
     // +++ Send an TCP datagram for the update / i2c uchip
}
