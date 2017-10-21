import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import ApiTrace 1.0

Item {
    property QStateModel stateModel

    ScrollView {
        anchors.fill: parent
        ListView {
            id: stateList
            model: stateModel.state
            anchors.fill: parent
            delegate: Component {
                Row {
                    visible: modelData.visible
                    Rectangle {
                        id: indent
                        width: nameText.height * modelData.indent
                        height: 1
                        opacity: 0.0
                    }
                    Rectangle {
                        id: collapse
                        width: nameText.height
                        height: nameText.height
                        visible: modelData.choices.length == 0
                        property var collapsed: false
                        color: collapse.collapsed ? "red" : "green"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (collapse.collapsed) {
                                    collapse.color = "green";
                                    stateModel.expand(modelData.path);
                                    collapse.collapsed = false
                                } else {
                                    collapse.color = "red";
                                    stateModel.collapse(modelData.path);
                                    collapse.collapsed = true
                                }
                            }
                        }
                    }
                    Text {
                        id: nameText
                        anchors.verticalCenter: parent.verticalCenter
                        text: modelData.name + " : "
                    }
                    ComboBoxFitContents {
                        anchors.verticalCenter: parent.verticalCenter
                        model: modelData.choices
                        currentIndex: modelData.value
                        visible: (modelData.choices.length > 0)
                        onActivated: {
                            stateModel.setState(modelData.group,
                                                modelData.path,
                                                modelData.name,
                                                modelData.choices[currentIndex]);
                        }
                    }
                }
            }
        }
    }
}
