import QtQuick 2.0

Item {
	focus: true
	width: currentLevel.width * tileSize
	height: currentLevel.height * tileSize
	property var currentLevel: game.currentCollection.currentLevel
	property int tileSize
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
		cellHeight: tileSize
		cellWidth: tileSize
		delegate: Item {
			Image {
				width: tileSize
				height: tileSize
				source: model.tileImage
			}
		}
	}
	Repeater {
		model: currentLevel.boxes
		Image {
			x: modelData.x * tileSize
			y: modelData.y * tileSize
			width: tileSize
			height: tileSize
			source: "qrc:/images/box.png"
		}
	}

	Image {
		x: currentLevel.manPos.x * tileSize
		y: currentLevel.manPos.y * tileSize
		width: tileSize
		height: tileSize
		source: "qrc:/images/man.png"
		Behavior on x { SmoothedAnimation { velocity: 400; } }
		Behavior on y { SmoothedAnimation { velocity: 400; } }
	}
}
