import QtQuick 2.0

Item {
	focus: true
	width: level.width * tileSize
	height: level.height * tileSize
	property var level
	property int tileSize
	Keys.onDownPressed: {
		level.manPos.y += 1
	}
	Keys.onUpPressed: {
		level.manPos.y -= 1
	}
	Keys.onLeftPressed: {
		level.manPos.x -= 1
	}
	Keys.onRightPressed: {
		level.manPos.x += 1
	}
	Keys.onPressed: {
		if (event.key == Qt.Key_Z && event.modifiers == Qt.ControlModifier && level.canUndo)
			level.undo();
	}

	GridView {
		id: map
		model: level
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
		model: level.boxes
		Image {
			x: modelData.x * tileSize
			y: modelData.y * tileSize
			width: tileSize
			height: tileSize
			source: "qrc:/images/box.png"
		}
	}

	Image {
		x: level.manPos.x * tileSize
		y: level.manPos.y * tileSize
		width: tileSize
		height: tileSize
		source: "qrc:/images/man.png"
		Behavior on x { SmoothedAnimation { velocity: 400; } }
		Behavior on y { SmoothedAnimation { velocity: 400; } }
	}
}
