#pragma once

#include <vector>

#include <hex/ui/banner.hpp>
#include <hex/helpers/utils.hpp>

#include <imgui.h>
#include <hex/ui/imgui_imhex_extensions.h>

namespace hex::ui {

    class MultiBannerButton : public Banner<MultiBannerButton> {
    public:
        MultiBannerButton(const char *icon, UnlocalizedString message, ImColor color, std::vector<UnlocalizedString> buttonTexts, std::vector<std::function<void()>> buttonCallbacks, std::function<void()> closeCallback = []{})
            : Banner(color), m_icon(icon), m_message(std::move(message)), m_buttonTexts(std::move(buttonTexts)), m_buttonCallbacks(std::move(buttonCallbacks)), m_closeCallback(std::move(closeCallback)) { }

        void drawContent() override {
            std::vector<std::string> buttonTexts = {};
            float buttonSize = {};
            for (auto index = 0u; index < m_buttonTexts.size(); ++index) {
                const std::string buttonText = fmt::format(" {} ", Lang(m_buttonTexts[index]).get());
                buttonSize += ImGui::CalcTextSize(buttonText.c_str()).x + 20_scaled;
                buttonTexts.emplace_back(buttonText);
            }

            const auto iconSize = ImGui::CalcTextSize(m_icon);
            const auto textHeight = std::max(ImGui::CalcTextSize(Lang(m_message)).y, iconSize.y);
            const auto textOffset = (ImGui::GetWindowHeight() - textHeight) / 2;

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + textOffset);
            ImGui::TextUnformatted(m_icon);
            ImGui::SameLine(0, 10_scaled);

            const std::string message = Lang(m_message);
            const auto messageSize = ImGui::CalcTextSize(message.c_str());
            ImGuiExt::TextFormatted("{}", limitStringLength(message, message.size() * ((ImGui::GetContentRegionAvail().x - buttonSize - 40_scaled) / messageSize.x)));
            if (ImGui::IsItemHovered()) {
                ImGui::SetNextWindowSize(ImVec2(400_scaled, 0));
                if (ImGui::BeginTooltip()) {
                    ImGui::PushStyleColor(ImGuiCol_Text, getColor().Value);
                    ImGuiExt::TextFormattedWrapped("{}", message);
                    ImGui::PopStyleColor();
                    ImGui::EndTooltip();
                }
            }

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2_scaled);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1_scaled);
            ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, 0.0F);
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(ImGuiCol_Button, 0.9F));

            for (auto index = 0u; index < buttonTexts.size(); ++index) {
                ImGui::SameLine();

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - buttonSize);
                if (ImGui::Button(buttonTexts[index].c_str())) {
                    m_buttonCallbacks[index]();
                    this->close();
                }
                buttonSize -= ImGui::CalcTextSize(buttonTexts[index].c_str()).x + 20_scaled;
            }
            ImGui::PopStyleColor();
            ImGui::PopStyleVar(3);
        }

        void onClose() override {
            m_closeCallback();
        }

    private:
        const char *m_icon;
        UnlocalizedString m_message;
        std::vector<UnlocalizedString> m_buttonTexts;
        std::vector<std::function<void()>> m_buttonCallbacks;
        std::function<void()> m_closeCallback;
    };

}
