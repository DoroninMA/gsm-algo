#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"

#include <Network/Bcd.h>

#include <GsmCrypto/Auth/AuthComp1281.h>

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
    _mobileId.setType(MobileIdentity::Type::IMSI);
    setImsi("001010123456789");
    setKi("");

    auto socket = std::make_shared<UdpSocket>(_io_context, static_cast<uint16_t>(12344));
    socket->setRemote("127.0.0.1", 12345);
    _pTransport = std::move(socket);
    _pRadioLink = std::make_unique<RadioLink>(_pTransport);

    // _pRadioLink->setReceiveHandler(
    //     [](std::error_code ec, size_t bytes, const std::vector<uint8_t>& data, const std::string& addr, uint16_t port)
    //     {
    //         try {
    //             if (!ec)
    //             {
    //                 std::cout << "Received " << bytes << " bytes from " << addr << ":" << port << "\n";
    //             }
    //             else
    //             {
    //                 std::cerr << "Receive error: " << ec.message() << "\n";
    //             }
    //         }
    //         catch(const std::exception& e) {
    //             std::cerr << "Exception in receive handler: " << e.what() << std::endl;
    //         }
    //         catch(...) {
    //             std::cerr << "Unknown exception in receive handler\n";
    //         }
    //     });


    _pTransport->asyncReceive();
    _ioThread = std::thread([this]() {
        _io_context.run();
    });

    connect(ui->connectBtn, &QPushButton::clicked, this, &MainWindow::connectBtnActivated);
    connect(ui->sendLineEdit, &QLineEdit::returnPressed, this, &MainWindow::sendMessage);

    connect(ui->kiLineEdit, &QLineEdit::returnPressed, this, [this](){
        this->setKi(ui->kiLineEdit->text().toStdString());
    });

    connect(ui->identityLabelData, &QLineEdit::returnPressed, this, [this](){
        this->setImsi(ui->identityLabelData->text().toStdString());
    });
}

void MainWindow::setImsi(const std::string &imsi)
{
    try
    {
        _mobileId.setValue(Bcd::pack(imsi));
        ui->identityLabelData->setText(QString::fromStdString(imsi));
        std::cout << "IMSI updated: " << imsi << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void MainWindow::setKi(const std::string &ki)
{
    if (ki.size() != 16)
    {
        std::cerr << "incorrect ki size: " << ki << std::endl;

        setKi("0000000000000000");
        return;
    }

    try
    {
        _ki = Bcd::pack(ki);
        ui->kiLineEdit->setText(QString::fromStdString(ki));
        std::cout << "Ki updated: " << ki << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what();
    }
}

void MainWindow::connectBtnActivated()
{
    if (!_isConnected)
    {
        _pMobileStation = std::make_unique<MobileStation>(*_pRadioLink, _mobileId, _ki);
        _pMobileStation->setAuthGenerator(std::make_unique<Comp1281>());

        // set LAI
        std::vector<uint8_t> lai(5);
        std::vector<uint8_t> imsi = _mobileId.pack();
        std::copy(imsi.begin(), imsi.begin() + 3, lai.begin());
        lai[3] = 0x12;
        lai[4] = 0x42;
        _pMobileStation->setLai(lai);

        try
        {
            _pMobileStation->connectToBts();
            ui->connectBtn->setText("Отключиться");

            _isConnected = !_isConnected;
        }
        catch(const std::exception& e)
        {
            _pMobileStation.reset();
            std::cerr << e.what() << std::endl;
        }
    }
    else
    {
        ui->connectBtn->setText("Подключиться");
        _pMobileStation.reset();

        _isConnected = !_isConnected;
    }
}

void MainWindow::sendMessage()
{
    if ((_pMobileStation == nullptr)  || _pMobileStation->state() != MobileStation::State::IN_CALL)
    {
        ui->messageTextEdit->append("Ошибка отправки (соединение не установлено)");
        std::cerr << "sendMessage error: not in call\n";
        return;
    }

    const QString& message = ui->sendLineEdit->text();
    if (!message.isEmpty())
    {
        try
        {
            _pMobileStation->sendVoiceData(message.toStdString());
            ui->messageTextEdit->append("Отправлено: " + message);
            ui->sendLineEdit->clear();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what();

        }
    }
}
