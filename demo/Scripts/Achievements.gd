extends Node

var achievement_manager:GDKAchievements;
var achievements:Array[GDKAchievement];

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
			"Unlock Achievement With Index",
			false,
			Callable(self, "unlock_achievement_with_index"),
			false,
			[BaseScript.createInputInfo(BaseScript.InputType.string, "Achievement Index")]
		),
			
		BaseScript.createButtonInfo(
			"Unlock Achievement With ID",
			false,
			Callable(self, "unlock_achievement_with_id"),
			false,
			[BaseScript.createInputInfo(BaseScript.InputType.string, "Achievement ID")]
		),
		
		BaseScript.createButtonInfo(
			"Set Achievement percentage With Index",
			false,
			Callable(self, "set_achievement_percentage_with_index"),
			false,
			[BaseScript.createInputInfo(BaseScript.InputType.string, "Achievement Index"),
			BaseScript.createInputInfo(BaseScript.InputType.string, "Percentage")]
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

func _ready() -> void:
	achievement_manager = GDKAchievements.new()
	print("Achievements initialized")
	
func get_achievements(output:LineEdit, max_items:SpinBox, skip_items:SpinBox, order:OptionButton, unlockedOnly:CheckBox, achievementType:OptionButton) -> void:
	var result = await achievement_manager.get_achievements_async(XUser.user, achievementType.get_selected_id(), unlockedOnly.button_pressed, order.get_selected_id(), int(skip_items.value), int(max_items.value)).completed
	
	if result["hresult"] != 0:
		output.text = "Failed to retrieve achievements: 0x%08ux" % result["hresult"]
		return
		
	var handle:GDKAchievementsResultHandle = result["result"]
	
	achievements = achievement_manager.get_achievements_result(handle)
	
	while achievement_manager.has_more_achievements(handle):
		print("get more achievements")
		result = await achievement_manager.get_next_achievements_async(handle, int(max_items.value)).completed
		
		if result["hresult"] != 0:
			output.text = "Failed to retrieve achievements: 0x%08ux" % result["hresult"]
			return
		
		handle = result["result"]
		achievements += achievement_manager.get_achievements_result(handle)
	
	var achievementNames:String = ""
	
	for a in achievements:
		print(a.name);
		achievementNames += "\""+a.name+"\", "
	
	output.text = achievementNames
	
func get_achievement(output:LineEdit, idInput:LineEdit) -> void:
	print("Start retrieving achievement")
	var result = await achievement_manager.get_achievement(XUser.user, idInput.text).completed
	
	if result["hresult"] != 0:
		output.text = "Failed to retrieve achievements: 0x%08ux" % result["hresult"]
		return
		
	var handle:GDKAchievementsResultHandle = result["result"]
	var achievements_array:Array[GDKAchievement] = achievement_manager.get_achievements_result(handle)
	
	if(achievements.size() > 0):
		var achievement:GDKAchievement = achievements_array[0]
		var progression:GDKXblAchievementProgression = achievement.progression
		output.text = achievement.name + " " + achievement.id + " " + str(achievement.is_secret) + " " + achievement.locked_desc + " " + str(progression.time_unlocked)
		
func unlock_achievement_with_index(indexInput:LineEdit):
	if achievements == null:
		printerr("Achievement list is null. Retrieve achievements first")
		return
		
	if !indexInput.text.is_valid_int():
		printerr("The given index isn't valid int. Either give a number or use unlock_achievement_with_id")
		return
	
	var result = await achievement_manager.unlock_achievement(XUser.user, achievements[int(indexInput.text)].id).completed
	print("Hresult: 0x%08ux" % result["hresult"])
	
	
func unlock_achievement_with_id(idInput:LineEdit):
	var result = await achievement_manager.unlock_achievement(XUser.user, idInput.text).completed
	print("Hresult: 0x%08ux" % result["hresult"])
		
func set_achievement_percentage_with_index(percentage:LineEdit, indexInput:LineEdit):
	if achievements == null:
		printerr("Achievement list is null. Retrieve achievements first")
		return
		
	if !indexInput.text.is_valid_int():
		printerr("The given index isn't valid int. Either give a number or use unlock_achievement_with_id")
		return
	
	var result = await achievement_manager.set_achievement_percentage(XUser.user, achievements[int(indexInput.text)].id, int(percentage.text)).completed
	print("Hresult: 0x%08ux" % result["hresult"])
	
	
func set_achievement_percentage_with_id(percentage:LineEdit, idInput:LineEdit):
	var result = await achievement_manager.set_achievement_percentage(XUser.user, idInput.text, int(percentage.text)).completed
	print("Hresult: 0x%08ux" % result["hresult"])
