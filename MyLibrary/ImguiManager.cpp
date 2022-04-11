#include "ImguiManager.h"
#include"CreateBuffer.h"
#include"Library.h"

#include"Values.h"

#include"imgui/imgui_impl_dx12.h"
#include"imgui/imgui_impl_win32.h"

using namespace MelLib;

bool MelLib::ImguiManager::CheckReleaseDrawFlag()
{
    if (!releaseDrawFlag)
    {
#ifdef _DEBUG
        return false;
#else
        return true;

#endif // _DEBUG

    }
    return false;
}

ImguiManager* ImguiManager::GetInstance()
{
    static ImguiManager i;
    return &i;
}

bool ImguiManager::Initialize(ID3D12Device* pDev, ID3D12GraphicsCommandList* pCmdList)
{

    if (CheckReleaseDrawFlag())return true;

    pDevice = pDev;
    this->pCmdList = pCmdList;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;
    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    pDev->CreateDescriptorHeap(&desc,IID_PPV_ARGS(&desHeap));

    //imgui初期化
    if (!ImGui::CreateContext()) return false;
    if (!ImGui_ImplWin32_Init(Library::GetHWND()))return false;
    if (!ImGui_ImplDX12_Init
    (
        pDev,
        3,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        desHeap.Get(),
        desHeap->GetCPUDescriptorHandleForHeapStart(),
        desHeap->GetGPUDescriptorHandleForHeapStart()
    ))return false;

 

}

void MelLib::ImguiManager::Finalize()
{
    if (CheckReleaseDrawFlag())return;

    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX12_Shutdown();
}

void MelLib::ImguiManager::Begin()
{

    if (CheckReleaseDrawFlag())return;

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void MelLib::ImguiManager::Draw()
{

    if (CheckReleaseDrawFlag())return;

    //描画処理
    ImGui::Render();
    pCmdList->SetDescriptorHeaps(1, desHeap.GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), pCmdList);
}


void MelLib::ImguiManager::BeginDrawWindow
(
    const std::string& name
)
{
    if (CheckReleaseDrawFlag())return;

    ImGui::Begin(name.c_str());
    
  

}

void MelLib::ImguiManager::EndDrawWindow()
{
    if (CheckReleaseDrawFlag())return;

    ImGui::End();
}

void MelLib::ImguiManager::SetPosition(const Vector2& pos)
{
    if (CheckReleaseDrawFlag())return;

    ImGui::SetWindowPos(ImVec2(pos.x, pos.y), ImGuiCond_::ImGuiCond_FirstUseEver);
}

void MelLib::ImguiManager::SetSize(const Vector2& size)
{
    if (CheckReleaseDrawFlag())return;
    ImGui::SetWindowSize(ImVec2(size.x, size.y), ImGuiCond_::ImGuiCond_FirstUseEver);
}

void MelLib::ImguiManager::SameLine()
{
    if (CheckReleaseDrawFlag())return;
    ImGui::SameLine();
}


bool MelLib::ImguiManager::DrawRadioButton(const std::string& label, int& refInt, const int num)
{
    if (CheckReleaseDrawFlag())return false;

    return ImGui::RadioButton(label.c_str(), &refInt, num);
}

bool MelLib::ImguiManager::DrawCheckBox(const std::string& label, bool& refBool)
{
    if (CheckReleaseDrawFlag())return false;

    return ImGui::Checkbox(label.c_str(), &refBool);
}

bool MelLib::ImguiManager::DrawSliderInt(const std::string& label, int& refInt, const int numMin, const int numMax)
{
    if (CheckReleaseDrawFlag())return false;

    return ImGui::SliderInt(label.c_str(), &refInt, numMin, numMax);
}

bool MelLib::ImguiManager::DrawSliderInt2(const std::string& label, Value2<int>& refInt, const int numMin, const int numMax)
{
    if (CheckReleaseDrawFlag())return false;

    int i[2] = { refInt.v1,refInt.v2 };
    bool result = ImGui::SliderInt2(label.c_str(), i, numMin, numMax);
    
    refInt.v1 = i[0];
    refInt.v2 = i[1];
    
    return result;
}

bool MelLib::ImguiManager::DrawSliderInt3(const std::string& label, Value3<int>& refInt, const int numMin, const int numMax)
{
    if (CheckReleaseDrawFlag())return false;

    int i[3] = { refInt.v1,refInt.v2,refInt.v3 };
    bool result = ImGui::SliderInt3(label.c_str(), i, numMin, numMax);
   refInt.v1 = i[0];
   refInt.v2 = i[1];
   refInt.v3 = i[2];

    return result;
}

bool MelLib::ImguiManager::DrawSliderInt4(const std::string& label, Value4<int>& refInt, const int numMin, const int numMax)
{
    if (CheckReleaseDrawFlag())return false;

    int i[4] = { refInt.v1,refInt.v2,refInt.v3,refInt.v4 };
    bool result = ImGui::SliderInt4(label.c_str(), i, numMin, numMax);

    refInt.v1 = i[0];
    refInt.v2 = i[1];
    refInt.v3 = i[2];
    refInt.v4 = i[3];

    return result;
}


bool MelLib::ImguiManager::DrawSliderFloat(const std::string& label, float& refFloat, const float numMin, const float numMax)
{
    if (CheckReleaseDrawFlag())return false;

    return ImGui::SliderFloat(label.c_str(), &refFloat, numMin, numMax);
}

bool MelLib::ImguiManager::DrawSliderFloat2(const std::string& label, Value2<float>& refFloat, const float numMin, const float numMax)
{
    if (CheckReleaseDrawFlag())return false;

    float f[2] = { refFloat.v1,refFloat.v2 };
    bool result = ImGui::SliderFloat2(label.c_str(), f, numMin, numMax);

    refFloat.v1 = f[0];
    refFloat.v2 = f[1];

    return result;
}

bool MelLib::ImguiManager::DrawSliderFloat3(const std::string& label, Value3<float>& refFloat, const float numMin, const float numMax)
{
    if (CheckReleaseDrawFlag())return false;

    float f[3] = { refFloat.v1, refFloat.v2 , refFloat.v3 };
    bool result = ImGui::SliderFloat3(label.c_str(), f, numMin, numMax);
    refFloat.v1 = f[0];
    refFloat.v2 = f[1];
    refFloat.v3 = f[2];

    return result;
}

bool MelLib::ImguiManager::DrawSliderFloat4(const std::string& label, Value4<float>& refFloat, const float numMin, const float numMax)
{
    if (CheckReleaseDrawFlag())return false;

    float f[4] = { refFloat.v1,refFloat.v2 ,refFloat.v3 ,refFloat.v4 };
    bool result = ImGui::SliderFloat4(label.c_str(), f, numMin, numMax);

    refFloat.v1 = f[0];
    refFloat.v2 = f[1];
    refFloat.v3 = f[2];
    refFloat.v4 = f[3];

    return result;
}

bool MelLib::ImguiManager::DrawSliderVector2(const std::string& label, Vector2& refVec2, const float numMin, const float numMax)
{
    if (CheckReleaseDrawFlag())return false;

    float f[2] = { refVec2.x,refVec2.y };
    bool result = ImGui::SliderFloat2(label.c_str(), f, numMin, numMax);

   refVec2.x = f[0];
   refVec2.y = f[1];

    return result;
}

bool MelLib::ImguiManager::DrawSliderVector3(const std::string& label, Vector3& refVec3, const float numMin, const float numMax)
{
    if (CheckReleaseDrawFlag())return false;

    float f[3] = { refVec3.x,refVec3.y ,refVec3.z };
    bool result = ImGui::SliderFloat3(label.c_str(), f, numMin, numMax);

    refVec3.x = f[0];
    refVec3.y = f[1];
    refVec3.z = f[2];

    return result;
}


bool MelLib::ImguiManager::DrawColorPicker(const std::string& label, Color& refColor, const ImGuiColorEditFlags flag)
{
    if (CheckReleaseDrawFlag())return false;

    Value4<float>fCor4 = refColor.ToFloat();
    float color[4] = { fCor4.v1,fCor4.v2,fCor4.v3,fCor4.v4 };
    bool result = ImGui::ColorPicker4(label.c_str(), color, flag);
    refColor = Color(color[0] * 255.0f, color[1] * 255.0f, color[2] * 255.0f, color[3] * 255.0f);
    return result;
}

bool MelLib::ImguiManager::DrawInputText(const std::string& label, std::string& text, const size_t maxChar,const ImGuiInputTextFlags flag)
{
    // ウィジェットです。キーボードによる入力
    // - InputText()をstd::stringやカスタムの動的文字列型で使用したい場合は、misc/cpp/imgui_stdlib.hやimgui_demo.cppのコメントを参照してください。
    // - ImGuiInputTextFlagsフラグのほとんどはInputText()にのみ有効で、InputFloatX, InputIntX, InputDoubleなどには使えません。

    //if (CheckReleaseDrawFlag())return false;
   
    //size_t size = text.size();
    //char* str = new char[size + 1];
    //for (int i = 0; i < size; i++)
    //{
    //    str[i] = text[i];
    //}
    //str[size] = '\0';

    //bool result = ImGui::InputText(label.c_str(), str, maxChar,flag);
    //text = str;
    //delete[] str;
    //return result;


    return false;
}
