#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class saveDialog;
}

class saveDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit saveDialog(QStringList names, QWidget *parent = 0);
    ~saveDialog();
    
    bool getUseDefault();
    QString getName();

private slots:
    void on_nameLineEdit_textChanged(QString );
    void on_namesListWidget_doubleClicked(QModelIndex index);
    void on_namesListWidget_clicked(QModelIndex index);

    void on_defaultRadioButton_clicked(bool checked);

    void on_namedRadioButton_clicked(bool checked);

private:
    Ui::saveDialog *ui;

    void enableNamedItems( bool enable );

    bool savingStartup;
    void enableSave();

};

#endif // SAVEDIALOG_H
