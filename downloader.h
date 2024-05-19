#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMutex>
#include <QHash>

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);
    virtual ~Downloader();

    void download(const QString &url, std::function<void(QByteArray)> callBack);

private:
    QNetworkAccessManager *m_networkManager;
    QMutex m_loadMutex;
    QHash<QString, std::function<void(const QByteArray)>> m_requests;
};

#endif // DOWNLOADER_H
