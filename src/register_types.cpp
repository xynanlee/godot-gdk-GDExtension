#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <classes/engine.hpp>

#include "godot_gdk.h"
#include "gdk_asyncblock.h"
#include "gdk_achievement.h"
#include "gdk_achievements.h"
#include "gdk_game_save_files.h"
#include "gdk_game_save.h"
#include "gdk_game_save_blob.h"
#include "gdk_user.h"
#include "gdk_error.h"
#include "gdk_store.h"
#include "gdk_stringverify.h"

using namespace godot;

static GodotGDK* gdk = nullptr;
static GDKError* gdk_error = nullptr;

void initialize_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_CORE) {
		GDREGISTER_CLASS(GDKError);
		gdk_error = memnew(GDKError);
		Engine::get_singleton()->register_singleton("GDKError", gdk_error);
	}

	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(GDKXStoreProductKind);
	GDREGISTER_CLASS(GDKXStorePrice);
	GDREGISTER_CLASS(GDKXStoreCollectionData);
	GDREGISTER_CLASS(GDKXStoreSubscriptionInfo);
	GDREGISTER_CLASS(GDKXStoreImage);
	GDREGISTER_CLASS(GDKXStoreVideo);
	GDREGISTER_CLASS(GDKXStoreAvailability);
	GDREGISTER_CLASS(GDKXStoreSku);
	GDREGISTER_CLASS(GDKXStoreProduct);
	GDREGISTER_CLASS(GDKXStoreProductQueryHandle);
	GDREGISTER_CLASS(GDKXStoreCanLicenseStatus);
	GDREGISTER_CLASS(GDKXStoreLicenseHandle);
	GDREGISTER_CLASS(GDKStore);
	GDREGISTER_CLASS(GDKXblStringVerifyResult);
	GDREGISTER_CLASS(GDKStringVerify);
	GDREGISTER_CLASS(GDKAsyncBlock);
	GDREGISTER_CLASS(GDKAchievement);
	GDREGISTER_CLASS(GDKAchievements);
	GDREGISTER_CLASS(GDKAchievementsResultHandle);
	GDREGISTER_RUNTIME_CLASS(GDKPromise);
	GDREGISTER_CLASS(GDKGameSaveFiles);
	GDREGISTER_CLASS(GDKGameSave);
	GDREGISTER_CLASS(GDKGameSaveBlob);
	GDREGISTER_CLASS(GDKGameSaveBlobInfo);
	GDREGISTER_CLASS(GDKXUserAddOptions);
	GDREGISTER_CLASS(GDKXUserState);
	GDREGISTER_CLASS(GDKXUserChangeEvent);
	GDREGISTER_CLASS(GDKXUserGamertagComponent);
	GDREGISTER_CLASS(GDKXUserGamerPictureSize);
	GDREGISTER_CLASS(GDKXUserAgeGroup);
	GDREGISTER_CLASS(GDKXUserPrivilege);
	GDREGISTER_CLASS(GDKXUserPrivilegeOptions);
	GDREGISTER_CLASS(GDKXUserPrivilegeDenyReason);
	GDREGISTER_CLASS(GDKXUserGetTokenAndSignatureOptions);
	GDREGISTER_CLASS(GDKXUserDefaultAudioEndpointKind);
	GDREGISTER_CLASS(GDKXUserGetTokenAndSignatureData);
	GDREGISTER_CLASS(GDKXUserSignOutDeferralHandle);
	GDREGISTER_CLASS(GDKUser);
	GDREGISTER_CLASS(GodotGDK);

	gdk = GodotGDK::get_singleton();
	Engine::get_singleton()->register_singleton("GDK", gdk);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_CORE) {
		Engine::get_singleton()->unregister_singleton("GDKError");
		memdelete(gdk_error);
		gdk_error = nullptr;
	}

	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	Engine::get_singleton()->unregister_singleton("GDK");
	memdelete(gdk);
	gdk = nullptr;
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT init_gdk(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
