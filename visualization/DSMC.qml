import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import DSMC 1.0

Item {
    id: dsmcRoot

    property real aspectRatio: width/height
    property bool applicationActive: {
        if(Qt.platform.os === "android" || Qt.platform.os === "ios") {
            if(Qt.application.state === Qt.ApplicationActive) {
                return true
            } else {
                return false
            }
        } else {
            return true
        }
    }

    width: 1920
    height: 1080

    focus: true

    Component.onCompleted: {
        console.log("DSMC started.")
        console.log("Platform: " + Qt.platform.os)
    }

    DSMC {
        id: dsmc
        anchors.fill: parent

        running: true

        Timer {
            id: timer
            property real lastTime: Date.now()
            property real lastSampleTime: Date.now()
            running: dsmc.running && dsmcRoot.applicationActive
            repeat: true
            interval: 1
            onTriggered: {
                if(!dsmc.previousStepCompleted) {
                    return
                }

                var currentTime = Date.now()
                var dt = currentTime - lastTime
                dt /= 1000
                dsmc.step(dt)

                var sampleTimeDifference = (currentTime - lastSampleTime)/1000
                lastTime = currentTime
            }
        }
    }

    Keys.onPressed: {

    }
}
