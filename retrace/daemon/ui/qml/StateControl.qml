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
                Column {
                    Text {
                        id: nameText
                        text: modelData.name
                    }
                    Grid {
                        id: stateGrid
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 100
                        columns: 4
                        Repeater {
                            model: modelData.values
                            delegate: Component {
                                Rectangle {
                                    width: stateList.width / 4
                                    height: stateText.height
                                    border.width: 1
                                    Text {
                                        id: stateText
                                        text: modelData
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
