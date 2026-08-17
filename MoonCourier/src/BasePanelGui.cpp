#include "../include/BasePanelGui.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>

namespace
{
    constexpr float charge_price = 50.0f;

    constexpr float panel_width = 330.0f;
    constexpr float panel_height = 520.0f;
    constexpr float panel_margin = 10.0f;

    constexpr float rover_list_width = 300.0f;
    constexpr float rover_list_height = 420.0f;

    constexpr float rover_block_width = 290.0f;
    constexpr float rover_block_height = 145.0f;
    constexpr float rover_block_spacing = 5.0f;
}

BasePanelGui::BasePanelGui(
    tgui::Gui& gui,
    Base& base,
    unsigned int window_width,
    unsigned int window_height
)
    : base(base)
{
    (void)window_height;

    panel = tgui::Panel::create({
        panel_width,
        panel_height
        });

    panel->setPosition({
        static_cast<float>(window_width) -
            panel_width -
            panel_margin,

        panel_margin
        });

    panel->setVisible(false);

    panel->getRenderer()->setBackgroundColor(
        tgui::Color{ 24, 29, 38 }
    );

    // ---------------------------------------------------------
    // Money
    // ---------------------------------------------------------

    money_label = tgui::Label::create();

    money_label->setPosition({
        15,
        15
        });

    money_label->setTextSize(20);

    money_label->getRenderer()->setTextColor(
        tgui::Color::White
    );

    panel->add(money_label);

    // ---------------------------------------------------------
    // Rovers title
    // ---------------------------------------------------------

    auto rovers_title = tgui::Label::create(
        "Rovers"
    );

    rovers_title->setPosition({
        15,
        55
        });

    rovers_title->setTextSize(18);

    rovers_title->getRenderer()->setTextColor(
        tgui::Color::White
    );

    panel->add(rovers_title);

    // ---------------------------------------------------------
    // Rover list
    // ---------------------------------------------------------

    rover_list_panel = tgui::Panel::create({
        rover_list_width,
        rover_list_height
        });

    rover_list_panel->setPosition({
        15,
        85
        });

    rover_list_panel->getRenderer()->setBackgroundColor(
        tgui::Color{ 35, 42, 53 }
    );

    panel->add(rover_list_panel);

    // ---------------------------------------------------------
    // Add GUI
    // ---------------------------------------------------------

    gui.add(panel);

    // ---------------------------------------------------------
    // Select first rover
    // ---------------------------------------------------------

    if (!base.getRovers().empty())
    {
        base.selectRover(
            base.getRovers().front().getId()
        );
    }

    refresh();
}

void BasePanelGui::toggle()
{
    panel->setVisible(
        !panel->isVisible()
    );

    if (panel->isVisible())
    {
        refresh();
    }
}

bool BasePanelGui::isVisible() const
{
    return panel->isVisible();
}

void BasePanelGui::refresh()
{
    money_label->setText(
        "Money: " +
        std::to_string(
            static_cast<int>(
                base.getMoney()
                )
        )
    );

    rebuild_rover_list();
}

void BasePanelGui::rebuild_rover_list()
{
    rover_list_panel->removeAllWidgets();

    float current_y = 5.0f;

    for (const Rover& rover : base.getRovers())
    {
        const int rover_id = rover.getId();

        // -----------------------------------------------------
        // Rover block
        // -----------------------------------------------------

        auto rover_block = tgui::Panel::create({
            rover_block_width,
            rover_block_height
            });

        rover_block->setPosition({
            5.0f,
            current_y
            });

        rover_block->getRenderer()->setBackgroundColor(
            tgui::Color{ 45, 52, 65 }
        );

        // -----------------------------------------------------
        // Rover name
        // -----------------------------------------------------

        auto rover_name = tgui::Label::create(
            "Rover #" +
            std::to_string(
                rover_id
            )
        );

        rover_name->setPosition({
            10,
            8
            });

        rover_name->setTextSize(17);

        rover_name->getRenderer()->setTextColor(
            tgui::Color::White
        );

        rover_block->add(rover_name);

        // -----------------------------------------------------
        // Status
        // -----------------------------------------------------

        const bool is_active =
            rover.getActive();

        auto status_label = tgui::Label::create(
            is_active
            ? "ЗАНЯТ"
            : "СВОБОДЕН"
        );

        status_label->setPosition({
            185,
            9
            });

        status_label->setTextSize(14);

        status_label->getRenderer()->setTextColor(
            is_active
            ? tgui::Color{ 255, 180, 80 }
            : tgui::Color{ 100, 220, 120 }
        );

        rover_block->add(status_label);

        // -----------------------------------------------------
        // Battery text
        // -----------------------------------------------------

        const float battery_level =
            std::clamp(
                rover.getBatteryLevel(),
                0.0f,
                100.0f
            );

        auto battery_label = tgui::Label::create(
            "Заряд: " +
            std::to_string(
                static_cast<int>(
                    std::round(
                        battery_level
                    )
                    )
            ) +
            "%"
        );

        battery_label->setPosition({
            10,
            38
            });

        battery_label->setTextSize(14);

        battery_label->getRenderer()->setTextColor(
            tgui::Color::White
        );

        rover_block->add(battery_label);

        // -----------------------------------------------------
        // Battery progress bar
        // -----------------------------------------------------

        auto battery_progress =
            tgui::ProgressBar::create();

        battery_progress->setPosition({
            85,
            41
            });

        battery_progress->setSize({
            195,
            18
            });

        battery_progress->setMinimum(0);
        battery_progress->setMaximum(100);

        battery_progress->setValue(
            static_cast<unsigned int>(
                std::round(
                    battery_level
                )
                )
        );

        battery_progress
            ->getRenderer()
            ->setBackgroundColor(
                tgui::Color{ 55, 60, 70 }
            );

        battery_progress
            ->getRenderer()
            ->setFillColor(
                get_battery_color(
                    battery_level
                )
            );

        rover_block->add(
            battery_progress
        );

        // -----------------------------------------------------
        // Load
        // -----------------------------------------------------

        auto load_label = tgui::Label::create(
            "Вес груза: " +
            std::to_string(
                static_cast<int>(
                    rover.getLoad()
                    )
            )
        );

        load_label->setPosition({
            10,
            68
            });

        load_label->setTextSize(14);

        load_label->getRenderer()->setTextColor(
            tgui::Color::White
        );

        rover_block->add(load_label);

        // -----------------------------------------------------
        // Charge button
        // -----------------------------------------------------

        auto charge_button = tgui::Button::create(
            "Зарядить - 50"
        );

        charge_button->setPosition({
            10,
            98
            });

        charge_button->setSize({
            270,
            35
            });

        charge_button->setEnabled(
            battery_level < 100.0f &&
            base.getMoney() >= charge_price
        );

        charge_button->onPress(
            [this, rover_id]
            {
                charge_rover(
                    rover_id
                );
            }
        );

        rover_block->add(
            charge_button
        );

        // -----------------------------------------------------
        // Add block
        // -----------------------------------------------------

        rover_list_panel->add(
            rover_block
        );

        current_y +=
            rover_block_height +
            rover_block_spacing;
    }
}

void BasePanelGui::charge_rover(
    int rover_id
)
{
    base.selectRover(
        rover_id
    );

    base.chargeSelectedRover(
        charge_price
    );

    refresh();
}

tgui::Color BasePanelGui::get_battery_color(
    float battery_level
) const
{
    battery_level = std::clamp(
        battery_level,
        0.0f,
        100.0f
    );

    const uint8_t red =
        static_cast<uint8_t>(
            255.0f *
            (1.0f - battery_level / 100.0f)
            );

    const uint8_t green =
        static_cast<uint8_t>(
            255.0f *
            (battery_level / 100.0f)
            );

    return {
        red,
        green,
        35
    };
}