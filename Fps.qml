import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Shapes 1.12
import Harmen 1.0

Rectangle {
   id: root
   property int frameCounter: 0
   property int frameCounterAvg: 0
   property int counter: 0
   property int fps: 0
   property int fpsAvg: 0

   color: "black"

   Rectangle {
      anchors.fill: row
      color: "green"
   }

   Row {
      id: row
      Rectangle {
           id: spinnerImage
           color: "black"
           anchors.verticalCenter: parent.verticalCenter
           width: 12
           height: 12
           NumberAnimation on rotation {
               from:0
               to: 360
               duration: 800
               loops: Animation.Infinite
           }
           onRotationChanged: frameCounter++;
       }

      Text {
         id: text
         anchors.verticalCenter: parent.verticalCenter
         padding: 12
         color: "#c0c0c0"
         font.pixelSize: 18
         text: "Ø " + root.fpsAvg + " | " + root.fps + " fps"
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
         if (counter >= 3) {
            root.fpsAvg = frameCounterAvg / (2 * counter)
            frameCounterAvg = 0
            counter = 0
         }
      }
   }
}
