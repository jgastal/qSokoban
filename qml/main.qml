import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ApplicationWindow {
	title: "qSokoban"
	visible: true
	objectName: "Window"
	x: game.readSetting("x") ? game.readSetting("x") : 0
	y: game.readSetting("y") ? game.readSetting("y") : 0
	width: game.readSetting("width") ? game.readSetting("width") : 800
	height: game.readSetting("height") ? game.readSetting("height") : 800
	property var currentLevel: game.currentCollection.currentLevel
	toolBar: ToolBar {
		id: toolbar

		RowLayout {
			Button {
				text: "Reset"
				iconName: "reset"
				onClicked: currentLevel.reset()
			}

			Button {
				text: "Undo"
				iconName: "undo"
				enabled: currentLevel.canUndo
				onClicked: currentLevel.undo()
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
				text: "Level:" + game.currentCollection.levelNumber + "/" + game.currentCollection.numberOfLevels
			}
			Label {
				text: "Steps:" + currentLevel.steps
			}
			Label {
				text: "pushes:"  + currentLevel.pushes
			}
		}
	}

	function min(a, b)
	{
		return a <= b ? a : b;
	}

	Board {
		id: board
		objectName: "Board"
		focus: true
		tileSize: min(parent.width / currentLevel.width, parent.height / currentLevel.height)
		level: currentLevel
	}
	Component.onCompleted: board.forceActiveFocus()
	Component.onDestruction: {
		game.saveSetting("x", x);
		game.saveSetting("y", y);
		game.saveSetting("width", width);
		game.saveSetting("height", height);
	}
}
