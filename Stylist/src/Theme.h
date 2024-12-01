/*
* Stylist: Theme.h  Copyright (C) 2024  fairybow
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

#include <QByteArray>
#include <QJsonDocument>
#include <QString>
#include <QVariant>

// Warning! "Null" could still have a path and return isNull. I'm not sure it
// ever realistically would, especially if the JSON is retrieved as soon as the
// path is received by the class, but it can be set on a null theme nonetheless.
class Theme
{
public:
    Theme();
    Theme(const Theme& theme);
    //Theme(Theme&& theme) noexcept;
    Theme(const Coco::Path& path);
    ~Theme();

    //Theme& operator=(const Theme& other);
    //Theme& operator=(Theme&& other) noexcept;
    bool operator==(const Theme& other) const;
    bool operator!=(const Theme& other) const;

    Coco::Path path() const;
    void setPath(const Coco::Path& path);
    QString name() const;
    void setName(const QString& name);
    QString compileQss(const QString& themeTemplate) const;
    QByteArray toJson() const;
    QVariant toQVariant() const;

    // Has an internal JSON document
    bool isNull() const;

    // Has content. A null document will also be empty. To query whether the
    // document exists and has content, call !theme.isEmpty()
    bool isEmpty() const;

private:
    Coco::Path m_path{};

    // Initializes to a null document
    QJsonDocument m_jsonDoc{};

}; // class Theme
