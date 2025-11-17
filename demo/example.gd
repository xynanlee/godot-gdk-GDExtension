extends Node

var example;

func _ready() -> void:
	example = godot_gdk.new()
	example.InitializeGDK(on_init_done)
	
func on_init_done() -> void:
	print("callback invoked")
	
	var achievement = gdk_achievements.new()
	achievement.GetAchievements()
