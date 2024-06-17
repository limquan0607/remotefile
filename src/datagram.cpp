#include "datagram.h"
#include <QByteArray>
#include <QtTypes>
#include <QDataStream>
#include <QDebug>

QByteArray createDatagram(const QByteArray &payload)
{
    QByteArray datagram(MAGIC_WORD, MAGIC_WORD.size());
    datagram.append(payload);
    quint16 crc = qChecksum(payload);
    datagram.append(reinterpret_cast<char*>(&crc), sizeof(crc));
    return datagram;
}

bool validateDatagram(const QByteArray &datagram, QByteArray &payload)
{
    if (!datagram.startsWith(MAGIC_WORD)) {
        return false;
    }
    payload = datagram.mid(MAGIC_WORD.size(), datagram.size() - MAGIC_WORD.size() - sizeof(quint16));
    QByteArray crcData = datagram.right(sizeof(quint16));
    QDataStream ds(crcData);
    quint16 receivedCrc;
    ds >> receivedCrc;
    quint16 crc = qChecksum(payload);
    return crcData == QByteArray(reinterpret_cast<char*>(&crc), sizeof(quint16));
}
