#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"

#include <Network/Bcd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox_2->setVisible(false);

    _init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_init()
{
    _ki.resize(16, 0x00);

    _mobileId.setType(MobileIdentity::Type::IMSI);
    _mobileId.setValue({ Bcd::pack("001010123456789") });


    _pTransport = std::make_shared<UdpSocket>(_io_context, static_cast<uint16_t>(12345));
    _pRadioLink = std::make_unique<RadioLink>(_pTransport);

    _pRadioLink->setReceiveHandler(
        [](std::error_code ec, size_t bytes, const std::vector<uint8_t>& data, const std::string& addr, uint16_t port)
        {
            try {
                if (!ec)
                {
                    std::cout << "Received " << bytes << " bytes from " << addr << ":" << port << "\n";
                }
                else
                {
                    std::cerr << "Receive error: " << ec.message() << "\n";
                }
            }
            catch(const std::exception& e) {
                std::cerr << "Exception in receive handler: " << e.what() << std::endl;
            }
            catch(...) {
                std::cerr << "Unknown exception in receive handler\n";
            }
        });


    _pTransport->asyncReceive();
    _ioThread = std::thread([this]() {
        _io_context.run();
    });

    connect(ui->connectBtn, &QPushButton::clicked, this, &MainWindow::connectBtnActivated);
}

void MainWindow::connectBtnActivated()
{
    if (!_isConnected)
    {
        _pMobileStation = std::make_unique<MobileStation>(*_pRadioLink, _mobileId, _ki);
        ui->connectBtn->setText("");
    }
    else
    {
        _pMobileStation.reset();
    }
}
