#include "exewrapper.h"

#include <QApplication>
#include <QDir>
#include <QProcess>

namespace ExeWrapper
{

static QString getWitPath() {
    return QDir(QApplication::applicationDirPath()).filePath("wit/bin/wit");
}

static const QStringList &getWiimmsEnv() {
    static QStringList witEnv;
    if (witEnv.isEmpty()) {
        witEnv = QProcessEnvironment::systemEnvironment().toStringList();
        // mac is stupid at handling wit, so we have to do this
#ifdef Q_OS_MACOS
        witEnv.append("TERM=xterm-256color");
#endif
    }
    return witEnv;
}

static QFuture<void> observeProcess(QProcess *proc) {
    if (proc->error() == QProcess::FailedToStart) {
        delete proc;
        return QtFuture::makeExceptionalFuture(Exception("could not start new process"));
    }
    return QtFuture::connect(proc, &QProcess::finished)
            .then([=](const std::tuple<int, QProcess::ExitStatus> &codeAndStatus) {
        delete proc;
        if (std::get<0>(codeAndStatus) != 0) {
            throw Exception(QString("process failed, exit code: %1").arg(std::get<0>(codeAndStatus)));
        }
    });
}

QFuture<void> extractGame(const QString &src, const QString &dest) {
    QProcess *proc = new QProcess;
    proc->setEnvironment(getWiimmsEnv());
    proc->start(getWitPath(), {"EXTRACT", "--overwrite", "--", src, dest});
    return observeProcess(proc);
}

QFuture<void> packGame(const QString &src, const QString &dest, bool toFolder) {
    QProcess *proc = new QProcess;
    proc->setEnvironment(getWiimmsEnv());
    QStringList args{"COPY", "--overwrite"};
    if (toFolder) args << "--fst";
    args << "--" << src << dest;
    proc->start(getWitPath(), args);
    return observeProcess(proc);
}

}
