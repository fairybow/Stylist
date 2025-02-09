/*
* Stylist: Stylist.cpp  Copyright (C) 2024  fairybow
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
*
* This file uses Qt 6. Qt is a free and open-source widget toolkit for creating
* graphical user interfaces. For more information, visit <https://www.qt.io/>.
*
* Updated: 2025-2-2
*/

#include "Coco/Io.h"
#include "Coco/Layout.h"
#include "Controller.h"
#include "Debug.h"
#include "Stylist.h"
#include "StylistPrivate.h"
#include "Utility.h"
#include "WidgetGroup.h"

#include <QDialog>
#include <QMetaObject>
#include <QPushButton>

//------------------------------------------------------------
// Stylist definitions
//------------------------------------------------------------

Stylist::Stylist(QObject* parent)
    : QObject(parent)
    , m_private(std::make_unique<Private>(this))
{
    if (parent)
        parent->installEventFilter(this);
}

// std::unique_ptr doesn't like this being defaulted in header
Stylist::~Stylist() = default;

// A-Z
//------------------------------------------------------------

void Stylist::addThemesDirectory(const std::filesystem::path& directory)
{
    auto& themes_dirs = m_private->themesDirectories;
    if (themes_dirs.contains({ directory })) return;

    themes_dirs << directory;

    // Currently, calling setThemesDirectories will cause the Group to clear all
    // current themes and rebuild from the new directories
    for (auto& group : m_private->widgetGroups)
        group.setThemesDirectories(themes_dirs);
}

void Stylist::define
(
    std::type_index qWidgetType,
    const QString& name,
    const std::filesystem::path& themesTemplatePath,
    const QString& themesFileExtension,
    const std::filesystem::path& baseQssPath,
    int role
)
{
    if (auto group = m_private->findOrMake({ qWidgetType, role }))
    {
        group->setName(name);
        group->setThemesTemplate(Coco::Io::readTxt(themesTemplatePath));
        group->setThemesExtension(themesFileExtension);

        if (Coco::Path::isValid(baseQssPath))
            group->setBaseQss(Coco::Io::readTxt(baseQssPath));
    }
}

void Stylist::define
(
    std::type_index qWidgetType,
    const QString& name,
    const std::filesystem::path& themesTemplatePath,
    const QString& themesFileExtension,
    int role
)
{
    define
    (
        qWidgetType,
        name,
        themesTemplatePath,
        themesFileExtension,
        {},
        role
    );
}

void Stylist::dump() const
{
    printThemesDirectories();
    dumpGroupsInfo();
    dumpTemplates();
}

void Stylist::dumpGroupsInfo() const
{
    constexpr auto format =         \
        "\n\tGroup %1 info:"        \
        "\n\t\tName: %2"            \
        "\n\t\tType name: %3"       \
        "\n\t\tKey: %4"             \
        "\n\t\tTheme file extension: %5\n";

    for (auto i = 0; i < m_private->widgetGroups.count(); ++i)
    {
        auto& group = m_private->widgetGroups.at(i);
        auto group_id = group.id();

        qDebug(D::log) << dUtf8Format
        (
            format,
            i + 1,
            group.name(),
            group_id.type.name(),
            group_id.role,
            group.themesExtension()
        );
    }
}

void Stylist::dumpTemplates() const
{
    constexpr auto format = \
        "\n\tGroup %1 template:\n```\n%2\n```\n";

    for (auto i = 0; i < m_private->widgetGroups.count(); ++i)
    {
        auto& group = m_private->widgetGroups.at(i);

        qDebug(D::log) << dUtf8Format
        (
            format,
            i + 1,
            group.themesTemplate()
        );
    }
}

// If we are not modifying or consuming the event, we should always return the
// default filter result
bool Stylist::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::Close)
        m_private->handleCloseEvent(watched);

    return QObject::eventFilter(watched, event);
}

void Stylist::printGroupInfo(std::type_index qWidgetType, int role) const
{
    if (auto group = m_private->find({ qWidgetType, role }))
    {
        auto group_id = group->id();

        constexpr auto format =         \
            "\n\tGroup \"%1\" info:"    \
            "\n\t\tType name: %2"       \
            "\n\t\tKey: %3"             \
            "\n\t\tTheme file extension: %4\n";

        qDebug(D::log) << dUtf8Format
        (
            format,
            group->name(),
            group_id.type.name(),
            group_id.role,
            group->themesExtension()
        );
    }
}

void Stylist::printTemplate(std::type_index qWidgetType, int role) const
{
    constexpr auto format = "\n\tGroup \"%1\" template:\n```\n%2\n```\n";
    if (auto group = m_private->find({ qWidgetType, role }))
    {
        qDebug(D::log) << dUtf8Format
        (
            format,
            group->name(),
            group->themesTemplate()
        );
    }
}

void Stylist::printThemesDirectories() const
{
    qDebug(D::log)
        << "Themes directories:"
        << m_private->themesDirectories;
}

void Stylist::sayHello() const
{
    qDebug(D::log) << "Hello!";
}

// Test paths with special characters (e.g. e-acute)
// Adjust to avoid creating group with invalid path!
void Stylist::setBaseQss
(
    std::type_index qWidgetType,
    const std::filesystem::path& path,
    int role
)
{
    if (auto group = m_private->findOrMake({ qWidgetType, role }))
        group->setBaseQss(Coco::Io::readTxt(path));
}

void Stylist::setCurrentTheme
(
    std::type_index qWidgetType,
    const std::filesystem::path& themePath,
    int role
)
{
    if (auto group = m_private->find({ qWidgetType, role }))
        group->setCurrentTheme(themePath);
}

void Stylist::setName
(
    std::type_index qWidgetType,
    const QString& name,
    int role
)
{
    if (auto group = m_private->findOrMake({ qWidgetType, role }))
        group->setName(name);
}

void Stylist::setThemesExtension
(
    std::type_index qWidgetType,
    const QString& fileExtension,
    int role
)
{
    if (auto group = m_private->findOrMake({ qWidgetType, role }))
        group->setThemesExtension(fileExtension);
}

// Adjust to avoid creating group with invalid path!
void Stylist::setThemesTemplate
(
    std::type_index qWidgetType,
    const std::filesystem::path& path,
    int role
)
{
    if (auto group = m_private->findOrMake({ qWidgetType, role }))
        group->setThemesTemplate(Coco::Io::readTxt(path));
}

void Stylist::setUseTheme
(
    std::type_index qWidgetType,
    bool useTheme,
    int role
)
{
    if (auto group = m_private->find({ qWidgetType, role }))
        group->setUseTheme(useTheme);
}

void Stylist::showDialog()
{
    m_private->showOwnDialog();
}

void Stylist::yoke(QWidget* widget, int role)
{
    // Bad calls
    if (!widget || role < 0) return;

    // Perform checks/operations for already grouped widgets
    for (auto& group : m_private->widgetGroups)
    {
        if (!group.contains(widget)) continue;
        // A WidgetGroup already contains widget:

        // Trying to yoke the same widget & role, so do nothing
        if (group.id().role == role) return;

        // Unassociate yoked widget and will yoke again with new role
        group.remove(widget);
    }

    // This search will create the WidgetGroup if not found
    if (auto group = m_private->findOrMake({ typeid(*widget), role }))
        group->addWidget(widget);
}

//------------------------------------------------------------
// Stylist::Private definitions
//------------------------------------------------------------

// A-Z
//------------------------------------------------------------

WidgetGroup* Stylist::Private::find(const WidgetGroup::Id& id) const
{
    return Utility::findIf
    (
        widgetGroups,
        [&](const WidgetGroup& group)
        {
            return group.id() == id;
        }
    );
}

WidgetGroup* Stylist::Private::find(QWidget* const& widget) const
{
    return Utility::findIf
    (
        widgetGroups,
        [&](const WidgetGroup& group)
        {
            return group.contains(widget);
        }
    );
}

WidgetGroup* Stylist::Private::find(const QPointer<QWidget>& widget) const
{
    return find(widget.get());
}

WidgetGroup* Stylist::Private::findOrMake(const WidgetGroup::Id& id)
{
    auto group = find(id);

    if (!group)
        group = make(id);

    return group;
}

void Stylist::Private::handleCloseEvent(QObject* watched)
{
    // We watched for the parent's close event to close our modeless dialog, if
    // open
    if (ownDialog && publicClass()->parent() == watched)
        ownDialog->close();
}

WidgetGroup* Stylist::Private::make(const WidgetGroup::Id& id)
{
    widgetGroups << WidgetGroup(id, themesDirectories);
    return &widgetGroups.last();
}

QDialog* Stylist::Private::newDialog()
{
    auto dialog = new QDialog{};
    auto themes_widget = new Controller(publicClass(), dialog);
    auto button = new QPushButton("OK", dialog);
    auto main_layout = new QVBoxLayout(dialog);
    auto button_layout = new QVBoxLayout{};

    main_layout->addWidget(themes_widget);
    main_layout->addLayout(button_layout);
    button_layout->addWidget(button, 0, Qt::AlignCenter);
    button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // setFocus invoked when control returns to the event loop, which
    // effectively means focus will be set right after the dialog is shown
    QMetaObject::invokeMethod(button, "setFocus", Qt::QueuedConnection);

    QObject::connect
    (
        button,
        &QPushButton::clicked,
        dialog,
        &QDialog::accept
    );

    return dialog;
}

void Stylist::Private::showOwnDialog()
{
    if (ownDialog)
    {
        ownDialog->activateWindow();
        return;
    }

    auto pub = publicClass();
    ownDialog = newDialog();
    ownDialog->setModal(false);

    pub->connect
    (
        ownDialog,
        &QDialog::finished,
        pub,
        [&](int result)
        {
            (void)result;

            if (ownDialog)
                ownDialog->deleteLater();
        }
    );

    // QDialog exec() should be avoided
    ownDialog->open();
}
