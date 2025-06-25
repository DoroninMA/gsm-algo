#ifndef DUALSTREAMBUF_H
#define DUALSTREAMBUF_H

#include <streambuf>
#include <iostream>
#include <sstream>
#include <QTextEdit>
#include <QString>
#include <QMetaObject>
#include <qt6/QtCore/qobjectdefs.h>

class DualStreamBuf : public std::streambuf
{
public:
    DualStreamBuf() = default;
    DualStreamBuf(std::streambuf* originalBuf, QTextEdit* outputWidget);

protected:
    int overflow(int ch) override;

private:
    std::streambuf* _originalBuf;
    QTextEdit* _outputWidget;
};

#endif
