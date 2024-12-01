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

// For AOT, would need to be implemented in the dialog not the widget itself
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
