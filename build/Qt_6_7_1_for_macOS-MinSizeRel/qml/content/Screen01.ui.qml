import QtQuick 2.15
import QtQuick.Controls 2.15


Rectangle {
    visible: true
    width: 400
    height: 600

    Column {
        anchors.centerIn: parent
        spacing: 5

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


        Button {
            text: "Create File"

        }

        Button {
            text: "Delete File"
        }

        Button {
            text: "Rename File"
        }

        Connections {
            target: fileClient
        }
    }
}
