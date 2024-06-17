#ifndef DATAGRAM_H
#define DATAGRAM_H

#include <QByteArray>
#include <QtTypes>

const QByteArray MAGIC_WORD = "MAGIC";

QByteArray createDatagram(const QByteArray &payload);
bool validateDatagram(const QByteArray &datagram, QByteArray &payload);

#endif // DATAGRAM_H
