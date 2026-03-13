extends Node

var invite_token: GDKGameInviteToken

func get_infos():
	return [
		BaseScript.createButtonInfo(
			"Register For Invite Event",
			false,
			Callable(self, "register_for_event")
		),
		BaseScript.createButtonInfo(
			"Unregister For Invite Event",
			false,
			Callable(self, "unregister_for_event"),
			false,
			[BaseScript.createInputInfo(true, {
				"Wait" : true,
				"No Wait" : false
			})]
		),
	]

func register_for_event() -> void:
	invite_token = GDKGameInvite.register_for_event(Callable(self, "on_invite_received"))
	print("Registered for game invite events")

func unregister_for_event(wait: OptionButton) -> void:
	if invite_token == null:
		printerr("No invite token. Register for event first.")
		return
	var result = GDKGameInvite.unregister_for_event(invite_token, wait.get_selected_id() == 0)
	print("Unregistered: ", result)
	invite_token = null

func on_invite_received(invite_uri: String) -> void:
	print("Game invite received: ", invite_uri)
