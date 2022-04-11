#include "DirectionalLight.h"

using namespace MelLib;
std::unordered_map<std::string, std::unique_ptr<DirectionalLight>>DirectionalLight::pLights;
DirectionalLight::UINT DirectionalLight::createCount = 0;
std::string DirectionalLight::mainLightName = "main";



void DirectionalLight::Create(const std::string& name)
{
	std::string key = name;
	if (key == "")key = "DirectionalLight_" + std::to_string(createCount);
	createCount++;

	pLights.emplace(key,std::make_unique<DirectionalLight>());

	if (mainLightName == "")mainLightName = key;

}

void DirectionalLight::Delete(const std::string& name)
{
	pLights.erase(name);
}

void DirectionalLight::SetDirection(const Vector3& vector)
{
	direction = Vector3::Normalize(vector);
}

std::vector<DirectionalLight*> DirectionalLight::GetAll()
{
	std::vector<DirectionalLight*>lights(pLights.size());
	int count = 0;
	for(auto& p : pLights)
	{
		lights[count] = p.second.get();
		count++;
	}
	return lights;
}
