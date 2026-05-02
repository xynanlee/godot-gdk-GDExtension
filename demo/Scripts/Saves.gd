extends Node

var saveSystem:GDKGameSave;

func get_infos():
	return [
		BaseScript.createButtonInfo(
			"Load File",
			false,
			Callable(self, "ReadBlob"),
			true,
			[BaseScript.createInputInfo(BaseScript.InputType.string, "File name")]
		),
		
		BaseScript.createButtonInfo(
			"Save File as String",
			false,
			Callable(self, "WriteStringBlob"),
			false,
			[
				BaseScript.createInputInfo(BaseScript.InputType.string, "File name"),
				BaseScript.createInputInfo(BaseScript.InputType.string, "Data to save")
			]
		),
		
		BaseScript.createButtonInfo(
			"Save File as Byte Array",
			false,
			Callable(self, "WriteByteBlob"),
			false,
			[
				BaseScript.createInputInfo(BaseScript.InputType.string, "File name"),
				BaseScript.createInputInfo(BaseScript.InputType.string, "Data to save")
			]
		)
	]

func InitializeGameSave() -> void:
	#saveSystem = GDKGameSave.new()
	#saveSystem.InitializeGameSaveProviderAsync(InitializeGameSaveCallback, false, "TestGameName");
	pass

func ReadBlob(output:LineEdit, fileName:LineEdit) -> void:
	#var blobs:Array = saveSystem.ReadBlobData("test", [fileName.text]);
#
	#if blobs.size() > 0:
		#var test:GDKGameSaveBlob = blobs[0]
		#output.text = test.data.get_string_from_utf8();
	pass

func WriteStringBlob(dataInput:LineEdit, fileName:LineEdit):
	#saveSystem.WriteBlobDataString(fileName.text, dataInput.text);
	pass

func WriteByteBlob(dataInput:LineEdit, fileName:LineEdit):
	#saveSystem.WriteBlobDataByte(fileName.text, dataInput.text.to_utf8_buffer());
	pass
