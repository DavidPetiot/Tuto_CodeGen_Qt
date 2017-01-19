#include "outputwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QTabWidget>
#include "mainwindow.h"

OutputWindow::OutputWindow(QString &p_hCode, QString &p_cCode, QMainWindow *mainWindow)
{
    //hpp
    QTextEdit* l_hCodeOutput = new QTextEdit();
    l_hCodeOutput->setPlainText(p_hCode);
    l_hCodeOutput->setReadOnly(true);
    l_hCodeOutput->setFont(QFont("courier"));
    l_hCodeOutput->setLineWrapMode(QTextEdit::NoWrap);

    //cpp
    QTextEdit* l_cCodeOutput = new QTextEdit();
    l_cCodeOutput->setPlainText(p_cCode);
    l_cCodeOutput->setReadOnly(true);
    l_cCodeOutput->setFont(QFont("courier"));
    l_cCodeOutput->setLineWrapMode(QTextEdit::NoWrap);

    QTabWidget* l_tabLayout = new QTabWidget();
    l_tabLayout->addTab(l_hCodeOutput, "hpp");
    l_tabLayout->addTab(l_cCodeOutput, "cpp");

    //buttons
    QPushButton* l_closeButton = new QPushButton(tr("close"));
    QPushButton* l_saveButton = new QPushButton(tr("save"));

    QHBoxLayout* l_buttonLayout = new QHBoxLayout();
    l_buttonLayout->addWidget(l_closeButton);
    l_buttonLayout->addWidget(l_saveButton);

    QVBoxLayout* l_mainLayout = new QVBoxLayout();
    l_mainLayout->addWidget(l_tabLayout);
    l_mainLayout->addLayout(l_buttonLayout);

    this->setLayout(l_mainLayout);

    QObject::connect(l_closeButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    QObject::connect(l_saveButton, SIGNAL(clicked(bool)), mainWindow, SLOT(saveCode()));
    QObject::connect(l_saveButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
}
