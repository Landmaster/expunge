#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "exewrapper.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openFolder, &QPushButton::clicked, this, [this](bool) {
        auto dir = QFileDialog::getExistingDirectory(this, "Open Game Directory");
        if (dir.isEmpty()) {
            return;
        }
        importGame(dir);
    });
    connect(ui->openWbfsIso, &QPushButton::clicked, this, [this](bool) {
        auto wbfsIso = QFileDialog::getOpenFileName(this, "Open WBFS/ISO", QString(), "Fortune Street disc files (*.wbfs *.iso *.ciso)");
        if (wbfsIso.isEmpty()) {
            return;
        }
        importGame(wbfsIso);
    });
    connect(ui->openExtractedFiles, &QPushButton::clicked, this, [this](bool) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(tempGameDir->path()));
    });
    connect(ui->exportFolder, &QPushButton::clicked, this, [this](bool) {
        auto dir = QFileDialog::getExistingDirectory(this, "Save Game Directory");
        if (dir.isEmpty()) {
            return;
        }
        if (!QDir(dir).isEmpty()) {
            QMessageBox::critical(this, "Error saving game directory", "Directory is non-empty");
            return;
        }
        exportGame(dir, true);
    });
    connect(ui->exportWbfsIso, &QPushButton::clicked, this, [this](bool) {
        auto wbfsIso = QFileDialog::getSaveFileName(this, "Export WBFS/ISO", QString(), "Fortune Street disc files (*.wbfs *.iso *.ciso)");
        if (wbfsIso.isEmpty()) {
            return;
        }
        exportGame(wbfsIso, false);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::importGame(const QString &path) {
    auto newTempGameDir = QSharedPointer<QTemporaryDir>::create();
    if (!newTempGameDir->isValid()) {
        QMessageBox::critical(this, "Error creating temporary directory",
                              "Could not create temporary directory: " + newTempGameDir->errorString());
        return;
    }
    ui->openProgress->setText("Importing…");
    ExeWrapper::extractGame(path, newTempGameDir->path()).then([=] {
        ui->openProgress->setText("Imported");
        tempGameDir = newTempGameDir;
        QDesktopServices::openUrl(QUrl::fromLocalFile(tempGameDir->path()));
        ui->openExtractedFiles->setEnabled(true);
        ui->exportFolder->setEnabled(true);
        ui->exportWbfsIso->setEnabled(true);
    }).onFailed([this](const std::runtime_error &ex) {
        ui->openProgress->setText("Failed to import");
        QMessageBox::critical(this, "Error copying game to temporary directory",
                              "Could not copy game to temporary directory: " + QString(ex.what()));
    });
}

void MainWindow::exportGame(const QString &path, bool isDirectory)
{
    ui->exportProgress->setText("Exporting…");
    ExeWrapper::packGame(tempGameDir->path(), path, isDirectory).then([this] {
        ui->exportProgress->setText("Exported");
        QMessageBox::information(this, "Export successful", "Successfully exported the game.");
    }).onFailed([this](const std::runtime_error &ex) {
        ui->openProgress->setText("Failed to export");
        QMessageBox::critical(this, "Error exporting game", "Failed to export game: " + QString(ex.what()));
    });
}
