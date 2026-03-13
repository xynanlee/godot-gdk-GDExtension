extends Node

func get_infos():
	return [
		BaseScript.createButtonInfo(
			"Get Xbox Title ID",
			false,
			Callable(self, "get_xbox_title_id"),
			true
		),
		BaseScript.createButtonInfo(
			"Launch New Game",
			false,
			Callable(self, "launch_new_game"),
			false,
			[
				BaseScript.createInputInfo(false, "Exe Path"),
				BaseScript.createInputInfo(false, "Args")
			]
		),
		BaseScript.createButtonInfo(
			"Launch Restart On Crash",
			false,
			Callable(self, "launch_restart_on_crash"),
			false,
			[BaseScript.createInputInfo(false, "Args")]
		),
	]

func get_xbox_title_id(output: LineEdit) -> void:
	var title_id = GDKGame.get_xbox_title_id()
	output.text = str(title_id)

func launch_new_game(exe_path: LineEdit, args: LineEdit) -> void:
	GDKGame.launch_new_game(exe_path.text, args.text, null)

func launch_restart_on_crash(args: LineEdit) -> void:
	var result = GDKGame.launch_restart_on_crash(args.text)
	print("launch_restart_on_crash hresult: 0x%08ux" % result)
