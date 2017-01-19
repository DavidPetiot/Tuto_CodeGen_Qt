#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QLineEdit>
#include <QDateEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QGroupBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void generateCode();
    void saveCode();
    void updateHeader();

private:
    QString generateHpp();
    QString generateCpp();

private:
    QLineEdit m_name;
    QLineEdit m_motherName;
    QCheckBox m_checkHeader;
    QLineEdit m_headerEdit;
    QCheckBox m_checkConstructor;
    QCheckBox m_checkDestructor;
    QGroupBox m_groupComments;
    QLineEdit m_author;
    QDateEdit m_creationDate;
    QTextEdit m_comments;

    QString m_generatedHpp;
    QString m_generatedCpp;
};

#endif // MAINWINDOW_H
