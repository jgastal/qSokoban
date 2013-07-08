import QtQuick 2.0
import Board 1.0

Item {
	focus: true
	width: board.width * tileSize
	height: board.height * tileSize
	property var board
	property int tileSize

	GridView {
		id: map
		model: board
		interactive: false
		anchors.fill: parent
		cellHeight: tileSize
		cellWidth: tileSize
		delegate: Item {
			Image {
				width: tileSize
				height: tileSize
				source: switch(model.display) {
					case Board.WALL: return "qrc:/images/wall.png"
					case Board.FLOOR: return "qrc:/images/floor.png"
					case Board.BOX_DESTINATION: return "qrc:/images/box_destination.png"
					case Board.OUTSIDE: return "qrc:/images/outside.png"
				}
			}
		}
	}
	Repeater {
		model: board.boxes
		Image {
			x: modelData.pos.x * tileSize
			y: modelData.pos.y * tileSize
			width: tileSize
			height: tileSize
			source: "qrc:/images/box.png"
			Behavior on x { SmoothedAnimation { velocity: 400; } }
			Behavior on y { SmoothedAnimation { velocity: 400; } }
		}
	}

	Image {
		x: board.manPos.x * tileSize
		y: board.manPos.y * tileSize
		width: tileSize
		height: tileSize
		source: "qrc:/images/man.png"
		Behavior on x { SmoothedAnimation { velocity: 400; } }
		Behavior on y { SmoothedAnimation { velocity: 400; } }
	}
}
