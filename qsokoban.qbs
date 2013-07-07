import qbs 1.0

Application {
	name: "qsokoban"
	Depends { name: "cpp" }
	Depends { name: "Qt"; submodules: ["core", "qml", "quick"] }
	cpp.cxxFlags: "-std=c++11"
	files: ["*.cpp",
		"*.h",
		"qml/*.qml",
		"data.qrc"
	]
}
