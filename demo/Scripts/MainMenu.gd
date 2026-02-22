extends Node
	
func get_infos():
	var node = get_node("/root/Node")
	return [
		BaseScript.createButtonInfo(
			"Achievements",
			true,
			Callable(node, "_button_pressed").bind("Achievements")
		),
			
		BaseScript.createButtonInfo(
			"Saves",
			true,
			Callable(node, "_button_pressed").bind("Saves")
		),
			
		BaseScript.createButtonInfo(
			"XUser",
			true,
			Callable(node, "_button_pressed").bind("XUser")
		)
	]
