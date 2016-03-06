
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
    initHash();

    submitButton = new QPushButton(tr("Submit"));
    submitButton->setDefault(true);
    revertButton = new QPushButton(tr("&Revert"));
    quitButton = new QPushButton(tr("Quit"));
    importButton = new QPushButton(tr("Import"));

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(submitButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(importButton, QDialogButtonBox::RejectRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);


    connect(submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(revertButton, SIGNAL(clicked()), model, SLOT(revertAll()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(importButton, SIGNAL(clicked()), this, SLOT(import()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    QVBoxLayout *rightSideLayout = new QVBoxLayout;
    mainLayout->addWidget(view);
    rightSideLayout->addWidget(buttonBox);

    setWindowTitle(tr("Items Editor"));
    db_movie = new QMovie(":/images/loader.gif");
    mv_lable = new QLabel(this);
    mv_lable->setMovie(db_movie);
    rightSideLayout->addWidget(mv_lable,0,Qt::AlignTop|Qt::AlignHCenter);
    mainLayout->addLayout(rightSideLayout);
    setLayout(mainLayout);
}
void DatabaseEditor::initHash()
{
    cat_hash = new QHash<QString, QString>;
    cat_hash->insert("ACS", "librerie");
    cat_hash->insert("TAS", "tavoli");
    cat_hash->insert("MUS", "murature");
    cat_hash->insert("COS", "cornici");
    cat_hash->insert("MES", "mensole");
    cat_hash->insert("PIS", "piani");
    cat_hash->insert("ANS", "piani");
    cat_hash->insert("MUS", "misto");
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
    //QSqlQuery query;
    //query.exec("INSERT INTO items VALUES('first','test','piani')");

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
int DatabaseEditor::processLine(QString *out_parsed ,QStringList *in_line)
{
    if(in_line)
    {

      int l_offs = (int)(in_line->count()-4);
      QString l_cat = in_line->at(l_offs);
      *out_parsed = cat_hash->value(l_cat);
      if(!out_parsed->isEmpty())
          return true;
      else
      {
          QMessageBox::warning(this, tr("Item Category not recognized: "),
                               tr(" %1:\n")
                               .arg(l_cat));
          return false;
      }

    }
    else
        return false;


}
void DatabaseEditor::import()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        QFile f(fileName);
        if(f.open (QIODevice::ReadOnly | QFile::Text)){
            QSqlQuery que;
            QTextStream ts (&f);
            QStringList line = ts.readLine().split(','); //throw away title line
            if(model->rowCount())
            {
                if (QMessageBox::No == QMessageBox(QMessageBox::Information, tr("Database Update"), tr("The database is not empty do you want to overwrite it?"),
                                                    QMessageBox::Yes|QMessageBox::No).exec())
                {
                    QMessageBox::warning(this, tr("Database Update"),
                                         tr("Database update skipped"));
                    return;
                }
                else
                {
                    //need to clear the database first
                    QSqlQuery query_clear;
                    query_clear.exec("DELETE from " DATABASE_TABLE);
                    query_clear.exec("VACUUM");
                    /*QMessageBox::warning(this, tr("Query not successful"),
                                     tr("%1 \n%2")
                                     .arg(query_clear.executedQuery())
                                     .arg(query_clear.lastError().text()));*/

                }
            }
            mv_lable->show();
            db_movie->start();

            while(!ts.atEnd()){

                // split every lines on comma
                line = ts.readLine().split(',');
                /*for every values on a line,
                  append it to the INSERT request*/
                QString l_processed;
                if(processLine(&l_processed,&line)){
                    QString req = "INSERT INTO "DATABASE_TABLE" VALUES('";

                        req.append(line.at(0));
                        req.append("','");
                        QString l_mid = line.at(1);
                        //remove special characters//
                        l_mid.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»?/{}\'\"\\\[\\\]\\\\]")));
                        req.append(l_mid);
                        req.append("','");
                        req.append(l_processed);
                        req.append("',");
                        req.chop(1); // remove the trailing comma
                        req.append(");"); // close the "VALUES([...]" with a ");"
                        if(!que.exec(req))
                            QMessageBox::warning(this, tr("Query not successful"),
                                             tr("%1 \n%2")
                                             .arg(req)
                                             .arg(que.lastError().text()));
                         QCoreApplication::processEvents();
                } //otherwise skip the line
            }
            f.close ();
            model->select(); //update Database View
            db_movie->stop();
            mv_lable->hide();
        } else
        {
            QMessageBox::warning(this, tr("Application"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(f.errorString()));
            return;
        }

    }
}
