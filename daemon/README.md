WhoWhere Daemon Example
=======================

This Qt C++ example application demonstrates how to use the QtMobility Service 
Framework.

There are two parts in this example:
 * The Daemon Service executable binary
 * The Client QML UI example application that communicates with the executable 
   binary Daemon.exe via the Service Framework

A separate Qt example application called WhoWhere shows your position on a 
map. You can send your location or ask a friend (from your contacts list) for 
his or her current location using short message service (SMS) messages.
 
The Daemon Service offers a QtMobility Service Framework interface to the Client.
The Daemon finds your current GPS location if it is requested by the QML WhoWhere
Daemon UI example or the Qt WhoWhere example. The Daemon responds to the example 
application's location requests automatically without user interaction. You can 
request your friend's current GPS location via SMS using Qt WhoWhere, and if 
the friend in question has this Daemon running on their device, it responds automatically.

The Client UI application tries to find the service offered by the Daemon, 
com.nokia.qt.examples.qwhowheredaemon.WhoWhereDaemonService, and connects to 
it.  

The example has been tested to work with Qt 4.7.4. and QtMobility 1.2.1 on 
Symbian Anna.


PREREQUISITES
-------------------------------------------------------------------------------

- Qt basics.


IMPORTANT FILES/CLASSES
-------------------------------------------------------------------------------

Daemon:
- main.cpp:  Registers the com.nokia.qt.examples.qwhowheredaemon service
- Daemon.h:  Daemon that listens for and sends SMS messages and requests GPS 
             location if asked
- Message.h: SMS message handling

QML Client:
- watergen.h:       Generates water
- wateritem.h:      Custom QML item that shows a moving ocean
- main.qml:         Main QML, connects to Daemon 
                    com.nokia.qt.examples.qwhowheredaemon.WhoWhereDaemonService 
                    service 
- ContactsView.qml: Shows contacts from device memory
- DaemonLog.qml:    Reads and shows log messages from the Daemon Server

Important QtMobility classes: QRemoteServiceRegister, QServiceManager, 
QGeoPositionInfoSource, QGeoPositionInfoSource.


REQUIRED CAPABILITIES
-------------------------------------------------------------------------------

The Client UI application can be self-signed.

The Daemon service executable binary must be Developer Signed at: 
https://www.symbiansigned.com/app/page/public/openSignedOnline.do


KNOWN ISSUES
-------------------------------------------------------------------------------

None.


BUILD & INSTALLATION INSTRUCTIONS
-------------------------------------------------------------------------------

Preparations
~~~~~~~~~~~~

Check that you have the latest Qt/Qt SDK installed in the development 
environment and on the device.


Build & installation instructions using Qt SDK
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Open the Qt PRO file
   File > Open File or Project, select pro-file.

2. Select target(s), for example 'Qt for Symbian^3 (Qt SDK)', and press the 
   Finish button.

3. Press the Run button to build the project and create the Symbian install 
   package. The application is installed on the device. Remember that Daemon 
   must be Developer Signed.


COMPATIBILITY
-------------------------------------------------------------------------------

- Qt 4.7.4
- QtMobility 1.2.1

Tested on: 
- Nokia N8-00
- Nokia E7-00

Developed with:
- Qt SDK 1.1.4


CHANGE HISTORY
-------------------------------------------------------------------------------

1.1 Daemon start on device startup removed
1.0 First version


RELATED DOCUMENTATION
-------------------------------------------------------------------------------

Qt WhoWhere example
https://projects.forum.nokia.com/whowhere

New Qt APIs - Qt Mobility
http://qt.nokia.com/products/qt-addons/mobility

Qt for Symbian
http://qt.nokia.com/downloads