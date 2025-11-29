#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <QObject>
#include <QTcpSocket>

class ClientManager : public QObject
{
    Q_OBJECT
public:
    explicit ClientManager(QObject *parent = nullptr);

    void connectToServer(const QString& address, int port);
    bool isConnected() const;

    QTcpSocket* getSocket();

    void sendMessageToServer(const QString header, const QString message);

private:
    void onReadyRead();

signals:
    void OnConnectionSuccessful();
    void OnConnectionFailed(const QString& error);
    void clientListUpdated(const QStringList& clients);
    void chatMessageReceived(const QString& message);

private:
    QTcpSocket* socket_;
};

#endif // CLIENT_MANAGER_H
