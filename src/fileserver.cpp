#include "fileserver.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

FileServer::FileServer(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, &QUdpSocket::readyRead, this, &FileServer::processPendingDatagrams);
}

void FileServer::startServer(quint16 port)
{
    udpSocket->bind(QHostAddress::Any, port);
}

void FileServer::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        QByteArray response = handleRequest(datagram);
        udpSocket->writeDatagram(response, sender, senderPort);
    }
}

QByteArray FileServer::handleRequest(const QByteArray &request)
{
    QJsonDocument requestDoc = QJsonDocument::fromJson(request);
    QJsonObject requestObj = requestDoc.object();

    QString command = requestObj["command"].toString();
    QString path = requestObj["path"].toString();
    QString newPath = requestObj["newPath"].toString();
    QString content = requestObj["content"].toString();

    QJsonObject responseObj;

    if (command == "create") {
        responseObj["result"] = createFile(path);
    } else if (command == "delete") {
        responseObj["result"] = deleteFile(path);
    } else if (command == "rename") {
        responseObj["result"] = renameFile(path, newPath);
    } else if (command == "read") {
        responseObj["result"] = readFile(path);
    } else if (command == "write") {
        responseObj["result"] = writeFile(path, content);
    } else {
        responseObj["result"] = false;
    }

    QJsonDocument responseDoc(responseObj);
    return responseDoc.toJson();
}

bool FileServer::createFile(const QString &path)
{
    QFile file(path);
    if (file.exists()) {
        return false;
    }
    return file.open(QIODevice::WriteOnly);
}

bool FileServer::deleteFile(const QString &path)
{
    QFile file(path);
    if (!file.exists()) {
        return false;
    }
    return file.remove();
}

bool FileServer::renameFile(const QString &oldPath, const QString &newPath)
{
    QFile file(oldPath);
    if (!file.exists()) {
        return false;
    }
    return file.rename(newPath);
}

QString FileServer::readFile(const QString &path)
{
    QFile file(path);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }

    QTextStream in(&file);
    return in.readAll();
}

bool FileServer::writeFile(const QString &path, const QString &content)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << content;
    file.close();
    return true;
}
