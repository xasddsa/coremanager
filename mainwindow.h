#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <unistd.h> // For geteuid()

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
    Ui::MainWindow *ui;
    // The following members are now accessed via ui pointer
    // QLabel *overallCpuFrequencyLabel;
    // QComboBox *cpuGovernorComboBox;
    // QLabel *cpuCoresLabel;
    // QListWidget *cpuCoresListWidget;
    // QPushButton *applyChangesButton; // This is also in the .ui file

private slots:
    void loadCpuInformation();
    void applyCpuSettings();

private:
    int getCpuCoreCount();
    QStringList getAvailableFrequencies(int coreNum);
    QString getCurrentFrequency(int coreNum);
    QStringList getAvailableGovernors(int coreNum);
    QString getCurrentGovernor(int coreNum);
    bool isCoreOnline(int coreNum);
    // QPushButton *applyChangesButton; // Removed, as it's part of the UI file and accessed via ui->applyChangesButton

    bool isRunningAsRoot();
};
#endif // MAINWINDOW_H
