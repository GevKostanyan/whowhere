/*
* Copyright (c) 2011 Nokia Corporation.
*/

import QtQuick 1.1
import QtMobility.contacts 1.1
import "JavaScript.js" as JavaScript

Rectangle {
    id: container
    anchors.fill: parent
    opacity: 0

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#a5cbe0" }
        GradientStop { position: 1.0; color: "#388cc8" }
    }

    property int rowHeight: 100

    function show() {
        container.opacity = 0.9
    }

    Behavior on opacity {
        NumberAnimation { duration: 300 }
    }

    ContactModel {
        id: contactModel
        sortOrders: [
            SortOrder {
                detail:ContactDetail.Name
                field:Name.DisplayLabel
                direction:Qt.AscendingOrder
            }
        ]
    }

    Component.onCompleted: {
    }

    // Contact row for PathView
    Component {
        id: pathContactDelegate

        Image {
            scale: PathView.elementScale
            opacity: PathView.elementOpacity
            z: PathView.elementZ
            source: "qrc:/images/back_contactlist.png"
            width: PathView.width
            height: container.rowHeight
            clip: true
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
                color:"black";
                text: model.contact.name.firstName + " " +  model.contact.name.lastName
                font.pixelSize:30
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    main.showContacts(false);
                    var nro = model.contact.phoneNumber.number;
                    if (nro.length > 0) {
                        JavaScript.sendLocationTo(model.contact.phoneNumber.number);
                    }
                }
            }
        }
    }

    // Contact row for ListView
    Component {
        id: listContactDelegate

        Image {
            source: "qrc:/images/back_contactlist.png"
            width: ListView.width
            height: container.rowHeight
            clip: true
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
                color:"black";
                text: model.contact.name.firstName + " " +  model.contact.name.lastName
                font.pixelSize:30
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    main.showContacts(false);
                    JavaScript.sendLocationTo(model.contact.phoneNumber.number);
                }
            }
        }
    }

    // No contact message
    Image {
        id: searchingMsg
        opacity: contactModel.contacts.length==0 ? 1 : 0
        width: parent.width * 0.8
        height: container.rowHeight
        anchors.centerIn: parent
        source: "qrc:/images/back_contactlist.png"
        smooth: true
        Text {
            anchors.centerIn: parent
            text: "No contacts found"
            smooth: true
            font.pointSize: 8
        }
    }

    MouseArea {
        id: mainMouseArea
        anchors.fill: parent
        onClicked: {
            main.showContacts(false);
        }
    }

    // ListView show one contact and PathView shows if there is more that one
    ListView {
        id: lview
        opacity: contactModel.contacts.length < 2 ? 1 : 0
        width: parent.width * 0.8
        height: container.rowHeight
        anchors.centerIn: parent
        model: contactModel
        delegate: listContactDelegate
    }

    // Used when more than one contact exists
    PathView {
        id: pview
        opacity: contactModel.contacts.length > 1 ? 1 : 0
        width: parent.width * 0.8
        height:  parent.height * 0.6
        anchors.centerIn: parent
        clip: true
        pathItemCount: 4
        model: contactModel
        delegate: pathContactDelegate

        property int xCap: width * 0.5
        property int yCap: container.rowHeight * 0.5

        path:
            Path {

            // Top
            startX: pview.xCap; startY: pview.yCap
            PathAttribute { name: "elementScale"; value: 0.85}
            PathAttribute { name: "elementZ"; value: 10 }
            PathAttribute { name: "elementOpacity"; value: 0 }

            // Mid
            PathQuad { x: pview.xCap; y: pview.height * 0.5;
                controlX: pview.xCap; controlY: pview.yCap }
            PathAttribute { name: "elementScale"; value: 1 }
            PathAttribute { name: "elementZ"; value: 100 }
            PathAttribute { name: "elementOpacity"; value: 1 }

            // Low
            PathQuad { x: pview.xCap; y: pview.height - pview.yCap;
                controlX: pview.xCap; controlY: pview.height - pview.yCap}
            PathAttribute { name: "elementScale"; value: 0.85 }
            PathAttribute { name: "elementZ"; value: 1 }
            PathAttribute { name: "elementOpacity"; value: 0 }

        }
    }


}
