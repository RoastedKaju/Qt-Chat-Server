#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <memory.h>

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(QObject *parent = nullptr);

    void setupServer(ushort port = 5500);

signals:
    void OnServerManagerClientConnected(QTcpSocket* client);

private slots:
    void OnNewClientReceived();
    void OnClientDisconnected();
    void OnReadyRead();

private:
    QTcpServer* _server;
    QList<QTcpSocket*> _clients;

private:

};

#endif // SERVER_MANAGER_H
