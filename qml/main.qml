import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ApplicationWindow {
	title: "qSokoban"
	visible: true
	objectName: "Window"
	width: 800
	height: 800
	toolBar: ToolBar {
		id: toolbar

		RowLayout {
			Button {
				text: "Reset"
				iconName: "reset"
				onClicked: game.currentCollection.currentLevel.reset()
			}

			Button {
				text: "Undo"
				iconName: "undo"
				enabled: game.currentCollection.currentLevel.canUndo
				onClicked: game.currentCollection.currentLevel.undo()
			}

			Button {
				text: "Previous level"
				iconName: "previous"
				enabled: game.currentCollection.hasPreviousLevel
				onClicked: game.currentCollection.previousLevel()
			}

			Button {
				text: "Next level"
				iconName: "next"
				enabled: game.currentCollection.nextLevelUnlocked
				onClicked: game.currentCollection.nextLevel()
			}

			Button {
				text: "Quit"
				iconName: "quit"
				onClicked: Qt.quit()
			}

			ComboBox {
				model: game.collections
				onCurrentTextChanged: game.changeCollection(currentText)
				currentIndex: game.currentCollectionIndex
			}
		}
	}
	statusBar: StatusBar {
		id: infoBar

		RowLayout {
			Label {
				text: game.currentCollection.objectName
			}
			Label {
				text: "Level:" + game.currentCollection.levelNumber
			}
			Label {
				text: "Steps:" + game.currentCollection.currentLevel.steps
			}
			Label {
				text: "pushes:"  + game.currentCollection.currentLevel.pushes
			}
		}
	}

	Board {
		id: board
		objectName: "Board"
		focus: true
		width: game.currentCollection.currentLevel.width * 64
		height: game.currentCollection.currentLevel.height * 64
	}
	Component.onCompleted: board.forceActiveFocus()
}

