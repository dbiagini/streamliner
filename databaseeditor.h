#ifndef DATABASEEDITOR
#define DATABASEEDITOR

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSqlTableModel>
#include <QMovie>
#include <QLabel>

class DatabaseEditor : public QDialog
{
    Q_OBJECT
public:
    DatabaseEditor(QWidget *parent = 0);
    ~DatabaseEditor();

private slots:
    void submit();
    void import();

private:
    QPushButton *submitButton;
    QMovie *db_movie;
    QLabel *mv_lable;
    QPushButton *revertButton;
    QPushButton *quitButton;
    QPushButton *importButton;
    QDialogButtonBox *buttonBox;
    QSqlTableModel *model;
    QSqlDatabase m_db;
    QHash<QString,QString> *cat_hash;
    bool createConnection();
    void initHash();
    int processLine(QString *out_parsed ,QStringList *in_line);

};

#endif // DATABASEEDITOR

