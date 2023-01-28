#include "land.h"
#include "ui_land.h"
#include "control.h"

land::land(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::land)
{
    ui->setupUi(this);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    LoginInit();
    loadcfg();
    connect(ui->userComboBox,SIGNAL(currentTextChanged(QString)),
    ui->passwordLineEdit,SLOT(clear()));

    connect(ui->userComboBox,SIGNAL(currentIndexChanged(int)),
    this,SLOT(setlogint(int)));

}


void land::LoginInit()
 {
    //暗注释
     ui->userComboBox->lineEdit()->setPlaceholderText("请输入用户名");
     ui->userComboBox->lineEdit()->setMaxLength(8);
     ui->passwordLineEdit->setPlaceholderText(QStringLiteral("请输入密码"));
     ui->passwordLineEdit->setMaxLength(8);

     m_userNameList.clear();
     m_passwdList.clear();
     m_isremberList.clear();

     movie = new QMovie(":/background/image/3.gif");
     ui->label_background->setMovie(movie);
     movie->setScaledSize(QSize(800,480));
     movie->start();
}


void land::ReadInit(QString key, QStringList&value)
{
    value.clear();
    QString path = "../SmartHome/db/login.ini";

    //创建配置文件操作对象
    QSettings *config = new QSettings(path, QSettings::IniFormat);

    //读取配置信息

    QVariant variant = config->value(QString("config/") + key);
    value = variant.value<QStringList>();

    delete config;
}


void land::setlogint(int index)
{
    if(m_passwdList.size()>index){
        password = m_passwdList.at(index);

        isrembered = m_isremberList.at(index);
        if(isrembered== "true")
        {
            ui->checkBox->setChecked(true);//勾选记住用户名密码
            ui->passwordLineEdit->setText(password);
        }else{
            ui->checkBox->setChecked(false);
            ui->passwordLineEdit->setText("");
        }
    }
}

void land::WriteInit(QString key, QStringList value)
{
    QString path = "../SmartHome/db/login.ini";

    //创建配置文件操作对象
    QSettings *config = new QSettings(path, QSettings::IniFormat);
    QVariant variant;
    variant.setValue(value);
    //将信息写入配置文件
    config->beginGroup("config");
    config->setValue(key, variant);
    config->endGroup();
    delete config;
}

//载入用户名和密码
void land::loadcfg()
{
    QString ip;
    ReadInit("username",m_userNameList);
    ReadInit("password",m_passwdList);
    ReadInit("isrembered",m_isremberList);

    ui->userComboBox->addItems(m_userNameList);
    if(m_userNameList.size()>0)
        username = m_userNameList.at(0);
    if(m_passwdList.size()>0)
        password = m_passwdList.at(0);
    if(m_isremberList.size()>0)
        isrembered = m_isremberList.at(0);
    if(isrembered == "true")
    {
        ui->checkBox->setChecked(true);
        ui->userComboBox->setCurrentText(username);
        ui->passwordLineEdit->setText(password);
    }
}

land::~land()
{
    delete ui;
    delete  movie;
    m_userNameList.clear();
    m_passwdList.clear();
    m_isremberList.clear();

}


void land::on_pushButton_login_clicked()
{
    /*获取账号和密码*/
    QString username = ui->userComboBox->currentText();
    QString password = ui->passwordLineEdit->text();
    /*判断是否为空*/
    if(username==nullptr||password==nullptr)
    {
        QMessageBox::information(this,"登录认证","请输入正确的账号密码");
    }else
    {
        /*sqlite插入语句*/
        QString sql=QString("select * from userdata where username='%1' and password='%2'")
                .arg(username).arg(password);
        /*创建执行语句对象*/
        QSqlQuery query(sql);
        /*判断执行结果*/
        if(!query.next()){
            QMessageBox::information(this,"登录认证","登录失败,账户或者密码错误");
        }else{
            //QMessageBox::information(this,"登录认证","登录成功");
            if( isrembered =="true"&&m_userNameList.contains(username)==false)
            {
                m_userNameList.push_front(username);
                m_passwdList.push_front(password);
                m_isremberList.push_front(isrembered);

                WriteInit("username",m_userNameList);
                WriteInit("password",m_passwdList);
                WriteInit("isrembered",m_isremberList);
            }
            CONTROL *c=new CONTROL();
            c->show();
            this->close();
        }
    }

}

void land::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()){
          isrembered = "true";
      }else{
          isrembered = "false";
      }
}

void land::on_pushButton_login_p_clicked()
{
    f=new Face(this);
    if(f->facenew())
        f->show();
    else
        f->deleteface();
}

void land::reviecefacedata(bool flag)
{
    if(flag){
        CONTROL *c=new CONTROL();
        c->show();
        this->close();
        f->deleteface();
    }else{
        f->deleteface();
    }
}
