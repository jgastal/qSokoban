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
				enabled: collection.hasPreviousLevel
				onClicked: collection.previousLevel()
			}

			Button {
				id: nextButton
				text: "Next level"
				iconName: "next"
				enabled: collection.nextLevelUnlocked
				onClicked: collection.nextLevel()
			}

			Button {
				id: quitButton
				text: "Quit"
				iconName: "quit"
				onClicked: Qt.quit()
			}
		}
	}
	statusBar: StatusBar {
		id: infoBar

		RowLayout {
			Label {
				text: collection.objectName
			}
			Label {
				text: "Level:" + collection.levelNumber
			}
			Label {
				text: "Steps:" + collection.currentLevel.steps
			}
			Label {
				text: "pushes:"  + collection.currentLevel.pushes
			}
		}
	}

	Board {
		id: board
		objectName: "Board"
		focus: true
		width: collection.currentLevel.width * 64
		height: collection.currentLevel.height * 64
	}
	Component.onCompleted: board.forceActiveFocus()
}

