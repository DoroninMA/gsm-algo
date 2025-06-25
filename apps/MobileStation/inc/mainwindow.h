#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <memory>
#include <thread>

#include <Mobile/MobileStation.h>
#include <Network/UdpSocket.h>

#include "DualStreamBuf.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTimer* _tmr = nullptr;
    std::vector<uint8_t> _kc;
    std::vector<uint8_t> _rand;
    int _cryptAlgoNum = 0;
    int _authId = 0;

    std::vector<uint8_t> _ki;
    MobileIdentity _mobileId;

    boost::asio::io_context _io_context;
    std::shared_ptr<ITransport> _pTransport;
    std::unique_ptr<RadioLink> _pRadioLink;

    std::unique_ptr<MobileStation> _pMobileStation;

    std::thread _ioThread;

    DualStreamBuf _dualStreamBuf;

    bool _isConnected = false;

    void _init();

    void setImsi(const std::string& imsi);
    void setKi(const std::string& ki);

private slots:
    void connectBtnActivated();
    void sendMessage();

    void checkUpdateMobile();
};

#endif // MAINWINDOW_H
