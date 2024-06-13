#include <Windows.h>
#include <ui/widget.hpp>
#include <anticheat/CAnticheat.hpp>
#include <anticheat/CInternals.hpp>
#include <imgui.h>
struct AntiCheatData
{
    const char* module_name = "game.exe";
    std::uint32_t pid; // process pid
    structs::PSYSTEM_HANDLE_INFORMATION handleInfo;
    std::vector<structs::SYSTEM_HANDLE_TABLE_ENTRY_INFO> handles; // filtered handles
};

class rAntiCheatWidget : public rWidget
{
    AntiCheatData* s_Anti;
public:
    rAntiCheatWidget() {}
    void Init() override
    {
        s_Anti = new AntiCheatData();
        s_Anti->pid = Internal::GetCurrentPid().value_or(0);
        s_Anti->handleInfo = Internal::QueryHandleInformations().value();
    }

    void Render() override
    {
        QueryHandlesBtn();
        DrawHandleData();
    }

    void QueryHandlesBtn()
    {
        if (ImGui::Button("Query Handles"))
        {
            s_Anti->handleInfo = Internal::QueryHandleInformations().value();
            FilterHandles();
        }
    }

    void DrawHandleData()
    {
        ImGui::BeginTable("Handles", 2);
        ImGui::TableSetupColumn("Handle");
        ImGui::TableSetupColumn("Granted Access");
        ImGui::TableHeadersRow();
        for (auto& handle : s_Anti->handles)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Handle %d", handle.usHandleValue);
            ImGui::TableNextColumn();
            ImGui::Text("%d", handle.ulGrantedAcess);
        }
        ImGui::EndTable();
    };

    void FilterHandles()
    {
        s_Anti->handles.clear();
        for (int i = 0; i < s_Anti->handleInfo->ulHandleCount; i++)
        {
            structs::SYSTEM_HANDLE_TABLE_ENTRY_INFO handle = s_Anti->handleInfo->Handles[i];
            if (handle.usUniqueProcessId != s_Anti->pid)
                continue;
            s_Anti->handles.push_back(handle);
        }

    }
};

void CAnti::RegisterWidget()
{
    ui::RegisterWidget(new rAntiCheatWidget());
}