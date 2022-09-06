#ifndef EXEWRAPPER_H
#define EXEWRAPPER_H

#include <QFuture>

namespace ExeWrapper {

QFuture<void> extractGame(const QString &src, const QString &dest);
QFuture<void> packGame(const QString &src, const QString &dest, bool toFolder);

class Exception : public QException, public std::runtime_error {
    public:
        const char *what() const noexcept override { return std::runtime_error::what(); }
        using std::runtime_error::runtime_error;
        Exception(const QString &str) : std::runtime_error(str.toStdString()) {}
        void raise() const override { throw *this; }
        Exception *clone() const override { return new Exception(*this); }
    };

}

#endif // EXEWRAPPER_H
