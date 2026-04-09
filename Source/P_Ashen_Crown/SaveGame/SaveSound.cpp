#include "SaveSound.h"

DEFINE_ENUM_TO_STRING(ESoundSettingType, "/Script/P_Ashen_Crown");

USaveSound::USaveSound()
{
	mVolumes.Init(1.f, StaticCast<int32>(ESoundSettingType::Count));
}

float USaveSound::GetVolume(ESoundSettingType SettingType) const
{
	return mVolumes[StaticCast<int32>(SettingType)];
}

const TArray<float>& USaveSound::GetVolumes() const
{
	return mVolumes;
}

void USaveSound::SetVolume(ESoundSettingType SettingType, float Volume)
{
	mVolumes[StaticCast<int32>(SettingType)] = Volume;
}

void USaveSound::SetVolumes(const TArray<float>& Volumes)
{
	mVolumes = Volumes;
}

USaveSound* USaveSound::Clone() const
{
	auto* CloneObj = NewObject<USaveSound>(GetOuter());
	CloneObj->SetVolumes(GetVolumes());
	return CloneObj;
}
