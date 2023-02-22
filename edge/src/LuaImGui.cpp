#include "edge/LuaImGui.h"

#include <sol/sol.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace
{
bool ImGui_Begin(const char* name)
{
    return ImGui::Begin(name);
}

void ImGui_End()
{
    ImGui::End();
}

std::pair<bool, bool> ImGui_Checkbox(const char* label, bool value)
{
    auto ret = ImGui::Checkbox(label, &value);
    return std::make_pair(ret, value);
}

std::pair<bool, float> ImGui_DragFloat(const char* label, float value)
{
    auto ret = ImGui::DragFloat(label, &value);
    return std::make_pair(ret, value);
}

bool ImGui_TreeNode(const char* label)
{
    return ImGui::TreeNode(label);
}

void ImGui_TreePop()
{
    ImGui::TreePop();
}

auto ImGui_InputInt(const char* label, int value, sol::this_state l)
{
    if (ImGui::InputInt(label, &value)) {
        return std::make_tuple(true, sol::make_object(l, value));
    }
    return std::make_tuple(false, sol::make_object(l, sol::nil));
}

auto ImGui_InputText(const char* label, const char* value)
{
    // This is not 100% efficient due to string copies...
    // However, let's hope that SSO saves us here and we
    // don't allocate too much in every frame.
    std::string str = value;
    if (ImGui::InputText(label, &str)) {
        return std::make_tuple(true, str);
    }
    return std::make_tuple(false, str);
}

bool ImGui_IsItemActivated()
{
    return ImGui::IsItemActivated();
}

bool ImGui_IsItemDeactivatedAfterEdit()
{
    return ImGui::IsItemDeactivatedAfterEdit();
}

bool ImGui_BeginCombo(const char* label, const char* defaultItem)
{
    return ImGui::BeginCombo(label, defaultItem);
}

bool ImGui_Selectable(const char* label, bool isSelected)
{
    return ImGui::Selectable(label, isSelected);
}

void ImGui_SetItemDefaultFocus()
{
    ImGui::SetItemDefaultFocus();
}

void ImGui_EndCombo()
{
    ImGui::EndCombo();
}

void ImGui_OpenPopup(const char* label)
{
    ImGui::OpenPopup(label);
}

bool ImGui_BeginPopup(const char* label)
{
    return ImGui::BeginPopup(label);
}

bool ImGui_BeginPopupModal(const char* label)
{
    return ImGui::BeginPopupModal(label, nullptr, ImGuiWindowFlags_AlwaysAutoResize);
}

void ImGui_EndPopup()
{
    return ImGui::EndPopup();
}

void ImGui_CloseCurrentPopup()
{
    return ImGui::CloseCurrentPopup();
}

bool ImGui_Button(const char* label)
{
    return ImGui::Button(label);
}

void ImGui_Text(const char* label)
{
    ImGui::Text("%s", label);
}

/* void ImGui_TextColored(Color color, const char* label)
{
    ImVec4 c(color.r, color.g, color.b, color.a);
    ImGui::TextColored(c, "%s", label);
}

void ImGui_PushTextColor(Color color)
{
    ImVec4 c(color.r, color.g, color.b, color.a);
    ImGui::PushStyleColor(ImGuiCol_Text, c);
} */

void ImGui_PopTextColor()
{
    ImGui::PopStyleColor();
}

void ImGui_PushInputErrorStyle()
{
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0.f, 0.f, 138));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(255, 0, 0, 102));
}

void ImGui_PopInputErrorStyle()
{
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

void ImGui_PushID(int id)
{
    ImGui::PushID(id);
}

void ImGui_PopID()
{
    ImGui::PopID();
}

void ImGui_Columns(int columnsCount)
{
    ImGui::Columns(columnsCount);
}

void ImGui_NextColumn()
{
    ImGui::NextColumn();
}

void ImGui_Separator()
{
    ImGui::Separator();
}

void ImGui_AlignTextToFramePadding()
{
    ImGui::AlignTextToFramePadding();
}

void ImGui_PushItemWidth(float w)
{
    ImGui::PushItemWidth(w);
}

void ImGui_PopItemWidth()
{
    ImGui::PopItemWidth();
}

void ImGui_SetNextItemWidth(float w)
{
    ImGui::SetNextItemWidth(w);
}

void ImGui_SetColumnWidth(int column_index, float width)
{
    ImGui::SetColumnWidth(column_index, width);
}

void ImGui_SameLine()
{
    ImGui::SameLine();
}

bool ImGui_ListBoxHeader(const char* label)
{
    return ImGui::ListBoxHeader(label);
}

void ImGui_ListBoxFooter()
{
    ImGui::ListBoxFooter();
}

void ImGui_PushDisabledButtonStyle()
{
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
}

void ImGui_PopStyleVar()
{
    ImGui::PopStyleVar();
}

} // end of anonymous namespace

namespace edge
{
namespace luab
{
    void registerImGuiFunctions(sol::state& lua)
    {
        auto ImGui = lua.create_named_table("im");

#define ADD_IMGUI_FUNCTION(x) ImGui.set_function(#x, ImGui_##x)
        ADD_IMGUI_FUNCTION(Begin);
        ADD_IMGUI_FUNCTION(End);
        ADD_IMGUI_FUNCTION(Checkbox);
        ADD_IMGUI_FUNCTION(DragFloat);
        ADD_IMGUI_FUNCTION(TreeNode);
        ADD_IMGUI_FUNCTION(TreePop);
        ADD_IMGUI_FUNCTION(InputInt);
        ADD_IMGUI_FUNCTION(InputText);
        ADD_IMGUI_FUNCTION(IsItemActivated);
        ADD_IMGUI_FUNCTION(IsItemDeactivatedAfterEdit);
        ADD_IMGUI_FUNCTION(BeginCombo);
        ADD_IMGUI_FUNCTION(Selectable);
        ADD_IMGUI_FUNCTION(EndCombo);
        ADD_IMGUI_FUNCTION(SetItemDefaultFocus);
        ADD_IMGUI_FUNCTION(OpenPopup);
        ADD_IMGUI_FUNCTION(BeginPopup);
        ADD_IMGUI_FUNCTION(BeginPopupModal);
        ADD_IMGUI_FUNCTION(EndPopup);
        ADD_IMGUI_FUNCTION(CloseCurrentPopup);
        ADD_IMGUI_FUNCTION(Button);
        ADD_IMGUI_FUNCTION(Text);
        // ADD_IMGUI_FUNCTION(TextColored);
        // ADD_IMGUI_FUNCTION(PushTextColor);
        ADD_IMGUI_FUNCTION(PopTextColor);
        ADD_IMGUI_FUNCTION(PushInputErrorStyle);
        ADD_IMGUI_FUNCTION(PopInputErrorStyle);
        ADD_IMGUI_FUNCTION(PushID);
        ADD_IMGUI_FUNCTION(PopID);
        ADD_IMGUI_FUNCTION(Columns);
        ADD_IMGUI_FUNCTION(NextColumn);
        ADD_IMGUI_FUNCTION(Separator);
        ADD_IMGUI_FUNCTION(AlignTextToFramePadding);
        ADD_IMGUI_FUNCTION(PushItemWidth);
        ADD_IMGUI_FUNCTION(PopItemWidth);
        ADD_IMGUI_FUNCTION(SetNextItemWidth);
        ADD_IMGUI_FUNCTION(SetColumnWidth);
        ADD_IMGUI_FUNCTION(SameLine);
        ADD_IMGUI_FUNCTION(ListBoxFooter);
        ADD_IMGUI_FUNCTION(ListBoxHeader);
        ADD_IMGUI_FUNCTION(PushDisabledButtonStyle);
        ADD_IMGUI_FUNCTION(PopStyleVar);
#undef ADD_IMGUI_FUNCTION

        /* clang-format off */
    lua.new_usertype<ImGuiTextFilter>("ImGuiTextFilter",
        "Draw", [](ImGuiTextFilter& filter, const char* label) { filter.Draw(label); },
        "PassFilter", [](ImGuiTextFilter& filter, const char* str) { return filter.PassFilter(str); },
        "Clear", &ImGuiTextFilter::Clear
    );
        /* clang-format on */
    }

} // end of namespace luab

} // end of namespace edge
