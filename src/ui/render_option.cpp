#include <ui/render_option.hpp>

namespace ui {
    ftxui::Color RenderOption::GetForeground() { return foregroundColour; }
    void RenderOption::SetForeground(ftxui::Color fcolour) { foregroundColour = fcolour; }
    ftxui::Color RenderOption::GetBackground() { return backgroundColour; }
    void RenderOption::SetBackground(ftxui::Color bcolour) { backgroundColour = bcolour; }
    bool RenderOption::GetBold() { return bold; }
    void RenderOption::SetBold(bool b) { bold = b; }
    bool RenderOption::GetItalic() { return italic; }
    void RenderOption::SetItalic(bool b) { italic = b; }
    bool RenderOption::GetUnderline() { return underline; }
    void RenderOption::SetUnderline(bool b) { underline = b; }
    bool RenderOption::GetBlink() { return blink; }
    void RenderOption::SetBlink(bool b) { blink = b; }
    ftxui::Element RenderOption::Render() {
        ftxui::Element ele = ftxui::text(std::to_string(character))
                                | ftxui::color(foregroundColour)
                                | ftxui::bgcolor(backgroundColour);
        if (bold) ele |= ftxui::bold;
        if (italic) ele |= ftxui::italic;
        if (underline) ele |= ftxui::underlined;
        if (blink) ele |= ftxui::blink;
        return ele;
    }
}