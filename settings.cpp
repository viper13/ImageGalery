#include "settings.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "downloader.h"

class SettingsPrivate {
public:
    SettingsPrivate(Settings* p)
    : p(p)
    , m_downloader()
    {
        m_downloader.download("https://cdn-media.brightline.tv/dev/templates/image_gallery_exercise.json", [this](const QByteArray data)
        {
            if (data.size() > 0 )
            {
                if (!parseJson(data))
                {
                    qWarning() << "Failed to parse JSON: " << data << "\n";
                    return;
                }
            }
        });
    }

    bool parseJson(const QByteArray &data) {
        QJsonParseError parseErr;
        QJsonDocument document = QJsonDocument::fromJson(data, &parseErr);
        if (document.isNull()) {
            qWarning() << "Invalid JSON document: " << parseErr.errorString();
            return false;
        }

        const QJsonArray jsonArray = document.object().value("components").toArray();

        for (const QJsonValue &value : jsonArray) {
            if (value.isObject()) {
                const QJsonObject obj = value.toObject();
                if (obj.value("componentName").toString() == "carousel")
                {
                    p->m_images->load(obj.value("components").toArray());
                }
                else if (obj.value("componentName").toString() == "background")
                {
                    m_downloader.download(obj.value("componentSrc").toString(), [this](const QByteArray& data)
                    {
                        p->m_background = QSharedPointer<QTemporaryFile>::create();
                        if (data.size() > 0 && p->m_background->open())
                        {
                            p->m_background->write(data);
                            p->m_background->flush();
                            emit p->backgroundChanged();
                        }
                    });
                }
                else if (obj.value("componentName").toString() == "closeButton")
                {
                    m_downloader.download(obj.value("normalSrc").toString(), [this](const QByteArray& data)
                    {
                        p->m_closeBtn = QSharedPointer<QTemporaryFile>::create();
                        if (data.size() > 0 && p->m_closeBtn->open())
                        {
                            p->m_closeBtn->write(data);
                            p->m_closeBtn->flush();
                            emit p->closeBtnChanged();
                        }
                    });
                    m_downloader.download(obj.value("focusSrc").toString(), [this](const QByteArray& data)
                    {
                        p->m_closeBtnFocus = QSharedPointer<QTemporaryFile>::create();
                        if (data.size() > 0 && p->m_closeBtnFocus->open())
                        {
                            p->m_closeBtnFocus->write(data);
                            p->m_closeBtnFocus->flush();
                            emit p->closeBtnFocusChanged();
                        }
                    });
                }
            }
        }

        return true;
    }

    Settings* p;
    Downloader m_downloader;
};

Settings::Settings(QObject *parent)
    : QObject{parent}
    , impl(new SettingsPrivate(this))
    , m_images(new ImagesModel(parent))
{}

Settings::~Settings()
{
    if (impl)
        delete impl;
}

QString Settings::background() const
{
    return m_background ? m_background->fileName() : "";
}

ImagesModel *Settings::images()
{
    return m_images.data();
}

QString Settings::closeBtn() const
{
    return m_closeBtn ? m_closeBtn->fileName() : "";;
}

QString Settings::closeBtnFocus() const
{
    return m_closeBtnFocus ? m_closeBtnFocus->fileName() : "";;
}
