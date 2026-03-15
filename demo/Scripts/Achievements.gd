extends Node

var callback_context:int;

func get_infos():
	return [
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
		),
			
		BaseScript.createButtonInfo(
			"Set Callback for achievement changes",
			false,
			Callable(self, "add_achievement_progress_change_handler"),
			true
		),
			
		BaseScript.createButtonInfo(
			"Remove Callback for achievement changes",
			false,
			Callable(self, "remove_achievement_progress_change_handler"),
			false
		)
	]
	
func get_achievements(output:LineEdit, max_items:SpinBox, skip_items:SpinBox, order:OptionButton, unlockedOnly:CheckBox, achievementType:OptionButton) -> void:
	var result = await GDKAchievements.get_achievements_async(XUser.user, achievementType.get_selected_id(), unlockedOnly.button_pressed, order.get_selected_id(), int(skip_items.value), int(max_items.value)).completed
	
	if result["hresult"] != 0:
		output.text = "Failed to retrieve achievements: 0x%08ux" % result["hresult"]
		return
		
	var handle:GDKAchievementsResultHandle = result["result"]
	
	var achievements:Array[GDKAchievement] = GDKAchievements.get_achievements_result(handle)
	
	while GDKAchievements.has_more_achievements(handle):
		print("get more achievements")
		result = await GDKAchievements.get_next_achievements_async(handle, int(max_items.value)).completed
		
		if result["hresult"] != 0:
			output.text = "Failed to retrieve achievements: 0x%08ux" % result["hresult"]
			return
		
		handle = result["result"]
		achievements += GDKAchievements.get_achievements_result(handle)
	
	var achievementNames:String = ""
	
	for a in achievements:
		print(a.name);
		achievementNames += "\""+a.name+"\", "
	
	output.text = achievementNames
	
func get_achievement(output:LineEdit, idInput:LineEdit) -> void:
	print("Start retrieving achievement")
	var result = await GDKAchievements.get_achievement(XUser.user, idInput.text).completed
	
	if result["hresult"] != 0:
		output.text = "Failed to retrieve achievements: 0x%08ux" % result["hresult"]
		return
		
	var handle:GDKAchievementsResultHandle = result["result"]
	var achievements:Array[GDKAchievement] = GDKAchievements.get_achievements_result(handle)
	
	if(achievements.size() > 0):
		var achievement:GDKAchievement = achievements[0]
		var progression:GDKXblAchievementProgression = achievement.progression
		output.text = achievement.name + " " + achievement.id + " " + str(achievement.is_secret) + " " + achievement.locked_desc + " " + str(progression.time_unlocked)
	
func unlock_achievement_with_id(idInput:LineEdit):
	var result = await GDKAchievements.unlock_achievement(XUser.user, idInput.text).completed
	print("Hresult: 0x%08ux" % result["hresult"])
	
func set_achievement_percentage_with_id(percentage:LineEdit, idInput:LineEdit):
	var result = await GDKAchievements.set_achievement_percentage(XUser.user, idInput.text, int(percentage.text)).completed
	print("Hresult: 0x%08ux" % result["hresult"])

func add_achievement_progress_change_handler(output:LineEdit) -> void:
	if callback_context != 0:
		output.text = "Callback already exists"
		
	callback_context = GDKAchievements.add_achievement_progress_change_handler(func (changed_achievements:Array[GDKXblAchievementProgressChangeEntry]):
		print("Finished callback")
		for changed_entry in changed_achievements:
			output.text = "ID: " + changed_entry.achievement_id + ", Current Progress:" + changed_entry.progression.requirements[0].current_progress_value
		
	)
	output.text = "Callback added"
	
func remove_achievement_progress_change_handler() -> void:
	GDKAchievements.remove_achievement_progress_change_handler(callback_context)
	callback_context = 0;
