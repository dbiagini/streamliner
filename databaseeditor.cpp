
#include <QtWidgets>
#include <QtSql>

#include "databaseeditor.h"

#define DATABASE_TABLE "items"
#define DATABASE_PATH "C:/Users/dabi/Documents/Mantovana/mantovana.db"

DatabaseEditor::DatabaseEditor(QWidget *parent)
    : QDialog(parent)
{
    createConnection();
    model = new QSqlTableModel(this);
    model->setTable(DATABASE_TABLE);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, tr("Code"));
    model->setHeaderData(1, Qt::Horizontal, tr("Description"));
    model->setHeaderData(2, Qt::Horizontal, tr("Category"));

    QTableView *view = new QTableView;
    view->setModel(model);
    view->resizeColumnsToContents();

    submitButton = new QPushButton(tr("Submit"));
    submitButton->setDefault(true);
    revertButton = new QPushButton(tr("&Revert"));
    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(submitButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(revertButton, SIGNAL(clicked()), model, SLOT(revertAll()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(view);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Items Editor"));
}

DatabaseEditor::~DatabaseEditor()
{
    if (m_db.isOpen())
    {
        m_db.close(); //when the editor is destroyed close the connection
    }

}

void DatabaseEditor::submit()
{
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QMessageBox::warning(this, tr("Cached Table"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
    }
}

bool DatabaseEditor::createConnection()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(DATABASE_PATH);
    if (!m_db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

//    QSqlQuery query;
//    query.exec("create table person (id int primary key, "
//               "firstname varchar(20), lastname varchar(20))");
//    query.exec("insert into person values(101, 'Danny', 'Young')");
//    query.exec("insert into person values(102, 'Christine', 'Holand')");
//    query.exec("insert into person values(103, 'Lars', 'Gordon')");
//    query.exec("insert into person values(104, 'Roberto', 'Robitaille')");
//    query.exec("insert into person values(105, 'Maria', 'Papadopoulos')");

//    query.exec("create table items (id int primary key,"
//                                             "imagefile int,"
//                                             "itemtype varchar(20),"
//                                             "description varchar(100))");
//    query.exec("insert into items "
//               "values(0, 0, 'Qt',"
//               "'Qt is a full development framework with tools designed to "
//               "streamline the creation of stunning applications and  "
//               "amazing user interfaces for desktop, embedded and mobile "
//               "platforms.')");
//    query.exec("insert into items "
//               "values(1, 1, 'Qt Quick',"
//               "'Qt Quick is a collection of techniques designed to help "
//               "developers create intuitive, modern-looking, and fluid "
//               "user interfaces using a CSS & JavaScript like language.')");
//    query.exec("insert into items "
//               "values(2, 2, 'Qt Creator',"
//               "'Qt Creator is a powerful cross-platform integrated "
//               "development environment (IDE), including UI design tools "
//               "and on-device debugging.')");
//    query.exec("insert into items "
//               "values(3, 3, 'Qt Project',"
//               "'The Qt Project governs the open source development of Qt, "
//               "allowing anyone wanting to contribute to join the effort "
//               "through a meritocratic structure of approvers and "
//               "maintainers.')");

//    query.exec("create table images (itemid int, file varchar(20))");
//    query.exec("insert into images values(0, 'images/qt-logo.png')");
//    query.exec("insert into images values(1, 'images/qt-quick.png')");
//    query.exec("insert into images values(2, 'images/qt-creator.png')");
//    query.exec("insert into images values(3, 'images/qt-project.png')");

    return true;
}
