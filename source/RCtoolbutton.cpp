
#include "ncwin.h"

#include "RCtoolbutton.h"
#include "RCView.h"

/**! Implementation of the RCToolButton class
 *
 */
RCToolButton::RCToolButton(QWidget * parent)
    : QToolButton(parent)
{
     //qDebug() << "Entering : RCToolButton::RCTooMenuButton";
     isOn=false;
     isOver=false;
     mToolBox = 0;
     rcparent = 0;
     mcomId = 0;
     mIsSubButton = false;
     mcName = "";
     setMenu(0);
     setDefaultAction(0);
     setCheckable(true);
     setChecked(false);
     setAutoRaise(false);
     setEnabled(true);
     setToolButtonStyle(Qt::ToolButtonIconOnly);
     //setToolButtonStyle(Qt::ToolButtonFollowStyle);
}

RCToolButton::RCToolButton(QString cname, int comid, QWidget * parent)
    : QToolButton(parent)
{
    isOn=false;
    isOver=false;
    mToolBox = 0;
    rcparent = 0;
    mcomId = comid;
    mIsSubButton = false;
    mcName = cname;
    setMenu(0);
    setDefaultAction(0);
    setCheckable(true);
    setChecked(false);
    setAutoRaise(false);
    setEnabled(true);
    setToolButtonStyle(Qt::ToolButtonIconOnly);
    //setToolButtonStyle(Qt::ToolButtonFollowStyle);
}

RCToolButton::~RCToolButton()
{
    //qDebug() << "Entering : RCToolButton::~RCToolMenuButton";
}

bool RCToolButton::event(QEvent * event)
{
    if (event->type() == QEvent::ToolTip)
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        if (toolTip().length() > 0)
        {
            if(commands.implemented(mcomId))
                QToolTip::showText(helpEvent->globalPos(),toolTip(),this,rect());
            else
                QToolTip::showText(helpEvent->globalPos(),toolTip()+rcparent->shortDescription(),this,rect());
        }
        else
        {
            QToolTip::hideText();
            event->ignore();
        }
        return true;
    }
    return QToolButton::event(event);
}

void RCToolButton::enterEvent(QEvent * e )
{
    if(rcparent)
        rcparent->in();
    isOver=true;
    if(rcparent)
    {
        // KMJ: just use the text between the <b></b> tags if any are there
        // ToDo add a short desription to all commands and use that for the prompt
        if(rcparent->Description().startsWith("<b>"))
        {
            QString text = rcparent->Description().mid(3);
            text = text.left(text.indexOf("</b>"));
            cadwindow->prompt->tempprompt(text.data());
        }
        else
            cadwindow->prompt->tempprompt(rcparent->Description().data());
        program->setcommandid(mcomId);
    }
    else
    {
        cadwindow->prompt->shownormalprompt();
        program->setcommandid(0);
    }
    QToolButton::enterEvent(e);
}

void RCToolButton::leaveEvent(QEvent * e)
{
    if(rcparent)
        rcparent->out();
    isOver=false;
    cadwindow->prompt->shownormalprompt();
    program->setcommandid(0);
    QToolButton::leaveEvent(e);
}

void RCToolButton::mouseReleaseEvent ( QMouseEvent * e )
{
    //qDebug() << "Entering : RCToolButton::mouseReleaseEvent";

    //if (e->button() == Qt::LeftButton && mcomId != 0)
    if ((e->button() == Qt::LeftButton || e->button() == Qt::RightButton) && mcomId != 0)
    {
        if(rcparent)
        {
            cadwindow->prompt->tempprompt(rcparent->Description().data());
            program->setcommandid(mcomId);
        }
        NewCommandEvent *nevent = new NewCommandEvent(mcomId,rcparent==0 ? 0 : rcparent->parent);
        state.sendevent(nevent);
        //state.process();
        //app->processEvents();
        // this kicks the Qt event loop
        if(cadwindow != 0 && cadwindow->getcurrentwindow()->gethwnd() != 0)
            ((RCView*)cadwindow->getcurrentwindow()->gethwnd())->updateLabel();

        e->accept();
    }
    else
        e->ignore();

    QToolButton::mouseReleaseEvent(e);

    /*
    qDebug() << "Leaving  : RCToolButton::mouseReleaseEvent";
    if(isChecked())
        qDebug() << "button is checked";
    else
        qDebug() << "button is un checked";
   if(isDown())
       qDebug() << "button is down";
   else
       qDebug() << "button is up";
   */
}

void RCToolButton::mousePressEvent ( QMouseEvent * e )
{
    //QToolButton::mousePressEvent(e);

    QToolButton::mousePressEvent(e);

    /*
    qDebug() << "Leaving  : RCToolButton::mousePressEvent";
    if(isChecked())
        qDebug() << "button is checked";
    else
        qDebug() << "button is un checked";
   if(isDown())
       qDebug() << "button is down";
   else
       qDebug() << "button is up";
   */
}

void RCToolButton::paintEvent(QPaintEvent * event)
{
    if( !rcparent )
        return;
    //return QToolButton::paintEvent(event);

    QRect prect,irect;
    //QRectF fprect,firect;
    irect = QRect(rect().left()+2, rect().top()+2, rect().width()-3, rect().height()-3);
    prect = QRect(rect().left(), rect().top(), rect().width(), rect().height());
    //firect = QRectF(rect().left()+2, rect().top()+2, rect().width()-3, rect().height()-3);
    //fprect = QRectF(rect().left(), rect().top(), rect().width(), rect().height());

    // this is a styles aware version
    if (!isDown() )
    {
        // up state
        QStyleOptionToolButton option;
        option.initFrom(this);
        if(isOver || isChecked())
            option.state = QStyle::State_MouseOver | QStyle::State_Sunken;
        else
            option.state = QStyle::State_Raised;
        option.text = text();
        option.icon = icon();
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing,false);
        style()->drawComplexControl(QStyle::CC_ToolButton, &option, &painter, this);
        //icon().paint(&painter,irect);
        painter.drawPixmap(irect,icon().pixmap(irect.size()));
        //painter.drawPixmap(firect,icon().pixmap(irect.size()),firect);
    }
    else if(isDown())
    {
        // down state
        QStyleOptionToolButton option;
        option.initFrom(this);
        if(isOver || isChecked())
            option.state = QStyle::State_MouseOver | QStyle::State_Sunken;
        else
            option.state = QStyle::State_Sunken;
        option.text = text();
        option.icon = icon();
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing,false);
        style()->drawComplexControl(QStyle::CC_ToolButton, &option, &painter, this);
        //icon().paint(&painter,irect);
        painter.drawPixmap(irect,icon().pixmap(irect.size()));
        //painter.drawPixmap(firect,icon().pixmap(irect.size()),firect);
    }

#if 0
    // this is a non styles aware version
    //QToolButton::paintEvent(event);
    //qDebug() << "Entering : RCToolButton::paintEvent";
    //qDebug() << "RCToolButton::paintEvent : " << this;
    //qDebug() << "RCToolButton::paintEvent : isDown() : " << isDown();

    QToolButton::paintEvent(event);

    //setStyleSheet("border: 1px solid black");
    QPainter p(this);
    p.setRenderHint(QPainter::HighQualityAntialiasing,true);
    QRect prect,irect;
    irect = QRect(rect().left()+2, rect().top()+2, rect().width()-3, rect().height()-3);
    //prect = QRect(rect().left(), rect().top(), rect().width()-1, rect().height()-1);
    prect = QRect(rect().left(), rect().top(), rect().width(), rect().height());
    p.eraseRect(prect);
    //p.eraseRect(irect);
    QPen pen;
    if (isOver && rcparent)
    {
        //qDebug() << "RCToolButton::paintEvent : isOver : ";
        pen = QPen(QBrush(Qt::white),1);
        p.setPen(pen);
        p.drawLine(prect.left(),prect.bottom(),prect.left(),prect.top());
        p.drawLine(prect.left(),prect.top(),prect.right(),prect.top());
        pen = QPen(QBrush(Qt::black),1);
        p.setPen(pen);
        p.drawLine(prect.left(),prect.bottom(),prect.right(),prect.bottom());
        p.drawLine(prect.right(),prect.bottom(),prect.right(),prect.top());
    }
    //if(isOn && rcparent )
    if(isDown() && rcparent )
    {
        //qDebug() << "RCToolButton::paintEvent : isDown : " << mcName;
        pen = QPen(QBrush(Qt::black),1);
        p.setPen(pen);
        p.drawLine(prect.left(),prect.bottom(),prect.left(),prect.top());
        p.drawLine(prect.left(),prect.top(),prect.right(),prect.top());
        pen = QPen(QBrush(Qt::white),1);
        p.setPen(pen);
        p.drawLine(prect.left(),prect.bottom(),prect.right(),prect.bottom());
        p.drawLine(prect.right(),prect.bottom(),prect.right(),prect.top());
    }

    icon().paint(&p,irect);
    //icon().paint(&p,event->rect(),Qt::AlignCenter,isEnabled() ? QIcon::Normal : QIcon::Disabled,isDown() ? QIcon::On : QIcon::Off);
    //qDebug() << "Leaving : RCToolButton::paintEvent";
    //qDebug() << "RCToolButton::paintEvent : " << this;
    //QToolButton::paintEvent(event);
#endif
}


RCStyleButton::RCStyleButton(QWidget * parent)
    : RCToolButton(parent)
    //: QToolButton(parent)
{
    rcparent = 0;
    mcomId = 0;
    mcName = "";
    setCheckable(false);
    setChecked(false);
    setAutoRaise(true);
}

RCStyleButton::RCStyleButton(QString cname, int comid, QWidget * parent)
    : RCToolButton(cname,comid,parent)
    //: QToolButton(parent)
{
    //rcparent = 0;
    //mcomId = comid;
    //mcName = cname;
    //setCheckable(false);
    //setChecked(false);
    //setAutoRaise(true);
}

void RCStyleButton::enterEvent(QEvent * e)
{
    RCToolButton::enterEvent(e);
    //QToolButton::enterEvent(e);
}

void RCStyleButton::leaveEvent(QEvent * e)
{
    RCToolButton::leaveEvent(e);
    //QToolButton::leaveEvent(e);
}

void RCStyleButton::mouseReleaseEvent ( QMouseEvent * e )
{
    RCToolButton::mouseReleaseEvent(e);
    //QToolButton::mouseReleaseEvent(e);
}

void RCStyleButton::mousePressEvent ( QMouseEvent * e )
{
    RCToolButton::mousePressEvent(e);
    //QToolButton::mousePressEvent(e);
}

void RCStyleButton::paintEvent(QPaintEvent * e)
{
    // use this for qt painting
#if 0
    if( !rcparent )
        return;
    QToolButton::paintEvent(e);
#endif
#if 1
    // use this for RCToolButton painting
    RCToolButton::paintEvent(e);
#endif
#if 0
    // customize the style button painting here
    if( !rcparent )
        return;

    QRect prect,irect;
    irect = QRect(rect().left()+2, rect().top()+2, rect().width()-3, rect().height()-3);
    prect = QRect(rect().left(), rect().top(), rect().width(), rect().height());

    if (!isDown())
    {
        // up state
        QStyleOptionToolButton option;
        option.initFrom(this);

        if(isOver)
            option.state = QStyle::State_MouseOver | QStyle::State_Sunken;
        else
            option.state = QStyle::State_Raised;

        option.text = text();
        option.icon = icon();
        QPainter painter(this);
        style()->drawComplexControl(QStyle::CC_ToolButton, &option, &painter, this);
        icon().paint(&painter,irect);
    }
    else if(isDown())
    {
        // down state
        QStyleOptionToolButton option;
        option.initFrom(this);

        if(isOver)
            option.state = QStyle::State_MouseOver | QStyle::State_Sunken;
        else
            option.state = QStyle::State_Sunken;

        option.text = text();
        option.icon = icon();
        QPainter painter(this);
        style()->drawComplexControl(QStyle::CC_ToolButton, &option, &painter, this);
        icon().paint(&painter,irect);
    }
#endif
}
