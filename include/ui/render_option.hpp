#ifndef UI_RENDEROPTION_HPP
#define UI_RENDEROPTION_HPP

#include <ftxui/screen/color.hpp>
#include <ftxui/dom/elements.hpp>

namespace ui {

// Renderable is an object that can be drawn on the screen by FTXUI on a canvas
class RenderOption {
    public:
        // Creates a new RenderOption.
        RenderOption(
            char c = ' ',
            ftxui::Color fcolour = ftxui::Color::Default,
            ftxui::Color bcolour = ftxui::Color::Default,
            bool bold = false,
            bool italic = false,
            bool underline = false,
            bool blink = false
        ) : character(c),
            foregroundColour(fcolour),
            backgroundColour(bcolour),
            bold(bold),
            italic(italic),
            underline(underline),
            blink(blink) {};


        // Property getter and setter

        ftxui::Color GetForeground();
        void SetForeground(ftxui::Color fcolour);
        ftxui::Color GetBackground();
        void SetBackground(ftxui::Color bcolour);
        bool GetBold();
        void SetBold(bool bold);
        bool GetItalic();
        void SetItalic(bool italic);
        bool GetUnderline();
        void SetUnderline(bool underline);
        bool GetBlink();
        void SetBlink(bool blink);
        char GetChar();
        void SetChar(char character);

        // Returns an FTXUI element that can be drawn on the screen.
        ftxui::Element Render();

    private:
        ftxui::Color foregroundColour = ftxui::Color::Default;
        ftxui::Color backgroundColour = ftxui::Color::Default;
        bool bold = false;
        bool italic = false;
        bool underline = false;
        bool blink = false;
        char character = ' ';
};

}

#endif // UI_RENDEROPTION_HPP