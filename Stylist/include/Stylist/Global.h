/*
* Stylist: Global.h  Copyright (C) 2024  fairybow
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
*
* This file uses Qt 6. Qt is a free and open-source widget toolkit for creating
* graphical user interfaces. For more information, visit <https://www.qt.io/>.
*
* Updated: 2024-11-30
*/

#pragma once

#include <QtCore/qglobal.h>
#include <QWidget>

#ifndef BUILD_STATIC
#   if defined(STYLIST_LIB)
#       define STYLIST_EXPORT Q_DECL_EXPORT
#   else
#       define STYLIST_EXPORT Q_DECL_IMPORT
#   endif
#else
#   define STYLIST_EXPORT
#endif
