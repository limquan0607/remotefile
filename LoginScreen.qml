import QtQuick 2.15
import QtQuick.Controls 2.15

// Rectangle {
//     id: loginScreen
//     width: 400
//     height: 300
//     color: "lightgray"
//     visible: true

//     signal loginSuccessful

//     Column {
//         anchors.centerIn: parent
//         spacing: 10

//         TextField {
//             id: usernameField
//             placeholderText: "Username"
//             width: parent.width * 0.8
//         }

//         TextField {
//             id: passwordField
//             placeholderText: "Password"
//             width: parent.width * 0.8
//             echoMode: TextInput.Password
//         }

//         Button {
//             text: "Login"
//             onClicked: {
//                 if (usernameField.text === "user" && passwordField.text === "password") {
//                     loginScreen.loginSuccessful()
//                 } else {
//                     errorMessage.text = "Invalid username or password"
//                 }
//             }
//         }

//         Text {
//             id: errorMessage
//             color: "red"
//             visible: errorMessage.text.length > 0
//         }
//     }
// }

Rectangle {
    width: 200
    height: 200
    color: "red"

    Text {
        text: "Login Screen"
        anchors.centerIn: parent
    }
}
