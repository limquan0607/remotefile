import QtQuick 2.15
import QtQuick.Controls 2.15
import content

ApplicationWindow {
    visible: true
    width: 400
    height: 600
    title: qsTr("Remote File System Manager")

    Column {

        anchors.topMargin: 10
        anchors.left: parent
        spacing: 10

        TextField {
            id: filePathField
            placeholderText: "Enter file path"
            width: parent.width * 0.8
        }

        TextField {
            id: newPathField
            placeholderText: "Enter new file path (for renaming)"
            width: parent.width * 0.8
        }

        Row {
            anchors.left: parent
            spacing: 5

            Button {
                text: "Create File"
                onClicked: {
                    fileClient.sendRequest("create", filePathField.text)
                }
            }

            Button {
                text: "Delete File"
                onClicked: {
                    fileClient.sendRequest("delete", filePathField.text)
                }
            }

            Button {
                text: "Rename File"
                onClicked: {
                    fileClient.sendRequest("rename", filePathField.text, newPathField.text)
                }
            }

        }

        Connections {
            target: fileClient
        }
    }
}
