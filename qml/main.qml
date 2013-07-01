import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import Level 1.0

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
			ToolButton {
				text: "Reset"
				iconName: "edit-clear"
				iconSource: "qrc:/images/edit-clear.png"
				onClicked: currentLevel.reset()
			}

			ToolButton {
				text: "Undo"
				iconName: "edit-undo"
				iconSource: "qrc:/images/edit-undo.png"
				enabled: currentLevel.canUndo
				onClicked: currentLevel.undo()
			}

			ToolButton {
				text: "Previous level"
				iconName: "go-previous"
				iconSource: "qrc:/images/go-previous.png"
				enabled: game.currentCollection.hasPreviousLevel
				onClicked: game.currentCollection.previousLevel()
			}

			ToolButton {
				text: "Next level"
				iconName: "go-next"
				iconSource: "qrc:/images/go-next.png"
				enabled: game.currentCollection.nextLevelUnlocked
				onClicked: game.currentCollection.nextLevel()
			}

			ToolButton {
				text: "Quit"
				iconName: "application-exit"
				iconSource: "qrc:/images/application-exit.png"
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
		anchors.centerIn: parent
		objectName: "Board"
		focus: true
		tileSize: min(parent.width / currentLevel.board.width, parent.height / currentLevel.board.height)
		board: currentLevel.board
		Keys.onDownPressed: {
			currentLevel.move(Level.Down);
		}
		Keys.onUpPressed: {
			currentLevel.move(Level.Up);
		}
		Keys.onLeftPressed: {
			currentLevel.move(Level.Left);
		}
		Keys.onRightPressed: {
			currentLevel.move(Level.Right);
		}
		Keys.onPressed: {
			if (event.key === Qt.Key_Z && event.modifiers === Qt.ControlModifier && currentLevel.canUndo)
				currentLevel.undo();
		}
	}

	Rectangle {
		id: won
		anchors.centerIn: board
		width: 400
		height: 100
		color: "yellow"
		visible: false
		Connections {
			target: currentLevel
			onLevelCompleted: {
				won.visible = true
				hideTimer.running = true
			}
		}
		transitions: Transition {
			PropertyAnimation {
				properties: "visible"
				easing.type: Easing.InOutQuad;
			}
		}

		Text {
			anchors.centerIn: parent
			color: "red"
			text: "Level completed!"
			font.pointSize: 30
		}
		Timer {
			id: hideTimer
			interval: 500
			running: false
			onTriggered: won.visible = false
		}
	}

	Component.onCompleted: board.forceActiveFocus()
	Component.onDestruction: {
		game.saveSetting("x", x);
		game.saveSetting("y", y);
		game.saveSetting("width", width);
		game.saveSetting("height", height);
	}
}
