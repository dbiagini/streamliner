#ifndef DATABASEEDITOR
#define DATABASEEDITOR

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSqlTableModel>

class DatabaseEditor : public QDialog
{
    Q_OBJECT
public:
    DatabaseEditor(QWidget *parent = 0);
    ~DatabaseEditor();

private slots:
    void submit();

private:
    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QSqlTableModel *model;
    QSqlDatabase m_db;
    bool createConnection();

};

#endif // DATABASEEDITOR

