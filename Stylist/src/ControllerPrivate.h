/*
* Stylist: ControllerPrivate.h  Copyright (C) 2024  fairybow
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

#include "Controller.h"
#include "NestedPrivate.h"
#include "WidgetGroup.h"

#include <QComboBox>

using Controller = Stylist::Controller;

class Controller::Private : public NestedPrivate<Controller>
{
public:
    Private(Stylist* s, Controller* publicClass);

    //------------------------------------------------------------
    // Stylist
    //------------------------------------------------------------

    // Remember: Stylist is NOT the public class here
    Stylist* stylist;

    // Returns the Stylist's private class's groups
    QList<WidgetGroup>& stylistGroups() const noexcept;

    //------------------------------------------------------------
    // UI
    //------------------------------------------------------------

    void initUi();
    void createMultiGroupUi();
    void createSingleGroupUi();

    //------------------------------------------------------------
    // Combo boxes
    //------------------------------------------------------------

    void updateTheme(QComboBox* comboBox, WidgetGroup& group);

}; // class Stylist::Controller::Private
