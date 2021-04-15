import QtQuick 2
import QtQuick.Layouts 1

import MuseScore.UiComponents 1
import MuseScore.UserScores 1

ColumnLayout {
    id: root
    spacing: 12

    property ExportDialogModel model
    property int firstColumnWidth

    property bool showBitRateControl: false

    CheckBox {
        text: qsTrc("userscores", "Normalize")
        checked: root.model.normalizeAudio
        onClicked: {
            root.model.normalizeAudio = !checked
        }
    }

    ExportOptionItem {
        Layout.fillWidth: false
        text: qsTrc("userscores", "Sample rate:")
        firstColumnWidth: root.firstColumnWidth

        StyledComboBox {
            Layout.preferredWidth: 126

            model: root.model.availableSampleRates().map(function (sampleRate) {
                return { text: qsTrc("userscores", "%1 Hz").arg(sampleRate), value: sampleRate }
            })

            textRoleName: "text"
            valueRoleName: "value"

            currentIndex: indexOfValue(root.model.sampleRate)
            onValueChanged: {
                root.model.sampleRate = value
            }
        }
    }

    ExportOptionItem {
        visible: root.showBitRateControl
        text: qsTrc("userscores", "Bitrate:")
        firstColumnWidth: root.firstColumnWidth

        StyledComboBox {
            Layout.preferredWidth: 126

            model: root.model.availableBitRates().map(function (bitRate) {
                return { text: qsTrc("userscores", "%1 kBit/s").arg(bitRate), value: bitRate }
            })

            textRoleName: "text"
            valueRoleName: "value"

            currentIndex: indexOfValue(root.model.bitRate)
            onValueChanged: {
                root.model.bitRate = value
            }
        }
    }

    StyledTextLabel {
        Layout.fillWidth: true
        text: qsTrc("userscores", "Each selected part will be exported as a separate audio file.")
        horizontalAlignment: Text.AlignLeft
        wrapMode: Text.WordWrap
    }
}
