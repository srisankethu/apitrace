import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import ApiTrace 1.0

Item {
    property QStateModel stateModel
    ScrollView {
        anchors.fill: parent
        ListView {
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
                        columns: 1 // todo
                        Repeater {
                            model: modelData.values
                            delegate: Component {
                                Rectangle {
                                    width: stateGrid.width
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
