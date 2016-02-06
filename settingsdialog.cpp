#include "settingsdialog.h"
//#include <QtGui>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QMessageBox>

#define SUCCESS 0
#define FAILURE 1

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    label = new QLabel(tr("Destination Directory:"));
    lineEdit = new QLineEdit;
    lineEdit->setMaximumWidth(600);
    label->setBuddy(lineEdit);

    dest_path = new QString;

    OkButton = new QPushButton(tr("OK"));
    OkButton->setDefault(true);
    CancelButton = new QPushButton(tr("Cancel"));
    ApplyButton = new QPushButton(tr("Apply"));


    //layouts//
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(label);
    leftLayout->addWidget(lineEdit);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(OkButton);
    rightLayout->addWidget(CancelButton);
    rightLayout->addWidget(ApplyButton);
    rightLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
    setWindowTitle(tr("Settings"));
    setFixedHeight(sizeHint().height());
    setFixedWidth(600);

    connect(OkButton, SIGNAL(clicked()), this, SLOT(applySettings()));
    connect(CancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ApplyButton, SIGNAL(clicked()), this, SLOT(applySettings()));

}

int SettingsDialog::setPath(QString *in_path)
{
    if(in_path != NULL)
    {
        dest_path->clear();
        dest_path->append(in_path);
        lineEdit->setText((const QString) *dest_path);
        return SUCCESS;
    }else
    {
        QMessageBox::warning(this, tr("Error"), tr("Empty Path or non valid"));
        return FAILURE;
    }
}

int SettingsDialog::getPath(QString &out_path)
{
        out_path.append(dest_path);
        return SUCCESS;
}

void SettingsDialog::applySettings()
{
    QString l_text = lineEdit->text();
    setPath(&l_text);
    emit saveSettings(l_text);
    this->close();
}
