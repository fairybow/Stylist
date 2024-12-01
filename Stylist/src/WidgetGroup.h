/*
* Stylist: WidgetGroup.h  Copyright (C) 2024  fairybow
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
*
* This file uses Qt 6. Qt is a free and open-source widget toolkit for creating
* graphical user interfaces. For more information, visit <https://www.qt.io/>.
*
* Updated: 2024-11-30
*
* Note: This header is part of the internal implementation of Stylist and not
* intended for use by users. It may change without notice.
*/

#pragma once

#include "Coco/Path.h"
#include "Theme.h"

#include <QList>
#include <QPointer>
#include <QString>
#include <QWidget>

#include <typeindex>

// Would it be better / worth the memory to save Themes in Stylist, centrally?
// Rename TypeGroup?
class WidgetGroup
{
public:
    struct Id
    {
        std::type_index type = typeid(void);
        int role = -1;

        bool operator==(const Id& other) const;
        bool operator!=(const Id& other) const;
    };

    WidgetGroup();

    WidgetGroup
    (
        const Id& id,
        const QList<Coco::Path>& themesDirectories = {}
    );

    bool operator==(const WidgetGroup& other) const noexcept;
    bool operator!=(const WidgetGroup& other) const noexcept;

    //------------------------------------------------------------
    // Properties
    //------------------------------------------------------------

private:
    Id m_id;

    // Monitor this and add new themes?:
    QList<Coco::Path> m_themesDirectories{};
    QString m_name{};

    void _resetThemes();

public:
    Id id() const noexcept;
    const QList<Coco::Path>& themesDirectories() const noexcept;
    void setThemesDirectories(const QList<Coco::Path>& paths);
    QString name() const noexcept;
    void setName(const QString& name);

    //------------------------------------------------------------
    // Themes
    //------------------------------------------------------------

private:
    bool m_useTheme = true;

    // Not something we need to monitor (like theme file paths)?:
    QString m_baseQss{};
    QString m_themesTemplate{};
    QString m_themesFileExtension{};
    QList<Theme> m_themes{};

public:
    bool useTheme() const noexcept;
    void setUseTheme(bool useTheme) noexcept;
    QString baseQss() const noexcept;
    void setBaseQss(const QString& qss);
    QString themesTemplate() const noexcept;
    void setThemesTemplate(const QString& themesTemplate);
    QString themesExtension() const noexcept;
    void setThemesExtension(const QString& fileExtension);
    void setCurrentTheme(const Theme& theme);
    void setCurrentTheme(const Coco::Path& themePath);
    Theme currentTheme() const noexcept;
    const QList<Theme>& themes() const noexcept;

    //------------------------------------------------------------
    // Styling
    //------------------------------------------------------------

private:
    void _clean();
    QString _styleSheet() const;

public:
    void style();
    void style(QWidget* widget);

    //------------------------------------------------------------
    // Widgets
    //------------------------------------------------------------

private:
    QList<QPointer<QWidget>> m_widgets{};

public:
    const QList<QPointer<QWidget>>& widgets() const noexcept;
    bool contains(QWidget* const& widget) const noexcept;
    qsizetype remove(QWidget* const& widget);
    void addWidget(QWidget* widget);

    //------------------------------------------------------------
    // Caching current theme
    //------------------------------------------------------------

private:
    struct Cache
    {
        Theme theme{};
        QString compiledQss{};

    } m_cache;

    Theme* _find(const Coco::Path& path) const;
    bool _cacheTheme(const Theme& theme);
    bool _compile();

}; // class WidgetGroup
