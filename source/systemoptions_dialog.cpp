
#include "ncwin.h"

#include "RCDialog.h"
#include "systemoptions_dialog.h"
#include "ui_SYSTEMOPTIONS_DIALOG.h"

extern int check_license(char *label,char* label1, char *label2,int application,int showmessages);


SystemOptions_Dialog::SystemOptions_Dialog(HANDLE parent, HANDLE rcparent, int f):
	SystemOptions_Dialog((QWidget*)parent, (Dialog*)rcparent, (Qt::WindowFlags) f)
{}
SystemOptions_Dialog::SystemOptions_Dialog(QWidget *parent,Dialog *rcparent, Qt::WindowFlags f) :
    RCDialog(rcparent,parent,f),
    ui(new Ui::SystemOptions_Dialog)
{
  ui->setupUi(this);
  ActivateParent = false;
  initDialog(this);

  // beta pending controls
  // disable these check boxes for now
  ui->_132->setEnabled(false);  
  ui->_132->hide();

  // show welcome screen check box should be disabled if software is not licensed
  int licensed = check_license((char*)"Main CAD Application", (char*)"RealCADKey1", (char*)"RealCADKey2",0,0);
  ui->_139->setEnabled((licensed == 0 ? false : true));

  connect(ui->_1,  SIGNAL(clicked(bool)),  this,  SLOT(on_accept()));
  connect(ui->_2,  SIGNAL(clicked(bool)),  this,  SLOT(reject()));
}

SystemOptions_Dialog::~SystemOptions_Dialog()
{
    delete ui;
}

void SystemOptions_Dialog::on__106_valueChanged(double v)
{
    ::v.setreal("wn::backgroundcolour.red",v);
    cadwindow->setbackground(RGB(v,ui->_107->value(),ui->_108->value()));
    cadwindow->paintall();
}

void SystemOptions_Dialog::on__107_valueChanged(double v)
{
    ::v.setreal("wn::backgroundcolour.green",v);
    cadwindow->setbackground(RGB(ui->_106->value(),v,ui->_108->value()));
    cadwindow->paintall();
}

void SystemOptions_Dialog::on__108_valueChanged(double v)
{
    ::v.setreal("wn::backgroundcolour.blue",v);
    cadwindow->setbackground(RGB(ui->_106->value(),ui->_107->value(),v));
    cadwindow->paintall();
}

void SystemOptions_Dialog::on__1000_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1000
    wParam = MAKEWPARAM((WORD)1000,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1000;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void SystemOptions_Dialog::on__150_valueChanged(double v)
{
    ::v.setreal("db::selectcolour.red",v);
}

void SystemOptions_Dialog::on__151_valueChanged(double v)
{
    ::v.setreal("db::selectcolour.green",v);
}

void SystemOptions_Dialog::on__152_valueChanged(double v)
{
    ::v.setreal("db::selectcolour.blue",v);
}

void SystemOptions_Dialog::on__1001_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 1001
    wParam = MAKEWPARAM((WORD)1001,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_1001;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void SystemOptions_Dialog::on__110_clicked(bool checked)
{
    /*
     *  beta pending
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 110
    wParam = MAKEWPARAM((WORD)110,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_110;
    dialogcb((HWND)this,msg,wParam,lParam);
    */
}

void SystemOptions_Dialog::on__111_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 111
    wParam = MAKEWPARAM((WORD)111,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_111;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void SystemOptions_Dialog::on__118_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 118
    wParam = MAKEWPARAM((WORD)118,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_118;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void SystemOptions_Dialog::on__120_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 120
    wParam = MAKEWPARAM((WORD)120,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_120;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void SystemOptions_Dialog::on__122_clicked(bool checked)
{
    /*
     * // beta pending
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 122
    wParam = MAKEWPARAM((WORD)122,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_122;
    dialogcb((HWND)this,msg,wParam,lParam);
    */
}

void SystemOptions_Dialog::on__129_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 129
    wParam = MAKEWPARAM((WORD)129,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_129;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void SystemOptions_Dialog::on__124_clicked(bool checked)
{
    /*
     * beta pending
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 124
    wParam = MAKEWPARAM((WORD)124,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_124;
    dialogcb((HWND)this,msg,wParam,lParam);
    */
}

void SystemOptions_Dialog::on__125_clicked(bool checked)
{
    // using the dialog callback function
    /*
     * beta pending
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 125
    wParam = MAKEWPARAM((WORD)125,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_125;
    dialogcb((HWND)this,msg,wParam,lParam);
    */
}

void SystemOptions_Dialog::on__126_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 126
    wParam = MAKEWPARAM((WORD)126,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_126;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void SystemOptions_Dialog::on__132_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 132
    wParam = MAKEWPARAM((WORD)132,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_132;
    dialogcb((HWND)this,msg,wParam,lParam);

}

void SystemOptions_Dialog::on__133_clicked(bool checked)
{
    /*
     *  beta pending
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 133
    wParam = MAKEWPARAM((WORD)133,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_133;
    dialogcb((HWND)this,msg,wParam,lParam);
    */
}

void SystemOptions_Dialog::on__134_clicked(bool checked)
{
    /*
     * beta pending
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 134
    wParam = MAKEWPARAM((WORD)134,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_134;
    dialogcb((HWND)this,msg,wParam,lParam);
    */
}

void SystemOptions_Dialog::on__135_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 135
    wParam = MAKEWPARAM((WORD)135,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_135;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void SystemOptions_Dialog::on__136_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 136
    wParam = MAKEWPARAM((WORD)136,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_136;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void SystemOptions_Dialog::on__138_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 138
    wParam = MAKEWPARAM((WORD)138,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_138;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void SystemOptions_Dialog::on__139_clicked(bool checked)
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 139
    wParam = MAKEWPARAM((WORD)139,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_139;
    dialogcb((HWND)this,msg,wParam,lParam);
}

void SystemOptions_Dialog::on__140_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDOK,(WORD)0);
    lParam = (LPARAM)ui->_1;
    dialogcb((HWND)this,msg,wParam,lParam);

    done(140);
}

void SystemOptions_Dialog::on_accept()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click OK
    wParam = MAKEWPARAM((WORD)IDOK,(WORD)0);
    lParam = (LPARAM)ui->_1;
    dialogcb((HWND)this,msg,wParam,lParam);

    QDialog::accept();
}

void SystemOptions_Dialog::on__143_clicked()
{
    // using the dialog callback function
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;

    msg = WM_COMMAND;

    // click 143
    wParam = MAKEWPARAM((WORD)143,(WORD)BN_CLICKED);
    lParam = (LPARAM)ui->_143;
    dialogcb((HWND)this,msg,wParam,lParam);

}
