import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Shapes 1.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Harmen 1.0

Window {
   visible: true
   width: 640
   height: 480
   title: qsTr("Hello World")
   flags: Qt.Window | Qt.WindowFullscreenButtonHint
   color: "#aaa"

   ColumnLayout {
      anchors.fill: parent
      Rectangle {
         color: "#fff"
         Layout.fillHeight: true
         Layout.fillWidth: true

         Axis {
            id: xaxis
            from: 0.0
            to: 100.0
         }

         Axis {
            id: yaxis
            from: 0
            to: 10

            Behavior on from { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad}}
            Behavior on to { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad}}
         }

         AreaSplineSeries {
            id: areaExample
            axisX: xaxis
            axisY: yaxis
            anchors.fill: parent
            strokeWidth: 10
            drawKnots: true
            knotSize: 5
            color: Qt.rgba(.9, .6, .3, 1)
            Component.onCompleted: GraphData.drawAreaSpline(areaExample)
         }

         SplineSeries {
            id: median
            axisX: xaxis
            axisY: yaxis
            anchors.fill: parent
            strokeWidth: 2
            color: Qt.rgba(0.5, 0.3, 1.0, 0.9);

            MouseArea {
               anchors.fill: parent
               hoverEnabled: true
               onPositionChanged: {
                  var xcoord = mouse.x / width * (xaxis.to - xaxis.from) + xaxis.from;
                  var ycoord = (1 - mouse.y / height) * (yaxis.to - yaxis.from) + yaxis.from;
                  FancyProvider.insert(xcoord, ycoord)
                  FancyProvider.drawMedianTo(median)
                  FancyProvider.drawBoundaries(areaExample)
               }
            }
         }

         Column {
            spacing: 16
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 16

            Button {
               text: "+"
               width: 46
               height: 46
               opacity: down ? 0.8 : 1
               onClicked: {yaxis.from -= 1; yaxis.to += 1}
            }

            Button {
               text: "-"
               width: 46
               height: 46
               opacity: down ? 0.8 : 1
               onClicked: {yaxis.from += 1; yaxis.to -= 1}
            }
         }
      }

      // Sine wave
      Rectangle {
         color: "#fff"
         Layout.fillHeight: true
         Layout.fillWidth: true

         Axis {
            id: xaxis2
            from: 0
            to: 2 * Math.PI
         }

         Axis {
            id: yaxis2
            from: -1.5
            to: 1.5
         }

         SplineSeries {
            id: sine1
            axisX: xaxis2
            axisY: yaxis2
            anchors.fill: parent
            strokeWidth: 1
            drawKnots: true
            knotSize: 10
            color: "orange"
         }

         SplineSeries {
            id: sine2
            axisX: xaxis2
            axisY: yaxis2
            anchors.fill: parent
            strokeWidth: 1
            drawKnots: true
            knotSize: 10
            color: "blue"
         }

         SplineSeries {
            id: sine3
            axisX: xaxis2
            axisY: yaxis2
            anchors.fill: parent
            strokeWidth: 3
            drawKnots: true
            knotSize: 10
            color: "pink"
         }

         VerticalLine {
            id: line
            anchors.fill: parent
            axisX: xaxis2
            valueX: 2.0
            strokeWidth: 2
            color: "black"
         }

         Point {
            id: point
            anchors.fill: parent
            axisX: xaxis2
            axisY: yaxis2
            valueX: 2.0
            valueY: 1.0
         }

         MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onPositionChanged: {
               var xcoord = mouse.x / width * (xaxis2.to - xaxis2.from) + xaxis2.from;
               var ycoord = (1 - mouse.y / height) * (yaxis2.to - yaxis2.from) + yaxis2.from;
               point.valueX = xcoord
               point.valueY = ycoord
               line.valueX = xcoord
            }
         }

         Component.onCompleted: GraphData.drawSines(sine1, sine2, sine3)

         Connections {
            target: GraphData
            onSineChanged: GraphData.drawSines(sine1, sine2, sine3)
         }
      }
   }

   Fps {
      id: root
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.margins: 12
      opacity: 0.8
   }
}
