/*
* Copyright (c) 2011 Nokia Corporation.
*/

import QtQuick 1.1

import "JavaScript.js" as JavaScript

Image {
    id: knob
    property real minimumvalue: 5
    property real maximumvalue: 100
    property real minimumrotation: 35
    property real maximumrotation: 290
    property real value: 10
    width: 300; height: 300
    source: "images/btn_accuracy_back.png"
    smooth: true
    Image {
        id: knobImage
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        source: "images/btn_accuracy_shadow.png"
        smooth: true
    }
    Image {
        id: knobHat
        width: parent.width - 10
        height: parent.height - 10
        x: 5; y:5
        source: "images/btn_accuracy.png"
        rotation: knob.minimumrotation + knob.value / knob.maximumvalue *
                  knob.maximumrotation
        smooth: true
    }
    Text {
        anchors.left: parent.right
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 6
        text: knob.value + "m"
    }
    MouseArea {
        property int previousY: 0
        anchors.fill: knob
        onPressed: previousY = mouse.y
        onPositionChanged: {
            var delta = (previousY - mouse.y) * 0.20
            if (knob.value + delta > knob.maximumvalue) {
                knob.value = knob.maximumvalue
            }
            else if (knob.value + delta < knob.minimumvalue) {
                knob.value = knob.minimumvalue
            }
            else {
                knob.value += delta
                previousY = mouse.y
            }
            var temp = knob.value;
            knob.value = temp.toFixed(0);

            JavaScript.setGpsAccuracy(knob.value);
        }
    }
}
