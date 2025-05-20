/*
* Stylist: Controller.h  Copyright (C) 2024  fairybow
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

#include "Global.h"
#include "Stylist.h"

#include <memory>

// For AOT, would need to be implemented in the dialog not the widget itself The
// built-in dialog is only modeless right now, so a toggle for it in Stylist
// makes sense, if it stays that way. If we switch to modal or make it an
// optional, then may not make sense (or could just have toggle only with
// modeless)
class STYLIST_EXPORT Stylist::Controller : public QWidget
{
    Q_OBJECT

public:
    Controller(Stylist* stylist, QWidget* parent = nullptr);
    ~Controller();

    void dumpStylistGroupNames() const;

private:
    class Private;
    std::unique_ptr<Private> m_private;

}; // class Stylist::Controller
