#ifndef IMAGESMODEL_H
#define IMAGESMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QJsonArray>

class ImagesModelPrivate;

class ImagesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged FINAL)
    Q_PROPERTY(int current READ current WRITE setCurrent NOTIFY currentChanged FINAL)
    Q_PROPERTY(QString currentImage READ currentImage NOTIFY currentChanged FINAL)

    explicit ImagesModel(QObject *parent = nullptr);
    virtual ~ImagesModel();

    enum {
        ImageSrcNormal = Qt::UserRole + 1,
        ImageSrcFocus,
        ImageSrcDisplay
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool loading() const;
    void setLoading(bool value);

    int current() const;
    void setCurrent(int value);

    QString currentImage() const;

    Q_INVOKABLE void increase();
    Q_INVOKABLE void decrease();

    void load(const QJsonArray& settings);

signals:
    void loadingChanged();

    void currentChanged();

private:
    friend class ImagesModelPrivate;
    ImagesModelPrivate* impl;
    bool m_loading = false;
    int m_current = 0;
};

#endif // IMAGESMODEL_H
