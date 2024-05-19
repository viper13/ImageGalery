import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Application.Settings 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 1920
    height: 1080
    title: "Image Gallery"
    property real heightScale: height / 1080
    property real widthScale: width / 1920

    property Settings settings: Settings
    Image {
        id: background
        anchors.fill: parent
        source: settings.background === "" ? "" : "file:///" + settings.background

        Column {
            id: content
            spacing: 0
            anchors.fill: parent
            Item {
                id: spacerTop
                width: content.width
                height: 168 * heightScale
                Image {
                    id: closeBtn
                    anchors.right: spacerTop.right
                    readonly property string normalSrc: settings.closeBtn === "" ? "" : "file:///" + settings.closeBtn
                    readonly property string focusedSrc: settings.closeBtnFocus === "" ? "" : "file:///" + settings.closeBtnFocus
                    source: hover.hovered ? focusedSrc : normalSrc
                    HoverHandler {
                        id: hover
                        cursorShape: Qt.PointingHandCursor
                    }
                    TapHandler {
                        onTapped: Qt.callLater(Qt.quit)
                    }

                }
            }
            Image {
                id: selected
                fillMode: Image.Stretch
                anchors.horizontalCenter: parent.horizontalCenter
                width: 1493 * widthScale
                height: 507 * heightScale
                source: settings.images.currentImage === "" ? "" : "file:///" + settings.images.currentImage
            }
            Item {
                id: spacerMid
                height: 43 * heightScale
                width: content.width
            }
            Row {
                height: 108 * heightScale
                width: content.width
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 0
                Item {
                    id: leftSpace
                    height: 108 * heightScale
                    width: 400 * widthScale
                }
                Item {
                    id: leftBtn
                    height: 108 * heightScale
                    width: 54 * widthScale
                    HoverHandler {
                        cursorShape: Qt.PointingHandCursor
                    }
                    TapHandler {
                        onTapped: settings.images.decrease()
                    }
                }

                CoruselList {
                    id: corusel
                    implicitHeight: 108 * heightScale
                    implicitWidth: 1000 * widthScale
                    model: settings.images.loading ? [] : settings.images
                    currentIndex: settings.images.current
                    clip: true
                    widthScale: window.widthScale
                    heightScale: window.heightScale
                }
                Item {
                    id: rightBtn
                    height: 108 * heightScale
                    width: 54 * widthScale
                    HoverHandler {
                        cursorShape: Qt.PointingHandCursor
                    }
                    TapHandler {
                        onTapped: settings.images.increase()
                    }
                }
            }
        }
        focus: true
        Keys.enabled: true
        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Left) {
                settings.images.decrease()
            } else if (event.key === Qt.Key_Right) {
                settings.images.increase()
            }
        }
    }
}
