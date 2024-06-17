import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import content
import com.example.fileclient 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("Remote File System Manager")

    property string selectedFile: ""
    property string errorMsg: ""
    property var selectedItem: null
    property bool loggedIn: false


    // Login Screen
    Rectangle {
        id: loginScreen
        width: parent.width
        height: parent.height
        // color: "lightgray"
        anchors.centerIn: parent
        visible: !loggedIn

        signal loginSuccessful

        Column {
            anchors.centerIn: parent
            spacing: 10

            TextField {
                id: usernameField
                placeholderText: "Username"
            }

            TextField {
                id: passwordField
                placeholderText: "Password"
                echoMode: TextInput.Password
            }

            Button {
                text: "Login"
                onClicked: {
                    if (usernameField.text === "user" && passwordField.text === "password") {
                        loggedIn = true
                        loginScreen.loginSuccessful()
                    } else {
                        errorMessage.text = "Invalid username or password"
                    }
                }
            }

            Text {
                id: errorMessage
                color: "red"
                visible: errorMessage.text.length > 0
            }
        }

        onLoginSuccessful: {

        }
    }

    // main screen
    Column {
        id:mainScreen
        visible:loggedIn

        Row {
            width: parent.width * 0.8
            spacing: 5
            ToolButton {
                text: qsTr("Back")
                onClicked: fileClient.sendRequest("cdUp");
            }

            ToolButton {
                text: qsTr("Upload")
                onClicked: uploadDialog.open()
            }

            ToolButton {
                text: qsTr("Download")
                onClicked: downloadDialog.open()
            }

            ToolButton {
                text: "Refresh"
                onClicked: {
                    fileClient.sendRequest("list")
                }
            }
        }

        Column {

            anchors.topMargin: parent
            anchors.left: parent.left
            spacing: 10

            Text {
                id: currentUpload
                text: "File to upload:" + uploadDialog.selectedFile
            }

            Text {
                id: currentDownload
                text: "Download location:" + downloadDialog.currentFolder
            }


            TextField {
                id: filePathField
                placeholderText: "Enter new file name or rename file"
                width: parent.width * 0.8
            }

            Component.onCompleted: {
                fileClient.sendRequest("list")
            }

            ListModel {
                id: listModel

                ListElement {
                    fileName: ""
                    isDir: false
                }
            }

            ListView {
                id: fileListView
                width: parent.width * 0.5
                height: parent.height * 0.6
                model: listModel
                // orientation: ListView.Horizontal
                spacing: 10
                currentIndex: -1

                delegate: ItemDelegate {
                    text: model.fileName
                    icon.source: model.isDir ? "/icons/folder_closed.svg" : "/icons/generic_file.svg"
                    highlighted: ListView.isCurrentItem
                    onClicked: {
                        fileListView.currentIndex = index
                        selectedFile = text
                        selectedItem = model
                    }

                    onDoubleClicked: {
                        if(model.isDir)
                            fileClient.sendRequest("cd", model.fileName)
                    }
                }
            }

            Row {
                anchors.left: parent.left
                spacing: 5

                Button {
                    text: "Create File"
                    onClicked: {
                        if (filePathField.text == "") {
                            errorMsg = "Please add name to create"
                        } else {
                            errorMsg = ""
                            fileClient.sendRequest("create", filePathField.text)
                        }
                    }
                }

                Button {
                    text: "Create Directory"
                    onClicked: {
                        if (filePathField.text == "") {
                            errorMsg = "Please add name to create"
                        } else {
                            errorMsg = ""
                            fileClient.sendRequest("mkdir", filePathField.text)
                        }
                    }
                }


                Button {
                    text: "Delete File"
                    onClicked: {
                        if (selectedFile == "") {
                            errorMsg = "Please choose file to delete"
                        } else if(selectedItem == null || selectedItem.isDir) {
                            errorMsg = selectedFile + " is a directory"
                        } else {
                            errorMsg = ""
                            fileClient.sendRequest("delete", selectedFile)
                        }
                    }
                }

                Button {
                    text: "Rename File"
                    onClicked: {
                        if (selectedFile == "") {
                            errorMsg = "Please choose file to rename"
                        } else if(selectedItem == null || selectedItem.isDir) {
                            errorMsg = selectedFile + " is a directory"
                        } else {
                            errorMsg = ""
                            fileClient.sendRequest("rename", selectedFile, filePathField.text)
                        }
                    }
                }


                Button {
                    text: "Upload File"
                    onClicked: {
                        if (uploadDialog.selectedFile == "") {
                            errorMsg = "Please choose file to upload"
                        } else {
                            errorMsg = ""
                            fileClient.uploadFile(uploadDialog.selectedFile)
                        }
                    }
                }

                Button {
                    text: "Download File"
                    onClicked: {
                        if (downloadDialog.currentFolder == "") {
                            errorMsg = "Please choose download location"
                        } else if(selectedItem == null || selectedItem.isDir) {
                            errorMsg = selectedFile + " is a directory"
                        } else {
                            errorMsg = ""
                            fileClient.downloadFile(selectedFile, downloadDialog.currentFolder);
                        }
                    }
                }
            }

            Text {
                text: errorMsg
                color: "red"
                visible: text.length > 0
            }

            Connections {
                target: fileClient
                onFileListReceived: (files, isDir) => {
                    // console.log(files);
                    listModel.clear();
                    for (var i = 0; i < files.length; ++i) {
                        listModel.append({"fileName": files[i], "isDir": isDir[i]});
                    }

                }

                onError: (message) => {
                    errorMsg = message;
                }
            }
        }

        FileDialog {
            id: uploadDialog
            title: "Choose a file to upload"
            // onAccepted: {
            //     console.log(uploadDialog.selectedFile)
            // }
        }

        FolderDialog {
            id: downloadDialog
            title: "Choose a download location"
            currentFolder: ""
            // onAccepted: {
            //     console.log(downloadDialog.currentFolder)
            // }
        }

        Button {
            id: logOut
            text: "Log Out"
            onClicked: loggedIn = false
        }
    }
}
