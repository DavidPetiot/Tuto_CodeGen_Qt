#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <QDialog>
#include <QTextEdit>
#include <QMainWindow>

class OutputWindow : public QDialog
{
public:
    OutputWindow(QString &p_hCode, QString &p_cCode, QMainWindow* mainWindow = 0);

private:
    QTextEdit m_output;
};

#endif // OUTPUTWINDOW_H
