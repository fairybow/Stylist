/*
* Stylist: WidgetGroup.cpp  Copyright (C) 2024  fairybow
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
*
* This file uses Qt 6. Qt is a free and open-source widget toolkit for creating
* graphical user interfaces. For more information, visit <https://www.qt.io/>.
*
* Updated: 2024-11-30
*/

/// SUPER NEED TO FIGURE OUT THE APPROPRIATE CHECKS AND WHEN TO MAKE THEM,
/// AS IN, CHACHED THEME, TEMPLATE EMPTY OR NULL OR WHAT

#include "Coco/Io.h"
#include "Debug.h"
#include "Theme.h"
#include "Utility.h"
#include "WidgetGroup.h"

//------------------------------------------------------------
// WidgetGroup::Id definitions
//------------------------------------------------------------

bool WidgetGroup::Id::operator==(const Id& other) const = default;
bool WidgetGroup::Id::operator!=(const Id& other) const = default;

//------------------------------------------------------------
// WidgetGroup definitions
//------------------------------------------------------------

WidgetGroup::WidgetGroup() = default;

WidgetGroup::WidgetGroup
(
    const Id& id,
    const QList<Coco::Path>& themesDirectories
)
    : m_id(id)
    , m_themesDirectories(themesDirectories)
{
}

bool WidgetGroup::operator==(const WidgetGroup& other) const noexcept
{
    return m_id == other.m_id;
}

bool WidgetGroup::operator!=(const WidgetGroup& other) const noexcept = default;

// A-Z
//------------------------------------------------------------

// What checks should this run?
bool WidgetGroup::_cacheTheme(const Theme& theme)
{
    // Should we cache it if empty? I suppose so, if we allow editable themes
    // one day...

    if (theme.isNull())
    {
        qDebug(D::log)
            << __FUNCTION__
            << "Theme parameter null";

        return false;
    }

    // Nor if we don't know this theme
    if (!m_themes.contains(theme))
    {
        qDebug(D::log)
            << __FUNCTION__
            << "Theme parameter not contained in Group themes";

        return false;
    }

    // This is now our "current theme"
    // Currently, if the theme is invalid or can't compile, so be it
    m_cache.theme = theme;
    return true;
}

/// UPDATE this to say the function ran but whether or not it actually cleaned anything
void WidgetGroup::_clean()
{
    auto it = std::remove_if
    (
        m_widgets.begin(),
        m_widgets.end(),
        [](const QPointer<QWidget>& widget)
        {
            return widget.isNull();
        }
    );

    m_widgets.erase(it, m_widgets.end());

    qDebug(D::log)
        << "WidgetGroup"
        << this
        << "cleaned!";
}

// What checks should this run?
bool WidgetGroup::_compile()
{
    // We can't compile without a template
    if (m_themesTemplate.isEmpty())
    {
        qDebug(D::log)
            << __FUNCTION__
            << "Theme template empty";

        return false;
    }

    // Nor if cached theme is null
    // If theme is not empty, then it both exists and has content
    if (m_cache.theme.isEmpty())
    {
        qDebug(D::log)
            << __FUNCTION__
            << "Cached theme is null";

        return false;
    }

    // Alter theme.compileQss to return a result pack of string and success? How
    // to tell if didn't compile? Return empty if contains sigil?
    auto compiled = m_cache.theme.compileQss(m_themesTemplate);
    m_cache.compiledQss = compiled;

    constexpr auto format = "\n\tCompiled to: \n```\n%1\n```\n";
    qDebug(D::log) << dUtf8Format(format, m_cache.compiledQss);
    return true;
}

Theme* WidgetGroup::_find(const Coco::Path& path) const
{
    return Utility::findIf
    (
        m_themes,
        [&](const Theme& theme)
        {
            return theme.path() == path;
        }
    );
}

void WidgetGroup::_resetThemes()
{
    qDebug(D::log) << __FUNCTION__;

    m_themes.clear();

    auto theme_paths = Coco::Path::findInDir
    (
        m_themesDirectories,
        m_themesFileExtension
    );

    for (auto& theme_path : theme_paths)
        m_themes << Theme{ theme_path };

    for (auto& theme : m_themes)
    {
        qDebug(D::log)
            << "Theme added from:"
            << theme.path();
    }

    // If selected.theme == one of the new themes, all good
    // If not, we need to select a new one?
    // Maybe not. We could just leave the current theme and let it get changed
    // when the theme changes?

    // May not need to style if we have a cached theme and the cached theme was
    // not removed in the replacement of directories or file extension (what
    // calls this function)
    if (!m_cache.theme.isNull() && m_themes.contains(m_cache.theme)) return;

    if (m_cache.theme.isNull() || !m_themes.contains(m_cache.theme))
    {
        m_cache.theme = m_themes.first();
    }

    // Compile (may be recompiling existing cached theme)
    if (_compile())
        style();
}

QString WidgetGroup::_styleSheet() const
{
    QString style_sheet = m_baseQss;

    if (m_useTheme)
        style_sheet += m_cache.compiledQss;

    return style_sheet;
}

void WidgetGroup::addWidget(QWidget* widget)
{
    // We should always style on yoking. Consider newly spawned widgets, like a
    // dialog, which will need the styling before they are shown: in what case
    // would you ever actually want to not style a widget that you've yoked?
    m_widgets << widget;
    // Do we need to check that cache theme and themes template are valid?
    style(widget);
}

QString WidgetGroup::baseQss() const noexcept
{
    return m_baseQss;
}

bool WidgetGroup::contains(QWidget* const& widget) const noexcept
{
    return m_widgets.contains(widget);
}

Theme WidgetGroup::currentTheme() const noexcept
{
    return m_cache.theme;
}

WidgetGroup::Id WidgetGroup::id() const noexcept
{
    return m_id;
}

QString WidgetGroup::name() const noexcept
{
    return m_name;
}

qsizetype WidgetGroup::remove(QWidget* const& widget)
{
    return m_widgets.removeAll(widget);
}

void WidgetGroup::setBaseQss(const QString& qss)
{
    m_baseQss = qss;
}

void WidgetGroup::setCurrentTheme(const Theme& theme)
{
    // Need to do something similar when we set template!
    // Need to do this also when first theme is added!

    if (!m_themes.contains(theme))
    {
        qDebug(D::log)
            << __FUNCTION__
            << "Theme parameter not contained in Group themes";

        return;
    }

    if (_cacheTheme(theme) && _compile())
        style();
}

// Template needs to be set before it will compile a theme
void WidgetGroup::setCurrentTheme(const Coco::Path& themePath)
{
    if (auto theme = _find(themePath))
        setCurrentTheme(*theme);
}

void WidgetGroup::setName(const QString& name)
{
    m_name = name;
}

// Currently, calling setThemesDirectories will cause the Group to clear all
// current themes and rebuild from the new directories. The same is true for
// setThemesExtension
void WidgetGroup::setThemesDirectories(const QList<Coco::Path>& paths)
{
    if (m_themesDirectories == paths) return;
    m_themesDirectories = paths;

    if (!m_themesFileExtension.isEmpty())
        _resetThemes();

    // (Re-examine this, may already be doing it)
    // Need to set selected theme if none present
    // Same for the extensions, I believe
    // So likely handle in resetThemes
}

void WidgetGroup::setThemesExtension(const QString& fileExtension)
{
    if (m_themesFileExtension == fileExtension) return;
    m_themesFileExtension = Coco::Path::resolveExtension(fileExtension).toQString();

    if (!m_themesDirectories.isEmpty())
        _resetThemes();
}

void WidgetGroup::setThemesTemplate(const QString& themeTemplate)
{
    if (m_themesTemplate == themeTemplate) return;
    m_themesTemplate = themeTemplate;

    // If theme is not empty, then it both exists and has content
    if (_compile())
        style();
}

void WidgetGroup::setUseTheme(bool useTheme) noexcept
{
    m_useTheme = useTheme;
    style();
}

void WidgetGroup::style()
{
    _clean();

    auto style_sheet = _styleSheet();
    for (auto& widget : m_widgets)
        widget->setStyleSheet(style_sheet);
}

void WidgetGroup::style(QWidget* widget)
{
    // No cleaning needed
    if (!widget) return;

    if (m_widgets.contains(widget))
        widget->setStyleSheet(_styleSheet());
}

const QList<Theme>& WidgetGroup::themes() const noexcept
{
    return m_themes;
}

const QList<Coco::Path>& WidgetGroup::themesDirectories() const noexcept
{
    return m_themesDirectories;
}

QString WidgetGroup::themesExtension() const noexcept
{
    return m_themesFileExtension;
}

QString WidgetGroup::themesTemplate() const noexcept
{
    return m_themesTemplate;
}

bool WidgetGroup::useTheme() const noexcept
{
    return m_useTheme;
}

const QList<QPointer<QWidget>>& WidgetGroup::widgets() const noexcept
{
    return m_widgets;
}
