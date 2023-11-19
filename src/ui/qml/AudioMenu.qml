import QtQuick 2.12
import QtQuick.Controls 2.12

import rc 1.0

Menu {
	id: audioMenu
    title: "Audio"
    width: 200

	Instantiator {
		model: Audio
		delegate: MenuItem {
			contentItem: Row {
				Text {
					width: 20
					text: selected ? "\u2022" : ""
				}
				Text {
					text: label + " - " + codec + " " + channels
				}
			}
			enabled: !selected
			onTriggered: player.changeAudioTrack(id);
		}
		onObjectAdded: audioMenu.insertItem(index, object)
		onObjectRemoved: audioMenu.removeItem(object)
	}
}
