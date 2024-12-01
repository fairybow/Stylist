/*
* Stylist: StylistPrivate.h  Copyright (C) 2024  fairybow
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
#include "NestedPrivate.h"
#include "Stylist.h"
#include "Theme.h"
#include "WidgetGroup.h"

#include <QList>
#include <QPointer>

class Stylist::Private : public NestedPrivate<Stylist>
{
public:
    //------------------------------------------------------------
    // WidgetStyleGroups
    //------------------------------------------------------------

    QList<WidgetGroup> widgetGroups{};

    // The find-or-make approach allows us to define a WidgetGroup without
    // having to yoke any widgets (so that it may appear in the editing dialog)
    WidgetGroup* findOrMake(const WidgetGroup::Id& id);
    WidgetGroup* find(const WidgetGroup::Id& id) const;
    WidgetGroup* find(QWidget* const& widget) const;
    WidgetGroup* find(const QPointer<QWidget>& widget) const;
    WidgetGroup* make(const WidgetGroup::Id& id);

    //------------------------------------------------------------
    // Themes
    //------------------------------------------------------------

    QList<Coco::Path> themesDirectories{};

    //------------------------------------------------------------
    // Other event handlers
    //------------------------------------------------------------

    void handleCloseEvent(QObject* watched);

    //------------------------------------------------------------
    // Dialog
    //------------------------------------------------------------

    QPointer<QDialog> ownDialog{};

    QDialog* newDialog();
    void showOwnDialog();

}; // class Stylist::Private
