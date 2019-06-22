import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Shapes 1.12
import Harmen 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Text {
        id: theidx
        property int idx: 0
        text: "Point " + idx
    }

    SplineSeries {
        id: spline
        anchors.fill: parent
        color: Qt.rgba(1, 0, 0, .3)
        strokeWidth: 4
    }

    Shortcut {
        context: Qt.ApplicationShortcut
        sequence: "F"
        onActivated: theidx.idx = ++theidx.idx % spline.size
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: spline.setData(theidx.idx, Qt.point(mouse.x / width, mouse.y / height))
    }
}
