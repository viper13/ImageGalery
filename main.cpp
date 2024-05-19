/*
 * Author: Vasyl Hryhorzhevskyi
 * Email: viper.kspu@gmail.com
 * Date: 19.05.2024
 *
 * Description:
 * This is Image galery app.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "imagesmodel.h"
#include "settings.h"

void registerSettingsSingleton();

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    registerSettingsSingleton();

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/ImageGalery/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

static QObject *settingsSingletonProvider(QQmlEngine *, QJSEngine *) {
    return new Settings();
}

void registerSettingsSingleton() {
    qmlRegisterUncreatableType<ImagesModel>("Application.Settings", 1, 0, "ImageModel", "it can be used via settings object");
    qmlRegisterSingletonType<Settings>("Application.Settings", 1, 0, "Settings", settingsSingletonProvider);
}
