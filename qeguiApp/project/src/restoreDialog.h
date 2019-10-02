/*  restoreDialog.h
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  Copyright (c) 2013-2019 Australian Synchrotron
 *
 *  The EPICS QT Framework is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The EPICS QT Framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with the EPICS QT Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

#ifndef RESTORE_DIALOG_H
#define RESTORE_DIALOG_H

#include <QEDialog.h>
#include <QListWidget>

namespace Ui {
    class restoreDialog;
}

class restoreDialog : public QEDialog
{
    Q_OBJECT

public:
    explicit restoreDialog( QStringList names, bool hasDefault, QWidget *parent = 0 );
    ~restoreDialog();

    bool getUseDefault() const;
    QString getName() const;

private:
    Ui::restoreDialog *ui;

    void enableNamedItems( bool enable );
    void enableOpen();

private slots:
    void on_namesListWidget_itemSelectionChanged();
    void on_namesListWidget_doubleClicked(QModelIndex index);
    void on_namedRadioButton_clicked(bool checked);
    void on_defaultRadioButton_clicked(bool checked);
};

#endif // RESTORE_DIALOG_H
