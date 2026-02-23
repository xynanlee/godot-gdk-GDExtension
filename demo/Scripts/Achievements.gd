extends Node

var achievement:GDKAchievements;
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
				})]
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
	achievement = GDKAchievements.new()
	print("Achievements initialized")
	
func get_achievements(output:LineEdit, order:OptionButton, unlockedOnly:CheckBox, achievementType:OptionButton) -> void:
	print("Start retrieving achievements")
	var result = await achievement.get_achievements(XUser.user, achievementType.get_selected_id(), unlockedOnly.button_pressed, order.get_selected_id()).completed
	print(result);
	achievements = result["achievements"]
	
	var achievementNames:String = ""
	
	for a in achievements:
		print(a.name);
		achievementNames += "\""+a.name+"\", "
	
	output.text = achievementNames
	
func get_achievement(output:LineEdit, idInput:LineEdit) -> void:
	print("Start retrieving achievement")
	var result = await achievement.get_achievement(XUser.user, idInput.text).completed
	
	if(result["hresult"] == 0):
		output.text = result["achievement"].name
	else:
		print("Hresult: 0x%08ux" % result["hresult"])
		
func unlock_achievement_with_index(indexInput:LineEdit):
	if achievements == null:
		printerr("Achievement list is null. Retrieve achievements first")
		return
		
	if !indexInput.text.is_valid_int():
		printerr("The given index isn't valid int. Either give a number or use unlock_achievement_with_id")
		return
	
	var result = await achievement.unlock_achievement(XUser.user, achievements[int(indexInput.text)].id).completed
	print("Hresult: 0x%08ux" % result["hresult"])
	
	
func unlock_achievement_with_id(idInput:LineEdit):
	var result = await achievement.unlock_achievement(XUser.user, idInput.text).completed
	print("Hresult: 0x%08ux" % result["hresult"])
		
func set_achievement_percentage_with_index(percentage:LineEdit, indexInput:LineEdit):
	if achievements == null:
		printerr("Achievement list is null. Retrieve achievements first")
		return
		
	if !indexInput.text.is_valid_int():
		printerr("The given index isn't valid int. Either give a number or use unlock_achievement_with_id")
		return
	
	var result = await achievement.set_achievement_percentage(XUser.user, achievements[int(indexInput.text)].id, int(percentage.text)).completed
	print("Hresult: 0x%08ux" % result["hresult"])
	
	
func set_achievement_percentage_with_id(percentage:LineEdit, idInput:LineEdit):
	var result = await achievement.set_achievement_percentage(XUser.user, idInput.text, int(percentage.text)).completed
	print("Hresult: 0x%08ux" % result["hresult"])
