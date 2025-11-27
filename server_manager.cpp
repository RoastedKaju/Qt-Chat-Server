#include "server_manager.h"

#include <QDebug>

ServerManager::ServerManager(QObject *parent)
    : QObject{parent}
{

}

void ServerManager::setupServer(ushort port)
{
    _server = new QTcpServer(this);

    // Bind event on new connection
    connect(_server, &QTcpServer::newConnection, this, &ServerManager::OnNewClientReceived);

    // Listen for incomming connections
    if(!_server->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server failed to start" << _server->errorString();
        return;
    }

    qDebug() << "Listening for incoming connections...";
}

void ServerManager::OnNewClientReceived()
{
    auto client = _server->nextPendingConnection();
    // Add new client to the list
    _clients.append(client);

    // Assign ID as a property to client
    auto id = _clients.length();
    client->setProperty("id", id);

    // Bind event on ready read
    connect(client, &QTcpSocket::readyRead, this, &ServerManager::OnReadyRead);

    // Bind an event when client gets disconnected
    connect(client, &QTcpSocket::disconnected, this, &ServerManager::OnClientDisconnected);

    emit OnServerManagerClientConnected(client);

    qDebug() << "New Client Received... " << id;
}

void ServerManager::OnClientDisconnected()
{
    auto client = qobject_cast<QTcpSocket*>(sender());

    if(client)
    {
        const int id = client->property("id").toInt();

        _clients.removeAll(client);
        client->deleteLater();

        qDebug() << "Client disconnected... " << id;
    }
}

void ServerManager::OnReadyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());

    if(!client)
        return;

    // Read data from client
    QByteArray data = client->readAll();
    qDebug() << "Received data... " << data;

    // Broadcast to all other clients
    for (int i = 0; i < _clients.size(); ++i)
    {
        QTcpSocket *other = _clients[i];

        if (other != client)
        {
            other->write(data);
        }
    }
}
