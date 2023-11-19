import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.3

import rc 1.0

Window {
	id: root
	title: "RC Media Player"
	y: 0
	x: 0
	width: Screen.width/2
	height: Screen.height/2
	visible: false

	Shortcut {
		sequence: "Space"
		onActivated: player.togglePause()
	}

	Shortcut {
		sequence: "v"
		onActivated: player.toggleSubtitles()
	}

	Shortcut {
		sequence: "m"
		onActivated: player.toggleMute()
	}

	Shortcut {
		sequence: "Left"
		onActivated: player.seekPosition(-5)
	}

	Shortcut {
		sequence: "Right"
		onActivated: player.seekPosition(5)
	}

	Shortcut {
		sequence: "Esc"
		onActivated: player.stopMovie()
	}

	Menu {
		id: contextMenu
		AudioMenu {}
		VideoMenu {}
		SubtitleMenu {}
	}

	MpvPlayer {
		id: player
		objectName: "player"
		anchors.fill: parent

		MouseArea {
			anchors.fill: parent
			acceptedButtons: Qt.RightButton

			onClicked: {
				if (mouse.button == Qt.RightButton) {
					contextMenu.popup()
				}
			}
		}
	}

	TransportControls {
		id: transportControls
		visible: UiState.showControls
	}
	
	MouseArea {
		anchors.fill: parent
		enabled: false
		cursorShape: UiState.showControls ? Qt.ArrowCursor : Qt.BlankCursor
	}
}
