cp -r misc/dist/osx_tools.app ./Godot.app
mkdir -p Godot.app/Contents/MacOS
cp bin/godot.osx.tools.64 Godot.app/Contents/MacOS/Godot
chmod +x Godot.app/Contents/MacOS/Godot
