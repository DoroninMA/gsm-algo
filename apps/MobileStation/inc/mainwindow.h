#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>
#include <thread>

#include <Mobile/MobileStation.h>
#include <Network/UdpSocket.h>

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

    std::vector<uint8_t> _ki;
    MobileIdentity _mobileId;

    boost::asio::io_context _io_context;
    std::shared_ptr<ITransport> _pTransport;
    std::unique_ptr<RadioLink> _pRadioLink;

    std::unique_ptr<MobileStation> _pMobileStation;

    std::thread _ioThread;

    bool _isConnected = false;

    void _init();

private slots:
    void connectBtnActivated();
};

#endif // MAINWINDOW_H
