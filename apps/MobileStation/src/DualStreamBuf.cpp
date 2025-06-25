#include "../inc/DualStreamBuf.h"

DualStreamBuf::DualStreamBuf(std::streambuf* originalBuf, QTextEdit* outputWidget)
    : _originalBuf(originalBuf), _outputWidget(outputWidget)
{
}

int DualStreamBuf::overflow(int ch)
{
    if (ch == traits_type::eof())
        return traits_type::not_eof(ch);

    _originalBuf->sputc(ch);

    if (_outputWidget)
    {
        char c = static_cast<char>(ch);
        QMetaObject::invokeMethod(_outputWidget, [=]()
        {
            _outputWidget->moveCursor(QTextCursor::End);
            _outputWidget->insertPlainText(QString(c));
        });
    }

    return ch;
}
