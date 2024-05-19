#include "downloader.h"

Downloader::Downloader(QObject *parent)
    : QObject(parent), m_networkManager(new QNetworkAccessManager(this))
{
    connect(m_networkManager, &QNetworkAccessManager::finished, [this](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            const QByteArray imageData = reply->readAll();
            QString urlValue = reply->request().url().toString();
            if (m_requests[urlValue])
            {
                m_loadMutex.lock();
                auto cb = m_requests[urlValue];
                m_requests.remove(urlValue);
                m_loadMutex.unlock();
                cb(imageData);
            }
        }

        reply->deleteLater();
    });
}

Downloader::~Downloader()
{
    if (m_networkManager)
        delete m_networkManager;
}

void Downloader::download(const QString &url, std::function<void (const QByteArray)> callBack) {
    QMutexLocker ml(&m_loadMutex);
    QNetworkRequest request((QUrl(url)));
    m_networkManager->get(request);
    m_requests[url] = callBack;
}
