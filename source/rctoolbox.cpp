
#include "ncwin.h"
#include <QDebug.h>
#include "rcview.h"
#include "rcgraphicsview.h"
#include "rctoolbox.h"
#include "RCImageResources.h"

#define SBUTTON_SIZE   32
#define MBUTTON_SIZE   52
#define LBUTTON_SIZE   62
#define DBUTTON_SIZE   SBUTTON_SIZE
#define DBUTTON_MARGIN 0
#define DMIDDLE_GAP    10

RCToolBox::RCToolBox(QWidget *parent)
    : QDockWidget(parent)
{
    BUTTON_SIZE = DBUTTON_SIZE;
    switch (v.getinteger("wn::menusize")) {
    case 0:
    default:
        BUTTON_SIZE = SBUTTON_SIZE;
        break;
    case 1:
        BUTTON_SIZE = MBUTTON_SIZE;
        break;
    case 2:
        BUTTON_SIZE = LBUTTON_SIZE;
        break;
    }
    BUTTON_MARGIN = DBUTTON_MARGIN;
    MIDDLE_GAP = DMIDDLE_GAP;

    mButtonMenu=0;
    lastMainButton=0;
    lastSubButton=0;
    dockWidgetContents=0;
    gridLayoutWidgetMain=0;
    gridLayoutMain=0;
    gridLayoutWidgetSub=0;
    gridLayoutSub=0;
    mnextId=0;
    mnextSid=0;
    mArea = Qt::NoDockWidgetArea;

    setVerticalTitleBar(false);
    setStyleSheet("titlebar-close-icon: url(:/images/closeview-16.png)");
    mnMainButtons=1;
    mnSubButtons=0;
    lType=hLine;
    mInitlType=lType;
    mInitCols=1;
    mnCols=1;
    mnRows=1;
    mAppPath.clear();

    if(parent) // should be the owning window
    {
        QList<QDockWidget *> allDockWidgets = parent->findChildren<QDockWidget *>();
        setObjectName("DockWidget"+QString("%1").arg(allDockWidgets.size()));
    }
    else
        setObjectName("DockWidget"+QString("%1").arg((long)this));


    qDebug() << "object name: " << objectName();

    setup(lType,mnCols,mnMainButtons);

    connect(this,SIGNAL(topLevelChanged(bool)),SLOT(doTopLevelChanged(bool)));
    connect(this,SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),SLOT(doDockLocationChanged(Qt::DockWidgetArea)));
    connect(this,SIGNAL(visibilityChanged(bool)),SLOT(onVisibilityChanged(bool)));
}

RCToolBox::RCToolBox(layoutType type, int nCols, int nButtons, int nSubButtons, QWidget *parent )
    : QDockWidget( parent)
{
    BUTTON_SIZE = DBUTTON_SIZE;
    switch (v.getinteger("wn::menusize")) {
    case 0:
    default:
        BUTTON_SIZE = SBUTTON_SIZE;
        break;
    case 1:
        BUTTON_SIZE = MBUTTON_SIZE;
        break;
    case 2:
        BUTTON_SIZE = LBUTTON_SIZE;
        break;
    }
    BUTTON_MARGIN = DBUTTON_MARGIN;
    MIDDLE_GAP = DMIDDLE_GAP;

    mButtonMenu=0;
    lastMainButton=0;
    lastSubButton=0;
    lastButton=0;
    previousButton=0;
    dockWidgetContents=0;
    gridLayoutWidgetMain=0;
    gridLayoutMain=0;
    gridLayoutWidgetSub=0;
    gridLayoutSub=0;
    mnextId=0;
    mnextSid=0;
    mArea = Qt::NoDockWidgetArea;

    setVerticalTitleBar(false);
    //setStyleSheet("titlebar-close-icon: url(:/images/closeview-16.png)");
    mnMainButtons=0;
    mnSubButtons=0;
    lType=type;
    mInitlType=lType;
    mInitCols=nCols;
    mnCols=1;
    mnRows=1;
    mAppPath.clear();

    if(parent) // should be the owning window
    {
        QList<QDockWidget *> allDockWidgets = parent->findChildren<QDockWidget *>();
        setObjectName("DockWidget"+QString("%1").arg(allDockWidgets.size()));
    }
    else
        setObjectName("DockWidget"+QString("%1").arg((long)this));

    qDebug() << "object name: " << objectName();

    setup(type, nCols, nButtons, nSubButtons);   

    connect(this,SIGNAL(topLevelChanged(bool)),SLOT(doTopLevelChanged(bool)));
    connect(this,SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),SLOT(doDockLocationChanged(Qt::DockWidgetArea)));
    connect(this,SIGNAL(visibilityChanged(bool)),SLOT(onVisibilityChanged(bool)));
}

RCToolBox::~RCToolBox()
{
    saveState();

    QMutableListIterator<QToolButton*> it1(mainButtons);
    while (it1.hasNext()) {
        delete it1.next();
    }

    if(mainButtons.size() > 0)
        mainButtons.clear();

    QMutableListIterator<QToolButton*> it2(subButtons);
    while (it2.hasNext()) {
        delete it2.next();
    }

    if(subButtons.size() > 0)
        subButtons.clear();
}

void RCToolBox::resizeEvent(QResizeEvent *e)
{
    //qDebug() << "In RCToolBox::resizeEvent";
    saveState();
    QDockWidget::resizeEvent(e);
}

void RCToolBox::moveEvent(QMoveEvent *e)
{
    //qDebug() << "In RCToolBox::moveEvent";
    saveState();
    QDockWidget::moveEvent(e);
}


void RCToolBox::enterEvent(QEvent *e)
{
    if(mButtonMenu)
        mButtonMenu->setOver(1);
    QDockWidget::enterEvent(e);
}

extern void graphicsCursorDrawer();

void RCToolBox::leaveEvent(QEvent *e)
{
    if(mButtonMenu)
        mButtonMenu->setOver(0);

    QDockWidget::leaveEvent(e);

    program->drawGraphicsCursor();

    QTimer::singleShot(200,graphicsCursorDrawer);
}

void RCToolBox::changeEvent(QEvent *e)
{
    //qDebug() << "In RCToolBox::changeEvent";

    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
    QDockWidget::changeEvent(e);

}

void RCToolBox::closeEvent(QCloseEvent *event)
{
    ButtonMenu *bm = buttonmenus.match(mRole.data());
    //buttonmenus.deletemenu(bm);
    if(bm != 0)
    {
        RCHMODULE instance = program->getloadedapp(mTitle.toLower().data());
        if(instance)
            mAppPath = ((QLibrary*)instance)->fileName();
        program->unloadapp(mTitle.data());
    }
    else
    {
        bm = buttonmenus.match(mTitle.data());
        if(bm != 0)
            cadwindow->delcontrol(bm);
    }
    setVisible(false);
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
    settings.remove("Layout/"+mRole);
    event->accept();
}

void RCToolBox::doTopLevelChanged(bool how)
{
    //qDebug() << "In RCToolBox::doTopLevelChanged";
    //qDebug() << "In RCToolBox::doTopLevelChanged : how : " << how;
     if( how )
    {
        // we have changed state to floated so do something interesting
        //qDebug() << "In RCToolBox::doTopLevelChanged : changed to floating : ";
        mArea = Qt::NoDockWidgetArea;
        saveState();
    }
    else
    {
        // we have changed state to docked so do something interesting
        //qDebug() << "In RCToolBox::doTopLevelChanged : changed to docked : ";
    }
}

void RCToolBox::doDockLocationChanged(Qt::DockWidgetArea area)
{
    //qDebug() << "In RCToolBox::doDockLocationChanged";
    //qDebug() << "In RCToolBox::doDockLocationChanged : area : " << area;

    mArea = area;

    QString lastMainButtonName, lastSubButtonName;
    //QString lastButtonName;
    if(lastMainButton)
        lastMainButtonName =((RCToolButton*)lastMainButton)->mcName;
    if(lastSubButton)
        lastSubButtonName = ((RCToolButton*)lastSubButton)->mcName;
    //if(lastButton)
    //    lastButtonName = ((RCToolButton*)lastButton)->mcName;

    bool lastMainButtonChecked = false;
    bool lastSubButtonChecked = false;

    if(lastMainButton)
        lastMainButtonChecked = lastMainButton->isChecked();

    if(lastSubButton)
        lastSubButtonChecked = lastSubButton->isChecked();

    lastMainButton = 0;
    lastSubButton = 0;
    //lastButton = 0;

    if(area == Qt::TopDockWidgetArea || area == Qt::BottomDockWidgetArea )
        if(lType == vBlock || lType == hBlock)
            resetLayout(hBlock);
        else if(lType == vLine || lType == hLine)
            resetLayout(hLine);
    if(area == Qt::LeftDockWidgetArea || area == Qt::RightDockWidgetArea)
        if(lType == vBlock || lType == hBlock)
            resetLayout(vBlock);
        else if(lType == vLine || lType == hLine)
            resetLayout(vLine);

     // match the last used buttons to the new ones
    QMutableListIterator<QToolButton*> itm(mainButtons);
    while (itm.hasNext())
    {
        RCToolButton *button = (RCToolButton*)itm.next();
        if(button)
        {
            if(!button->mcName.isEmpty() && button->mcName == lastMainButtonName)
            {
                lastMainButton = button;
                button->setChecked(lastMainButtonChecked);
                button->update();
            }
        }
    }

    QMutableListIterator<QToolButton*> its(subButtons);
    while (its.hasNext())
    {
        RCToolButton *button = (RCToolButton*)its.next();
        if(button)
        {
            if(!button->mcName.isEmpty() && button->mcName == lastSubButtonName)
            {
                lastSubButton = button;
                button->setChecked(lastSubButtonChecked);
                button->update();
            }
        }
    }

    saveState();
    update();

}

void RCToolBox::resetLayout(layoutType type)
{
    QList<QToolButton*> mainCopy,subCopy;
    getButtons(&mainCopy, &subCopy);

    if(type == vLine)
    {
        int ncols = ceil(mnMainButtons / (double)mInitCols);
        if(mInitCols == 1)
            ncols = mInitCols;
        if(mButtonMenu != 0)
            mButtonMenu->setorientation(1);
        setup(type,ncols,mnMainButtons,mnSubButtons);
    }
    else if(type == vBlock || type == hBlock)
        setup(type,3,mnMainButtons,mnSubButtons);
    else if(type == hLine)
    {
        int ncols = mInitCols;
        if(mInitCols == 1)
            ncols = ceil(mnMainButtons / (double)mInitCols);
        if(mButtonMenu != 0)
            mButtonMenu->setorientation(2);
        setup(type,ncols,mnMainButtons,mnSubButtons);
    }

    addButtons(&mainCopy, &subCopy);

    saveState();
}

void RCToolBox::paintEvent(QPaintEvent *e)
{
    //qDebug() << "In RCToolBox::paintEvent";
    QDockWidget::paintEvent(e);
}

void RCToolBox::getButtons(QList<QToolButton*> * mainCopy , QList<QToolButton*> * subCopy)
{
    if(mainCopy != 0)
    {
        QMutableListIterator<QToolButton*> it(mainButtons);
        while (it.hasNext()) {
            RCToolButton *button = (RCToolButton*)it.next();
            if(button)
            {
                disconnect(button,SIGNAL(pressed()),this,SLOT(doMainButtonPressed()));
                disconnect(button,SIGNAL(released()),this,SLOT(doMainButtonReleased()));
                RCToolButton *tmp = new RCToolButton();
                tmp->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                tmp->setMinimumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                tmp->setMaximumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                tmp->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
                tmp->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
                tmp->sizePolicy().setHorizontalStretch(0);
                tmp->sizePolicy().setVerticalStretch(0);

                tmp->setObjectName(button->objectName());
                tmp->mcName = button->mcName;
                tmp->mcomId = button->mcomId;
                tmp->rcparent = button->rcparent;
                if(button->rcparent!= 0)
                    tmp->setToolTip(button->rcparent->Description());
                //tmp->setToolTip(button->rcparent->shortDescription());
                mainCopy->append(tmp);
            }
        }
    }

    if(subCopy != 0)
    {
        QMutableListIterator<QToolButton*> it(subButtons);
        while (it.hasNext()) {
            RCToolButton *button = (RCToolButton*)it.next();
            if(button)
            {
                disconnect(button,SIGNAL(pressed()),this,SLOT(doSubButtonPressed()));
                disconnect(button,SIGNAL(released()),this,SLOT(doSubButtonReleased()));
                RCToolButton *tmp = new RCToolButton();
                tmp->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                tmp->setMinimumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                tmp->setMaximumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                tmp->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
                tmp->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
                tmp->sizePolicy().setHorizontalStretch(0);
                tmp->sizePolicy().setVerticalStretch(0);

                tmp->setObjectName(button->objectName());
                tmp->mcName = button->mcName;
                tmp->mcomId = button->mcomId;
                tmp->rcparent = button->rcparent;
                if(button->rcparent!= 0)
                    tmp->setToolTip(button->rcparent->Description());
                    //tmp->setToolTip(button->rcparent->shortDescription());
                subCopy->append(tmp);
            }
        }
    }
}

void RCToolBox::addButtons( QList<QToolButton*> * mainButts , QList<QToolButton*> * subButts)
{
    doMainButtonReleased();
    lastMainButton = 0;
    doSubButtonReleased();
    lastSubButton = 0;

    for(int i=0; i<mainButts->size(); i++)
    {
        addButton(i+1,0,((RCToolButton*)mainButts->at(i))->mcName,
                  ((RCToolButton*)mainButts->at(i))->mcomId,
                  ((RCToolButton*)mainButts->at(i))->rcparent);
        connect(mainButts->at(i),SIGNAL(pressed()),this,SLOT(doMainButtonPressed()));
        connect(mainButts->at(i),SIGNAL(released()),this,SLOT(doMainButtonReleased()));
    }

    for(int i=0; i<subButts->size(); i++)
    {
        addButton(i+mnMainButtons+1,1,((RCToolButton*)subButts->at(i))->mcName,
                  ((RCToolButton*)subButts->at(i))->mcomId,
                  ((RCToolButton*)subButts->at(i))->rcparent);
        connect(subButts->at(i),SIGNAL(pressed()),this,SLOT(doSubButtonPressed()));
        connect(subButts->at(i),SIGNAL(released()),this,SLOT(doSubButtonReleased()));
    }
}

void RCToolBox::setup(layoutType type, int nCols, int nButtons, int nSubButtons)
{
    int iButton,iCol,iRow;

    if( nCols < 1 || nButtons < 1 )
    {
        qDebug() << "RCToolBox set up warning : number of columns or buttons are zero.";
        return;
    }

    switch (v.getinteger("wn::menusize")) {
    case 0:
    default:
        BUTTON_SIZE = SBUTTON_SIZE;
        break;
    case 1:
        BUTTON_SIZE = MBUTTON_SIZE;
        break;
    case 2:
        BUTTON_SIZE = LBUTTON_SIZE;
        break;
    }

    lType = type;
    mnMainButtons = nButtons;
    mnSubButtons = nSubButtons;
    mnCols = nCols;
    mnRows = ceil(nButtons / (double)nCols);

    //QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //sizePolicy.setHorizontalStretch(0);
    //sizePolicy.setVerticalStretch(0);
    //sizePolicy.setWidthForHeight(true);
    //sizePolicy.setHeightForWidth( this->sizePolicy().hasHeightForWidth());
    //setSizePolicy(sizePolicy);
    //setMinimumSize(QSize((BUTTON_SIZE * 3) + (BUTTON_MARGIN * 4), ((BUTTON_SIZE * 4) + (BUTTON_MARGIN * 5)) * 2 + MIDDLE_GAP));
    //setMaximumSize(QSize((BUTTON_SIZE * 3) + (BUTTON_MARGIN * 4), ((BUTTON_SIZE * 4) + (BUTTON_MARGIN * 5)) * 2 + MIDDLE_GAP));
    //setLayoutDirection(Qt::LeftToRight);
    setFeatures(features()|QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
    setAutoFillBackground(true);
    //setFloating(false);

    // contents widget
    if(dockWidgetContents == 0)
        dockWidgetContents = new QWidget();

    int nMainRows   = ceil(nButtons / (double)nCols);
    int hMainWidth  = (BUTTON_SIZE * nCols) + (BUTTON_MARGIN * (nCols+1));
    int hMainHeight = (BUTTON_SIZE * nMainRows) + (BUTTON_MARGIN * (nMainRows+1));
    int nSubRows=0,nSubCols=mnCols,hSubWidth=0,hSubHeight=0;
    if( nSubButtons > 0)
    {
         //nSubRows  = ceil(mnSubButtons / (double)mnCols);
         nSubRows  = nMainRows;
         //if(nSubRows > nMainRows)
         if(ceil(mnSubButtons / (double)mnCols) > nMainRows)
         {
            nSubRows = nMainRows;
            nSubCols = ceil(mnSubButtons / (double)nSubRows);
         }
         hSubWidth  = (BUTTON_SIZE * nSubCols) + (BUTTON_MARGIN * (nSubCols+1));
         hSubHeight = (BUTTON_SIZE * nSubRows) + (BUTTON_MARGIN * (nSubRows+1));
    }

    QSize thisSize;
    if(nSubButtons > 0)
    {
        if(lType == vBlock)
            thisSize = QSize(hMainWidth, hMainHeight + hSubHeight + MIDDLE_GAP);
        else if (lType == vLine)
            thisSize = QSize(hMainWidth + hSubWidth, hMainHeight);
        else if (lType == hBlock)
            thisSize = QSize(hMainWidth + hSubWidth + MIDDLE_GAP, hMainHeight);
        else if (lType == hLine)
            thisSize = QSize(hMainWidth, hMainHeight + hSubHeight);
    }
    else
        thisSize = QSize(hMainWidth, hMainHeight);

    dockWidgetContents->setMinimumSize(thisSize);
    dockWidgetContents->setMaximumSize(thisSize);

    // main layout
    dockWidgetContents->setWindowOpacity(0);
    dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));

    if(gridLayoutWidgetMain == 0)
        gridLayoutWidgetMain = new QWidget(dockWidgetContents);

    gridLayoutWidgetMain->setObjectName(QStringLiteral("gridLayoutWidgetMain"));
    gridLayoutWidgetMain->setGeometry(QRect(0, 0, hMainWidth, hMainHeight));

    if(gridLayoutMain != 0)
        delete gridLayoutMain;
    gridLayoutMain = new QGridLayout(gridLayoutWidgetMain);

    gridLayoutMain->setSpacing(BUTTON_MARGIN);
    gridLayoutMain->setObjectName(QStringLiteral("gridLayoutMain"));
    gridLayoutMain->setContentsMargins(BUTTON_MARGIN, BUTTON_MARGIN, BUTTON_MARGIN, BUTTON_MARGIN);

    // main buttons
    QMutableListIterator<QToolButton*> it(mainButtons);
    while (it.hasNext()) {
        delete it.next();
    }


    if(mainButtons.size() > 0)
        mainButtons.clear();

    iButton = 1;
    for( iRow=0; iRow < nMainRows; iRow++)
    {
        for( iCol=0; iCol < nCols; iCol++)
        {
            //QToolButton *thisButton = new QToolButton(gridLayoutWidgetMain);
            RCToolButton *thisButton = new RCToolButton(gridLayoutWidgetMain);
            if(thisButton)
            {
                QString name;
                name = QString("toolButton_%1").arg(iButton);
                thisButton->mToolBox = this;
                thisButton->setWindowOpacity(0);
                thisButton->setObjectName(name);
                thisButton->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                thisButton->setMinimumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                thisButton->setMaximumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                thisButton->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
                thisButton->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
                thisButton->sizePolicy().setHorizontalStretch(0);
                thisButton->sizePolicy().setVerticalStretch(0);

                //gridLayoutMain->addWidget(thisButton, iRow, iCol, 1, 1);
                gridLayoutMain->addWidget(thisButton, iRow, iCol, Qt::AlignCenter);
                mainButtons.append(thisButton);
                if(iButton < nButtons)
                {
                    connect(thisButton,SIGNAL(pressed()),this,SLOT(doMainButtonPressed()));
                    connect(thisButton,SIGNAL(released()),this,SLOT(doMainButtonReleased()));
                }
                //qDebug() << "iRow : " << iRow << "iCol : " << iCol ;
            }
            iButton++;
            if(iButton-1 == nButtons)
                break;
        }
        if(iButton-1 == nButtons)
            break;
    }

    //qDebug() << "grid rows : " << gridLayoutMain->rowCount();
    //qDebug() << "grid cols : " << gridLayoutMain->columnCount();

    // sub buttons
    if( nSubButtons > 0)
    {
        if(gridLayoutWidgetSub == 0)
            gridLayoutWidgetSub = new QWidget(dockWidgetContents);

        gridLayoutWidgetSub->setObjectName(QStringLiteral("gridLayoutWidgetSub"));

        if(lType == vBlock)
            gridLayoutWidgetSub->setGeometry(QRect(0, hMainHeight + MIDDLE_GAP, hMainWidth, hSubHeight));
        else if (lType == vLine)
            gridLayoutWidgetSub->setGeometry(QRect(hMainWidth, 0, hSubWidth, hSubHeight));
        else if (lType == hBlock)
            gridLayoutWidgetSub->setGeometry(QRect(hMainWidth + MIDDLE_GAP, 0, hMainWidth, hSubHeight));
        else if (lType == hLine)
            gridLayoutWidgetSub->setGeometry(QRect(0, hMainHeight, hMainWidth, hSubHeight));

        if(gridLayoutSub != 0)
            delete gridLayoutSub;
        gridLayoutSub = new QGridLayout(gridLayoutWidgetSub);

        gridLayoutSub->setSpacing(BUTTON_MARGIN);
        gridLayoutSub->setObjectName(QStringLiteral("gridLayoutSub"));
        gridLayoutSub->setContentsMargins(BUTTON_MARGIN, BUTTON_MARGIN, BUTTON_MARGIN, BUTTON_MARGIN);

        QMutableListIterator<QToolButton*> it(subButtons);
        while (it.hasNext()) {
            delete it.next();
        }

        if(subButtons.size() > 0)
            subButtons.clear();

        iButton = mnMainButtons+1;
        for( iRow=0; iRow < nMainRows; iRow++)
        {
            for( iCol=0; iCol < nCols; iCol++)
            {
                //QToolButton *thisButton = new QToolButton(gridLayoutWidgetSub);
                RCToolButton *thisButton = new RCToolButton(gridLayoutWidgetSub);
                if(thisButton)
                {
                    QString name;
                    name = QString("toolButton_%1").arg(iButton);
                    thisButton->mToolBox = this;
                    thisButton->mIsSubButton = true;
                    thisButton->setWindowOpacity(0);
                    thisButton->setObjectName(name);
                    thisButton->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                    thisButton->setMinimumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                    thisButton->setMaximumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                    thisButton->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
                    thisButton->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
                    thisButton->sizePolicy().setHorizontalStretch(0);
                    thisButton->sizePolicy().setVerticalStretch(0);

                    //gridLayoutSub->addWidget(thisButton, iRow, iCol, 1, 1);
                    gridLayoutSub->addWidget(thisButton, iRow, iCol, Qt::AlignCenter);
                    subButtons.append(thisButton);
                    if(iButton < mnMainButtons+nSubButtons)
                    {
                        connect(thisButton,SIGNAL(pressed()),this,SLOT(doSubButtonPressed()));
                        connect(thisButton,SIGNAL(released()),this,SLOT(doSubButtonReleased()));
                    }
                    //qDebug() << "iRow : " << iRow << "iCol : " << iCol ;
                }
                iButton++;
                //if(iButton-1 == mnMainButtons+nSubButtons)
                if(iButton-1 == mnMainButtons+nButtons)
                    break;
            }
            //if(iButton-1 == mnMainButtons+nSubButtons)
            if(iButton-1 == mnMainButtons+nButtons)
                break;
        }
    }

    setWidget(dockWidgetContents);
    //retranslateUi();
    QMetaObject::connectSlotsByName(this);

}

QString RCToolBox::getIconName( QString name )
{
    for(int i=0; i<RC_NUM_NAMED_BITMAP_RESOURCES; i++)
    {
        QString lwrresr(RCnamedBitmapResources[i].name);
        QString lwrname(name);
        //if( strcmp(lwrresr.toLower(), lwrname.toLower())  == 0 &&
        //    stricmp(RCnamedBitmapResources[i].typeName, "BITMAP") == 0  )
        if( lwrresr.compare(lwrname,Qt::CaseInsensitive) == 0 &&
             strcmp(RCnamedBitmapResources[i].typeName, "BITMAP") == 0 )
        {
            QString qsname;
            qsname = QString(":/images/%1").arg(RCnamedBitmapResources[i].fileName);
            return qsname;
        }
    }
    // not in the ap resources so use the full name
    return QString("");
}

RCToolButton *RCToolBox::addButton(int bid, QString cname, int comid )
{
    return addButton(bid,mnCols,cname,comid);
}

RCToolButton *RCToolBox::addButton(int bid, int sid, QString cname, int comid , Button *rcparent)
{
    int iButton,iCol,iRow;

    doMainButtonReleased();
    lastMainButton = 0;
    doSubButtonReleased();
    lastSubButton = 0;

    if( bid <= mnMainButtons && sid == 1 )
    {
        //qDebug() << "RCToolBox Error addButton : sub id (" << bid << ") is a main id. Adding to sub (" << mnMainButtons+bid << ")";
        bid += mnMainButtons;
    }

    // make the buttons name and find it
    QString bname = QString("toolButton_%1").arg(bid);
    //QToolButton *button = findChild<QToolButton*>(bname);
    RCToolButton *button = (RCToolButton*)findChild<QToolButton*>(bname);

    // if a button exists set it's icon image
    // else add a new button to the correct layout
    if(button == 0)
    {
        if(sid == 0)
        {// main button
            // check the contents widget
            if(dockWidgetContents == 0)
            {
                // set up a new main menu block with 1 column and bid rows
                setup(vBlock,1,bid);
            }
            else
            {
                if( bid > mnMainButtons )
                     mnMainButtons = bid - mnMainButtons;
                int nMainRows   =  ceil(mnMainButtons / (double)mnCols);
                int hMainWidth  = (BUTTON_SIZE * mnCols) + (BUTTON_MARGIN * (mnCols+1));
                int hMainHeight = (BUTTON_SIZE * nMainRows) + (BUTTON_MARGIN * (nMainRows+1));
                int nSubRows=0,nSubCols=mnCols,hSubWidth=0,hSubHeight=0;
                if( mnSubButtons > 0)
                {
                     //nSubRows  = ceil(mnSubButtons / (double)mnCols);
                     nSubRows  = nMainRows;
                     //if(nSubRows > nMainRows)
                     if(ceil(mnSubButtons / (double)mnCols) > nMainRows)
                     {
                        nSubRows = nMainRows;
                        nSubCols = ceil(mnSubButtons / (double)nSubRows);
                     }
                     hSubWidth  = (BUTTON_SIZE * nSubCols) + (BUTTON_MARGIN * (nSubCols+1));
                     hSubHeight = (BUTTON_SIZE * nSubRows) + (BUTTON_MARGIN * (nSubRows+1));
                }
                // add it to the existing content widget
                if(gridLayoutWidgetMain == 0)
                    gridLayoutWidgetMain = new QWidget(dockWidgetContents);

                QSize thisSize;
                if(mnSubButtons > 0)
                {
                    if(lType == vBlock)
                        thisSize = QSize(hMainWidth, hMainHeight + hSubHeight + MIDDLE_GAP);
                    else if (lType == vLine)
                        thisSize = QSize(hMainWidth + hSubWidth, hMainHeight);
                    else if (lType == hBlock)
                        thisSize = QSize(hMainWidth + hSubWidth + MIDDLE_GAP, hMainHeight);
                    else if (lType == hLine)
                        thisSize = QSize(hMainWidth, hMainHeight + hSubHeight);
                }
                else
                    thisSize = QSize(hMainWidth, hMainHeight);

                dockWidgetContents->setMinimumSize(thisSize);
                dockWidgetContents->setMaximumSize(thisSize);

                gridLayoutWidgetMain->setObjectName(QStringLiteral("gridLayoutWidgetMain"));
                gridLayoutWidgetMain->setGeometry(QRect(0, 0, hMainWidth, hMainHeight));

                if(gridLayoutMain != 0)
                    delete gridLayoutMain;
                gridLayoutMain = new QGridLayout(gridLayoutWidgetMain);

                gridLayoutMain->setSpacing(BUTTON_MARGIN);
                gridLayoutMain->setObjectName(QStringLiteral("gridLayoutMain"));
                gridLayoutMain->setContentsMargins(BUTTON_MARGIN, BUTTON_MARGIN, BUTTON_MARGIN, BUTTON_MARGIN);

                iButton = 1;
                for( iRow=0; iRow < nMainRows; iRow++)
                {
                    for( iCol=0; iCol < mnCols; iCol++)
                    {
                        button = 0;
                        if( iButton == bid || iButton > mainButtons.size())
                            button = new RCToolButton(cname,comid,gridLayoutWidgetMain);
                            //button = new QToolButton(gridLayoutWidgetMain);

                        if(button)
                        {
                            QString name;
                            name = QString("toolButton_%1").arg(iButton);
                            button->mToolBox = this;
                            button->setWindowOpacity(0);
                            button->setObjectName(name);
                            button->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                            button->setMinimumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                            button->setMaximumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                            button->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
                            button->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
                            button->sizePolicy().setHorizontalStretch(0);
                            button->sizePolicy().setVerticalStretch(0);

                            if(iButton > mainButtons.size())
                            {
                                //gridLayoutMain->addWidget(button, iRow, iCol, 1, 1);
                                gridLayoutMain->addWidget(button, iRow, iCol,Qt::AlignCenter);
                                mainButtons.append(button);
                                connect(button,SIGNAL(pressed()),this,SLOT(doMainButtonPressed()));
                                connect(button,SIGNAL(released()),this,SLOT(doMainButtonReleased()));
                            }
                            else
                            {
                                disconnect(mainButtons.at(iButton-1),SIGNAL(pressed()),this,SLOT(doMainButtonPressed()));
                                disconnect(mainButtons.at(iButton-1),SIGNAL(released()),this,SLOT(doMainButtonreleased()));
                                gridLayoutMain->removeWidget(mainButtons.at(iButton-1));
                                delete mainButtons.at(iButton-1);
                                mainButtons.replace(iButton-1,button);
                                connect(button,SIGNAL(pressed()),this,SLOT(doMainButtonPressed()));
                                connect(button,SIGNAL(released()),this,SLOT(doMainButtonReleased()));
                                //gridLayoutMain->addWidget(button, iRow, iCol, 1, 1);
                                gridLayoutMain->addWidget(button, iRow, iCol, Qt::AlignCenter);
                            }
                            //qDebug() << "iRow : " << iRow << "iCol : " << iCol ;
                        }
                        iButton++;
                        if(iButton-1 == mnMainButtons)
                            break;
                    }
                    if(iButton-1 == mnMainButtons)
                        break;
                }
            }
            // look for it again
            bname = QString("toolButton_%1").arg(bid);
            //button = findChild<QToolButton*>(bname);
            button = (RCToolButton*)findChild<QToolButton*>(bname);
        }
        else
        {// sub button
            // check the contents widget
            if(dockWidgetContents == 0)
            {
                // set up a new sub menu block with 1 column and 1 row and bid sub buttons
                setup(vBlock,1,1,bid);
            }
            else
            {
                if( bid <= mnMainButtons )
                {
                    //qDebug() << "RCToolBox Error addButton : sub id (" << bid << ") is a main id. Adding to sub (" << mnMainButtons+bid << ")";
                    bid += mnMainButtons;
                }

                if( bid > mnSubButtons )
                     mnSubButtons = bid - mnMainButtons;
                    //mnSubButtons += bid - mnMainButtons;
                int nMainRows   =  ceil(mnMainButtons / (double)mnCols);
                int hMainWidth  = (BUTTON_SIZE * mnCols) + (BUTTON_MARGIN * (mnCols+1));
                int hMainHeight = (BUTTON_SIZE * nMainRows) + (BUTTON_MARGIN * (nMainRows+1));
                int nSubRows=0,nSubCols=mnCols,hSubWidth=0,hSubHeight=0;
                if( mnSubButtons > 0)
                {
                     nSubRows  = ceil(mnSubButtons / (double)mnCols);
                     if(nSubRows > nMainRows)
                     {
                         nSubRows = nMainRows;
                         nSubCols = ceil(mnSubButtons / (double)nSubRows);
                     }
                     hSubWidth  = (BUTTON_SIZE * nSubCols) + (BUTTON_MARGIN * (nSubCols+1));
                     hSubHeight = (BUTTON_SIZE * nSubRows) + (BUTTON_MARGIN * (nSubRows+1));
                }
                // add it to the existing content widget
                if(gridLayoutWidgetSub == 0)
                    gridLayoutWidgetSub = new QWidget(dockWidgetContents);

                QSize thisSize;
                if(mnSubButtons > 0)
                {
                    if(lType == vBlock)
                        thisSize = QSize(hMainWidth, hMainHeight + hSubHeight + MIDDLE_GAP);
                    else if (lType == vLine)
                        thisSize = QSize(hMainWidth + hSubWidth, hMainHeight );
                    else if (lType == hBlock)
                        thisSize = QSize(hMainWidth + hSubWidth + MIDDLE_GAP, hMainHeight);
                    else if (lType == hLine)
                        thisSize = QSize(hMainWidth, hMainHeight + hSubHeight);
                }
                else
                    thisSize = QSize(hMainWidth, hMainHeight);

                dockWidgetContents->setMinimumSize(thisSize);
                dockWidgetContents->setMaximumSize(thisSize);

                gridLayoutWidgetSub->setObjectName(QStringLiteral("gridLayoutWidgetSub"));
                if(lType == vBlock)
                    gridLayoutWidgetSub->setGeometry(QRect(0, hMainHeight + MIDDLE_GAP, hMainWidth, hSubHeight));
                else if (lType == vLine)
                    if(mnSubButtons > 0)
                        gridLayoutWidgetSub->setGeometry(QRect(hMainWidth , 0, hSubWidth, hSubHeight));
                    else
                        gridLayoutWidgetSub->setGeometry(QRect(hMainWidth, 0, hMainWidth, hSubHeight));
                else if (lType == hBlock)
                    gridLayoutWidgetSub->setGeometry(QRect(hMainWidth + MIDDLE_GAP, 0, hMainWidth, hSubHeight));
                else if (lType == hLine)
                    gridLayoutWidgetSub->setGeometry(QRect(0, hMainHeight, hMainWidth, hSubHeight));

                if(gridLayoutSub != 0)
                    delete gridLayoutSub;
                gridLayoutSub = new QGridLayout(gridLayoutWidgetSub);

                gridLayoutSub->setSpacing(BUTTON_MARGIN);
                gridLayoutSub->setObjectName(QStringLiteral("gridLayoutSub"));
                gridLayoutSub->setContentsMargins(BUTTON_MARGIN, BUTTON_MARGIN, BUTTON_MARGIN, BUTTON_MARGIN);

                iButton = mnMainButtons+1;
                for( iRow=0; iRow < nMainRows; iRow++)
                {
                    for( iCol=0; iCol < nSubCols; iCol++)
                    {
                        button = 0;
                        if( iButton == bid || iButton-mnMainButtons > subButtons.size())
                            button = new RCToolButton(cname,comid,gridLayoutWidgetSub);
                            //button = new QToolButton(gridLayoutWidgetSub);

                        if(button)
                        {
                            QString name;
                            name = QString("toolButton_%1").arg(iButton);
                            button->mToolBox = this;
                            button->mIsSubButton=true;
                            button->setWindowOpacity(0);
                            button->setObjectName(name);
                            button->setIconSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                            button->setMinimumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                            button->setMaximumSize(QSize(BUTTON_SIZE, BUTTON_SIZE));
                            button->sizePolicy().setHorizontalPolicy(QSizePolicy::Fixed);
                            button->sizePolicy().setVerticalPolicy(QSizePolicy::Fixed);
                            button->sizePolicy().setHorizontalStretch(0);
                            button->sizePolicy().setVerticalStretch(0);

                            if(iButton > subButtons.size())
                            {
                                //gridLayoutSub->addWidget(button, iRow, iCol, 1, 1);
                                gridLayoutSub->addWidget(button, iRow, iCol,Qt::AlignCenter);
                                connect((QWidget*)button,SIGNAL(pressed()),this,SLOT(doSubButtonPressed()));
                                connect((QWidget*)button,SIGNAL(released()),this,SLOT(doSubButtonReleased()));
                                subButtons.append(button);
                            }
                            else
                            {
                                disconnect((QWidget*)subButtons.at(iButton-1),SIGNAL(pressed()),this,SLOT(doSubButtonPressed()));
                                disconnect((QWidget*)subButtons.at(iButton-1),SIGNAL(released()),this,SLOT(doSubButtonReleased()));
                                gridLayoutSub->removeWidget(subButtons.at(iButton-1));
                                delete subButtons.at(iButton-1);
                                subButtons.replace(iButton-1,button);
                                connect((QWidget*)subButtons.at(iButton-1),SIGNAL(pressed()),this,SLOT(doSubButtonPressed()));
                                connect((QWidget*)subButtons.at(iButton-1),SIGNAL(released()),this,SLOT(doSubButtonReleased()));
                                //gridLayoutSub->addWidget(button, iRow, iCol, 1, 1);
                                gridLayoutSub->addWidget(button, iRow, iCol,Qt::AlignCenter);
                            }
                            //qDebug() << "iRow : " << iRow << "iCol : " << iCol ;
                        }
                        iButton++;
                        if(iButton-1 == mnMainButtons+mnSubButtons)
                            break;
                    }
                    if(iButton-1 == mnMainButtons+mnSubButtons)
                        break;
                }
            }
            // look for it again
            bname = QString("toolButton_%1").arg(bid);
            //button = findChild<QToolButton*>(bname);
            button = (RCToolButton*)findChild<QToolButton*>(bname);
        }
    }

    if(button)
    {
        QImage iicon,imask;
        QString rname;


        //if( button->icon().isNull() ||
        //    (button->rcparent &&
        //     button->rcparent->bitmap()->isNull()))
        if( rcparent == 0  || rcparent->bitmap() == 0 ||
            (rcparent->bitmap() && rcparent->bitmap()->isNull()))
        {
            bname = cname;
            rname = getIconName(QString("CA_%1%2").arg(cname).arg("_bitmap"));
            if(rname.length() == 0)
                rname = cname;
            if(rname.contains(".svg"))
                iicon = QImage(rname,"svg");
            else if(rname.contains(".bmp"))
                iicon = QImage(rname,"bmp");
            else if(rname.contains(".png"))
                iicon = QImage(rname,"png");
            else
                iicon = QImage(getIconName(rname));
            //imask = QImage(rname,"svg");
            if (iicon.bits() == 0 )
            {
                rname = getIconName(QString("%1%2").arg(cname).arg("_bitmap"));
                if(rname.contains(".svg"))
                    iicon = QImage(rname,"svg");
                else if(rname.contains(".bmp"))
                    iicon = QImage(rname,"bmp");
                else if(rname.contains(".png"))
                    iicon = QImage(rname,"png");
                else
                    iicon = QImage(getIconName(rname));
                //imask = QImage(getIconName(rname),"svg");
            }
        }
        else if(rcparent && rcparent->bitmap() && !rcparent->bitmap()->isNull())
        {
            //qDebug() << "crparent bmap : " << rcparent->bitmap();
            QPixmap *pxmap = (QPixmap*)rcparent->bitmap();
            //pxmap->setDevicePixelRatio(((QMainWindow*)cadwindow->gethwnd())->devicePixelRatio());
            pxmap->setDevicePixelRatio(devicePixelRatio());
            iicon = pxmap->toImage();
            iicon.setDevicePixelRatio(devicePixelRatio());
            //iicon = ((QPixmap*)rcparent->bitmap())->toImage();
        }

        if(iicon.bits() != 0)
        {
            iicon.setDevicePixelRatio(app->devicePixelRatio());
            //qDebug() << "button image dpr" << iicon.devicePixelRatio();
            QPixmap pxmap;
            //pxmap.setDevicePixelRatio(((QMainWindow*)cadwindow->gethwnd())->devicePixelRatio());
            pxmap = QPixmap::fromImage(iicon);
            pxmap.setDevicePixelRatio(devicePixelRatio());
            QIcon bicon(QPixmap::fromImage(iicon));
            //QIcon bicon(QPixmap::fromImage(iicon));
            button->setIcon(bicon);
            button->setIconSize(QSize(BUTTON_SIZE,BUTTON_SIZE));
            button->mcName = cname;
            button->mcomId = comid;
            button->rcparent = rcparent;
            if(button->rcparent != 0)
            {
                button->setToolTip(rcparent->Description());
                //button->setToolTip(rcparent->shortDescription());
                button->rcparent->setQW(button);
            }
        }
    }
    return button;
}


void RCToolBox::retranslateUi()
{
    setWindowTitle(QApplication::translate("RCToolBox", "DockWidget", 0));
    /*
    toolButton_1->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_2->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_3->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_4->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_5->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_6->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_7->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_8->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_9->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_10->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_11->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_12->setText(QApplication::translate("RCToolBox", "...", 0));
    //
    toolButton_13->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_14->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_15->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_16->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_17->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_18->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_19->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_20->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_21->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_22->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_23->setText(QApplication::translate("RCToolBox", "...", 0));
    toolButton_24->setText(QApplication::translate("RCToolBox", "...", 0));
    */
} // retranslateUi

void RCToolBox::initButtons(int sid)
{
    int i;
    QString bname;
    QToolButton *button;

    int num = mainButtons.size() + subButtons.size();

    for(i=0; i<num; i++)
    {
        bname = QString("toolButton_%1").arg(i+1+(sid*mainButtons.size()));
        button = findChild<QToolButton*>(bname);
        if(button)
        {
            button->setIcon(QIcon());
            button->setIconSize(QSize(BUTTON_SIZE,BUTTON_SIZE));
            ((RCToolButton*)button)->rcparent = 0;
            ((RCToolButton*)button)->mcName = "";
            ((RCToolButton*)button)->mcomId = 0;
            ((RCToolButton*)button)->setToolTip("");
        }
    }
}

void RCToolBox::hideButtons(int sid)
{
    int i;
    QString bname;
    QToolButton *button;

    int num = mainButtons.size() + subButtons.size();

    for(i=0; i<num; i++)
    {
        bname = QString("toolButton_%1").arg(i+1+(sid*mainButtons.size()));
        button = findChild<QToolButton*>(bname);
        if(button)
            button->hide();
    }
}

void RCToolBox::pushButton(RCToolButton *b)
{
   if(b)
   {

        b->setDown(true);
        b->setChecked(true);
        b->repaint();
        //b->update();
        app->processEvents();

   }
}

void RCToolBox::releaseButton(RCToolButton *b)
{
   if(b)
   {
       b->setChecked(false);
       b->setDown(false);
       b->repaint();
       //b->update();
       app->processEvents();
   }
}

void RCToolBox::enableButton(RCToolButton *b, bool onOff)
{
   if(b)
   {
        b->setDisabled(onOff);
        app->processEvents();
   }
}

void releasAllButtons()
{
}

void enableAllButtons(bool onOff)
{

}

/*!
 * \brief RCToolBox::doMainButtonPressed
 * Buttons on tool boxes with sub buttons are treated as toggle buttons
 * this function releases the last used main button when another one is pressed
 */
void RCToolBox::doMainButtonPressed()
{
   //qDebug() << "Entering doMainButtonPressed";
}

/*!
 * \brief RCToolBox::doMainButtonReleased
 * when a tool box only has main buttons we treat as a push button
 * as opposed to a toggle type button.
 * this function resets the button to off as soon as it is released
 */
void RCToolBox::doMainButtonReleased()
{
    //qDebug() << "Entering doMainButtonReleased";
    //if(lastButton && lastButton != sender())
    //    previousButton = lastButton;
    if(mainButtons.contains((RCToolButton*)sender()))
    {
        if(subButtons.size() < 1)
        {
            if(lastMainButton && lastMainButton != sender())
                releaseButton((RCToolButton*)lastMainButton);
            pushButton((RCToolButton*)sender());
            releaseButton((RCToolButton*)sender());
            lastMainButton = (RCToolButton*)sender();
            //lastButton = (RCToolButton*)sender();
        }
        else
        {
            if(lastMainButton && lastMainButton != sender())
                releaseButton((RCToolButton*)lastMainButton);
            pushButton((RCToolButton*)sender());
            lastMainButton = (RCToolButton*)sender();
            //lastButton = (RCToolButton*)sender();
        }
    }
}

/*!
 * \brief RCToolBox::doMainButtonPressed
 * Buttons on tool boxes with sub buttons are treated as toggle buttons
 * this function releases the last used sub button when another one is pressed
 */
void RCToolBox::doSubButtonPressed()
{
    /*
    qDebug() << "";
    qDebug() << "Entering doSubButtonPressed";
    qDebug() << "subbutton name    : " << ((RCToolButton*)sender())->mcName;
    qDebug() << "subbutton checked : " << ((RCToolButton*)sender())->isChecked();
    qDebug() << "subbutton down    : " << ((RCToolButton*)sender())->isDown();
    if(((RCToolButton*)sender())->rcparent)
        qDebug() << "rcbutton state    : " << ((RCToolButton*)sender())->rcparent->buttonstate();

    qDebug() << "leaving doSubButtonPressed";
    qDebug() << "subbutton name    : " << ((RCToolButton*)sender())->mcName;
    qDebug() << "subbutton checked : " << ((RCToolButton*)sender())->isChecked();
    qDebug() << "subbutton down    : " << ((RCToolButton*)sender())->isDown();
    if(((RCToolButton*)sender())->rcparent)
        qDebug() << "rcbutton state    : " << ((RCToolButton*)sender())->rcparent->buttonstate();
    qDebug() << "";
    */
}

void RCToolBox::doSubButtonReleased()
{
    if(subButtons.contains((RCToolButton*)sender()))
    {
         if(((RCToolButton*)sender())->rcparent &&
            ((RCToolButton*)sender())->rcparent->buttonstate()==4)
             ((RCToolButton*)sender())->setChecked(true);
         else
             ((RCToolButton*)sender())->setChecked(false);

         lastSubButton = (RCToolButton*)sender();
    }
}

void RCToolBox::saveState()
{
    // save the position info the 1 at the end indicates if it is floating
    // and where it is docked 0 = floating; 1 = left; 2 = right; 4 = top; 8 = bottom
    RCCHAR text[300];
    QRect grect = geometry();

    if(mTitle.length() == 0) /// don't save without a title
        return;

    //sprintf(text,"%d,%d,%d,%d,%d",grect.x(),grect.y(),grect.width(),grect.height(),(int)mArea);
    //QString fullName = QString("ApplicationMenu::") + mTitle;
    //WritePrivateProfileString(QString("Layout").data(),fullName.data(),text,home.getinifilename());
    if(!isVisible())
        return;

    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
    settings.setValue("Layout/"+mRole,saveGeometry());
    //settings.setValue("WindowState/MainWindow", ((QMainWindow*)cadwindow->gethwnd())->saveState());

}

void RCToolBox::restoreState()
{
    if(mTitle.length() == 0) // dont restore if no title
        return;

    //QString fullName = QString("ApplicationMenu::") + mTitle;
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
    restoreGeometry(settings.value("Layout/"+mRole).toByteArray());

}

void RCToolBox::replaceButton(RCToolButton *from,RCToolButton *to)
{
    doMainButtonReleased();
    lastMainButton = 0;
    doSubButtonReleased();
    lastSubButton = 0;

    for(int i=0; i<mainButtons.size(); i++)
    {
        if(mainButtons[i] == from)
            mainButtons[i] = to;
    }

    for(int i=0; i<subButtons.size(); i++)
    {
        if(subButtons[i] == from)
            subButtons[i] = to;
    }
}

void RCToolBox::onVisibilityChanged(bool visibility)
{
    qDebug() << "onVisibilityChanged";

    if(visibility)
    {
        ButtonMenu *bm = buttonmenus.match(mRole.data());
        if(bm != 0)
        {
            if(!mAppPath.isEmpty())
                program->loadapp(mAppPath.data());
            cadwindow->addcontrol(bm);
        }
        else
        {
            bm = buttonmenus.match(mTitle.data());
            if(bm != 0)
                cadwindow->addcontrol(bm);
        }
        //restoreState();
        //program->savedefaultapps();
        saveState();
    }
    else if(!visibility && cadwindow && !((QtMainWindow*)cadwindow->gethwnd())->isClosing())
    {
        ButtonMenu *bm = buttonmenus.match(mRole.data());
        if(bm != 0 && !cadwindow->isMinimized())
            program->unloadapp(mTitle.data());
        else
        {
            bm = buttonmenus.match(mTitle.data());
            if(bm != 0)
                cadwindow->delcontrol(bm);
        }
    }
}
