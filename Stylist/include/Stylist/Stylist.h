/*
* Stylist: Stylist.h  Copyright (C) 2024  fairybow
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
*
* This file uses Qt 6. Qt is a free and open-source widget toolkit for creating
* graphical user interfaces. For more information, visit <https://www.qt.io/>.
*
* Updated: 2024-12-1
*/

/// @todo Redo code style
/// @todo Connect destroyed signal to lambda that runs clean and remove it elsewhere
/// @todo IWYU
/// @todo Update scripts (from Fernanda dev)
/// @todo License header txt (from Fernanda dev)
/// @todo Restructure filters
/// @todo Probably make controller dialog blocking if it isn't already

#pragma once

#include "Global.h"

#include <QEvent>
#include <QString>

#include <filesystem>
#include <memory>
#include <typeindex>

class STYLIST_EXPORT Stylist : public QObject
{
    Q_OBJECT

public:
    class Controller;
    friend class Controller;

    Stylist(QObject* parent = nullptr);
    ~Stylist();

    // * = If a Group does not exist one will be created by this function

    // Associate widgets with Stylist
    void yoke(QWidget* widget, int role = 0); // *

    // Set all relevant properties for a widget style group at once
    void define // *
    (
        std::type_index qWidgetType,
        const QString& name,
        const std::filesystem::path& themesTemplatePath,
        const QString& themesFileExtension,
        const std::filesystem::path& baseQssPath,
        int role = 0
    );

    template <typename QWidgetT> // *
    void define
    (
        const QString& name,
        const std::filesystem::path& themesTemplatePath,
        const QString& themesFileExtension,
        const std::filesystem::path& baseQssPath,
        int role = 0
    )
    {
        define
        (
            typeid(QWidgetT),
            name,
            themesTemplatePath,
            themesFileExtension,
            baseQssPath,
            role
        );
    }

    // Overload for skipping base QSS
    void define // *
    (
        std::type_index qWidgetType,
        const QString& name,
        const std::filesystem::path& themesTemplatePath,
        const QString& themesFileExtension,
        int role = 0
    );

    template <typename QWidgetT> // *
    void define
    (
        const QString& name,
        const std::filesystem::path& themesTemplatePath,
        const QString& themesFileExtension,
        int role = 0
    )
    {
        define
        (
            typeid(QWidgetT),
            name,
            themesTemplatePath,
            themesFileExtension,
            role
        );
    }

    //------------------------------------------------------------
    // WidgetGroup properties
    //------------------------------------------------------------

    void setName // *
    (
        std::type_index qWidgetType,
        const QString& name,
        int role = 0
    );

    template <typename QWidgetT> // *
    void setName(const QString& name, int role = 0)
    {
        setName
        (
            typeid(QWidgetT),
            name,
            role
        );
    }

    //------------------------------------------------------------
    // WidgetGroup themes
    //------------------------------------------------------------

    // Themes will be added whenever directories and theme file extension are
    // both set. In WidgetGroup private, themes are just reset each time when
    // calling setThemesDirectories. We keep a list in Stylist::Private, add to
    // that, and call setThemesDirectories with the appeneded list
    void addThemesDirectory(const std::filesystem::path& directory);

    void setUseTheme
    (
        std::type_index qWidgetType,
        bool useTheme,
        int role = 0
    );

    template <typename QWidgetT>
    void setUseTheme(bool useTheme, int role = 0)
    {
        setUseTheme
        (
            typeid(QWidgetT),
            useTheme,
            role
        );
    }

    void setBaseQss // *
    (
        std::type_index qWidgetType,
        const std::filesystem::path& path,
        int role = 0
    );

    template <typename QWidgetT> // *
    void setBaseQss(const std::filesystem::path& path, int role = 0)
    {
        setBaseQss
        (
            typeid(QWidgetT),
            path,
            role
        );
    }

    void setThemesTemplate // *
    (
        std::type_index qWidgetType,
        const std::filesystem::path& path,
        int role = 0
    );

    template <typename QWidgetT> // *
    void setThemesTemplate(const std::filesystem::path& path, int role = 0)
    {
        setThemesTemplate
        (
            typeid(QWidgetT),
            path,
            role
        );
    }

    void setThemesExtension // *
    (
        std::type_index qWidgetType,
        const QString& fileExtension,
        int role = 0
    );

    template <typename QWidgetT> // *
    void setThemesExtension(const QString& fileExtension, int role = 0)
    {
        setThemesExtension
        (
            typeid(QWidgetT),
            fileExtension,
            role
        );
    }

    void setCurrentTheme
    (
        std::type_index qWidgetType,
        const std::filesystem::path& themePath,
        int role = 0
    );

    template <typename QWidgetT>
    void setCurrentTheme(const std::filesystem::path& themePath, int role = 0)
    {
        setCurrentTheme
        (
            typeid(QWidgetT),
            themePath,
            role
        );
    }

    //------------------------------------------------------------
    // Dialog
    //------------------------------------------------------------

    // Show a modeless dialog containing a Stylist::Controller widget
    void showDialog();

    //------------------------------------------------------------
    // Debug
    //------------------------------------------------------------

    void sayHello() const;
    void printThemesDirectories() const;
    void printGroupInfo(std::type_index qWidgetType, int role = 0) const;

    template <typename QWidgetT>
    void printGroupInfo(int role = 0) const
    {
        printGroupInfo(typeid(QWidgetT), role);
    }

    void dumpGroupsInfo() const;
    void printTemplate(std::type_index qWidgetType, int role = 0) const;

    template <typename QWidgetT>
    void printTemplate(int role = 0) const
    {
        printTemplate(typeid(QWidgetT), role);
    }

    void dumpTemplates() const;
    void dump() const;

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    class Private;
    std::unique_ptr<Private> m_private;

}; // class Stylist
