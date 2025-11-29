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

void ServerManager::broadcastClientList()
{
    QStringList names;
    // Store all the names in a list
    for(int i = 0; i < _clients.length(); i++)
    {
        names.append(_clients[i]->property("name").toString());
    }

    // Broadcast to all the clients the new list
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << QString("CLIENT_LIST") << names;
    for(int i = 0; i < _clients.length(); i++)
    {
        _clients[i]->write(data);
    }
}

void ServerManager::broadcastChatMessage(const QString& message)
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out << QString("CHAT") << message;
    for(int i = 0; i < _clients.length(); i++)
    {
        _clients[i]->write(data);
    }
}

void ServerManager::OnNewClientReceived()
{
    auto client = _server->nextPendingConnection();
    // Add new client to the list
    _clients.append(client);

    // Assign ID as a property to client
    auto id = _clients.count();
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

        // Broadcast the client list again after a client disconnects
        broadcastClientList();
    }
}

void ServerManager::OnReadyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if(!client) return;

    QDataStream in(client);

    while(client->bytesAvailable() > 0)
    {
        QString header;
        in >> header;

        if(header == "SET_NAME")
        {
            QString name;
            in >> name;
            client->setProperty("name", name);

            qDebug() << "User connected: " << name;

            // Broadcast the new client list to everyone
            broadcastClientList();
        }
        else if(header == "CHAT")
        {
            QString message;
            in >> message;

            QString fullMessage = client->property("name").toString() + ": " + message;

            broadcastChatMessage(fullMessage);
        }
        else
        {
            qWarning() << "Unknown header received:" << header;
        }
    }
}
