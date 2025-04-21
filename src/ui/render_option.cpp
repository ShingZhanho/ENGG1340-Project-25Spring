#include <ui/render_option.hpp>
#include <util/log.hpp>

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
    char RenderOption::GetChar() { return character; }
    void RenderOption::SetChar(char c) { character = c; }
    ftxui::Element RenderOption::Render() {
        ftxui::Element ele = ftxui::text(std::string(1, character))
                                | ftxui::color(foregroundColour)
                                | ftxui::bgcolor(backgroundColour);
        if (bold) ele |= ftxui::bold;
        if (italic) ele |= ftxui::italic;
        if (underline) ele |= ftxui::underlined;
        if (blink) ele |= ftxui::blink;
        ele |= ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 1);
        ele |= ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 1);
        return ele;
    }
}