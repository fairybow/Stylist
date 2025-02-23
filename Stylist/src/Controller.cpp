/*
* Stylist: Controller.cpp  Copyright (C) 2024  fairybow
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
*
* This file uses Qt 6. Qt is a free and open-source widget toolkit for creating
* graphical user interfaces. For more information, visit <https://www.qt.io/>.
*
* Updated: 2025-02-22
*/

#include "Controller.h"
#include "ControllerPrivate.h"
#include "Debug.h"
#include "StylistPrivate.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

//------------------------------------------------------------
// Stylist::Controller definitions
//------------------------------------------------------------

// Try paint event for UI, wrt group count?
Controller::Controller(Stylist* stylist, QWidget* parent)
    : QWidget(parent)
    , m_private(std::make_unique<Private>(stylist, this))
{
    // I'm not actually sure if it matters whether I call this from
    // Controller::Private constructor or here
    m_private->initUi();
}

// std::unique_ptr doesn't like this being defaulted in header
Controller::~Controller() = default;

// A-Z
//------------------------------------------------------------

void Controller::dumpStylistGroupNames() const
{
    for (auto& group : m_private->stylistGroups())
        qDebug(D::log) << group.name();
}

//------------------------------------------------------------
// Stylist::Controller::Private definitions
//------------------------------------------------------------

Controller::Private::Private(Stylist* s, Controller* publicClass)
    : Coco::Private<Controller>(publicClass)
    , stylist(s)
{
}

// A-Z
//------------------------------------------------------------

// LOL shut up
void Controller::Private::createMultiGroupUi()
{
    auto pub = publicClass();
    auto stacked_widget = new QStackedWidget(pub);

    // For multiple groups, we need indexing, auto-switch, a top QGroupBox,
    // buttons...

    // Make a main "Groups" QGroupBox (for controller buttons)
    auto groups_buttons_gbox = new QGroupBox("Groups", pub);
    auto groups_buttons_gbox_layout = new QGridLayout(groups_buttons_gbox);
    groups_buttons_gbox->setCheckable(false);

    // Make a QButtonGroup to manage button states and store ID for comparison
    // to QStackWidget index
    auto button_group = new QButtonGroup(pub);
    button_group->setExclusive(true);

    for (auto& group : stylistGroups())
    {
        // Make a QGroupBox for each individual group's themes QComboBox
        auto gbox = new QGroupBox(group.name(), stacked_widget);
        auto gbox_layout = new QHBoxLayout(gbox);
        auto gbox_index = stacked_widget->addWidget(gbox);

        // Add a corresponding button
        auto button = new QPushButton(group.name(), groups_buttons_gbox);
        groups_buttons_gbox_layout->addWidget(button);
        button_group->addButton(button, gbox_index);
        button->setCheckable(true);
        button->setFocusPolicy(Qt::NoFocus);

        // Make a QComboBox and fill it with themes
        auto combo_box = new QComboBox(gbox);
        for (auto& theme : group.themes())
            combo_box->addItem(theme.name(), theme.toQVariant());

        // Set the QComboBox's current theme
        auto variant = group.currentTheme().toQVariant();
        auto current_theme_index = combo_box->findData(variant);
        combo_box->setCurrentIndex(current_theme_index);

        // Make a check box for Group's useTheme property
        auto check_box = new QCheckBox(gbox);
        check_box->setChecked(group.useTheme());

        gbox_layout->addWidget(check_box, 0);
        gbox_layout->addWidget(combo_box, 1);

        // Clicking the check box toggles whether or not the current group
        // uses a theme
        pub->connect
        (
            check_box,
            &QCheckBox::clicked,
            pub,
            [&](bool checked)
            {
                group.setUseTheme(checked);
            }
        );

        // This allows the combo box to change the theme
        pub->connect
        (
            combo_box,
            &QComboBox::currentIndexChanged,
            pub,
            [=, &group](int index)
            {
                (void)index;
                updateTheme(combo_box, group);
            }
        );

    } // for (auto& group : stylistGroups())

    auto main_layout = pub->layout();
    main_layout->addWidget(groups_buttons_gbox);
    main_layout->addWidget(stacked_widget);

    stacked_widget->setCurrentIndex(0);
    button_group->button(0)->setChecked(true);

    pub->connect
    (
        button_group,
        &QButtonGroup::idClicked,
        pub,
        [=](int id)
        {
            stacked_widget->setCurrentIndex(id);
        }
    );
}

void Controller::Private::createSingleGroupUi()
{
    auto pub = publicClass();
    auto& group = stylistGroups().first();

    // Make a QGroupBox for the group's themes QComboBox
    auto single_group_box = new QGroupBox(group.name(), pub);
    auto gbox_layout = new QHBoxLayout(single_group_box);

    // Make the QComboBox and fill it with themes
    auto combo_box = new QComboBox(single_group_box);
    for (auto& theme : group.themes())
        combo_box->addItem(theme.name(), theme.toQVariant());

    // Set the QComboBox's current theme
    auto variant = group.currentTheme().toQVariant();
    auto current_theme_index = combo_box->findData(variant);
    combo_box->setCurrentIndex(current_theme_index);

    // Make a check box for Group's useTheme property
    auto check_box = new QCheckBox(single_group_box);
    check_box->setChecked(group.useTheme());

    gbox_layout->addWidget(check_box, 0);
    gbox_layout->addWidget(combo_box, 1);

    // Clicking the check box toggles whether or not the current
    // group uses a theme
    pub->connect
    (
        check_box,
        &QCheckBox::clicked,
        pub,
        [&](bool checked)
        {
            group.setUseTheme(checked);
        }
    );

    // This allows the combo box to change the theme
    pub->connect
    (
        combo_box,
        &QComboBox::currentIndexChanged,
        pub,
        [=, &group](int index)
        {
            (void)index;
            updateTheme(combo_box, group);
        }
    );

    pub->layout()->addWidget(single_group_box);
}

void Controller::Private::initUi()
{
    auto pub = publicClass();

    pub->setLayout(new QVBoxLayout);

    // Good idea?
    pub->setMinimumWidth(250);
    pub->setMinimumHeight(150);

    // Could use this for refreshing UI on new group added
    auto group_count = stylistGroups().count();
    if (group_count > 1)
        createMultiGroupUi();
    else if (group_count == 1)
        createSingleGroupUi();
    else // No groups
        ;
}

// What is OOP
QList<WidgetGroup>& Controller::Private::stylistGroups() const noexcept
{
    return stylist->m_private->widgetGroups;
}

void Controller::Private::updateTheme(QComboBox* comboBox, WidgetGroup& group)
{
    // If we want to map later, make sure to map combo box to
    // group, not gbox index to group!
    auto variant = comboBox->currentData();
    auto theme = variant.value<Theme>();
    group.setCurrentTheme(theme);
}
