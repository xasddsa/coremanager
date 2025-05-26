#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadCpuInformation();
    connect(ui->applyChangesButton, &QPushButton::clicked, this, &MainWindow::applyCpuSettings);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Helper function to read the first line from a file
QString readFirstLine(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Cannot open file: %s", qPrintable(filePath));
        return QString();
    }
    QTextStream in(&file);
    QString line = in.readLine();
    file.close();
    return line;
}

// Helper function to read all lines from a file
QStringList readAllLines(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Cannot open file: %s", qPrintable(filePath));
        return QStringList();
    }
    QStringList lines;
    QTextStream in(&file);
    while (!in.atEnd()) {
        lines << in.readLine();
    }
    file.close();
    return lines;
}

int MainWindow::getCpuCoreCount()
{
    QDir cpuDir("/sys/devices/system/cpu/");
    cpuDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList cpuEntries = cpuDir.entryList(QStringList() << "cpu[0-9]*", QDir::Dirs);
    return cpuEntries.count();
}

QStringList MainWindow::getAvailableFrequencies(int coreNum)
{
    QString path = QString("/sys/devices/system/cpu/cpu%1/cpufreq/scaling_available_frequencies").arg(coreNum);
    QString line = readFirstLine(path);
    if (line.isEmpty()) return QStringList();
    return line.split(" ", Qt::SkipEmptyParts);
}

QString MainWindow::getCurrentFrequency(int coreNum)
{
    return readFirstLine(QString("/sys/devices/system/cpu/cpu%1/cpufreq/scaling_cur_freq").arg(coreNum));
}

QStringList MainWindow::getAvailableGovernors(int coreNum)
{
    QString path = QString("/sys/devices/system/cpu/cpu%1/cpufreq/scaling_available_governors").arg(coreNum);
    QString line = readFirstLine(path);
    if (line.isEmpty()) return QStringList();
    return line.split(" ", Qt::SkipEmptyParts);
}

QString MainWindow::getCurrentGovernor(int coreNum)
{
    return readFirstLine(QString("/sys/devices/system/cpu/cpu%1/cpufreq/scaling_governor").arg(coreNum));
}

bool MainWindow::isCoreOnline(int coreNum)
{
    QString path = QString("/sys/devices/system/cpu/cpu%1/online").arg(coreNum);
    QString line = readFirstLine(path);
    return line == "1";
}

void MainWindow::loadCpuInformation()
{
    int coreCount = getCpuCoreCount();
    // Store coreCount in a member variable if it's intended to be m_cpuCoreCount,
    // otherwise, just use the local variable. For now, assuming it's a local var as m_cpuCoreCount is not declared.
    // int m_cpuCoreCount = coreCount; // If m_cpuCoreCount is a member
    if (coreCount <= 0) {
        QMessageBox::critical(this, "Fatal Error", "Could not determine CPU core count. The application might not function correctly.");
        ui->overallCpuFrequencyLabel->setText("Overall CPU Frequency: Error");
        ui->cpuCoresLabel->setText("CPU Cores: Error");
        // Disable UI elements
        ui->cpuGovernorComboBox->setEnabled(false);
        ui->cpuCoresListWidget->setEnabled(false);
        ui->applyChangesButton->setEnabled(false);
        return;
    }

    // For CPU0 or the first available online core
    int primaryCore = 0;
    for (int i = 0; i < coreCount; ++i) {
        if (isCoreOnline(i)) {
            primaryCore = i;
            break;
        }
    }

    if (isCoreOnline(primaryCore)) {
        QString currentFreq = getCurrentFrequency(primaryCore);
        if (!currentFreq.isEmpty()) {
            bool ok;
            double freqMHz = currentFreq.toDouble(&ok) / 1000.0;
            if (ok) {
                ui->overallCpuFrequencyLabel->setText(QString("Overall CPU Frequency (Core %1): %2 MHz").arg(primaryCore).arg(freqMHz, 0, 'f', 0));
            } else {
                ui->overallCpuFrequencyLabel->setText(QString("Overall CPU Frequency (Core %1): %2 (raw)").arg(primaryCore).arg(currentFreq));
            }
        } else {
            ui->overallCpuFrequencyLabel->setText(QString("Overall CPU Frequency (Core %1): N/A").arg(primaryCore));
        }

        ui->cpuGovernorComboBox->clear();
        QStringList frequencies = getAvailableFrequencies(primaryCore);
        for (const QString &freq : frequencies) {
            bool ok;
            double freqMHz = freq.toDouble(&ok) / 1000.0;
            if (ok) {
                ui->cpuGovernorComboBox->addItem(QString("%1 MHz").arg(freqMHz, 0, 'f', 0), freq);
            } else {
                 ui->cpuGovernorComboBox->addItem(QString("%1 (raw)").arg(freq), freq);
            }
        }

        QStringList governors = getAvailableGovernors(primaryCore);
        for (const QString &gov : governors) {
            ui->cpuGovernorComboBox->addItem(QString("Governor: %1").arg(gov), gov);
        }

        // Select current governor/frequency (governor takes precedence if active)
        QString currentGov = getCurrentGovernor(primaryCore);
        if (!currentGov.isEmpty()) {
            int index = ui->cpuGovernorComboBox->findText(QString("Governor: %1").arg(currentGov));
            if (index != -1) {
                ui->cpuGovernorComboBox->setCurrentIndex(index);
            }
        } else {
            // If no governor, try to select current frequency
            if (!currentFreq.isEmpty()) {
                 int index = ui->cpuGovernorComboBox->findData(currentFreq);
                 if (index != -1) {
                     ui->cpuGovernorComboBox->setCurrentIndex(index);
                 }
            }
        }


    } else {
        ui->overallCpuFrequencyLabel->setText(QString("Overall CPU Frequency (Core %1): Offline").arg(primaryCore));
        ui->cpuGovernorComboBox->clear();
        ui->cpuGovernorComboBox->addItem("N/A - Core Offline");
        QMessageBox::warning(this, "Warning", QString("Could not read frequency or governor information for the primary CPU (Core %1 is offline or info unavailable). Controls may be limited.").arg(primaryCore));
    // This else was misplaced. It should be part of the if (isCoreOnline(primaryCore)) block.
    // } else { 
    // The following logic should be inside the 'if (isCoreOnline(primaryCore))' or handled differently if primaryCore is offline.
    // For now, let's assume if primaryCore is not online, the ComboBox might be empty or show "N/A"
    // and this warning might be redundant or should be phrased differently.
    // A simple check after attempting to populate:
        if (isCoreOnline(primaryCore) && ui->cpuGovernorComboBox->count() == 0) {
             QMessageBox::warning(this, "Warning", QString("Frequency/governor data for primary CPU (Core %1) could not be loaded, though core is online. Controls may be limited.").arg(primaryCore));
        }
    }


    ui->cpuCoresListWidget->clear();
    ui->cpuCoresLabel->setText(QString("CPU Cores: %1").arg(coreCount));

    for (int i = 0; i < coreCount; ++i) {
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, i); // Store core number
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

        QString itemText;
        bool coreIsOnline = isCoreOnline(i);

        if (coreIsOnline) {
            QString freq = getCurrentFrequency(i);
            if (!freq.isEmpty()) {
                bool ok;
                double freqMHz = freq.toDouble(&ok) / 1000.0;
                if (ok) {
                    itemText = QString("Core %1: Online - %2 MHz").arg(i).arg(freqMHz, 0, 'f', 0);
                } else {
                    itemText = QString("Core %1: Online - %2 (raw)").arg(i).arg(freq);
                }
            } else {
                itemText = QString("Core %1: Online - N/A Freq").arg(i);
            }
            item->setCheckState(Qt::Checked);
        } else {
            itemText = QString("Core %1: Offline").arg(i);
            item->setCheckState(Qt::Unchecked);
        }
        item->setText(itemText);
        ui->cpuCoresListWidget->addItem(item);

        if (i == 0) { // Disable checkbox for Core 0
            item->setFlags(item->flags() & ~Qt::ItemIsUserCheckable);
            // Ensure it remains visually checked if it is online, otherwise unchecked
            item->setCheckState(coreIsOnline ? Qt::Checked : Qt::Unchecked);
        }
    }
}

bool MainWindow::isRunningAsRoot()
{
    return (geteuid() == 0);
}

void MainWindow::applyCpuSettings()
{
    // Part 1: Apply Frequency/Governor settings
    QString selectedText = ui->cpuGovernorComboBox->currentText();
    QString selectedData = ui->cpuGovernorComboBox->currentData().toString(); // This is the value to write (freq or gov name)
    bool isGovernor = selectedText.startsWith("Governor: ");

    int coreCount = getCpuCoreCount();
    if (coreCount == 0) {
        qDebug() << "No CPU cores detected.";
        return;
    }

    for (int i = 0; i < coreCount; ++i) {
        if (!isCoreOnline(i)) {
            qDebug() << "Core" << i << "is offline. Skipping.";
            continue;
        }

        QProcess process;
        QString sysfsPath;
        QString valueToWrite = selectedData;

        if (isGovernor) {
            sysfsPath = QString("/sys/devices/system/cpu/cpu%1/cpufreq/scaling_governor").arg(i);
            qDebug() << "Setting governor for core" << i << ":" << valueToWrite << "to path" << sysfsPath;
        } else {
            // For setting frequency, first attempt to set userspace governor
            QString userspaceGovernor = "userspace";
            QString governorPath = QString("/sys/devices/system/cpu/cpu%1/cpufreq/scaling_governor").arg(i);
            qDebug() << "Attempting to set 'userspace' governor for core" << i << "to path" << governorPath;
            
            QProcess setGovProcess;
            if (isRunningAsRoot()) {
                setGovProcess.start("tee", QStringList() << governorPath);
                setGovProcess.write(userspaceGovernor.toUtf8());
                setGovProcess.closeWriteChannel();
            } else {
                QString cmd = QString("echo %1 > %2").arg(userspaceGovernor).arg(governorPath);
                setGovProcess.start("pkexec", QStringList() << "sh" << "-c" << cmd);
            }

            if (!setGovProcess.waitForStarted()) {
                qDebug() << "Failed to start process for setting userspace governor on core" << i << ":" << setGovProcess.errorString();
                QMessageBox::warning(this, "Operation Failed", QString("Failed to start process for setting 'userspace' governor for Core %1.\n%2").arg(i).arg(setGovProcess.errorString()));
            } else if (!setGovProcess.waitForFinished(-1)) {
                qDebug() << "Process for setting userspace governor on core" << i << "timed out or failed:" << setGovProcess.errorString();
                QMessageBox::warning(this, "Operation Failed", QString("Process for setting 'userspace' governor for Core %1 timed out or failed.\n%2").arg(i).arg(setGovProcess.errorString()));
            } else {
                if (!(setGovProcess.exitStatus() == QProcess::NormalExit && setGovProcess.exitCode() == 0)) {
                    QString errorOutput = setGovProcess.readAllStandardError();
                    QString stdOutput = setGovProcess.readAllStandardOutput();
                    QMessageBox::warning(this, "Operation Failed", QString("Failed to set 'userspace' governor for Core %1.\nError: %2\nOutput: %3").arg(i).arg(errorOutput).arg(stdOutput));
                }
                qDebug() << "Set userspace governor for core" << i << "finished. Output:" << setGovProcess.readAllStandardOutput() << "Error:" << setGovProcess.readAllStandardError();
            }

            // Proceed to set frequency
            sysfsPath = QString("/sys/devices/system/cpu/cpu%1/cpufreq/scaling_setspeed").arg(i);
            qDebug() << "Setting frequency for core" << i << ":" << valueToWrite << "to path" << sysfsPath;
        }
        // process is for the main operation (governor or frequency)
        // setGovProcess was for the preliminary step of setting userspace governor
        
        QProcess mainProcess; // Renamed from 'process' to avoid confusion with setGovProcess scope
        if (isRunningAsRoot()) {
            mainProcess.start("tee", QStringList() << sysfsPath);
            mainProcess.write(valueToWrite.toUtf8());
            mainProcess.closeWriteChannel();
        } else {
            QString cmd = QString("echo %1 > %2").arg(valueToWrite).arg(sysfsPath);
            mainProcess.start("pkexec", QStringList() << "sh" << "-c" << cmd);
        }

        if (!mainProcess.waitForStarted()) {
            qDebug() << "Failed to start main process for core" << i << (isGovernor ? "governor" : "frequency") << ":" << mainProcess.errorString();
            QMessageBox::warning(this, "Operation Failed", QString("Failed to start process for setting %1 for Core %2.\n%3").arg(isGovernor ? "governor" : "frequency").arg(i).arg(mainProcess.errorString()));
            continue; 
        }
        if (!mainProcess.waitForFinished(-1)) {
            qDebug() << "Main process for core" << i << (isGovernor ? "governor" : "frequency") << "timed out or failed:" << mainProcess.errorString();
             QMessageBox::warning(this, "Operation Failed", QString("Process for setting %1 for Core %2 timed out or failed.\n%3").arg(isGovernor ? "governor" : "frequency").arg(i).arg(mainProcess.errorString()));
        } else {
            if (!(mainProcess.exitStatus() == QProcess::NormalExit && mainProcess.exitCode() == 0)) {
                QString errorOutput = mainProcess.readAllStandardError();
                QString stdOutput = mainProcess.readAllStandardOutput();
                QMessageBox::warning(this, "Operation Failed", QString("Failed to set %1 for Core %2.\nError: %3\nOutput: %4").arg(isGovernor ? "governor" : "frequency").arg(i).arg(errorOutput).arg(stdOutput));
            }
            qDebug() << "Core" << i << (isGovernor ? "governor" : "frequency") << "settings applied. Output:" << mainProcess.readAllStandardOutput();
            qDebug() << "Core" << i << (isGovernor ? "governor" : "frequency") << "settings applied. Error:" << mainProcess.readAllStandardError();
        }
    }

    // Part 2: Apply Core Online/Offline settings
    qDebug() << "Starting core online/offline settings application.";
    for (int i = 0; i < ui->cpuCoresListWidget->count(); ++i) {
        QListWidgetItem *item = ui->cpuCoresListWidget->item(i);
        if (!item) continue;

        int coreNum = item->data(Qt::UserRole).toInt();

        if (coreNum == 0) {
            qDebug() << "Skipping Core 0 online/offline state change.";
            continue; // Core 0 cannot be taken offline
        }

        bool shouldBeOnline = (item->checkState() == Qt::Checked);
        bool currentlyOnline = isCoreOnline(coreNum);

        qDebug() << "Core" << coreNum << ": desired state =" << (shouldBeOnline ? "Online" : "Offline")
                 << ", current state =" << (currentlyOnline ? "Online" : "Offline");

        if (shouldBeOnline != currentlyOnline) {
            QString value = shouldBeOnline ? "1" : "0";
            QString path = QString("/sys/devices/system/cpu/cpu%1/online").arg(coreNum);
            qDebug() << "Attempting to set Core" << coreNum << (shouldBeOnline ? "online" : "offline") << ". Writing" << value << "to" << path;

            QProcess coreProcess;
            if (isRunningAsRoot()) {
                coreProcess.start("tee", QStringList() << path);
                coreProcess.write(value.toUtf8());
                coreProcess.closeWriteChannel();
            } else {
                QString cmd = QString("echo %1 > %2").arg(value).arg(path);
                coreProcess.start("pkexec", QStringList() << "sh" << "-c" << cmd);
            }

            if (!coreProcess.waitForStarted()) {
                qDebug() << "Failed to start core state change process for core" << coreNum << ":" << coreProcess.errorString();
                 QMessageBox::warning(this, "Operation Failed", QString("Failed to start process for changing online state of Core %1.\n%2").arg(coreNum).arg(coreProcess.errorString()));
                continue;
            }
            if (!coreProcess.waitForFinished(-1)) {
                qDebug() << "Core state change process timed out or failed for core" << coreNum << ":" << coreProcess.errorString();
                QMessageBox::warning(this, "Operation Failed", QString("Process for changing online state of Core %1 timed out or failed.\n%2").arg(coreNum).arg(coreProcess.errorString()));
            } else {
                if (!(coreProcess.exitStatus() == QProcess::NormalExit && coreProcess.exitCode() == 0)) {
                    QString errorOutput = coreProcess.readAllStandardError();
                    QString stdOutput = coreProcess.readAllStandardOutput();
                    QMessageBox::warning(this, "Operation Failed", QString("Failed to change online state for Core %1.\nError: %2\nOutput: %3").arg(coreNum).arg(errorOutput).arg(stdOutput));
                }
                qDebug() << "Core" << coreNum << "state change process finished. Output:" << coreProcess.readAllStandardOutput();
                qDebug() << "Core" << coreNum << "state change process finished. Error:" << coreProcess.readAllStandardError();
            }
        } else {
            qDebug() << "Core" << coreNum << "is already in the desired state.";
        }
    }

    // Refresh UI with all changes
    loadCpuInformation();
}
// Stray code from a previous merge/replace operation is removed from here.
// The block starting with "continue;" and subsequent lines down to the extra "}"
// were duplicates and syntactically incorrect in this position.
