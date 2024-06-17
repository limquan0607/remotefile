#ifndef FILECLIENT_H
#define FILECLIENT_H

#include <QObject>
#include <QUdpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

class FileClient : public QObject
{
    Q_OBJECT
public:
    explicit FileClient(QObject *parent = nullptr);
    Q_INVOKABLE void sendRequest(const QString &command, const QString &name = "", const QString &newPath = "", const QString &content = "");
    Q_INVOKABLE void requestFileList();
    Q_INVOKABLE void uploadFile(const QString &fileName);
    Q_INVOKABLE void downloadFile(const QString &name,const QString &location);


signals:
    void responseReceived(const QString &response);
    void fileListReceived(const QStringList &files, const QList<bool> isDir);
    void error(const QString &message);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *udpSocket;
    QFile download_file;
};

#endif // FILECLIENT_H
