import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Shapes 1.12
import Harmen 1.0

Item {
   id: root
   property int frameCounter: 0
   property int frameCounterAvg: 0
   property int counter: 0
   property int fps: 0
   property int fpsAvg: 0

   Rectangle {
      anchors.fill: row
      color: "green"
   }

   Row {
      id: row
      spacing: 4

      Item {
          width: 1
          height: 1
      }

      Rectangle {
         id: spinnerImage
         color: "white"
         anchors.verticalCenter: parent.verticalCenter
         width: 10
         height: 10
         NumberAnimation on rotation {
            from: 0
            to: 360
            duration: 800
            loops: Animation.Infinite
         }
         onRotationChanged: frameCounter++;
      }

      Text {
         id: text
         anchors.verticalCenter: parent.verticalCenter
         padding: 4
         color: "white"
         font.pixelSize: 10
         text: root.fps + " fps"
      }
   }

   Timer {
      interval: 2000
      repeat: true
      running: true
      onTriggered: {
         frameCounterAvg += frameCounter
         root.fps = frameCounter / 2
         counter++
         frameCounter = 0
      }
   }
}
