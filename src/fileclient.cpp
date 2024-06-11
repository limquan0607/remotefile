#include "fileclient.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

FileClient::FileClient(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, &QUdpSocket::readyRead, this, &FileClient::readPendingDatagrams);
    udpSocket->bind(QHostAddress::LocalHost, 0);  // Bind to any available port
}

void FileClient::sendRequest(const QString &command, const QString &path, const QString &newPath, const QString &content)
{
    QJsonObject requestObj;
    requestObj["command"] = command;
    requestObj["path"] = path;
    requestObj["newPath"] = newPath;
    requestObj["content"] = content;

    QJsonDocument requestDoc(requestObj);
    QByteArray requestData = requestDoc.toJson();

    udpSocket->writeDatagram(requestData, QHostAddress::LocalHost, 45454);  // Assuming the server is on the same machine and port 45454
}

void FileClient::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        QJsonDocument responseDoc = QJsonDocument::fromJson(datagram);
        QJsonObject responseObj = responseDoc.object();
        emit responseReceived(QString::fromUtf8(QJsonDocument(responseObj).toJson(QJsonDocument::Indented)));
    }
}
