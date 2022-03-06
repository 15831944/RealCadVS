
#include "ncwin.h"

#include "rcdoublespinbox.h"
#include "RCDialog.h"

//RCDialog::RCDialog(Dialog *rcdlg)
RCDialog::RCDialog(Dialog *rcdlg, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent,f)
{
    RCdlg = rcdlg;
    RCdlg->sethdlg(this);
    setFocusPolicy(Qt::StrongFocus);
	ActivateParent = true;
}

RCDialog::~RCDialog()
{
    RCdlg = 0;
}

void RCDialog::focusInEvent(QFocusEvent * event)
{
    qDebug() << "in dialog focus in event ";
    qDebug() << "focus reason : " << event->reason();
    QDialog::focusInEvent(event);
}

void RCDialog::focusOutEvent(QFocusEvent * event)
{
    qDebug() << "in dialog focus out event ";
    qDebug() << "focus reason : " << event->reason();

    if(app->mouseButtons() == Qt::LeftButton)
        qDebug() << "mousebutton : left button";
    else if(app->mouseButtons() == Qt::RightButton)
        qDebug() << "mousebutton : right button";
    else if(app->mouseButtons() == Qt::MiddleButton)
        qDebug() << "mousebutton : right button";
    else
        qDebug() << "mousebutton : ???";

    /*
    if(event->reason() != Qt::TabFocusReason  && event->reason() != Qt::BacktabFocusReason )
    {
        if(!underMouse())
            if(v.getinteger("wn::dialogboxautook"))
                close();
    }
    */
    QDialog::focusOutEvent(event);
}

QWidget* RCDialog::getControl(int id)
{
    return getControl(RCintToName(id));
}

QWidget* RCDialog::getControl(QString name)
{
    return findChild<QWidget *>(name);
}

void RCDialog::setRCDialog(Dialog *rcdlg)
{
    RCdlg = rcdlg;
}

Dialog* RCDialog::rcDialog()
{
    return RCdlg;
}

void RCDialog::accept()
{
    QDialog::accept();
}

void RCDialog::reject()
{
    QDialog::reject();
}

void RCDialog::initDialog(QWidget *qdlg)
{
    qdlg->setWindowFlag(Qt::WindowContextHelpButtonHint,false);

    // this code calls the Dialog object directly
    DialogControl *dc,*lastdc;
    CustomDialogControl *cdc,*lastcdc;
    DisplayDialogControl *ddc;
    int id;
    GeneralList list;

    RCdlg->hDlg = qdlg;

    // put the widget pointers into the controls
    for (RCdlg->controllist.start() ; (dc = (DialogControl *)RCdlg->controllist.next()) != NULL ; )
    {
        //qDebug() << "RCDialog setting control : " << dc->getid() << " to : " << findChild<QWidget*>(RCintToName(dc->getid()));
        dc->setQW(findChild<QWidget*>(RCintToName(dc->getid())));

        if(qobject_cast<RCDoubleSpinBox *>(dc->getQW()) != 0)
            ((RCDoubleSpinBox*)dc->getQW())->defaultValue = ((RealDialogControl*)dc)->getDefaultValue();
    }

    // set the decimal places depending on the current option setting
    // also se the default values
    QList<QDoubleSpinBox *> DbSpinners = qdlg->findChildren<QDoubleSpinBox *>();
    for(int i=0; i<DbSpinners.size(); i++)
    {
        DbSpinners.at(i)->setDecimals(db.getnumericdisplaytolerance());
    }

    for (RCdlg->controllist.start() ; (dc = (DialogControl *)RCdlg->controllist.next()) != NULL ; )
    {
         ShowWindow(GetDlgItem(RCdlg->hDlg,dc->getid()),SW_SHOWNORMAL);
         EnableWindow(GetDlgItem(RCdlg->hDlg,dc->getid()),TRUE);
    }

    for (RCdlg->customlist.start() ; (cdc = (CustomDialogControl *)RCdlg->customlist.next()) != NULL ; )
    {
         ShowWindow(GetDlgItem(RCdlg->hDlg,cdc->getid()),SW_SHOWNORMAL);
         EnableWindow(GetDlgItem(RCdlg->hDlg,cdc->getid()),TRUE);
    }

    for (RCdlg->disablelist.start() ; (id = (LONG) RCdlg->disablelist.next()) != 0 ; )
      EnableWindow(GetDlgItem(RCdlg->hDlg,id),FALSE);

    list = RCdlg->controllist;
    for (list.start(),lastdc = NULL ; (dc = (DialogControl *)list.next()) != 0 ; lastdc = dc)
      dc->load(RCdlg);

    /*
    for (RCdlg->customlist.start() ; (cdc = (CustomDialogControl *)RCdlg->customlist.next()) != NULL ; )
    {
        cdc->sethwnd(GetDlgItem(RCdlg,cdc->getid()));
        cdc->draw(RCdlg,0);
    }
*/

    for (RCdlg->displaylist.start() ; (ddc = (DisplayDialogControl *)RCdlg->displaylist.next()) != NULL ; )
    {
         //ddc->sethwnd(GetDlgItem(RCdlg,ddc->getid()));
         ddc->draw(RCdlg,0);
    }


    if (strlen(RCdlg->dtitle) > 0)
      SetWindowText(RCdlg->hDlg,RCdlg->dtitle);
    if (strlen(RCdlg->dlabel) > 0)
      SetDlgItemText(RCdlg->hDlg,100,RCdlg->dlabel);

    if (lastdc != NULL && (lastdc->type() == RealControl || lastdc->type() == IntegerControl ||
                           lastdc->type() == ListControl || lastdc->type() == StringControl))
    {
        lastdc->selectitem(RCdlg);
    }

}
