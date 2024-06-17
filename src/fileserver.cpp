#include "fileserver.h"
#include "datagram.h"
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
    curDir.setCurrent(curDir.homePath());
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
        QByteArray payload;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if (!validateDatagram(datagram, payload)) {
            QByteArray response = createDatagram("Invalid datagram received.");
            udpSocket->writeDatagram(response, sender, senderPort);
            continue;
        }
        QByteArray response = handleRequest(payload);
        QByteArray finalResponse = createDatagram(response);
        udpSocket->writeDatagram(finalResponse, sender, senderPort);
    }
}

QByteArray FileServer::handleRequest(const QByteArray &request)
{
    QJsonDocument requestDoc = QJsonDocument::fromJson(request);
    QJsonObject requestObj = requestDoc.object();

    QString command = requestObj["command"].toString();
    QString name = requestObj["name"].toString();
    QString newPath = requestObj["newPath"].toString();
    QString content = requestObj["content"].toString();

    QJsonObject responseObj;
    QString path = curDir.path();
    path.append("/");
    path.append(name);
    // qDebug() << path;
    if (command == "create") {
        responseObj["result"] = createFile(path);
    } else if(command == "cd") {
        responseObj["result"] = curDir.cd(name);
    } else if(command == "mkdir") {
        responseObj["result"] = curDir.mkdir(name);
    } else if(command == "cdUp") {
        responseObj["result"] = curDir.cdUp();
    } else if (command == "delete") {
        responseObj["result"] = deleteFile(path);
    } else if (command == "rename") {
        responseObj["result"] = renameFile(path, newPath);
    } else if (command == "read") {
        responseObj["result"] = readFile(path);
    } else if (command == "write") {
        responseObj["result"] = writeFile(path, content);
    } else if (command == "list") {
        responseObj["result"] = true;
    } else if (command == "upload") {
        responseObj["result"] = uploadFile(path, content);
    } else if (command == "download") {
        QFile file(path);
        if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            responseObj["result"] = false;
        } else {
            QTextStream in(&file);
            QString downloadContent = in.readAll();
            responseObj["downloadContent"] = downloadContent;
            responseObj["result"] = true;
        }
    } else {
        responseObj["result"] = false;
    }

    if (responseObj["result"] == true) {
        QJsonArray fileList;
        QFileInfoList entries = curDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::AllDirs);
        foreach (QFileInfo entry, entries) {
            QJsonObject fileObj;
            fileObj["name"] = entry.fileName();
            fileObj["isDir"] = entry.isDir();
            fileList.append(fileObj);
        }
        // QJsonArray jsonArray = QJsonArray::fromStringList(files);
        responseObj["files"] = fileList;
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

bool FileServer::uploadFile(const QString &path, const QString &content) {
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << content;
        file.close();
        return true;
    }
    return false;
}
