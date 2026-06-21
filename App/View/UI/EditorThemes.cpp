#include "EditorThemes.h"
#include <ImGui/imgui.h>

static void UIsizes(ImGuiStyle& style) {
	style.Alpha                            =  1.f;
	style.DisabledAlpha                    =  0.6f;
	style.WindowPadding                    = ImVec2(4.f, 4.f);
	style.WindowRounding                   =  0.f;
	style.WindowBorderSize                 =  1.f;
	style.WindowMinSize                    = ImVec2(32.f, 32.f);
	style.WindowTitleAlign                 = ImVec2( 0.f,  0.5f);
	style.WindowMenuButtonPosition         = ImGuiDir_Left;
	style.ChildRounding                    =  0.f;
	style.ChildBorderSize                  =  1.f;
	style.PopupRounding                    =  0.f;
	style.PopupBorderSize                  =  1.f;
	style.FramePadding                     = ImVec2(4.f, 3.f);
	style.FrameRounding                    =  0.f;
	style.FrameBorderSize                  =  1.f;
	style.ItemSpacing                      = ImVec2(8.f, 4.f);
	style.ItemInnerSpacing                 = ImVec2(4.f, 4.f);
	style.CellPadding                      = ImVec2(5.f, 5.f);
	style.IndentSpacing                    = 21.f;
	style.ColumnsMinSpacing                =  6.f;
	style.ScrollbarSize                    = 15.f;
	style.ScrollbarRounding                =  9.f;
	style.GrabMinSize                      = 10.f;
	style.GrabRounding                     =  3.f;
	style.TabRounding                      =  0.f;
	style.TabBorderSize                    =  0.f;
	style.TabCloseButtonMinWidthSelected   =  0.f;
	style.TabCloseButtonMinWidthUnselected =  0.f;
	style.ColorButtonPosition              = ImGuiDir_Right;
	style.ButtonTextAlign                  = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign              = ImVec2(0.f,  0.f);
}

void Themes::classicValveTheme() {
	// Classic Steam style by metasprite from ImThemes
	ImGuiStyle& style                            = ImGui::GetStyle();

	UIsizes(style);
	style.GrabRounding                           = 0.f;
	style.ScrollbarRounding                      = 0.f;

	style.Colors[ImGuiCol_Text]                  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.49803922f, 0.49803922f, 0.49803922f, 1.0f);
	style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.28627452f, 0.3372549f, 0.25882354f, 1.0f);
	style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.28627452f, 0.3372549f, 0.25882354f, 1.0f);
	style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.23921569f, 0.26666668f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_Border]                = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.13725491f, 0.15686275f, 0.10980392f, 0.52f);
	style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.23921569f, 0.26666668f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.26666668f, 0.29803923f, 0.22745098f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.29803923f, 0.3372549f, 0.25882354f, 1.0f);
	style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.23921569f, 0.26666668f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.28627452f, 0.3372549f, 0.25882354f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.23921569f, 0.26666668f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.2784314f, 0.31764707f, 0.23921569f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.24705882f, 0.29803923f, 0.21960784f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.22745098f, 0.26666668f, 0.20784314f, 1.0f);
	style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 0.5f);
	style.Colors[ImGuiCol_Button]                = ImVec4(0.28627452f, 0.3372549f, 0.25882354f, 0.4f);
	style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 0.5f);
	style.Colors[ImGuiCol_Header]                = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 0.6f);
	style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 0.5f);
	style.Colors[ImGuiCol_Separator]             = ImVec4(0.13725491f, 0.15686275f, 0.10980392f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.1882353f, 0.22745098f, 0.1764706f, 0.0f);
	style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_Tab]                   = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.5372549f, 0.5686275f, 0.50980395f, 0.78f);
	style.Colors[ImGuiCol_TabActive]             = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.23921569f, 0.26666668f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.34901962f, 0.41960785f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.60784316f, 0.60784316f, 0.60784316f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(1.0f, 0.7764706f, 0.2784314f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.7294118f, 0.6666667f, 0.23921569f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.5882353f, 0.5372549f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}

void Themes::classicImGuiTheme() {
	ImGui::StyleColorsClassic();
	UIsizes(ImGui::GetStyle());
}

void Themes::darkImGuiTheme() {
	ImGui::StyleColorsDark();
	UIsizes(ImGui::GetStyle());
}

void Themes::lightImGuiTheme() {
	ImGui::StyleColorsLight();
	UIsizes(ImGui::GetStyle());
}

void Themes::draculaTheme() {
	// Darcula style by ice1000 from ImThemes
	ImGuiStyle& style                            = ImGui::GetStyle();

	UIsizes(style);
	style.WindowRounding                         = 5.3f;
	style.FrameRounding                          = 2.3f;
	style.TabRounding                            = 4.f;

	style.Colors[ImGuiCol_Text]                  = ImVec4(0.73333335f, 0.73333335f, 0.73333335f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.34509805f, 0.34509805f, 0.34509805f, 1.0f);
	style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.23529412f, 0.24705882f, 0.25490198f, 0.94f);
	style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.23529412f, 0.24705882f, 0.25490198f, 0.0f);
	style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.23529412f, 0.24705882f, 0.25490198f, 0.94f);
	style.Colors[ImGuiCol_Border]                = ImVec4(0.33333334f, 0.33333334f, 0.33333334f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.15686275f, 0.15686275f, 0.15686275f, 0.0f);
	style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.16862746f, 0.16862746f, 0.16862746f, 0.54f);
	style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.4509804f, 0.6745098f, 0.99607843f, 0.67f);
	style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.47058824f, 0.47058824f, 0.47058824f, 0.67f);
	style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.039215688f, 0.039215688f, 0.039215688f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
	style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.15686275f, 0.28627452f, 0.47843137f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.27058825f, 0.28627452f, 0.2901961f, 0.8f);
	style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.27058825f, 0.28627452f, 0.2901961f, 0.6f);
	style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.21960784f, 0.30980393f, 0.41960785f, 0.51f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.21960784f, 0.30980393f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.13725491f, 0.19215687f, 0.2627451f, 0.91f);
	style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.8980392f, 0.8980392f, 0.8980392f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.69803923f, 0.69803923f, 0.69803923f, 0.62f);
	style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.29803923f, 0.29803923f, 0.29803923f, 0.84f);
	style.Colors[ImGuiCol_Button]                = ImVec4(0.33333334f, 0.3529412f, 0.36078432f, 0.49f);
	style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.21960784f, 0.30980393f, 0.41960785f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.13725491f, 0.19215687f, 0.2627451f, 1.0f);
	style.Colors[ImGuiCol_Header]                = ImVec4(0.33333334f, 0.3529412f, 0.36078432f, 0.53f);
	style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.4509804f, 0.6745098f, 0.99607843f, 0.67f);
	style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.47058824f, 0.47058824f, 0.47058824f, 0.67f);
	style.Colors[ImGuiCol_Separator]             = ImVec4(0.3137255f, 0.3137255f, 0.3137255f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.3137255f, 0.3137255f, 0.3137255f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.3137255f, 0.3137255f, 0.3137255f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.0f, 1.0f, 1.0f, 0.85f);
	style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.0f, 1.0f, 1.0f, 0.6f);
	style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.0f, 1.0f, 1.0f, 0.9f);
	style.Colors[ImGuiCol_Tab]                   = ImVec4(0.1764706f, 0.34901962f, 0.5764706f, 0.862f);
	style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.8f);
	style.Colors[ImGuiCol_TabActive]             = ImVec4(0.19607843f, 0.40784314f, 0.6784314f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.06666667f, 0.101960786f, 0.14509805f, 0.9724f);
	style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13333334f, 0.25882354f, 0.42352942f, 1.0f);
	style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.60784316f, 0.60784316f, 0.60784316f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.0f, 0.42745098f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.8980392f, 0.69803923f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.18431373f, 0.39607844f, 0.7921569f, 0.9f);
	style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
	style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}

void Themes::discordTheme() {
	// Discord (Dark) style by BttrDrgn from ImThemes
	ImGuiStyle& style                            = ImGui::GetStyle();

	UIsizes(style);
	style.FrameBorderSize                        = 0.f;
	style.ScrollbarRounding                      = 0.f;
	style.GrabRounding                           = 0.f;

	style.Colors[ImGuiCol_Text]                  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.49803922f, 0.49803922f, 0.49803922f, 1.0f);
	style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.21176471f, 0.22352941f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.18431373f, 0.19215687f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.078431375f, 0.078431375f, 0.078431375f, 0.94f);
	style.Colors[ImGuiCol_Border]                = ImVec4(0.42745098f, 0.42745098f, 0.49803922f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.30980393f, 0.32941177f, 0.36078432f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.30980393f, 0.32941177f, 0.36078432f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.34509805f, 0.39607844f, 0.9490196f, 1.0f);
	style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.18431373f, 0.19215687f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.1254902f, 0.13333334f, 0.14509805f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.1254902f, 0.13333334f, 0.14509805f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.1254902f, 0.13333334f, 0.14509805f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.019607844f, 0.019607844f, 0.019607844f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.30980393f, 0.30980393f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40784314f, 0.40784314f, 0.40784314f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.50980395f, 0.50980395f, 0.50980395f, 1.0f);
	style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.23137255f, 0.64705884f, 0.3647059f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab]            = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Button]                = ImVec4(0.30980393f, 0.32941177f, 0.36078432f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.40784314f, 0.42745098f, 0.4509804f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.1254902f, 0.13333334f, 0.14509805f, 1.0f);
	style.Colors[ImGuiCol_Header]                = ImVec4(0.30980393f, 0.32941177f, 0.36078432f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.40784314f, 0.42745098f, 0.4509804f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.40784314f, 0.42745098f, 0.4509804f, 1.0f);
	style.Colors[ImGuiCol_Separator]             = ImVec4(0.42745098f, 0.42745098f, 0.49803922f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.09803922f, 0.4f, 0.7490196f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.09803922f, 0.4f, 0.7490196f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.2f);
	style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.95f);
	style.Colors[ImGuiCol_Tab]                   = ImVec4(0.18431373f, 0.19215687f, 0.21176471f, 1.0f);
	style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.23529412f, 0.24705882f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_TabActive]             = ImVec4(0.25882354f, 0.27450982f, 0.3019608f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.06666667f, 0.101960786f, 0.14509805f, 0.9724f);
	style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13333334f, 0.25882354f, 0.42352942f, 1.0f);
	style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.60784316f, 0.60784316f, 0.60784316f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.34509805f, 0.39607844f, 0.9490196f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.34509805f, 0.39607844f, 0.9490196f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.36078432f, 0.4f, 0.42745098f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.050980393f, 0.41960785f, 0.85882354f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.34509805f, 0.39607844f, 0.9490196f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
}

void Themes::enemyMouseTheme() {
	// Enemymouse style by enemymouse from ImThemes
	ImGuiStyle& style                            = ImGui::GetStyle();

	UIsizes(style);
	style.WindowRounding                         =  3.f;
	style.ChildRounding                          =  3.f;
	style.FrameRounding                          =  3.f;
	style.FrameBorderSize                        =  0.f;
	style.GrabMinSize                            = 20.f;
	style.GrabRounding                           =  1.f;
	style.TabRounding                            =  4.f;

	style.Colors[ImGuiCol_Text]                  = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.0f, 0.4f, 0.40784314f, 1.0f);
	style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.0f, 0.0f, 0.0f, 0.83f);
	style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.15686275f, 0.23921569f, 0.21960784f, 0.6f);
	style.Colors[ImGuiCol_Border]                = ImVec4(0.0f, 1.0f, 1.0f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.4392157f, 0.8f, 0.8f, 0.18f);
	style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.4392157f, 0.8f, 0.8f, 0.27f);
	style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.4392157f, 0.80784315f, 0.85882354f, 0.66f);
	style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.13725491f, 0.1764706f, 0.20784314f, 0.73f);
	style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.0f, 1.0f, 1.0f, 0.27f);
	style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.0f, 0.0f, 0.0f, 0.54f);
	style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.0f, 0.0f, 0.0f, 0.2f);
	style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.21960784f, 0.28627452f, 0.29803923f, 0.71f);
	style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.0f, 1.0f, 1.0f, 0.44f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.0f, 1.0f, 1.0f, 0.74f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.0f, 1.0f, 1.0f, 0.68f);
	style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.0f, 1.0f, 1.0f, 0.36f);
	style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.0f, 1.0f, 1.0f, 0.76f);
	style.Colors[ImGuiCol_Button]                = ImVec4(0.0f, 0.64705884f, 0.64705884f, 0.46f);
	style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.007843138f, 1.0f, 1.0f, 0.43f);
	style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.0f, 1.0f, 1.0f, 0.62f);
	style.Colors[ImGuiCol_Header]                = ImVec4(0.0f, 1.0f, 1.0f, 0.33f);
	style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.0f, 1.0f, 1.0f, 0.42f);
	style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.0f, 1.0f, 1.0f, 0.54f);
	style.Colors[ImGuiCol_Separator]             = ImVec4(0.0f, 0.49803922f, 0.49803922f, 0.33f);
	style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.0f, 0.49803922f, 0.49803922f, 0.47f);
	style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.0f, 0.69803923f, 0.69803923f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.0f, 1.0f, 1.0f, 0.54f);
	style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.0f, 1.0f, 1.0f, 0.74f);
	style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Tab]                   = ImVec4(0.1764706f, 0.34901962f, 0.5764706f, 0.862f);
	style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.8f);
	style.Colors[ImGuiCol_TabActive]             = ImVec4(0.19607843f, 0.40784314f, 0.6784314f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.06666667f, 0.101960786f, 0.14509805f, 0.9724f);
	style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.13333334f, 0.25882354f, 0.42352942f, 1.0f);
	style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.0f, 1.0f, 1.0f, 0.22f);
	style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
	style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.039215688f, 0.09803922f, 0.08627451f, 0.51f);
}

void Themes::greenTheme() {
	// Green Leaf style by Fizub from ImThemes
	ImGuiStyle& style                            = ImGui::GetStyle();

	UIsizes(style);
	style.WindowBorderSize                       =  0.f;
	style.ChildRounding                          =  4.2f;
	style.PopupRounding                          =  7.3f;
	style.FrameBorderSize                        =  0.f;
	style.ScrollbarSize                          = 10.2f;
	style.GrabRounding                           =  0.f;
	style.TabRounding                            =  4.f;

	style.Colors[ImGuiCol_Text]                  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.44313726f, 0.44313726f, 0.44313726f, 1.0f);
	style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.19607843f, 0.22745098f, 0.14117648f, 0.85f);
	style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.10980392f, 0.10980392f, 0.13725491f, 0.92f);
	style.Colors[ImGuiCol_Border]                = ImVec4(0.49803922f, 0.49803922f, 0.49803922f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.42745098f, 0.42745098f, 0.42745098f, 0.39f);
	style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.07450981f, 0.75686276f, 0.57254905f, 0.4f);
	style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.2784314f, 1.0f, 0.7490196f, 0.69f);
	style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.3882353f, 0.53333336f, 0.34117648f, 0.83f);
	style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.3019608f, 0.57254905f, 0.29803923f, 0.87f);
	style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.4392157f, 0.8f, 0.4f, 0.2f);
	style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.2901961f, 0.38039216f, 0.27450982f, 0.8f);
	style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.2f, 0.24705882f, 0.29803923f, 0.6f);
	style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.39215687f, 0.9137255f, 0.58431375f, 0.3f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.4f, 0.8f, 0.5803922f, 0.4f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.3882353f, 0.8f, 0.4117647f, 0.6f);
	style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.5137255f, 0.5882353f, 0.3372549f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.23921569f, 0.8784314f, 0.49411765f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.19607843f, 0.49019608f, 0.25882354f, 1.0f);
	style.Colors[ImGuiCol_Button]                = ImVec4(0.5254902f, 0.8039216f, 0.64705884f, 0.62f);
	style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.39607844f, 0.63529414f, 0.47843137f, 0.79f);
	style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.1764706f, 0.28627452f, 0.16862746f, 1.0f);
	style.Colors[ImGuiCol_Header]                = ImVec4(0.3882353f, 0.5019608f, 0.30980393f, 0.45f);
	style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.47058824f, 0.6392157f, 0.53333336f, 0.8f);
	style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.5686275f, 0.78431374f, 0.56078434f, 0.8f);
	style.Colors[ImGuiCol_Separator]             = ImVec4(0.49803922f, 0.49803922f, 0.49803922f, 0.6f);
	style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.20784314f, 0.32941177f, 0.27058825f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.29411766f, 0.4509804f, 0.3764706f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.0f, 1.0f, 1.0f, 0.1f);
	style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.41568628f, 0.99215686f, 0.58431375f, 0.6f);
	style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.35686275f, 0.5372549f, 0.43529412f, 0.9f);
	style.Colors[ImGuiCol_Tab]                   = ImVec4(0.23529412f, 0.34509805f, 0.26666668f, 0.786f);
	style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.10980392f, 0.23137255f, 0.12941177f, 0.8f);
	style.Colors[ImGuiCol_TabActive]             = ImVec4(0.5764706f, 1.0f, 0.5803922f, 0.38197422f);
	style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.22745098f, 0.40392157f, 0.22352941f, 0.8212f);
	style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.39607844f, 0.77254903f, 0.5921569f, 0.8372f);
	style.Colors[ImGuiCol_PlotLines]             = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.8980392f, 0.69803923f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.8980392f, 0.69803923f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.28235295f, 0.3764706f, 0.26666668f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.30980393f, 0.44705883f, 0.3254902f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.25882354f, 0.25882354f, 0.2784314f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0f, 1.0f, 1.0f, 0.07f);
	style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.2627451f, 0.63529414f, 0.3647059f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
	style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.36862746f, 0.5019608f, 0.24313726f, 0.8f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.2f, 0.2f, 0.2f, 0.35f);
}

void Themes::photoshopTheme() {
	// Photoshop style by Derydoca from ImThemes
	ImGuiStyle& style                            = ImGui::GetStyle();

	UIsizes(style);
	style.WindowRounding                         =  4.f;
	style.ChildRounding                          =  4.f;
	style.PopupRounding                          =  2.f;
	style.FrameRounding                          =  2.f;
	style.ScrollbarSize                          = 13.f;
	style.ScrollbarRounding                      = 12.f;
	style.GrabMinSize                            =  7.f;
	style.GrabRounding                           =  0.f;
	style.TabBorderSize                          =  1.f;

	style.Colors[ImGuiCol_Text]                  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.49803922f, 0.49803922f, 0.49803922f, 1.0f);
	style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.1764706f, 0.1764706f, 0.1764706f, 1.0f);
	style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.2784314f, 0.2784314f, 0.2784314f, 0.0f);
	style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.30980393f, 0.30980393f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_Border]                = ImVec4(0.2627451f, 0.2627451f, 0.2627451f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.15686275f, 0.15686275f, 0.15686275f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.2784314f, 0.2784314f, 0.2784314f, 1.0f);
	style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.14509805f, 0.14509805f, 0.14509805f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.14509805f, 0.14509805f, 0.14509805f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.14509805f, 0.14509805f, 0.14509805f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.19215687f, 0.19215687f, 0.19215687f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.15686275f, 0.15686275f, 0.15686275f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.27450982f, 0.27450982f, 0.27450982f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.29803923f, 0.29803923f, 0.29803923f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(1.0f, 0.3882353f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_CheckMark]             = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.3882353f, 0.3882353f, 0.3882353f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(1.0f, 0.3882353f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Button]                = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
	style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(1.0f, 1.0f, 1.0f, 0.156f);
	style.Colors[ImGuiCol_ButtonActive]          = ImVec4(1.0f, 1.0f, 1.0f, 0.391f);
	style.Colors[ImGuiCol_Header]                = ImVec4(0.30980393f, 0.30980393f, 0.30980393f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.46666667f, 0.46666667f, 0.46666667f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.46666667f, 0.46666667f, 0.46666667f, 1.0f);
	style.Colors[ImGuiCol_Separator]             = ImVec4(0.2627451f, 0.2627451f, 0.2627451f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.3882353f, 0.3882353f, 0.3882353f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(1.0f, 0.3882353f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.0f, 1.0f, 1.0f, 0.25f);
	style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.0f, 1.0f, 1.0f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.0f, 0.3882353f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Tab]                   = ImVec4(0.09411765f, 0.09411765f, 0.09411765f, 1.0f);
	style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.34901962f, 0.34901962f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TabActive]             = ImVec4(0.19215687f, 0.19215687f, 0.19215687f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.09411765f, 0.09411765f, 0.09411765f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.19215687f, 0.19215687f, 0.19215687f, 1.0f);
	style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.46666667f, 0.46666667f, 0.46666667f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.0f, 0.3882353f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.58431375f, 0.58431375f, 0.58431375f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.0f, 0.3882353f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(1.0f, 1.0f, 1.0f, 0.156f);
	style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(1.0f, 0.3882353f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight]          = ImVec4(1.0f, 0.3882353f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 0.3882353f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.0f, 0.0f, 0.0f, 0.586f);
	style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.0f, 0.0f, 0.0f, 0.586f);
}

void Themes::visualStudioTheme() {
	// Visual Studio style by MomoDeve from ImThemes
	ImGuiStyle& style                            = ImGui::GetStyle();

	UIsizes(style);
	style.FrameBorderSize                        = 0.f;
	style.ScrollbarRounding                      = 0.f;
	style.GrabRounding                           = 0.f;

	style.Colors[ImGuiCol_Text]                  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.5921569f, 0.5921569f, 0.5921569f, 1.0f);
	style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_Border]                = ImVec4(0.30588236f, 0.30588236f, 0.30588236f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.30588236f, 0.30588236f, 0.30588236f, 1.0f);
	style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.32156864f, 0.32156864f, 0.33333334f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.3529412f, 0.3529412f, 0.37254903f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.3529412f, 0.3529412f, 0.37254903f, 1.0f);
	style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_Button]                = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_Header]                = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_Separator]             = ImVec4(0.30588236f, 0.30588236f, 0.30588236f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.30588236f, 0.30588236f, 0.30588236f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.30588236f, 0.30588236f, 0.30588236f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.2f, 0.2f, 0.21568628f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.32156864f, 0.32156864f, 0.33333334f, 1.0f);
	style.Colors[ImGuiCol_Tab]                   = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_TabActive]             = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.11372549f, 0.5921569f, 0.9254902f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.1882353f, 0.1882353f, 0.2f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.30980393f, 0.30980393f, 0.34901962f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.22745098f, 0.22745098f, 0.24705882f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.0f, 0.46666667f, 0.78431374f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.14509805f, 0.14509805f, 0.14901961f, 1.0f);
}

void Themes::progtestTheme() {
	ImGuiStyle& style                            = ImGui::GetStyle();

	UIsizes(style);

	style.Colors[ImGuiCol_Text]                  = ImVec4(1e-6f, 9.99995e-7f, 9.9999e-7f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.23529412f, 0.23529412f, 0.23529412f, 0.76862746f);
	style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.50980395f, 0.50980395f, 0.4509804f, 1.0f);
	style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.6666667f, 0.6666667f, 0.6666667f, 1.0f);
	style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.50980395f, 0.50980395f, 0.4509804f, 1.0f);
	style.Colors[ImGuiCol_Border]                = ImVec4(0.8392157f, 0.827451f, 0.8f, 0.6509804f);
	style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.91764706f, 0.9098039f, 0.8784314f, 0.0f);
	style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.0f, 0.6f, 0.8f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.0f, 0.7027451f, 0.8784314f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.0f, 0.7204613f, 0.95686275f, 1.0f);
	style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(1.0f, 0.9764706f, 0.9490196f, 0.7490196f);
	style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.6666667f, 0.6666667f, 0.6666667f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(1.0f, 0.9764706f, 0.9490196f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.0f, 0.0f, 0.0f, 0.21f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.6666667f, 0.6666667f, 0.6666667f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.50980395f, 0.50980395f, 0.4509804f, 1.0f);
	style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.9999925f, 1.0f, 0.99999f, 0.8f);
	style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.0f, 0.0f, 0.0f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Button]                = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive]          = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Header]                = ImVec4(0.0f, 0.7529412f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.5019608f, 0.7529412f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.7529412f, 0.2509804f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Separator]             = ImVec4(0.0f, 0.0f, 0.0f, 0.32f);
	style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.0f, 1.0f, 0.0f, 0.78039217f);
	style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.0f, 0.0f, 0.0f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.0f, 1.0f, 0.0f, 0.78039217f);
	style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Tab]                   = ImVec4(0.0f, 0.6f, 0.8f, 1.0f);
	style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.0f, 0.6614071f, 0.8784314f, 1.0f);
	style.Colors[ImGuiCol_TabActive]             = ImVec4(0.0f, 0.7204613f, 0.95686275f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.0f, 0.6f, 0.8f, 0.46666667f);
	style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.0f, 0.6f, 0.8f, 0.6666667f);
	style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.4f, 0.3882353f, 0.3764706f, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.4f, 0.3882353f, 0.3764706f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.59607846f, 0.59607846f, 0.5372549f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.0f, 1.0f, 0.0f, 0.43137255f);
	style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
	style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
	style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
	style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.0f, 0.9764706f, 0.9490196f, 0.73f);
}
