extends Node

var gdk_achievements:GDKAchievements

func get_infos():
	return [
		BaseScript.createButtonInfo(
			"Initialize Achievements",
			false,
			Callable(self,"initialize"),
			true
		),
		BaseScript.createButtonInfo(
			"Get Achievements",
			false,
			Callable(self,"get_achievements"),
			true,
			[BaseScript.createInputInfo(BaseScript.InputType.enums, {
				"Unknown" : GDKXblAchievementType.Unknown, 
				"All" : GDKXblAchievementType.All, 
				"Persistent": GDKXblAchievementType.Persistent, 
				"Challenge": GDKXblAchievementType.Challenge
				}),
			BaseScript.createInputInfo(BaseScript.InputType.bool, "Unlocked only"),
			BaseScript.createInputInfo(BaseScript.InputType.enums, {
				"DefaultOrder" : GDKXblAchievementOrderBy.DefaultOrder, 
				"TitleId" : GDKXblAchievementOrderBy.TitleId, 
				"UnlockTime": GDKXblAchievementOrderBy.UnlockTime
				}),
			BaseScript.createInputInfo(BaseScript.InputType.int, "Skip Items"),
			BaseScript.createInputInfo(BaseScript.InputType.int, "Max Items")
			]
		),
		BaseScript.createButtonInfo(
			"Get Achievement",
			false,
			Callable(self,"get_achievement"),
			true,
			[BaseScript.createInputInfo(BaseScript.InputType.string, "Achievement ID")]
		),
			
		BaseScript.createButtonInfo(
			"Unlock Achievement With ID",
			false,
			Callable(self, "unlock_achievement_with_id"),
			false,
			[BaseScript.createInputInfo(BaseScript.InputType.string, "Achievement ID")]
		),
			
		BaseScript.createButtonInfo(
			"Set Achievement percentage With ID",
			false,
			Callable(self, "set_achievement_percentage_with_id"),
			false,
			[BaseScript.createInputInfo(BaseScript.InputType.string, "Achievement ID"),
			BaseScript.createInputInfo(BaseScript.InputType.string, "Percentage")]
		)
	]

func initialize(output:LineEdit):
	if XUser.user == null:
		printerr("User not initialized first.")
		return
	
	if gdk_achievements != null:
		printerr("Achievements already initialized")
		return
		
	gdk_achievements = GDKAchievements.create(XUser.user)
	gdk_achievements.achievement_progress_changed.connect(changed_achievements)
	output.text = "Achievements initialized"

func _exit_tree() -> void:
	if gdk_achievements != null:
		gdk_achievements.achievement_progress_changed.disconnect(changed_achievements)

func changed_achievements(achievements:Array[GDKXblAchievementProgressChangeEntry]) -> void:
	var output_text = "";
	for changed_entry in achievements:
		output_text += "ID: " + changed_entry.achievement_id + ", Current Progress:" + changed_entry.progression.requirements[0].current_progress_value
	print(output_text)
	
func get_achievements(output:LineEdit, max_items:SpinBox, skip_items:SpinBox, order:OptionButton, unlockedOnly:CheckBox, achievementType:OptionButton) -> void:
	var result = await gdk_achievements.get_achievements_async(achievementType.get_selected_id(), unlockedOnly.button_pressed, order.get_selected_id(), int(skip_items.value), int(max_items.value)).completed
	
	if result["hresult"] != 0:
		output.text = "Failed to retrieve achievements: 0x%08ux" % result["hresult"]
		return
		
	var handle:GDKAchievementsResultHandle = result["result"]
	
	var achievements:Array[GDKAchievement] = gdk_achievements.get_achievements_result(handle)
	
	while gdk_achievements.has_more_achievements(handle):
		print("get more achievements")
		result = await gdk_achievements.get_next_achievements_async(handle, int(max_items.value)).completed
		
		if result["hresult"] != 0:
			output.text = "Failed to retrieve achievements: 0x%08ux" % result["hresult"]
			return
		
		handle = result["result"]
		achievements += gdk_achievements.get_achievements_result(handle)
	
	var achievementNames:String = ""
	
	for a in achievements:
		print(a.name);
		achievementNames += "\""+a.name+"\", "
	
	output.text = achievementNames
	
func get_achievement(output:LineEdit, idInput:LineEdit) -> void:
	print("Start retrieving achievement")
	var result = await gdk_achievements.get_achievement(idInput.text).completed
	
	if result["hresult"] != 0:
		output.text = "Failed to retrieve achievements: 0x%08ux" % result["hresult"]
		return
		
	var handle:GDKAchievementsResultHandle = result["result"]
	var achievements:Array[GDKAchievement] = gdk_achievements.get_achievements_result(handle)
	
	if(achievements.size() > 0):
		var achievement:GDKAchievement = achievements[0]
		var progression:GDKXblAchievementProgression = achievement.progression
		output.text = achievement.name + " " + achievement.id + " " + str(achievement.is_secret) + " " + achievement.locked_desc + " " + str(progression.time_unlocked)
	
func unlock_achievement_with_id(idInput:LineEdit):
	var result = await gdk_achievements.unlock_achievement(idInput.text).completed
	print("Hresult: 0x%08ux" % result["hresult"])
	
func set_achievement_percentage_with_id(percentage:LineEdit, idInput:LineEdit):
	var result = await gdk_achievements.set_achievement_percentage(idInput.text, int(percentage.text)).completed
	print("Hresult: 0x%08ux" % result["hresult"])
