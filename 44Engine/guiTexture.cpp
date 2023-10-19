#include "guiTexture.h"
#include "yaTexture.h"
#include "StrConverter.h"

namespace gui
{
	Texture::Texture()
	{

	}

	Texture::~Texture()
	{

	}

	void Texture::Update()
	{
		ya::graphics::Texture* targetTex 
			= dynamic_cast<ya::graphics::Texture*>(GetTarget());

		if (targetTex == nullptr)
			return;

		ImGui::Image(targetTex->GetSRV().Get(), ImVec2(150.0f, 150.0f));

		std::string key = StrConverter::ConvertUnicodeToUTF8(targetTex->GetKey());
		std::string path = StrConverter::ConvertUnicodeToUTF8(targetTex->GetPath());

		ImGui::Text("Key	"); ImGui::SameLine();
		ImGui::InputText("##TexKey"
			, (char*)key.data(), key.size(), ImGuiInputTextFlags_ReadOnly);

		ImGui::Text("Path	"); ImGui::SameLine();
		ImGui::InputText("##TexPath"
			, (char*)path.data(), path.size(), ImGuiInputTextFlags_ReadOnly);

		int width = (int)targetTex->GetWidth();
		int height = (int)targetTex->GetHeight();


		char wBuffer[256] = "";
		_itoa_s(width, wBuffer, 10);

		ImGui::Text("Width	"); ImGui::SameLine();
		ImGui::InputText("##TexWidth"
			, wBuffer, 256, ImGuiInputTextFlags_ReadOnly);

		char hBuffer[256] = "";
		_itoa_s(height, hBuffer, 10);

		ImGui::Text("Height	"); ImGui::SameLine();
		ImGui::InputText("##TexHeight"
			, hBuffer, 256, ImGuiInputTextFlags_ReadOnly);
	}
}
