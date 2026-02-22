extends Node

var achievement:GDKAchievements;
var achievements:Array[GDKAchievement];

func get_infos():
	return [
		BaseScript.createButtonInfo(
			"Get Achievements",
			false,
			Callable(self,"get_achievements"),
			true
		),
		
		BaseScript.createButtonInfo(
			"Unlock Achievement With Index",
			false,
			Callable(self, "unlock_achievement_with_index"),
			false,
			[BaseScript.createInputInfo(false, "Achievement Index")]
		),
			
		BaseScript.createButtonInfo(
			"Unlock Achievement With ID",
			false,
			Callable(self, "unlock_achievement_with_id"),
			false,
			[BaseScript.createInputInfo(false, "Achievement ID")]
		)
	]

func _ready() -> void:
	achievement = GDKAchievements.new()
	print("Achievements initialized")
	
func get_achievements(output:LineEdit) -> void:
	print("Start retrieveing achievements")
	var callable:Callable = Callable(self, "receive_achievements").bind(output)
	achievement.GetAchievements(callable)
	
func receive_achievements(received_achievements:Array, output:LineEdit):
	print("Received achievements");
	
	achievements = []
	var achievementNames:String = ""
	
	for a in received_achievements:
		print(a.name);
		achievementNames += "\""+a.name+"\", "
		achievements.push_back(a as GDKAchievement)
	
	output.text = achievementNames
		
func unlock_achievement_with_index(indexInput:LineEdit):
	if achievements == null:
		printerr("Achievement list is null. Retrieve achievements first")
		return
		
	if !indexInput.text.is_valid_int():
		printerr("The given index isn't valid int. Either give a number or use unlock_achievement_with_id")
		return
	
	achievement.UnlockAchievement(achievements[int(indexInput.text)].id)
	
	
func unlock_achievement_with_id(idInput:LineEdit):
	achievement.UnlockAchievement(idInput.text)
