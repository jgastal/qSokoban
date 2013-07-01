import QtQuick 2.0

Item {
	focus: true
	property var currentLevel: game.currentCollection.currentLevel
	Keys.onDownPressed: {
		currentLevel.manPos.y += 1
	}
	Keys.onUpPressed: {
		currentLevel.manPos.y -= 1
	}
	Keys.onLeftPressed: {
		currentLevel.manPos.x -= 1
	}
	Keys.onRightPressed: {
		currentLevel.manPos.x += 1
	}
	Keys.onPressed: {
		if (event.key == Qt.Key_Z && event.modifiers == Qt.ControlModifier && currentLevel.canUndo)
			currentLevel.undo();
	}

	GridView {
		id: map
		model: currentLevel
		interactive: false
		anchors.fill: parent
		cellHeight: 64
		cellWidth: 64
		delegate: Item {
			Image {
				width: 64
				height: 64
				source: model.tileImage
			}
		}
	}
	Repeater {
		model: currentLevel.boxes
		Image {
			x: modelData.x * 64
			y: modelData.y * 64
			width: 64
			height: 64
			source: "qrc:/images/box.png"
		}
	}

	Image {
		x: currentLevel.manPos.x * 64
		y: currentLevel.manPos.y * 64
		width: 64
		height: 64
		source: "qrc:/images/man.png"
		Behavior on x { SmoothedAnimation { velocity: 400; } }
		Behavior on y { SmoothedAnimation { velocity: 400; } }
	}
}
