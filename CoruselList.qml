import QtQuick

ListView {
    id: listView
    orientation: ListView.Horizontal
    snapMode: ListView.SnapToItem
    currentIndex: model.current
    spacing: 5 * widthScale
    property real heightScale: 1
    property real widthScale: 1

    delegate: Image {
            property bool isCurrent: ListView.isCurrentItem
            width: 228 * widthScale
            height: 108 * heightScale
            property int index: index
            source: "file:///" + (isCurrent ? model.imageSrcFocus : model.imageSrc)
            fillMode: Image.Stretch
        }
}
