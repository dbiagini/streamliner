#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent = 0);
    int setPath(QString *in_path);
    int getPath(QString &out_path);
signals:
    saveSettings(const QString &out_path);
private slots:
    void applySettings();
private:
    QString *dest_path;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *OkButton;
    QPushButton *CancelButton;
    QPushButton *ApplyButton;
};
#endif // SETTINGSDIALOG_H

