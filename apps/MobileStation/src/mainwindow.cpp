#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"

#include <Network/Bcd.h>

#include <GsmCrypto/Auth/AuthComp1281.h>

#include <IOUtils/Utils.h>

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
    _dualStreamBuf = DualStreamBuf(std::cout.rdbuf(), ui->messageTextEdit);
    std::cout.rdbuf(&_dualStreamBuf);

    _mobileId.setType(MobileIdentity::Type::IMSI);
    setImsi("001010123456789");
    setKi("");

    auto socket = std::make_shared<UdpSocket>(_io_context, static_cast<uint16_t>(12344));
    socket->setRemote("127.0.0.1", 12345);
    _pTransport = std::move(socket);
    _pRadioLink = std::make_unique<RadioLink>(_pTransport);


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


    _tmr = new QTimer(this);
    connect(_tmr, &QTimer::timeout, this, &MainWindow::checkUpdateMobile);
    _tmr->start(50);
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
            ui->messageTextEdit->append("Отправлено: " + message + "\n");
            ui->sendLineEdit->clear();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what();

        }
    }
}

void MainWindow::checkUpdateMobile()
{
    if (_pMobileStation)
    {
        if (_pMobileStation->kc() != _kc)
        {
            _kc = _pMobileStation->kc();
            ui->kcLabelData->setText(QString::fromStdString(_bytesToHexString(_kc.data(), _kc.size())));
        }

        if (_pMobileStation->rand() != _rand)
        {
            _rand = _pMobileStation->rand();
            ui->randLabelData->setText(QString::fromStdString(_bytesToHexString(_rand.data(), _rand.size())));
        }

        if (_pMobileStation->alogId() != _cryptAlgoNum)
        {
            _cryptAlgoNum = _pMobileStation->alogId();
            if (_cryptAlgoNum == 0)
            {
                ui->cryptoLabelData->setText("");
            }
            else
            {
                ui->cryptoLabelData->setText(QString::number(_cryptAlgoNum));
            }
        }

        if (_pMobileStation->authId() != _authId)
        {
            _authId = _pMobileStation->authId();
            if (_authId == 0)
            {
                ui->authDataLabel->setText("");
            }
            else
            {
                ui->authDataLabel->setText(QString::number(_authId));
            }
        }
    }
}
