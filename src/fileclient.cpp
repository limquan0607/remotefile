#include "fileclient.h"
#include "datagram.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <QString>

FileClient::FileClient(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, &QUdpSocket::readyRead, this, &FileClient::readPendingDatagrams);
    udpSocket->bind(QHostAddress::LocalHost, 0);  // Bind to any available port
}

void FileClient::sendRequest(const QString &command, const QString &name, const QString &newPath, const QString &content)
{
    QJsonObject requestObj;
    requestObj["command"] = command;
    requestObj["name"] = name;
    requestObj["newPath"] = newPath;
    requestObj["content"] = content;
    QJsonDocument requestDoc(requestObj);
    QByteArray requestData = requestDoc.toJson();
    QByteArray finalRequest = createDatagram(requestData);
    udpSocket->writeDatagram(finalRequest, QHostAddress::LocalHost, 45454);  // Assuming the server is on the same machine and port 45454
}

void FileClient::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        QByteArray payload;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        if (!validateDatagram(datagram, payload)) {
            emit error("Datagram compromised");
            continue;
        }

        QJsonDocument responseDoc = QJsonDocument::fromJson(payload);
        QJsonObject responseObj = responseDoc.object();
        // qDebug() << responseObj;
        bool result = responseObj["result"].toBool();
        if (!result) {
            emit error("Something go wrong");
        } else {
            if (responseObj.contains("files")) {
                QStringList fileList;
                QList<bool> isDir;
                for (const QJsonValue &value : responseObj["files"].toArray()) {
                    fileList.append(value.toObject()["name"].toString());
                    isDir.append(value.toObject()["isDir"].toBool());
                }
                emit fileListReceived(fileList, isDir);
            }

            if (responseObj.contains("downloadContent")) {
                QString content = responseObj["downloadContent"].toString();
                if (download_file.exists()) {
                    emit error("File with that name already existed in directory");
                    continue;
                }
                download_file.open(QIODevice::WriteOnly);
                QTextStream out(&download_file);
                out << content;
                download_file.close();
            }
        }
    }
}

void FileClient::requestFileList()
{
    sendRequest("list");
}

void FileClient::uploadFile(const QString &pathName)
{
    QString fileName = pathName.sliced(7);
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    QString name = fileName.sliced(fileName.lastIndexOf("/") + 1);
    sendRequest("upload", name, "", content);
}

void FileClient::downloadFile(const QString &name,const QString &location)
{
    QString loc_name = location.sliced(7);
    QString path =  loc_name + "/" + name;
    download_file.setFileName(path);
    sendRequest("download", name, "", "");
}


