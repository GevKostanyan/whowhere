/*
* Copyright (c) 2011 Nokia Corporation.
*/

import QtQuick 1.1
import QtMobility.serviceframework 1.1
import QtMobility.systeminfo 1.2
import QtMultimediaKit 1.1
import CustomItems 1.0

import "JavaScript.js" as JavaScript

Rectangle {
    id: main
    width: 360; height: 640
    anchors.fill: parent
    color: "black"

    // WhoWhereDaemon service
    property variant daemonService: 0

    // ContactsView
    property variant contactsView: 0

    // InfoView
    property variant infoView: 0

    // Show/hide ContactsView
    function showContacts(enable)
    {
        if (enable) {
            // Create ContactsView dynamically
            if (!contactsView) {
                var component = Qt.createComponent("qrc:/ContactsView.qml");
                if (component.status == Component.Ready) {
                    contactsView = component.createObject(main);
                }
            }
            water.pauseWater(true);
            contactsView.show();
        } else {
            // Hide ContactsView
            if (contactsView) {
                water.pauseWater(false);
                contactsView.opacity = 0;
            }
        }
    }

    // Show/hide InfoView
    function showInfo(enable)
    {
        if (enable) {
            water.setWaterUpdateSpeed(200);
            // Create InfoView dynamically
            if (!infoView) {
                var component = Qt.createComponent("qrc:/InfoView.qml");
                if (component.status == Component.Ready) {
                    infoView = component.createObject(main);
                }
            }
            infoView.opacity = 1;
        } else {
            // Hide ContactsView
            if (infoView) {
                infoView.opacity = 0;
                water.setWaterUpdateSpeed(80);
            }
        }
    }



    // Application start sound
    Audio {
        id: playMusic
        source: appFolder + "13564__acclivity__GullsByTheSea.wav"
        volume: 0.4
    }

    // QtMobility Service FW
    // Connect WhoWhereDaemon Service
    Service {
        id: daemonServiceId
        interfaceName: "com.nokia.qt.examples.qwhowheredaemon"

        Component.onCompleted: {
            main.daemonService = daemonServiceId.serviceObject;
            console.log(daemonServiceId.serviceName);
            if (daemonServiceId.valid) {
                console.log("Service valid");
                // Read values form Daemot to UI
                if(JavaScript.isDaemonEnabled()) {
                    onOffSwitch.state = "on";
                }
                if (JavaScript.isGpsRunningEnabled()) {
                    console.log("gps running");
                    gpsSwitch.state = "on";
                }
                knobDial.value = JavaScript.getGpsAccuracy();
            } else {
                console.log("Service NOT valid!");
            }
        }
    }

    // Listening device profile (silent or not)
    DeviceInfo {
        id: deviceInfo
    }

    Component.onCompleted: {
        audioTimer.restart();
    }

    Timer {
        id: audioTimer
        interval: 3000
        repeat: false
        onTriggered: {
            splashScreen.opacity = 0;
            water.opacity = 1;
            if (deviceInfo.currentProfile != 1){
                playMusic.play();
            }
        }
    }

    // Water background
    WaterItem {
        id:water
        anchors.fill: parent
        opacity: 0;
    }

    // Application background paper
    Image {
        id: background
        anchors.fill: parent
        anchors.centerIn: parent
        anchors.topMargin: 40
        anchors.bottomMargin: 40
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        fillMode: Image.PreserveAspectFit
        source: "qrc:/images/back_portrait.png"
        smooth: true
    }

    // Gps icon
    GpsIcon {
        width:  50
        height:  50
        x: 5
        y: 5
    }

    // Exit
    Button {
        id: exit
        buttonId: 1
        width:  50
        height:  50
        x: parent.width - width - 5
        y: 5
        onBtnClicked: {
            Qt.quit();
        }
    }


    // Main content
    Item {
        id: mainContent
        anchors.fill: background
        smooth:  true

        property int rowHeight: background.height * 0.105
        property int colWidth: background.width * 0.28

        // 1. row ---------------------------------------------------------
        Image {
            id: logo
            anchors.top: mainContent.top
            anchors.left: mainContent.left
            anchors.leftMargin: 10
            anchors.topMargin: background.height * 0.05
            width: mainContent.rowHeight * 2.2
            height: mainContent.rowHeight * 2.2
            fillMode: Image.PreserveAspectFit
            source: "qrc:/images/logo.png"
            smooth: true
        }
        Button {
            id: info
            buttonId: 2
            anchors.top: mainContent.top
            anchors.right: mainContent.right
            anchors.rightMargin: 40
            anchors.topMargin: background.height * 0.1
            width: mainContent.colWidth
            height: mainContent.rowHeight
            onBtnClicked: {
                main.showInfo(true);
            }
        }

        // 2. row --------------------------------------------------------
        Text {
            anchors.top: logo.bottom
            anchors.right: logo.right
            anchors.topMargin: 15
            color:"black"
            font.pointSize: 6
            text:"Server activated"
            smooth: true
        }
        Switch {
            id: onOffSwitch
            anchors.top: logo.bottom
            anchors.left: info.left
            anchors.topMargin: 5
            width: mainContent.colWidth
            height: mainContent.rowHeight
            onSwitched: {
                JavaScript.enableDaemon(on);
            }
        }

        // 3. row --------------------------------------------------------
        Text {
            anchors.top: onOffSwitch.bottom
            anchors.right: logo.right
            anchors.topMargin: 15
            color:"black"
            font.pointSize: 6
            text:"GPS always on"
            smooth: true
        }
        Switch {
            id: gpsSwitch
            anchors.top: onOffSwitch.bottom
            anchors.left: info.left
            anchors.topMargin: 5
            width: mainContent.colWidth
            height: mainContent.rowHeight
        }
        Connections {
            target:gpsSwitch
            onSwitched: {
                JavaScript.keepGpsRunning(on);
            }
        }

        // 4. row --------------------------------------------------------
        Text {
            anchors.top: gpsSwitch.bottom
            anchors.right: logo.right
            anchors.topMargin: 20
            color:"black"
            font.pointSize: 6
            text:"GPS accuracy"
            smooth: true
        }

        KnobDial {
            id: knobDial
            anchors.top: gpsSwitch.bottom
            anchors.left: info.left
            anchors.topMargin: 10
            width: mainContent.rowHeight
            height: mainContent.rowHeight
        }

        // 5. row --------------------------------------------------------
        Text {
            id: sendTxt
            anchors.top: knobDial.bottom
            anchors.right: logo.right
            anchors.topMargin: 20
            color:"black"
            font.pointSize: 6
            text:"Send location to..."
            smooth: true
        }

        Button {
            id: contactBtn
            buttonId: 3
            anchors.top: knobDial.bottom
            anchors.left: info.left
            anchors.topMargin: 10
            width: mainContent.colWidth
            height: mainContent.rowHeight
            onBtnClicked: {
                showContacts(true);
            }
        }

        // WhoWhereDaemon log screen
        DaemonLog {
            id: logScreen
            anchors.top: contactBtn.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            width: background.width - 70
            height: mainContent.rowHeight * 1.4
        }
    }

    MouseArea {
        id: mainMouseArea
        anchors.fill: parent
        onPressed: {
            // Note: need to handle onPressed() for
            // igonore MouseArea
            showContacts(false);
            showInfo(false);
            mouse.accepted = false;
        }
    }

    // Spalsh screen
    Rectangle {
        id:splashScreen
        anchors.fill: parent
        color: "black"
        Image {
            anchors.fill: parent
            source: "qrc:/images/back_portrait.png"
            smooth: true
        }
        Image {
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            source: "qrc:/images/logo.png"
            smooth: true
        }
    }


}
