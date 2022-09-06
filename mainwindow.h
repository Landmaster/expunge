#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTemporaryDir>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QSharedPointer<QTemporaryDir> tempGameDir;
    Ui::MainWindow *ui;
    void importGame(const QString &path);
    void exportGame(const QString &path, bool isDirectory);
};
#endif // MAINWINDOW_H
