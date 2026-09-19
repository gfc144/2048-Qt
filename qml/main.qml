import QtQuick
import QtQuick.Controls
import "2048.js" as MyScript

Rectangle {
    id: mainWindow
    width: 550
    height: 740
    
    property string currentLabel: MyScript.label
    function startNewGame() { MyScript.startupFunction(); }
    function setLabel(name) {
        if (MyScript.label !== name) {
            MyScript.label = name;
            MyScript.startupFunction();
        }
    }

    Connections {
        target: dialogManager
        function onRequestNewGame() { MyScript.startupFunction(); }
        function onContinuePlaying() { MyScript.checkTargetFlag = false; }
        function onQuitApp() { MyScript.cleanUpAndQuit(); }
    }
    Item {
        id: helper
        focus: false
        property var myColors: {"bglight": "#FAF8EF",
                                "bggray": Qt.rgba(238/255, 228/255, 218/255, 0.35),
                                "bgdark": "#BBADA0",
                                "fglight": "#EEE4DA",
                                "fgdark": "#776E62",
                                "bgbutton": "#8F7A66", // Background color for the "New Game" button
                                "fgbutton": "#F9F6F2" // Foreground color for the "New Game" button
        }
    }
    color: helper.myColors.bglight

    Item {
        width: 500
        height: 670
        anchors.centerIn: parent

        focus: true
        Keys.onPressed: MyScript.moveKey(event)

        MouseArea {
            anchors.fill: parent
            onClicked: parent.forceActiveFocus()
        }

        FontLoader { id: localFont; source: "qrc:///res/fonts/DroidSansFallback.ttf" }

        Text {
            id: gameName
            font.family: localFont.name
            font.pixelSize: 55
            font.bold: true
            text: "2048"
            color: helper.myColors.fgdark
        }

        Row {
            anchors.right: parent.right
            spacing: 5
            Repeater {
                id: scoreBoard
                model: 2
                Rectangle {
                    width: (index == 0) ? 95 : 125
                    height: 55
                    radius: 3
                    color: helper.myColors.bgdark
                    property string scoreText: (index === 0) ? MyScript.score.toString() : MyScript.bestScore.toString()
                    Text {
                        text: (index == 0) ? qsTr("SCORE") : qsTr("BEST")
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: 6
                        font.family: localFont.name
                        font.pixelSize: 13
                        color: helper.myColors.fglight
                    }
                    Text {
                        text: scoreText
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: 22
                        font.family: localFont.name
                        font.pixelSize: 25
                        font.bold: true
                        color: "white"
                    }
                }
            }

            Text {
                id: addScoreText
                font.family: localFont.name
                font.pixelSize: 25
                font.bold: true
                color: Qt.rgba(119/255, 110/255, 101/255, 0.9);
//                parent: scoreBoard.itemAt(0)
                anchors.horizontalCenter: parent.horizontalCenter

                property bool runAddScore: false
                property real yfrom: 0
                property real yto: -(parent.y + parent.height)
                property int addScoreAnimTime: 600

                ParallelAnimation {
                    id: addScoreAnim
                    running: false

                    NumberAnimation {
                        target: addScoreText
                        property: "y"
                        from: addScoreText.yfrom
                        to: addScoreText.yto
                        duration: addScoreText.addScoreAnimTime

                    }
                    NumberAnimation {
                        target: addScoreText
                        property: "opacity"
                        from: 1
                        to: 0
                        duration: addScoreText.addScoreAnimTime
                    }
                }
            }
        }

        Text {
            id: banner
            y: 90
            height: 40
            text: qsTr("Join the numbers and get to the <b>2048 tile</b>!")
            color: helper.myColors.fgdark
            font.family: localFont.name
            font.pixelSize: 16
            verticalAlignment: Text.AlignVCenter
        }

        Button {
            width: 129
            height: 40
            y: 90
            anchors.right: parent.right

            background: Rectangle {
                color: helper.myColors.bgbutton
                radius: 3
            }
            contentItem: Text {
                text: qsTr("New Game")
                color: helper.myColors.fgbutton
                font.family: localFont.name
                font.pixelSize: 18
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: MyScript.startupFunction()
        }

        Rectangle {
            y: 170
            width: 500
            height: 500
            color: helper.myColors.bgdark
            radius: 6

            Grid {
                id: tileGrid
                x: 15;
                y: 15;
                rows: 4; columns: 4; spacing: 15

                Repeater {
                    id: cells
                    model: 16
                    Rectangle {
                        width: 425/4; height: 425/4
                        radius: 3
                        color: helper.myColors.bggray
                    }
                }
            }
        }

    }

    Component.onCompleted: MyScript.startupFunction();
}
