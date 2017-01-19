#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include "outputwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_name(this)
    , m_motherName(this)
    , m_checkHeader(tr("&Header protection"),this)
    , m_checkConstructor(tr("Generate &constructor"),this)
    , m_checkDestructor(tr("Generate &destructor"),this)
{
    QWidget* l_centralWidget = new QWidget(this);
    this->setCentralWidget(l_centralWidget);

    QVBoxLayout* l_mainLayout = new QVBoxLayout(this);
    l_centralWidget->setLayout(l_mainLayout);

    //class definition
    QGroupBox* l_groupClassDef = new QGroupBox(tr("Class definition"), this);
    l_mainLayout->addWidget(l_groupClassDef);

    QFormLayout* l_formClassDef = new QFormLayout(this);
    l_groupClassDef->setLayout(l_formClassDef);

    l_formClassDef->addRow(tr("&Name"), &m_name);
    l_formClassDef->addRow(tr("&Mother class"), &m_motherName);

    //options
    QGroupBox* l_groupOptions = new QGroupBox(tr("Options"), this);
    l_mainLayout->addWidget(l_groupOptions);

    QGridLayout* l_layoutOptions = new QGridLayout(this);
    l_groupOptions->setLayout(l_layoutOptions);

    l_layoutOptions->addWidget(&m_checkHeader, 0, 0);
    l_layoutOptions->addWidget(&m_checkConstructor, 1, 0);
    l_layoutOptions->addWidget(&m_checkDestructor, 2, 0);
    l_layoutOptions->addWidget(&m_headerEdit, 0, 1);

    //Edit of header is hidden until check box is checked
    m_headerEdit.setVisible(false);

    //comments
    m_groupComments.setTitle(tr("&Comments"));
    l_mainLayout->addWidget(&m_groupComments);
    m_groupComments.setCheckable(true);
    m_groupComments.setChecked(false);

    QFormLayout* l_formComments = new QFormLayout(this);
    m_groupComments.setLayout(l_formComments);

    l_formComments->addRow(tr("&Author"), &m_author);
    l_formComments->addRow(tr("Creation &date"), &m_creationDate);
    l_formComments->addRow(tr("&Role"), &m_comments);

    m_creationDate.setDate(QDate::currentDate());

    //buttons
    QHBoxLayout* l_layoutButtons = new QHBoxLayout(this);
    l_mainLayout->addLayout(l_layoutButtons);

    QPushButton* l_buttonQuit = new QPushButton(tr("&quit"));
    QPushButton* l_buttonGenerate = new QPushButton(tr("&generate"));

    l_layoutButtons->addWidget(l_buttonGenerate);
    l_layoutButtons->addWidget(l_buttonQuit);

    QObject::connect(l_buttonQuit, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(l_buttonGenerate, SIGNAL(clicked()), this, SLOT(generateCode()));

    connect(&m_checkHeader, SIGNAL(clicked(bool)), &m_headerEdit, SLOT(setVisible(bool)));
    QObject::connect(&m_checkHeader, SIGNAL(clicked()), this, SLOT(updateHeader()));
    QObject::connect(&m_name, SIGNAL(textChanged(QString)), this, SLOT(updateHeader()));
}

void MainWindow::generateCode()
{
    if(m_name.text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("class name not filled"));
    }
    //test upper case ?
    else
    {
        m_generatedHpp = generateHpp();
        m_generatedCpp = generateCpp();

        OutputWindow* l_OutputWindow = new OutputWindow(m_generatedHpp, m_generatedCpp, this);
        l_OutputWindow->show();
    }
}

QString MainWindow::generateHpp()
{
    QString l_hCode;

    //comments
    if(m_groupComments.isChecked())
    {
        l_hCode += "/*\n";
        l_hCode += "Author : " + m_author.text() + "\n";
        l_hCode += "Creation date : " + m_creationDate.date().toString() + "\n";
        l_hCode += "Role :\n" + m_comments.toPlainText() + "\n*/\n\n";
    }

    //Header
    if(m_checkHeader.isChecked())
    {
        l_hCode += "#ifndef " + m_headerEdit.text() + "\n";
        l_hCode += "#define " + m_headerEdit.text() + "\n";
    }

    //class
    l_hCode += "class " + m_name.text();

    if(!m_motherName.text().isEmpty())
    {
        l_hCode += " : public " + m_motherName.text();
    }

    l_hCode += "\n{\npublic:\n";
    if(m_checkConstructor.isChecked())
    {
        l_hCode += "    " + m_name.text() + "();\n";
    }
    if(m_checkDestructor.isChecked())
    {
        l_hCode += "    ~" + m_name.text() + "();\n";
    }

    l_hCode += "\n\n    protected:\n";
    l_hCode += "\n\n    private:\n";
    l_hCode += "};\n\n";

    //close header
    if(m_checkHeader.isChecked())
    {
        l_hCode += "#endif //" + m_headerEdit.text() + "\n";
    }

    return l_hCode;
}

QString MainWindow::generateCpp()
{
    return "this will be the cpp";
}

void MainWindow::updateHeader()
{
    if( m_checkHeader.isChecked() && !(m_name.text().isEmpty()) )
    {
        m_headerEdit.setText("HEADER_" + m_name.text().toUpper());
    }
    else
    {
        m_headerEdit.setText(QString());
    }
}

void MainWindow::saveCode()
{
    QString l_directoryName = QFileDialog::getExistingDirectory(this, tr("save file"), QString(), QFileDialog::ShowDirsOnly) + "/";

    QFile l_headerFile(l_directoryName + "/" + m_name.text().toLower() + ".h");
    if(l_headerFile.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&l_headerFile);
        stream << m_generatedHpp << endl;
        l_headerFile.close();
    }

    QFile l_sourceFile(l_directoryName + "/" + m_name.text().toLower() + ".cpp");
    if(l_sourceFile.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&l_sourceFile);
        stream << m_generatedCpp << endl;
        l_sourceFile.close();
    }
}
