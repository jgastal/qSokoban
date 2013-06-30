import QtQuick 2.0

Item {
	focus: true
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

	GridView {
		id: map
		model: level
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
		model: level.boxes
		Image {
			x: modelData.x * 64
			y: modelData.y * 64
			width: 64
			height: 64
			source: "qrc:/images/box.png"
		}
	}

	Image {
		x: level.manPos.x * 64
		y: level.manPos.y * 64
		width: 64
		height: 64
		source: "qrc:/images/man.png"
		Behavior on x { SmoothedAnimation { velocity: 400; } }
		Behavior on y { SmoothedAnimation { velocity: 400; } }
	}
}
