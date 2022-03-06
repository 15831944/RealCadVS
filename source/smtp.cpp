#include "smtp.h"

Smtp::Smtp(QString smtphost, QString smtpusername, QString smtppass)
{
    qDebug() << "### Class Smtp start"; 
    this->smtphost = smtphost;
    this->smtpusername = smtpusername;
    this->smtppass = smtppass; 
}
void Smtp::SetConfig(QString smtphost, QString smtpusername, QString smtppass)
{
    this->smtphost = smtphost;
    this->smtpusername = smtpusername;
    this->smtppass = smtppass; 
}

void Smtp::Send( const QString &from, const QString &to, const QString &subject, const QString &body )
{
    int waittime = 5 * 1000;
    this->from = from;
    rcpt = to;
    ErrorMSG.clear();
    Timeout = waittime;
    linesend = 0;
    isconnect = false;
    
    
    message = TimeStampMail()+"\n";
    message.append("User-Agent: Mozilla Thunderbird 1.0.6 (Macintosh/20050716)\n");
    message.append("X-Accept-Language: en-us, en\n");
    message.append("MIME-Version: 1.0\n");
    message.append("To: " + to + "\n");
    message.append("From: "+from+" <" + from + ">\n");
    message.append("Subject: " + subject + "\n");
    message.append("Content-Type: text/plain; charset=UTF8;\n");
    message.append("Content-transfer-encoding: 7BIT\n\n\n\n");
    message.append(body);
    message.replace( QString::fromLatin1( "\n" ), QString::fromLatin1( "\r\n" ) );
    message.replace( QString::fromLatin1( "\r\n.\r\n" ),QString::fromLatin1( "\r\n..\r\n" ) );
    
    qDebug() << "### Launch mail compose....  "  << from << to << subject << body;
    qDebug() << "### Config server smtp connect to......  "  << smtphost;
    smtpsocket = new QTcpSocket(this);
    connect( this, SIGNAL(ConnectorSuccess()), this ,SLOT(ReadLiner())); 
    connect( this, SIGNAL(SendLine()), this ,SLOT(PutSendLine())); 
    if (smtphost.size() > 0) {    
    smtpsocket->connectToHost(smtphost,25);
    } else {
    smtpsocket->connectToHost("localhost",25);  
    }
    if (smtpsocket->waitForConnected(Timeout)) {
    qDebug() <<"### connected on  " << smtphost; 
         if (smtpsocket->waitForReadyRead(Timeout)) {
          qDebug() <<"### emit from waitForReadyRead connect go can read";
          isconnect = true;
          emit ConnectorSuccess();
          } 
    } else {
        emit ErrorCloseAll();
    }
   
}

void Smtp::ReadLiner()
{
    if (isconnect) {
        qDebug() << "### socketType = " << smtpsocket->socketType();
        qDebug() << "### ReadLiner is start by textstream ";
        t = new QTextStream( smtpsocket );
        int loops = 0;
        while (!t->atEnd()) {
        loops++;
        response = t->readLine();
        qDebug() << loops << " in line  " << response;
        }
        if (response.size() > 0) {
        RemoteServerName = response;
        mailstatus = response.left(3);
        qDebug() << "###Status=" << mailstatus;
                if (mailstatus == "220") {
                response="";
                linesend = 1;
                emit SendLine();
                }
        } else {
           emit ErrorCloseAll(); 
        }
    }
}

Smtp::~Smtp()

{
    qDebug() << "### Class Smtp stop and delete "; 
}

/* LINE SENDER  */
void Smtp::PutSendLine()
{
    int current = linesend;
    qDebug() <<"### Go and Send line " << linesend;
    switch(current) {
      case 1:
          response = SendLineAndGrab("ehlo localhost");
           if (response.size() > 0) {
               ErrorMSG.append(response);
               qDebug() << "1---- " << response;
                linesend = 2;
                emit SendLine();
           } else {
                qDebug() << "Connection loost";
           }
          response ="";
      break;
      case 2:
          response = SendLineAndGrab("AUTH LOGIN");
           if (response.size() > 0) {
           ErrorMSG.append(response);
                qDebug() << "2---- " << response;
                linesend = 3;
                emit SendLine();
           } else {
                qDebug() << "Connection loost";
           }
          response ="";
      break;
      case 3:
          response = SendLineAndGrab(encodeBase64(smtpusername));   /* username send */
           if (response.size() > 0) {
           ErrorMSG.append(response);
                qDebug() << "3---- " << response;
                linesend = 4;
                emit SendLine();
           } else {
                qDebug() << "Connection loost";
           }
          response ="";
      break;
      case 4:
          response = SendLineAndGrab(encodeBase64(smtppass));     /* pass send */
           qDebug() << "4---- " << response;
           if (response.size() > 0) {
           ErrorMSG.append(response);
                if (response.contains("ok", Qt::CaseInsensitive)) {
                   linesend = 5;
                   emit SendLine();
                } else {
                   qDebug() << "Connection loost";
                }
           } else {
                qDebug() << "Connection loost";
           }
          response ="";
      break;
      case 5:
          response = SendLineAndGrab("MAIL FROM: "+from);
           qDebug() << "5---- " << response;
           if (response.size() > 0) {
                linesend = 6;
                emit SendLine();               
           }
          
      break;
      case 6:
          response = SendLineAndGrab("RCPT TO: "+rcpt);
           qDebug() << "6---- " << response;
           if (response.size() > 0) {
                ErrorMSG.append(response);
                response ="";
                response = SendLineAndGrab("DATA");
                         if (!response.contains("not", Qt::CaseInsensitive)) {
                         ErrorMSG.append(response);
                         response ="";             
                         linesend = 7;
                         emit SendLine();                      
                         }
           }
          response ="";
      break;
      case 7:
          response = SendLineAndGrab(message+"\r\n.");
           qDebug() << "7---- " << response;
           if (response.size() && response.contains("ok", Qt::CaseInsensitive) ) {
                ErrorMSG.append(response);
                linesend = 8;
                emit SendLine();               
           }
           response ="";
      break;
      case 8:
          SendLineAndGrab("QUIT");
      break;
      default:
      qDebug() << "Last line ...";
      /*emit ErrorCloseAll();*/
      return;
      break;
    }   
}

/* SENDER AND RECIVER  */
QString Smtp::SendLineAndGrab(QString senddata)
{
    QString incommingData = "";
    if (isconnect) {
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    int current = linesend;
    int loops = 0;
    
    qDebug() << "####Send" << current << "Now => " << senddata;
    *t << senddata << "\r\n";
     t->flush();
        if (senddata != "QUIT") {
          if (smtpsocket->waitForReadyRead(Timeout)) {
                    while (!t->atEnd()) {
                    loops++;
                    QString opera = t->readLine()+"\n";
                    incommingData = opera + incommingData;
                    qDebug() << loops << "|#" << opera << "#|";
                    }
          } 
        } else {
          QApplication::restoreOverrideCursor();
          delete smtpsocket;
          delete t;
          isconnect = false;
          emit SuccessQuit();
          return incommingData;
        }
    QApplication::restoreOverrideCursor();
      } else {
      emit ErrorCloseAll();
      }
return incommingData;
}
 
void Smtp::disconnected()
{
    qDebug() <<"disconneted";
    qDebug() << "error "  << smtpsocket->errorString();
}
void Smtp::connected()
{
    output.append("connected");
    qDebug() << "Connected ";
}

QString Smtp::encodeBase64( QString xml )
{
    QByteArray text;
    text.append(xml);
    return text.toBase64();
}

QString Smtp::decodeBase64( QString xml )
{
    QByteArray xcode("");;
    xcode.append(xml);
    QByteArray precode(QByteArray::fromBase64(xcode));
    QString notetxt = precode.data();
    return notetxt;
}


QStringList Smtp::GetErrorlist()
{
    return ErrorMSG;
}




/*
220 remote.com ESMTP
ehlo localhost
250-remote.com
250-STARTTLS
250-PIPELINING
250-8BITMIME
250-SIZE 0
250 AUTH LOGIN PLAIN CRAM-MD5
http://www.projektfarm.com/en/support/howto/postfix_smtp_auth_tls.html
*/


