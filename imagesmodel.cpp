#include "imagesmodel.h"

#include <QObject>
#include <QStandardPaths>
#include <QJsonObject>
#include <QTemporaryFile>
#include <QSharedPointer>
#include <QDebug>

#include "downloader.h"

struct ImageData {
    QString title;
    QString name;
    int orderNumber = 0;
    QString displaySrc;
    QString normalSrc;
    QString focusSrc;
    QSharedPointer<QTemporaryFile> displayFile;
    QSharedPointer<QTemporaryFile> normalFile;
    QSharedPointer<QTemporaryFile> focusFile;
    bool loaded = false;

    void checkLoaded() {
        loaded = displayFile && normalFile && focusFile;
    }
};

class ImagesModelPrivate {
public:
    ImagesModelPrivate(ImagesModel* p)
        : p(p)
        , m_downloader()
    {
    }

    void parseJson(const QJsonArray &imagesArray) {
        for (const QJsonValue &imageValue : imagesArray) {
            ImageData imageData;
            imageData.title = imageValue.toObject().value("title").toString();
            imageData.name = imageValue.toObject().value("componentName").toString();
            imageData.orderNumber = imageValue.toObject().value("ordinality").toInt();
            imageData.displaySrc = imageValue.toObject().value("displayImgSrc").toString();
            imageData.normalSrc = imageValue.toObject().value("normalSrc").toString();
            imageData.focusSrc = imageValue.toObject().value("focusSrc").toString();
            p->beginInsertRows(QModelIndex(), m_images.size(), m_images.size() + 1);
            m_images.push_back(std::move(imageData));
            p->endInsertRows();
        }

        loadImages();
    }

    void loadImages()
    {
        for(int idx = 0; idx < m_images.size(); ++idx)
        {
            ImageData& imageData = m_images[idx];
            m_downloader.download(imageData.displaySrc, [this, &imageData, idx](const QByteArray& data)
            {
                imageData.displayFile = QSharedPointer<QTemporaryFile>::create();
                if (data.size() > 0 && imageData.displayFile->open()) {
                    imageData.displayFile->write(data);
                    imageData.displayFile->flush();
                    p->dataChanged(p->index(idx), p->index(idx));
                }
                imageData.checkLoaded();
                checkImdagesLoaded();
            });
            m_downloader.download(imageData.normalSrc, [this, &imageData, idx](const QByteArray& data)
            {
                imageData.normalFile = QSharedPointer<QTemporaryFile>::create();
                if (data.size() > 0 && imageData.normalFile->open()) {
                    imageData.normalFile->write(data);
                    imageData.normalFile->flush();
                    p->dataChanged(p->index(idx), p->index(idx));
                }
                imageData.checkLoaded();
                checkImdagesLoaded();
            });
            m_downloader.download(imageData.focusSrc, [this, &imageData, idx](const QByteArray& data)
            {
                imageData.focusFile = QSharedPointer<QTemporaryFile>::create();
                if (data.size() > 0 && imageData.focusFile->open()) {
                    imageData.focusFile->write(data);
                    imageData.focusFile->flush();
                    p->dataChanged(p->index(idx), p->index(idx));
                }
                imageData.checkLoaded();
                checkImdagesLoaded();
            });
        }
    }

    void checkImdagesLoaded()
    {
        bool loaded = true;
        for (const ImageData& data : m_images)
        {
            if (!data.loaded)
            {
                loaded = false;
                break;
            }
        }
        if (loaded)
        {
            p->setLoading(false);
            p->setCurrent(0);
        }
    }

    ImagesModel* p;
    Downloader m_downloader;
    QVector<ImageData> m_images;
};

ImagesModel::ImagesModel(QObject *parent)
    : QAbstractListModel(parent)
    , impl(new ImagesModelPrivate(this))
{
}

ImagesModel::~ImagesModel()
{
    if (impl)
    {
        delete impl;
    }
}

int ImagesModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return impl->m_images.count();
}

QVariant ImagesModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= impl->m_images.size() || index.row() < 0)
        return QVariant();

    const ImageData &image = impl->m_images[index.row()];

    if (role == ImageSrcNormal)
        return image.normalFile->fileName();
    else if (role == ImageSrcFocus)
        return image.focusFile->fileName();
    else if (role == ImageSrcDisplay)
        return image.displayFile->fileName();

    return QVariant();
}

QHash<int, QByteArray> ImagesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ImageSrcNormal] = "imageSrc";
    roles[ImageSrcFocus] = "imageSrcFocus";
    roles[ImageSrcDisplay] = "imageSrcDislay";
    return roles;
}

bool ImagesModel::loading() const
{
    return m_loading;
}

void ImagesModel::setLoading(bool value)
{
    if (m_loading == value)
        return;
    m_loading = value;
    emit loadingChanged();
}

int ImagesModel::current() const
{
    return m_current;
}

void ImagesModel::setCurrent(int value)
{
    m_current = value;
    emit currentChanged();
}

QString ImagesModel::currentImage() const
{
    if (m_current >= 0 && m_current < impl->m_images.size())
    {
        return impl->m_images[m_current].displayFile->fileName();
    }
    return "";
}

void ImagesModel::increase()
{
    if (m_current < impl->m_images.size() - 1)
    {
        setCurrent(m_current + 1);
    }
}

void ImagesModel::decrease()
{
    if (m_current > 0)
    {
        setCurrent(m_current - 1);
    }
}

void ImagesModel::load(const QJsonArray &settings)
{
    setLoading(true);
    impl->parseJson(settings);
}
