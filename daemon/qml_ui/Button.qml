/*
* Copyright (c) 2011 Nokia Corporation.
*/

import QtQuick 1.1

Item {
    id:button

    property int buttonId:0

    signal btnClicked(int buttonId)

    function sendBtnClicked()
    {
        if (buttonId!=0) {
            btnClicked(buttonId);
        }
    }

    Image {
        id: imageId_down
        opacity: 0
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        smooth:  true
        source:  {
            if (buttonId==1)
                "qrc:/images/close_on.png"
            else if (buttonId==2)
                "qrc:/images/btn_info_down.png"
            else if (buttonId==3)
                "qrc:/images/btn_select_down.png"
            else
                console.log("Error: no buttonId defined")
        }
    }

    Image {
        id: imageId_up
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        smooth:  true
        source:  {
            if (buttonId==1)
                "qrc:/images/close.png"
            else if (buttonId==2)
                "qrc:/images/btn_info.png"
            else if (buttonId==3)
                "qrc:/images/btn_select.png"
            else
                console.log("Error: no buttonId defined")
        }
    }

    SequentialAnimation {
        id:btnAnim
        PropertyAction { target: imageId_up; property: "opacity"; value: 0 }
        PropertyAction { target: imageId_down; property: "opacity"; value: 1 }
        PauseAnimation { duration: 500 }
        ScriptAction { script: sendBtnClicked(); }
        PropertyAction { target: imageId_down; property: "opacity"; value: 0 }
        PropertyAction { target: imageId_up; property: "opacity"; value: 1 }
    }

    MouseArea {
        anchors.fill: imageId_up
        onClicked: {
            btnAnim.restart()
        }
    }
}
