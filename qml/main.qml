import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ApplicationWindow {
	title: "qSokoban"
	visible: true
	objectName: "Window"
	width: board.width
	height: board.height + toolbar.height + infoBar.height
	toolBar: ToolBar {
		id: toolbar

		RowLayout {
			Button {
				id: restartButton
				text: "Restart"
				iconName: "restart"
			}

			Button {
				id: prevButton
				text: "Previous level"
				iconName: "previous"
			}

			Button {
				id: nextButton
				text: "Next level"
				iconName: "next"
			}

			Button {
				id: quitButton
				text: "Quit"
				iconName: "quit"
			}
		}
	}
	statusBar: StatusBar {
		id: infoBar

		RowLayout {
			Label {
				id: collectionInfo
			}
			Label {
				text: "Level:"
			}
			Label {
				text: "Steps:"
			}
			Label {
				text: "Steps:"
			}
			Label {
				text: "pushes:"
			}
			Label {
				text: "pushes:"
			}
		}
	}

	Board {
		id: board
		objectName: "Board"
		focus: true
		width: level.width * 64
		height: level.height * 64
	}
	Component.onCompleted: board.forceActiveFocus()
}

