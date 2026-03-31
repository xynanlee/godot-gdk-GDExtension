extends Node
class_name XUser

static var user:GDKUser
var otherUser:GDKUser
var deferral
var texture_node:Sprite2D

func get_infos():
	return [
		BaseScript.createButtonInfo(
			"Add user async", 
			false, 
			Callable(self, "add_user_async"), 
			false,
			[BaseScript.createInputInfo(BaseScript.InputType.enums, {
				"None" : GDKXUserAddOptions.None, 
				"AddDefaultUserSilently" : GDKXUserAddOptions.AddDefaultUserSilently, 
				"AllowGuests": GDKXUserAddOptions.AllowGuests, 
				"AddDefaultUserAllowingUI": GDKXUserAddOptions.AddDefaultUserAllowingUI
				})]
		),
		BaseScript.createButtonInfo(
			"Add user by id with ui async", 
			false, 
			Callable(self, "add_user_by_id_with_ui_async"), 
			false,
			[BaseScript.createInputInfo(BaseScript.InputType.enums, {
				"None" : GDKXUserAddOptions.None, 
				"AddDefaultUserSilently" : GDKXUserAddOptions.AddDefaultUserSilently, 
				"AllowGuests": GDKXUserAddOptions.AllowGuests, 
				"AddDefaultUserAllowingUI": GDKXUserAddOptions.AddDefaultUserAllowingUI
				})]
		),
		BaseScript.createButtonInfo(
			"Get max user amount", 
			false, 
			Callable(self, "get_max_users"), 
			true
		),
		BaseScript.createButtonInfo(
			"Find user by id", 
			false,
			Callable(self, "find_user_by_id"), 
			false,
			[BaseScript.createInputInfo(BaseScript.InputType.string, "User ID")]
		),
		BaseScript.createButtonInfo(
			"Is signing out supported", 
			false,
			Callable(self, "is_sign_out_present"), 
			true
		),
		BaseScript.createButtonInfo(
			"Find user by local id", 
			false,
			Callable(self, "find_user_by_local_id"), 
			false,
			[BaseScript.createInputInfo(BaseScript.InputType.string, "User ID")]
		),
		BaseScript.createButtonInfo(
			"Find user by device id", 
			false,
			Callable(self, "find_user_by_device"), 
			true,
			[BaseScript.createInputInfo(BaseScript.InputType.string, "device id")]
		),
		BaseScript.createButtonInfo(
			"Duplicate user", 
			false,
			Callable(self, "duplicate_user")
		),
		BaseScript.createButtonInfo(
			"Close Handle", 
			false,
			Callable(self, "close_handle")
		),
		BaseScript.createButtonInfo(
			"Compare Users", 
			false,
			Callable(self, "compare"),
			true
		),
		BaseScript.createButtonInfo(
			"Sign out", 
			false,
			Callable(self, "sign_out"),
			true
		),
		BaseScript.createButtonInfo(
			"Get ID", 
			false,
			Callable(self, "get_id"),
			true
		),
		BaseScript.createButtonInfo(
			"Get local ID", 
			false,
			Callable(self, "get_local_id"),
			true
		),
		BaseScript.createButtonInfo(
			"Is Guest", 
			false,
			Callable(self, "is_guest"), 
			true
		),
		BaseScript.createButtonInfo(
			"Get user state", 
			false,
			Callable(self, "get_state"),
			true
		),
		BaseScript.createButtonInfo(
			"Get gamer tag", 
			false,
			Callable(self, "get_gamer_tag"),
			true,
			[BaseScript.createInputInfo(BaseScript.InputType.enums, {
				"Classic" : GDKXUserGamertagComponent.Classic,
				"Modern" : GDKXUserGamertagComponent.Modern,
				"ModernSuffix" : GDKXUserGamertagComponent.ModernSuffix,
				"UniqueModern" : GDKXUserGamertagComponent.UniqueModern
				})]
		),
		BaseScript.createButtonInfo(
			"Get gamer image", 
			false,
			Callable(self, "get_gamer_picture_async"), 
			false,
			[BaseScript.createInputInfo(BaseScript.InputType.enums, {
				"Small" : GDKXUserGamerPictureSize.Small, 
				"Medium" : GDKXUserGamerPictureSize.Medium, 
				"Large": GDKXUserGamerPictureSize.Large, 
				"ExtraLarge": GDKXUserGamerPictureSize.ExtraLarge
				} )]
		),
		BaseScript.createButtonInfo(
			"Get user age group", 
			false,
			Callable(self, "get_user_age_group"),
			true
		),
		BaseScript.createButtonInfo(
			"Check privileges", 
			false,
			Callable(self, "check_privilege"),
			true,
			[BaseScript.createInputInfo(BaseScript.InputType.enums, {
				"None" : GDKXUserPrivilegeOptions.None, 
				"AllUsers" : GDKXUserPrivilegeOptions.AllUsers
				} ),
			BaseScript.createInputInfo(BaseScript.InputType.enums, {
				"CrossPlay" : GDKXUserPrivilege.CrossPlay, 
				"Clubs" : GDKXUserPrivilege.Clubs,
				"Sessions" : GDKXUserPrivilege.Sessions, 
				"Broadcast" : GDKXUserPrivilege.Broadcast,
				"ManageProfilePrivacy" : GDKXUserPrivilege.ManageProfilePrivacy, 
				"GameDvr" : GDKXUserPrivilege.GameDvr,
				"MultiplayerParties" : GDKXUserPrivilege.MultiplayerParties, 
				"CloudManageSession" : GDKXUserPrivilege.CloudManageSession,
				"CloudJoinSession" : GDKXUserPrivilege.CloudJoinSession, 
				"CloudSavedGames" : GDKXUserPrivilege.CloudSavedGames,
				"SocialNetworkSharing" : GDKXUserPrivilege.SocialNetworkSharing, 
				"UserGeneratedContent" : GDKXUserPrivilege.UserGeneratedContent,
				"Communications" : GDKXUserPrivilege.Communications, 
				"Multiplayer" : GDKXUserPrivilege.Multiplayer,
				"AddFriends" : GDKXUserPrivilege.AddFriends,
				} )]
		),
		BaseScript.createButtonInfo(
			"Resolve privileges with ui async", 
			false,
			Callable(self, "resolve_privilege_with_ui_async"),
			true,
			[BaseScript.createInputInfo(BaseScript.InputType.enums, {
				"None" : GDKXUserPrivilegeOptions.None, 
				"AllUsers" : GDKXUserPrivilegeOptions.AllUsers
				} ),
			BaseScript.createInputInfo(BaseScript.InputType.enums, {
				"CrossPlay" : GDKXUserPrivilege.CrossPlay, 
				"Clubs" : GDKXUserPrivilege.Clubs,
				"Sessions" : GDKXUserPrivilege.Sessions, 
				"Broadcast" : GDKXUserPrivilege.Broadcast,
				"ManageProfilePrivacy" : GDKXUserPrivilege.ManageProfilePrivacy, 
				"GameDvr" : GDKXUserPrivilege.GameDvr,
				"MultiplayerParties" : GDKXUserPrivilege.MultiplayerParties, 
				"CloudManageSession" : GDKXUserPrivilege.CloudManageSession,
				"CloudJoinSession" : GDKXUserPrivilege.CloudJoinSession, 
				"CloudSavedGames" : GDKXUserPrivilege.CloudSavedGames,
				"SocialNetworkSharing" : GDKXUserPrivilege.SocialNetworkSharing, 
				"UserGeneratedContent" : GDKXUserPrivilege.UserGeneratedContent,
				"Communications" : GDKXUserPrivilege.Communications, 
				"Multiplayer" : GDKXUserPrivilege.Multiplayer,
				"AddFriends" : GDKXUserPrivilege.AddFriends,
				} )]
		),
		BaseScript.createButtonInfo(
			"Get token and signature async", 
			false,
			Callable(self, "get_token_and_signature_async"),
			true
		),
		BaseScript.createButtonInfo(
			"Resolve issue with UI async", 
			false,
			Callable(self, "resolve_issue_with_ui_async"),
			true,
			[BaseScript.createInputInfo(BaseScript.InputType.string, "URL")]
		),
		BaseScript.createButtonInfo(
			"Get sign out deferral", 
			false,
			Callable(self, "get_sign_out_deferral"),
		),
		BaseScript.createButtonInfo(
			"Close sign out deferral", 
			false,
			Callable(self, "close_sign_out_deferral"),
		),
		BaseScript.createButtonInfo(
			"Find controller for user with UI async", 
			false,
			Callable(self, "find_controller_for_user_with_ui_async"),
			true
		),
		BaseScript.createButtonInfo(
			"Is store user", 
			false,
			Callable(self, "is_store_user"),
			true
		),
	]

func add_user_async(userAddOption:OptionButton) -> void:
	var data = await GDKUser.add_user_async(userAddOption.get_selected_id()).completed
	user = data["user"]
	print(user)
	
	
func add_user_by_id_with_ui_async(userAddOption:OptionButton) -> void:
	var data = await GDKUser.add_user_by_id_with_ui_async(userAddOption.get_selected_id()).completed
	if(data["hresult"] == 0):
		user = data["user"]

func get_max_users(output:LineEdit) -> void:
	var maxUsers = GDKUser.get_max_users()
	output.text = str(maxUsers)
	
func find_user_by_id(userId:LineEdit) -> void:
	otherUser = GDKUser.find_user_by_id(int(userId.text))
	print(otherUser)
	
func is_sign_out_present(output:LineEdit) -> void:
	output.text = str(GDKUser.is_sign_out_present())
	
func find_user_by_local_id(userId:LineEdit) -> void:
	otherUser = GDKUser.find_user_by_local_id(int(userId.text))
	print(otherUser)

func find_user_by_device(device:LineEdit) -> void:
	otherUser = GDKUser.find_user_by_device(device.text.to_utf8_buffer())
	print(otherUser)
	
func duplicate_user() -> void:
	otherUser = user.duplicate()
	print(otherUser)
	
func close_handle() -> void:
	user.close_handle()
	user = null

func compare(output:LineEdit) -> void:
	var equal = user.compare(otherUser)
	output.text = str(equal)

func sign_out(output:LineEdit) -> void:
	var result = await user.sign_out().completed
	output.text = "Hresult: 0x%08ux" % result["hresult"]
	
func get_id(output:LineEdit) -> void:
	var result = user.get_id()
	output.text = str(result)
	
func get_local_id(output:LineEdit) -> void:
	var result = user.get_local_id()
	output.text = str(result)
	
func is_guest(output:LineEdit) -> void:
	output.text = str(user.is_guest())
	
func get_state(output:LineEdit) -> void:
	output.text = str(user.get_state())

func get_gamer_tag(output:LineEdit, gamertag_type:OptionButton) -> void:
	var gamertag:String = user.get_gamer_tag(gamertag_type.get_selected_id())
	output.text = gamertag
	
func get_gamer_picture_async(imageSize:OptionButton) -> void:
	var data = await user.get_gamer_picture_async(imageSize.get_selected_id()).completed
	var image = data["image"]
	var imageTexture = ImageTexture.create_from_image(image)
	
	if(texture_node == null):
		texture_node = Sprite2D.new()
		texture_node.position = Vector2(500,200)
		texture_node.z_index = -100
		add_child(texture_node)
	
	texture_node.texture = imageTexture
	texture_node.offset = imageTexture.get_size() / 2

func get_user_age_group(output:LineEdit) -> void:
	output.text = str(user.get_user_age_group())
	
func check_privilege(output:LineEdit, privilege:OptionButton, options:OptionButton) -> void:
	output.text = str(user.check_privilege(options.get_selected_id(), privilege.get_selected_id()))

func resolve_privilege_with_ui_async(output:LineEdit, privilege:OptionButton, options:OptionButton) -> void:
	var result = await user.resolve_privilege_with_ui_async(options.get_selected_id(), privilege.get_selected_id()).completed
	output.text = "Hresult: 0x%08ux" % result["hresult"]

func get_token_and_signature_async(output:LineEdit):
	output.text = "Not Implemented"	

func resolve_issue_with_ui_async(output:LineEdit, url:LineEdit):
	var result = await user.resolve_issue_with_ui_async(url.text).completed
	output.text = "Hresult: 0x%08ux" % result["hresult"]

func get_sign_out_deferral():
	deferral = user.get_sign_out_deferral()

func close_sign_out_deferral_handle():
	user.close_sign_out_deferral_handle(deferral)
	
func find_controller_for_user_with_ui_async(output:LineEdit):
	var id = await user.find_controller_for_user_with_ui_async().completed
	output.text = str(id["device_id"])

func is_store_user(output:LineEdit):
	output.text = str(user.is_store_user())
