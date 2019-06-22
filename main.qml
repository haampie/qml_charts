import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Shapes 1.12
import QtQuick.Layouts 1.12
import Harmen 1.0

Window {
   visible: true
   width: 640
   height: 480
   title: qsTr("Hello World")

   Text {
      id: theidx
      text: "Point " + data.idx + (data.lower ? " lower" : "upper")
      anchors.horizontalCenter: parent.horizontalCenter
   }

   ColumnLayout {
      anchors.fill: parent
      AreaSplineSeries {
         id: areaspline
         Layout.fillHeight: true
         Layout.fillWidth: true
         strokeWidth: 10
         drawKnots: true
         knotSize: 5
         color: Qt.rgba(.9,.9,.9,1)

         Shortcut {
            context: Qt.ApplicationShortcut
            sequence: "D"
            onActivated: data.idx = Math.min(areaspline.size - 1, data.idx + 1)
         }

         Shortcut {
            context: Qt.ApplicationShortcut
            sequence: "A"
            onActivated: data.idx = Math.max(0, data.idx - 1)
         }

         Shortcut {
            context: Qt.ApplicationShortcut
            sequence: "W"
            onActivated: data.lower = true
         }

         Shortcut {
            context: Qt.ApplicationShortcut
            sequence: "S"
            onActivated: data.lower = false
         }

         MouseArea {
            id: data
            property int idx: 0
            property bool lower: true
            anchors.fill: parent
            hoverEnabled: true
            onPositionChanged: {
               if (lower)
                  areaspline.setDataLower(idx, Qt.point(mouse.x / width, mouse.y / height))
               else
                  areaspline.setDataUpper(idx, Qt.point(mouse.x / width, mouse.y / height))
            }
         }
      }

      SplineSeries {
         id: spline
         Layout.fillHeight: true
         Layout.fillWidth: true
         strokeWidth: 2
         drawKnots: true
         knotSize: 3
         color: "orange"
      }
   }

   Fps {
      id: root
   }
}
