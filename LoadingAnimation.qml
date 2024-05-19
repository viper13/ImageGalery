import QtQuick

Rectangle {
    id: loadingOverlay
    visible: true
    color: "#00000080"
    anchors.fill: parent

    Rectangle {
        id: circle1
        width: 20
        height: 20
        radius: 10
        color: "#3498db"
        y: parent.height / 2
        x: parent.width / 2
        NumberAnimation on x {
            from: parent.width / 2
            to: parent.width
            duration: 1000
            loops: Animation.Infinite
            easing.type: Easing.InExpo
        }
    }
    Rectangle {
        id: circle2
        width: 20
        height: 20
        radius: 10
        color: "#3498db"
        y: parent.height / 2
        x: parent.width / 2
        NumberAnimation on x {
            from: parent.width / 2
            to: 0
            duration: 1000
            loops: Animation.Infinite
            easing.type: Easing.InExpo
        }
    }
}
