#ifndef FILECLIENT_H
#define FILECLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QJsonDocument>
#include <QJsonObject>

class FileClient : public QObject
{
    Q_OBJECT
public:
    explicit FileClient(QObject *parent = nullptr);
    Q_INVOKABLE void sendRequest(const QString &command, const QString &path, const QString &newPath = "", const QString &content = "");

signals:
    void responseReceived(const QString &response);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *udpSocket;
};

#endif // FILECLIENT_H
