#ifndef UI_RENDERABLE_HPP
#define UI_RENDERABLE_HPP

#include <ftxui/dom/elements.hpp>

namespace ui {

// Renderable is an object that can be drawn on the screen by FTXUI on a canvas
class Renderable {
    public:
        // Returns an FTXUI element that can be drawn on the screen.
        virtual ftxui::Element GetUIElement() = 0;
};

}

#endif // UI_RENDERABLE_HPP