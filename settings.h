#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QTemporaryFile>
#include "imagesmodel.h"

class SettingsPrivate;
class Settings : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString background READ background NOTIFY backgroundChanged FINAL)
    Q_PROPERTY(ImagesModel *images READ images CONSTANT)
    Q_PROPERTY(QString closeBtn READ closeBtn NOTIFY closeBtnChanged FINAL)
    Q_PROPERTY(QString closeBtnFocus READ closeBtnFocus NOTIFY closeBtnFocusChanged FINAL)

    explicit Settings(QObject *parent = nullptr);
    virtual ~Settings();

    QString background() const;

    ImagesModel *images();

    QString closeBtn() const;

    QString closeBtnFocus() const;

signals:
    void backgroundChanged();

    void closeBtnChanged();

    void closeBtnFocusChanged();

private:
    friend class SettingsPrivate;
    SettingsPrivate* impl;
    QSharedPointer<QTemporaryFile> m_background;
    QScopedPointer<ImagesModel> m_images;
    QSharedPointer<QTemporaryFile> m_closeBtn;
    QSharedPointer<QTemporaryFile> m_closeBtnFocus;
};

#endif // SETTINGS_H
