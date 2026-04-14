extends Node

func _ready() -> void:
	GDK.game_invite_received.connect(on_invite_received)

func _exit_tree() -> void:
	GDK.game_invite_received.disconnect(on_invite_received)

func get_infos():
	return []

func on_invite_received(invite_uri: String) -> void:
	print("Game invite received: ", invite_uri)
