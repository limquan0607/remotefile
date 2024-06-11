#ifndef FILESERVER_H
#define FILESERVER_H

#include <QObject>
#include <QUdpSocket>

class FileServer : public QObject
{
    Q_OBJECT
public:
    explicit FileServer(QObject *parent = nullptr);
    void startServer(quint16 port);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *udpSocket;
    QByteArray handleRequest(const QByteArray &request);
    bool createFile(const QString &path);
    bool deleteFile(const QString &path);
    bool renameFile(const QString &oldPath, const QString &newPath);
    QString readFile(const QString &path);
    bool writeFile(const QString &path, const QString &content);
};

#endif // FILESERVER_H
