#ifndef MANAGECONFIGDIALOG_H
#define MANAGECONFIGDIALOG_H

#include <QDialog>

namespace Ui {
    class manageConfigDialog;
}

class manageConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit manageConfigDialog( QStringList names, bool hasDefault, QWidget *parent = 0 );
    ~manageConfigDialog();
    void setCurrentNames( QStringList currentNamesIn );


private:
    Ui::manageConfigDialog *ui;
    QStringList currentNames;

signals:
    void deleteConfigs( manageConfigDialog* mcd, const QStringList names );

private slots:
    void on_deleteDefaultPushButton_clicked();
    void on_deletePushButton_clicked();
    void on_namesListWidget_itemSelectionChanged();
};

#endif // MANAGECONFIGDIALOG_H
