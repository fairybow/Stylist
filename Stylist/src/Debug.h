/*
* Stylist: Debug.h  Copyright (C) 2024  fairybow
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
*
* This file uses Qt 6. Qt is a free and open-source widget toolkit for creating
* graphical user interfaces. For more information, visit <https://www.qt.io/>.
*
* Updated: 2024-11-26
*
* Note: This header is part of the internal implementation of Stylist and not
* intended for use by users. It may change without notice.
*/

#pragma once

#include <QDebug>
#include <QLoggingCategory>
#include <QString>

#include <utility>

#define dUtf8Format(...) qUtf8Printable(D::format(__VA_ARGS__))

// Add a way to print function name, like Stylist (functionCall):...
namespace D
{
    // Q_LOGGING_CATEGORY
    inline const QLoggingCategory& log()
    {
        static const QLoggingCategory category("Stylist");
        return category;
    }

    template <typename... Args>
    inline QString format(const char* formatStr, Args&&... args)
    {
        auto message = QString::fromUtf8(formatStr);
        ((message = message.arg(std::forward<Args>(args))), ...);
        return message;
    }

} // namespace D
