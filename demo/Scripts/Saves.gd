extends Node

var saveSystem:GDKGameSave;

func get_infos():
	return [
		BaseScript.buttonInfo("Load File", false, Callable(self, "ReadBlob"), true, ["File name"]),
		BaseScript.buttonInfo("Save File as String", false, Callable(self, "WriteStringBlob"), false, ["File name", "Data to save"]),
		BaseScript.buttonInfo("Save File as Byte Array", false, Callable(self, "WriteByteBlob"), false, ["File name", "Data to save"])
	]

func InitializeGameSave() -> void:
	saveSystem = GDKGameSave.new()
	saveSystem.InitializeGameSaveProviderAsync(InitializeGameSaveCallback, false, "TestGameName");

func InitializeGameSaveCallback() -> void:
	print("Finished initializing Game Save Provider")

func ReadBlob(output:Label, fileName:LineEdit) -> void:
	var blobs:Array = saveSystem.ReadBlobData("test", [fileName.text]);

	if blobs.size() > 0:
		var test:GDKGameSaveBlob = blobs[0]
		output.text = test.data.get_string_from_utf8();

func WriteStringBlob(dataInput:LineEdit, fileName:LineEdit):
	saveSystem.WriteBlobDataString(fileName.text, dataInput.text);

func WriteByteBlob(dataInput:LineEdit, fileName:LineEdit):
	saveSystem.WriteBlobDataByte(fileName.text, dataInput.text.to_utf8_buffer());
