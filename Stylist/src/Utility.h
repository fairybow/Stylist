/*
* Stylist: Utility.h  Copyright (C) 2024  fairybow
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
*
* This file uses Qt 6. Qt is a free and open-source widget toolkit for creating
* graphical user interfaces. For more information, visit <https://www.qt.io/>.
*
* Updated: 2024-11-28
*
* Note: This header is part of the internal implementation of Stylist and not
* intended for use by users. It may change without notice.
*/

#pragma once

#include <QList>

#include <algorithm>

namespace Utility
{
    template <typename T, typename UnaryPredicateT>
    T* findIf(const QList<T>& qList, UnaryPredicateT predicate)
    {
        auto it = std::find_if
        (
            qList.constBegin(),
            qList.constEnd(),
            predicate
        );

        if (it != qList.constEnd())
            return const_cast<T*>(&(*it));

        return nullptr;
    }

} // namespace Utility
