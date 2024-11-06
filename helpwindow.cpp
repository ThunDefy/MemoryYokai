#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(this->width(),this->height());
    this->setMinimumSize(this->width(),this->height());

    connect(ui->nextButton, SIGNAL(clicked()), ui->textBrowser, SLOT(forward()));
    connect(ui->backButton, SIGNAL(clicked()), ui->textBrowser, SLOT(backward()));
    connect(ui->homeButton, SIGNAL(clicked()), ui->textBrowser, SLOT(home()));
    connect(ui->textBrowser, SIGNAL(forwardAvailable(bool)), ui->nextButton, SLOT(setEnabled(bool)));
    connect(ui->textBrowser, SIGNAL(backwardAvailable(bool)), ui->backButton, SLOT(setEnabled(bool)));
    ui->textBrowser->setSource(QUrl::fromLocalFile("../MemoryYokai/src/Help/index.html"));
}

HelpWindow::~HelpWindow()
{
    delete ui;
}
