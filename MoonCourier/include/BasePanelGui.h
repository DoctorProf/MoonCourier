#pragma once

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/ProgressBar.hpp>

#include "models/Base.h"

class BasePanelGui
{
public:
    BasePanelGui(
        tgui::Gui& gui,
        Base& base,
        unsigned int window_width,
        unsigned int window_height
    );

    void toggle();
    void refresh();

    bool isVisible() const;

private:
    void rebuild_rover_list();
    void charge_rover(int rover_id);

    tgui::Color get_battery_color(
        float battery_level
    ) const;

    Base& base;

    tgui::Panel::Ptr panel;
    tgui::Label::Ptr money_label;
    tgui::Panel::Ptr rover_list_panel;
};