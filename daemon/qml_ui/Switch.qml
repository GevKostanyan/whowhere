/*
* Copyright (c) 2011 Nokia Corporation.
*/

import QtQuick 1.1

Item {
    id: toggleswitch

    property bool on: false
    property int farX

    signal switched(bool on);

    function toggle() {
        if (toggleswitch.state == "on")
            toggleswitch.state = "off";
        else 
            toggleswitch.state = "on";

        switched(toggleswitch.on);
    }

    function releaseSwitch() {
        if (knob.x == 1) {
            if (toggleswitch.state == "off") return;
        }
        if (knob.x == toggleswitch.farX) {
            if (toggleswitch.state == "on") return;
        }
        toggle();
    }

    Image {
        id: background
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        smooth: true
        source: "qrc:/images/btn_back.png"
        MouseArea {
            anchors.fill: parent;
            onClicked: {
                toggle();
            }
        }
    }

    Component.onCompleted: {
        farX = background.width - knob.width
    }

    Text {
        anchors.left: background.left
        anchors.leftMargin: 10
        anchors.verticalCenter: background.verticalCenter
        color:"white"
        font.pixelSize: background.height * 0.4
        text:"ON"
        smooth: true
    }

    Text {
        anchors.right: background.right
        anchors.rightMargin: 10
        anchors.verticalCenter: background.verticalCenter
        color:"white"
        font.pixelSize: background.height * 0.4
        text:"OFF"
        smooth: true
    }

    Image {
        id: knob
        height: background.height
        width: background.width * 0.5
        x: 1;
        fillMode: Image.PreserveAspectFit
        smooth: true
        source: "qrc:/images/back_on-off_active.png"

        MouseArea {
            anchors.fill: parent
            drag.target: knob; drag.axis: Drag.XAxis; drag.minimumX: 1; drag.maximumX: toggleswitch.farX
            onClicked: {
                toggle();
            }
            onReleased: {
                releaseSwitch();
            }
        }
    }

    states: [
        State {
            name: "on"
            PropertyChanges { target: knob; x: toggleswitch.farX }
            PropertyChanges { target: toggleswitch; on: true }
        },
        State {
            name: "off"
            PropertyChanges { target: knob; x: 1 }
            PropertyChanges { target: toggleswitch; on: false }
        }
    ]

    transitions: Transition {
        NumberAnimation { properties: "x"; easing.type: Easing.InOutQuad; duration: 200 }
    }
}
