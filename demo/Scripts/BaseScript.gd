extends Node
class_name BaseScript

var currentNode:Control;
@export var SCID:String

static func createButtonInfo(text:String, requiresSubMenu:bool, function:Callable, hasOutput:bool = false, inputs = []):
	return {
		"name" = text,
		"submenu" = requiresSubMenu,
		"function" = function,
		"output" = hasOutput,
		"inputs" = inputs
	}

static func createInputInfo(useEnumInputs:bool, input):
	return {
		"useEnumInputs" = useEnumInputs,
		"input" = input
	}
	


func _ready():
	GDK.InitializeGDK(on_init_done, SCID)
	create_menu("MainMenu")
	currentNode = $Control/MainMenu

func create_menu(menuName:String, previousMenu:String = ""):
	#Create the control as base for new menu
	var control := Control.new()
	control.name = menuName

	#Load script for the new menu if there is one
	if ResourceLoader.exists("res://Scripts/"+menuName+".gd", "Script"):
		var script := load("res://Scripts/"+menuName+".gd")
		control.set_script(script)

	$Control.add_child(control)
	control.visible=false
	
	var scrollContainer = ScrollContainer.new();
	scrollContainer.horizontal_scroll_mode = ScrollContainer.SCROLL_MODE_DISABLED
	scrollContainer.custom_minimum_size = DisplayServer.window_get_size()
	control.add_child(scrollContainer);
	
	#Create grid container for all the buttons
	var gridContainer := GridContainer.new()
	gridContainer.name = "Buttons"
	scrollContainer.add_child(gridContainer)

	#Load script for the new menu if there is one
	if control.has_method("get_infos"):
		for info in control.get_infos():

			var horizontalContainer := GridContainer.new();
			var columns := 1;
			gridContainer.add_child(horizontalContainer)

			#Create button to the new menu
			var button := Button.new()
			button.text = info["name"]
			horizontalContainer.add_child(button)

			for input in info["inputs"]:
				var inputField = create_input_field(input);
				
				horizontalContainer.add_child(inputField)
				info["function"] = info["function"].bind(inputField)
				columns += 1

			if info["output"]:
				var label := LineEdit.new()
				label.text = "Output"
				label.editable = false;
				label.expand_to_text_length = true
				label.add_theme_color_override("font_uneditable_color", Color(1,1,1,1))
				horizontalContainer.add_child(label)
				info["function"] = info["function"].bind(label)
				columns += 1

			horizontalContainer.columns = columns
			button.pressed.connect(info["function"])

			#If the menu should have submenus, start same function with new info
			if info["submenu"]:
				create_menu(info["name"], menuName)

	if previousMenu != "":
		#Add back button as last button to go back to previous menus
		var returnButton := Button.new()
		returnButton.text = "Back"
		returnButton.pressed.connect(Callable(self, "_button_pressed").bind(previousMenu))
		gridContainer.add_child(returnButton)

func create_input_field(input):
	var inputField
	
	if(input["useEnumInputs"]):
		inputField = OptionButton.new()
		var enumValues = input["input"]
		
		for key in enumValues:
			inputField.add_item(key, enumValues[key])
	else:
		inputField = LineEdit.new()
		inputField.expand_to_text_length = true
		inputField.placeholder_text = input["input"]
	
	return inputField

func _button_pressed(menu:String):
	currentNode.visible = false;
	currentNode = get_node("Control/" + menu)
	currentNode.visible = true;

func on_init_done() -> void:
	print("GDK Initialization finished")
	var node = get_node("Control/Saves")
	node.InitializeGameSave();
	currentNode.visible = true;
