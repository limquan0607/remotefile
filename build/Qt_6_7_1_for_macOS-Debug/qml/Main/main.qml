import QtQuick 2.15
import QtQuick.Controls 2.15
import content

ApplicationWindow {
    visible: true
    width: 400
    height: 600
    title: qsTr("Remote File System Manager")

    Column {
        anchors.centerIn: parent
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

        TextArea {
            id: contentArea
            placeholderText: "Enter file content"
            width: parent.width * 0.8
            height: parent.height * 0.3
        }

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

        Button {
            text: "Read File"
            onClicked: {
                fileClient.sendRequest("read", filePathField.text)
            }
        }

        Button {
            text: "Write File"
            onClicked: {
                fileClient.sendRequest("write", filePathField.text, "", contentArea.text)
            }
        }

        TextArea {
            id: responseArea
            placeholderText: "Response"
            width: parent.width * 0.8
            height: parent.height * 0.3
            readOnly: true
        }

        Connections {
            target: FileClient
            onResponseReceived: {
                responseArea.text = response
            }
        }
    }
}
