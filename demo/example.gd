extends Node

var example;
# Fill out with your games SCID
var SCID = "00000000-0000-0000-0000-000000000000";

func _ready() -> void:
	example = godot_gdk.new()
	example.InitializeGDK(SCID)
