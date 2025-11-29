#include "client_manager.h"

ClientManager::ClientManager(QObject *parent)
    : QObject{parent}
{
    socket_ = new QTcpSocket(this);

    connect(socket_, &QTcpSocket::connected, this, [this]() {
        emit OnConnectionSuccessful();
    });

    connect(socket_, &QTcpSocket::errorOccurred, this, [this]() {
        emit OnConnectionFailed(socket_->errorString());
    });

    connect(socket_, &QTcpSocket::readyRead, this, &ClientManager::onReadyRead);
}

void ClientManager::connectToServer(const QString &address, int port)
{
    socket_->connectToHost(address, port);
}

bool ClientManager::isConnected() const
{
  return socket_->state() == QTcpSocket::ConnectedState;
}

QTcpSocket *ClientManager::getSocket()
{
    return socket_;
}

void ClientManager::sendMessageToServer(const QString header, const QString message)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    out << header << message;
    socket_->write(data);
    socket_->flush();
}

void ClientManager::onReadyRead()
{
    QDataStream in(socket_);

    while(socket_->bytesAvailable() > 0)
    {
        QString header;
        in >> header;

        if(header == "CLIENT_LIST")
        {
            QStringList names;
            in >> names;
            emit clientListUpdated(names);
        }
        else if(header == "CHAT")
        {
            QString msg;
            in >> msg;
            emit chatMessageReceived(msg);
        }
        else
        {
            qWarning() << "Unknown header received:" << header;
        }
    }
}
