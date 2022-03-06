
// Changes
// v7 bug fixes/enhancements
//
// b1 Leader option bug
// Welcome screen added
// a4 fixed feet and inches spinbox display\
// v8 Bug fixes/Enhancements
//
// b9 right click does not work sometimes to end commands
// b13 check spelling errors in tooltips
// b15 current layer should not be allowed to be invisible
// b16 selections sould not be lost after appliing color,wieght, style etc.
// b17 numeric fields need to accept negative values
// b18 reset selection for polygon input commands
// b20 planes loose transparency when enhanced tools are used
// printing p1 100% white and black option
// printing p3 printer tries to use the last printer selected
// Printing p4 don't close the pront dialog after printing
// UI/UX u2 polyfence changed to use a closed plygon rubberband
// UI/UX u3 changed referenced from Figures to Components
// UI/UX u4 changed references from Composed to Layout
// UI/UX u25 change discription of standard views
// File f2 network licensing needs to be working
// File f9 activate the import SketchUp Warehouse tool
// b22 fixed default button for various dialogs
// 2017/6/7 v7.5
// added Flat Pattern and Insert Spiral modules
// 2018/1 v7.9
// added LOD (bounding box displayed) feature to the figure entity
// change the figure options check box to read "Auto Attach"
// fixed text Zooming crash (added a zero height check before creating polys)
// changed the figure browser show the path in the file tree
// fixed the figure bounding box display for figures in the virtical plane
// tweaked the Gl line thickness display
// made the light verify dialog recognise Landworks sun light attributes
//
// v7.11
// fixed a bug when the scaled bitmap command would crash when selecting settings dailog

#include "ncwin.h"
#include <QtNetwork>
#include "time.h"

// internet connection checking stuff
#ifdef _MAC
//#include <SystemConfiguration/SystemConfiguration.h>
#else
// windows
#include <wininet.h>
#pragma comment(lib, "wininet")
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi")
#endif

#include "RCView.h"
#include "RCCentralWidget.h"
#include "UnitTests.h"
#include "rcsplashscreen_dialog.h"

#ifdef _MAC
#include <unistd.h>
#include "MacNativeCode.h"
#endif

#ifndef _USING_QT
#include <crtdbg.h>
#include "HtmlHelp.h"
#include "new.h"
//#include "shlobj.h"
//#include <mapi.h>
//#include <mapix.h>
#endif

#include "RCplatform.h"
#include "mainwindow.h"

#include "RCStyleSheets.h"

// KMJ: I think we don't need this any more
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

//class DumpLeaks
//  {public:
//     ~DumpLeaks()  {  _CrtDumpMemoryLeaks();  }
//  } dumpleaks;


//

//  Vista
//    Store data files under environment variable %PUBLIC%\\AppName
//      AppName is taken from the current install directory of the application
//  XP  
//    Store data files under environment variable %AllUsersProfile%
//
//

#ifdef _USING_QT
RCApplication* app = 0;
#endif

Home  home;
Values v;
Program *program = 0;
WindowManager wm;     
StateMachine state;
Commands commands;
ButtonMenus buttonmenus;
Database db;
Rubberband rubberband;
MainWindow *cadwindow = 0;
ClassFunctions classfunctions;
LoadedFigures loadedfigures;
CommandFile commandfile;
MainMenu mainmenu;
Calculator calculator;
DimensionVisibilityCallback dimensionvisibilitycallback = 0;

class TestQString;

extern void open_dwg(RCCHAR *filename);

#ifdef _RCDEBUG
int gMatrixUseCnt=0;
int goffscreenbmUseCnt=0;
#endif

class RBviewmapper
{
public:
    View3dSurface *vs;
    QGraphicsItem *qgi;

    RBviewmapper()
    {
        vs=0;
        qgi=0;
    }
    RBviewmapper(View3dSurface *v, QGraphicsItem *gi)
    {
        vs = v;
        qgi = gi;
    }
    RBviewmapper(const RBviewmapper &o)
    {
        vs = o.vs;
        qgi = o.qgi;
    }
    RBviewmapper operator=(const RBviewmapper &o)
    {
        vs = o.vs;
        qgi = o.qgi;
    }
};
typedef std::vector<RBviewmapper> RBviewmapperList;

static const char *LicenseAgreement = 

    "END USER LICENSE AGREEMENT\r\n"
    "\r\n"
    "\r\n"
    "\r\n"
    "NOTICE TO USER:\r\n"
    "\r\n"
    "PLEASE READ CAREFULLY: THIS IS A CONTRACT. BY INSTALLING, COPYING OR USING\r\n"
    "THIS SOFTWARE, YOU AGREE TO BE BOUND BY ALL THE TERMS AND CONDITIONS OF THIS\r\n"
    "LICENSE AGREEMENT, ON YOUR OWN BEHALF AND ON THE BEHALF OF THE ENTITY BY\r\n"
    "WHOM YOU ARE EMPLOYED OR REPRESENT. IF YOU DO NOT AGREE WITH THE TERMS OF\r\n"
    "THIS AGREEMENT, DO NOT INSTALL, USE OR COPY THE SOFTWARE.\r\n"
    "\r\n"
    "\r\n"
    "\r\n"
    "This CAD Australia Pty Limited (\"CAD\") End User License Agreement (\"License\r\n"
    "Agreement\") is a legal agreement between you (an individual or single legal\r\n"
    "entity) and CAD, and sets forth the terms and conditions under which you are\r\n"
    "licensed to use the Software. \"Software\" means all of the contents of the\r\n"
    "CD-ROM(s) or file(s) loaded from the internet or email or any other media\r\n"
    "source, including, but not limited to (i) CAD or third party software, (ii)\r\n"
    "printed, \"online\" or electronic explanatory materials (\"Documentation\"),\r\n"
    "(iii) digital images, sample plans, images, symbols, photographs, videos or\r\n"
    "other artistic works (\"Graphical Files\").\r\n"
    "\r\n"
    "Software License is activated by the provision of a unique number (\"Key\")\r\n"
    "that is linked to the identity of your computer's hard drive.\r\n"
    "\r\n"
    "1.         GRANT OF LICENSE TO USE SOFTWARE: CAD grants you a non-exclusive\r\n"
    "license (\"License\") to use the Software in object code only (and\r\n"
    "specifically not in source code) subject to the following terms and\r\n"
    "conditions:\r\n"
    "\r\n"
    "(a) Software License: Unless otherwise agreed to in writing, CAD grants you\r\n"
    "the right to use one copy of the Software on only one computer at one\r\n"
    "location at any one time.\r\n"
    "\r\n"
    "(b) Supplementary Key Codes: Unless otherwise agreed to in writing, CAD\r\n"
    "grants you the right to use one additional unlocking Key code on an\r\n"
    "additional computer owned by the licensee. A service fee may be payable to\r\n"
    "CAD for generation of this key code number. This does not constitute the\r\n"
    "granting of an additional software License and does not allow the use of\r\n"
    "both copies of the software at the same time.\r\n"
    "\r\n"
    "(c) Site License: If you have purchased a multiple concurrent user License,\r\n"
    "CAD grants you the right to use the software on an agreed number of\r\n"
    "computers across a single internal network at any one geographical location\r\n"
    "(Site). A single unlocking Key code is provided for this license, placed on\r\n"
    "the nominated network server.\r\n"
    "\r\n"
    "(d) Student License: The Student License of the Software must be used only\r\n"
    "by a student for educational purposes while studying at an educational\r\n"
    "institution and may not be used for commercial purposes. The Student License\r\n"
    "can not be upgraded. No supplementary Key codes can be issued for a student\r\n"
    "license.\r\n"
    "\r\n"
    "(e) Time Limited: A time limited License, if available, may only be used for\r\n"
    "the duration of time purchased. This license does not grant the licensee\r\n"
    "perpetual license rights to use the software.\r\n"
    "\r\n"
    "(f) Backup: You may make a single backup copy of the Software for archival\r\n"
    "purposes.\r\n"
    "\r\n"
    "(g) Evaluation Mode: CAD grants you license to install the software and\r\n"
    "operate it in evaluation (Demo) mode for an unlimited period of time. This\r\n"
    "mode may only be used for software evaluation purposes.\r\n"
    "\r\n"
    "2.         OTHER RIGHTS AND RESTRICTIONS:\r\n"
    "\r\n"
    "(a) Reverse Engineering: You may not modify, translate, reverse engineer,\r\n"
    "decompile, disassemble, or modify the executable in any way or use any other\r\n"
    "method to convert the Software or any component of the Software into\r\n"
    "human-readable code (except to the extent that this restriction is expressly\r\n"
    "prohibited by law), create derivative works based upon the Software,\r\n"
    "Documentation, or Graphical Files or allow anyone else to do so;\r\n"
    "\r\n"
    "(b) Renting: You may not rent, lease, sub-license, lend, duplicate or\r\n"
    "distribute the Software, Documentation, or Graphical Files without the prior\r\n"
    "written consent of CAD, which, if given, is subject to the transferee's\r\n"
    "consent to the terms and conditions of this License Agreement;\r\n"
    "\r\n"
    "(c) Copying: You may not copy or reproduce the Software, Documentation, or\r\n"
    "Graphical Files (except for back-up purposes);\r\n"
    "\r\n"
    "(d) Upgrades: If the Software is being licensed to you as an upgrade or\r\n"
    "update to software previously licensed to you, you must cease using the\r\n"
    "software previously licensed to you, including any copies installed on your\r\n"
    "computer hard-disk drives, once you have successfully installed and\r\n"
    "commenced use of the Software.  You may however, maintain a copy of the\r\n"
    "software previously licensed to you for archival purposes.\r\n"
    "\r\n"
    "(e) Proprietary Notices: You may not remove or cover any proprietary notices\r\n"
    "or labels from the Software, Documentation, or Graphical Files;\r\n"
    "\r\n"
    "(f) Protection: You may not utilize any computer hardware or software\r\n"
    "designed to defeat any hardware or software copy-protection device, should\r\n"
    "the Software be equipped with such protection. You may not disable any\r\n"
    "licensing or control features of the Software.\r\n"
    "\r\n"
    "(g) Public Access: You may not copy the Software or any component of the\r\n"
    "Software onto any public network. You may not, in any manner, make it\r\n"
    "accessible to others, including via internet web site, telecommunication\r\n"
    "transmittal, electronic bulletin board, or any other form of electronic\r\n"
    "distribution system;\r\n"
    "\r\n"
    "(h) License Reissuing: If, due to failure of computer hardware or operating\r\n"
    "system or other software installed on your computer, the license\r\n"
    "authorisation Key code is rendered invalid, CAD will at its discretion\r\n"
    "reissue the licensee with another Key code. A fee for this service will be\r\n"
    "charged.\r\n"
    "\r\n"
    "3.         TRANSFERING YOUR SOFTWARE: You may permanently transfer your\r\n"
    "license to another person or entity only according to the following terms\r\n"
    "and conditions:\r\n"
    "\r\n"
    "(a) You may not transfer any license of the Software without the express\r\n"
    "written permission of CAD.\r\n"
    "\r\n"
    "(c) You may not transfer any earlier versions of Software which have been\r\n"
    "upgraded.\r\n"
    "\r\n"
    "(d) The transferor or transferee must pay to CAD a license transfer fee for\r\n"
    "each license transferred to complete the process of transfer of ownership.\r\n"
    "\r\n"
    "(e) The originally installed software and license details must be erased.\r\n"
    "You must cease using the software previously licensed to you, including any\r\n"
    "copies installed on your computer hard-disk drives.\r\n"
    "\r\n"
    "4.         TITLE: Title, ownership, rights, and intellectual property rights\r\n"
    "in and to the Software, Documentation, Graphical Files, and any other\r\n"
    "accompanying materials shall remain with CAD or the respective third party.\r\n"
    "Unauthorized copying of the Software, Documentation, Graphical Files, or\r\n"
    "Sample Drawings, or failure to comply with the foregoing restrictions, will\r\n"
    "result in the automatic termination of this License Agreement.  Unauthorized\r\n"
    "duplication of the Software, Documentation, Graphical Files, or Sample\r\n"
    "Drawings constitutes copyright infringement and is punishable in a federal\r\n"
    "criminal action by a fine of up to $250,000 and imprisonment of up to five\r\n"
    "(5) years.  In addition, federal civil penalties allow the recovery of\r\n"
    "actual damages based on the number of unauthorized copies produced or\r\n"
    "statutory damages of up to $100,000 for wilful copyright infringement.\r\n"
    "\r\n"
    "5.         THIRD PARTY CONTENT: All trademarks & logos belong to their\r\n"
    "respective owners. All Graphical Files, including symbols, images, and\r\n"
    "videos are the copyrightable subject matter of CAD or third parties. You may\r\n"
    "not sell or distribute the Graphical Files for any purpose whatsoever. You\r\n"
    "may not use the Graphical Files with other software.  CAD and third party\r\n"
    "content, symbols, or textures may not be copied, redistributed, or sold\r\n"
    "separately from the Software. CAD and third party product content is\r\n"
    "provided for representational purposes only. For information regarding the\r\n"
    "represented items, including product specification, contact the Supplier\r\n"
    "directly for more details. CAD and third parties assume no responsibility\r\n"
    "for any discrepancies between actual product appearance and that represented\r\n"
    "in the RealCAD Software.\r\n"
    "\r\n"
    "6.         LIMITED SOFTWARE WARRANTY:  CAD warrants that for a period of\r\n"
    "thirty (30) days from the date of your purchase of the Software as evidenced\r\n"
    "by a copy of your receipt, the media upon which the Software is furnished\r\n"
    "will be free from defects in materials and workmanship under normal use.\r\n"
    "EXCEPT FOR THE ABOVE EXPRESS LIMITED WARRANTIES, CAD MAKES NO WARRANTIES AS\r\n"
    "TO THE SOFTWARE, EXPRESS, IMPLIED, STATUTORY, OR IN ANY COMMUNICATION WITH\r\n"
    "YOU.  CAD SPECIFICALLY DISCLAIMS ANY OTHER WARRANTY INCLUDING THE IMPLIED\r\n"
    "WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE WITH REGARD\r\n"
    "TO THE SOFTWARE, AND THE ACCOMPANYING WRITTEN MATERIALS.  CAD DOES NOT\r\n"
    "WARRANT THAT YOUR USE OF THE SOFTWARE WILL BE UNINTERRUPTED OR THAT THE\r\n"
    "OPERATION OF THE SOFTWARE WILL BE ERROR-FREE.\r\n"
    "\r\n"
    "7.         CUSTOMER REMEDIES:  The entire liability of CAD and your\r\n"
    "exclusive remedy under the warranty provided as to Software set forth above\r\n"
    "will be, at CAD's sole discretion: (i) to replace the media; (ii) to attempt\r\n"
    "to correct or work around software errors, if any; or (iii) if the above\r\n"
    "remedies are impracticable to refund the license fee you paid, if any, for\r\n"
    "the Software.  The latter remedy is subject to pre-authorized return of the\r\n"
    "Software, Documentation and packaging to CAD or to the Authorized CAD Dealer\r\n"
    "or Reseller from whom the Software and Documentation were obtained together\r\n"
    "with a copy of your receipt.\r\n"
    "\r\n"
    "            Repaired, corrected or replaced Software and Documentation shall\r\n"
    "be covered by this limited warranty for the period remaining under the\r\n"
    "warranty that covered the original Software or advised you as to how to\r\n"
    "operate the Software, whichever is applicable. Only if you inform CAD of the\r\n"
    "problem with the Software during the applicable warranty period and provide\r\n"
    "evidence of the date you acquired the Software will CAD be obligated to\r\n"
    "honour this warranty.\r\n"
    "\r\n"
    "9.         NO WARRANTY AS TO THE SAMPLE DRAWINGS OR VIDEOS:  CAD MAKES NO\r\n"
    "WARRANTY WHATSOEVER AS TO THE SAMPLE DRAWINGS OR TRAINING VIDEOS WHICH ARE\r\n"
    "PROVIDED TO YOU SOLELY FOR DEMONSTRATIVE AND ILLUSTRATIVE PURPOSES.  THE\r\n"
    "SAMPLE DRAWINGS ARE PROVIDED TO YOU \"AS IS,\" AND CAD AND THE LICENSORS OF\r\n"
    "THE SAMPLE DRAWINGS DISCLAIM ANY AND ALL WARRANTIES WITH RESPECT TO THE\r\n"
    "SAMPLE DRAWINGS, WHETHER EXPRESS OR IMPLIED OR ARISING BY CUSTOM OR TRADE\r\n"
    "USAGE, AND, SPECIFICALLY, MAKE NO WARRANTY OF MERCHANTABILITY OR FITNESS FOR\r\n"
    "ANY PARTICULAR PURPOSE.\r\n"
    "\r\n"
    "10.       DISCLAIMER AND LIMITATION OF LIABILITY:  COMPUTER-AIDED DESIGN\r\n"
    "SOFTWARE AND OTHER TECHNICAL SOFTWARE ARE TOOLS INTENDED TO BE USED BY\r\n"
    "TRAINED AND EXPERIENCED INDIVIDUALS ONLY.  THEY ARE NOT SUBSTITUTES FOR YOUR\r\n"
    "TRAINED, EXPERIENCED AND PRACTICAL JUDGMENT.  COMPUTER-AIDED DESIGN SOFTWARE\r\n"
    "AND OTHER TECHNICAL SOFTWARE ARE INTENDED TO ASSIST WITH DESIGN, PREMISES OR\r\n"
    "PRODUCT DESIGN AND ARE NOT SUBSTITUTES FOR INDEPENDENT TESTING OF STRUCTURAL\r\n"
    "INTEGRITY, SOUNDNESS, SAFETY, AND UTILITY.  DUE TO THE LARGE VARIETY OF\r\n"
    "POTENTIAL APPLICATIONS FOR THE SOFTWARE, THE SOFTWARE HAS NOT BEEN TESTED IN\r\n"
    "ALL SITUATIONS UNDER WHICH IT MAY BE USED.  CAD SHALL NOT BE LIABLE IN ANY\r\n"
    "MANNER WHATSOEVER FOR THE RESULTS OBTAINED THROUGH THE USE OF THE SOFTWARE.\r\n"
    "PERSONS USING THE SOFTWARE ARE RESPONSIBLE FOR THE SUPERVISION, MANAGEMENT\r\n"
    "AND CONTROL OF THE SOFTWARE AND THEIR EMPLOYEES OR CONTRACTORS WHO USE THE\r\n"
    "SOFTWARE.  THIS RESPONSIBILITY INCLUDES, BUT IS NOT LIMITED TO, THE\r\n"
    "DETERMINATION OF APPROPRIATE USES FOR THE SOFTWARE AND THE SELECTION OF THE\r\n"
    "SOFTWARE AND OTHER PROGRAMS TO ACHIEVE INTENDED RESULTS.  PERSONS USING THE\r\n"
    "SOFTWARE ARE ALSO RESPONSIBLE FOR ESTABLISHING THE ADEQUACY OF INDEPENDENT\r\n"
    "PROCEDURES FOR TESTING THE RELIABILITY AND ACCURACY OF ANY PROGRAM OUTPUT,\r\n"
    "INCLUDING ALL ITEMS DESIGNED OR MEASURED BY USING THE SOFTWARE.\r\n"
    "\r\n"
    "UNDER NO CIRCUMSTANCES WILL CAD, ITS RESELLERS, ITS DISTRIBUTORS OR THE\r\n"
    "LICENSORS OF THE SAMPLE DRAWINGS BE LIABLE FOR ANY DAMAGES, WHETHER ARISING\r\n"
    "FROM TORT OR CONTRACT, INCLUDING LOSS OF DATA, LOST PROFITS, COST OF COVER,\r\n"
    "OR OTHER SPECIAL, INCIDENTAL, CONSEQUENTIAL OR INDIRECT DAMAGES ARISING OUT\r\n"
    "OF THE USE OR INABILITY TO USE THE SOFTWARE, DOCUMENTATION, GRAPHICAL FILES,\r\n"
    "OR THE SAMPLE DRAWINGS. CAD, ITS RESELLERS, ITS DISTRIBUTORS AND THE\r\n"
    "LICENSORS OF THE SAMPLE DRAWINGS ASSUME NO LIABILITY OR RESPONSIBILITY FOR,\r\n"
    "AMONG OTHER THINGS, INADVERTENT DISCREPANCIES, CONSTRUCTION INTERPRETATIONS\r\n"
    "OR CONSTRUCTED RESULTS.  THESE LIMITATIONS WILL APPLY EVEN IF CAD, ITS\r\n"
    "RESELLERS, ITS DISTRIBUTORS OR THE LICENSORS OF THE SAMPLE DRAWINGS HAVE\r\n"
    "BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  YOU ACKNOWLEDGE THAT THE\r\n"
    "LICENSE FEE REFLECTS THIS ALLOCATION OF RISK.  FURTHER, IN NO EVENT SHALL\r\n"
    "THE LIABILITY OF CAD, ITS RESELLERS, ITS DISTRIBUTORS AND THE LICENSORS OF\r\n"
    "THE SAMPLE DRAWINGS UNDER ANY PROVISION OF THIS AGREEMENT EXCEED THE LICENSE\r\n"
    "FEE PAID TO CAD FOR THE SOFTWARE, DOCUMENTATION, GRAPHICAL FILES, AND THE\r\n"
    "SAMPLE DRAWINGS.  BECAUSE SOME STATES MAY NOT ALLOW THE EXCLUSION OR\r\n"
    "LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES, THE ABOVE\r\n"
    "LIMITATION MAY NOT APPLY TO YOU.\r\n"
    "\r\n"
    "11.       TERMINATION: This license shall terminate automatically if you\r\n"
    "fail to comply with the limitations described in this license. No notice\r\n"
    "shall be required from CAD to effectuate such termination. On termination,\r\n"
    "you must return or destroy all copies of the Software and Documentation.\r\n"
    "\r\n"
    "12.       GOVERNING LAW: This License Agreement shall be governed by the\r\n"
    "laws of the State of New South Wales without regard to conflicts of law\r\n"
    "principles.  This License Agreement shall not be governed by the United\r\n"
    "Nations Convention of Contracts for the International Sale of Goods, the\r\n"
    "application of which is hereby expressly excluded. If either you or CAD\r\n"
    "employs lawyers to enforce any right arising out of this Agreement, the\r\n"
    "prevailing party shall be entitled to recover reasonable lawyer's fees and\r\n"
    "costs.\r\n"
    "\r\n"
    "13.       ENTIRE AGREEMENT: This agreement constitutes the complete and\r\n"
    "exclusive agreement between you and CAD with respect to the subject matter\r\n"
    "hereof and supersedes all prior oral or written understandings,\r\n"
    "communications or agreements not specifically incorporated herein.  This\r\n"
    "agreement may not be modified except in a writing duly signed by an\r\n"
    "authorized representative of CAD and you.  THE ACCEPTANCE OF ANY PURCHASE\r\n"
    "ORDER PLACED BY YOU IS EXPRESSLY MADE CONDITIONAL ON YOUR CONSENT TO THE\r\n"
    "TERMS SET FORTH HEREIN.\r\n"
    "\r\n"
    "\r\n"
    "\r\n"
    "\r\n"
    "\r\n";

#if VERSION != TRICAD_STUDENT
VideoWindow *activevideowindow;
#endif
const char *llabel = "REGISTRATION ID  :  XX-X00-000000XXXXXXX|XXXXXXXXXXX\0001234-1234";

#define INSTALLLICENSE

#if defined(INSTALLLICENSE) && (VERSION != FLEXICAD) && ! defined(_WIN32_WCE)

//#define SCODE_KEY  585680423
//#define ACODE_KEY  937696127
#define SCODE_KEY  219048767
#define ACODE_KEY  740030213

static unsigned long encrypt(unsigned long key,unsigned long n)
{ return n * key;
}

static unsigned int encrypt(unsigned int key,unsigned int n)
{ return n * key;
}

static unsigned long decrypt(unsigned long key,unsigned long n)
{
  unsigned long sum,m;
  //unsigned int sum,m;
  sum = 1;
  m = 1 - key;
  if (key % 2 == 0)
    return 0;
  do sum += m; while (m *= 1 - key);
  return n * sum;
}

static unsigned int decrypt(unsigned int key,unsigned int n)
{
  //unsigned long sum,m;
  unsigned int sum,m;
  sum = 1;
  m = 1 - key;
  if (key % 2 == 0)
    return 0;
  do sum += m; while (m *= 1 - key);
  return n * sum;
}

char *getdrive(char *filename)
{
#ifdef _MAC
    static char drive[] = ("/");
    return drive;
#else
 static char drive[300];
 int i;
 char *s;
  strcpy(drive,filename);
  if (strchr(drive,':') != 0)
    strchr(drive,':')[2] = 0;
  else if (strncmp(drive,"\\\\",2) == 0)
    {  s = drive;
       for (i = 0 ; i < 3 ; i++)
         if (s != 0)
           s = strchr(s+1,'\\');
       if (s != 0)
         s[1] = 0;
    }
  return drive;
#endif
}

void GetNetworkSerial(char *windir,char *fname,DWORD *serial)
{
 DWORD dlserial,dinfo1,dinfo2,dkey1,dkey2;
 unsigned int lserial,info1,info2,key1,key2;
 char key[300],value[300],name[300],sysname[300];
 DWORD dmaxlen,dflags;
 unsigned int  maxlen,flags;
 int notread,notwritten;
 FILE *infile;

  GetVolumeInformation(getdrive((char*)windir),name,298,&dlserial,&dmaxlen,&dflags,sysname,298);
#ifdef _MAC
  lserial = (unsigned int)dlserial;
  info1 = lserial & 0xFFFF;
  key1 = encrypt(SCODE_KEY,info1 * 97 + unsigned(fname[0] & 63));
  info2 = (lserial >> 16) & 0xFFFF;
  //info2 = (lserial >> 8) & 0xFFFF;
  key2 = encrypt(SCODE_KEY,info2 * 72 + unsigned(fname[0] & 63));
  sprintf(key,"%8.8x-%8.8x",key1,key2);
  _strupr(key);
#else
  dinfo1 = dlserial & 0xFFFF;
  dkey1 = encrypt(SCODE_KEY,dinfo1 * 97 + (fname[0] & 63));
  dinfo2 = (dlserial >> 16) & 0xFFFF;
  dkey2 = encrypt(SCODE_KEY,dinfo2 * 72 + (fname[0] & 63));
  sprintf(key,"%8.8lx-%8.8lx",dkey1,dkey2);
  _strupr(key);
#endif
  infile = fopen(fname,"r");
  if (infile != 0)
    {  fclose(infile);
       memset(value,0,300);
       notread = GetPrivateProfileString("Cad",key,"",value,300,fname) == 0;
    }
  else
    {  strcpy(value,"");
       notread = 2;
       notwritten = 1;
    }
#ifdef _MAC
  if (strlen(value) > 0 && sscanf(value,"%8x-%8x",&key1,&key2) == 2)
  {
      key1 = decrypt(SCODE_KEY,key1);
      key2 = decrypt(SCODE_KEY,key2);

      *serial = (key1 / 97) | ((key2 / 72) << 16);
      if (key1 % 97 != unsigned(fname[0] & 63) || key2 % 72 != unsigned(fname[0] & 63))
        *serial = 0;
      notwritten = 0;
  }
#else
  if (strlen(value) > 0 && sscanf(value,"%8lx-%8lx",&dkey1,&dkey2) == 2)
  {
      dkey1 = decrypt(SCODE_KEY,(unsigned long)dkey1);
      dkey2 = decrypt(SCODE_KEY,(unsigned long)dkey2);

      *serial = (dkey1 / 97) | ((dkey2 / 72) << 16);
      if (dkey1 % 97 != unsigned(fname[0] & 63) || dkey2 % 72 != unsigned(fname[0] & 63))
        *serial = 0;
      notwritten = 0;
  }
#endif
  else if (notread != 2)
    {  *serial = 0;
       notwritten = WritePrivateProfileString("Cad",key,"?",fname) == 0;
    }
  if (notwritten)
    RCMessageBox(cadwindow->gethwnd(),"The license information on the server could not be accessed.","CAD",MB_ICONINFORMATION);
}

void UpdateNetworkKeys(char *filename,DWORD serial)
{
 DWORD dinfo1,dinfo2,dkey1,dkey2;
 unsigned int info1,info2,key1,key2;
 char key[300]={0},*p,*pairs,value[300]={0},serverfilename[300]={0};
 int i,nvalidated;

  p = pairs = new char [10000];
  if (pairs == 0)
    return;
  for (i = 0 ; i < 10000 ; i++)
      pairs[i]=0;
  nvalidated = 0;
  GetProfileString("Cad","ServerFilename","keyserver.ini",serverfilename,298);
  GetPrivateProfileString("Cad",0,"",pairs,10000,serverfilename);

  // debugging
  //for(i=0; i < 10000; i++)
  //    qDebug() << "pairs[" << i << "]" << pairs[i];
  // end debugging

  for (i = 1 ; i < 11 ; i++)
    {  sprintf(key,"RealCADKey%d",i);
       memset(value,0,300); // TODO: fix getprivateprofilestring
       GetPrivateProfileString("Cad",key,"",value,300,filename);
       WritePrivateProfileString("Cad",key,value,serverfilename);
    }

  while (*pairs != 0)
    {  strcpy(key,pairs);
       pairs += strlen(pairs) + 1;
       memset(value,0,300);// TODO: fix getprivateprofilestring
       GetPrivateProfileString("Cad",key,"",value,300,serverfilename);
       _strupr(key);
       _strupr(value);
#ifdef _MAC
       if (strcmp(value,"?") == 0 && sscanf(key,"%8x-%8x",&key1,&key2) == 2)
       {
            key1 = decrypt(SCODE_KEY,key1);
            key2 = decrypt(SCODE_KEY,key2);
            info1 = serial & 0xFFFF;
            key1 = encrypt(SCODE_KEY,info1 * 97 + (key1 % 97));
            info2 = (serial >> 16) & 0xFFFF;
            //info2 = (serial >> 8) & 0xFFFF;
            key2 = encrypt(SCODE_KEY,info2 * 72 + (key2 % 72));
            sprintf(value,"%8.8x-%8.8x",key1,key2);
            _strupr(value);
            WritePrivateProfileString("Cad",key,value,serverfilename);
            nvalidated++;
       }
#else
       if (strcmp(value,"?") == 0 && sscanf(key,"%8lx-%8lx",&dkey1,&dkey2) == 2)
       {
            dkey1 = decrypt(SCODE_KEY,(unsigned long)dkey1);
            dkey2 = decrypt(SCODE_KEY,(unsigned long)dkey2);
            dinfo1 = serial & 0xFFFF;
            dkey1 = encrypt(SCODE_KEY,dinfo1 * 97 + (dkey1 % 97));
            dinfo2 = (serial >> 16) & 0xFFFF;
            dkey2 = encrypt(SCODE_KEY,dinfo2 * 72 + (dkey2 % 72));
            sprintf(value,"%8.8lx-%8.8lx",dkey1,dkey2);
            _strupr(value);
            WritePrivateProfileString("Cad",key,value,serverfilename);
            nvalidated++;
       }
#endif
    }
  delete [] p;
  if (nvalidated > 0)
    {char message[300];
       sprintf(message,"%d new network client key%s validated.",nvalidated,nvalidated == 1 ? " has" : "s have");
      RCMessageBox(cadwindow->gethwnd(),message,"License Server",MB_ICONINFORMATION);
    }
}


extern void RunVideo(int);
extern void OpenPDF(RCCHAR *title);
extern void OpenBrowser(char *url);

void make_initial_key(char *filename,char *label1,int application)
{char licensestring[300],windir[300],name[300],sysname[300];
 //DWORD serial,maxlen,flags,key1,key2;
 DWORD dserial,dmaxlen,dflags,dkey1,dkey2;;
 unsigned int serial,maxlen,flags,key1,key2;
 int days,ret;

  days = (int)(time(0L) / 86400L);

  //GetWindowsDirectory(windir,255);
#ifdef _MAC
  uuid_t id;
  struct timespec wait;
  wait.tv_nsec=0;
  wait.tv_sec=0;

  if((ret = gethostuuid(id,&wait)) == 0)
  {
      qDebug() << "got an id";
      serial=0;
      for(int i=0; i<sizeof(uuid_t); i++)
        serial += id[i];
  }
  else
      qDebug() << "did no get an id";

  //serial = (unsigned int)dserial;

#else
  GetVolumeInformation(getdrive(filename),name,298,&dserial,&dmaxlen,&dflags,sysname,298);
#endif
#if _MAC
  srandom((unsigned int)timeGetTime());
  key1 = encrypt(SCODE_KEY,(unsigned int)(days * 80 * 62 + application * 62 + (random() % 62)));
  key2 = encrypt(SCODE_KEY,(unsigned int)(((serial + (serial >> 16)) & 0xFFFF) * 63 + (random() % 63)));
  sprintf(licensestring,"%8.8x-%8.8x",key1,key2);
#else
  srand(timeGetTime());
  dkey1 = encrypt(SCODE_KEY,(unsigned long)(days * 80 * 62 + application * 62 + (rand() % 62)));
  dkey2 = encrypt(SCODE_KEY,(unsigned long)(((dserial + (dserial >> 16)) & 0xFFFF) * 63 + (rand() %63)));
  sprintf(licensestring,"%8.8lx-%8.8lx",dkey1,dkey2);
#endif

  //sprintf(licensestring,"%8.8lx-%8.8lx",key1,key2);
  //sprintf(licensestring,"%8.8x-%8.8x",key1,key2);
  _strupr(licensestring);
  WritePrivateProfileString("Cad",label1,licensestring,filename);
}

class LDButtonDialogControl : public ButtonDialogControl
  {public:
     LDButtonDialogControl(int id) : ButtonDialogControl(id) {}
     int select(Dialog *);
  };

int LDButtonDialogControl::select(Dialog *dialog)
{RCCHAR key[300];
 HGLOBAL hglobal;
 RCCHAR *clipdata;
 DWORD size;
 QClipboard *clipboard = 0;

  if (id == 103)
    OpenPDF(_RCT("LANDWorksCAD-Purchase-Form.PDF"));
  else if (id == 108)
  {
      if (dialog->currentvalue(107,key,300))
      {
          //if (OpenClipboard(cadwindow->gethwnd()))
          if ((clipboard = QApplication::clipboard()))
          {
              //if ((hglobal = GlobalAlloc(0,strlen(key) + 1)) != 0)
              //{
                  //if ((clipdata = (RCCHAR *) GlobalLock(hglobal)) != 0)
                  //{
                      //strcpy(clipdata,key);
                      //GlobalUnlock(clipdata);
                      //EmptyClipboard();
                      //SetClipboardData(CF_TEXT,clipdata);
                      clipboard->clear();
                      clipboard->setText(QString(key));
                  //}
              //}
              //CloseClipboard();
          }
      }
  }
  else if (id == 109)
  {
      //if (OpenClipboard(cadwindow->gethwnd()))
      if ((clipboard = QApplication::clipboard()))
      {
          //if ((hglobal = GetClipboardData(CF_TEXT)) != NULL)
          //{
                 //size = GlobalSize(hglobal);
                 //if (size >= 300) size = 299;
                 //if ((clipdata = (RCCHAR *) GlobalLock(hglobal)) != NULL)
                 //{
                      strncpy(key,clipboard->text().data(),clipboard->text().length());
                      key[clipboard->text().length()] = 0;
                      if (strchr(key,'\r') != 0) *strchr(key,'\r') = 0;
                      if (strchr(key,'\n') != 0) *strchr(key,'\n') = 0;
                      ((StringDialogControl *)dialog->getcontrol(110))->change(dialog,key);
                      //GlobalUnlock(clipdata);
                 //}
          //}
          //CloseClipboard();
      }
  }
  else if (id == 113)
    dialog->store();
  return id == 113 ? 113 : 0;
}

class LDRadioButtonDialogControl : public RadioButtonDialogControl
  {private:
     int loading;
   public:
     LDRadioButtonDialogControl(int id,int id1,int id2,int *v) : RadioButtonDialogControl(id,id1,id2,v) {  loading = 1;  }
     void load(Dialog *dialog);
     int select(Dialog *);
  };

void LDRadioButtonDialogControl::load(Dialog *dialog)
{ RadioButtonDialogControl::load(dialog);
  select(dialog);
  loading = 0;
}

int LDRadioButtonDialogControl::select(Dialog *dialog)
{int networklicense,bought,online;

  if (! loading)
    RadioButtonDialogControl::select(dialog);

#if 1
  // for new V7 license dialog
  bought = 1;
#else
  bought = SendDlgItemMessage(dialog->gethdlg(),100,BM_GETCHECK,0,0) == BST_CHECKED;
#endif
  networklicense = SendDlgItemMessage(dialog->gethdlg(),105,BM_GETCHECK,0,0) == BST_CHECKED;
  online = SendDlgItemMessage(dialog->gethdlg(),111,BM_GETCHECK,0,0) == BST_CHECKED;

  EnableWindow(GetDlgItem(dialog->gethdlg(),102),! bought && ! online);
  EnableWindow(GetDlgItem(dialog->gethdlg(),103),! bought);
  EnableWindow(GetDlgItem(dialog->gethdlg(),104),bought);
  EnableWindow(GetDlgItem(dialog->gethdlg(),105),bought);
  EnableWindow(GetDlgItem(dialog->gethdlg(),106),bought && networklicense);
  EnableWindow(GetDlgItem(dialog->gethdlg(),107),! networklicense && ! online);
  EnableWindow(GetDlgItem(dialog->gethdlg(),108),! networklicense && ! online);
  EnableWindow(GetDlgItem(dialog->gethdlg(),109),bought && ! networklicense && ! online);
  EnableWindow(GetDlgItem(dialog->gethdlg(),110),bought && ! networklicense && ! online);
  EnableWindow(GetDlgItem(dialog->gethdlg(),111),bought && ! networklicense);
  EnableWindow(GetDlgItem(dialog->gethdlg(),112),bought && ! networklicense);
  EnableWindow(GetDlgItem(dialog->gethdlg(),113),bought && ! networklicense && online);
  EnableWindow(GetDlgItem(dialog->gethdlg(),114),bought && ! networklicense);
  EnableWindow(GetDlgItem(dialog->gethdlg(),115),bought && ! networklicense);
  EnableWindow(GetDlgItem(dialog->gethdlg(),1),bought && ! networklicense && ! online);

  return 0;

}

typedef int (WINAPI *CheckLicense)(int);

extern char *Expand(const char *string,int buffer);
extern RCCHAR *Expand(const RCCHAR *string,int buffer);

char *Expand(const char *string,int buffer)
{
    return (char*)Expand(CHAR2RCCHR(string),buffer);
}

RCCHAR *Expand(const RCCHAR *string,int buffer)
{static RCCHAR estring[20][3000];
 RCCHAR *s = estring[buffer];
  while (*string != 0)
    {  if (*string == ' ')
         {  *s++ = '%';  *s++ = '2';  *s++ = '0';  string++;
         }
       else
         *s++ = *string++;
    }
  *s = 0;
  return estring[buffer];
}

extern int GetAddr(char *hostname,int port,struct sockaddr *result);
extern int GetAddr(RCCHAR *hostname,int port,struct sockaddr *result);

int GetAddr(char *hostname,int port,struct sockaddr *result)
{
    return GetAddr(CHAR2RCCHR(hostname),port,result);
}

int GetAddr(RCCHAR *hostname,int port,struct sockaddr *result)
{struct hostent *host;
 SOCKADDR_IN address;
 unsigned long iaddr;
 RCCHAR hostn[300];

  strcpy(hostn,hostname);
  if (strchr(hostn,':') != 0)
    {  port = atoi(strchr(hostn,':') + 1);
       *strchr(hostn,':') = 0;
    }

  memset(result,0,sizeof(*result));
  memset(&address,0,sizeof(address));

  if ((iaddr = inet_addr(hostn)) != INADDR_NONE)
    {  address.sin_family = AF_INET;
       address.sin_port = htons((short)port);
       address.sin_addr.s_addr = iaddr;
       memcpy(result,&address,sizeof(address));
       host = (struct hostent *) -1;
    }
  else if ((host = gethostbyname(hostn)) != 0)
    {  address.sin_family = AF_INET;
       address.sin_port = htons((short)port);
       memcpy(&address.sin_addr,host->h_addr_list[0],host->h_length);
       memcpy(result,&address,sizeof(address));
    }

   return host != 0;
}

extern RCCHAR *GetWebPageData(RCCHAR *url);
extern char *GetWebPageData(char *url);

/*
char *GetWebPageData(char *url)
{
    return (char*)GetWebPageData(CHAR2RCCHR(url));
}
*/

#if USING_WIDGETS
QNetworkAccessManager *networkmanager = 0;

RCCHAR *GetWebPageData(RCCHAR *url)
{
    return QString(GetWebPageData(QString(url).toLatin1().data())).data();
}

char *GetWebPageData(char *url)
{
    // reusing the network access manager and reply
    static char creply[10000]={0};
    if(networkmanager == 0)
    {
        networkmanager = new QNetworkAccessManager(app);
        app->connect(networkmanager, SIGNAL(finished(QNetworkReply*)),
                 app, SLOT(webPageReplyFinished(QNetworkReply*)));
    }

    if(app->mlastWebPageReply != 0)
    {
        app->mlastWebPageReply->deleteLater();
        app->mlastWebPageReply = 0;
    }

    // various urls for testing
    //sprintf(url,"http://www.realcad.info/realcad-licensing/Licensee.ASP?EMailAddress=%s&SerialNumber=%s&GenerateKey=1&Lock=%s",emailaddress,serialnumber,licensestring1)
    //sprintf(request,"POST http://%s%s HTTP/1.1\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close\r\nContent-length: %d\r\n\n%s\n",address,page,strlen(content),content);
    //http://example.markit.com/TestApi/SampleRequest/xml?count=3&echo=example
    //QString urlStr("http://example.markit.com/TestApi/SampleRequest/xml?count=3&echo=example");

    QString urlStr(url);
    QUrl inUrl(urlStr);
    QUrl outUrl = inUrl.scheme() + "://" + inUrl.host() + inUrl.path();
    QByteArray data = inUrl.query().toLatin1();

    //qDebug() << "inUrl.authority " << inUrl.authority();
    //qDebug() << "inUrl.query " << inUrl.query();
    //qDebug() << "inUrl.fileName " << inUrl.fileName();
    //qDebug() << "inUrl.path " << inUrl.path();
    //qDebug() << "inUrl.host " << inUrl.host();
    //qDebug() << "inUrl.scheme " << inUrl.scheme();

    //qDebug() << "outUrl" << outUrl;
    //qDebug() << "data" << data;

    QNetworkRequest request(outUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    networkmanager->post(request,data);

    while(app->mlastWebPageReply == 0)
    {
        app->processEvents();
    }

    QByteArray replyData = app->mlastWebPageReply->readAll();
    QString result(replyData);

    //qDebug() << "result " << result;

    strcpy(creply,result.data());
    // forcing a key for testing and debugging
    // paste this text to the RealCAD-V7.INI
    // [Cad]
    // windows
    // RealCADKey1=ACA3B46D-96A6F566
    // mac
    // RealCADKey1=2AB61A30-786B03D5
    // RealCADKey2=
    // AcceptedAgreement=1
    // KeyInfo=5802d03a
    // uncomment the next line
    // windows
    //strcpy(creply,"A new key \"8711AB9F-79538182\"");
    // mac
    //strcpy(creply,"A new key \"23972110-8E0FAC97\"");

    delete app->mlastWebPageReply;
    app->mlastWebPageReply = 0;
    app->processEvents();

    return creply;
}
#else
RCCHAR *GetWebPageData(RCCHAR *url)
{
 static RCCHAR reply[10000];
 RCCHAR address[3000],page[3000],content[3000],request[3000],proxyserveripport[300],proxyserveripaddress[300];
 struct sockaddr sockaddr;
 SOCKET s;
 int port = 80;  //  HTTP port
 int nbytes;
 HKEY hkey;
 DWORD length,type;


  strcpy(proxyserveripport,"");
  strcpy(proxyserveripaddress,"");
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,_RCT("Software\\RealCAD"),0,KEY_READ,&hkey) == ERROR_SUCCESS)
    {  length = 300;
       RegQueryValueEx(hkey,"ProxyServerIPAddress",0,&type,(unsigned char *)proxyserveripaddress,&length);
       length = 300;
       RegQueryValueEx(hkey,"ProxyServerIPPort",0,&type,(unsigned char *)proxyserveripport,&length);
    }

  strcpy(page,Expand(url,0));
  strcpy(url,page);


  reply[0] = 0;

  strcpy(address,url+7);
  if (strchr(address,'/') != 0)
    *strchr(address,'/') = 0;

  if (strchr(url+7,'/') != 0)
    strcpy(page,strchr(url+7,'/'));
  else
    strcpy(page,"/");
  if (strncmp(page,"/~",2) == 0 && strchr(page+1,'/') == 0)
    strcat(page,"/");

  if (strchr(page,'?') != 0)
    {  strcpy(content,strchr(page,'?')+1);
       *strchr(page,'?') = 0;
    }
  else
    strcpy(content,"");

  if (strlen(proxyserveripaddress) == 0)
    sprintf(request,"POST %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close\r\nContent-length: %d\r\n\n%s\n",page,address,strlen(content),content);
  else
    {  sprintf(request,"POST http://%s%s HTTP/1.1\r\nContent-type: application/x-www-form-urlencoded\r\nConnection: close\r\nContent-length: %d\r\n\n%s\n",address,page,strlen(content),content);
       strcpy(address,proxyserveripaddress);
       port = atoi(proxyserveripport);
    }

  if (GetAddr(address,port,&sockaddr))
    {  s = socket(AF_INET,SOCK_STREAM,0);
       if (RCconnect(s,&sockaddr,sizeof(sockaddr)) >= 0)
         {  send(s,request,strlen(request),0);

            length = 0;
            for (;;)
         			  {  nbytes = recv(s,reply+length,512,0);
                 if (nbytes == 0 || nbytes == SOCKET_ERROR)
                   break;
                 length += nbytes;
       
              }

         }
       closesocket(s);
    }

  return reply;
}
#endif

extern int check_license(char *label,char* label1, char *label2,int application,int showmessages);
extern RCINT check_license(RCCHAR *label,RCCHAR* label1, RCCHAR *label2,int application,int showmessages);

//RCINT check_license(RCCHAR *label,RCCHAR* label1, RCCHAR *label2,int application,int showmessages)
int check_license(char *label,char* label1, char *label2,int application,int showmessages)
{
 char licensestring1[300]={0},licensestring2[300]={0},windir[300]={0},name[300]={0},sysname[300]={0},filename[300]={0},title[300]={0},password[300]={0},key[300]={0},text[300]={0},emailaddress[300]={0},serialnumber[300]={0},oldlicensefilename[300]={0};
 RCCHAR rclicensestring1[300]={0},rclicensestring2[300]={0},rcemailaddress[300]={0},rcserialnumber[300]={0};
 unsigned long cs1,cs2;
 unsigned char a[1]={0},app[300]={0},pass[300]={0},*s;
 int i,k,status,count,button,day,day1,day2,lastday,licenceperiod,online;
 //DWORD serial,maxlen,flags;
 //DWORD key1,key2,key3,key4;
 DWORD dserial,dmaxlen,dflags;;
 unsigned int serial,maxlen,flags;
 unsigned int key1,key2,key3,key4;
 RCHKEY hkey;

#if 0
//#ifdef _RCDEBUG
//#ifdef NO_LICENSE
  return 1;
#else
  if (application == 0)
    program->setlicensed(0);

  if (strncmp(_RCT(llabel),_RCT("REGISTRATION ID  :  XX-X00-000000XXXXXXX"),40) != 0)
    {
       program->setlicensed(0);

       strcpy((char *)app,llabel);  _strlwr((char *)app);
       strcpy((char *)pass,llabel+strlen(llabel)+1);  _strlwr((char *)pass);
       sscanf((char *)pass,"%ld-%ld",&cs1,&cs2);
       cs1 += cs2 * 10000L;

       if (application == 0 || application == 2)
         {  if (check_license((char*)"Machining Application", (char*)"RealCADKey7", (char*)"RealCADKey8",3,1))
              return 1;
            if (check_license((char*)"Architect Application", (char*)"RealCADKey3", (char*)"RealCADKey4",1,1))
              return 1;
         }

       if (application == 0 || application == 2)
         a[0] = 'c';
       else if (application == 3)
         a[0] = 'm';
       else if (application == 1)
         a[0] = 'a';
       cs2 = 241435679L + a[0] * 223465677L;
       s = app;
       while (*s)
         {  if (*s != ' ')
              cs2 += *s * 246815677L;
            s++;
         }
       cs2 = cs2 % 100000000L;

       if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,_RCT("SOFTWARE\\CCICASI"),0,KEY_READ,&hkey) == ERROR_SUCCESS)
         {long length;
            length = 300;
            strcpy((char *)key,"Level1");
            if (a[0] == 'm') strcat((char *)key,"M");
            if (a[0] == 'a') strcat((char *)key,"A");
            if (RegQueryValue(hkey,key,password,&length) == ERROR_SUCCESS)
              {  k = 37;
                 for (i = 0 ; i < (int)length-1 ; i++)
                   {  password[i] ^= (unsigned char)(k | 0x80);
                      k *= 67;
                   }
                 if (strcmp(llabel,password) != 0)
                   return 0;
              }
            else
              {  RegCloseKey(hkey);
                 return 0;
              }
            strcpy((char *)key,"Level2");
            if (a[0] == 'm') strcat((char *)key,"M");
            if (a[0] == 'a') strcat((char *)key,"A");

            length = 300;
            if (RegQueryValue(hkey,key,password,&length) == ERROR_SUCCESS)
              {  for (i = 0 ; i < (int)length-1 ; i++)
                   {  password[i] ^= (unsigned char)(k | 0x80);
                      k *= 67;
                   }
                 if (strcmp(llabel+strlen(llabel)+1,password) != 0)
                   return 0;
              }
            else
              {  RegCloseKey(hkey);
                 return 0;
              }
            RegCloseKey(hkey);
         }
       else
         return 0;
       return cs1 == cs2;
    }

  if (application == 0)
    program->setlicensed(1);  //  In the demo version all functions should be available.

  strcpy(title,"Software License - ");  strcat(title,label);

  cadwindow->prompt->normalprompt("Checking license validity...");
  //program->setbusy(1);

  qDebug() << "license filename : " << QString(home.getlicensefilename());

  if (strlen(home.getlicensefilename()) > 0)
    {
       // TODO: get rid of this as we dont use it any more
       // Copy the license file information from win.ini if there is nothing in the license file at the moment
       GetWindowsDirectory(oldlicensefilename,255);
       // check for a seperator Windows does not add one to the end??
       if( !QString(oldlicensefilename).endsWith(QDir::separator()))
           strcat(oldlicensefilename,QString(QDir::separator()).data());
       strcat(oldlicensefilename,"win.ini");

       strcpy(windir,home.getlicensefilename());
       if (strlen(home.getlicensefilename()) > 0)
         GetPrivateProfileString("Cad","FileName","",filename,298,home.getlicensefilename());
       else  
         GetProfileString("Cad","Filename","",filename,298);

       GetPrivateProfileString("Cad",label1,"",licensestring1,300,_RCT(strlen(filename) == 0 ? windir : filename));
       if (strlen(licensestring1) == 0)
         {  GetPrivateProfileString("Cad",label1,"",licensestring1,300,_RCT(oldlicensefilename));
            WritePrivateProfileString("Cad",label1,licensestring1,_RCT(strlen(filename) == 0 ? windir : filename));
         }
       strcpy(rclicensestring1,licensestring1);
       GetPrivateProfileString("Cad",label2,"",licensestring2,298,_RCT(strlen(filename) == 0 ? windir : filename));
       if (strlen(licensestring2) == 0)
         {  GetPrivateProfileString("Cad",label2,"",licensestring2,300,_RCT(oldlicensefilename));
            WritePrivateProfileString("Cad",label2,licensestring2,_RCT(strlen(filename) == 0 ? windir : filename));
         }

    }
  else
    {  GetWindowsDirectory(windir,255);
       strcat(windir,"\\win.ini");
       if (strlen(home.getlicensefilename()) > 0)
         GetPrivateProfileString("Cad","FileName","",filename,298,home.getlicensefilename());
       else  
         GetProfileString("Cad","Filename","",filename,298);
    }

  for (count = 0 ;; count++)
    {  for (i = 0 ; i < 2 ; i++)
         {  GetPrivateProfileString("Cad",label1,"",licensestring1,300,_RCT(strlen(filename) == 0 ? windir : filename));
            if (strlen(filename) > 0 || strlen(licensestring1) > 0)
              break;
            make_initial_key(windir,(char*)label1,application);
         }
       strcpy(rclicensestring1,licensestring1);
       GetPrivateProfileString("Cad",label2,"",licensestring2,298,_RCT(strlen(filename) == 0 ? windir : filename));
       if (strlen(filename) > 0)
         {
           GetNetworkSerial(windir,filename,&dserial);// network license client comes here
           serial = (unsigned int)dserial;
         }
       else
         {  name[0] = sysname[0] = 0;
            GetVolumeInformation(getdrive(windir),name,298,&dserial,&dmaxlen,&dflags,sysname,298);
            serial = (unsigned int)dserial;
         }
       qDebug() << "serial " << serial;
       qDebug() << "licensestring1 " << licensestring1;

#ifdef _MAC
       if (sscanf(licensestring1,"%x-%x",&key1,&key2) == 2)
#else
       if (sscanf(licensestring1,"%lx-%lx",&key1,&key2) == 2)
#endif
       {
           //qDebug() << "key1 " << key1;
           //qDebug() << "key2 " << key2;

#ifdef _MAC
            key1 = decrypt(SCODE_KEY,key1);
            key2 = decrypt(SCODE_KEY,key2);
/*
            qDebug() << "decrypt key1 " << key1;
            qDebug() << "decrypt key2 " << key2;

            qDebug() << "key2 / 63 " << key2 / 63;
            qDebug() << "((serial + (serial >> 16)) & 0xFFFF) " << ((serial + (serial >> 16)) & 0xFFFF);

            qDebug() << "((key1 / 62)%80) " << ((key1 / 62)%80);
            qDebug() << "(unsigned) application " << (unsigned) application;
*/
#else
           key1 = decrypt(SCODE_KEY,(unsigned long)key1);
           key2 = decrypt(SCODE_KEY,(unsigned long)key2);
/*
           qDebug() << "decrypt key1 " << key1;
           qDebug() << "decrypt key2 " << key2;

           qDebug() << "key2 / 63 " << key2 / 63;
           qDebug() << "((serial + (serial >> 16)) & 0xFFFF) " << ((serial + (serial >> 16)) & 0xFFFF);

           qDebug() << "((key1 / 62)%80) " << ((key1 / 62)%80);
           qDebug() << "(unsigned) application " << (unsigned) application;
*/
#endif

#ifdef _MAC
          if (strlen(filename) == 0 && key2 / 63 != ((serial + (serial >> 16)) & 0xFFFF) || ((key1 / 62)%80) != (unsigned) application)
#else
          if (strlen(filename) == 0 && key2 / 63 != ((dserial + (dserial >> 16)) & 0xFFFF) || ((key1 / 62)%80) != (unsigned) application)
#endif
          {  WritePrivateProfileString("Cad",label1,"",_RCT(strlen(filename) > 0 ? filename : windir));
             WritePrivateProfileString("Cad",label2,"",_RCT(strlen(filename) > 0 ? filename : windir));
             //qDebug() << "license continue 1 ";
             continue;
          }
       }
       else if (strlen(filename) == 0)
         {  WritePrivateProfileString("Cad",label1,"",_RCT(strlen(filename) > 0 ? filename : windir));
            WritePrivateProfileString("Cad",label2,"",_RCT(strlen(filename) > 0 ? filename : windir));
            //qDebug() << "license continue 2 ";
            continue;

         }
        strcpy(rclicensestring1,licensestring1);
       if ((serial != 0 || LOBYTE(LOWORD(GetVersion())) == 3) &&
       //if (serial != 0 &&
#ifdef _MAC
           sscanf(licensestring1,"%x-%x",&key1,&key2) == 2 &&
           sscanf(licensestring2,"%x-%x",&key3,&key4) == 2)
#else
           sscanf(licensestring1,"%lx-%lx",&key1,&key2) == 2 &&
           sscanf(licensestring2,"%lx-%lx",&key3,&key4) == 2)
#endif
       {
#ifdef _MAC
            key1 = decrypt(SCODE_KEY,key1);
            key2 = decrypt(SCODE_KEY,key2);
            key3 = decrypt(ACODE_KEY,key3);
            key4 = decrypt(ACODE_KEY,key4);
#else
           key1 = decrypt(SCODE_KEY,(unsigned long)key1);
           key2 = decrypt(SCODE_KEY,(unsigned long)key2);
           key3 = decrypt(ACODE_KEY,(unsigned long)key3);
           key4 = decrypt(ACODE_KEY,(unsigned long)key4);
#endif
            //  Try to match key1 to key3.
            //  key3 may be key1 + 1..12 
            for (licenceperiod = 0 ; licenceperiod <= 365 ; licenceperiod++) 
            {
              if (key1 + licenceperiod * 80 * 62 == key3 ||
                  key1 + licenceperiod * 80 * 62 == ~key3)
                break;
            }

            if ((key1 + licenceperiod * 80 * 62 == key3 || key1+ licenceperiod * 80 * 62 == ~ key3) && ((key2 == key4 || key2 == ~key4) && key2 / 63 == ((serial + (serial >> 16)) & 0xFFFF)) && ((key1 / 62)%80) == (unsigned) application)
              {
                if (strlen(filename) == 0)
                   UpdateNetworkKeys(windir,serial);// network license server comes here
                 //program->setbusy(0);
                 if (count > 0)
                  RCMessageBox(cadwindow->gethwnd(),strlen(filename) == 0 ? "The license key entry is successful." : "The license key was validated by the server.",title,MB_ICONINFORMATION);

                 day = (int)(time(0) / 86400L);
                 day1 = key1 / 80 / 62;
                 day2 = key3 / 80 / 62;

                 if (day1 != day2)
                   {  GetPrivateProfileString("Cad","KeyInfo","0",text,300,strlen(filename) > 0 ? filename : windir);
                      sscanf(text,"%x",&lastday);
#ifdef _MAC
                      lastday = decrypt(SCODE_KEY,(unsigned int)lastday);
#else
                      lastday = decrypt(SCODE_KEY,(unsigned long)lastday);
#endif
                      if (day < lastday)
                        {  day1 = day + 1;
                           day2 = day - 2;
                        } 
#ifdef _MAC
                      sprintf(text,"%8.8lx",encrypt(SCODE_KEY,(unsigned int)day));
#else
                      sprintf(text,"%8.8lx",encrypt(SCODE_KEY,(unsigned long)day));
#endif
                      WritePrivateProfileString("Cad","KeyInfo",text,strlen(filename) > 0 ? filename : windir);
                   }
  
                 if (day1 != day2 && (day < day1 || day > day2))
                   {  //  Clear the license info
                      WritePrivateProfileString("Cad",label1,"",CHAR2RCCHR(strlen(filename) > 0 ? filename : windir));
                      WritePrivateProfileString("Cad",label2,"",CHAR2RCCHR(strlen(filename) > 0 ? filename : windir));
                      RCMessageBox(cadwindow->gethwnd(),"The license has expired.",title,MB_ICONSTOP);
                   }
                 else
                   {  //  Licensed is 1 in the full licensed version and 2 in the limited 2D version and 3 in the draft version.

                      if (day + 14 >= day2 && day1 != day2 && showmessages)
                        {
                           if(day2 - day < 1)
                               sprintf(text,"Your license will expire today.");
                           else
                               sprintf(text,"Your license will expire in %d days.",day2 - day);
                           RCMessageBox(cadwindow->gethwnd(),text,title,MB_ICONSTOP);
                        }

                      if (application == 0)
                      {
#ifdef _RCDEBUGx
                          program->setlicensed(1); // pro/demo version contains all fetures
                          //program->setlicensed(2); // simulate Light version
                          //program->setlicensed(3); // simulate Draft version
#else
                          // set the app type according to the license keys
                           program->setlicensed(1 + (key1 == ~key3) + 2*(key2 == ~key4));
#endif
                           if (day1 != day2)
                             program->setdaysleft(day2 - day);

                      }
                      cadwindow->updatetitle();
                      cadwindow->prompt->normalprompt(NCWIN+1);

                      return 1;
                   } 
              }
            else
             RCMessageBox(cadwindow->gethwnd(),"The license key is invalid or has not been validated by the server.",title,MB_ICONSTOP);
         }
       else if (strlen(licensestring1) > 0 && strlen(licensestring2) > 0)
        RCMessageBox(cadwindow->gethwnd(),"The license key is invalid or has not been validated by the server.",title,MB_ICONSTOP);

        //program->setbusy(0);
        strcpy(rclicensestring1,licensestring1);

       if (strlen(filename) > 0)
       {
            Dialog dialog("NetworkSoftwareLicense_Dialog");
            dialog.title(title);
            dialog.add(new ButtonDialogControl(100));
            dialog.add(new ButtonDialogControl(101));
            dialog.add(new ButtonDialogControl(102));
            button = dialog.process();
            if (button == 100)
              continue;
            else if (button == 101)
            {
                  //program->setbusy(0);
                  cadwindow->prompt->normalprompt(NCWIN+1);
                  cadwindow->updatetitle();
                  return 0;
            }
            else
            {
              ::app->exit();
              #ifdef _MAC
                _Exit(1);
              #else
                _exit(1);
              #endif
            }
       }
       else
       {
            Dialog dialog("LicenseKey_Dialog");
            dialog.title(title);
            dialog.add(new ButtonDialogControl(100));
            dialog.add(new ButtonDialogControl(101));
            dialog.add(new ButtonDialogControl(102));
            button = dialog.process();
            if (button == 100)
            {
                OpenPDF(_RCT("LANDWorksCAD-Purchase-Form.PDF"));
            }
            else if (button == 101)
            {
                /*Do Nothing*/;
#ifdef _MAC
                 sprintf(text,"%8.8lx",encrypt(SCODE_KEY,(unsigned int)(time(0) / 86400L)));
#else
                sprintf(text,"%8.8lx",encrypt(SCODE_KEY,(unsigned long)(time(0) / 86400L)));
#endif
                WritePrivateProfileString("Cad","KeyInfo",text,strlen(filename) > 0 ? filename : windir);
            }
            else if (button == 102)
            {
                 //program->setbusy(0);
                 cadwindow->prompt->normalprompt(NCWIN+1);
                 cadwindow->updatetitle();
                 return 0;
            }
       }

       if (strlen(filename) == 0)
         strcpy(licensestring2,"");
       else
         {  strcpy(licensestring1,"N/A");
            strcpy(licensestring2,"N/A");
         }

       for (;;)
         {
          Dialog agreementdialog("LICENSEAGREEMENT_DIALOG");
          //Dialog dialog("License_dialog");
          Dialog dialog("V7License_Dialog");
          int bought,network;


            if (GetPrivateProfileInt("Cad","AcceptedAgreement",0,windir) == 0)
              {
                 RCCHAR *rcLicenseAgreement = new RCCHAR[strlen(LicenseAgreement)+1];
                 strcpy(rcLicenseAgreement,LicenseAgreement);
                 agreementdialog.add(new ButtonDialogControl(101));
                 agreementdialog.add(new ButtonDialogControl(102));
                 //agreementdialog.add(new StringDialogControl(1000,rcLicenseAgreement,strlen(rcLicenseAgreement)));
                 agreementdialog.add(new StringDialogControl(100,rcLicenseAgreement,strlen(rcLicenseAgreement)));
                 
                 if (agreementdialog.process() != 101)
                 {
                   delete [] rcLicenseAgreement;
                   rcLicenseAgreement=0;
                   ::app->exit();
                   #ifdef _MAC
                     _Exit(1);
                   #else
                     _exit(1);
                   #endif
                 }

                 delete [] rcLicenseAgreement;
                 rcLicenseAgreement=0;
                 WritePrivateProfileString("Cad","AcceptedAgreement","1",windir);
              }


            dialog.title(title);

            bought = 1;
            network = 0;
            online = 0;
            strcpy(emailaddress,"");
            strcpy(serialnumber,"");

#if 1
            // new dialog for V7
            bought=1;
            //dialog.add(new LDRadioButtonDialogControl(100,100,101,&bought));
            //dialog.add(new LDRadioButtonDialogControl(101,100,101,&bought));
            //dialog.add(new ButtonDialogControl(102));
            //dialog.add(new LDButtonDialogControl(103));
            dialog.add(new LDRadioButtonDialogControl(104,104,105,&network));
            dialog.add(new LDRadioButtonDialogControl(105,104,105,&network));
            dialog.add(new ButtonDialogControl(106));
            dialog.add(new StringDialogControl(107,rclicensestring1,300));
            //dialog.add(new LDButtonDialogControl(108));
            //dialog.add(new LDButtonDialogControl(109));
            dialog.add(new LDRadioButtonDialogControl(111,111,112,&online));
            dialog.add(new LDRadioButtonDialogControl(112,111,112,&online));
            dialog.add(new LDButtonDialogControl(113));
            dialog.add(new StringDialogControl(110,rclicensestring2,300));
            dialog.add(new StringDialogControl(114,rcemailaddress,300));
            dialog.add(new StringDialogControl(115,rcserialnumber,300));
            dialog.add(new ButtonDialogControl(120));
#else
            dialog.add(new LDRadioButtonDialogControl(100,100,101,&bought));
            dialog.add(new LDRadioButtonDialogControl(101,100,101,&bought));
            dialog.add(new ButtonDialogControl(102));
            dialog.add(new LDButtonDialogControl(103));
            dialog.add(new LDRadioButtonDialogControl(104,104,105,&network));
            dialog.add(new LDRadioButtonDialogControl(105,104,105,&network));
            dialog.add(new ButtonDialogControl(106));
            dialog.add(new StringDialogControl(107,rclicensestring1,300));
            dialog.add(new LDButtonDialogControl(108));
            dialog.add(new LDButtonDialogControl(109));
            dialog.add(new LDRadioButtonDialogControl(111,111,112,&online));
            dialog.add(new LDRadioButtonDialogControl(112,111,112,&online));
            dialog.add(new LDButtonDialogControl(113));
            dialog.add(new StringDialogControl(110,rclicensestring2,300));
            dialog.add(new StringDialogControl(114,rcemailaddress,300));
            dialog.add(new StringDialogControl(115,rcserialnumber,300));
#endif
            status = dialog.process();
            strcpy(licensestring1,rclicensestring1);
            strcpy(licensestring2,rclicensestring2);
            strcpy(emailaddress,rcemailaddress);
            strcpy(serialnumber,rcserialnumber);

            if (status == 102)
              {Printer printer(1);
               double tabs[20];
               char line[300];
                 for (i = 0 ; i < 20 ; i++)
                   tabs[i] = i * 10.0;
                 printer.print(0);
                 if (printer.getdefined())
                   {  printer.newpage();
                      printer.setmargins(10.0,10.0,10.0,10.0);
                      printer.setcolumns(20,tabs);
                      printer.setlarge();
                      printer.printtext(_RCT("CAD Software Registration"),0,1);
                      printer.newline(3);
#if VERSION == TRICAD
                      printer.printtext("IMPORTANT : COMPLETE THIS FORM AND RETURN BY FAX",0,1);
                      printer.newline(1);
                      printer.printtext("Australia - FAX 03 93291833    Outside Australia +61 3 93291833",0,1);
                      printer.newline(2);
#endif
                      printer.printtext((char*)"\t\tContact Name:");
                      printer.newline(2);
                      printer.printtext((char*)"\t\tSchool/Company name:");
                      printer.newline(2);
                      printer.printtext((char*)"\t\tAddress:");
                      printer.newline(3);
                      printer.printtext((char*)"\t\tCity:");
                      printer.newline(2);
                      printer.printtext((char*)"\t\tState:");
                      printer.newline(2);
                      printer.printtext((char*)"\t\tCountry:");
                      printer.newline(2);
                      printer.printtext((char*)"\t\tPostcode/Zip:");
                      printer.newline(2);
                      printer.printtext((char*)"\t\tTelephone:");
                      printer.newline(2);
                      printer.printtext((char*)"\t\tFacsimile:");
                      printer.newline(2);
                      printer.printtext((char*)"\t\tEmail address:");
                      printer.newline(2);
                      printer.newline(3);
                      if (application == 0)
                        printer.printtext((char*)"\t\tApplication:\t\t\t\t\t\tMain CAD Application");
                      else if (application == 1)
                        printer.printtext((char*)"\t\tApplication:\t\t\t\t\t\tArchitect");
                      else if (application == 2)
                        printer.printtext((char*)"\t\tApplication:\t\t\t\t\t\tAnimation");
                      else if (application == 3)
                        printer.printtext((char*)"\t\tApplication:\t\t\t\t\t\tMachining");
                      printer.newline();
                      sprintf(line,"\t\tLicense Lock:\t\t\t\t\t\t%s",licensestring1);
                      printer.printtext(line);
                      printer.newline();
                      printer.printtext((char*)"\t\tLicense Lock:");
                      printer.newline(2);
                      printer.printtext((char*)"\t\tPurchased from:");
                      printer.newline(3);
#if VERSION == TRICAD
                      printer.setnormal();
                      printer.printtext("Your signature is an acknowledgment that you have read and understood the",0,1);
                      printer.newline(1);
                      printer.printtext("software license agreement contained in the manual.",0,1);
                      printer.newline(3);
                      printer.setlarge();
#endif
                      printer.printtext((char*)"\t\tSignature:");
                      printer.newline(2);
                      printer.printtext((char*)"\t\tDate:");
                      printer.newline(2);
#if VERSION == TRICAD
                      printer.setnormal();
                      printer.printtext("CPE Technologies Pty Ltd, PO Box 866, North Melbourne, Victoria, Australia.  3051",0,1);
                      printer.newline(1);
                      printer.printtext("Telephone 03 93291100 (61393291100)  Fax: 03 93291833 (61393291833)",0,1);
                      printer.newline(1);
#endif
                      continue;
                   }
              }
            else if (status == 106)
              {
                 RCOPENFILENAME ofn;
                 RCCHAR szDirName[256]={0},szFile[256]={0},szFilter[256]={0},szTitle[256]={0};
                 uint  i;
                 GetWindowsDirectory(szDirName,sizeof(szDirName));
                 strcpy(szFile,"keyserver.ini");
                 char sep[1] = { '/' };
                 *sep = QDir::separator().toLatin1();
                 if (strrchr(strlen(filename) > 0 ? filename : windir, (int)*sep) != 0)
                 {
                      strcpy(szDirName,strlen(filename) > 0 ? filename : windir);
                      *strrchr(szDirName, (int)*sep) = 0;
                 }
                 strcpy(szFilter,"Key files (*.ini)");
                 //for (i = 0; szFilter[i] != '\0'; i++)
                 //  if (szFilter[i] == '|') szFilter[i] = '\0';
                 memset(&ofn, 0, sizeof(RCOPENFILENAME));
                 ofn.lStructSize = sizeof(RCOPENFILENAME);
                 ofn.hwndOwner = cadwindow->gethwnd();
                 ofn.lpstrFilter = szFilter;
                 ofn.nFilterIndex = 1;
                 ofn.lpstrFile= szFile;
                 ofn.nMaxFile = sizeof(szFile);
                 ofn.lpstrInitialDir = szDirName;
                 ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                 ofn.lpstrDefExt = _RCT("ini");
                 strcpy(szTitle,"Select the file that contains the license information");
                 ofn.lpstrTitle = szTitle;
                 ofn.nMaxFileTitle = sizeof(ofn.lpstrTitle);
                 RCMessageBox(cadwindow->gethwnd(),"You must now locate the keyserver.ini file on a server which has a licensed copy of the CAD software installed.\nThe keyserver.ini file is usually found in the windows directory on the server.\nIt is preferable to select the server via the 'Network Neighborhood' path rather than via a mapped network drive(i.e. G:\\).",title,MB_ICONINFORMATION);
                 if (GetOpenFileName(&ofn))
                 {
                      QString nativeDir(QDir::toNativeSeparators(QString(szFile)));
                      if (strlen(home.getlicensefilename()) > 0)
                        WritePrivateProfileString("Cad","FileName",nativeDir.toLatin1(),home.getlicensefilename());
                      else  
                        WriteProfileString("Cad","FileName",nativeDir.toLatin1());
                      strcpy(filename,nativeDir.data());
                      if (strlen(filename) > 0)
                        GetNetworkSerial(windir,filename,&dserial);
                      RCMessageBox(cadwindow->gethwnd(),"The network server has been recorded.\nRestart the CAD software on the server to validate this license\nand then choose retry or OK on the dialog that follows.",title,MB_ICONINFORMATION);
                      break;
                 }
                 else
                 {
                     if (RCMessageBox(cadwindow->gethwnd(),"Do you want to use local validation?",title,MB_YESNO) == IDYES)
                     {
                           if (strlen(home.getlicensefilename()) > 0)
                             WritePrivateProfileString("Cad","FileName","",home.getlicensefilename());
                           else  
                             WriteProfileString("Cad","FileName","");
                           strcpy(filename,"");
                           break;
                     }
                 }
              }
            else if (status == 113)
              {
                 program->setbusy(1);
                 char url[300],*key;
                 //sprintf(url,"http://www.ccic.com.au/Licensee.ASP?EMailAddress=%s&SerialNumber=%s&GenerateKey=1&Lock=%s",emailaddress,serialnumber,licensestring1);
                 sprintf(url,"http://www.realcad.info/realcad-licensing/Licensee.ASP?EMailAddress=%s&SerialNumber=%s&GenerateKey=1&Lock=%s",emailaddress,serialnumber,licensestring1);
                 key = GetWebPageData(url);
                 program->setbusy(0);
                 if (strstr(key,"A new key \"") != 0)
                   {  key = strstr(key,"A new key \"") + 11;
                      if (strchr(key,'"') != 0)
                        {  *strchr(key,'"') = 0;
                           WritePrivateProfileString("Cad",label2,key,windir);
                           break;
                        }
                   } 
              }
            else if(status == 120)
            {
                // new for V7
                // go to the license help page
                OpenBrowser((char*)"http://www.cadinternational.com/cadinternational/support-answer.php?faq_id=0000015&dis=yes");
                continue;
            }
            else if (status == FALSE)
              {  //program->setbusy(0);
#if 1
                // new for V7 returns to the license key dialog
                break;
#else
                 cadwindow->prompt->normalprompt(NCWIN+1);
                 return 0;
#endif
              }
            if (strlen(filename) == 0)
              {
                //program->setbusy(1);
                 char *s;
                 WritePrivateProfileString("Cad",label2,licensestring2,_RCT(windir));
                 s = licensestring2;
                 while (*s)
                   {  if (*s >= 'A' && *s <= 'F' || *s >= 'a' && *s <= 'f' || *s >= '0' && *s <= '9' || *s == '-')
                        s++;
                      else
                        break;
                   }
                 if (*s)
                 {
                  //program->setbusy(0);
                  RCMessageBox(cadwindow->gethwnd(),"The licence key contains invalid characters.  Only the letters A to F, the numbers 0 to 9 and the minus sign (-) are allowed.",title,MB_ICONINFORMATION);
                 }
                 else
                   break;
              }
            else
              break;
         }
    }
#endif
}

#if 0
int check_license(char *label,char *label1,char *label2,int application,int showmessages)
{
    // licensing off for now
    //return 1;
    return check_license( _RCT(label), _RCT(label1), _RCT(label2),  application,  showmessages);
}
#endif

RCINT check_license(RCCHAR *label,RCCHAR *label1,RCCHAR *label2,int application,int showmessages)
{
    // licensing off for now
    //return 1;
    return check_license( QString(label).toLatin1().data(), QString(label1).toLatin1().data(), QString(label2).toLatin1().data(),  application,  showmessages);
}
#endif

class MenuActive
  {private:
     int active;
     ButtonMenu *buttonmenu;
   public:
     MenuActive() {  active = 0;  buttonmenu = NULL;  }
     ~MenuActive()  {  delete buttonmenu;  }
     int getactive(void)  {  return active;  }
     void setactive(int a)  {  active = a;  }
     void setbuttonmenu(ButtonMenu *bm)  {  buttonmenu = bm;  }
     ButtonMenu *getbuttonmenu(void)  {  return buttonmenu;  }
  };

MenuActive menuactive;

class VersionControl : public DialogControl
  {public:
     VersionControl(int id) : DialogControl(id) {}
     int type(void)  {  return 999;  }
     void load(Dialog *dialog);
  };

void VersionControl::load(Dialog *dialog)
{char string[300];
   dialog->GetDlgItemText(id,string,300);
   sprintf(string+strlen(string),"     %s  %s",__DATE__,__TIME__);
   dialog->SetDlgItemText(id,string);
}

int SendEmail(RCCHAR *emailaddress,RCCHAR *subject,RCCHAR *body,RCCHAR *attachment,RCCHAR *attachmentname)
{
#ifdef USING_WIDGETS
    //Smtp *newMail  = new Smtp("SERVER","EMAILADDRESS","PASSWORD");
    //newMail->Send("EMAILADDRESS","EMAILADDRESS"," Your Subject","My body text");
    //class emit ErrorCloseAll() / SuccessQuit()   or get QStringList ErrorMSG; to read error and data....

    //Smtp *newMail  = new Smtp("from@address.com","to@address.com"," Your Subject","My body text");
    //delete newMail;

    return 0;
#if 0
    FILE *email= popen( "mail", "wb" );

    if(email)
    {
        fprintf( email, "To: EMAILADDRESS" );
        fprintf( email, "From: EMAILADDRESS\r\n" );
        fprintf( email, "\r\n" );
        fprintf( email, "Hello ken!\r\n" );

        pclose( email );
        return 1;
    }
    return 0;  //  Failed
#endif

#else
 RCMapiMessage mm;
 RCMapiRecipDesc omrdfrom,omrdto;
 RCMapiFileDesc mfd;
 RCMAPIINIT_0 mi0;

  mm.ulReserved = 0;
  mm.lpszSubject = subject;
  mm.lpszNoteText = body;
  mm.lpszMessageType = _RCT("");
  mm.lpszDateReceived = _RCT("");
  mm.lpszConversationID = _RCT("");
  mm.lpOriginator = &omrdfrom;
  mm.nRecipCount = 1;
  mm.lpRecips = &omrdto;
  mm.nFileCount = 1;
  mm.lpFiles = &mfd;
  mm.flFlags = MAPI_LOGON_UI;

  omrdfrom.ulReserved = 0;
  omrdfrom.ulRecipClass = MAPI_ORIG;      
  omrdfrom.lpszName = _RCT("");
  omrdfrom.lpszAddress = _RCT("");
  omrdfrom.ulEIDSize = 0; 
  omrdfrom.lpEntryID = 0;
 
  omrdto.ulReserved = 0;
  omrdto.ulRecipClass = MAPI_TO;      
  omrdto.lpszName = _RCT("");
  omrdto.lpszAddress = emailaddress;
  omrdto.ulEIDSize = 0; 
  omrdto.lpEntryID = 0;
 
  mfd.ulReserved = 0;
  mfd.flFlags = 0;      
  mfd.nPosition = (ULONG)-1;
  mfd.lpszPathName = attachment;  
  mfd.lpszFileName = attachmentname;
  mfd.lpFileType = 0;

  mi0.ulVersion = MAPI_INIT_VERSION;      
  mi0.ulFlags = 0;

  RCHMODULE hmapi = LoadLibrary(_RCT("MAPI32.DLL"));
  MAPISENDMAIL *MAPISendMail;

  if (hmapi != 0)
    {  MAPISendMail = (MAPISENDMAIL *) GetProcAddress(hmapi,"MAPISendMail");
       if (MAPISendMail(0,0,(lpMapiMessage)&mm,0,0) == SUCCESS_SUCCESS)
         {  FreeLibrary(hmapi);
            return 1;  //  Succedded
         }
       FreeLibrary(hmapi);
    }

  return 0;  //  Failed
#endif
}

typedef char *(*GetVersionString)(void);

void deactivateLicense()
{
char string[300];
Dialog dialog("Unlicense_Dialog");
int unlicense;
unlicense = 0;
dialog.add(new CheckBoxDialogControl(100,&unlicense));
if (dialog.process() == TRUE && unlicense)
{
   RCOPENFILENAME ofn;
   RCCHAR szDirName[256],szFile[256],szFileTitle[256],szFilter[256],initialdirectory[300];
   UINT i;
   RCCHAR title[256];

     for (;;)
       {
          _getcwd(szDirName, 256);
          strcpy(szFile,"licenseinfo.dat");
          strcpy(szFilter,"license files (*.dat)");

          //for (i = 0; szFilter[i] != '\0'; i++)
          //  if (szFilter[i] == '|') szFilter[i] = '\0';

          memset(&ofn, 0, sizeof(OPENFILENAME));

          strcpy(initialdirectory,QDir::homePath());
          //SHGetSpecialFolderPath(NULL, initialdirectory, CSIDL_MYDOCUMENTS, 0);
          if(QDir().exists(QString(initialdirectory)+"/Documents"))
              strcat(initialdirectory,"/Documents");
#ifdef _MAC
          //strcat(initialdirectory,"/Documents");
#else
          if(QDir().exists(QString(initialdirectory)+"/My Documents"))
             strcat(initialdirectory,"/My Documents");
          else if(QDir().exists(QString(initialdirectory)+"/Documents"))
              strcat(initialdirectory,"/Documents");

#endif
          strcpy(initialdirectory,QDir::toNativeSeparators(QString(initialdirectory)));

          ofn.lStructSize = sizeof(OPENFILENAME);
          //ofn. lpstrInitialDir = _RCT("My Documents");
          ofn.hwndOwner = cadwindow->gethwnd();
          ofn.lpstrFilter = szFilter;
          ofn.nFilterIndex = 1;
          ofn.lpstrFile= szFile;
          ofn.nMaxFile = sizeof(szFile);
          ofn.lpstrFileTitle = szFileTitle;
          ofn.nMaxFileTitle = sizeof(szFileTitle);
          ofn.lpstrInitialDir = initialdirectory;
          ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST;
          ofn.lpstrDefExt = _RCT(".dat");
          ofn.lpstrFileTitle = title;
          ofn.lpstrTitle = _RCT("Select the file to save the license information into");
          if (GetSaveFileName(&ofn))
          {
               FILE *outfile = 0;//fopen(QString(szFile).toLatin1().data(),"r");
               //if(QFile(QString(szFile)).exists())
               if ((outfile = fopen(QString(szFile).toLatin1().data(),"r")) != 0)
               {
                    cadwindow->MessageBox("The license information file already exists.  You cannot overwrite this file.  The license information has not been saved.","Unlicense",MB_OK);
                    fclose(outfile);
               }
               else if ((outfile = fopen(QString(szFile).toLatin1().data(),"w"))  != 0)
               {
                    fprintf(outfile,"License File Information\n");

                    if (strlen(home.getlicensefilename()) == 0)
                      GetProfileString("Cad","RealCADKey1","",string,298);
                    else
                      GetPrivateProfileString("Cad","RealCADKey1","",string,298,home.getlicensefilename());
                    fprintf(outfile,"1:%s\n",string);

                    if (strlen(home.getlicensefilename()) == 0)
                      GetProfileString("Cad","RealCADKey2","",string,298);
                    else
                      GetPrivateProfileString("Cad","RealCADKey2","",string,298,home.getlicensefilename());
                    fprintf(outfile,"2:%s\n",string);

                    if (strlen(home.getlicensefilename()) == 0)
                      GetProfileString("Cad","RealCADKey3","",string,298);
                    else
                      GetPrivateProfileString("Cad","RealCADKey3","",string,298,home.getlicensefilename());
                    fprintf(outfile,"3:%s\n",string);

                    if (strlen(home.getlicensefilename()) == 0)
                      GetProfileString("Cad","RealCADKey4","",string,298);
                    else
                      GetPrivateProfileString("Cad","RealCADKey4","",string,298,home.getlicensefilename());
                    fprintf(outfile,"4:%s\n",string);

                    if (strlen(home.getlicensefilename()) == 0)
                      GetProfileString("Cad","RealCADKey5","",string,298);
                    else
                      GetPrivateProfileString("Cad","RealCADKey5","",string,298,home.getlicensefilename());
                    fprintf(outfile,"5:%s\n",string);

                    if (strlen(home.getlicensefilename()) == 0)
                      GetProfileString("Cad","RealCADKey6","",string,298);
                    else
                      GetPrivateProfileString("Cad","RealCADKey6","",string,298,home.getlicensefilename());
                    fprintf(outfile,"6:%s\n",string);

                    if (strlen(home.getlicensefilename()) == 0)
                      GetProfileString("Cad","RealCADKey7","",string,298);
                    else
                      GetPrivateProfileString("Cad","RealCADKey7","",string,298,home.getlicensefilename());
                    fprintf(outfile,"7:%s\n",string);

                    if (strlen(home.getlicensefilename()) == 0)
                      GetProfileString("Cad","RealCADKey8","",string,298);
                    else
                      GetPrivateProfileString("Cad","RealCADKey8","",string,298,home.getlicensefilename());
                    fprintf(outfile,"8:%s\n",string);

                    fclose(outfile);

                    WriteProfileString("Cad","RealCADKey1","");
                    WriteProfileString("Cad","RealCADKey2","");
                    WriteProfileString("Cad","RealCADKey3","");
                    WriteProfileString("Cad","RealCADKey4","");
                    WriteProfileString("Cad","RealCADKey5","");
                    WriteProfileString("Cad","RealCADKey6","");
                    WriteProfileString("Cad","RealCADKey7","");
                    WriteProfileString("Cad","RealCADKey8","");

                    if (strlen(home.getlicensefilename()) > 0)
                      {  WritePrivateProfileString("Cad","RealCADKey1","",home.getlicensefilename());
                         WritePrivateProfileString("Cad","RealCADKey2","",home.getlicensefilename());
                         WritePrivateProfileString("Cad","RealCADKey3","",home.getlicensefilename());
                         WritePrivateProfileString("Cad","RealCADKey4","",home.getlicensefilename());
                         WritePrivateProfileString("Cad","RealCADKey5","",home.getlicensefilename());
                         WritePrivateProfileString("Cad","RealCADKey6","",home.getlicensefilename());
                         WritePrivateProfileString("Cad","RealCADKey7","",home.getlicensefilename());
                         WritePrivateProfileString("Cad","RealCADKey8","",home.getlicensefilename());
                      }

                    if (SendEmail(_RCT("SMTP:support@cad.com.au"),_RCT("Unlicense information"),_RCT("\r\nPlease find unlicense information attached.\r\n"),szFile,_RCT("licenseinfo.dat")))
                      cadwindow->MessageBox("Your software has been unlicensed.\r\nThe license file has been emailed to support@cad.com.au","Unlicense",MB_OK);
                    else
                      cadwindow->MessageBox("Your software has been unlicensed.\r\nThe license file could not be emailed to support@cad.com.au    Please email the licenseinfo.dat file to support@cad.com.au","Unlicense",MB_OK);

                    break;

               }
               else
                 cadwindow->MessageBox("The license file could not be created.\r\nPlease try a different directory or filename.","Unlicense",MB_OK);

          }
          else
            break;
       }
  }
}

void about_command(int *cstate,HighLevelEvent *,void **)
{
#ifdef USING_WIDGETS
 // the new about dialog
 // will be used for showing the relacad version
 // the title and main CAD name can be changed to things like LANDWorksCAD, ArborCAD etc.
 // as required by special cases
 // other currently loaded plugins will also be listed.
 //
 // essential bits
 // Title: About XXXX for YYYY
 //     where XXXX is the Application name
 //     and YYYY is the platform name Windows, the Mac etc.
 // Application name: RealCAD, LANDWorksCAD etc.
 // Version number: 7.0 etc.
 // Copyright: Copyright (c) 1998 to 2015 All rights recerved.
 // Other copyright: "Windows" is a registered trademark of Microsoft Corporation
 // "Mac" is a registered trademark of Apple Inc.
 // Installed plugin applications: a list of the installed plugins and there versions is displyed
 //
 // examples:
 //
 // About RealCAD - for the Mac
 // RealCAD
 // For the Mac
 // Version 7.0
 // Copyright (c) 1998 to 2015
 // All right reserved
 //
 // Mac is a registered trade mark of Apple Inc.
 //
 // The following Plugin Applications or Libraries are loaded:
 //
 //
    Dialog dialog1("AboutRealCAD_Dialog");
    //Dialog *dialog;
    char filename[300],string[300],l[300],dwgversion[300],openglversion[300];
    RCCHAR appversion[300];
    int status;
    RCHINSTANCE hinstance;
    GetVersionString GVS;

    // this is the html for the about text
    // replace keywords $$XXXXX$$ for the various text that is dependent on the app, version etc.
    // add a line for each plugin before the </body></html> tags
    /*
    RealCAD
    CAD for the Mac
    Version 7.0
    Copyright ? 1998 to 2016
    All rights reserved

    Mac is a registered trade mark of Apple Inc.

    License expires in 30 days

    LANDWorksCAD V6.0 Jul 4 2015 18:13:35
    ArborCAD V6.0 Jul 4 2015 18:13:15
    */
    /*
    <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
    <html><head><meta name="qrichtext" content="1" /><style type="text/css">
    p, li { white-space: pre-wrap; }
    </style></head><body style=" font-family:'Arial'; font-size:12pt; font-weight:400; font-style:normal;">
    <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt; font-weight:600;">RealCAD</span></p>
    <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;">CAD for the Mac</span></p>
    <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;">Version 7.0</span></p>
    <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;">Copyright &copy; 1998 to 2015</span></p>
    <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;">All rights reserved</span></p>
    <p align="center" style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;"><br /></p>
    <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'.Helvetica Neue DeskInterface';">Mac is a registered trade mark of Apple Inc.</span></p>
    <p align="center" style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;"><br /></p>
    <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;">License expires in 30 days</span></p>
    <p align="center" style="-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;"><br /></p>
    <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;">LANDWorksCAD V6.0 Jul 4 2015 18:13:35</span></p>
    <p align="center" style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;">ArborCAD V6.0 Jul 4 2015 18:13:15</span></p>
    </body></html>
    */
    QString abouttext;
    abouttext += "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">";
    abouttext += "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\"></style></head>";
    abouttext += "p, li { white-space: pre-wrap; }";
    if(RCGetHDPIYscale() > 1)
    {
        abouttext += "<body style=\" font-family:'Arial'; font-size:15px; font-weight:400; font-style:normal;\">";
        abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px; font-weight:600;\">$$MAIN_APP$$</span></p>";
        //abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\">$$PLATFORM$$</span></p>";
        abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\">$$MAIN_APP_VERSION$$</span></p>";
        abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\">$$CAD_COPYRIGHT$$</span></p>";
        abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\">All rights reserved</span></p>";
        abouttext += "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\"><br /></p>";
        //abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface';font-size:15px;\">$$PLATFORM_COPYRIGHT$$</span></p>";
        abouttext += "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\"><br /></p>";
        abouttext += "$$LICENSE_TEXT$$";
        abouttext += "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\"><br /></p>";
    }
    else
    {
        abouttext += "<body style=\" font-family:'Arial'; font-size:12pt; font-weight:400; font-style:normal;\">";
        abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt; font-weight:600;\">$$MAIN_APP$$</span></p>";
        //abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;\">$$PLATFORM$$</span></p>";
        abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;\">$$MAIN_APP_VERSION$$</span></p>";
        abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;\">$$CAD_COPYRIGHT$$</span></p>";
        abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;\">All rights reserved</span></p>";
        abouttext += "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;\"><br /></p>";
        //abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface';font-size:14pt;\">$$PLATFORM_COPYRIGHT$$</span></p>";
        abouttext += "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;\"><br /></p>";
        abouttext += "$$LICENSE_TEXT$$";
        abouttext += "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;\"><br /></p>";
    }
    // insert each plugin except the main app plugin here
    //abouttext += "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;\">LANDWorksCAD V6.0 Jul 4 2015 18:13:35</span></p>";
    // end plugins
    //abouttext += "</body></html>";
    QString licensetext = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:14pt;\">$$LICENSE_TIME$$</span></p>";
    if(RCGetHDPIYscale() > 1)
        licensetext = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\">$$LICENSE_TIME$$</span></p>";

    // $$MAIN_APP$$
    // $$PLATFORM$$
    // $$MAIN_APP_VERSION$$
    // $$CAD_COPYRIGHT$$
    // $$PLATFORM_COPYRIGHT$$
    // $$LICENSE_TIME$$
    // $$LICENSE_TEXT$$
    // $$PLUGIN_APP$$

    QString mainAppName("RealCAD");
    if(GetModuleHandle(_RCT("landworks")) == 0 && GetModuleHandle(_RCT("arborcad")) == 0)
        abouttext.replace("$$MAIN_APP$$",mainAppName);
    else
    {
        if (GetModuleHandle(_RCT("landworks")) != 0 &&
            QString(home.getpublicfilename(_RCT(""))).toLower().contains("landworkscad"))
        {
            mainAppName = "LANDWorksCAD";
            abouttext.replace("$$MAIN_APP$$","LANDWorksCAD");
        }
        else if (GetModuleHandle(_RCT("arborcad")) != 0 &&
                 QString(home.getpublicfilename(_RCT(""))).toLower().contains("arborcad"))
        {
            mainAppName = "ArborCAD";
            abouttext.replace("$$MAIN_APP$$","ArborCAD");
        }
        else
            abouttext.replace("$$MAIN_APP$$",mainAppName);
    }

#if 0
    // removed at Nigels request
#ifdef _MAC
    abouttext.replace("$$PLATFORM$$","For the Mac");
#else
    abouttext.replace("$$PLATFORM$$","For Windows");
#endif
#endif
#ifdef MAJOR_VERSION
    QString rcmajor = QString("%1").arg(MAJOR_VERSION);
#else
    QString rcmajor("8");
#endif
#ifdef MINOR_VERSION
    QString rcminor = QString("%1").arg(MINOR_VERSION);
#else
    QString rcminor("0");
#endif
#ifdef VERSION_SUFFIX
    QString versionSuffix = QString("%1").arg(VERSION_SUFFIX);
#else
#if MINOR_VERSION > 0
    QString versionSuffix("");
    //QString versionSuffix("(ALPHA)");
#else
    QString versionSuffix("");
#endif
#endif
#ifdef _RCDEBUG
    versionSuffix += ("(Debug)");
#endif

    QString RealCADVersion(rcmajor + "." + rcminor + versionSuffix);
    QString appDate = QString(" %1 %2").arg(__DATE__).arg(__TIME__);
    QString appVersion;
    QString cadcopyright = QString("Copyright &copy; 1998 to %1").arg(QDate::currentDate().year());

    // note all plugin apps are required to include the build date in their version number
    if(GetModuleHandle(_RCT("landworks")) == 0 && GetModuleHandle(_RCT("arborcad")) == 0)
        appVersion = "Version " + RealCADVersion + appDate;
    else
    {
        if (GetModuleHandle(_RCT("landworks")) != 0 && mainAppName.toLower().contains("landworkscad"))
            appVersion = " Version " + QString(program->getloadedappversion(_RCT("landworks"),appversion));
        else if (GetModuleHandle(_RCT("arborcad")) != 0 && mainAppName.toLower().contains("arborcad"))
            appVersion = "Version " + QString(program->getloadedappversion(_RCT("arborcad"),appversion));
        else
            appVersion = "Version " + RealCADVersion + appDate;
    }
    abouttext.replace("$$MAIN_APP_VERSION$$", appVersion);

    abouttext.replace("$$CAD_COPYRIGHT$$",cadcopyright);

#if 0
    // removed at Nigels request
#ifdef _MAC
    abouttext.replace("$$PLATFORM_COPYRIGHT$$","Mac is a registered trade mark of Apple Inc.");
#else
    abouttext.replace("$$PLATFORM_COPYRIGHT$$","Windows is a registered trade mark of Microsoft Corporation.");
#endif
#endif

    QString daysleft = QString("License expires in %1 days").arg(program->getdaysleft());
    if (program->getdaysleft() >= 0)
    {
        licensetext.replace("$$LICENSE_TIME$$",daysleft);
        abouttext.replace("$$LICENSE_TEXT$$",licensetext);
    }
    else
        abouttext.replace("$$LICENSE_TEXT$$","");

    // insert each plugin excepting the main app plugin here
    RCCHAR pluginname[300];
    QString pluginApp("RealCAD");
    QString pluginAppText("<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:12pt;\">$$PLUGIN_APP$$</span></p>");
    if(RCGetHDPIYscale() > 1)
        pluginAppText = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:12px;\">$$PLUGIN_APP$$</span></p>";

    // if there is a main plugin app RealCAD comes here
    //if(GetModuleHandle(_RCT("landworks")) != 0 || GetModuleHandle(_RCT("arborcad")) != 0)
    if (mainAppName.toLower() != "realcad")
    {
        pluginApp += " Version " + RealCADVersion + appDate;
        pluginAppText.replace("$$PLUGIN_APP$$",pluginApp);
        abouttext += pluginAppText;
        // reset the plugin app text
        pluginAppText = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:12pt;\">$$PLUGIN_APP$$</span></p>";
        if(RCGetHDPIYscale() > 1)
            pluginAppText = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:12px;\">$$PLUGIN_APP$$</span></p>";
    }
    for (int i = 0 ; program->getloadedappname(i,pluginname) != 0 ; i++)
    {
        QString pluginAppText("<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:12pt;\">$$PLUGIN_APP$$</span></p>");
        if(RCGetHDPIYscale() > 1)
            pluginAppText = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:12px;\">$$PLUGIN_APP$$</span></p>";
        pluginApp = QString(pluginname);
        // don't list any main apps
        // add main apps to this conditional list as required
        if(abouttext.toLower().contains("landworks") && pluginApp.toLower().contains("landworks"))
            continue;
        if(abouttext.toLower().contains("arborcad") && pluginApp.toLower().contains("arborcad"))
            continue;

        appVersion = QString(program->getloadedappversion(pluginApp.toLower().data(),appversion));
        pluginApp += " Version " + appVersion;
        pluginAppText.replace("$$PLUGIN_APP$$",pluginApp);
        abouttext += pluginAppText;
    }
    // end of plugins

    // look for other external library modules
    // add more sections here if required
    QStringList moduleNames;
    RCCHAR moduleName[1024];

#ifdef _MAC
    //strcpy(moduleName,"libRealCADOpenDWG.1.0.0.dylib");
    moduleNames << "libExporter.1.0.0.dylib";
    moduleNames << "libImporter.1.0.0.dylib";
    moduleNames << "libRealCADSketchUp.1.0.0.dylib";
#else
    //strcpy(moduleName,"RealCADOpenDWG.dll");
    moduleNames << "Exporter.dll";
    moduleNames << "Importer.dll";
    moduleNames << "RealCADSketchUp.dll";
#endif

    for(int im=0; im < moduleNames.size(); im++)
    {
        strcpy(moduleName,moduleNames.at(im).data());
        if(program->ismoduleloaded(home.getexecutablefilename(moduleName)))
        {
            QString pluginAppText("<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:12pt;\">$$PLUGIN_APP$$</span></p>");
            if(RCGetHDPIYscale() > 1)
                pluginAppText = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:12px;\">$$PLUGIN_APP$$</span></p>";
            strcpy(pluginname,program->getmodulename(home.getexecutablefilename(moduleName)));
            pluginApp = QString(pluginname);
            appVersion = QString(program->getmoduleversion(home.getexecutablefilename(moduleName),appversion));
            pluginApp += " Version " + appVersion;
            pluginAppText.replace("$$PLUGIN_APP$$",pluginApp);
            abouttext += pluginAppText;
        }
    }
    // end of other libraries
//#if 1
#ifdef _RCDEBUG
    QString dbgmsg1;
    dbgmsg1 = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\">$$DEBUG_TEXT$$</span></p>";
    QString dbgmsg2;
    dbgmsg2 = QString("Device Pixel Ratio : %1").arg(app->desktop()->screen()->devicePixelRatio());
    dbgmsg1.replace("$$DEBUG_TEXT$$",dbgmsg2);
    abouttext += dbgmsg1;
    //
    dbgmsg1 = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\">$$DEBUG_TEXT$$</span></p>";
    dbgmsg2 = QString("HDPIXscale : %1").arg(RCGetHDPIXscale(0));
    dbgmsg1.replace("$$DEBUG_TEXT$$",dbgmsg2);
    abouttext += dbgmsg1;
    //
    dbgmsg1 = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\">$$DEBUG_TEXT$$</span></p>";
    dbgmsg2 = QString("logicalDotsPerInch : %1").arg(app->primaryScreen()->logicalDotsPerInch());
    dbgmsg1.replace("$$DEBUG_TEXT$$",dbgmsg2);
    abouttext += dbgmsg1;
    //
    dbgmsg1 = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\">$$DEBUG_TEXT$$</span></p>";
    dbgmsg2 = QString("physicalDotsPerInch : %1").arg(app->primaryScreen()->physicalDotsPerInch());
    dbgmsg1.replace("$$DEBUG_TEXT$$",dbgmsg2);
    abouttext += dbgmsg1;
    //
    dbgmsg1 = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\">$$DEBUG_TEXT$$</span></p>";
    QSizeF physize = app->primaryScreen()->physicalSize();
    dbgmsg2 = QString("physical size (mm) : x(%1) , y(%2)").arg(physize.width()).arg(physize.height());
    dbgmsg1.replace("$$DEBUG_TEXT$$",dbgmsg2);
    abouttext += dbgmsg1;
    //
    dbgmsg1 = "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'.Helvetica Neue DeskInterface'; font-size:15px;\">$$DEBUG_TEXT$$</span></p>";
    QSize pixsize = app->primaryScreen()->size();
    dbgmsg2 = QString("pixel size (px) : x(%1) , y(%2)").arg(pixsize.width()).arg(pixsize.height());
    dbgmsg1.replace("$$DEBUG_TEXT$$",dbgmsg2);
    abouttext += dbgmsg1;
#endif

    // end of html
    abouttext += "</body></html>";

    RCCHAR *rctext = new RCCHAR[abouttext.length()+1];
    strcpy(rctext,abouttext.data());
    dialog1.add(new StringDialogControl(100,rctext,abouttext.length()));
    dialog1.add(new ButtonDialogControl(104));
    dialog1.add(new ButtonDialogControl(106));

    QString dialogTitle;
#ifdef _MAC
    // not showing the platform name anymore
    dialogTitle = "About " + mainAppName;// + " - CAD for the Mac";
#else
    dialogTitle = "About " + mainAppName;// + " - CAD for Windows";
#endif
    dialog1.title(dialogTitle.data());

 #else
 Dialog dialog1("AboutReal_dialog"),dialog2("AboutStudent_dialog");
 Dialog dialog3("AboutTriCad_dialog"),dialog4("AboutAUS_dialog");
 Dialog dialog5("AboutTriCadDemo_dialog"),dialog6("AboutTriCadStudent_dialog");
 Dialog dialog7("AboutFlexiCAD_dialog"),dialog8("AboutRealDemo_dialog");
 Dialog dialog9("AboutLandworks_dialog");
 Dialog *dialog;
 char filename[300],string[300],l[300],dwgversion[300],openglversion[300];
 int status;
 RCHINSTANCE hinstance;
 GetVersionString GVS;


  //  Get the version of the DWG and OpenGL Interfaces
  strcpy(dwgversion,"");
  hinstance = LoadLibrary(_RCT("RealCADOpenDWG.dll"));
  if (hinstance != 0)
    {  GVS = (GetVersionString) GetProcAddress(hinstance,"GetVersionString");
       if (GVS != 0)
         strcpy(dwgversion,GVS());
       else
         strcpy(dwgversion,"RealCAD DWG Interface : Version Unavailable");
       FreeLibrary(hinstance);
    }
  else
    strcpy(dwgversion,"RealCAD DWG Interface : Not loaded");

  strcpy(openglversion,"");
  hinstance = LoadLibrary(_RCT(("RealCADOpenGL.dll")));
  if (hinstance != 0)
    {  GVS = (GetVersionString) GetProcAddress(hinstance,"GetVersionString");
       if (GVS != 0)
         strcpy(openglversion,GVS());
       else
         strcpy(openglversion,"RealCAD OpenGl Interface : Version Unavailable");
       FreeLibrary(hinstance);
    }
  else
    strcpy(dwgversion,"RealCAD OpenGl Interface : Not loaded");

  switch (cad_version&15)
    {case 0 :
       if (GetModuleHandle(_RCT("Landworks.DLL")) != 0)
         dialog = & dialog9;
       else 
         dialog = & dialog1;

       if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) == 0)
         {  if (program->getdaysleft() >= 0)
              sprintf(l,"License expires in %d days",program->getdaysleft());
            else
              strcpy(l,"");
         } 
       else
         strcpy(l,llabel);
       dialog->add(new StringDialogControl(100,l,300));
       dialog->add(new ButtonDialogControl(104));


       break;
     case 1 :
       dialog = & dialog2;
       break;
     case 2 :
       dialog = & dialog3;
       if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) == 0)
         strcpy(l,"");
       else
         strcpy(l,llabel);
       dialog->add(new StringDialogControl(100,l,300));
       break;
     case 3 :
       dialog = & dialog4;
       if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) == 0)
         strcpy(l,"");
       else
         strcpy(l,llabel);
       dialog->add(new StringDialogControl(100,l,300));
       break;
     case 4 :
       dialog = & dialog5;
       break;
     case 5 :
       dialog = & dialog6;
       break;
     case 6 :
       dialog = & dialog7;
       break;
     case 7 :
       dialog = & dialog8;
       break;
    }
  dialog->add(new VersionControl(101));
  dialog->add(new StringDialogControl(102,dwgversion,300));
  dialog->add(new StringDialogControl(103,openglversion,300));

  if (strlen(home.getlicensefilename()) > 0)
    GetPrivateProfileString("Cad","FileName","",filename,298,home.getlicensefilename());
  else  
    GetProfileString("Cad","FileName","",filename,298);
  if (strlen(filename) > 0 && strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) == 0)
    {  dialog->add(new ButtonDialogControl(105));
    }
  else
    {  
       if (strlen(home.getlicensefilename()) > 0)
            GetPrivateProfileString("Cad","RealCADKey2","",string,298,home.getlicensefilename());
       else
            GetProfileString("Cad","RealCADKey2","",string,298);
       if (strlen(string) == 0 && strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) == 0)
         dialog->add(new ButtonDialogControl(105));
       else if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) == 0)
         dialog->add(new ButtonDialogControl(106));
    }
#endif

  status = dialog1.process();
  if (status == 104)
    {
       Dialog dialog("Unlicense_dialog");
       int unlicense;
       unlicense = 0;
       dialog.add(new CheckBoxDialogControl(100,&unlicense));
       if (dialog.process() == TRUE && unlicense)
       {
          RCOPENFILENAME ofn;
          RCCHAR szDirName[256]={0},szFile[256]={0},szFileTitle[256]={0},szFilter[256]={0},initialdirectory[300]={0};
          UINT i;
          RCCHAR title[256]={0},dlgtitle[256]={0};

            for (;;)
              {
                 _getcwd(szDirName, 256);
                 strcpy(szFile,"licenseinfo.dat");
                 strcpy(szFilter,"license files (*.dat)");

                 //for (i = 0; szFilter[i] != '\0'; i++)
                 //  if (szFilter[i] == '|') szFilter[i] = '\0';

                 memset(&ofn, 0, sizeof(OPENFILENAME));
 
                 strcpy(initialdirectory,QDir::homePath());
                 //SHGetSpecialFolderPath(NULL, initialdirectory, CSIDL_MYDOCUMENTS, 0);
                 if(QDir().exists(QString(initialdirectory)+"/Documents"))
                     strcat(initialdirectory,"/Documents");
#ifdef _MAC
                 //strcat(initialdirectory,"/Documents");
#else
                 if(QDir().exists(QString(initialdirectory)+"/My Documents"))
                    strcat(initialdirectory,"/My Documents");
                 else if(QDir().exists(QString(initialdirectory)+"/Documents"))
                     strcat(initialdirectory,"/Documents");

#endif
                 strcpy(initialdirectory,QDir::toNativeSeparators(QString(initialdirectory)));

                 ofn.lStructSize = sizeof(OPENFILENAME);
                 //ofn. lpstrInitialDir = _RCT("My Documents");
                 ofn.hwndOwner = cadwindow->gethwnd();
                 ofn.lpstrFilter = szFilter;
                 ofn.nFilterIndex = 1;
                 ofn.lpstrFile= szFile;
                 ofn.nMaxFile = sizeof(szFile);
                 ofn.lpstrFileTitle = szFileTitle;
                 ofn.nMaxFileTitle = sizeof(szFileTitle);
                 ofn.lpstrInitialDir = initialdirectory;
                 ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST;
                 ofn.lpstrDefExt = _RCT(".dat");
                 ofn.lpstrFileTitle = title;
                 strcpy(dlgtitle,"Select the file to save the license information into");
                 ofn.lpstrTitle = dlgtitle;
                 if (GetSaveFileName(&ofn))
                 {
                      FILE *outfile = 0;//fopen(QString(szFile).toLatin1().data(),"r");
                      //if(QFile(QString(szFile)).exists())
                      if ((outfile = fopen(QString(szFile).toLatin1().data(),"r")) != 0)
                      {
                           cadwindow->MessageBox("The license information file already exists.  You cannot overwrite this file.  The license information has not been saved.","Unlicense",MB_OK);
                           fclose(outfile);
                      }
                      else if ((outfile = fopen(QString(szFile).toLatin1().data(),"w"))  != 0)
                      {
                           fprintf(outfile,"License File Information\n");
  
                           if (strlen(home.getlicensefilename()) == 0)
                             GetProfileString("Cad","RealCADKey1","",string,298);
                           else
                             GetPrivateProfileString("Cad","RealCADKey1","",string,298,home.getlicensefilename());
                           fprintf(outfile,"1:%s\n",string);   

                           if (strlen(home.getlicensefilename()) == 0)
                             GetProfileString("Cad","RealCADKey2","",string,298);
                           else
                             GetPrivateProfileString("Cad","RealCADKey2","",string,298,home.getlicensefilename());
                           fprintf(outfile,"2:%s\n",string);   

                           if (strlen(home.getlicensefilename()) == 0)
                             GetProfileString("Cad","RealCADKey3","",string,298);
                           else
                             GetPrivateProfileString("Cad","RealCADKey3","",string,298,home.getlicensefilename());
                           fprintf(outfile,"3:%s\n",string);   

                           if (strlen(home.getlicensefilename()) == 0)
                             GetProfileString("Cad","RealCADKey4","",string,298);
                           else
                             GetPrivateProfileString("Cad","RealCADKey4","",string,298,home.getlicensefilename());
                           fprintf(outfile,"4:%s\n",string);   

                           if (strlen(home.getlicensefilename()) == 0)
                             GetProfileString("Cad","RealCADKey5","",string,298);
                           else
                             GetPrivateProfileString("Cad","RealCADKey5","",string,298,home.getlicensefilename());
                           fprintf(outfile,"5:%s\n",string);   

                           if (strlen(home.getlicensefilename()) == 0)
                             GetProfileString("Cad","RealCADKey6","",string,298);
                           else
                             GetPrivateProfileString("Cad","RealCADKey6","",string,298,home.getlicensefilename());
                           fprintf(outfile,"6:%s\n",string);   

                           if (strlen(home.getlicensefilename()) == 0)
                             GetProfileString("Cad","RealCADKey7","",string,298);
                           else
                             GetPrivateProfileString("Cad","RealCADKey7","",string,298,home.getlicensefilename());
                           fprintf(outfile,"7:%s\n",string);   

                           if (strlen(home.getlicensefilename()) == 0)
                             GetProfileString("Cad","RealCADKey8","",string,298);
                           else
                             GetPrivateProfileString("Cad","RealCADKey8","",string,298,home.getlicensefilename());
                           fprintf(outfile,"8:%s\n",string);   

                           fclose(outfile);

                           WriteProfileString("Cad","RealCADKey1","");
                           WriteProfileString("Cad","RealCADKey2","");
                           WriteProfileString("Cad","RealCADKey3","");
                           WriteProfileString("Cad","RealCADKey4","");
                           WriteProfileString("Cad","RealCADKey5","");
                           WriteProfileString("Cad","RealCADKey6","");
                           WriteProfileString("Cad","RealCADKey7","");
                           WriteProfileString("Cad","RealCADKey8","");

                           if (strlen(home.getlicensefilename()) > 0)
                             {  WritePrivateProfileString("Cad","RealCADKey1","",home.getlicensefilename());
                                WritePrivateProfileString("Cad","RealCADKey2","",home.getlicensefilename());
                                WritePrivateProfileString("Cad","RealCADKey3","",home.getlicensefilename());
                                WritePrivateProfileString("Cad","RealCADKey4","",home.getlicensefilename());
                                WritePrivateProfileString("Cad","RealCADKey5","",home.getlicensefilename());
                                WritePrivateProfileString("Cad","RealCADKey6","",home.getlicensefilename());
                                WritePrivateProfileString("Cad","RealCADKey7","",home.getlicensefilename());
                                WritePrivateProfileString("Cad","RealCADKey8","",home.getlicensefilename());
                             }

                           if (SendEmail(_RCT("SMTP:support@cad.com.au"),_RCT("Unlicense information"),_RCT("\r\nPlease find unlicense information attached.\r\n"),szFile,_RCT("licenseinfo.dat")))
                             cadwindow->MessageBox("Your software has been unlicensed.\r\nThe license file has been emailed to support@cad.com.au","Unlicense",MB_OK);
                           else
                             cadwindow->MessageBox("Your software has been unlicensed.\r\nThe license file could not be emailed to support@cad.com.au    Please email the licenseinfo.dat file to support@cad.com.au","Unlicense",MB_OK);

                           break;

                      }
                      else
                        cadwindow->MessageBox("The license file could not be created.\r\nPlease try a different directory or filename.","Unlicense",MB_OK);
                     
                 }
                 else
                   break;
              }
         }
    }
  else if (status == 105)
    {  if (strlen(filename) == 0 || cadwindow->MessageBox("A network server has already been defined.  Do you really want to change the network server?","Change Software License Server",MB_YESNO) == IDYES)
         {  if (strlen(home.getlicensefilename()) > 0)
              WritePrivateProfileString("Cad","FileName","",home.getlicensefilename());
            else  
              WriteProfileString("Cad","FileName","");
#if MENUS != FLEXICADMENUS
            check_license((char*)"Main CAD Application", (char*)"RealCADKey1", (char*)"RealCADKey2",0,1);
#endif            
         }
    }
  else 
  if (status == 106)
    {
     RCOPENFILENAME ofn;
     RCCHAR szDirName[256],szFile[256],szFileTitle[256],szFilter[256];
     UINT i;
     RCCHAR title[256];
     ResourceString rsnc1(NC+1);
     ResourceString rsnc2(NC+2);

       _getcwd(szDirName, 256);
       strcpy(szFile,"keyserver.ini");
       strcpy(szFilter,"Key files (*.ini)");

       //for (i = 0; szFilter[i] != '\0'; i++)
       //  if (szFilter[i] == '|') szFilter[i] = '\0';

       memset(&ofn, 0, sizeof(OPENFILENAME));

       ofn.lStructSize = sizeof(OPENFILENAME);
       ofn.hwndOwner = cadwindow->gethwnd();
       ofn.lpstrFilter = szFilter;
       ofn.nFilterIndex = 1;
       ofn.lpstrFile= szFile;
       ofn.nMaxFile = sizeof(szFile);
       ofn.lpstrFileTitle = szFileTitle;
       ofn.nMaxFileTitle = sizeof(szFileTitle);
       ofn.lpstrInitialDir = szDirName;
       ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST;
       ofn.lpstrDefExt = rsnc2.getws();
       ofn.lpstrFileTitle = title;
       qDebug() << "license server : " << QString(szFile);
       if (GetSaveFileName(&ofn))
       {
           QString nativeDir(QDir::toNativeSeparators(QString(szFile)));
         WriteProfileString(_RCT("Cad"),_RCT("ServerFilename"),nativeDir.data());
       }
       qDebug() << "license server : " << QString(szFile);
    }
  delete [] rctext;
  *cstate = ExitState;
}

void position_command0(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Position"));
  *cstate = ExitState;
}

/**
 * @brief selectall_command
 * @param cstate
 * @param event
 *
 * Since V7
 * Select all command
 * run from a Ctrl+A short cut
 *
 */
void selectall_command(int *cstate,HighLevelEvent *event,void **)
{
  switch (*cstate)
  {
  case InitialState :
      // select all the entities selectable in the model
      View3dWindow *v;
      Entity *e=0;
      EntityList list;

      cadwindow->startdrawingbatch();

      state.destroyselection(0);
      state.sethandledentity(0);

      list = db.geometry.pickall(0);
      state.setselection(list);

      cadwindow->enddrawingbatch();

      *cstate = ExitState;
      break;
  }
}

/**
 * @brief position_command1
 * @param cstate
 * @param event
 *
 * Locate a reference point
 *
 */
void position_command1(int *cstate,HighLevelEvent *event,void **)
{ switch (*cstate)
    {  case InitialState :
         program->setcursorshape(CursorPosition);
         *cstate = 1;
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           state.fromposition();
         *cstate = ExitState;
         break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt(NCMAIN+18);
    }
  else if (*cstate == ExitState)
    program->setcursorshape(CursorRestore);

}

struct RBPosition2_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
    Entity *e;
};

#ifdef NEW_RUBBERB
void RBPosition2(int dmode,void *data,Point *p2,View3dSurface *)
{
  RBPosition2_data *rbdata = (RBPosition2_data *) data;

  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  *p2 = rbdata->e->nearp(*p2);
  PointE point(*p2);
  point.draw(DM_INVERT);
}
#else
void RBPosition2(int,void *data,Point *p2,View3dSurface *)
{
  RBPosition2_data *rbdata = (RBPosition2_data *) data;
  *p2 = rbdata->e->nearp(*p2);
  PointE point(*p2);
  point.qgi = rbdata->qgi; point.zValue = rbdata->zValue;
  point.draw(DM_INVERT);
  rbdata->qgi = point.qgi; rbdata->zValue = point.zValue;
}
#endif

/**
 * @brief position_command2
 * @param cstate
 * @param event
 * @param data
 *
 * Locate a position near a point or at the instersection of two entities
 * KMJ : working in the Qt version
 */
void position_command2(int *cstate,HighLevelEvent *event,void **data)
{
 RBPosition2_data *rbdata = (RBPosition2_data *) *data;
 View3dWindow *window;
 Point p,xa,ya;
 Entity *e;
 switch (*cstate)
    {  case InitialState :
         rbdata = new RBPosition2_data;
#ifdef USING_WIDGETS
         rbdata->qgi=0; rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         *data = rbdata;
#endif
         *cstate = 1;
         program->setcursorshape(CursorPosition);
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              rbdata->e->highlight();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(1);
         if (event->isa(Coordinate3d))
           {  p = rbdata->e->nearp(((Coordinate3dEvent *) event)->getp());
              xa = ((Coordinate3dEvent *) event)->getxaxis();
              ya = ((Coordinate3dEvent *) event)->getyaxis();
              window = ((Coordinate3dEvent *) event)->getwindow();
              Coordinate3dEvent *cevent = new Coordinate3dEvent(p,xa,ya,window,0);
              state.sendevent(cevent);
              rbdata->e->unhighlight();
              *cstate = ExitState;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              p = ((EntitySelectedEvent *) event)->getp();
              xa = ((EntitySelectedEvent *) event)->getxaxis();
              ya = ((EntitySelectedEvent *) event)->getyaxis();
              Intersect i(rbdata->e,e,p);
              if (i.nintersections() == 1 && i.intersection(0)->isa(point_entity))
                {  p = ((PointE *)i.intersection(0))->getp();
                   Coordinate3dEvent *cevent = new Coordinate3dEvent(p,xa,ya,NULL,0);
                   state.sendevent(cevent);
                   *cstate = ExitState;
                   rbdata->e->unhighlight();
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  *cstate = ExitState;
              rbdata->e->unhighlight();
           }
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(linear_mask,surface_mask,group_entity,end_list);
       cadwindow->prompt->normalprompt(NCMAIN+19);
    }
  else if (*cstate == 2)
    {  if (rbdata->e->isa(group_entity))
         state.selmask.entity.clearandset(xyz_mask,end_list);
       else
         state.selmask.entity.clearandset(xyz_mask,surface_mask,linear_mask,end_list);
       rubberband.begin(1);
       rubberband.add(RBPosition2,rbdata);
       cadwindow->prompt->normalprompt(NCMAIN+20);
    }
  else if (*cstate == ExitState)
    {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
      if(rbdata->qgi)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi; rbdata->qgi=0;
#endif
#endif
      program->setcursorshape(CursorRestore);
      delete rbdata;
    }
}

struct RBPosition3_data
  {double distance,percentage;
   int type;
   RCCHAR fraction[300];
  };

class DARadioButtonDialogControl : public RadioButtonDialogControl
  {public:
    DARadioButtonDialogControl(int id,int id1,int id2,int *v) : RadioButtonDialogControl(id,id1,id2,v) {};
    void load(Dialog *);
    int select(Dialog *);
  };

void DARadioButtonDialogControl::load(Dialog *dialog)
{ RadioButtonDialogControl::load(dialog);
  dialog->getcontrol(100)->enable(dialog,oldvalue == 0);
  //dialog->getcontrol(1100)->enable(dialog,oldvalue == 0);
  dialog->getcontrol(101)->enable(dialog,oldvalue == 1);
  //dialog->getcontrol(1101)->enable(dialog,oldvalue == 1);
  dialog->getcontrol(102)->enable(dialog,oldvalue == 2);
}

int DARadioButtonDialogControl::select(Dialog *dialog)
{int status;
  status = RadioButtonDialogControl::select(dialog);
  dialog->getcontrol(100)->enable(dialog,oldvalue == 0);
  //dialog->getcontrol(1100)->enable(dialog,oldvalue == 0);
  dialog->getcontrol(101)->enable(dialog,oldvalue == 1);
  //dialog->getcontrol(1101)->enable(dialog,oldvalue == 1);
  dialog->getcontrol(102)->enable(dialog,oldvalue == 2);
  return status;
}

/**
 * @brief position_command3
 * @param cstate
 * @param event
 * @param data
 *
 * Locate a position along a reference entity
 *
 */
void position_command3(int *cstate,HighLevelEvent *event,void **data)
{
 RBPosition3_data *rbdata = (RBPosition3_data *) *data;
 double l,t;
 Entity *e;
 Point p,xa,ya;
 int end;
 RCCHAR *fractionl[9];
#define FRACTIONS 4112
 ResourceString rs0(FRACTIONS),rs1(FRACTIONS+1),rs2(FRACTIONS+2),rs3(FRACTIONS+3),rs4(FRACTIONS+4),rs5(FRACTIONS+5),rs6(FRACTIONS+6),rs7(FRACTIONS+7),rs8(FRACTIONS+8);
 ResourceString rs21(NCMAIN+21);
 ResourceString rs22(NCMAIN+22);
 Dialog dialog("PositionAlong_Dialog");
 DialogControl *dc;

 fractionl[0] = rs0.gets();// 1/2
 fractionl[1] = rs1.gets();// 1/3
 fractionl[2] = rs2.gets();// 1/4
 fractionl[3] = rs3.gets();// 1/5
 fractionl[4] = rs4.gets();// 1/6
 fractionl[5] = rs5.gets();// 1/7
 fractionl[6] = rs6.gets();// 1/8
 fractionl[7] = rs7.gets();// 1/9
 fractionl[8] = rs8.gets();// 1/10

  switch (*cstate)
    {  case InitialState :
         rbdata = new RBPosition3_data;  *data = rbdata;
         program->setcursorshape(CursorPosition);
         if (rbdata == 0)
           *cstate = ExitState;
         else
           {  rbdata->type = v.getinteger("ps::alongtype");
              rbdata->distance = v.getreal("ps::dalong");
              rbdata->percentage = v.getreal("ps::palong");
              strcpy(rbdata->fraction,v.getstring("ps::falong"));
              dialog.add(dc = new RealDialogControl(100,&rbdata->distance,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1100,dc));
              dialog.add(dc = new RealDialogControl(101,&rbdata->percentage));
              //dialog.add(new ScrollBarDialogControl(1101,dc));
              dialog.add(new ListDialogControl(102,9,fractionl,rbdata->fraction,300));
              dialog.add(new DARadioButtonDialogControl(200,200,202,&rbdata->type));
              dialog.add(new DARadioButtonDialogControl(201,200,202,&rbdata->type));
              dialog.add(new DARadioButtonDialogControl(202,200,202,&rbdata->type));
              if (dialog.process() == FALSE)
                *cstate = ExitState;
              else
                {  v.setinteger("ps::alongtype",rbdata->type);
                   v.setreal("ps::dalong",rbdata->distance);
                   v.setreal("ps::palong",rbdata->percentage);
                   v.setstring("ps::falong",rbdata->fraction);
                   *cstate = 1;
                }
           }
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              end = ((EntitySelectedEvent *) event)->getendselected();
              if (e->islinear())
                {  l = ((LinearEntity *)e)->length();
                   if (l > db.getptoler())
                     {  if (rbdata->type == 0)
                          t = rbdata->distance / l;
                        else if (rbdata->type == 1)
                          t = rbdata->percentage / 100.0;
                        else
                          {int t1,t2;
                             if (sscanf(rbdata->fraction,"%d/%d",&t1,&t2) == 2 && t2 != 0)
                               t = (double)t1 / (double)t2;
                             else
                               t = 0.0;
                          }
                        if (end == 2) t = 1.0 - t;
                        p = ((LinearEntity *)e)->position(t);
                        if (db.workplanes.getcurrent() != NULL)
                          {  xa = db.workplanes.getcurrent()->getxaxis();
                             ya = db.workplanes.getcurrent()->getyaxis();
                          }
                        else
                          {  xa.setxyz(1.0,0.0,0.0);  ya.setxyz(0.0,1.0,0.0);
                          }
                        Coordinate3dEvent *cevent = new Coordinate3dEvent(p,xa,ya,NULL,0);
                        state.sendevent(cevent);
                        *cstate = ExitState;
                     }
                }
           } 
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(line_entity,circle_entity,end_list);
       cadwindow->prompt->normalprompt(NCMAIN+24);
    }
  else if (*cstate == ExitState)
    {  program->setcursorshape(CursorRestore);
       delete rbdata;
    }
}

struct RBPosition4_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
    Point p;
};

#ifdef NEW_RUBBERB
void RBPosition4(int drawmode,void *data,Point *p2,View3dSurface *)
{
  RBPosition4_data *rbdata = (RBPosition4_data *) data;
  if(drawmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  if (drawmode == RB_DRAW)
    *p2 = (*p2 + rbdata->p) / 2.0;
  PointE point(*p2);
  point.draw(DM_INVERT);
}
#else
void RBPosition4(int drawmode,void *data,Point *p2,View3dSurface *)
{
  RBPosition4_data *rbdata = (RBPosition4_data *) data;
  if (drawmode == RB_DRAW)
    *p2 = (*p2 + rbdata->p) / 2.0;
  PointE point(*p2);
#ifdef USING_WIDGETS
  point.qgi = rbdata->qgi; point.zValue = rbdata->zValue;
  point.draw(DM_INVERT);
  rbdata->qgi = point.qgi; rbdata->zValue = point.zValue;
#else
  point.draw(DM_INVERT);
#endif
}
#endif

/**
 * @brief position_command4
 * @param cstate
 * @param event
 * @param data
 *
 * Locate the origin or midpoint of an entity or locate the midpoint of two positions
 *
 * KMJ : working in Qt version
 */
void position_command4(int *cstate,HighLevelEvent *event,void **data)
{RBPosition4_data *rbdata = (RBPosition4_data *) *data;
 Point p,xa,ya;
 Entity *e;
  switch (*cstate)
    {  case InitialState :
         if ((rbdata = new RBPosition4_data) == NULL)
           *cstate = ExitState;
         else
         {
#ifdef USING_WIDGETS
             rbdata->qgi = 0;
             rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif
           *cstate = 1;
         }
         *data = rbdata;
         program->setcursorshape(CursorPosition);
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              if (e->isa(line_entity))
                p = (((Line *)e)->getp1() + ((Line *)e)->getp2()) / 2.0;
              else if (e->isa(circle_entity))
                p = ((Circle *)e)->getorigin();
              else if (e->isa(ellipse_entity))
                p = ((EllipseE *)e)->getorigin();
              else if (e->isa(text_entity))
                p = ((Text *)e)->point(0);
              else if (e->isa(figure_entity))
                {  if (cadwindow->getshiftstate())
                     p = ((Figure *)e)->getorigin();
                   else
                     p = ((EntitySelectedEvent *) event)->getp();
                }
              else if (e->isa(patch_entity))
                {int i,n;
                 double dmin,d;
                 Point *polygon,p1,pmin;

                    p1 = ((EntitySelectedEvent *) event)->getp();
                    polygon = ((Patch *)e)->getpolygon();

                    n = ((Patch *)e)->npoints();
                    dmin = (p1 - polygon[0]).length();
                    p = polygon[0];
                    for (i = 1 ; i < n ; i++)
                        if ((d = (p1 - polygon[i]).length()) < dmin)
                        {   dmin = d;
                            p = polygon[i];
                        }

                }

              xa = ((EntitySelectedEvent *) event)->getxaxis();
              ya = ((EntitySelectedEvent *) event)->getyaxis();
              Coordinate3dEvent *cevent = new Coordinate3dEvent(p,xa,ya,NULL,0);
              state.sendevent(cevent);
              *cstate = ExitState;
           }
         else if (event->isa(Coordinate3d))
           {  rbdata->p = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(1);
         if (event->isa(Coordinate3d))
           {  p = (((Coordinate3dEvent *) event)->getp() + rbdata->p) / 2.0;
              xa = ((Coordinate3dEvent *) event)->getxaxis();
              ya = ((Coordinate3dEvent *) event)->getyaxis();
              Coordinate3dEvent *cevent = new Coordinate3dEvent(p,xa,ya,NULL,0);
              state.sendevent(cevent);
              *cstate = ExitState;
           }
         else if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              p = (e->nearp(rbdata->p) + rbdata->p) / 2.0;
              xa = ((EntitySelectedEvent *) event)->getxaxis();
              ya = ((EntitySelectedEvent *) event)->getyaxis();
              Coordinate3dEvent *cevent = new Coordinate3dEvent(p,xa,ya,NULL,0);
              state.sendevent(cevent);
              *cstate = ExitState;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,ellipse_entity,text_entity,figure_entity,patch_entity,end_list);
       cadwindow->prompt->normalprompt(NCMAIN+25);
    }
  else if (*cstate == 2)
    {  rubberband.begin(1);
       rubberband.add(RBPosition4,rbdata);
       state.selmask.entity.clearandset(xyz_mask,line_entity,circle_entity,/*curve_entity,*/end_list);
       cadwindow->prompt->normalprompt(NCMAIN+201);
    }
  else if (*cstate == ExitState)
    {
#ifdef USING_WIDGETS
#ifndef NEW_RUBBERB
      if(rbdata->qgi)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi; rbdata->qgi=0;
#endif
#endif
      program->setcursorshape(CursorRestore);
       delete rbdata;
    }
}

/**
 * @brief position_command5
 * @param cstate
 * @param event
 *
 * Locate the end point of an entity
 * KMJ : Working in the Qt version
 */
void position_command5(int *cstate,HighLevelEvent *event,void **)
{Point p,xa,ya;
 int i;

  switch (*cstate)
    {  case InitialState :
         *cstate = 1;
         program->setcursorshape(CursorPosition);
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  if (((EntitySelectedEvent *) event)->getentity()->isa(figure_entity))
                {  state.destroyselection(1);
                   p = ((EntitySelectedEvent *) event)->getp();
                   xa = ((EntitySelectedEvent *) event)->getxaxis();
                   ya = ((EntitySelectedEvent *) event)->getyaxis();
                   Coordinate3dEvent *cevent = new Coordinate3dEvent(p,xa,ya,NULL,0);
                   state.sendevent(cevent);
                   *cstate = ExitState;
                }
              else
                {  state.destroyselection(1);
                   xa = ((EntitySelectedEvent *) event)->getxaxis();
                   ya = ((EntitySelectedEvent *) event)->getyaxis();

                   if (((EntitySelectedEvent *) event)->getentity()->isa(lineardimension_entity))
                     {LinearDimension *ld = (LinearDimension *)((EntitySelectedEvent *) event)->getentity();
                        for (i = 1 ; i < ld->npoints() ; i++)
                          {  if (i == 1 || (((EntitySelectedEvent *) event)->getp() - ld->point(i)).length() < (((EntitySelectedEvent *) event)->getp() - p).length())
                               p = ld->point(i);  
                          }
                     }
                   else if (cadwindow->getshiftstate() && ((EntitySelectedEvent *) event)->getentity()->isa(circle_entity))
                     {  //  Snap to a circle quadrant
                        Circle *circle = (Circle *)  ((EntitySelectedEvent *) event)->getentity();
                        Line line1(circle->getorigin(),circle->getorigin() + xa);
                        Line line2(circle->getorigin(),circle->getorigin() + ya);

                        Intersect i1(circle,&line1,((EntitySelectedEvent *) event)->getp());
                        if (i1.nintersections() == 1)
                          p = ((PointE *)i1.intersection(0))->getp();

                        Intersect i2(circle,&line2,((EntitySelectedEvent *) event)->getp());
                        if (i2.nintersections() == 1 && (((PointE *)i2.intersection(0))->getp() - ((EntitySelectedEvent *) event)->getp()).length() < (p-((EntitySelectedEvent *) event)->getp()).length())
                          p = ((PointE *)i2.intersection(0))->getp();

                     }
                   else if (cadwindow->getshiftstate() && ((EntitySelectedEvent *) event)->getentity()->isa(ellipse_entity))
                     {  //  Snap to an ellipse quadrant
                        EllipseE *ellipse = (EllipseE *)  ((EntitySelectedEvent *) event)->getentity();
                        Line line1(ellipse->getorigin(),ellipse->getorigin() + ellipse->getxaxis());
                        Line line2(ellipse->getorigin(),ellipse->getorigin() + ellipse->getyaxis());

                        Intersect i1(ellipse,&line1,((EntitySelectedEvent *) event)->getp());
                        if (i1.nintersections() == 1)
                          p = ((PointE *)i1.intersection(0))->getp();

                        Intersect i2(ellipse,&line2,((EntitySelectedEvent *) event)->getp());
                        if (i2.nintersections() == 1 && (((PointE *)i2.intersection(0))->getp() - ((EntitySelectedEvent *) event)->getp()).length() < (p-((EntitySelectedEvent *) event)->getp()).length())
                          p = ((PointE *)i2.intersection(0))->getp();

                     }
                   else
                     {  if (((EntitySelectedEvent *) event)->getendselected() == 1)
                          p = ((LinearEntity *)((EntitySelectedEvent *) event)->getentity())->start();
                        else
                          p = ((LinearEntity *)((EntitySelectedEvent *) event)->getentity())->end();
                     }
                   Coordinate3dEvent *cevent = new Coordinate3dEvent(p,xa,ya,NULL,0);
                   state.sendevent(cevent);
                   *cstate = ExitState;
               }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(linear_mask,figure_entity,lineardimension_entity,end_list);
       cadwindow->prompt->normalprompt(NCMAIN+212);
    }
  else if (*cstate == ExitState)
    program->setcursorshape(CursorRestore);

}

/**
 * @brief position_command6
 * @param cstate
 * @param event
 *
 * the select a chain of entities command
 * KMJ : working in Qt version
 */
void position_command6(int *cstate,HighLevelEvent *event,void **)
{static EntityList list;
 EntityList tracelist;
 Entity *e;
  switch (*cstate)
    {  case InitialState :
         list = state.getselection().copy();
         state.destroyselection(0);
         *cstate = 1;
         break;
       case 1 :
         state.destroyselection(1);
         if (event->isa(EntitySelected))
           {  if (db.geometry.trace((LinearEntity *)((EntitySelectedEvent *)event)->getentity(),&tracelist))
                {  if (! ((EntitySelectedEvent *)event)->getaddtoselection())
                     {  list.unhighlight();
                        list.destroy();
                     }
                   for (tracelist.start() ; (e = tracelist.next()) != 0 ; )
                     {  if (((EntitySelectedEvent *)event)->getaddtoselection())
                          {  if (! list.inlist(e))
                               {  list.add(e);  e->highlight();
                               }
                             else
                               {  list.del(e);  e->unhighlight();
                               }
                          }
                        else
                          {  list.add(e);  e->highlight();
                          }
                     }
                   tracelist.destroy();
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  state.setselection(list,1);
              *cstate = ExitState;
           }
         break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(linear_mask,end_list);
       cadwindow->prompt->normalprompt(NCMAINB);
    }
}

#define MAX_PWVERTICES 1000

struct RBPosition10_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QList<QGraphicsItem*> qlist;
#endif
   EntityList list;
   Point *pwvertice;
   int nvertices;
   View3dWindow *window;
   BitMask entitymask;
   RBPosition10_data() : entitymask(32) {}
};

// KMJ this rubberband does not need to work in multiple views
void RBPosition10(int drawmode,void *data,int x,int y,View3dSurface *s)
{RBPosition10_data *rbdata = (RBPosition10_data *) data;
 int i;
 Point p1,p2;

  if (drawmode == RB_TIMER)
    return;  //  Ignore timer events

  QGraphicsItem *qline = 0;

  if (rbdata->window != 0)
  {
      QGraphicsScene *scene = ((View3dWindow*)rbdata->window)->getScene();
      if(scene  != 0)
      {
           for (i = 0 ; i < rbdata->qlist.size(); i++)
           {
               if(rbdata->qlist.at(i)) scene->removeItem(rbdata->qlist.at(i));
               delete rbdata->qlist.at(i); rbdata->qlist[i]=0;
           }
      }

       rbdata->window->setup(0,0.0,DM_INVERT,0,0,0,0);
       for (i = 1 ; i < rbdata->nvertices ; i++)
       {
            p1 = rbdata->window->uvtoscreen(rbdata->pwvertice[i-1]);
            p2 = rbdata->window->uvtoscreen(rbdata->pwvertice[i]);
            qline = rbdata->window->line(p1.x,p1.y,p2.x,p2.y,1);
            rbdata->qlist[i] = qline;
       }
       if (rbdata->nvertices > 0)
       {
            p1 = rbdata->window->uvtoscreen(rbdata->pwvertice[rbdata->nvertices-1]);
            qline = rbdata->window->line(p1.x,p1.y,x,y,1);
            if(qline) rbdata->qlist.append(qline);
#if MAJOR_VERSION > 7
            p2 = rbdata->window->uvtoscreen(rbdata->pwvertice[0]);
            qline = rbdata->window->line(p2.x,p2.y,x,y,1);
            if(qline) rbdata->qlist.append(qline);
#endif
       }
  }
 
}

/**
 * @brief position_command10
 * @param cstate
 * @param event
 * @param data
 *
 * Select entites using a poly fence or poly window
 *
 * KMJ : working in Qt version 7
 * version 8
 * upgraded as per enhancement u2 to used a closed loop rubberband
 *
 */
void position_command10(int *cstate,HighLevelEvent *event,void **data)
{RBPosition10_data *rbdata = (RBPosition10_data *) *data;
 EntityList picklist;
 Entity *e;
 int inside;
 
  switch (*cstate)
    {  case InitialState :
         *data = rbdata = new RBPosition10_data;  
         rbdata->pwvertice = new Point[MAX_PWVERTICES]; 
         rbdata->nvertices = 0;
         rbdata->window = 0;
         rbdata->list = state.getselection().copy();
         rbdata->entitymask = state.selmask.entity;
         rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         rbdata->qlist.clear();
         state.destroyselection(0);
         *cstate = 1;
         break;
       case 1 :

#if MAJOR_VERSION > 7
          state.destroyselection(1);
          rubberband.end(1);
         if (event->isa(Coordinateuv))
           {  rbdata->pwvertice[rbdata->nvertices++] = ((CoordinateuvEvent *) event)->coordinate();
              if (rbdata->nvertices == 1)
                rbdata->window = ((CoordinateuvEvent *) event)->window();
              if (rbdata->window == 0)
                rbdata->nvertices = 0;  //  A window must be specified
           }
         else if(event->isa(Abort) || event->isa(Exit))
           {
                if (rbdata->nvertices == MAX_PWVERTICES || rbdata->nvertices > 2 )
                {  state.selmask.entity = rbdata->entitymask;
                   inside = cadwindow->getcontrolstate() ? 2 : 1;
                   if (db.geometry.pick(rbdata->window,rbdata->nvertices,rbdata->pwvertice,inside,&picklist))
                     {  if (! ((CoordinateuvEvent *)event)->getaddtoselection())
                          {  rbdata->list.unhighlight();
                             rbdata->list.destroy();
                          }
                        for (picklist.start() ; (e = picklist.next()) != 0 ; )
                          {  if (((CoordinateuvEvent *)event)->getaddtoselection())
                               {  if (! rbdata->list.inlist(e))
                                    {  rbdata->list.add(e);  e->highlight();
                                    }
                                  else
                                    {  rbdata->list.del(e);  e->unhighlight();
                                    }
                               }
                             else
                               {  rbdata->list.add(e);  e->highlight();
                               }
                          }
                        picklist.destroy();
                     }
                   rbdata->nvertices = 0;
                }
                state.setselection(rbdata->list,1);
                *cstate = ExitState;
           }
         break;
#else
          state.destroyselection(1);
          rubberband.end(1);
         if (event->isa(Coordinateuv))
           {  rbdata->pwvertice[rbdata->nvertices++] = ((CoordinateuvEvent *) event)->coordinate();
              if (rbdata->nvertices == 1)
                rbdata->window = ((CoordinateuvEvent *) event)->window();
              if (rbdata->window == 0)
                rbdata->nvertices = 0;  //  A window must be specified 
              if (rbdata->nvertices == MAX_PWVERTICES || rbdata->nvertices > 2 && (rbdata->window->uvtoscreen(rbdata->pwvertice[rbdata->nvertices-1]) - rbdata->window->uvtoscreen(rbdata->pwvertice[0])).length() < 8)
                {  state.selmask.entity = rbdata->entitymask;
                   inside = cadwindow->getcontrolstate() ? 2 : 1;
                   if (db.geometry.pick(rbdata->window,rbdata->nvertices,rbdata->pwvertice,inside,&picklist))
                     {  if (! ((CoordinateuvEvent *)event)->getaddtoselection())
                          {  rbdata->list.unhighlight();
                             rbdata->list.destroy();
                          }
                        for (picklist.start() ; (e = picklist.next()) != 0 ; )
                          {  if (((CoordinateuvEvent *)event)->getaddtoselection())
                               {  if (! rbdata->list.inlist(e))
                                    {  rbdata->list.add(e);  e->highlight();
                                    }
                                  else
                                    {  rbdata->list.del(e);  e->unhighlight();
                                    }
                               }
                             else
                               {  rbdata->list.add(e);  e->highlight();
                               }
                          }
                        picklist.destroy();
                     }
                   rbdata->nvertices = 0; 
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           {  state.setselection(rbdata->list,1);
              *cstate = ExitState;
           }
         break;
#endif
    }

  if (*cstate == 1)
  {
       rubberband.begin(1);
       rubberband.add(RBPosition10,rbdata);
       state.selmask.entity.clearandset(uv_mask,end_list);
       cadwindow->prompt->normalprompt(NCMAINB+86);
  }
  else if (*cstate == ExitState)
  {
      if(rbdata->window !=0)
      {
          QGraphicsScene *scene = ((View3dWindow*)rbdata->window)->getScene();
          if(scene  != 0)
          {
              for (int i = 0 ; i < rbdata->qlist.size(); i++)
              {
                  if(rbdata->qlist.at(i)) scene->removeItem(rbdata->qlist.at(i));
                  delete rbdata->qlist.at(i); rbdata->qlist[i]=0;
              }
          }
      }
      delete rbdata->pwvertice;
      delete rbdata;
  }

}

struct RBPosition11_data
{
#ifdef USING_WIDGETS
   qreal zValue;
   QGraphicsItem *qgi1,*qgi2,*qgi3;
   RBviewmapperList qgivlist1,qgivlist2,qgivlist3;
#endif
   int state;
   Point p1,p2,p3;
};

#ifdef NEW_RUBBERB
void RBPosition11(int drawmode,void *data,Point *p2,View3dSurface *vs)
{RBPosition11_data *rbdata = (RBPosition11_data *) data;

  if (drawmode == RB_TIMER)
    return;  //  Ignore timer events

  if(drawmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
#if 0
      for(int i=0;i<rbdata->qgivlist1.size();i++)
      {
          RBviewmapper vmap = rbdata->qgivlist1.at(i);
          //if(vmap.vs == vs)
          {
              if(vmap.qgi && vmap.qgi->scene() == vmap.vs->getScene())
              {
                vmap.vs->getScene()->removeItem(vmap.qgi);
                delete vmap.qgi;
                vmap.qgi=0;
              }
          }
      }
      rbdata->qgivlist1.clear();
      for(int i=0;i<rbdata->qgivlist2.size();i++)
      {
          RBviewmapper vmap = rbdata->qgivlist2.at(i);
          //if(vmap.vs == vs)
          {
              if(vmap.qgi && vmap.qgi->scene() == vmap.vs->getScene())
              {
                vmap.vs->getScene()->removeItem(vmap.qgi);
                delete vmap.qgi;
                vmap.qgi=0;
              }
          }
      }
      rbdata->qgivlist2.clear();
      for(int i=0;i<rbdata->qgivlist3.size();i++)
      {
          RBviewmapper vmap = rbdata->qgivlist3.at(i);
          //if(vmap.vs == vs)
          {
              if(vmap.qgi && vmap.qgi->scene() == vmap.vs->getScene())
              {
                vmap.vs->getScene()->removeItem(vmap.qgi);
                delete vmap.qgi;
                vmap.qgi=0;
              }
          }
      }
      rbdata->qgivlist3.clear();
      return;
#endif
  }
  if (rbdata->state == 2)
    {  rbdata->p2 = db.workplanes.getcurrent()->modeltoworkplane(*p2);
       rbdata->p2.x = db.workplanes.getcurrent()->modeltoworkplane(rbdata->p1).x;
       rbdata->p2 = db.workplanes.getcurrent()->workplanetomodel(rbdata->p2);

       Line line1(rbdata->p1,rbdata->p2);
       line1.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line1.draw(DM_INVERT);
       //rbdata->qgivlist1.push_back(RBviewmapper(vs,line1.qgi));
       //line1.qgi=0;
       Line line2(rbdata->p2,*p2);
       line2.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line2.draw(DM_INVERT);
       //rbdata->qgivlist2.push_back(RBviewmapper(vs,line2.qgi));
       //line2.qgi=0;
    }
  else if (rbdata->state == 3)
    {  rbdata->p3 = db.workplanes.getcurrent()->modeltoworkplane(*p2);
       rbdata->p3.x = db.workplanes.getcurrent()->modeltoworkplane(rbdata->p1).x;
       rbdata->p3.y = db.workplanes.getcurrent()->modeltoworkplane(rbdata->p2).y;
       rbdata->p3 = db.workplanes.getcurrent()->workplanetomodel(rbdata->p3);

       Line line1(rbdata->p1,rbdata->p3);
       line1.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line1.draw(DM_INVERT);
       //rbdata->qgivlist1.push_back(RBviewmapper(vs,line1.qgi));
       //line1.qgi=0;
       Line line2(rbdata->p2,rbdata->p3);
       line2.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line2.draw(DM_INVERT);
       //rbdata->qgivlist2.push_back(RBviewmapper(vs,line2.qgi));
       //line2.qgi=0;
       Line line3(rbdata->p3,*p2);
       line3.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line3.draw(DM_INVERT);
       //rbdata->qgivlist3.push_back(RBviewmapper(vs,line3.qgi));
       //line3.qgi=0;
    }
}
#else
void RBPosition11(int drawmode,void *data,Point *p2,View3dSurface *)
{RBPosition11_data *rbdata = (RBPosition11_data *) data;

  if (drawmode == RB_TIMER)
    return;  //  Ignore timer events

  if (rbdata->state == 2)
    {  rbdata->p2 = db.workplanes.getcurrent()->modeltoworkplane(*p2);
       rbdata->p2.x = db.workplanes.getcurrent()->modeltoworkplane(rbdata->p1).x;
       rbdata->p2 = db.workplanes.getcurrent()->workplanetomodel(rbdata->p2);  

#ifdef USING_WIDGETS
       Line line1(rbdata->p1,rbdata->p2);
       line1.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line1.qgi = rbdata->qgi1; line1.zValue = rbdata->zValue;
       line1.draw(DM_INVERT);
       rbdata->qgi1 = line1.qgi; rbdata->zValue = line1.zValue;

       Line line2(rbdata->p2,*p2);
       line2.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line2.qgi = rbdata->qgi2; line2.zValue = rbdata->zValue;
       line2.draw(DM_INVERT);
       rbdata->qgi2 = line2.qgi; rbdata->zValue = line2.zValue;
#else
       Line line1(rbdata->p1,rbdata->p2);
       line1.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line1.draw(DM_INVERT);
       Line line2(rbdata->p2,*p2);
       line2.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line2.draw(DM_INVERT);
#endif

    }
  else if (rbdata->state == 3)
    {  rbdata->p3 = db.workplanes.getcurrent()->modeltoworkplane(*p2);
       rbdata->p3.x = db.workplanes.getcurrent()->modeltoworkplane(rbdata->p1).x;
       rbdata->p3.y = db.workplanes.getcurrent()->modeltoworkplane(rbdata->p2).y;
       rbdata->p3 = db.workplanes.getcurrent()->workplanetomodel(rbdata->p3);  

#ifdef USING_WIDGETS
       Line line1(rbdata->p1,rbdata->p3);
       line1.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line1.qgi = rbdata->qgi1; line1.zValue = rbdata->zValue;
       line1.draw(DM_INVERT);
       rbdata->qgi1 = line1.qgi; rbdata->zValue = line1.zValue;

       Line line2(rbdata->p2,rbdata->p3);
       line2.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line2.qgi = rbdata->qgi2; line2.zValue = rbdata->zValue;
       line2.draw(DM_INVERT);
       rbdata->qgi2 = line2.qgi; rbdata->zValue = line2.zValue;

       Line line3(rbdata->p3,*p2);
       line3.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line3.qgi = rbdata->qgi3; line3.zValue = rbdata->zValue;
       line3.draw(DM_INVERT);
       rbdata->qgi3 = line3.qgi; rbdata->zValue = line3.zValue;
#else
       Line line1(rbdata->p1,rbdata->p3);
       line1.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line1.draw(DM_INVERT);
       Line line2(rbdata->p2,rbdata->p3);
       line2.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line2.draw(DM_INVERT);
       Line line3(rbdata->p3,*p2);
       line3.EntityHeader::change(line1.getcolour(),line1.getlayer(),1,line1.getweight());
       line3.draw(DM_INVERT);
#endif
    }

}
#endif

/**
 * @brief position_command11
 * @param cstate
 * @param event
 * @param data
 *
 * select in x y z
 *
 * KMJ : working in Qt version
 */
void position_command11(int *cstate,HighLevelEvent *event,void **data)
{RBPosition11_data *rbdata = (RBPosition11_data *) *data;
 EntityList picklist;
 
  switch (*cstate)
    {  case InitialState :
         *data = rbdata = new RBPosition11_data;  
         *cstate = 1;
         rbdata->state = *cstate; 
         rbdata->qgi1 = rbdata->qgi2 = rbdata->qgi3 = 0;
         rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
         rbdata->qgivlist1.clear();
         rbdata->qgivlist2.clear();
         rbdata->qgivlist3.clear();
         break;
       case 1 :
         if (event->isa(Coordinate3d))
           {  rbdata->p1 = ((Coordinate3dEvent *) event)->getp();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(1);
         if (event->isa(Coordinate3d))
           {  rbdata->p2 = ((Coordinate3dEvent *) event)->getp();
              *cstate = 3;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 3 :
         //rubberband.end(1);
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  rbdata->p3 = db.workplanes.getcurrent()->modeltoworkplane(((Coordinate3dEvent *) event)->getp());
              rbdata->p3.x = db.workplanes.getcurrent()->modeltoworkplane(rbdata->p1).x;
              rbdata->p3.y = db.workplanes.getcurrent()->modeltoworkplane(rbdata->p2).y;
              rbdata->p3 = db.workplanes.getcurrent()->workplanetomodel(rbdata->p3);  
              Coordinate3dEvent *cevent = new Coordinate3dEvent(rbdata->p3,((Coordinate3dEvent *) event)->getxaxis(),((Coordinate3dEvent *) event)->getyaxis(),NULL,0);
              state.sendevent(cevent);
              *cstate = ExitState;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }

  if (*cstate == 1)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the position for the X coordinate");
    }
  else if (*cstate == 2)
    {  rbdata->state = *cstate;
       //rubberband.begin(1);
       rubberband.begin(0);
       rubberband.add(RBPosition11,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the position for the Y coordinate");
    }
  else if (*cstate == 3)
    {  rbdata->state = *cstate;
       //rubberband.begin(1);
       rubberband.begin(0);
       rubberband.add(RBPosition11,rbdata);
       state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the position for the Z coordinate");
    }
  else if (*cstate == ExitState)
  {
#ifndef USING_WIDGETS
#ifndef NEW_RUBBERB
      for(int i=0;i<rbdata->qgivlist1.size();i++)
      {
          RBviewmapper vmap = rbdata->qgivlist1.at(i);
          vmap.vs->getScene()->removeItem(vmap.qgi);
      }
      rbdata->qgivlist1.clear();
      for(int i=0;i<rbdata->qgivlist2.size();i++)
      {
          RBviewmapper vmap = rbdata->qgivlist2.at(i);
          vmap.vs->getScene()->removeItem(vmap.qgi);
      }
      rbdata->qgivlist2.clear();
      for(int i=0;i<rbdata->qgivlist3.size();i++)
      {
          RBviewmapper vmap = rbdata->qgivlist3.at(i);
          vmap.vs->getScene()->removeItem(vmap.qgi);
      }
      rbdata->qgivlist3.clear();
      return;
#else
    if(rbdata->qgi1)
        cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi1);
    delete rbdata->qgi1; rbdata->qgi1=0;
    if(rbdata->qgi2)
        cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi2);
    delete rbdata->qgi2; rbdata->qgi2=0;
    if(rbdata->qgi3)
        cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi3);
    delete rbdata->qgi3; rbdata->qgi3=0;
#endif
#endif
    delete rbdata;
  }
}

/**
 * @brief position_command7
 * @param cstate
 * @param event
 *
 * locate intersection point
 *
 * KMJ : working in Qt version
 */
void position_command7(int *cstate,HighLevelEvent *event,void **)
{Point p,pickp,xa,ya;
 Entity *e1,*e2;
 int x,y;
 switch (*cstate)
    {  case InitialState :
         *cstate = 1;
         program->setcursorshape(CursorPosition);
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e1 = ((EntitySelectedEvent *) event)->getentity();
              p = ((EntitySelectedEvent *) event)->getp();
              xa = ((EntitySelectedEvent *) event)->getxaxis();
              ya = ((EntitySelectedEvent *) event)->getyaxis();
              pickp = ((EntitySelectedEvent *) event)->getwindow()->modeltoscreen(p);
              state.excludelist.add(e1);
              x = int(pickp.x);
              y = int(((EntitySelectedEvent *) event)->getwindow()->getheight() - 1.0 - pickp.y);
              if ((e2 = db.geometry.pick(((EntitySelectedEvent *) event)->getwindow(),0,0,x,y,0,(Point *)0)) != 0)
                {Intersect i(e1,e2,p);
                   if (i.nintersections() == 1)
                     {  p = ((PointE *)i.intersection(0))->getp();
                        Coordinate3dEvent *cevent = new Coordinate3dEvent(p,xa,ya,NULL,0);
                        state.sendevent(cevent);
                        *cstate = ExitState;
                     }
                }
              state.excludelist.del(e1);
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
//
//  About to enter state actions
//
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(linear_mask,end_list);
       cadwindow->prompt->normalprompt(NCMAINB+25);
    }
  else if (*cstate == ExitState)
    program->setcursorshape(CursorRestore);
}

struct RBPosition8_data
  {
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi,*qgi1,*qgi2;
#endif
   int fixdistance1,fixdistance2;
   double distance1,distance2;  
   Entity *e1,*e2;
   Point p1,p2,xaxis,yaxis;
  };

class POCheckBoxDialogControl : public CheckBoxDialogControl
  {public:
    POCheckBoxDialogControl(int id,int *value) : CheckBoxDialogControl(id,value)  { }
    void load(Dialog *dialog);
    int select(Dialog *dialog);
  };


void POCheckBoxDialogControl::load(Dialog *dialog)
{ CheckBoxDialogControl::load(dialog);
  EnableWindow(GetDlgItem(dialog->gethdlg(),101+(id-100)),*ivalue == 1);
  //EnableWindow(GetDlgItem(dialog->gethdlg(),201+(id-100)),*ivalue == 1);
  //EnableWindow(GetDlgItem(dialog->gethdlg(),1101+(id-100)),*ivalue == 1);
}

int POCheckBoxDialogControl::select(Dialog *dialog)
{int value;
  CheckBoxDialogControl::select(dialog);
  currentvalue(dialog,&value);
  EnableWindow(GetDlgItem(dialog->gethdlg(),101+(id-100)),value == 1);
  //EnableWindow(GetDlgItem(dialog->gethdlg(),201+(id-100)),value == 1);
  //EnableWindow(GetDlgItem(dialog->gethdlg(),1101+(id-100)),value == 1);
  return 0;
}

int GetOffsetPoint(RBPosition8_data *rbdata,int draw,Point p2,Point *p)
{Point p3,p4,n;
 double o1,o2,radius;
 Entity *e1,*e2;
 int found;
 char message[300];

 
  o1 = o2 = 0.0;
  e1 = e2 = 0;
  found = 0;

  if (rbdata->e1->isa(line_entity))
    {  p3 = ((Line *)rbdata->e1)->getp1();
       p4 = ((Line *)rbdata->e1)->getp2();
       n = ((p3 - p4).cross(rbdata->xaxis.cross(rbdata->yaxis))).normalize();
       if ((p2 - Line(p3+n,p4+n).nearp(p2)).length() > (p2 - Line(p3-n,p4-n).nearp(p2)).length())
         n = -n;
       if (rbdata->fixdistance1)
         o1 = rbdata->distance1;
       else
         o1 = (p2 - ((Line *)rbdata->e1)->nearp(p2)).length();
       e1 = new Line(p3+n*o1,p4+n*o1);
    }
  else if (rbdata->e1->isa(circle_entity))
    {  if ((p2 - ((Circle *)rbdata->e1)->getorigin()).length() < ((Circle *)rbdata->e1)->getradius())
         {  if (rbdata->fixdistance1)
              radius = ((Circle *)rbdata->e1)->getradius() - rbdata->distance1;
            else
              radius = (p2 - ((Circle *)rbdata->e1)->getorigin()).length();
         }
       else
         {  if (rbdata->fixdistance1)
              radius = ((Circle *)rbdata->e1)->getradius() + rbdata->distance1;
            else
              radius = (p2 - ((Circle *)rbdata->e1)->getorigin()).length();
         }
       e1 = new Circle(((Circle *)rbdata->e1)->getorigin(),((Circle *)rbdata->e1)->getxaxis(),((Circle *)rbdata->e1)->getyaxis(),radius,((Circle *)rbdata->e1)->getstarta(),((Circle *)rbdata->e1)->getsweepa());
    }
  else if (rbdata->e1->isa(curve_entity))
    {Point zaxis(0.0,0.0,1.0),startp,dir1,dir2;
     int reverse;
     double t,d;
     EntityList olist;

       if (rbdata->fixdistance1)
         d = rbdata->distance1;
       else
         d = 0.0;
       
       reverse = 0;
       dir1 = ((LinearEntity *)rbdata->e1)->direction(0.0);
       for (t = 0.0 ; t <= 1.0 ; t += 0.1)
         {  dir2 = ((LinearEntity *)rbdata->e1)->direction(t);
            if (dir1.dot(dir2) < 0.99)
              {  zaxis = dir1.cross(dir2).normalize();
                 break;
              }
         }
       ((LinearEntity *)rbdata->e1)->beginoffset(p2,&zaxis,&d,0,&reverse,&startp);
       ((LinearEntity *)rbdata->e1)->offset(zaxis,d,0,0,&reverse,&startp,&olist);
       olist.start();
       if (olist.length() > 0)
         e1 = olist.next();

    }



  if (rbdata->e2->isa(line_entity))
    {  p3 = ((Line *)rbdata->e2)->getp1();
       p4 = ((Line *)rbdata->e2)->getp2();
       n = ((p3 - p4).cross(rbdata->xaxis.cross(rbdata->yaxis))).normalize();
       if ((p2 - Line(p3+n,p4+n).nearp(p2)).length() > (p2 - Line(p3-n,p4-n).nearp(p2)).length())
         n = -n;
       if (rbdata->fixdistance2)
         o2 = rbdata->distance2;
       else
         o2 = (p2 - ((Line *)rbdata->e2)->nearp(p2)).length();
       e2 = new Line(p3+n*o2,p4+n*o2);
    }
  else if (rbdata->e2->isa(circle_entity))
    {  if ((p2 - ((Circle *)rbdata->e2)->getorigin()).length() < ((Circle *)rbdata->e2)->getradius())
         {  if (rbdata->fixdistance2)
              radius = ((Circle *)rbdata->e2)->getradius() - rbdata->distance2;
            else
              radius = (p2 - ((Circle *)rbdata->e2)->getorigin()).length();
         }
       else
         {  if (rbdata->fixdistance2)
              radius = ((Circle *)rbdata->e2)->getradius() + rbdata->distance2;
            else
              radius = (p2 - ((Circle *)rbdata->e2)->getorigin()).length();
         }
       e2 = new Circle(((Circle *)rbdata->e2)->getorigin(),((Circle *)rbdata->e2)->getxaxis(),((Circle *)rbdata->e2)->getyaxis(),radius,((Circle *)rbdata->e2)->getstarta(),((Circle *)rbdata->e2)->getsweepa());
    } 
  else if (rbdata->e2->isa(curve_entity))
    {Point zaxis(0.0,0.0,1.0),startp,dir1,dir2;
     int reverse;
     double t,d;
     EntityList olist;

       if (rbdata->fixdistance2)
         d = rbdata->distance2;
       else
         d = 0.0;
       
       reverse = 0;
       dir1 = ((LinearEntity *)rbdata->e2)->direction(0.0);
       for (t = 0.0 ; t <= 1.0 ; t += 0.1)
         {  dir2 = ((LinearEntity *)rbdata->e2)->direction(t);
            if (dir1.dot(dir2) < 0.99)
              {  zaxis = dir1.cross(dir2).normalize();
                 break;
              }
         }
       ((LinearEntity *)rbdata->e2)->beginoffset(p2,&zaxis,&d,0,&reverse,&startp);
       ((LinearEntity *)rbdata->e2)->offset(zaxis,d,0,0,&reverse,&startp,&olist);
       olist.start();
       if (olist.length() > 0)
         e2 = olist.next();

    }

  if (e1 != 0 && e2 != 0)
    {
      if (draw)
      {
#ifndef USING_WIDGETS
          e1->qgi = rbdata->qgi1; e1->zValue = rbdata->zValue;
          e1->draw(DM_INVERT);
          rbdata->qgi1 = e1->qgi; rbdata->zValue = e1->zValue;

          e2->qgi = rbdata->qgi2; e2->zValue = rbdata->zValue;
          e2->draw(DM_INVERT);
          rbdata->qgi2 = e2->qgi; rbdata->zValue = e2->zValue;
#else
            e1->draw(DM_INVERT);
            e2->draw(DM_INVERT);
#endif
      }

       Intersect i(e1,e2,p2);
       if (i.intersection(0) != 0)
         {
#ifndef USING_WIDGETS
           if (draw)
           {
               i.intersection(0)->qgi = rbdata->qgi; i.intersection(0)->zValue = rbdata->zValue;
               i.intersection(0)->draw(DM_INVERT);
               rbdata->qgi = i.intersection(0)->qgi; rbdata->zValue = i.intersection(0)->zValue;
           }
#else
           if (draw)
              i.intersection(0)->draw(DM_INVERT);
#endif
            *p = ((PointE *)i.intersection(0))->getp();
            found = 1;
            sprintf(message,"Locate the intersection position  Distance 1 = %.4lf   Distance 2 = %.4lf",o1,o2);
            cadwindow->prompt->normalprompt(message);

         }
    }

  delete e1;
  delete e2;

  return found;

}

void RBPosition8(int dmode,void *data,Point *p2,View3dSurface *)
{
  Point p;
  if(dmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  GetOffsetPoint((RBPosition8_data *) data,1,*p2,&p);
}

/**
 * @brief position_command8
 * @param cstate
 * @param event
 * @param data
 *
 * Locate offset points
 *
 *
 */
void position_command8(int *cstate,HighLevelEvent *event,void **data)
{
 RBPosition8_data *rbdata = (RBPosition8_data *) *data;
 Dialog dialog("PositionOffset_Dialog");
 DialogControl *dc;
 Point p;

  switch (*cstate)
    {  case InitialState :
         program->setcursorshape(CursorPosition);
         rbdata = new RBPosition8_data;

#ifdef USING_WIDGETS
         rbdata->qgi = rbdata->qgi1 = rbdata->qgi2 = 0;
         rbdata->zValue = QGDRAGZVALUE(QGMAXZVALUE);
#endif

         *data = rbdata;
         if (rbdata == 0)
           *cstate = ExitState;
         else
           {  rbdata->fixdistance1 = v.getinteger("ps::fixdistance1");
              rbdata->fixdistance2 = v.getinteger("ps::fixdistance2");
              rbdata->distance1 = v.getreal("ps::distance1");
              rbdata->distance2 = v.getreal("ps::distance2");
              dialog.add(dc = new POCheckBoxDialogControl(100,&rbdata->fixdistance1));
              dialog.add(dc = new RealDialogControl(101,&rbdata->distance1,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1101,dc));
              dialog.add(dc = new POCheckBoxDialogControl(102,&rbdata->fixdistance2));
              dialog.add(dc = new RealDialogControl(103,&rbdata->distance2,ScaleByLength));
              //dialog.add(new ScrollBarDialogControl(1103,dc));
              cadwindow->clearAllRubberBands();
              if (dialog.process() == TRUE)
                {  v.setinteger("ps::fixdistance1",rbdata->fixdistance1);
                   v.setinteger("ps::fixdistance2",rbdata->fixdistance2);
                   v.setreal("ps::distance1",rbdata->distance1);
                   v.setreal("ps::distance2",rbdata->distance2);
                   *cstate = 1;
                }
              else
                *cstate = ExitState;
           }
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  rbdata->e1 = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p1 = ((EntitySelectedEvent *) event)->getp();
              rbdata->xaxis = ((EntitySelectedEvent *) event)->getxaxis();
              rbdata->yaxis = ((EntitySelectedEvent *) event)->getyaxis();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         if (event->isa(EntitySelected))
           {  rbdata->e2 = ((EntitySelectedEvent *) event)->getentity();
              rbdata->p2 = ((EntitySelectedEvent *) event)->getp();
             *cstate = 3;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 3 :
         //rubberband.end(1);
         rubberband.end(0);
         if (event->isa(Coordinate3d))
           {  if (GetOffsetPoint(rbdata,0,((Coordinate3dEvent *) event)->getp(),&p) != 0)
                {  Coordinate3dEvent *cevent = new Coordinate3dEvent(p,rbdata->xaxis,rbdata->yaxis,NULL,0);
                   state.sendevent(cevent);
                   *cstate = ExitState;
                }
              else
                *cstate = 1;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }

  //
  //  About to enter state actions
  //
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(line_entity,circle_entity,curve_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the first offset entity");
    }
  else if (*cstate == 2)
    {  state.selmask.entity.clearandset(line_entity,circle_entity,curve_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the second offset entity");
    }
  else if (*cstate == 3)
    {  state.selmask.entity.clearandset(xyz_mask,end_list);
       cadwindow->prompt->normalprompt("Locate the intersection position");
       //rubberband.begin(1);
       rubberband.begin(0);
       rubberband.add(RBPosition8,rbdata);
    }
  else if (*cstate == ExitState)
  {
#ifndef USING_WIDGETS
      if(rbdata->qgi)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi; rbdata->qgi=0;
      if(rbdata->qgi1)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi1);
      delete rbdata->qgi1; rbdata->qgi1=0;
      if(rbdata->qgi2)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi2);
      delete rbdata->qgi2; rbdata->qgi2=0;
#endif
      program->setcursorshape(CursorRestore);
      delete rbdata;
  }

}

void transformmenu_command(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Transform"));
  *cstate = ExitState;
}

void dynamicviewmenu_command(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("DynamicView"));
  *cstate = ExitState;
}

void workplanemenu_command(int *cstate,HighLevelEvent *,void **)
{ cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("WorkPlane"));
  *cstate = ExitState;
}

struct RBPosition9_data
{
#ifdef USING_WIDGETS
    qreal zValue;
    QGraphicsItem *qgi;
#endif
    Entity *e;
};

#ifdef NEW_RUBBERB
void RBPosition9(int drawmode,void *data,Point *p2,View3dSurface *v)
{
  RBPosition9_data *rbdata = (RBPosition9_data *) data;

  if(drawmode == RB_ERASE)
  {
      cadwindow->clearAllRubberBands();
      return;
  }
  PointE point(rbdata->e->nearp(*p2));
  point.draw(DM_INVERT);
}
#else
void RBPosition9(int drawmode,void *data,Point *p2,View3dSurface *)
{
  RBPosition9_data *rbdata = (RBPosition9_data *) data;
  PointE point(rbdata->e->nearp(*p2));
  point.qgi = rbdata->qgi; point.zValue = rbdata->zValue;
  point.draw(DM_INVERT);
  rbdata->qgi = point.qgi; rbdata->zValue = point.zValue;
}
#endif

/**
 * @brief position_command9
 * @param cstate
 * @param event
 * @param data
 *
 * Locate an apparent intersection point
 * KMJ : works in the Qt version
 */
void position_command9(int *cstate,HighLevelEvent *event,void **data)
{RBPosition9_data *rbdata = (RBPosition9_data *) *data;
 Point p,p1,p2,p3,p4,xa,ya;
 Entity *e;
 double t;
  switch (*cstate)
    {  case InitialState :
         program->setcursorshape(CursorPosition);
         if ((rbdata = new RBPosition9_data) == NULL)
           *cstate = ExitState;
         else
         {
           *cstate = 1;
           rbdata->zValue=QGDRAGZVALUE(QGMAXZVALUE);
           rbdata->qgi=0;
         }
         *data = rbdata;
         break;
       case 1 :
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              rbdata->e = ((EntitySelectedEvent *) event)->getentity();
              *cstate = 2;
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
       case 2 :
         rubberband.end(1);
         if (event->isa(EntitySelected))
           {  state.destroyselection(1);
              e = ((EntitySelectedEvent *) event)->getentity();
              p1 = ((Line *)rbdata->e)->getp1();
              p2 = ((Line *)rbdata->e)->getp2();
              p3 = ((Line *)e)->getp1();
              p4 = ((Line *)e)->getp2();
              CoordinateSystem cs(p1,((EntitySelectedEvent *)event)->getwindow()->getuaxis(),((EntitySelectedEvent *)event)->getwindow()->getvaxis());
              p1 = cs.frommodel(p1);  p1.z = 0.0;
              p2 = cs.frommodel(p2);  p2.z = 0.0;
              p3 = cs.frommodel(p3);  p3.z = 0.0;
              p4 = cs.frommodel(p4);  p4.z = 0.0;
              Line l1(p1,p2);
              Line l2(p3,p4);
              Intersect i(&l1,&l2);
              if (i.nintersections() == 1)
                {  p = ((PointE *)i.intersection(0))->getp();
                   t = (p - p1).length() / (p2 - p1).length();
                   if ((p - p1).dot((p2 - p1)) < 0.0)
                     t = -t;
                   p = ((Line *)rbdata->e)->getp1() + (((Line *)rbdata->e)->getp2() - ((Line *)rbdata->e)->getp1()) * t;
                   xa = ((EntitySelectedEvent *)event)->getxaxis();  
                   ya = ((EntitySelectedEvent *)event)->getyaxis();  
                   Coordinate3dEvent *cevent = new Coordinate3dEvent(p,xa,ya,NULL,0);
                   state.sendevent(cevent);
                   *cstate = ExitState;
                }
           }
         else if (event->isa(Abort) || event->isa(Exit))
           *cstate = ExitState;
         break;
    }
  if (*cstate == 1)
    {  state.selmask.entity.clearandset(line_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the first intersection line");
    }
  else if (*cstate == 2)
    {  rubberband.begin(1);
       rubberband.add(RBPosition9,rbdata);
       state.selmask.entity.clearandset(line_entity,end_list);
       cadwindow->prompt->normalprompt("Locate the second intersection line");
    }
  else if (*cstate == ExitState)
    {
#ifndef USING_WIDGETS
      if(rbdata->qgi)
          cadwindow->getcurrentsurface()->getScene()->removeItem(rbdata->qgi);
      delete rbdata->qgi; rbdata->qgi=0;
#endif
       program->setcursorshape(CursorRestore);
       delete rbdata;
    }
}

void OpenBrowser(char *url)
{
#ifdef USING_WIDGETS
    QString qurl(url);
    QDesktopServices::openUrl(QUrl(url));
#else
 HKEY hkey;
 DWORD length;
 RCCHAR defaultbrowser[300],path[300];

  if (RegOpenKeyEx(HKEY_CLASSES_ROOT,_RCT("http\\shell\\open\\command"),0,KEY_READ,&hkey) == ERROR_SUCCESS)
    {  length = 300;
       if (RegQueryValueEx(hkey,(TCHAR*)0,0,NULL,(LPBYTE)defaultbrowser,&length) == ERROR_SUCCESS)
         {  if (strstr(defaultbrowser,"%1") == 0)
              strcat(defaultbrowser," \"%s\"");
            else
              strstr(defaultbrowser,"%1")[1] = 's';

            sprintf(path,defaultbrowser,url);
            WinExec(path,SW_NORMAL);
         }
       RegCloseKey(hkey);
    }
#endif
}

void RunVideo(int commandid)
{FILE *infile;
 RCCHAR filename[300],directory[300],line[300];
 RCCHAR command[300];

  //  For now, just go to the web page
  if (GetModuleHandle(_RCT("Landworks.DLL")) != 0)
    OpenBrowser((char*)"http://www.landworkscad.com/videos.htm");
  else
    OpenBrowser((char*)"http://www.realcad.com/videos.htm");

  return;

  strcpy(filename,home.getexecutablefilename(_RCT("VideoFiles.LST")));
  strcpy(directory,home.getexecutablefilename(_RCT("")));

  if ((infile = fopen(filename,"r")) != 0)
    {  while (fgets(line,300,infile) != 0)
         {  if (strchr(line,'\n') != 0)
              strchr(line,'\n')[0] = 0;
            if (strchr(line,'\r') != 0)
              strchr(line,'\r')[0] = 0;
            if (commandid == atoi(line) && strchr(line,',') != 0)
              {  sprintf(command,"mplayer /play %s%s",directory,strchr(line,',')+1);
                 WinExec(command,SW_SHOW);
              }
         }
    }

}

void file_open(int nocadfiles,RCCHAR *fname)
{int cstate;
 RCCHAR filename[600]={0},title[600]={0};
  if (fname[0] == '"')
    strcpy(filename,fname+1);
  else
    strcpy(filename,fname);
  GetFileTitle(filename,title,300);
  if (strlen(filename) > 0 && filename[strlen(filename)-1] == '"')
    filename[strlen(filename)-1] = 0;
  if (! nocadfiles && _stricmp(filename+(strlen(filename)-4),".cad") == 0)
    {CadFile infile;
       if (infile.open(filename,"rb"))
         {
            program->setbusy(1);
            db.clear();
            db.settitle(title);
            cadwindow->updatetitle();
            db.setname(filename);
            if (db.load(&infile))
              program->fileopenapps();
            else 
              db.initialize();
            infile.close();
            cadwindow->openlist.openeddatabase(db.getname(),db.gettitle());
            // clear all pending events
            app->processEvents();
            cadwindow->paintall();
            program->setbusy(0);
         }
    }
#if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD) && ! defined(_WIN32_WCE)
  else if (strlen(filename) > 3 && _stricmp(filename+(strlen(filename)-3),".hp") == 0 ||
           strlen(filename) > 4 && _stricmp(filename+(strlen(filename)-4),".hpg") == 0 ||
           strlen(filename) > 5 && _stricmp(filename+(strlen(filename)-5),".hpgl") == 0)
    {HPGLIn hpglin;
       if (hpglin.open(filename))
         {  hpglin.read();
            db.grid.setsnap(0);
            db.grid.setdisplay(0);
            if (cadwindow->getcurrentwindow() != 0)
              ShowWindow(cadwindow->getcurrentwindow()->gethwnd(),SW_MAXIMIZE);
            zoom_command6(&cstate,0,0);
         }
    }
  else if (_stricmp(filename+(strlen(filename)-4),".igs") == 0)
    {IGESIn igesin;
       if (igesin.open(filename))
         {  igesin.read(0);
            db.grid.setsnap(0);
            db.grid.setdisplay(0);
            if (cadwindow->getcurrentwindow() != 0)
              ShowWindow(cadwindow->getcurrentwindow()->gethwnd(),SW_MAXIMIZE);
            zoom_command6(&cstate,0,0);
         }
    }
#endif

}

void file_open_event(int nocadfiles,RCCHAR *filename)
{
    //QMessageBox::warning(0,"","file_open_event");

    if( ! app->eventLoopStarted || ! ((QtMainWindow*)cadwindow->gethwnd())->isVisible())
    {
        //QMessageBox::warning(0,"","! app->eventLoopStarted");
        QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
        settings.setValue("FileOpenEvent/filename","");
        settings.setValue("FileOpenEvent/filename",QString(filename));
        settings.sync();
    }
    else
    {
         //QMessageBox::warning(0,"","app->eventLoopStarted");
         ResourceString rs2(NCMAIN+2),rs3(NCMAIN+3);
         if(QString(filename).toLower().right(4) == ".cad")
         {
             if (filechanged(rs2.gets(),rs3.gets()))
             {
                 db.initialize();
                 file_open(0,filename);
             }
             QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
             settings.setValue("FileOpenEvent/filename","");
             settings.sync();
         }
         // KMJ: v8 enhancement
#if MAJOR_VERSION > 7
         else if(QString(filename).toLower().right(4) == ".dwg" ||
                 QString(filename).toLower().right(4) == ".dxf" )
         {
             if (filechanged(rs2.gets(),rs3.gets()))
             {
                 db.initialize();
                 open_dwg(filename);
             }
             QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);
             settings.setValue("FileOpenEvent/filename","");
             settings.sync();
         }
#endif
    }
}

extern void EnterIdle();

LRESULT CALLBACK TransparentEventProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
{RCPAINTSTRUCT  ps;

  switch(message)
    {case WM_PAINT :
       if (GetUpdateRect(hWnd,NULL,FALSE))
         {  BeginPaint(hWnd,&ps);
            EndPaint(hWnd,&ps);
         }
       return 1;
       break;
     case WM_ERASEBKGND :
       return 1;
    }
  return DefWindowProc(hWnd, message, wParam, lParam);

}


static DWORD lasttime = 0;

void usertimetimerevent(void)
{DWORD time,length;
 RCCHAR username[300],key[300],text[300];
 double seconds;

  time = timeGetTime();

  if (lasttime != 0)
    {  length = 300;
       GetUserName(username,&length);
       strcpy(key,"in::LoginTime:");
       strcat(key,username);
       seconds = atof(v.getstring(key)) + (time - lasttime) / 1000.0;
       sprintf(text,"%.3lf",seconds);
       v.setstring(key,text);
    }

  lasttime = time;

}


LRESULT CALLBACK MainEventProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
{Window      *window;
 RCPAINTSTRUCT  ps;
 Point        p;
 POINT cp;
 AbortEvent *aevent;
 int i,commandid,flags,item,accel,overcontrol;
 MenuWindow *mw;
 Control *c;
 HMENU hmenu;
#if ! defined(_WIN32_WCE)
 WINDOWPLACEMENT wp;
#endif
 RCCHAR swp[300],filename[300];
 extern void customtimerevent(void);

  if (db.MessageCB != 0)
    if (db.MessageCB(hWnd,message,wParam,lParam) != 0)
      return 0; 

  switch(message)
    {  case 0x020A : //  WM_MOUSEWHEEL
         //cp.x = (signed short)LOWORD(lParam);
         //cp.y = (signed short)HIWORD(lParam);
         GetCursorPos(&cp);
         //qDebug() << "MainEventProc : " << cp.x;
         //qDebug() << "MainEventProc : " << cp.y;
         window = wm.match(WindowFromPoint(cp));
         if (window != 0)
           window->mousewheelevent((short)HIWORD(wParam),(signed short)LOWORD(lParam),(signed short)HIWORD(lParam));
           //window->mousewheelevent((short)HIWORD(wParam));
         else if (hWnd == cadwindow->gethwnd() && cadwindow->getcurrentwindow() != 0)
           cadwindow->getcurrentwindow()->mousewheelevent((short)HIWORD(wParam));
         break;
       case WM_LBUTTONDOWN :
       //qDebug() << "MEP WM_LBUTTONDOWN";
         if(state.getcommandid() == 540 && program->m_IsRaytracing
            /* app->activeModalWidget()->objectName() != "Raytrace_Dialog"*/) // raytracing
             return TRUE;
         db.set_last_message(hWnd,message,wParam,lParam);
         program->ShowLeftClick();
         if(!cadwindow->coordinateentry->containsWidget((QWidget*)hWnd))
             cadwindow->coordinateentry->setEntryFocus(_RCT("XYZ"),false);
         if ((window = wm.match(hWnd)) != NULL)
         {
           window->lbuttondownevent((signed short)LOWORD(lParam),(signed short)HIWORD(lParam),wParam);
           if(cadwindow != 0 && cadwindow->getcurrentwindow()->gethwnd() != 0)
               ((RCView*)cadwindow->getcurrentwindow()->gethwnd())->updateLabel();
           return TRUE;
         }
         else
         {
             // KMJ: this line kicks the qt event loop
             if(cadwindow != 0 && cadwindow->getcurrentwindow()->gethwnd() != 0)
                 ((RCView*)cadwindow->getcurrentwindow()->gethwnd())->updateLabel();
             return DefWindowProc(hWnd, message, wParam, lParam);
         }
         break;
       case WM_LBUTTONUP :
      //qDebug() << "MEP WM_LBUTTONUP";
          if(state.getcommandid() == 540 && program->m_IsRaytracing
             /*app->activeModalWidget()->objectName() != "Raytrace_Dialog"*/) // raytracing
              return TRUE;
         db.set_last_message(hWnd,message,wParam,lParam);
         if(!cadwindow->coordinateentry->containsWidget((QWidget*)hWnd))
             cadwindow->coordinateentry->setEntryFocus(_RCT("XYZ"),false);
         if ((window = wm.match(hWnd)) != NULL)
         {
            window->lbuttonupevent((signed short)LOWORD(lParam),(signed short)HIWORD(lParam),wParam);
            // KMJ: this line kicks the qt event loop
            if(cadwindow != 0 && cadwindow->getcurrentwindow()->gethwnd() != 0)
                ((RCView*)cadwindow->getcurrentwindow()->gethwnd())->updateLabel();
            return TRUE;
         }
         else
         {
             // KMJ: this line kicks the qt event loop
            if(cadwindow != 0 && cadwindow->getcurrentwindow() !=0 )
                ((RCView*)cadwindow->getcurrentwindow()->gethwnd())->updateLabel();
           return DefWindowProc(hWnd, message, wParam, lParam);
         }
         break;
       case WM_LBUTTONDBLCLK :
      if(state.getcommandid() == 540) // raytracing
        break;
         db.set_last_message(hWnd,message,wParam,lParam);
         if ((window = wm.match(hWnd)) != NULL)
           window->lbuttondblclkevent((signed short)LOWORD(lParam),(signed short)HIWORD(lParam),wParam);
         else
           return DefWindowProc(hWnd, message, wParam, lParam);
         break;
       case WM_RBUTTONDBLCLK :
      if(state.getcommandid() == 540) // raytracing
        break;
         db.set_last_message(hWnd,message,wParam,lParam);
         if ((window = wm.match(hWnd)) != NULL)
           window->rbuttondblclkevent((signed short)LOWORD(lParam),(signed short)HIWORD(lParam),wParam);
         else
           return DefWindowProc(hWnd, message, wParam, lParam);
         break;
       case WM_MBUTTONDBLCLK :
      if(state.getcommandid() == 540) // raytracing
        break;
         db.set_last_message(hWnd,message,wParam,lParam);
         if ((window = wm.match(hWnd)) != NULL)
           window->mbuttondblclkevent((signed short)LOWORD(lParam),(signed short)HIWORD(lParam),wParam);
         else
           return DefWindowProc(hWnd, message, wParam, lParam);
         break;
       case WM_MBUTTONDOWN :
         if(state.getcommandid() == 540) // raytracing
            break;
         db.set_last_message(hWnd,message,wParam,lParam);
         if(!cadwindow->coordinateentry->containsWidget((QWidget*)hWnd))
             cadwindow->coordinateentry->setEntryFocus(_RCT("XYZ"),false);
         if ((window = wm.match(hWnd)) != NULL)
           window->mbuttondownevent((signed short)LOWORD(lParam),(signed short)HIWORD(lParam),wParam);
         else
           return DefWindowProc(hWnd, message, wParam, lParam);
         break;
       case WM_MBUTTONUP :
          if(state.getcommandid() == 540) // raytracing
            break;
         if(!cadwindow->coordinateentry->containsWidget((QWidget*)hWnd))
            cadwindow->coordinateentry->setEntryFocus(_RCT("XYZ"),false);
         db.set_last_message(hWnd,message,wParam,lParam);
         if ((window = wm.match(hWnd)) != NULL)
           window->mbuttonupevent((signed short)LOWORD(lParam),(signed short)HIWORD(lParam),wParam);
         else
           return DefWindowProc(hWnd, message, wParam, lParam);
         break;
       case WM_RBUTTONDOWN :
          if(state.getcommandid() == 540) // raytracing
            break;
         //qDebug() << "right click";
         db.set_last_message(hWnd,message,wParam,lParam);
         program->ShowRightClick();
         if(!cadwindow->coordinateentry->containsWidget((QWidget*)hWnd))
             cadwindow->coordinateentry->setEntryFocus(_RCT("XYZ"),false);

         overcontrol = 0;
         //overcontrol = 1;

         /*
         for (i = 0 ; (c = cadwindow->getcontrol(i)) != 0 ; i++)
           {
              GetCursorPos(&cp);
              ScreenToClient(cadwindow->gethwnd(),&cp);
              if (c->pick(cp.x,cp.y))
              {
                  //qDebug() << "control (" << i << ") was over ";
                  //qDebug() << "control : " << (QObject*)c->hDC;
                  overcontrol = 1;
              }
           }
          */

         /*
         for (cadwindow->floatingmenulist.start() ; (mw = (MenuWindow *)cadwindow->floatingmenulist.next()) != 0 ; )
           for (i = 0 ; (c = mw->getcontrol(i)) != 0 ; i++)
             {
               //qDebug() << "checking floatingmenulist " << i;
               GetCursorPos(&cp);
                ScreenToClient(mw->gethwnd(),&cp); 
                if (c->pick(cp.x,cp.y))
                  overcontrol = 1;
             }
           */


         //qDebug() << "overcontrol " << overcontrol;
         if (overcontrol)
           {
             if ((window = wm.match(hWnd)) != NULL)
                {
                   //qDebug() << "window matched hWnd " << " window " << window << " hWnd " << hWnd;
                   //qDebug() << "calling window lbuttondownevent on window " << window;
                   window->lbuttondownevent((signed)LOWORD(lParam),(signed)HIWORD(lParam),MK_LBUTTON);
                   window->lbuttondownevent((signed)LOWORD(lParam),(signed)HIWORD(lParam),0);
             }
           }
         else if ((wParam & MK_SHIFT) == MK_SHIFT)
           {
             //qDebug() << "shift key is presed";
             RCHMENU hmenu;
              if ((hmenu = (RCHMENU*)((QtMainWindow*)cadwindow->gethwnd())->Popup_Menu) != NULL)
              //if ((hmenu = LoadMenu(program->gethinst(), _RCT("Popup_Menu"))) != NULL)
              {
                  POINT p;
                   GetCursorPos(&p);
                   //TrackPopupMenu(GetSubMenu(hmenu,0),TPM_LEFTALIGN | TPM_LEFTBUTTON,
                   //    (signed short)p.x,(signed short)p.y, 0, cadwindow->gethwnd(), NULL);
                   //DestroyMenu(hmenu);
                   TrackPopupMenu(hmenu,0,p.x,p.y, 0, cadwindow->gethwnd(), NULL);
              }
           }
         else
           {
             //qDebug() << "state.getcommandactive()" << state.getcommandactive();
             if (state.getcommandactive())
             {
                   aevent = new AbortEvent;
                   state.sendevent(aevent);
                   // this line helps kick the Qt events ??don't know why
                   ((RCView*)cadwindow->getcurrentwindow()->gethwnd())->updateLabel();
                   //qDebug() << "aborting command";
                   return TRUE;
             }
             else
             {
                  RCHMENU hmenu;
                  POINT p;
                  int cancelselection;
                   cancelselection = state.getselection().length() > 0;
                   p.x = (signed)LOWORD(lParam);
                   p.y = (signed)HIWORD(lParam);
                   ClientToScreen(hWnd,&p);
                   aevent = new AbortEvent;
                   state.sendevent(aevent);
                   if ((window = wm.match(hWnd)) != NULL)
                     {
                        window->lbuttondownevent((signed)LOWORD(lParam),(signed)HIWORD(lParam),wParam);
                        window->lbuttonupevent((signed short)LOWORD(lParam),(signed short)HIWORD(lParam),wParam);
                        if (state.getselection().length() == 1 && (hmenu = (RCHMENU*)((QtMainWindow*)cadwindow->gethwnd())->Popup1_Menu) != NULL)
                        //if (state.getselection().length() == 1 && (hmenu = LoadMenu(program->gethinst(), _RCT("Popup1_Menu"))) != NULL)
                        {
                            if (db.RightClickCallback != 0)
                               db.RightClickCallback((Entity *)hmenu); 
                             //commandid = TrackPopupMenu(GetSubMenu(hmenu,0),TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,p.x,p.y, 0, cadwindow->gethwnd(), NULL);
                             //DestroyMenu(hmenu);
                             commandid = TrackPopupMenu(hmenu,TPM_RETURNCMD,p.x,p.y, 0, cadwindow->gethwnd(), NULL);
                             if (commandid == 60000)
                               window->lbuttondblclkevent((signed)LOWORD(lParam),(signed)HIWORD(lParam),wParam);
                             else
                               state.sendevent(new NewCommandEvent(commandid,0,1));
                        }
                        else if (! cancelselection && GetPrivateProfileInt("LastCommand","EnableRightButton",0,home.getinifilename()) != 0)
                          {int commandid;
                             if ((commandid = state.getlastcommandid()) >= 0)
                               {  NewCommandEvent *nevent = new NewCommandEvent(commandid,0,1);
                                   state.sendevent(nevent);
                               }
                          }
                     }
             }
         }

         break;
       case WM_MOUSEMOVE :
      if(state.getcommandid() == 540) // raytracing
        break;
       //qDebug() << "MEP WM_MOUSEMOVE";
         if ((window = wm.match(hWnd)) != NULL && IsWindow(hWnd))
         {
             //qDebug() << "MEP window :" << window;
             //qDebug() << "MEP hWnd :" << hWnd;
           window->mousemoveevent((signed short)LOWORD(lParam),(signed short)HIWORD(lParam),wParam);
           return FALSE; // Qt requires TRUE if we handles this??
         }
         else
         {
             // KMJ: this line kicks the qt event loop
             if(cadwindow != 0 && cadwindow->getcurrentwindow()->gethwnd() != 0)
                 ((RCView*)cadwindow->getcurrentwindow()->gethwnd())->updateLabel();
             return DefWindowProc(hWnd, message, wParam, lParam);
         }
         break;
#if ! defined(_WIN32_WCE)
       case WM_NCLBUTTONDOWN :
         program->ShowLeftClick();
         if (SendMessage(hWnd,WM_NCHITTEST,0,lParam) == HTCAPTION && (GetWindowLong(hWnd,GWL_EXSTYLE) & WS_EX_TOOLWINDOW) == WS_EX_TOOLWINDOW && (window = wm.match(hWnd)) != 0)
           {Control *c;
            int i;
              if ((c = window->getcontrol(0)) != 0)
                {  for (i = 1 ; c->y != 0 && window->getcontrol(i) != 0 ; i++)
                     c = window->getcontrol(i);
                   cadwindow->addcontrol(c,0);
                   window->addcontrol(c);
                   c->dragging = 2;
                   SetCapture(cadwindow->gethwnd());
                }
              return TRUE;
           }
         else if ((window = wm.match(hWnd)) != NULL)
           cadwindow->selectwindow(window);
         return DefWindowProc(hWnd, message, wParam, lParam);
       case WM_NCMOUSEMOVE :
         if ((window = wm.match(hWnd)) != NULL)
            window->mousemoveevent(-32767,-32767,0);
         return DefWindowProc(hWnd, message, wParam, lParam);
       case WM_NCLBUTTONDBLCLK :
         if ((window = wm.match(hWnd)) != NULL)
            window->nclbuttondblclk(0,0,0);
         return DefWindowProc(hWnd, message, wParam, lParam);
#endif
       case WM_VSCROLL:
         if ((window = wm.match(hWnd)) != 0)
#if defined(WIN32) || defined(_MAC)
           window->vscrollevent(LOWORD(wParam),HIWORD(wParam));
#else
           window->vscrollevent(wParam,LOWORD(lParam));
#endif
         else
           return DefWindowProc(hWnd,message,wParam,lParam);
         break;
       case WM_HSCROLL:
         if ((window = wm.match(hWnd)) != 0)
#if defined(WIN32) || defined(_MAC)
           window->hscrollevent(LOWORD(wParam),HIWORD(wParam));
#else
           window->hscrollevent(wParam,LOWORD(lParam));
#endif
         else
           return DefWindowProc(hWnd,message,wParam,lParam);
         break;
       case WM_KEYDOWN :
         if (cadwindow->getcadbasicwindow() != 0 && cadwindow->getcadbasicwindow()->gethwnd() == hWnd)
           cadwindow->getcadbasicwindow()->charevent(0,(DWORD)wParam);
         else if (cadwindow->getcommandwindow() != 0 && cadwindow->getcommandwindow()->gethwnd() == hWnd)
           cadwindow->getcommandwindow()->charevent(0,(DWORD)wParam);
         else if (wParam >= VK_F5 && wParam <= VK_F9 || wParam == VK_F11)
         {
              POINT p;
              UserEvent *uevent;
              if (wParam <= VK_F9)
                uevent = new UserEvent(1+(wParam-VK_F5));
              else
                uevent = new UserEvent(6);
              state.sendevent(uevent);
              GetCursorPos(&p);
              SetCursorPos(p.x,p.y);
         }
         else
             return DefWindowProc(hWnd,message,wParam,lParam);
         break;
       case WM_CHAR :
         if (wParam == VK_ESCAPE)
         {
             // ignore the escape if there is a dialog up
             // Qt should remove it
             if(!QApplication::activeModalWidget())
             {
                 AbortEvent *aevent = new AbortEvent;
                 //ExitEvent *aevent = new ExitEvent;
                 state.sendevent(aevent);
                 state.process();
             }
             else
             {
                 AbortEvent *aevent = new AbortEvent;
                 //ExitEvent *aevent = new ExitEvent;
                 state.sendevent(aevent);
                 state.process();
             }
         }
#ifdef USING_WIDGETS
         else if ((window = wm.match(hWnd)) != NULL && !cadwindow->styleentry->containsWidget((QWidget*)GetFocus()))
         {
             // send every char event to the cadwindow just incase??
             if(window != cadwindow)
                window->charevent((UINT)wParam,0);
             cadwindow->charevent((UINT)wParam,0);
         }
#else
         else if ((window = wm.match(hWnd)) != NULL && GetFocus() != cadwindow->styleentry->gettypehwnd())
           window->charevent((UINT)wParam,0);
#endif
         else
           return DefWindowProc(hWnd, message, wParam, lParam);
         break;
       case WM_COMMAND :
         commandid = LOWORD(wParam);
         accel = HIWORD(wParam);

#if VERSION != TRICAD_STUDENT
         if ((accel == 0 || cadwindow != 0 && cadwindow->getcadbasicwindow() == 0) && (window = wm.match(hWnd)) != NULL && commands.command(commandid) != NULL)
#else
         if ((window = wm.match(hWnd)) != NULL && commands.command(commandid) != NULL)
#endif
           {  NewCommandEvent *nevent = new NewCommandEvent(commandid,0);
              state.sendevent(nevent);
           }

         if ((HIWORD(wParam) == EN_SETFOCUS || HIWORD(wParam) == EN_KILLFOCUS) && cadwindow != 0)
           cadwindow->styleentry->updatedefaults();

         if (cadwindow->viewproperties && HIWORD(wParam) == CBN_CLOSEUP && LOWORD(wParam) == 0 && GetParent(GetFocus()) == cadwindow->viewproperties->get_control())
         {  if ((i = SendMessage(cadwindow->viewproperties->get_control(),CB_GETCURSEL,0,0)) != CB_ERR)
            {
#ifdef _USING_QTCHAR
				RCCHAR name[300];
                SendMessage(cadwindow->viewproperties->get_control(),CB_GETLBTEXT,i,(LPARAM)name);
                SetWindowTextW(cadwindow->viewproperties->get_control(), name);
#else
				char name[300];
                SendMessage(cadwindow->viewproperties->get_control(),CB_GETLBTEXT,i,(LPARAM)name);
                SetWindowText(cadwindow->viewproperties->get_control(),name);
#endif
            }
            cadwindow->viewproperties->mousemoveevent(-1,-1,0);
            cadwindow->viewproperties->mousemoveevent(-1,-1,MK_LBUTTON);
            cadwindow->viewproperties->mousemoveevent(-1,-1,0);
         }
         else if (cadwindow->viewproperties && HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == 0 && GetParent(GetFocus()) != cadwindow->viewproperties->get_control())
           {  if ((i = SendMessage(cadwindow->styleentry->gettypehwnd(),CB_GETCURSEL,0,0)) != CB_ERR)
                {char key[300],value[300],name[300],*s;
                 int colour,style,layer,c,st,l;
                 double weight,w;
                 EntityList list;
                 Entity *e;
                 BitMask change(32); 
 
                   SendMessage(cadwindow->styleentry->gettypehwnd(),CB_GETLBTEXT,i,(LPARAM)name);
 
                   c = colour = v.getinteger("df::colour");
                   l = layer = v.getinteger("df::layer");
                   st = style = v.getinteger("df::style");

                   w = weight = db.lineweights.weighttomm(v.getinteger("df::weight"));

                   for (i = 1 ; ; i++)
                     {  sprintf(key,"StyleSet%d",i);
                        GetPrivateProfileString("StyleSet",key,"",value,300,home.getinifilename());
                        if (strlen(value) == 0)
                          break;
                        s = strtok(value,",");
                        if (_stricmp(s,name) == 0)
                          {  s = strtok(0,",");
                             if (s != 0)
                               c = atoi(s);
                             s = strtok(0,",");
                             if (s != 0)
                               l = atoi(s);
                             s = strtok(0,",");
                             if (s != 0)
                               st = atoi(s);
                             s = strtok(0,",");
                             if (s != 0)
                               w = atof(s);
                          }
                     }

                   if (c != colour || layer != l || st != style || fabs(w - weight) > 0.01)   
                     {  db.header.change(c,l,st,db.lineweights.mmtoweight(w));
                        v.setinteger("df::colour",c);
                        v.setinteger("df::layer",l);
                        v.setinteger("df::style",st);
                        v.setinteger("df::weight",db.lineweights.mmtoweight(w));
                        cadwindow->updatetitle();
                     }

                   list = state.getselection();
                   change.set(0);
                   change.set(1);
                   change.set(2);
                   change.set(3);

                   if (! list.empty())
                     {  db.saveundo(UD_STARTBLOCK,NULL);
                        for (list.start() ; (e = list.next()) != NULL ; )
                          {  e->change(change,c,l,st,db.lineweights.mmtoweight(w));
                             if (e->isa(figure_entity))
                               {  cadwindow->invalidatedisplaylist(e);
                                  e->draw(DM_NORMAL);
                               }
                         
                          }
                        db.saveundo(UD_ENDBLOCK,NULL);
                     }


                }
           }
          break;
       case WM_TIMER :
         rubberband.timerevent();
         state.timerevent();
         //if (program != 0)
         //  program->checkautosavetimer();
#if (VERSION != TRICAD_STUDENT) && (VERSION != FLEXICAD)
         if (activevideowindow != 0)
           activevideowindow->timerevent();
         else if (wm.getlastwindow() != 0)
           wm.getlastwindow()->timerevent();
         customtimerevent();

         // not required for now??
         //usertimetimerevent();

#endif
         if (cadwindow != 0)
           cadwindow->timerevent();
         break;
       case WM_PAINT :
         if ((window = wm.match(hWnd)) != NULL)
           {  cadwindow->cursoroff();
              if (GetUpdateRect(hWnd,NULL,FALSE))
                {  BeginPaint(hWnd,&ps);
                   window->paintevent();
                   EndPaint(hWnd,&ps);
                }
              else
                window->paintevent();
              cadwindow->cursoron();
           }
         else
           return DefWindowProc(hWnd, message, wParam, lParam);
         break;
       case WM_SIZE :
         if ((window = wm.match(hWnd)) != NULL)
           {  window->sizeevent(LOWORD(lParam),HIWORD(lParam));
#if ! defined(_WIN32_WCE)
              if (window == cadwindow)
                {  wp.length = sizeof(WINDOWPLACEMENT);
                   GetWindowPlacement(hWnd,&wp);
                   sprintf(swp,"%d,%d,%d,%d,%d",
                      wp.rcNormalPosition.left,wp.rcNormalPosition.top,
                      wp.rcNormalPosition.right - wp.rcNormalPosition.left,
                      wp.rcNormalPosition.bottom - wp.rcNormalPosition.top,
                      wParam == SIZE_MAXIMIZED ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
                   WritePrivateProfileString(_RCT("WindowPlacement"),_RCT("Main"),swp,home.getinifilename());
                }
#endif
           }
         else
           return DefWindowProc(hWnd, message, wParam, lParam);
         break;
       case WM_MOVE :
         if ((window = wm.match(hWnd)) != 0 && window == cadwindow)
#if ! defined(_WIN32_WCE)
           {  wp.length = sizeof(WINDOWPLACEMENT);
              GetWindowPlacement(hWnd,&wp);
              sprintf(swp,"%d,%d,%d,%d,%d",
                 wp.rcNormalPosition.left,wp.rcNormalPosition.top,
                 wp.rcNormalPosition.right - wp.rcNormalPosition.left,
                 wp.rcNormalPosition.bottom - wp.rcNormalPosition.top,
                 wParam == SIZE_MAXIMIZED ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
              WritePrivateProfileString(_RCT("WindowPlacement"),_RCT("Main"),swp,home.getinifilename());
           }
#endif
         return DefWindowProc(hWnd, message, wParam, lParam);
#if ! defined(_WIN32_WCE)
       case WM_GETMINMAXINFO :
         if ((window = wm.match(hWnd)) != NULL)
           window->getminmaxinfoevent((MINMAXINFO *)lParam);
         else
           return DefWindowProc(hWnd, message, wParam, lParam);
         break;
       case WM_MENUSELECT:
#if defined(WIN32)
         item = LOWORD(wParam);
         flags = HIWORD(wParam);
         hmenu = (HMENU) lParam;
#else
         item = wParam;
         flags = LOWORD(lParam);
         hmenu = (HMENU) HIWORD(lParam);
#endif
         if (hmenu == 0)
           program->setcommandid(0);
         else if ((flags & MF_POPUP) == MF_POPUP)
           program->setcommandid(0);
         else
           program->setcommandid(item);
         break;
       case WM_F1DOWN:
         if (program->getcommandid() != 0)
           {  if ((cad_version&15) == -1)
               RCMessageBox(cadwindow->gethwnd(),"Help not available in demonstration version.","AUSCAD",MB_ICONINFORMATION);
              else
                program->help(HELP_CONTEXT,(DWORD)program->getcommandid());

              DrawMenuBar(hWnd);
           }
         break;
       case WM_QUERYENDSESSION :
         if (hWnd == cadwindow->gethwnd())
           {ResourceString rs6(NCMAIN+6);
            ResourceString rs10(NCMAIN+10);
              if (filechanged(rs6.gets(),rs10.gets()))
                return 1L;
           }
         else
           return DefWindowProc(hWnd, message, wParam, lParam);
         break;
#endif
       case WM_INITMENUPOPUP :
         mainmenu.pasteredoundo((HWND)wParam);
         break;
       case WM_CLOSE :
         if (hWnd == cadwindow->gethwnd())
           {ResourceString rs6(NCMAIN+6);
            ResourceString rs10(NCMAIN+10);
              if (filechanged(rs6.gets(),rs10.gets()))
                PostQuitMessage(0);
           }
         else
           {  if (cadwindow->matchview3dwindow(hWnd) != 0 && cadwindow->GetView3dList()->length() <= 1)
                {
                   cadwindow->MessageBox("This window cannot be closed.  There must be at least one view window open","Close Window",MB_OK);
                   return 0;
                }
              else if (cadwindow->getcadbasicwindow() == 0 || cadwindow->getcadbasicwindow()->gethwnd() != hWnd || ! cadwindow->getcadbasicwindow()->getrunning())
                return 1;//DefWindowProc(hWnd, message, wParam, lParam);
           }
         break;
#if ! defined(_WIN32_WCE)
       case WM_DROPFILES :
         if (DragQueryFile((HDROP)wParam,0,filename,300) > 0)
           {
             ResourceString rs2(NCMAIN+2),rs3(NCMAIN+3);
              _strlwr(filename);
              if (strcmp(filename+(strlen(filename)-4),".cad") == 0)
                {  if (! filechanged(rs2.gets(),rs3.gets()))
                     break;
                   db.initialize();
                }
              file_open(0,filename);
           }
         DragFinish((HDROP)wParam);
         break;
#endif
       case WM_DESTROY :
         if ((window = wm.match(hWnd)) != NULL && window != cadwindow)
           delete window;
         break;
       case WM_ENTERIDLE :
         EnterIdle(); 
         break;
       case WM_SETFOCUS :
         if (cadwindow != 0)
           cadwindow->styleentry->updatedefaults();
         return DefWindowProc(hWnd, message, wParam, lParam);
       case WM_WINDOWPOSCHANGING : 
         if (((LPWINDOWPOS) lParam)->hwndInsertAfter == HWND_BOTTOM)
           {  if (cadwindow != 0 && (window = wm.match(hWnd)) != NULL && window != cadwindow)
                cadwindow->movetoback(window);
           }
         return DefWindowProc(hWnd, message, wParam, lParam);
       case WM_SYSCOMMAND : 
         if (IsZoomed(hWnd) && ((wParam& 0xfff0) == SC_MOVE || (wParam& 0xfff0) == SC_SIZE))
            return NULL;
         else
            return DefWindowProc(hWnd, message, wParam, lParam);
       case WM_INITMENU : 
         if (db.getchanged())
            EnableMenuItem((HMENU)wParam,102,MF_BYCOMMAND | MF_ENABLED);
         else
            EnableMenuItem((HMENU)wParam,102,MF_BYCOMMAND | MF_DISABLED);
         break;
       default :
         return DefWindowProc(hWnd, message, wParam, lParam);
    }
  return NULL;
}

int InitApplication(HANDLE hInstance)
{ 
#ifdef _USING_QT
	return 1;
#else
  WNDCLASS  wc;
  wc.style = CS_OWNDC | CS_DBLCLKS | CS_HREDRAW	| CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = MainEventProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = (RCHINSTANCE) hInstance;
  wc.hIcon = (HICON)LoadImage((RCHINSTANCE)hInstance,MAKEINTRESOURCE(1002),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
  wc.hCursor = 0;
  wc.hbrBackground = NULL;//GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName =  NULL;
  wc.lpszClassName = "CadWindow";
  RegisterClass(&wc);

  wc.style = CS_OWNDC;
  wc.lpfnWndProc = TransparentEventProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = (RCHINSTANCE) hInstance;
  wc.hIcon = 0;
  wc.hCursor = 0;
  wc.hbrBackground = NULL;
  wc.lpszMenuName =  NULL;
  wc.lpszClassName = "CadSubWindow";

  return RegisterClass(&wc);
#endif

}

void RBxyz(int,void *,Point *p,View3dSurface *)
{
    if(cadwindow->coordinatedisplay)
        cadwindow->coordinatedisplay->position(*p);
    //qDebug() << "RCxyz";
}

Entity *load01(int i,CadFile *c) { return PointE::load(i,c); }
Entity *load02(int i,CadFile *c) { return Line::load(i,c); }
Entity *load03(int i,CadFile *c) { return Circle::load(i,c); }
Entity *load04(int i,CadFile *c) { return InfinitePlane::load(i,c); }
Entity *load05(int i,CadFile *c) { return Plane::load(i,c); }
Entity *load06(int i,CadFile *c) { return Text::load(i,c); }
Entity *load07(int i,CadFile *c) { return RuledSurface::load(i,c); }
Entity *load08(int i,CadFile *c) { return SurfaceOfRev::load(i,c); }
Entity *load09(int i,CadFile *c) { return Figure::load(i,c); }
Entity *load10(int i,CadFile *c) { return LinearDimension::load(i,c); }
Entity *load11(int i,CadFile *c) { return AngularDimension::load(i,c); }
Entity *load12(int i,CadFile *c) { return Curve::load(i,c); }
Entity *load13(int i,CadFile *c) { return EllipseE::load(i,c); }
Entity *load14(int i,CadFile *c) { return Group::load(i,c); }
Entity *load15(int i,CadFile *c) { return DistanceBearingDimension::load(i,c); }
Entity *load16(int i,CadFile *c) { return CircularDimension::load(i,c); }
Entity *load17(int i,CadFile *c) { return Light::load(i,c); }
Entity *load18(int i,CadFile *c) { return Patch::load(i,c); }
#if VERSION != FLEXICAD
Entity *load19(int i,CadFile *c) { return Wall::load(i,c); }
#endif
Entity *load20(int i,CadFile *c) { return BitMapE::load(i,c); }
Entity *load20a(int i,CadFile *c) { return NCSolid::load(i,c); }
Entity *load20b(int i,CadFile *c) { return Beam::load(i,c); }

Entity *load21(UndoFile *u) { return PointE::load(u); }
Entity *load22(UndoFile *u) { return Line::load(u); }
Entity *load23(UndoFile *u) { return Circle::load(u); }
Entity *load24(UndoFile *u) { return InfinitePlane::load(u); }
Entity *load25(UndoFile *u) { return Plane::load(u); }
Entity *load26(UndoFile *u) { return Text::load(u); }
Entity *load27(UndoFile *u) { return RuledSurface::load(u); }
Entity *load28(UndoFile *u) { return SurfaceOfRev::load(u); }
Entity *load29(UndoFile *u) { return Figure::load(u); }
Entity *load30(UndoFile *u) { return LinearDimension::load(u); }
Entity *load31(UndoFile *u) { return AngularDimension::load(u); }
Entity *load32(UndoFile *u) { return Curve::load(u); }
Entity *load33(UndoFile *u) { return EllipseE::load(u); }
Entity *load34(UndoFile *u) { return Group::load(u); }
Entity *load35(UndoFile *u) { return DistanceBearingDimension::load(u); }
Entity *load36(UndoFile *u) { return CircularDimension::load(u); }
Entity *load37(UndoFile *u) { return Light::load(u); }
Entity *load38(UndoFile *u) { return Patch::load(u); }
#if VERSION != FLEXICAD
Entity *load39(UndoFile *u) { return Wall::load(u); }
#endif
Entity *load40(UndoFile *u) { return BitMapE::load(u); }
Entity *load40a(UndoFile *u) { return Beam::load(u); }

int load41(CadFile *c,Database *d) { return Views3d::load(c,d); }
int load42(CadFile *c,Database *d) { return Workplanes::load(c,d); }
int load43(CadFile *c,Database *d) { return Grid::load(c,d); }
int load44(CadFile *c,Database *d) { return v.load(c,1); }
int load45(CadFile *c,Database *d) { return MainWindow::load(c,d); }
int load45a(CadFile *c,Database *d) { return MainWindow::loadsplitter(c,d); }
int load46(CadFile *c,Database *d) { return d->loadtables(c); }
int load47(CadFile *c,Database *d) { return Views2d::load(c,d); }
int load48(CadFile *c,Database *d) { return d->layernames.load(c); }
int load49(CadFile *c,Database *d) { return d->loadmasktables(c); }

class MenuMenuWindow : public MenuWindow
  {private:
     ButtonMenu *buttonmenu;
     MenuActive *menuactive;
   public:
     MenuMenuWindow(Window *,int,int,int,int,MenuActive *);
     ~MenuMenuWindow();
     void timerevent(void) {  if (buttonmenu != 0) buttonmenu->timerevent();  };
  };

MenuMenuWindow::MenuMenuWindow(Window *win,int x,int y,int w,int h,MenuActive *aa) :
  MenuWindow(win,x,y,w,h)
{ menuactive = aa;
  buttonmenu = menuactive->getbuttonmenu();
  addcontrol(buttonmenu);
  buttonmenu->move(this,0,0,2);
  menuactive->setactive(1);
}

MenuMenuWindow::~MenuMenuWindow()
{ menuactive->setactive(0);
  //if (cadwindow->getsubwindow() != 0)
  //  new MenuMenuWindow(cadwindow->getsubwindow(),
  //                     int(cadwindow->getsubwindow()->getwidth())-(48+GetSystemMetrics(SM_CXDLGFRAME)*2),0,
  //                     48,96,menuactive);
}


extern void start_architect(void);

// KMJ : TODO : fix getting resource strings
void make_menu(void)
{
 ButtonMenu *menu;
 RCHMENU hmenu;
 ResourceString rs0(NCVIEWA);
 ResourceString rsnc3(NC+3);
 int s,xm,ym,wm,hm,mf,position;
 char text[300];

 // KMJ : the main menu is done from the ui file in mainwindow.cpp
 /*
  switch((cad_version>>4)&15)
    {case 0 : hmenu = LoadMenu(program->gethinst(),_RCT("Full_menu"));  break;
     case 1 : hmenu = LoadMenu(program->gethinst(),_RCT("NoArchitect_menu"));  break;
     case 2 : hmenu = LoadMenu(program->gethinst(),_RCT("Student_menu"));  break;
     case 3 : hmenu = LoadMenu(program->gethinst(),_RCT("FlexiCad_menu"));  break;
    }
  cadwindow->newmenu(hmenu);
 */

  // KMJ : using this as is
  classfunctions.registerentityload(point_entity,load01);
  classfunctions.registerentityload(line_entity,load02);
  classfunctions.registerentityload(circle_entity,load03);
  classfunctions.registerentityload(infiniteplane_entity,load04);
  classfunctions.registerentityload(plane_entity,load05);
  classfunctions.registerentityload(text_entity,load06);
  classfunctions.registerentityload(ruledsurface_entity,load07);
  classfunctions.registerentityload(surfaceofrev_entity,load08);
  classfunctions.registerentityload(figure_entity,load09);
  classfunctions.registerentityload(lineardimension_entity,load10);
  classfunctions.registerentityload(angulardimension_entity,load11);
  classfunctions.registerentityload(curve_entity,load12);
  classfunctions.registerentityload(ellipse_entity,load13);
  classfunctions.registerentityload(group_entity,load14);
  classfunctions.registerentityload(distancebearingdimension_entity,load15);
  classfunctions.registerentityload(circulardimension_entity,load16);
  classfunctions.registerentityload(light_entity,load17);
  classfunctions.registerentityload(patch_entity,load18);
#if VERSION != FLEXICAD
  classfunctions.registerentityload(wall_entity,load19);
#endif
  classfunctions.registerentityload(bitmap_entity,load20);
  classfunctions.registerentityload(ncsolid_entity,load20a);
  classfunctions.registerentityload(beam_entity,load20b);

  classfunctions.registerentityundo(point_entity,load21);
  classfunctions.registerentityundo(line_entity,load22);
  classfunctions.registerentityundo(circle_entity,load23);
  classfunctions.registerentityundo(infiniteplane_entity,load24);
  classfunctions.registerentityundo(plane_entity,load25);
  classfunctions.registerentityundo(text_entity,load26);
  classfunctions.registerentityundo(ruledsurface_entity,load27);
  classfunctions.registerentityundo(surfaceofrev_entity,load28);
  classfunctions.registerentityundo(figure_entity,load29);
  classfunctions.registerentityundo(lineardimension_entity,load30);
  classfunctions.registerentityundo(angulardimension_entity,load31);
  classfunctions.registerentityundo(curve_entity,load32);
  classfunctions.registerentityundo(ellipse_entity,load33);
  classfunctions.registerentityundo(group_entity,load34);
  classfunctions.registerentityundo(distancebearingdimension_entity,load35);
  classfunctions.registerentityundo(circulardimension_entity,load36);
  classfunctions.registerentityundo(light_entity,load37);
  classfunctions.registerentityundo(patch_entity,load38);
#if VERSION != FLEXICAD
  classfunctions.registerentityundo(wall_entity,load39);
#endif
  classfunctions.registerentityundo(bitmap_entity,load40);
  classfunctions.registerentityundo(beam_entity,load40a);

  classfunctions.registerotherload(view3d_record,load41);
  classfunctions.registerotherload(workplane_record,load42);
  classfunctions.registerotherload(grid_record,load43);
  classfunctions.registerotherload(values_record,load44);
  classfunctions.registerotherload(windows_record,load45);
  classfunctions.registerotherload(splitter_record,load45a);
  classfunctions.registerotherload(tables_record,load46);
  classfunctions.registerotherload(view2d_record,load47);
  classfunctions.registerotherload(layername_record,load48);
  classfunctions.registerotherload(masktables_record,load49);

  // KMJ TODO: look at this

#if defined(CADAUSTRALIAVERSION)
  //  Cad Australia menus
  // we are just adding new menu objects to the list
  // we create RCButtonBox objects to use them later on
  // the actual RCToolButton objects are created and stored in the button objects now

  // old main menu commands
  menu = new ButtonMenu(2,2,12,12,27,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Point0",1000,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Line0",1100,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arc0",1300,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Circle0",1200,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Curve0",1400,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Plane0",1500,menu->getQW())); // old surface
  //menu->add(new Button(-1,-1,"Surface0",1500,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Figure0",1700,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Trim0",1900,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Dimen0",1800,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Text0",1600,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Transform0",320,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"",0,menu->getQW())); // padding??
  buttonmenus.add("Tool",menu);


  // changed this to the new menu arrangement
  menu = new ButtonMenu(2,2,13,13,27,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Point0",1000,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Line0",1100,menu->getQW()));
  //menu->add(new Button(-1,-1,"Arc0",1300,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Circle0",1200,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Curve0",1400,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Plane0",2700,menu->getQW())); // new plane
  menu->add(new Button(-1,-1, (char*)"Surface0",3500,menu->getQW())); // new surface
  //menu->add(new Button(-1,-1,"Solid0",2800,menu->getQW()));
  //menu->add(new Button(-1,-1,"Trim0",1900,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Dimen0",1800,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Text0",16000,menu->getQW())); // new text command
  menu->add(new Button(-1,-1, (char*)"Bitmap0",2900,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Figure0",17000,menu->getQW())); // new components command
  menu->add(new Button(-1,-1, (char*)"Group0",4000,menu->getQW())); // new group command
  menu->add(new Button(-1,-1, (char*)"Transform0",320,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"",0,menu->getQW())); // padding??
  buttonmenus.add("Entities",menu);

  // old/new point commands
  menu = new ButtonMenu(2,26,2,13,14, (QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Point1",1001, menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Point2",1002, menu->getQW()));
  buttonmenus.add("Point",menu);

  // old/new line commands
  menu = new ButtonMenu(2,26,8,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Line1",1101,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Line3",1103,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Line2",1102,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Line4",1104,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Line5",1105,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Line7",1107,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Line8",1108,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Line6",1106,menu->getQW()));
  buttonmenus.add("Line",menu);

  // new circle commands
  menu = new ButtonMenu(2,26,10,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Circle2",1202,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Circle1",1201,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Circle4",1204,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Circle3",1203,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arc3",1303,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arc2",1302,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arc1",1301,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arc4",1304,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Ellipse2",1206,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Ellipse1",1205,menu->getQW()));
  buttonmenus.add("Circle",menu);

  // old circle commands
  menu = new ButtonMenu(2,26,6,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Circle2",1202,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Circle1",1201,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Circle4",1204,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Circle3",1203,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Ellipse2",1206,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Ellipse1",1205,menu->getQW()));
  buttonmenus.add("Circle-old",menu);

  // arc commands
  menu = new ButtonMenu(2,26,4,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Arc3",1303,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arc2",1302,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arc1",1301,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arc4",1304,menu->getQW()));
  buttonmenus.add("Arc",menu);

  // curve commands
  menu = new ButtonMenu(2,26,6,13,14);
  menu->add(new Button(-1,-1, (char*)"Curve4",1404,(QWidget*)menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Curve1",1401,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Curve5",1405,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Curve2",1402,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Curve3",1403,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Curve6",1406,menu->getQW()));
  buttonmenus.add("Curve",menu);

  // old surface menu new plane names
  menu = new ButtonMenu(2,26,11,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Plane1",1501,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Surface9",1512,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Plane2",1510,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Plane3",1509,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Surface3",1503,menu->getQW()));
//#endif
  menu->add(new Button(-1,-1, (char*)"Surface2",1502,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Surface4",1504,menu->getQW()));
#if MENUS != STUDENTMENUS
  menu->add(new Button(-1,-1, (char*)"Surface5",1505,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Patch2",1507,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Patch3",1508,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Patch1",1506,menu->getQW()));
  //menu->add(new Button(-1,-1,"Trim11",1911,menu->getQW()));
#endif
  buttonmenus.add("Surface",menu);

  // new plane menu
  menu = new ButtonMenu(2,26,11,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Plane1",1501,menu->getQW()));// insert plane by boundary
  menu->add(new Button(-1,-1, (char*)"Plane10",1521,menu->getQW()));// insert plane by segment
  menu->add(new Button(-1,-1, (char*)"Plane2",1510,menu->getQW()));// insert plane by vertex points
  menu->add(new Button(-1,-1, (char*)"Plane3",1509,menu->getQW()));// plane by interior pick
  menu->add(new Button(-1,-1, (char*)"Plane9",1518,menu->getQW()));// Enhanced tools White Out
  menu->add(new Button(-1,-1, (char*)"Plane4",1513,menu->getQW()));// Enhanced tools Add Vertex
  menu->add(new Button(-1,-1, (char*)"Plane5",1514,menu->getQW()));// Enhanced tools Delete Vertex
  menu->add(new Button(-1,-1, (char*)"Plane6",1515,menu->getQW()));// Enhanced tools Move Vertex
  menu->add(new Button(-1,-1, (char*)"Surface9",1512,menu->getQW()));// remove holes
  menu->add(new Button(-1,-1, (char*)"Plane7",1516,menu->getQW()));// Enhanced tools Copy Plane Boundary
  menu->add(new Button(-1,-1, (char*)"Plane8",1517,menu->getQW()));// Enhanced tools Detach Entities
  buttonmenus.add("Plane",menu);

  menu = new ButtonMenu(2,26,9,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Surface3",1503,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Surface2",1502,menu->getQW()));
  //menu->add(new Button(-1,-1,"Surface6",1503,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Surface4",1504,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Surface5",1505,menu->getQW()));
  //menu->add(new Button(-1,-1,"Surface7",1503,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Patch2",1507,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Patch1",1506,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Patch3",1508,menu->getQW()));
  //menu->add(new Button(-1,-1,"Trim11",1911,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"FlatPattern",1519,menu->getQW()));// Flat Pattern
  menu->add(new Button(-1,-1, (char*)"InsertSpiral",1520,menu->getQW()));// Insert Spiral
  buttonmenus.add("NewSurface",menu);

  //menu = new ButtonMenu(2,26,1,1,14,(QWidget*)cadwindow->gethwnd());
  //buttonmenus.add("Solid",menu);

  // old text menu
  menu = new ButtonMenu(2,26,8,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Text1",1601,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Text6",1606,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Text5",1605,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Text2",1602,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Text3",1603,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Text4",1604,menu->getQW()));
#if VERSION != FLEXICAD
  menu->add(new Button(-1,-1, (char*)"BitMap1",1708,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"BitMap2",1709,menu->getQW()));
#endif
  buttonmenus.add("Text",menu);

  // new text menu
  menu = new ButtonMenu(2,26,6,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Text1",1601,menu->getQW())); // insert text
  menu->add(new Button(-1,-1, (char*)"Text6",1606,menu->getQW())); // insert with angle
  // to do implement this
  //menu->add(new Button(-1,-1,"Text7",1601,menu->getQW())); // text along curve
  menu->add(new Button(-1,-1, (char*)"Text5",1605,menu->getQW())); // insert text counter (not yet implemented)
  menu->add(new Button(-1,-1, (char*)"Dimen10",1810,menu->getQW())); // leader
  menu->add(new Button(-1,-1, (char*)"Text2",1602,menu->getQW())); // edit text
  menu->add(new Button(-1,-1, (char*)"Text8",211,menu->getQW())); // find and replace command
  buttonmenus.add("NewText",menu);

  menu = new ButtonMenu(2,26,2,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"BitMap1",1708,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"BitMap2",1709,menu->getQW()));
  buttonmenus.add("Image",menu);

  // old figure commands
  menu = new ButtonMenu(2,26,10,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Figure4",1710,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Figure1",1701,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Figure2",1702,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Figure3",1703,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Figure5",1711,menu->getQW()));
#if MENUS != 2
  menu->add(new Button(-1,-1, (char*)"Group1",1704,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Group2",1705,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Group3",1706,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Group4",1707,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Panel1",1712,menu->getQW()));
#endif
  buttonmenus.add("Figure",menu);

  // new figure commands
  menu = new ButtonMenu(2,26,7,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Figure4",1710,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Figure1",1701,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Figure2",1702,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Figure3",1703,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Figure5",1711,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Text3",1603,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Text4",1604,menu->getQW()));
  buttonmenus.add("Components",menu);

  // new group commands
  menu = new ButtonMenu(2,26,4,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Group1",1704,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Group2",1705,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Group3",1706,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Group4",1707,menu->getQW()));
  //menu->add(new Button(-1,-1,"Panel1",1712));
  buttonmenus.add("Group",menu);

  // old/new dimension commands
  menu = new ButtonMenu(2,26,10,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Dimen8",1808,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Dimen2",1802,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Dimen1",1801,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Dimen3",1803,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Dimen4",1804,menu->getQW()));
#if MENUS != FLEXICADMENUS
  menu->add(new Button(-1,-1, (char*)"Dimen7",1807,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Dimen6",1806,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Dimen9",1809,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Dimen5",1805,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Dimen11",1811,menu->getQW()));
  //menu->add(new Button(-1,-1,"Dimen10",1810));
#endif  
  buttonmenus.add("Dimen",menu);

  // new interupt commands
  menu = new ButtonMenu(288,2,13,13,27,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Position0",2000,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Zoom0",2100,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"ViewMenu0",564,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"WorkPlane0",430,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Render0",565,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"DynamicView0",580,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"ResetWindow0",2600,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Capture0",565,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Compose0",563,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Trim0",1900,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arrange0",8000,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"File0",10000,menu->getQW())); // padding??
  menu->add(new Button(-1,-1, (char*)"",0,menu->getQW())); // padding??
  buttonmenus.add("Interrupt",menu);

  // new assistant commands
#if MAJOR_VERSION > 7 // show rendering menu
  menu = new ButtonMenu(288,2,14,14,27,(QWidget*)cadwindow->gethwnd());
#else
  menu = new ButtonMenu(288,2,13,14,27,(QWidget*)cadwindow->gethwnd());
#endif
  menu->add(new Button(-1,-1, (char*)"Position0",2000,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Zoom0",2100,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"ViewMenu0",564,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"WorkPlane0",430,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Viewstyle0",5100,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"DynamicView0",580,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"ResetWindow0",2600,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Capture0",5130,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Layout0",563,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Trim0",1900,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arrange0",8000,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"File0",10000,menu->getQW()));
#if MAJOR_VERSION > 7// show rendering menu
  menu->add(new Button(-1,-1, (char*)"Render0",565,menu->getQW()));
#endif
  menu->add(new Button(-1,-1, (char*)"",0,menu->getQW())); // padding??
  buttonmenus.add("Assistant",menu);

  // new view style command
  menu = new ButtonMenu(2,26,12,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Viewstyle1",5101));  // wirefame
  menu->add(new Button(-1,-1, (char*)"Viewstyle2",5102));  // hidden line
  menu->add(new Button(-1,-1, (char*)"Viewstyle3",5103));  // hidden line with dashed lines
  menu->add(new Button(-1,-1, (char*)"Viewstyle4",5104));  // shaded surface
  menu->add(new Button(-1,-1, (char*)"Viewstyle5",5105));  // shaded shaded with lines
  menu->add(new Button(-1,-1, (char*)"Viewstyle6",5106));  // shaded dashed lines
  menu->add(new Button(-1,-1, (char*)"Viewstyle7",5107));  // texture shaded
  menu->add(new Button(-1,-1, (char*)"Viewstyle8",5108));  // textured with lines
  menu->add(new Button(-1,-1, (char*)"Viewstyle9",5109));  // virtual reality (stereo)
  menu->add(new Button(-1,-1, (char*)"Viewstyle10",5110)); // view as perspective projection
  menu->add(new Button(-1,-1, (char*)"Viewstyle11",5111)); // view as parallel projection
  menu->add(new Button(-1,-1, (char*)"Viewstyle12",5112)); // shade options
  buttonmenus.add("Viewstyle",menu);

  // new capture commands
  menu = new ButtonMenu(2,26,10,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Capture1",5131,menu->getQW())); // CaptureView
  menu->add(new Button(-1,-1, (char*)"Capture2",5132,menu->getQW())); // CaptureArea
  menu->add(new Button(-1,-1, (char*)"Capture3",5133,menu->getQW())); // CaptureBitmap
  menu->add(new Button(-1,-1, (char*)"Capture4",5134,menu->getQW())); // RaytraceRender
  menu->add(new Button(-1,-1, (char*)"Capture5",5135,menu->getQW())); // CameraPath
  menu->add(new Button(-1,-1, (char*)"Capture6",5136,menu->getQW())); // ReferencePath
  menu->add(new Button(-1,-1, (char*)"Capture7",5137,menu->getQW())); // ObjectPath
  menu->add(new Button(-1,-1, (char*)"Capture8",5138,menu->getQW())); // PatchPath
  menu->add(new Button(-1,-1, (char*)"Capture9",5139,menu->getQW())); // RecordAnimation
  menu->add(new Button(-1,-1, (char*)"Capture10",5140,menu->getQW()));// ReplayAnimationFile
  buttonmenus.add("Capture",menu);

  // old trim new edit commands
  menu = new ButtonMenu(2,26,8,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Trim1",1901,menu->getQW())); // trim/extend
  menu->add(new Button(-1,-1, (char*)"Trim10",1910,menu->getQW())); // trim to corner
  menu->add(new Button(-1,-1, (char*)"Trim2",1902,menu->getQW())); // trim by length
  menu->add(new Button(-1,-1, (char*)"Trim3",1903,menu->getQW())); // divide by entity
  menu->add(new Button(-1,-1, (char*)"Trim4",1904,menu->getQW())); // divide by length
  menu->add(new Button(-1,-1, (char*)"Trim5",1905,menu->getQW())); // divide by number
  menu->add(new Button(-1,-1, (char*)"Trim8",1908,menu->getQW())); // delete segment
  menu->add(new Button(-1,-1, (char*)"Trim9",1909,menu->getQW())); // explode
  buttonmenus.add("Trim",menu);

  // arrange commands
  menu = new ButtonMenu(2,26,4,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Arrange1",800,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arrange2",801,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arrange3",802,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Arrange4",803,menu->getQW()));
  buttonmenus.add("Arrange",menu);

  // old measure commands
  menu = new ButtonMenu(2,26,1,11,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Measure4",1954,menu->getQW()));
  //menu->add(new Button(-1,-1,"Measure1",1951,menu->getQW()));
  //menu->add(new Button(-1,-1,"Measure2",1952,menu->getQW()));
  //menu->add(new Button(-1,-1,"Measure3",1953,menu->getQW()));
  buttonmenus.add("Measure",menu);

  // new layout (compose) commands
  menu = new ButtonMenu(288,26,12,13,14);
  menu->add(new Button(-1,-1, (char*)"Compose1",524)); // new layout
  menu->add(new Button(-1,-1, (char*)"Compose11",567));// copy layout
  menu->add(new Button(-1,-1, (char*)"Compose8",501)); // select layout
  menu->add(new Button(-1,-1, (char*)"Compose12",571));// rename layout
  menu->add(new Button(-1,-1, (char*)"Compose2",525)); // delete layout
  menu->add(new Button(-1,-1, (char*)"Compose3",526)); // add viewport
  menu->add(new Button(-1,-1, (char*)"Compose7",558)); // fit in viewport
  menu->add(new Button(-1,-1, (char*)"Compose6",530)); // pan in viewport
  menu->add(new Button(-1,-1, (char*)"Compose9",581)); // move viewport
  menu->add(new Button(-1,-1, (char*)"Compose10",566));// align viewport
  menu->add(new Button(-1,-1, (char*)"Compose4",556)); // change viewport
  menu->add(new Button(-1,-1, (char*)"Compose5",527)); // delete viewport
  buttonmenus.add("Layout",menu);

  // old view menu
  menu = new ButtonMenu(288,26,7,13,14);
  menu->add(new Button(-1,-1, (char*)"ViewMenu1",500)); // select a view
  menu->add(new Button(-1,-1, (char*)"ViewMenu2",506)); // create a new view
  menu->add(new Button(-1,-1, (char*)"ViewMenu3",507)); // delete a view
  menu->add(new Button(-1,-1, (char*)"ViewMenu4",521)); // set plot scale
  menu->add(new Button(-1,-1, (char*)"ViewMenu5",535)); // new zone
  menu->add(new Button(-1,-1, (char*)"ViewMenu6",536)); // delete zone
  menu->add(new Button(-1,-1, (char*)"DynamicView10",522)); // reset view
  buttonmenus.add("View-old",menu);

  // new view menu
  menu = new ButtonMenu(288,26,9,13,14);
  menu->add(new Button(-1,-1, (char*)"ViewMenu2",506)); // create a new view
  menu->add(new Button(-1,-1, (char*)"ViewMenu1",500)); // select a view
  menu->add(new Button(-1,-1, (char*)"ViewMenu4",521)); // set plot scale
  menu->add(new Button(-1,-1, (char*)"DynamicView9",517)); // align to workplane
  menu->add(new Button(-1,-1, (char*)"DynamicView8",516)); // rotate about axis
  menu->add(new Button(-1,-1, (char*)"ViewMenu3",507)); // delete a view
  menu->add(new Button(-1,-1, (char*)"ViewMenu7",587)); // previous
  menu->add(new Button(-1,-1, (char*)"DynamicView10",522)); // reset view
  menu->add(new Button(-1,-1, (char*)"FullScreen",588)); // toggle to full screen
  buttonmenus.add("View",menu);

  // old render commands
  menu = new ButtonMenu(288,26,7,13,14);
  menu->add(new Button(-1,-1, (char*)"Render1",529)); // hidden line
  menu->add(new Button(-1,-1, (char*)"Render3",528)); // shaded
  menu->add(new Button(-1,-1, (char*)"Render4",2200));// repaint
  menu->add(new Button(-1,-1, (char*)"Render5",538)); // lights
  menu->add(new Button(-1,-1, (char*)"Render6",539)); // materials
  menu->add(new Button(-1,-1, (char*)"Render7",537)); // lights
  menu->add(new Button(-1,-1, (char*)"Render8",540)); // render
  buttonmenus.add("Render-old",menu);

  // new rendering commands
  menu = new ButtonMenu(288,26,5,13,14);
  //menu->add(new Button(-1,-1,"Render1",529)); // hidden line
  //menu->add(new Button(-1,-1,"Render2",569)); // hidden line with dashed lines
  //menu->add(new Button(-1,-1,"Render3",528)); // shaded
  menu->add(new Button(-1,-1, (char*)"Render5",537)); // add a light
  menu->add(new Button(-1,-1, (char*)"Render9",582)); // add sunlight
  menu->add(new Button(-1,-1, (char*)"Render7",538)); // lights options dialog
  menu->add(new Button(-1,-1, (char*)"Render6",539)); // add/change materials
  menu->add(new Button(-1,-1, (char*)"Render8",540)); // render raytracing
  buttonmenus.add("Render",menu);

  // old position commands
  menu = new ButtonMenu(288,26,9,13,14);
  menu->add(new Button(-1,-1, (char*)"Position5",2005));
  menu->add(new Button(-1,-1, (char*)"Position2",2002));
  menu->add(new Button(-1,-1, (char*)"Position3",2003));
  menu->add(new Button(-1,-1, (char*)"Position4",2004));
  menu->add(new Button(-1,-1, (char*)"Position7",2007));
#if MENUS != STUDENTMENUS
  menu->add(new Button(-1,-1, (char*)"Position8",2008));
  menu->add(new Button(-1,-1, (char*)"Position1",2001));
  menu->add(new Button(-1,-1, (char*)"Position9",2009));
  menu->add(new Button(-1,-1, (char*)"Position11",2011));
#endif
  buttonmenus.add("Position",menu);

  // old zoom commands
  menu = new ButtonMenu(288,26,9,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Zoom1",2101,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Zoom6",2106,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Zoom2",2102,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Zoom5",2105,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Zoom8",2108,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Zoom3",2103,menu->getQW()));
#if VERSION != TRICAD
  menu->add(new Button(-1,-1, (char*)"Zoom4",2104,menu->getQW()));
#endif
  menu->add(new Button(-1,-1, (char*)"Zoom7",2107,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Zoom9",505,menu->getQW()));
  buttonmenus.add("Zoom-old",menu);

  // new zoom commands
  menu = new ButtonMenu(288,26,8,13,14,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Zoom1",2101,menu->getQW())); // window
  menu->add(new Button(-1,-1, (char*)"Zoom6",2106,menu->getQW())); // fit
  menu->add(new Button(-1,-1, (char*)"Zoom2",2102,menu->getQW())); // pan
  menu->add(new Button(-1,-1, (char*)"Zoom5",2105,menu->getQW())); // key in scale
  menu->add(new Button(-1,-1, (char*)"Zoom7",2107,menu->getQW())); // previous
  menu->add(new Button(-1,-1, (char*)"ViewMenu5",535)); // new zone
  menu->add(new Button(-1,-1, (char*)"Zoom8",2108,menu->getQW())); // zoom to zone
  menu->add(new Button(-1,-1, (char*)"ViewMenu6",536)); // delete zone
  //menu->add(new Button(-1,-1,"Zoom9",505,menu->getQW())); // repaint
  //menu->add(new Button(-1,-1,"Zoom3",2103,menu->getQW()));
  //menu->add(new Button(-1,-1,"Zoom4",2104,menu->getQW()));
  buttonmenus.add("Zoom",menu);

  // old selection mask commands
  menu = new ButtonMenu(288,26,7,11,14);
  menu->add(new Button(-1,-1, (char*)"Mask1",2301));
  menu->add(new Button(-1,-1, (char*)"Mask2",2302));
  menu->add(new Button(-1,-1, (char*)"Mask3",2303));
  menu->add(new Button(-1,-1, (char*)"Mask4",2304));
  menu->add(new Button(-1,-1, (char*)"Mask5",2305));
  menu->add(new Button(-1,-1, (char*)"Position6",2006));
  menu->add(new Button(-1,-1, (char*)"Position10",2010));
  buttonmenus.add("SelectionMask",menu);

  // new selection mask commands
  menu = new ButtonMenu(288,26,7,11,14);
  menu->add(new Button(-1,-1, (char*)"Mask1",23101));
  menu->add(new Button(-1,-1, (char*)"Mask2",23102));
  menu->add(new Button(-1,-1, (char*)"Mask3",23103));
  menu->add(new Button(-1,-1, (char*)"Mask4",23104));
  menu->add(new Button(-1,-1, (char*)"Mask5",23105));
  menu->add(new Button(-1,-1, (char*)"Position6",2006));
  menu->add(new Button(-1,-1, (char*)"Position10",2010));
  buttonmenus.add("SelectionMask",menu);

  // old display mask commands
  menu = new ButtonMenu(288,26,5,11,14);
  menu->add(new Button(-1,-1, (char*)"Mask1",2401));
  menu->add(new Button(-1,-1, (char*)"Mask2",2402));
  menu->add(new Button(-1,-1, (char*)"Mask3",2403));
  menu->add(new Button(-1,-1, (char*)"Mask4",2404));
  menu->add(new Button(-1,-1, (char*)"Mask5",2405));
  buttonmenus.add("DisplayMask",menu);

  // new display mask commands
  menu = new ButtonMenu(288,26,5,11,14);
  menu->add(new Button(-1,-1, (char*)"Mask1",24101));
  menu->add(new Button(-1,-1, (char*)"Mask2",24102));
  menu->add(new Button(-1,-1, (char*)"Mask3",24103));
  menu->add(new Button(-1,-1, (char*)"Mask4",24104));
  menu->add(new Button(-1,-1, (char*)"Mask5",24105));
  buttonmenus.add("DisplayMask",menu);

  menu = new ButtonMenu(288,26,9,13,14);
  menu->add(new Button(-1,-1, (char*)"ViewTop",2501));
  menu->add(new Button(-1,-1, (char*)"ViewFront",2502));
  menu->add(new Button(-1,-1, (char*)"ViewRight",2504));
  menu->add(new Button(-1,-1, (char*)"ViewBack",2505));
  menu->add(new Button(-1,-1, (char*)"ViewLeft",2503));
  menu->add(new Button(-1,-1, (char*)"ViewBottom",2506));
  menu->add(new Button(-1,-1, (char*)"ViewPar",2508));
  menu->add(new Button(-1,-1, (char*)"ViewIso",2507));
  menu->add(new Button(-1,-1, (char*)"ViewPersp",2509));
  buttonmenus.add("CreateWindow",menu);

  menu = new ButtonMenu(288,26,9,13,14);
  menu->add(new Button(-1,-1, (char*)"ViewTopReset",2601));
  menu->add(new Button(-1,-1, (char*)"ViewFrontReset",2602));
  menu->add(new Button(-1,-1, (char*)"ViewRightReset",2604));
  menu->add(new Button(-1,-1, (char*)"ViewBackReset",2605));
  menu->add(new Button(-1,-1, (char*)"ViewLeftReset",2603));
  //menu->add(new Button(-1,-1,"ViewBottomReset",2606));
  menu->add(new Button(-1,-1, (char*)"ViewParReset",2608));
  menu->add(new Button(-1,-1, (char*)"ViewIsoReset",2607));
  menu->add(new Button(-1,-1, (char*)"ViewPerspReset",2609));
  menu->add(new Button(-1,-1, (char*)"ViewTopPerspReset",2610));
  buttonmenus.add("ResetWindow",menu);

  // old dynamic view commands
  menu = new ButtonMenu(288,26,11,13,14);
  menu->add(new Button(-1,-1, (char*)"DynamicView3",555)); // dynamic rotate single roatation
  menu->add(new Button(-1,-1, (char*)"DynamicView5",518)); // dynamic rotation
  menu->add(new Button(-1,-1, (char*)"DynamicView2",561)); // dynamic rotate axis
  menu->add(new Button(-1,-1, (char*)"DynamicView1",520)); // fly
  menu->add(new Button(-1,-1, (char*)"DynamicView4",519)); // walk
  menu->add(new Button(-1,-1, (char*)"DynamicView6",511)); // architectual perspective
  menu->add(new Button(-1,-1, (char*)"DynamicView7",541)); // adjust view menu
  menu->add(new Button(-1,-1, (char*)"DynamicView8",516)); // change by rotation
  menu->add(new Button(-1,-1, (char*)"DynamicView9",517)); // align to workplane
  menu->add(new Button(-1,-1, (char*)"DynamicView10",522));// reset view
  buttonmenus.add("DynamicView-old",menu);

  // new dynamic view commands
  menu = new ButtonMenu(288,26,8,13,14);
  menu->add(new Button(-1,-1, (char*)"DynamicView11",583));  // dynamic rotate axis
  menu->add(new Button(-1,-1, (char*)"DynamicView3",555));   // dynamic rotate single roatation
  menu->add(new Button(-1,-1, (char*)"DynamicView6",511));   // architectual perspective
  //menu->add(new Button(-1,-1,"DynamicView12",584));   // Position viewer command
  menu->add(new Button(-1,-1, (char*)"DynamicView13",585));   // look around
  menu->add(new Button(-1,-1, (char*)"DynamicView4",519));   // walk
  menu->add(new Button(-1,-1, (char*)"DynamicView1",520));   // fly
  menu->add(new Button(-1,-1, (char*)"DynamicView14",586));  // section plane
  menu->add(new Button(-1,-1, (char*)"DynamicView7",541));   // adjust view menu
  buttonmenus.add("DynamicView",menu);

  // new workplane commands
  menu = new ButtonMenu(288,26,8,13,14);
  menu->add(new Button(-1,-1, (char*)"WorkPlane1",403)); // create
  menu->add(new Button(-1,-1, (char*)"WorkPlane6",402)); // select
  menu->add(new Button(-1,-1, (char*)"WorkPlane3",405)); // orient axis
  menu->add(new Button(-1,-1, (char*)"WorkPlane4",404)); // move
  menu->add(new Button(-1,-1, (char*)"WorkPlane8",425)); // set on entity
  menu->add(new Button(-1,-1, (char*)"WorkPlane2",407)); // delete
  menu->add(new Button(-1,-1, (char*)"WorkPlane5",406)); // change
  menu->add(new Button(-1,-1, (char*)"WorkPlane7",416)); // grid
  buttonmenus.add("WorkPlane",menu);

  // quick menu
  menu = new ButtonMenu(-76,2,13,13,15,(QWidget*)cadwindow->gethwnd());
  menu->add(new Button(-1,-1, (char*)"Delete",205,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Undo",200,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Redo",201,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Measure4",1954,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"SnapSettings",2406,menu->getQW())); // ToDo: new snap Settings button
  menu->add(new Button(-1,-1, (char*)"Position6",2006));
  menu->add(new Button(-1,-1, (char*)"Position10",2010));
  menu->add(new Button(-1,-1, (char*)"BackColour1",3106,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"WorkPlane7",416,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"Paint0",2200,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"PrintWindow",120,menu->getQW())); // ToDo: new combined print command
  menu->add(new Button(-1,-1, (char*)"PrintSelection",121,menu->getQW())); // ToDo: new combined print command
  menu->add(new Button(-1,-1, (char*)"PrefSettings",417,menu->getQW())); // ToDo: new Settings button
  buttonmenus.add("Quick",menu);

  menu = new ButtonMenu(10,2,4,4,15);
  menu->add(new Button(-1,-1, (char*)"PrintPreview1",122));
  menu->add(new Button(-1,-1, (char*)"PrintPreview2",123));
  menu->add(new Button(-1,-1, (char*)"PrintPreview3",124));
  menu->add(new Button(-1,-1, (char*)"PrintPreview4",125));
  buttonmenus.add("PrintPreview",menu);

  menu = new ButtonMenu(524,2,12,12,27);
  buttonmenus.add("BlankMain",menu);
  menu = new ButtonMenu(524,26,12,12,14);
  buttonmenus.add("BlankSub",menu);

  // new File menu
  menu = new ButtonMenu(524,2,10,13,14);
  menu->add(new Button(-1,-1, (char*)"Save",102,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"SaveAsExport",103,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"OpenImport",101,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"New",100,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"NewFromTemplate",117,menu->getQW()));
  //menu->add(new Button(-1,-1,"PrintWindow",120,menu->getQW())); // ToDo: new combined pront command
  //menu->add(new Button(-1,-1,"PrintSelection",121,menu->getQW())); // ToDo: new combined pront command
  menu->add(new Button(-1,-1, (char*)"FileTools",10018,menu->getQW())); // ToDO: new command
  menu->add(new Button(-1,-1, (char*)"FileInfo",212,menu->getQW())); // ToDO: new command
  menu->add(new Button(-1,-1, (char*)"FindInFiles",215,menu->getQW()));
  menu->add(new Button(-1,-1, (char*)"FileEmail",10019,menu->getQW())); // ToDO: new command
  //menu->add(new Button(-1,-1,"PrefSettings",417,menu->getQW())); // ToDo: new Settings button
  menu->add(new Button(-1,-1, (char*)"3DWarehouse",10020,menu->getQW())); // ToDO: new command SketchUp 3D warehouse
  buttonmenus.add("File",menu);

#else

  //  Standard menus
  menu = new ButtonMenu(2,2,12,12,27);
  menu->add(new Button(-1,-1,"Point0",1000));
  menu->add(new Button(-1,-1,"Line0",1100));
  menu->add(new Button(-1,-1,"Circle0",1200));
  menu->add(new Button(-1,-1,"Arc0",1300));
  menu->add(new Button(-1,-1,"Curve0",1400));
  menu->add(new Button(-1,-1,"Surface0",1500));
  menu->add(new Button(-1,-1,"Text0",1600));
  menu->add(new Button(-1,-1,"Figure0",1700));
  menu->add(new Button(-1,-1,"Dimen0",1800));
  menu->add(new Button(-1,-1,"Trim0",1900));
  menu->add(new Button(-1,-1,"Measure0",1950));
  menu->add(new Button(-1,-1,"Transform0",320));
  buttonmenus.add("Tool",menu);

  menu = new ButtonMenu(2,26,12,12,14);
  menu->add(new Button(-1,-1,"Point1",1001));
  menu->add(new Button(-1,-1,"Point2",1002));
  buttonmenus.add("Point",menu);

  menu = new ButtonMenu(2,26,12,12,14);
  menu->add(new Button(-1,-1,"Line1",1101));
  menu->add(new Button(-1,-1,"Line2",1102));
  menu->add(new Button(-1,-1,"Line3",1103));
  menu->add(new Button(-1,-1,"Line4",1104));
  menu->add(new Button(-1,-1,"Line5",1105));
  menu->add(new Button(-1,-1,"Line6",1106));
  menu->add(new Button(-1,-1,"Line7",1107));
  menu->add(new Button(-1,-1,"Line8",1108));
  buttonmenus.add("Line",menu);

  menu = new ButtonMenu(2,26,12,12,14);
  menu->add(new Button(-1,-1,"Circle1",1201));
  menu->add(new Button(-1,-1,"Circle2",1202));
  menu->add(new Button(-1,-1,"Circle3",1203));
  menu->add(new Button(-1,-1,"Circle4",1204));
  menu->add(new Button(-1,-1,"Ellipse1",1205));
  menu->add(new Button(-1,-1,"Ellipse2",1206));
  buttonmenus.add("Circle",menu);

  menu = new ButtonMenu(2,26,12,12,14);
  menu->add(new Button(-1,-1,"Arc1",1301));
  menu->add(new Button(-1,-1,"Arc2",1302));
  menu->add(new Button(-1,-1,"Arc3",1303));
  menu->add(new Button(-1,-1,"Arc4",1304));
  buttonmenus.add("Arc",menu);

  menu = new ButtonMenu(2,26,12,12,14);
  menu->add(new Button(-1,-1,"Curve1",1401));
  menu->add(new Button(-1,-1,"Curve4",1404));
  menu->add(new Button(-1,-1,"Curve2",1402));
  menu->add(new Button(-1,-1,"Curve5",1405));
  menu->add(new Button(-1,-1,"Curve3",1403));
  menu->add(new Button(-1,-1,"Curve6",1406));
  buttonmenus.add("Curve",menu);

  menu = new ButtonMenu(2,26,12,12,14);
  menu->add(new Button(-1,-1,"Surface1",1501));
  menu->add(new Button(-1,-1,"Surface9",1512));
  menu->add(new Button(-1,-1,"Surface7",1510));
  menu->add(new Button(-1,-1,"Surface6",1509));
  menu->add(new Button(-1,-1,"Surface2",1502));
#if MENUS != STUDENTMENUS
  menu->add(new Button(-1,-1,"Surface3",1503));
#endif
  menu->add(new Button(-1,-1,"Surface4",1504));
#if MENUS != STUDENTMENUS
  menu->add(new Button(-1,-1,"Surface5",1505));
  menu->add(new Button(-1,-1,"Patch1",1506));
  menu->add(new Button(-1,-1,"Patch2",1507));
  menu->add(new Button(-1,-1,"Patch3",1508));
#endif
  buttonmenus.add("Surface",menu);

  menu = new ButtonMenu(2,26,12,12,14);
  menu->add(new Button(-1,-1,"Text1",1601));
  menu->add(new Button(-1,-1,"Text6",1606));
  menu->add(new Button(-1,-1,"Text2",1602));
  menu->add(new Button(-1,-1,"Text3",1603));
  menu->add(new Button(-1,-1,"Text4",1604));
  menu->add(new Button(-1,-1,"Text5",1605));
  buttonmenus.add("Text",menu);

  menu = new ButtonMenu(2,26,12,12,14);
  menu->add(new Button(-1,-1,"Figure1",1701));
  menu->add(new Button(-1,-1,"Figure2",1702));
  menu->add(new Button(-1,-1,"Figure5",1711));
  menu->add(new Button(-1,-1,"Figure3",1703));
  menu->add(new Button(-1,-1,"Figure4",1710));
#if MENUS != 2
#if VERSION != FLEXICAD
  menu->add(new Button(-1,-1,"BitMap1",1708));
  menu->add(new Button(-1,-1,"BitMap2",1709));
#endif
  menu->add(new Button(-1,-1,"Group1",1704));
  menu->add(new Button(-1,-1,"Group2",1705));
  menu->add(new Button(-1,-1,"Group3",1706));
  menu->add(new Button(-1,-1,"Group4",1707));
  menu->add(new Button(-1,-1,"Panel1",1712));
#endif
  buttonmenus.add("Figure",menu);

  menu = new ButtonMenu(2,26,12,12,14);
  menu->add(new Button(-1,-1,"Dimen1",1801));
  menu->add(new Button(-1,-1,"Dimen2",1802));
  menu->add(new Button(-1,-1,"Dimen8",1808));
  menu->add(new Button(-1,-1,"Dimen3",1803));
  menu->add(new Button(-1,-1,"Dimen4",1804));
  menu->add(new Button(-1,-1,"Dimen5",1805));
  menu->add(new Button(-1,-1,"Dimen6",1806));
#if MENUS != FLEXICADMENUS
  menu->add(new Button(-1,-1,"Dimen7",1807));
  menu->add(new Button(-1,-1,"Dimen9",1809));
  menu->add(new Button(-1,-1,"Dimen10",1810));
  menu->add(new Button(-1,-1,"Dimen11",1811));
#endif  
  buttonmenus.add("Dimen",menu);

  menu = new ButtonMenu(2,26,12,12,14);
  menu->add(new Button(-1,-1,"Trim1",1901));
  menu->add(new Button(-1,-1,"Trim10",1910));
  menu->add(new Button(-1,-1,"Trim2",1902));
  menu->add(new Button(-1,-1,"Trim3",1903));
  menu->add(new Button(-1,-1,"Trim4",1904));
  menu->add(new Button(-1,-1,"Trim5",1905));
  menu->add(new Button(-1,-1,"Trim8",1908));
  menu->add(new Button(-1,-1,"Trim6",1906));
  menu->add(new Button(-1,-1,"Trim7",1907));
  menu->add(new Button(-1,-1,"Trim9",1909));
#if MENUS != FLEXICADMENUS
  menu->add(new Button(-1,-1,"Trim11",1911));
#endif
  buttonmenus.add("Trim",menu);

  menu = new ButtonMenu(2,26,12,12,14);
  menu->add(new Button(-1,-1,"Measure1",1951));
  menu->add(new Button(-1,-1,"Measure2",1952));
  menu->add(new Button(-1,-1,"Measure3",1953));
  buttonmenus.add("Measure",menu);

  menu = new ButtonMenu(288,2,11,11,27);
  menu->add(new Button(-1,-1,"Position0",2000));
  menu->add(new Button(-1,-1,"Zoom0",2100));
  menu->add(new Button(-1,-1,"Paint0",2200));
  menu->add(new Button(-1,-1,"SelectionMask0",2300));
  menu->add(new Button(-1,-1,"DisplayMask0",2400));
  menu->add(new Button(-1,-1,"CreateWindow",2500));
  menu->add(new Button(-1,-1,"ResetWindow",2600));
  menu->add(new Button(-1,-1,"WorkPlane0",430));
  menu->add(new Button(-1,-1,"Compose0",563));
  buttonmenus.add("Interrupt",menu);

  menu = new ButtonMenu(288,26,11,11,14);
  menu->add(new Button(-1,-1,"Compose8",501));
  menu->add(new Button(-1,-1,"Compose1",524));
  menu->add(new Button(-1,-1,"Compose3",526));
  menu->add(new Button(-1,-1,"Compose7",558));
  menu->add(new Button(-1,-1,"Compose6",530));
  menu->add(new Button(-1,-1,"Compose4",));
#if MENUS != STUDENTMENUS
  menu->add(new Button(-1,-1,"Compose5",527));
  menu->add(new Button(-1,-1,"Compose2",525));
  menu->add(new Button(-1,-1,"Compose9",581));
  menu->add(new Button(-1,-1,"Compose10",));
#endif
  buttonmenus.add("ICompose",menu);

  menu = new ButtonMenu(288,26,11,11,14);
  menu->add(new Button(-1,-1,"Position1",2001));
  menu->add(new Button(-1,-1,"Position2",2002));
  menu->add(new Button(-1,-1,"Position3",2003));
  menu->add(new Button(-1,-1,"Position4",2004));
  menu->add(new Button(-1,-1,"Position5",2005));
#if MENUS != STUDENTMENUS
  menu->add(new Button(-1,-1,"Position6",2006));
  menu->add(new Button(-1,-1,"Position7",2007));
  menu->add(new Button(-1,-1,"Position8",2008));
  menu->add(new Button(-1,-1,"Position9",2009));
  menu->add(new Button(-1,-1,"Position10",2010));
  menu->add(new Button(-1,-1,"Position11",2011));
#endif
  buttonmenus.add("Position",menu);

  menu = new ButtonMenu(288,26,11,11,14);
  menu->add(new Button(-1,-1,"Zoom1",2101));
  menu->add(new Button(-1,-1,"Zoom2",2102));
  menu->add(new Button(-1,-1,"Zoom3",2103));
#if VERSION != TRICAD
  menu->add(new Button(-1,-1,"Zoom4",2104));
#endif
  menu->add(new Button(-1,-1,"Zoom5",2105));
  menu->add(new Button(-1,-1,"Zoom6",2106));
  menu->add(new Button(-1,-1,"Zoom8",2108));
  menu->add(new Button(-1,-1,"Zoom7",2107));
  buttonmenus.add("Zoom",menu);

  menu = new ButtonMenu(288,26,11,11,14);
  menu->add(new Button(-1,-1,"Mask1",2301));
  menu->add(new Button(-1,-1,"Mask2",2302));
  menu->add(new Button(-1,-1,"Mask3",2303));
  menu->add(new Button(-1,-1,"Mask4",2304));
  menu->add(new Button(-1,-1,"Mask5",2305));
  buttonmenus.add("SelectionMask",menu);

  menu = new ButtonMenu(288,26,11,11,14);
  menu->add(new Button(-1,-1,"Mask1",2401));
  menu->add(new Button(-1,-1,"Mask2",2402));
  menu->add(new Button(-1,-1,"Mask3",2403));
  menu->add(new Button(-1,-1,"Mask4",2404));
  menu->add(new Button(-1,-1,"Mask5",2405));
  buttonmenus.add("DisplayMask",menu);

  menu = new ButtonMenu(288,26,11,11,14);
  menu->add(new Button(-1,-1,"ViewTop",2501));
  menu->add(new Button(-1,-1,"ViewFront",2502));
  menu->add(new Button(-1,-1,"ViewLeft",2503));
  menu->add(new Button(-1,-1,"ViewRight",2504));
  menu->add(new Button(-1,-1,"ViewBack",2505));
  menu->add(new Button(-1,-1,"ViewBottom",2506));
  menu->add(new Button(-1,-1,"ViewIso",2507));
  menu->add(new Button(-1,-1,"ViewPar",2508));
  menu->add(new Button(-1,-1,"ViewPersp",2509));
  buttonmenus.add("CreateWindow",menu);

  menu = new ButtonMenu(288,26,11,11,14);
  menu->add(new Button(-1,-1,"ViewTopReset",2601));
  menu->add(new Button(-1,-1,"ViewFrontReset",2602));
  menu->add(new Button(-1,-1,"ViewLeftReset",2603));
  menu->add(new Button(-1,-1,"ViewRightReset",2604));
  menu->add(new Button(-1,-1,"ViewBackReset",2605));
  menu->add(new Button(-1,-1,"ViewBottomReset",2606));
  menu->add(new Button(-1,-1,"ViewIsoReset",2607));
  menu->add(new Button(-1,-1,"ViewParReset",2608));
  menu->add(new Button(-1,-1,"ViewPerspReset",2609));
  buttonmenus.add("ResetWindow",menu);

  menu = new ButtonMenu(288,26,11,11,14);
  menu->add(new Button(-1,-1,"DynamicView1",520));
  menu->add(new Button(-1,-1,"DynamicView2",561));
  menu->add(new Button(-1,-1,"DynamicView3",518));
  menu->add(new Button(-1,-1,"DynamicView4",519));
  menu->add(new Button(-1,-1,"DynamicView5",555));
  buttonmenus.add("DynamicView",menu);

  menu = new ButtonMenu(288,26,11,11,14);
  menu->add(new Button(-1,-1,"WorkPlane1",403));
  menu->add(new Button(-1,-1,"WorkPlane2",407));
  menu->add(new Button(-1,-1,"WorkPlane3",405));
  menu->add(new Button(-1,-1,"WorkPlane4",404));
  menu->add(new Button(-1,-1,"WorkPlane5",406));
  menu->add(new Button(-1,-1,"WorkPlane6",402));
  menu->add(new Button(-1,-1,"WorkPlane7",416));
  buttonmenus.add("WorkPlane",menu);

  menu = new ButtonMenu(-76,2,7,4,15);
  menu->add(new Button(-1,-1,"Colour",3000));
  menu->add(new Button(-1,-1,"Layer",3001));
  menu->add(new Button(-1,-1,"Style",3002));
  menu->add(new Button(-1,-1,"Weight",3003));
  menu->add(new Button(-1,-1,"StyleSet",3004));
  menu->add(new Button(-1,-1,"Undo",200));
  menu->add(new Button(-1,-1,"Redo",201));
  buttonmenus.add("Quick",menu);

  menu = new ButtonMenu(10,2,4,4,15);
  menu->add(new Button(-1,-1,"PrintPreview1",122));
  menu->add(new Button(-1,-1,"PrintPreview2",123));
  menu->add(new Button(-1,-1,"PrintPreview3",124));
  menu->add(new Button(-1,-1,"PrintPreview4",125));
  buttonmenus.add("PrintPreview",menu);

#endif

  //  Application menus
  menu = new ButtonMenu(0,0,12,12,15);
  menu->add(new Button(-1,-1, (char*)"Animation1",20000));
  menu->add(new Button(-1,-1, (char*)"Animation2",20001));
  menu->add(new Button(-1,-1, (char*)"Animation3",20002));
#if VERSION != FLEXICAD
  menu->add(new Button(-1,-1, (char*)"Animation12",20005));
#endif
  menu->add(new Button(-1,-1, (char*)"Animation4",20003));
#if VERSION != FLEXICAD
  menu->add(new Button(-1,-1, (char*)"Animation5",20004));
#endif
  buttonmenus.add("Animation",menu);

  menu = new ButtonMenu(2,2,11,11,14);
  menu->add(new Button(-1,-1, (char*)"Transform5",306));
  menu->add(new Button(-1,-1, (char*)"Transform3",301));
  menu->add(new Button(-1,-1, (char*)"Transform10",303));
  menu->add(new Button(-1,-1, (char*)"Transform4",304));
  menu->add(new Button(-1,-1, (char*)"Transform11",300));
  menu->add(new Button(-1,-1, (char*)"Transform9",305));
  menu->add(new Button(-1,-1, (char*)"Transform2",307));
  menu->add(new Button(-1,-1, (char*)"Transform1",308));
  menu->add(new Button(-1,-1, (char*)"Transform8",302));
  menu->add(new Button(-1,-1, (char*)"Transform6",1906));
  menu->add(new Button(-1,-1, (char*)"Transform7",1907));
  buttonmenus.add("Transform",menu);

  //buttonmenus.match("Tool")->parent = cadwindow;
  buttonmenus.match("Entities")->parent = cadwindow;
  buttonmenus.match("Line")->parent = cadwindow;
  //buttonmenus.match("Interrupt")->parent = cadwindow;
  buttonmenus.match("Assistant")->parent = cadwindow;
  buttonmenus.match("Zoom")->parent = cadwindow;
  buttonmenus.match("Quick")->parent = cadwindow;

  QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

  // this is where we create the RCToolBox ojects for any button menus we want to display
  // create the RToolBox object and then populate the initial sub command with it
  if(settings.contains("Layout/MainMenu"))
  {
      cadwindow->buttonmenu(BM_MAINCOMMAND,buttonmenus.match("Entities"));
      cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Line"),buttonmenus.match("Entities"));
      ((Button*)buttonmenus.match(_RCT("Entities"))->match(_RCT("Line0")))->select();
   }
  else
  {
      cadwindow->buttonmenu(BM_MAINCOMMAND,buttonmenus.match("Entities"));
      cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Line"),buttonmenus.match("Entities"));
      ((Button*)buttonmenus.match(_RCT("Entities"))->match(_RCT("Line0")))->select();
      cadwindow->delcontrol(buttonmenus.match("Entities"));
      settings.sync();
      buttonmenus.match("Entities")->getQW()->hide();
  }

  if(settings.contains("Layout/MainInterrupt"))
  {
      cadwindow->buttonmenu(BM_MAININTERRUPT,buttonmenus.match("Assistant"));
      cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Zoom"),buttonmenus.match("Assistant"));
      ((Button*)buttonmenus.match(_RCT("Assistant"))->match(_RCT("Zoom0")))->select();
  }
  else
  {
      cadwindow->buttonmenu(BM_MAININTERRUPT,buttonmenus.match("Assistant"));
      cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Zoom"),buttonmenus.match("Assistant"));
      ((Button*)buttonmenus.match(_RCT("Assistant"))->match(_RCT("Zoom0")))->select();
      cadwindow->delcontrol(buttonmenus.match("Assistant"));
      buttonmenus.match("Assistant")->getQW()->hide();
      settings.sync();
      settings.remove("Layout/MainInterrupt");
  }

  if(settings.contains("Layout/Quick"))
  {
      cadwindow->buttonmenu(BM_QUICK,buttonmenus.match("Quick"));
  }
  else
  {
      cadwindow->buttonmenu(BM_QUICK,buttonmenus.match("Quick"));
      cadwindow->delcontrol(buttonmenus.match("Quick"));
      buttonmenus.match("Quick")->getQW()->hide();
  }

#if 0
  cadwindow->buttonmenu(BM_MAINCOMMAND,buttonmenus.match("Tool"));
  cadwindow->buttonmenu(BM_SUBCOMMAND,buttonmenus.match("Line"));
  cadwindow->buttonmenu(BM_MAININTERRUPT,buttonmenus.match("Interrupt"));
  cadwindow->buttonmenu(BM_SUBINTERRUPT,buttonmenus.match("Zoom"));
  cadwindow->buttonmenu(BM_MAINAPPLICATION,buttonmenus.match("BlankMain"));
  cadwindow->buttonmenu(BM_SUBAPPLICATION,buttonmenus.match("BlankSub"));
  cadwindow->buttonmenu(BM_QUICK,buttonmenus.match("Quick"));
#endif

  /* debug */
  //cadwindow->GetMainCADMenu()->debug();
  //cadwindow->GetSubCADMenu()->debug();

#if 0
  buttonmenus.match("Tool")->mousemoveevent(buttonmenus.match("Tool")->getx()+30,buttonmenus.match("Tool")->gety()+10,1);
  buttonmenus.match("Interrupt")->mousemoveevent(buttonmenus.match("Interrupt")->getx()+30,buttonmenus.match("Interrupt")->gety()+10,1);
  buttonmenus.match("Tool")->mousemoveevent(-1,-1,0);
  buttonmenus.match("Interrupt")->mousemoveevent(-1,-1,0);
#endif

              /*
  GetPrivateProfileString("Layout","MainMenu","",text,300,home.getinifilename());
  if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&mf) == 5 && mf == 1)
    cadwindow->GetMainCADMenu()->mousemoveevent(xm,ym,-1);
  */

  /*
  GetPrivateProfileString("Layout","MainInterrupt","",text,300,home.getinifilename());
  if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&mf) == 5 && mf == 1)
    cadwindow->GetInterruptCADMenu()->mousemoveevent(xm,ym,-1);
  */

   /*
  GetPrivateProfileString("Layout","Quick","",text,300,home.getinifilename());
  if (sscanf(text,"%d,%d,%d,%d,%d",&xm,&ym,&wm,&hm,&mf) == 5 && mf == 1)
    cadwindow->GetQuickCADMenu()->mousemoveevent(xm,ym,-1);
  */

  db.views3d.initialize();
  db.views2d.initialize();
  db.workplanes.initialize();

  rubberband.add(RBxyz,NULL);

  db.setname(rsnc3.getws());
  db.settitle(rsnc3.getws());
  cadwindow->updatetitle();
  cadwindow->sizeevent(-1,-1);
  cadwindow->updatemenu();

  // ToDO: fix custom menus
  if (v.getinteger("cm::active") != 0)
    {  v.setinteger("cm::active",0);
       if (v.getinteger("cm::orientation") == 1)
         custom_command15(&s,0,0);
       else 
         custom_command19(&s,0,0);
    }


  /* KMJ : TODO : Fix this*/
  /*
   // pending
  if (v.getinteger("ar::active") != 0)
  {
       architect_command0(&s,0,0);
       start_architect();
  }
  else
  */
  {
      // get the top view
      if (db.views3d.match(rs0.gets()) != NULL)
         cadwindow->create3dsubwindow(db.views3d.match(rs0.gets()));
  }


  //program->loaddefaultapps();

  // setting up the help menu items
  // KMJ : Fix This

#ifdef USING_WIDGETS
  // old help items
  /*
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_Contents_and_Index);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_Current_command);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addSeparator();

  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_View_RealCAD_Reference_command);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_View_RealCAD_GettingStarted_command);

  if (GetModuleHandle(_RCT("landworks")) != 0)
  {
      ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_View_LANDWorksCAD_Reference_command);
      ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_View_LANDWorksCAD_GettingStarted_command);
  }

  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_View_Training_Videos_command);

  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addSeparator();

  if (GetModuleHandle(_RCT("landworks")) != 0)
      ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_Visit_LANDWorksCAD_Website_command);
  else
      ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_Visit_RealCAD_Website_command);

  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addSeparator();

  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_ProgramUpdates_command);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_TechnicalSupport_command);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_RemoteAssistance_command);
  */

  // new help items
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_Manual);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_CheatSheet);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_GetHelp);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_WelcomeScreen);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addSeparator();
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_ShareMyScreen);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_OnlineTraining);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addSeparator();
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_CheckForUpdates);
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_DeactivateLicense);
  //((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_TellaFriend);
#ifndef _MAC
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addSeparator();
#endif
  ((QtMainWindow*)cadwindow->gethwnd())->menu_Help->addAction(((QtMainWindow*)cadwindow->gethwnd())->action_Help_About);

#else
 RCHMENU hsubmenu = GetSubMenu(hmenu,7);

 position = 3;
 InsertMenu(hsubmenu,position++,MF_BYPOSITION,603,"View RealCAD Reference Manual...");
 InsertMenu(hsubmenu,position++,MF_BYPOSITION,606,"View RealCAD Getting Started Tutorial...");

 if (GetModuleHandle(_RCT("Landworks.DLL")) != 0)
   {  InsertMenu(hsubmenu,position++,MF_BYPOSITION,610,"View Landworks Reference Manual...");
      InsertMenu(hsubmenu,position++,MF_BYPOSITION,611,"View Landworks Getting Started Tutorial...");
   }

 InsertMenu(hsubmenu,position++,MF_BYPOSITION,604,"View Training Videos...");
 InsertMenu(hsubmenu,position++,MF_BYPOSITION | MF_SEPARATOR,-1,"");
 if (GetModuleHandle(_RCT("Landworks.DLL")) != 0)
   InsertMenu(hsubmenu,position++,MF_BYPOSITION,607,"Visit Landworks Web Site...");
 else
   InsertMenu(hsubmenu,position++,MF_BYPOSITION,607,"Visit RealCAD (tm) Web Site...");
 InsertMenu(hsubmenu,position++,MF_BYPOSITION | MF_SEPARATOR,-1,"");
 InsertMenu(hsubmenu,position++,MF_BYPOSITION,608,"Program Updates...");
 InsertMenu(hsubmenu,position++,MF_BYPOSITION,609,"Technical Support");
 InsertMenu(hsubmenu,position++,MF_BYPOSITION | MF_SEPARATOR,-1,""); 
#endif

  // KMJ : TODO : Fix the Cursors
#if ! defined(_WIN32_WCE)
  //RCCursor *cursor = new RCCursor(IDC_ARROW);
  SetCursor(program->getharrowcursor());
  //delete cursor;
#endif

}

LRESULT CALLBACK helpfilter(int ncode,WORD wparam,DWORD lparam)
{RCLPMSG lpmsg = (RCLPMSG)lparam;
#if ! defined(_WIN32_WCE)
  if ((ncode == MSGF_DIALOGBOX || ncode == MSGF_MENU) &&
        lpmsg->message == WM_KEYDOWN && lpmsg->wParam == VK_F1)
  PostMessage(cadwindow->gethwnd(), WM_F1DOWN, ncode, 0L);
//#if defined(_MSC_VER)
  CallNextHookEx((RCHHOOK)program->getpnextfilter(),ncode,wparam,lparam);
//#else
//  DefHookProc(ncode,wparam,lparam,program->getpnextfilter());
//#endif
#endif
  return 0;
}

void help_command2(int *cstate,HighLevelEvent *,void **)
{static int showvideopage = 1;
#if ! defined(_WIN32_WCE)
  if ((cad_version&15) == -1)
    cadwindow->MessageBox("Help not available in demonstration version.","AUSCAD",MB_ICONINFORMATION);
  else if (program->getcommandid() != 0)
    {  program->help(HELP_CONTEXT,(DWORD)program->getcommandid());
       if (showvideopage)
         RunVideo(program->getcommandid());
       showvideopage = 0;
    }
  else
    program->help(HELP_INDEX,0);
//WCEFIX
#endif
  *cstate = ExitState;
}


void help_command3(int *cstate,HighLevelEvent *,void **)
{
   OpenPDF(_RCT("01-RealCAD-Reference-Manual.pdf"));
   *cstate = ExitState;
#if VERSION != REALCAD
  int i;
  if (RegOpenKeyEx(HKEY_CLASSES_ROOT,"http\\shell\\open\\command",0,KEY_READ,&hkey) == ERROR_SUCCESS)
    {  length = 300;
       if (RegQueryValueEx(hkey,0,0,NULL,(LPBYTE)defaultbrowser,&length) == ERROR_SUCCESS)
         {  if (strstr(defaultbrowser,"%1") == 0)
              strcat(defaultbrowser," \"%s\"");
            else
              strstr(defaultbrowser,"%1")[1] = 's';

            SetErrorMode(SEM_NOOPENFILEERRORBOX);
            for (i = 2 ; i < 32 ; i++)
              if ((GetLogicalDrives() & (1 << i)) != 0)
                {FILE *infile;
                 char filename[300];
                   strcpy(filename,"C:\\ShowMe\\TriCAD.HTM");
                   filename[0] = 'A' + i;
                   if ((infile = fopen(filename,"r")) != 0)
                     {  fclose(infile);
                        sprintf(path,defaultbrowser,filename);
                        WinExec(path,SW_NORMAL);
                        break;
                     }
                }
            if (i == 32)
             RCMessageBox(cadwindow->gethwnd(),"The Show Me Tutorials could not be found.  Insert the CD or reinstall the software","Show Me",MB_OK);
       
         }
       RegCloseKey(hkey);
    }
#endif

}

void help_command4(int *cstate,HighLevelEvent *,void **)
{char filename[300];
 FILE *infile;

  infile = fopen(home.getexecutablefilename(_RCT("videos.htm")),"r");
  if (infile != 0)
    {  fclose(infile);
       OpenBrowser(filename);
    }
  else
    {  if (GetModuleHandle(_RCT("Landworks.DLL")) != 0)
         OpenBrowser((char*)"http://www.landworkscad.com/videos.htm");
       else
         OpenBrowser((char*)"http://www.realcad.com/realcad/videos.htm");
    }
  *cstate = ExitState;
}

void help_command5(int *cstate,HighLevelEvent *,void **)
{  //  Dummy function for help on the current command
   *cstate = ExitState;
}


void help_command6(int *cstate,HighLevelEvent *,void **)
{
    OpenPDF(_RCT("02-RealCAD-Getting-Started-Tutorial.pdf"));
   *cstate = ExitState;
}

void help_command10(int *cstate,HighLevelEvent *,void **)
{  OpenPDF(_RCT("03-LANDWorksCAD-v6-Reference-Manual.pdf"));
   *cstate = ExitState;
}

void help_command11(int *cstate,HighLevelEvent *,void **)
{  OpenPDF(_RCT("04-LANDWorksCAD-v6-Getting-Started-Tutorial.pdf"));
   *cstate = ExitState;
}

void help_command7(int *cstate,HighLevelEvent *,void **)
{ if (GetModuleHandle(_RCT("landworks")) != 0)
    OpenBrowser((char*)"http://www.landworkscad.com");
  else
    OpenBrowser((char*)"http://www.realcad.com");
  *cstate = ExitState;
}

void help_command8(int *cstate,HighLevelEvent *,void **)
{
  if (GetModuleHandle(_RCT("landworks")) != 0)
    OpenBrowser((char*)"http://www.landworkscad.com/updates");
  else
    OpenBrowser((char*)"http://www.realcad.com/updates");
  *cstate = ExitState;
}

void help_command9(int *cstate,HighLevelEvent *,void **)
{
  if (GetModuleHandle(_RCT("landworks")) != 0)
    OpenBrowser((char*)"http://www.landworkscad.com/support");
  else
    OpenBrowser((char*)"http://www.realcad.com/support");
  *cstate = ExitState;
}

void help_command12(int *cstate,HighLevelEvent *,void **)
{
   OpenBrowser((char*)"http://www.cadinternational.com/cadinternational/support-answer.php?faq_id=0000015&dis=yes");
   *cstate = ExitState;
}
// new help command functions
void help_Manual_command(int *cstate,HighLevelEvent *,void **)
{
#ifdef _MAC
    if (GetModuleHandle(_RCT("landworks")) != 0)
        OpenPDF(_RCT("LANDWorksCAD-v8-Manual-A4.pdf")); // temporary link
        //OpenBrowser("http://www.realcad.com/landworks/manual");
    else if (GetModuleHandle(_RCT("arborcad")) != 0)
        OpenPDF(_RCT("ArborCAD-v8-Manual-A4.pdf")); // temporary link
        //OpenBrowser("http://www.realcad.com/arborcad/manual");
    else
        OpenPDF(_RCT("RealCAD-v8-Manual-A4.pdf")); // temporary link
        //OpenBrowser("http://www.realcad.com/realcad/manual");
#else
    if (GetModuleHandle(_RCT("landworks")) != 0)
        OpenPDF(_RCT("LANDWorksCAD-v8-Manual-A4.pdf")); // temporary link
    else if (GetModuleHandle(_RCT("arborcad")) != 0)
        OpenPDF(_RCT("ArborCAD-v8-Manual-A4.pdf")); // temporary link
    else
        OpenPDF(_RCT("RealCAD-v8-Manual-A4.pdf")); // temporary link
#endif
    *cstate = ExitState;
}

void help_CheatSheet_command(int *cstate,HighLevelEvent *,void **)
{
    if (GetModuleHandle(_RCT("landworks")) != 0)
        //OpenBrowser("http://www.realcad.com/landworks/manual");
        OpenPDF(_RCT("landworkscad-shortcuts-cheat-sheet.pdf")); // temporary link
    else if (GetModuleHandle(_RCT("arborcad")) != 0)
        //OpenBrowser("http://www.realcad.com/arborcad/manual");
        OpenPDF(_RCT("arborcad-shortcuts-cheat-sheet.pdf")); // temporary link
    else if (GetModuleHandle(_RCT("ecosimacad")) != 0)
        //OpenBrowser("http://www.realcad.com/ecosimacad/manual");
        OpenPDF(_RCT("realcad-shortcuts-cheat-sheet.pdf")); // temporary link
    else
        //OpenBrowser("http://www.realcad.com/realcad/manual");
        OpenPDF(_RCT("realcad-shortcuts-cheat-sheet.pdf")); // temporary link
    *cstate = ExitState;
}

void help_GetHelp_command(int *cstate,HighLevelEvent *,void **)
{
    if (GetModuleHandle(_RCT("landworks")) != 0)
        OpenBrowser((char*)"http://www.realcad.com/landworks/support.htm");
        //OpenBrowser("http://www.landworkscad.com/support"); //  temporary link
    else if (GetModuleHandle(_RCT("arborcad")) != 0)
        OpenBrowser((char*)"http://www.realcad.com/arborcad/support.htm");
        //OpenBrowser("http://http://www.realcad.com/support");//  temporary link
    else if (GetModuleHandle(_RCT("ecosimacad")) != 0)
        OpenBrowser((char*)"http://www.realcad.com/ecosimacad/support.htm");
        //OpenBrowser("http://http://www.realcad.com/support");//  temporary link
    else
        OpenBrowser((char*)"http://www.realcad.com/realcad/support.htm");
        //OpenBrowser("http://http://www.realcad.com/support");//  temporary link
    *cstate = ExitState;
}

void help_ShareMyScreen_command(int *cstate,HighLevelEvent *,void **)
{
    QString command,program,arguments;

#if defined(Q_OS_WIN)
    if(QFileInfo::exists(QString(home.getpublicfilename(_RCT("CAD-International-Remote.exe")))))
        command = QString(home.getpublicfilename(_RCT("CAD-International-Remote.exe")));
    else if(QFileInfo::exists(QString(home.getexecutablefilename(_RCT("CAD-International-Remote.exe")))))
        command = "\"" + QString(home.getexecutablefilename(_RCT("CAD-International-Remote.exe"))) + "\"";
#elif defined(Q_OS_MAC)
    if(QFileInfo::exists(QString(home.getexecutablefilename(_RCT("TeamViewerQS.app")))))
        program = QString(home.getexecutablefilename(_RCT("TeamViewerQS.app")));
    else if(QFileInfo::exists(QString(home.getpublicfilename(_RCT("TeamViewerQS.app")))))
        program = QString(home.getpublicfilename(_RCT("TeamViewerQS.app")));
    command = QString(_RCT("open ")) + program;
#endif

    QProcess *myProcess = new QProcess((QtMainWindow*)cadwindow->gethwnd());
    if( !myProcess->startDetached(command))
        cadwindow->MessageBox(_RCT("Your Remote Assistance executable is missing or corrupted!\nPlease contact CAD International support for further assistance."),_RCT("RealCAD"),MB_ICONEXCLAMATION);

    *cstate = ExitState;
}

void help_OnlineTraining_command(int *cstate,HighLevelEvent *,void **)
{
    OpenBrowser((char*)"http://www.cadinternational.com/cadinternational/training");
    *cstate = ExitState;
}

void help_CheckForUpdates_command(int *cstate,HighLevelEvent *,void **)
{
    if (GetModuleHandle(_RCT("landworks")) != 0)
        OpenBrowser((char*)"http://www.realcad.com/landworks/updates.htm");
        //OpenBrowser("http://www.landworkscad.com/updates"); // temporary link
    else if (GetModuleHandle(_RCT("arborcad")) != 0)
        OpenBrowser((char*)"http://www.realcad.com/arborcad/updates.htm");
        //OpenBrowser("http://www.realcad.com/support"); // temporary link
    else if (GetModuleHandle(_RCT("ecosimacad")) != 0)
        OpenBrowser((char*)"http://www.realcad.com/ecosimacad/updates.htm");
        //OpenBrowser("http://www.realcad.com/support"); // temporary link
    else
        OpenBrowser((char*)"http://www.realcad.com/realcad/updates.htm");
        //OpenBrowser("http://www.realcad.com/support"); // temporary link
    *cstate = ExitState;
}

void help_DeactivateLicense_command(int *cstate,HighLevelEvent *,void **)
{
    deactivateLicense();
    *cstate = ExitState;
}

void help_TellaFriend_command(int *cstate,HighLevelEvent *,void **)
{
    if (GetModuleHandle(_RCT("landworks")) != 0)
        OpenBrowser((char*)"http://www.realcad.com/landworks/tellafriend.htm");
    else if (GetModuleHandle(_RCT("arborcad")) != 0)
        OpenBrowser((char*)"http://www.realcad.com/arborcad/tellafriend.htm");
    else if (GetModuleHandle(_RCT("ecosimacad")) != 0)
        OpenBrowser((char*)"http://www.realcad.com/ecosimacad/tellafriend.htm");
    else
        OpenBrowser((char*)"http://www.realcad.com/realcad/tellafriend.htm");
    *cstate = ExitState;
}

// these are all the commands
void CreateCommandList(void)
{
  commands.add(1,10000,0,file_command0,5160); // file tools
  commands.add(1,10017,0,file_command17,NCMAIN+42);
  //commands.add(1,100,3,file_command1,NCMAIN+38);
  commands.add(1,100,3,file_command1,5161);
  commands.add(1,101,3,file_command2,5163);
  //commands.add(1,101,3,file_command2,NCMAIN+39);
  //commands.add(1,102,0,file_command3,NCMAIN+40);
  commands.add(1,102,0,file_command3,5164);
  //commands.add(1,103,0,file_command4,NCMAIN+41);
  commands.add(1,103,0,file_command4,5165);
  commands.add(1,126,0,file_command20,NCMAIN+287);
  commands.add(1,104,0,file_command5,NCMAIN+42);
  commands.add(1,105,0,file_command6,NCMAIN+43);
  commands.add(1,106,1,command_command,NCMAIN+257);
  commands.add(1,107,0,file_command7,NCMAIN+44);
  commands.add(1,108,0,file_command8,NCMAIN+45);
  commands.add(1,109,0,file_command9,NCMAIN+45);
  commands.add(1,110,0,file_command10,NCMAIN+45);
  commands.add(1,111,0,file_command11,NCMAIN+45);
  commands.add(1,112,0,file_command12,NCMAIN+45);
  commands.add(1,113,0,file_command13,NCMAIN+45);
  commands.add(1,114,0,file_command14,NCMAIN+45);
  commands.add(1,115,0,file_command15,NCMAIN+45);
  commands.add(1,116,0,file_command16,NCMAIN+45);
  //commands.add(1,117,0,file_newfromtemplate,NCMAINB+93);
  commands.add(1,117,0,file_newfromtemplate,5162);
  //commands.add(1,10018,0,file_toolscommand,NCMAINB+121);
  commands.add(1,10018,0,file_toolscommand,5174,999999);
  //commands.add(1,10019,0,file_emailcommand,NCMAINB+122);
  commands.add(1,10019,0,file_emailcommand,5177,999999);
  commands.add(1,10020,0,file_3DWarehouse,5180);

  //commands.add(1,120,0,printpreview_command1,NCMAINB+45);
  commands.add(1,120,0,printpreview_command1,5166);
  //commands.add(1,121,0,printpreview_command2,NCMAINB+46);
  commands.add(1,121,0,printpreview_command2,5167);
  commands.add(1,122,0,printpreview_command3,NCMAINB+47);
  commands.add(1,123,0,printpreview_command4,NCMAINB+48);
  commands.add(1,124,0,printpreview_command5,NCMAINB+49);
  commands.add(1,125,0,printpreview_command6,NCMAINB+50);
  commands.add(1,200,0,undo_command,NCMAIN+46);
  commands.add(1,201,0,redo_command,NCMAIN+47);
  commands.add(1,202,0,cut_command,NCMAIN+48);
  commands.add(1,203,0,copy_command,NCMAIN+49);
  commands.add(1,204,0,paste_command,NCMAIN+50);
  commands.add(1,205,0,delete_command,NCMAIN+51);
  commands.add(1,206,0,attribute_command1,NCMAIN+52);
  commands.add(1,207,0,attribute_command2,NCMAIN+53);
  commands.add(1,208,0,attribute_command3,NCMAIN+54);
  commands.add(1,209,0,attribute_command4,NCMAIN+55);
  commands.add(1,210,0,copymeta_command,NCMAINB+30);
  commands.add(1,211,0,findandreplace_command,NCMAINB+6);
  commands.add(1,215,0,findinfiles_command,5172);
  //commands.add(1,212,0,editinfo_command,NCMAINB+9);
  commands.add(1,212,0,editinfo_command,5173);
  commands.add(1,213,0,copymetawindow_command,NCMAINB+29);
  commands.add(1,214,0,copybitmap_command,NCMAINB+51);
  commands.add(1,216,0,clone_command,NCMAINB+80);
  commands.add(1,222,0,copybitmap_toclipboard_command,NCMAINB+80);
  commands.add(1,223,0,copybitmapwindow_toclipboard_command,NCMAINB+80);
#ifdef USING_WIDGETS
  commands.add(1,217,0,clone_colour_command,NCCLONING);
  commands.add(1,218,0,clone_layer_command,NCCLONING+1);
  commands.add(1,219,0,clone_style_command,NCCLONING+2);
  commands.add(1,220,0,clone_weight_command,NCCLONING+3);
  commands.add(1,221,0,define_pattern_line_style_command,NCCLONING+4);
  commands.add(1,224,0,clone_all_command,NCCLONING+7);
  commands.add(1,225,0,clone_to_default_colour_command,NCCLONING);
  commands.add(1,226,0,clone_to_default_layer_command,NCCLONING+1);
  commands.add(1,227,0,clone_to_default_style_command,NCCLONING+2);
  commands.add(1,228,0,clone_to_default_weight_command,NCCLONING+3);
  commands.add(1,229,0,clone_all_to_default_command,NCCLONING+7);
#endif
  commands.add(1,320,0,transformmenu_command,NCMAINB+74);
  commands.add(1,300,0,stretch_command,NCMAIN+204);
  commands.add(1,301,0,translate_command,NCMAIN+56);
  commands.add(1,302,0,project_command,NCMAIN+57);
  commands.add(1,303,0,scale_command,NCMAIN+58);
  commands.add(1,304,0,mirror_command,NCMAIN+59);
  commands.add(1,305,0,rotate_command,NCMAIN+60);
  commands.add(1,306,0,move_command,NCMAIN+90);
  commands.add(1,307,0,copyarray_command,NCMAINB+69);
  commands.add(1,308,0,copyalong_command,NCMAINB+71);
  commands.add(1,400,1,grid_command2,NCMAIN+61);
  commands.add(1,401,1,grid_command3,NCMAIN+62);
  commands.add(1,430,1,workplanemenu_command,NCMAINB+76);
  commands.add(1,402,1,workplane_command1,NCMAIN+63);
  commands.add(1,403,1,workplane_command2,NCMAIN+64);
  commands.add(1,404,1,workplane_command3,NCMAIN+65);
  commands.add(1,405,1,workplane_command4,NCMAIN+66);
  commands.add(1,406,1,workplane_command5,NCMAIN+67);
  commands.add(1,407,1,workplane_command6,NCMAIN+68);
  commands.add(1,425,1,workplane_command7,NCMAINB+90);
#if VERSION != FLEXICAD
  commands.add(1,408,1,line_command9,NCMAIN+289);
#endif
  commands.add(1,409,1,plane_command2,NCMAIN+69);
  commands.add(1,410,1,ruledsurface_command3,NCMAIN+70);
  commands.add(1,411,1,surfaceofrev_command3,NCMAIN+71);
  commands.add(1,412,1,text_command2,NCMAIN+72);
  commands.add(1,413,1,figure_command4,NCMAIN+73);
#if VERSION != FLEXICAD
  commands.add(1,428,1,bitmap_command3,NCMAIN+293);
#endif
  commands.add(1,4000,1,group_command0,5151);
  commands.add(1,414,1,group_command5,NCMAIN+292);
  commands.add(1,415,1,dimen_command8,NCMAIN+74);
  commands.add(1,416,1,grid_command1,NCMAIN+75);
  //commands.add(1,417,1,system_command1,NCMAIN+76);
  commands.add(1,417,1,system_command1,5176);
  commands.add(1,418,0,animation_command, (char*)"");
#if VERSION != FLEXICAD
  commands.add(1,22000,0,simulation_command, (char*)"");
#endif
  commands.add(1,419,0,patch_command4,NCMAIN+236);
  commands.add(1,420,1,grid_command4,NCMAIN+255);
  commands.add(1,421,1,grid_command5,NCMAIN+256);
  commands.add(1,422,1,system_command2,NCMAINB+5);
  commands.add(1,424,1,leader_command,NCMAINB+83);
  commands.add(1,426,0,patch_compare_command,NCMAIN+236);
#if VERSION != FLEXICAD
  commands.add(1,423,0,nc_command, (char*)"");
#endif
#if VERSION != FLEXICAD
  commands.add(1,21000,0,architect_command0, (char*)"");
#endif
  commands.add(1,500,1,window_command1,NCMAIN+77);
  commands.add(1,5001,1,window_command11,NCMAIN+77); // new window replace command
  commands.add(1,501,1,window_command6,NCMAIN+213);
  commands.add(1,502,1,window_command2,NCMAIN+78);
  commands.add(1,503,1,window_command3,NCMAIN+79);
  commands.add(1,504,1,window_command4,NCMAIN+205);
  commands.add(1,505,1,window_command5,NCMAIN+206);
  commands.add(1,506,1,view_command1,NCMAIN+80);
  commands.add(1,507,1,view_command18,NCMAIN+107);
  commands.add(1,508,1,view_command2,NCMAIN+81);
  commands.add(1,509,1,view_command3,NCMAIN+82);
  commands.add(1,510,1,view_command4,NCMAIN+83);
  commands.add(1,511,1,view_command5,NCMAIN+84);
  commands.add(1,512,1,view_command6,NCMAIN+85);
  commands.add(1,513,1,view_command7,NCMAIN+86);
  commands.add(1,514,1,view_command8,NCMAIN+87);
  commands.add(1,515,1,view_command9,NCMAIN+88);
  commands.add(1,516,1,view_command10,NCMAIN+89);
  commands.add(1,517,1,view_command11,NCMAIN+100);
  commands.add(1,518,1,view_command12,NCMAIN+101);
  commands.add(1,555,1,view_command33,NCMAIN+217,999999);
  commands.add(1,519,1,view_command13,NCMAIN+102,999999);
  commands.add(1,520,1,view_command14,NCMAIN+103,999999);
  commands.add(1,521,1,view_command15,NCMAIN+104);
  commands.add(1,522,1,view_command16,NCMAIN+105);
  commands.add(1,523,1,view_command17,NCMAIN+106);
  commands.add(1,541,0,view_command19,NCMAIN+240,999999);
  commands.add(1,542,0,view_command20,NCMAIN+241);
  commands.add(1,543,0,view_command21,NCMAIN+242);
  commands.add(1,544,0,view_command22,NCMAIN+243);
  commands.add(1,545,0,view_command23,NCMAIN+244);
  commands.add(1,546,0,view_command24,NCMAIN+245);
  commands.add(1,547,0,view_command25,NCMAIN+246);
  commands.add(1,548,0,view_command26,NCMAIN+247);
  commands.add(1,549,0,view_command27,NCMAIN+248);
  commands.add(1,550,0,view_command28,NCMAIN+249);
  commands.add(1,551,0,view_command29,NCMAIN+250);
  commands.add(1,552,0,view_command30,NCMAIN+251);
  commands.add(1,553,0,view_command31,NCMAIN+252);
  commands.add(1,554,0,view_command32,NCMAIN+253);
  commands.add(0,527,0,view_command34,NCMAIN+294);
  commands.add(0,530,0,view_command35,NCMAIN+295);
  commands.add(0,556,0,view_command36,NCMAIN+296);
  commands.add(1,583,1,view_command10,NCMAIN+89,999999);
  commands.add(0,572,0,view_switch_screens_command,NCMAINB+103);
  commands.add(0,581,0,view_command41,NCMAINB+91);
  commands.add(1,580,0,dynamicviewmenu_command,NCMAINB+75);
  commands.add(0,557,0,layerwindow_command,NCMAINB+38);
  commands.add(0,563,0,compose_command0,NCMAINB+77);
  commands.add(0,571,0,renamedrawing_command,5155);
  commands.add(0,564,0,view_command0,NCMAINB+78);
  commands.add(0,565,0,render_command0,NCMAINB+79);
  commands.add(0,15000,0,view_command37,NCMAIN+296);
  commands.add(0,558,0,view_command38,NCMAINB+44);
  commands.add(0,559,0,view_command39,NCMAINB+52);
  commands.add(0,561,0,view_command40,NCMAINB+56);
  commands.add(0,566,0,view_command42,NCMAINB+95);
  commands.add(0,567,0,view_command43,5154); // copy layout
// new view commands
  commands.add(0,584,0,view_command44,5120);
  commands.add(0,585,0,view_command45,5121,999999);
  commands.add(0,586,0,view_command46,5122,999999);
  commands.add(0,587,0,view_command47,5152); // set view to previous
  commands.add(0,588,0,view_command48,5156); // toggle full screen
//
  commands.add(1,560,1,window_command10,NCMAIN+79);
  commands.add(1,524,1,window_command7,NCMAIN+214);
  commands.add(1,525,1,window_command8,NCMAIN+215);
  commands.add(1,526,1,window_command9,NCMAIN+216);
  commands.add(1,528,0,shade_command1,NCMAIN+108);
  commands.add(1,529,0,hide_command1,NCMAIN+109);
  commands.add(1,569,0,hide_command2,5009);
// kmj view style commands
  commands.add(1,5100,0,viewstyle_command0,5100);  // start command
  commands.add(1,5101,0,viewstyle_command1,5101);  // wireframe
  commands.add(1,5102,0,viewstyle_command2,5102);  // hidden line removal
  commands.add(1,5103,0,viewstyle_command3,5103);  // dashed hidden line
  commands.add(1,5104,0,viewstyle_command4,5104);  // shaded surfaces
  commands.add(1,5105,0,viewstyle_command5,5105);  // shaded surfaces with lines
  commands.add(1,5106,0,viewstyle_command6,5106);  // shaded dashed lines
  commands.add(1,5107,0,viewstyle_command7,5107);  // texture shaded
  commands.add(1,5108,0,viewstyle_command8,5108);  // texture shaded with lines
  commands.add(1,5109,0,viewstyle_command9,5109,999999);  // VR 3d (stereo)
  commands.add(1,5110,0,viewstyle_command10,5110); // view as perspective
  commands.add(1,5111,0,viewstyle_command11,5111); // view as parallel
  commands.add(1,5112,0,viewstyle_command12,5112); // shade options
// KMJ capture commands
  commands.add(1,5130,0,capture_command0,5130);  // capture scene and image capture tools
  commands.add(1,5131,0,capture_command1,5131);  // capture view
  commands.add(1,5132,0,capture_command2,5132);  // capture area
  commands.add(1,5133,0,capture_command3,5133);  // capture bitmap
  commands.add(1,5134,0,capture_command4,5134);  // raytrace render
  commands.add(1,5135,0,capture_command5,5135,999999);  // camera path
  commands.add(1,5136,0,capture_command6,5136,999999);  // reference path
  commands.add(1,5137,0,capture_command7,5137,999999);  // object path
  commands.add(1,5138,0,capture_command8,5138,999999);  // patch path
  commands.add(1,5139,0,capture_command9,5139,999999);  // record animation
  commands.add(1,5140,0,capture_command10,5140,999999); // replay animation
//
  commands.add(1,531,0,toolmenu_command,NCMAIN+110);  
  commands.add(1,568,0,showproperties_command,NCMAINB+101);  
  commands.add(1,532,0,interruptmenu_command,NCMAIN+111);
  commands.add(1,533,0,quickmenu_command,NCMAIN+112);
  commands.add(1,534,0,prompt_command,NCMAIN+113);
  commands.add(1,570,0,applicationmenu_command,NCMAIN+113);
  commands.add(1,535,0,zone_command1,NCMAIN+224);
  commands.add(1,536,0,zone_command2,NCMAIN+225);
  commands.add(1,537,0,raytrace_command1,NCMAIN+235);// add a raytrace light
  commands.add(1,538,0,raytrace_command2,NCMAIN+227);// light setup/verify dialog
  commands.add(1,539,0,raytrace_command3,NCMAIN+228);// Add/Change material parameters
  commands.add(1,540,0,raytrace_command4,NCMAIN+218);// execute raytracing
  commands.add(1,562,0,raytrace_command5,NCMAINB+61);// ??? dont know what this is yet
  commands.add(1,582,0,insertlightatsunposition_command,NCMAINB+94);
  // old help commands
  commands.add(1,600,0,help_command1,NCMAIN+114);
  commands.add(1,601,0,about_command,NCMAIN+115);
  commands.add(1,602,0,help_command2,NCMAIN+114);
  commands.add(1,603,0,help_command3,NCMAIN+114);
  commands.add(1,604,0,help_command4,NCMAIN+114);
  commands.add(1,605,0,help_command5,NCMAIN+114);
  //commands.add(1,606,0,help_command6,NCMAIN+114);
  //commands.add(1,607,0,help_command7,NCMAIN+114);
  //commands.add(1,608,0,help_command8,NCMAIN+114);
  //commands.add(1,609,0,help_command9,NCMAIN+114);
  //commands.add(1,610,0,help_command10,NCMAIN+114);
  //commands.add(1,611,0,help_command11,NCMAIN+114);
  //commands.add(1,612,0,help_command12,NCMAIN+114);
  // new help commands
  commands.add(1,606,0,help_Manual_command,NCMAIN+114);
  commands.add(1,607,0,help_CheatSheet_command,NCMAIN+114);
  commands.add(1,608,0,help_GetHelp_command,NCMAIN+114);
  commands.add(1,609,0,help_ShareMyScreen_command,NCMAIN+114);
  commands.add(1,610,0,help_CheckForUpdates_command,NCMAIN+114);
  commands.add(1,611,0,help_DeactivateLicense_command,NCMAIN+114);
  commands.add(1,612,0,help_TellaFriend_command,NCMAIN+114);
  commands.add(1,613,0,help_OnlineTraining_command,NCMAIN+114);

  commands.add(1,700,0,dxfout_command,NCMAIN+230);
  commands.add(1,701,0,igesout_command,NCMAINB+59);
  commands.add(1,702,0,hpglout_command,NCMAINB+60);
  commands.add(1,703,0,strokedigesout_command,NCMAINB+63);
  commands.add(1,704,0,dwgout_command,NCMAINB+73);
  commands.add(1,750,0,dxfin_command,NCMAIN+223);
#if VERSION != FLEXICAD
  commands.add(1,751,0,igesin_command,NCMAIN+285);
#endif
#if VERSION != FLEXICAD
  commands.add(1,752,0,hpglin_command,NCMAIN+286);
  commands.add(1,754,0,dwgin_command,NCMAINB+72);
  commands.add(1,797,0,medusain_command,NCMAINB+55);
#endif
  commands.add(1,753,0,metafilein_command,NCMAIN+229);
  commands.add(1,755,0,skpfilein_command,5179);
  commands.add(1,8000,0,arrange_command0,5153);
  commands.add(1,800,0,arrange_command1,NCMAIN+231);
  commands.add(1,801,0,arrange_command2,NCMAIN+232);
  commands.add(1,802,0,arrange_command3,NCMAIN+233);
  commands.add(1,803,0,arrange_command4,NCMAIN+234);
#if VERSION != FLEXICAD
  commands.add(1,900,4,custom_command1,NCMAINB+1);
  commands.add(1,901,0,custom_command2,NCMAINB+12);
  commands.add(1,902,0,custom_command3,NCMAINB+13);
  commands.add(1,903,0,custom_command4,NCMAINB+14);
  commands.add(1,904,0,custom_command5,NCMAINB+15);
  commands.add(1,905,0,custom_command6,NCMAINB+16);
  commands.add(1,906,0,custom_command7,NCMAINB+17);
  commands.add(1,907,0,custom_command8,NCMAINB+18);
  commands.add(1,908,0,custom_command9,NCMAINB+19);
  commands.add(1,909,0,custom_command10,NCMAINB+20);
  commands.add(1,910,0,custom_command11,NCMAINB+21);
  commands.add(1,910,0,custom_command11,NCMAINB+21);
  commands.add(1,911,0,custom_command12,NCMAINB+26);
  commands.add(1,912,0,custom_command15,NCMAINB+33);
  commands.add(1,918,0,custom_command19,NCMAINB+33);
  commands.add(1,913,4,custom_command13,NCMAINB+34);
  commands.add(1,914,4,custom_command14,NCMAINB+35);
  commands.add(1,915,0,custom_command16,NCMAINB+37);
  commands.add(1,916,0,custom_command18,NCMAINB+64);
  commands.add(1,917,4,keyboard_command,NCMAINB+88);
#endif
  commands.add(1,1000,0,point_command0,NCMAIN+116);
  commands.add(1,1001,0,point_command1,NCMAIN+117);
  commands.add(1,1002,0,point_command2,NCMAIN+118);
  commands.add(1,1100,1,line_command0,NCMAIN+119);
  commands.add(1,1101,0,line_command1,NCMAIN+120);
  commands.add(1,1102,0,line_command2,NCMAIN+121);
  commands.add(1,1103,0,line_command3,NCMAIN+122);
  commands.add(1,1104,0,line_command4,NCMAIN+123);
  commands.add(1,1105,0,line_command5,NCMAIN+124);
  commands.add(1,1106,0,line_command6,NCMAIN+125);
  commands.add(1,1107,0,line_command7,NCMAIN+126);
  commands.add(1,1108,0,line_command8,NCMAIN+203);
  commands.add(1,1200,1,circle_command0,NCMAIN+127);
  commands.add(1,1201,0,circle_command1,NCMAIN+128);
  commands.add(1,1202,0,circle_command2,NCMAIN+129);
  commands.add(1,1203,0,circle_command3,NCMAIN+130);
  commands.add(1,1204,0,circle_command4,NCMAIN+226);
  commands.add(1,1205,0,ellipse_command1,NCMAIN+197);
  commands.add(1,1206,0,ellipse_command2,NCMAIN+198);
  commands.add(1,1300,1,arc_command0,NCMAIN+131);
  commands.add(1,1301,0,arc_command1,NCMAIN+132);
  commands.add(1,1302,0,arc_command2,NCMAIN+133);
  commands.add(1,1303,0,arc_command3,NCMAIN+134);
  commands.add(1,1304,0,arc_command4,NCMAIN+135);
  commands.add(1,1400,1,curve_command0,NCMAIN+136);
  commands.add(1,1401,0,curve_command1,NCMAIN+137);
  commands.add(1,1402,0,curve_command2,NCMAIN+138);
  commands.add(1,1403,0,curve_command3,NCMAIN+202);
  commands.add(1,1404,0,curve_command4,NCMAIN+297);
  commands.add(1,1405,0,curve_command5,NCMAIN+298);
  commands.add(1,1406,0,curve_command6,5194/*NCMAINB+2*/);
  commands.add(1,2800,1,solid_command0,NCMAIN+139);
  commands.add(1,1500,1,surface_command0,NCMAIN+139);
  commands.add(1,3500,1,new_surface_command0,NCMAIN+139);
  commands.add(1,2700,1,plane_command0,5150); // new tool tip number
  commands.add(1,1501,0,plane_command1,NCMAIN+140);
  commands.add(1,1502,0,ruledsurface_command1,NCMAIN+141);
  commands.add(1,1503,0,ruledsurface_command2,NCMAIN+142);
  commands.add(1,1504,0,surfaceofrev_command1,NCMAIN+143);
  commands.add(1,1505,0,surfaceofrev_command2,NCMAIN+144);
  commands.add(1,1506,0,patch_command1,NCMAIN+237);
  commands.add(1,1507,0,patch_command2,NCMAIN+238);
  commands.add(1,1508,0,patch_command3,NCMAIN+239);
  commands.add(1,1509,0,plane_command3,NCMAIN+284);
  commands.add(1,1510,0,plane_command4,NCMAINB+57);
  commands.add(1,1511,0,plane_command5,NCMAINB+102);
  commands.add(1,1512,0,plane_command6,NCMAINB+97);
  commands.add(1,1513,0,plane_command7,5182);// Enhanced tools Add Vertex
  commands.add(1,1514,0,plane_command8,5183);// Enhanced tools Delete Vertex
  commands.add(1,1515,0,plane_command9,5184);// Enhanced tools Move Vertex
  commands.add(1,1516,0,plane_command10,5185);// Enhanced tools Copy Plane Boundary
  commands.add(1,1517,0,plane_command11,5186);// Enhanced tools Detach Entities
  commands.add(1,1518,0,plane_command12,5187);// Enhanced tools White Out
  commands.add(1,1519,0,flatpattern_command,5192);// Flat Pattern from ruled surface
  commands.add(1,1520,0,insertspiral_command,5193);// Insert Spiral surface
  commands.add(1,1521,0,plane_command13,5195);// Insert Spiral surface
#if defined(CADAUSTRALIAVERSION)
  commands.add(1,16000,1,new_text_command0,NCMAIN+145);
  commands.add(1,1600,1,text_command0,NCMAINB+82);
#else
  commands.add(1,1600,1,text_command0,NCMAIN+145);
#endif
  commands.add(1,1601,0,text_command1,NCMAIN+146);
  commands.add(1,1602,0,text_command3,NCMAIN+147);
  commands.add(1,1603,0,text_command4,NCMAINB+10);
  commands.add(1,1604,0,text_command5,NCMAINB+11);
  commands.add(1,1605,0,text_command6,NCMAINB+65);
  commands.add(1,1606,0,text_command7,NCMAINB+67);
#if defined(CADAUSTRALIAVERSION)
  commands.add(1,1700,1,figure_command0,NCMAINB+81);
  commands.add(1,17000,1,new_figure_command0,NCMAINB+81);
#else
  commands.add(1,1700,1,figure_command0,NCMAIN+148);
#endif
  commands.add(1,1701,0,figure_command1,NCMAIN+149);
  commands.add(1,1702,0,figure_command2,NCMAIN+150);
  commands.add(1,1703,0,figure_command3,NCMAIN+151);
  commands.add(1,1710,1,figure_command5,NCMAINB+54);
  commands.add(1,1711,0,figure_command6,NCMAINB+66);
  commands.add(1,1704,0,group_command1,NCMAIN+152);
  commands.add(1,1705,0,group_command2,NCMAIN+153);
  commands.add(1,1706,0,group_command3,NCMAIN+154);
  commands.add(1,1707,0,group_command4,NCMAIN+155);
  commands.add(1,1712,0,panel_command_insert,NCMAINB+100);
  commands.add(1,1713,0,panel_command_delete_vertex,NCMAINB+100);
  commands.add(1,1714,0,panel_command_insert_vertex,NCMAINB+100);
  commands.add(1,1715,0,panel_command_insert_hole,NCMAINB+100);
  commands.add(1,1716,0,panel_command_delete_hole,NCMAINB+100);
  commands.add(1,1717,0,panel_command_align,NCMAINB+100);
  commands.add(1,1718,0,panel_command_thickness,NCMAINB+100);
  commands.add(1,1719,0,panel_command_renumber,NCMAINB+100);
  commands.add(1,1720,0,panel_command_layout,NCMAINB+100);
  commands.add(1,1721,0,panel_command_export_csv,NCMAINB+100);
#if VERSION != FLEXICAD
  commands.add(1,2900,0,bitmap_command0,NCMAINB+82);
  commands.add(1,1708,0,bitmap_command1,NCMAIN+290);
  commands.add(1,1709,0,bitmap_command2,NCMAIN+291);
#endif
  commands.add(1,1800,1,dimen_command0,NCMAIN+156);
  commands.add(1,1801,0,dimen_command1,NCMAIN+157);
  commands.add(1,1802,0,dimen_command2,NCMAIN+158);
  commands.add(1,1803,0,dimen_command3,NCMAIN+159);
  commands.add(1,1804,0,dimen_command4,NCMAIN+207);
  commands.add(1,1805,0,dimen_command5,NCMAIN+208);
  commands.add(1,1806,0,dimen_command6,NCMAIN+209);
  commands.add(1,1807,0,dimen_command7,NCMAIN+210);
  commands.add(1,1808,0,dimen_command9,NCMAINB+36);
  commands.add(1,1809,0,dimen_command10,NCMAINB+40);
  commands.add(1,1810,0,dimen_command11,NCMAINB+41);
  commands.add(1,1811,0,dimen_command12,NCMAINB+58);
  commands.add(1,1812,0,dimen_addpoint,NCMAINB+58);
  commands.add(1,1813,0,dimen_delpoint,NCMAINB+58);
  commands.add(1,1814,0,dimen_movepoint,NCMAINB+58);
#if defined(CADAUSTRALIAVERSION)
  commands.add(1,1900,1,trim_command0,NCMAINB+87);
#else
  commands.add(1,1900,1,trim_command0,NCMAIN+160);
#endif
  commands.add(1,1901,0,trim_command1,NCMAIN+161);
  commands.add(1,1902,0,trim_command2,NCMAIN+162);
  commands.add(1,1903,0,trim_command3,NCMAIN+163);
  commands.add(1,1904,0,trim_command4,NCMAIN+164);
  commands.add(1,1905,0,trim_command5,NCMAIN+165);
  commands.add(1,1908,0,trim_command8,NCMAIN+199);
  commands.add(1,1906,0,trim_command6,NCMAIN+166);
  commands.add(1,1907,0,trim_command7,NCMAIN+167);
  commands.add(1,1909,0,trim_command9,NCMAIN+288);
  commands.add(1,1910,0,trim_command10,NCMAINB+42);
  commands.add(1,1911,0,trim_command11,NCMAINB+43);
  commands.add(1,1950,1,measure_command0,NCMAIN+219);
  commands.add(1,1951,0,measure_command1,NCMAIN+220);
  commands.add(1,1952,0,measure_command2,NCMAIN+221);
  commands.add(1,1953,0,measure_command3,NCMAIN+222);
  commands.add(1,1954,0,measure_command4,NCMAIN+219);
  commands.add(1,2000,1,position_command0,NCMAIN+168);
  commands.add(1,2001,6,position_command1,NCMAIN+169);
  commands.add(1,2002,6,position_command2,NCMAIN+170);
  commands.add(1,2003,6,position_command3,NCMAIN+171);
  commands.add(1,2004,6,position_command4,NCMAIN+172);
  commands.add(1,2005,6,position_command5,NCMAIN+211);
  commands.add(1,2006,5,position_command6,NCMAIN+299);
  commands.add(1,2007,6,position_command7,NCMAINB+23);
  commands.add(1,2008,6,position_command8,NCMAINB+70);
  commands.add(1,2009,6,position_command9,NCMAINB+84);
  commands.add(1,2010,5,position_command10,NCMAINB+85);
  commands.add(1,2011,6,position_command11,NCMAINB+98);
  commands.add(1,2012,2,selectall_command,5181);
  commands.add(1,2100,1,zoom_command0,NCMAIN+173);
  commands.add(1,2101,1,zoom_command1,NCMAIN+174);
  commands.add(1,2102,1,zoom_command2,NCMAIN+175);
  commands.add(1,2103,1,zoom_command3,NCMAIN+176);
  commands.add(1,2104,1,zoom_command4,NCMAIN+177);
  commands.add(1,2105,1,zoom_command5,NCMAIN+178);
  commands.add(1,2106,1,zoom_command6,NCMAIN+179);
  commands.add(1,2107,1,zoom_command7,NCMAIN+200);
  commands.add(1,2108,1,zone_command3,NCMAIN+254);
  commands.add(1,2109,1,zoom_command8,NCMAINB+22);
  commands.add(1,2110,1,zoom_command9,NCMAINB+24);
  commands.add(1,2200,1,paint_command0,NCMAIN+180);
  // new mask commands
  commands.add(1,23100,1,mask_command100,NCMAIN+181);
  commands.add(1,23101,2,mask_command101,NCMAIN+182);
  commands.add(1,23102,2,mask_command102,NCMAIN+183);
  commands.add(1,23103,2,mask_command103,NCMAIN+184);
  commands.add(1,23104,2,mask_command104,NCMAIN+185);
  commands.add(1,23105,2,mask_command105,NCMAIN+186);
  commands.add(1,24100,1,mask_command106,NCMAIN+187);
  commands.add(1,24101,2,mask_command107,NCMAIN+188);
  commands.add(1,24102,2,mask_command108,NCMAIN+189);
  commands.add(1,24103,2,mask_command109,NCMAIN+190);
  commands.add(1,24104,2,mask_command110,NCMAIN+191);
  commands.add(1,24105,2,mask_command111,NCMAIN+192);
  // old mask commands
  commands.add(1,2300,1,mask_command0,NCMAIN+181);
  commands.add(1,2301,2,mask_command1,NCMAIN+182);
  commands.add(1,2302,2,mask_command2,NCMAIN+183);
  commands.add(1,2303,2,mask_command3,NCMAIN+184);
  commands.add(1,2304,2,mask_command4,NCMAIN+185);
  commands.add(1,2305,2,mask_command5,NCMAIN+186);
  commands.add(1,2400,1,mask_command6,NCMAIN+187);
  commands.add(1,2401,2,mask_command7,NCMAIN+188);
  commands.add(1,2402,2,mask_command8,NCMAIN+189);
  commands.add(1,2403,2,mask_command9,NCMAIN+190);
  commands.add(1,2404,2,mask_command10,NCMAIN+191);
  commands.add(1,2405,2,mask_command11,NCMAIN+192);

  commands.add(1,2406,2,mask_snaps_command,NCMAINB+96);

  commands.add(1,2500,2,createwindow_command0,NCMAIN+258);
  commands.add(1,2501,2,createwindow_command1,NCMAIN+259);
  commands.add(1,2502,2,createwindow_command1,NCMAIN+260);
  commands.add(1,2503,2,createwindow_command1,NCMAIN+261);
  commands.add(1,2504,2,createwindow_command1,NCMAIN+262);
  commands.add(1,2505,2,createwindow_command1,NCMAIN+263);
  commands.add(1,2506,2,createwindow_command1,NCMAIN+264);
  commands.add(1,2507,2,createwindow_command1,NCMAIN+265);
  commands.add(1,2508,2,createwindow_command1,NCMAIN+266);
  commands.add(1,2509,2,createwindow_command1,NCMAIN+267);
  commands.add(1,2600,2,resetwindow_command0,NCMAIN+268);
  commands.add(1,2601,2,resetwindow_command1,NCMAIN+269);
  commands.add(1,2602,2,resetwindow_command1,NCMAIN+270);
  commands.add(1,2603,2,resetwindow_command1,NCMAIN+271);
  commands.add(1,2604,2,resetwindow_command1,NCMAIN+272);
  commands.add(1,2605,2,resetwindow_command1,NCMAIN+273);
  commands.add(1,2606,2,resetwindow_command1,NCMAIN+274);
  commands.add(1,2607,2,resetwindow_command1,NCMAIN+275);
  commands.add(1,2608,2,resetwindow_command1,NCMAIN+276);
  commands.add(1,2609,2,resetwindow_command1,NCMAIN+277);
  commands.add(1,2610,2,resetwindow_command1,5178); // top perspective

  // KMJ : new property commands
  commands.add(1,3100,1,colour_commandEx,NCMAIN+193);
  commands.add(1,3101,1,layer_commandEx,NCMAIN+194);
  commands.add(1,3102,1,style_commandEx,NCMAIN+195);
  commands.add(1,3103,1,weight_commandEx,NCMAIN+196);
  commands.add(1,3104,2,entity_commandEx,NCMAIN+4004);
  commands.add(1,3105,2,general_property_command,NCMAIN+4005);
  commands.add(1,3106,1,background_colour_command,NCMAIN+4006);
  commands.add(1,3107,1,gp_execute_command,NCMAIN+4005); // a launcher for the gp command
  commands.add(1,3108,1,background_colour_command,NCMAIN+4006);

  // KMJ : old property commands
  commands.add(1,3000,1,colour_command,NCMAIN+193);
  commands.add(1,3001,1,layer_command,NCMAIN+194);
  commands.add(1,3002,1,style_command,NCMAIN+195);
  commands.add(1,3003,1,weight_command,NCMAIN+196);
  commands.add(1,3004,1,styleset_command,NCMAINB+62);
  
  commands.add(1,9999,0,password_command, (char*)"");
  commands.add(1,60001,0,material_command, (char*)"");

  /*
      KMJ : added since 2016/08
  */
    commands.add(1,60002,0,selectsamecolour_command, (char*)"");
    commands.add(1,60003,0,selectsamelayer_command, (char*)"");
    commands.add(1,60004,0,selectsamestyle_command, (char*)"");
    commands.add(1,60005,0,selectsameweight_command, (char*)"");
    commands.add(1,60006,0,selectsameentity_command, (char*)"");
    commands.add(1,60007,0,selectsamename_command, (char*)"");

    commands.add(1,60010,0,hidesamecolour_command, (char*)"");
    commands.add(1,60011,0,hidesamelayer_command, (char*)"");
    commands.add(1,60012,0,hidesamestyle_command, (char*)"");
    commands.add(1,60013,0,hidesameweight_command, (char*)"");
    commands.add(1,60014,0,hidesameentity_command, (char*)"");
    commands.add(1,60015,0,hidesamename_command, (char*)"");
    commands.add(1,60016,0,unhideallentities_command, (char*)"");
  /*
      KMJ : end added since 2016/08
  */

  commands.add(1,20000,0,animation_command1, (char*)"Insert/Change camera path");
  commands.add(1,20001,0,animation_command2, (char*)"Insert/Change camera reference path");
  commands.add(1,20002,0,animation_command3, (char*)"Insert/Change object path");
  commands.add(1,20003,0,animation_command4, (char*)"Run animation");
#if VERSION != FLEXICAD
  commands.add(1,20004,0,animation_command5, (char*)"Replay AVI file");
  commands.add(1,20005,0,animation_command12, (char*)"Insert/Change patch path");
  commands.add(1,20010,0,animation_command6, (char*)"Stop");
  commands.add(1,20011,0,animation_command7, (char*)"Pause");
  commands.add(1,20012,0,animation_command8, (char*)"Play");
  commands.add(1,20013,0,animation_command9, (char*)"Reverse");
  commands.add(1,20014,0,animation_command10, (char*)"Step forward");
  commands.add(1,20015,0,animation_command11, (char*)"Step Reverse");
#endif

}


extern int LoadDirect3d(void);

int InitInstance(RCHINSTANCE hInstance, int nCmdShow,RCCHAR *commandline)
{
 RCHACCEL hAccel;
 //FARPROC fp1,fp2;
 HOOKPROC fp1,fp2;
 RCCHAR swp[256];
 int x,y,w,h,s;
/*
#if ! defined(WIN32)
  Ctl3dRegister(hInstance);
  Ctl3dAutoSubclass(hInstance);
#endif
*/
  //QMessageBox::warning(0,"","init instance :");
  //hAccel = LoadAccelerators(hInstance,TEXT("Keytable"));


  // this sets up a callback to watch for the F1 (help) key being pressed
//#ifndef _USING_QT
//#if ! defined(_WIN32_WCE)
//#if defined(_MSC_VER)
  /*
  // KMJ TODO: look at this
  fp1 =  (HOOKPROC)MakeProcInstance( helpfilter,hInstance);
  if (fp1 != 0)
      fp2 = (HOOKPROC)SetWindowsHook(WH_MSGFILTER,(long (__stdcall *)(int,unsigned int,long))fp1);
    //fp2 = (int (__stdcall *)(void))SetWindowsHook(WH_MSGFILTER,(long (__stdcall *)(int,unsigned int,long))fp1);
  else
    fp2 = 0;
  */
  /*
#else
  fp1 = MakeProcInstance((FARPROC)helpfilter,hInstance);
  if (fp1 != 0)
    fp2 = SetWindowsHook(WH_MSGFILTER,fp1);
  else
    fp2 = 0;
#endif
*/
//#else
//  fp1 = fp2 = 0;
//#endif
//#endif

  //  Attempt to load the opengl interface dll - if if fails, all operations should revert back to built in software operations.
  // KMJ TODO: look at this
  LoadDirect3d();

  // inits an instance of the program class
  //program = new Program(hInstance,nCmdShow,hAccel,fp1,fp2,commandline);
  program = new Program;
  //program->init(hInstance,nCmdShow,hAccel,fp1,fp2,commandline);

  // reset the database defaults;
  db.initialize();

  program->init(hInstance,nCmdShow,hAccel,fp1,fp2,commandline);

  // KMJ : this is used as is
  CreateCommandList();

  // KMJ TODO: look at this

#if 0
  GetPrivateProfileString("WindowPlacement","Main","",swp,256,home.getinifilename());
  qDebug() << "using window placement : " << QString(swp);
  if (strlen(swp) == 0)
    cadwindow = new MainWindow(CW_USEDEFAULT,CW_USEDEFAULT,800,600,SW_HIDE);
  else
  {
      if (sscanf(swp,"%d,%d,%d,%d,%d",&x,&y,&w,&h,&s) == 5)
         cadwindow = new MainWindow(x,y,w,h,s);
       else
         cadwindow = new MainWindow(CW_USEDEFAULT,CW_USEDEFAULT,800,600,SW_SHOW);
  }
#endif

  // create the main window hidden
  //cadwindow = new MainWindow(CW_USEDEFAULT,CW_USEDEFAULT,800,600,SW_HIDE);
  cadwindow=0;
  qDebug() << "primary scren width  : " << app->primaryScreen()->size().width();
  qDebug() << "primary scren height : " << app->primaryScreen()->size().height();

  int mww = app->primaryScreen()->size().width()/3*2;
  int mwh = app->primaryScreen()->size().height()/3*2;
#ifdef _MAC
  cadwindow = new MainWindow(0,0,mww,mwh,SW_HIDE);
#else
  cadwindow = new MainWindow(0,0,mww,mwh,SW_HIDE);
#endif
  //((QtMainWindow*)cadwindow->gethwnd())->readSettings();

  cadwindow->openlist.setwindow(cadwindow);
  make_menu();
  cadwindow->openlist.setwindow(cadwindow);


  ((QtMainWindow*)cadwindow->gethwnd())->readSettings();

  db.grid.initialize();
  program->filenewapps();

  hAccel = LoadAccelerators(hInstance,TEXT("Keytable"));
  program->sethaccel(hAccel);
  cadwindow->paintall();

  return cadwindow->created();
}

void CloseInstance(void)
{ 
#if ! defined(_MSC_VER)
    /*
  if (program->gethelpfilter() != 0)
    {  SetWindowsHook(WH_MSGFILTER,program->gethelpfilter());
       FreeProcInstance(program->gethelpfilter());
   }
   */
#endif

  cadwindow->deleteSpashScreen();

  cadwindow->destroyall();

  db.layernames.clear();
DebugTraceLine(__FILE__,__LINE__);
  db.linestyles.clear();
DebugTraceLine(__FILE__,__LINE__);
  db.views3d.clear();
DebugTraceLine(__FILE__,__LINE__);
  db.workplanes.clear();
DebugTraceLine(__FILE__,__LINE__);
  delete cadwindow;
DebugTraceLine(__FILE__,__LINE__);
  cadwindow = 0;
DebugTraceLine(__FILE__,__LINE__);
db.lineweights.clear();
DebugTraceLine(__FILE__,__LINE__);
  UnregisterClass(TEXT("CadWindow"),program->gethinst());
DebugTraceLine(__FILE__,__LINE__);
  UnregisterClass(TEXT("NoCursorCadWindow"),program->gethinst());
#if ! defined(WIN32)
  //Ctl3dUnregister(program->gethinst());
#endif
DebugTraceLine(__FILE__,__LINE__);
  delete program;
DebugTraceLine(__FILE__,__LINE__);
    program=0;
}

#if VERSION == TRICAD_STUDENT

#define FP_SEG(fp)     ((unsigned)(void __seg *)(void __far *)(fp))
#define FP_OFF(fp)     ((unsigned)(fp))

char *hashedserialnumber(int drive)
{static char hsn[12];
 unsigned char dbuffer[522];
 UINT sega,offa;
  dbuffer[0] = dbuffer[1] = dbuffer[2] = dbuffer[3] = 0;
  dbuffer[4] = 1;  dbuffer[5] = 0;
  *((UINT *)(dbuffer+6)) = FP_OFF(dbuffer+10);
  *((UINT *)(dbuffer+8)) = FP_SEG(dbuffer+10);
  sega = FP_SEG(dbuffer);
  offa = FP_OFF(dbuffer);
  asm push ds
  asm push es
  asm mov ax,drive
  asm mov cx,-1
  asm mov dx,0
  asm mov bx,sega
  asm mov ds,bx
  asm mov bx,offa
  asm int 0x25
  asm popf
  asm pop es
  asm pop ds
  sprintf(hsn,"%2.2x-%2.2x-%2.2x-%2.2x",dbuffer[0x31] ^ 'S',dbuffer[0x32] ^ 'p',dbuffer[0x33] ^ '0',dbuffer[0x34] ^ 'T');
  return hsn;
}

void check_installation(RCHINSTANCE hinstance)
{char filename[300],serial[12];
 FILE *infile;
  GetModuleFileName(hinstance,filename,300);
  strupr(filename);
  if ((infile = fopen(filename,"rb")) == 0)
    { RCMessageBox(0,"Please check the installation, or reinstall the cad software.","CAD",MB_ICONSTOP);
       exit(1);
    }
  else
    {  fseek(infile,-12L,SEEK_END);
       fread(serial,12,1,infile);
       fclose(infile);
       if (strcmp(serial,hashedserialnumber(filename[0]-'A')) != 0)
         { RCMessageBox(0,"Please check the installation, or reinstall the cad software.","CAD",MB_ICONSTOP);
            exit(1);
         }
    }
}

#else
void check_installation(RCHINSTANCE)
{
}
#endif

extern void LoadAcceleratorTable(void);

extern void OpenPDF(char *title);


class SplashDisplayDialogControl : public DisplayDialogControl
  {private:
     int currenttipindex,ntips; 
   public:
     SplashDisplayDialogControl(int id);
     void draw(Dialog *dialog,RCHDC hDC);
     void nexttip(void);
     void previoustip(void);
     
  };

void SplashDisplayDialogControl::nexttip(void)
{ if (ntips != 0)
    {  currenttipindex = currenttipindex++;
       if (currenttipindex > ntips)
         currenttipindex = 1;
    }  
}

void SplashDisplayDialogControl::previoustip(void)
{ if (ntips != 0)
    {  currenttipindex--;
       if (currenttipindex <= 0)
         currenttipindex = ntips;
    }
}

SplashDisplayDialogControl::SplashDisplayDialogControl(int id) : DisplayDialogControl(id) 
{FILE *infile;
 char filename[300],line[300];


  if (GetModuleHandle(_RCT("Landworks.DLL")) != 0)
    strcpy(filename,home.getexecutablefilename(_RCT("LWToolTips.DAT")));
  else
    strcpy(filename,home.getexecutablefilename(_RCT("ToolTips.DAT")));

  ntips = 0;
  if ((infile = fopen(filename,"r")) != 0)
    {  while (fgets(line,300,infile) != 0)
         {  if (sscanf(line,"[Tip%d]",&ntips) == 1)
              break;
         } 
       fclose(infile);
    }

  if (ntips > 1)
    currenttipindex = (timeGetTime() % ntips) + 1;
  else
    currenttipindex = 1;

}

void SplashDisplayDialogControl::draw(Dialog *dialog,RCHDC hDC)
{static int i = 0;
 int tipindex,lineno;
 RCCHAR filename[300],line[300];
 RCHBITMAP hbitmap,holdbitmap;
 RCHDC hbmdc;
 RECT rect;
 FILE *infile;

  if (dialog->currentvalue(110,filename,300) != 0 && strlen(filename) > 0)
    {  dialog->getcontrol(110)->store(dialog);
       EndDialog(dialog->gethdlg(),110);
       return;
    }

  if (hDC != 0)
    {  hbitmap = LoadBitmap(program->gethinst(),(RCLPTSTR)MAKEINTRESOURCE(104));
       hbmdc = CreateCompatibleDC(hDC);
       holdbitmap = (RCHBITMAP) SelectObject(hbmdc,hbitmap);
       GetClientRect(GetDlgItem(dialog->gethdlg(),id),&rect);
       StretchBlt(hDC,0,0,rect.right,rect.bottom,
                  hbmdc,0,0,390,250,WHITENESS);
       StretchBlt(hDC,0,0,390,255,
                  hbmdc,0,0,390,250,SRCCOPY);
       SelectObject(hbmdc,holdbitmap);
       DeleteObject(hbitmap);
       DeleteDC(hbmdc);

       RCHFONT hfont = CreateFont(16,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,_RCT("Arial"));
       RCHFONT holdfont = (RCHFONT)SelectObject(hDC,hfont);

       if (GetModuleHandle("Landworks.DLL") != 0)
         strcpy(filename,home.getexecutablefilename(_RCT("LWToolTips.DAT")));
       else
         strcpy(filename,home.getexecutablefilename(_RCT("ToolTips.DAT")));

       tipindex = -1;
       if ((infile = fopen(filename,"r")) != 0)
         {  while (fgets(line,300,infile) != 0)
              {  if (strchr(line,'\n') != 0)
                   strchr(line,'\n')[0] = 0;
                 if (sscanf(line,"[Tip%d]",&tipindex) == 1)
                   lineno = 0;
                 else if (currenttipindex == tipindex)
                   {  SetTextColor(hDC,RGB(0,0,0));
                      SetBkMode(hDC,TRANSPARENT);
                      TextOut(hDC,60,45+16*lineno++,line,strlen(line));
                   }
              }
            fclose(infile); 
         }

       SelectObject(hDC,holdfont);
       DeleteObject(hfont);

    } 


}

class SplashButtonDialogControl : public ButtonDialogControl
{public:
   SplashButtonDialogControl(int id) : ButtonDialogControl(id) {}
   int select(Dialog *);
};

int SplashButtonDialogControl::select(Dialog *dialog)
{char filename[300];
 FILE *infile;
  if (id == 103)
    {  //  For now just goto the web site

       strcpy(filename,home.getexecutablefilename(_RCT("videos.htm")));
       infile = fopen(filename,"r");
       if (infile != 0)
         {  fclose(infile);
            OpenBrowser(filename);
         }
       else
         {  if (GetModuleHandle("Landworks.DLL") != 0)
              OpenBrowser((char*)"http://www.landworkscad.com/videos.htm");
            else
              OpenBrowser((char*)"http://www.realcad.com/videos.htm");
         }

    }
  else if (id == 104)
    OpenPDF(_RCT("Tutorial.PDF"));
  else if (id == 105)
    {  if (GetModuleHandle("Landworks.DLL") != 0)
         OpenBrowser((char*)"http://www.landworkscad.com");
       else
         OpenBrowser((char*)"http://www.realcad.com");
    }
  else if (id == 106)
    {  ((SplashDisplayDialogControl *)dialog->getcontrol(108))->previoustip();
       InvalidateRect(GetDlgItem(dialog->gethdlg(),108),0,TRUE);
    }
  else if (id == 111)
    {  ((SplashDisplayDialogControl *)dialog->getcontrol(108))->nexttip();
       InvalidateRect(GetDlgItem(dialog->gethdlg(),108),0,TRUE);
    }
  else if (id == 107)
    OpenPDF(_RCT("LANDWorksCAD-Purchase-Form.PDF"));


  return 0;
}


int WINAPI Start(int a,int b,void *)
{char text[300];
    sprintf(text,"Start %d %d",a,b);
   RCMessageBox(cadwindow->gethwnd(),text,"",MB_OK);
    return 0;
}

int WINAPI End(int a,int b,void *)
{char text[300];
    sprintf(text,"Start %d %d",a,b);
   RCMessageBox(cadwindow->gethwnd(),text,"",MB_OK);
    return 0;
}



class SplashCheckBoxDialogControl : public CheckBoxDialogControl
{public:
   SplashCheckBoxDialogControl(int id,int *value) : CheckBoxDialogControl(id,value) {}
   int select(Dialog *);
};


int SplashCheckBoxDialogControl::select(Dialog *dialog)
{  
  if (SendDlgItemMessage(dialog->gethdlg(),109,BM_GETCHECK,0,0) == BST_CHECKED)
    {  v.setinteger("wn::showstartupdialog",1);
       WritePrivateProfileString("Integers","wn::showstartupdialog","1",home.getinifilename());
    }
  else
    {  v.setinteger("wn::showstartupdialog",0);
       WritePrivateProfileString("Integers","wn::showstartupdialog","0",home.getinifilename());
    }

  return 0;
}

bool checkInetUrlAvailable(const char *urlstring)
{
#ifdef _MAC
#if 1
    // one way of doing it with Qt
    // currently breaks realcad on windows but might get it to work
    bool netStatus=false;
    QNetworkAccessManager nam;
    QString urlstr(urlstring);
    QUrl url(urlstr);
    QNetworkRequest nreq(url);
    QNetworkReply *reply = nam.get(nreq);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if(reply->bytesAvailable())
        netStatus = true;
    else
        netStatus = false;
    QObject::disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    delete reply;
    return netStatus;
#endif
#if 0
    // the Qt way of doing it using the global nam and app network repy
    // this method crashes the app on windows
    bool netStatus=false;
    if(networkmanager == 0)
    {
        networkmanager = new QNetworkAccessManager(app);
        app->connect(networkmanager, SIGNAL(finished(QNetworkReply*)),
                 app, SLOT(webPageReplyFinished(QNetworkReply*)));
    }

    if(app->mlastWebPageReply != 0)
    {
        app->mlastWebPageReply->deleteLater();
        app->mlastWebPageReply = 0;
    }

    QString urlstr(urlstring);
    QUrl url(urlstr);
    QNetworkRequest nreq(url);

    app->mlastWebPageReply = networkmanager->get(nreq);

    while(app->mlastWebPageReply == 0)
    {
        app->processEvents();
    }

    if(app->mlastWebPageReply->bytesAvailable())
        netStatus = true;
    else
        netStatus = false;

    delete app->mlastWebPageReply;
    app->mlastWebPageReply = 0;
    app->processEvents();

    return netStatus;
#endif
#else
    // this is a native windows way to do it
    HINTERNET hInternet = InternetOpen(L"ValidURL", INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
    bool bValid = false;

    if (hInternet)
    {
        HINTERNET hFile = InternetOpenUrl(hInternet, QString(urlstring).toStdWString().c_str(), 0, 0, INTERNET_FLAG_RELOAD, 0);
        if (hFile)
        {
            int code = 0;
            DWORD length = sizeof(int);

            HttpQueryInfo(hFile, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &code, &length, 0);
            bValid = code == HTTP_STATUS_OK || code == HTTP_STATUS_REDIRECT;

            InternetCloseHandle(hFile);
        }

        InternetCloseHandle(hInternet);
    }

    return bValid;
#endif
}

bool checkInetAvailable()
{
    // TODO: to do make a more reliable one than this
    // ping cad australia or google
    if(checkInetUrlAvailable("http://www.cad.com.au"))
        return true;
    else if(checkInetUrlAvailable("http://www.google.com"))
        return true;
    else
        return false;
#if 0
    // this is a windows native way to do it
    bool bIsInternetAvailable = false;
    // Get the required buffer size
    DWORD dwBufferSize = 0;
    if (ERROR_INSUFFICIENT_BUFFER == GetIpForwardTable(NULL, &dwBufferSize, false))
    {
        BYTE *pByte = new BYTE[dwBufferSize];
        if (pByte)
        {
            PMIB_IPFORWARDTABLE pRoutingTable = reinterpret_cast<PMIB_IPFORWARDTABLE>(pByte);
            // Attempt to fill buffer with routing table information
            if (NO_ERROR == GetIpForwardTable(pRoutingTable, &dwBufferSize, false))
            {
                DWORD dwRowCount = pRoutingTable->dwNumEntries; // Get row count
                // Look for default route to gateway
                for (DWORD dwIndex = 0; dwIndex < dwRowCount; ++dwIndex)
                {
                    if (pRoutingTable->table[dwIndex].dwForwardDest == 0)
                    { // Default route designated by 0.0.0.0 in table
                        bIsInternetAvailable = true; // Found it
                        break; // Short circuit loop
                    }
                }
            }
            delete[] pByte; // Clean up. Just say "No" to memory leaks
        }
    }
    return bIsInternetAvailable;
#endif
}

int main(int argc, char *argv[])
{

//#ifdef _USING_QT
    // hInstance is the current instance of the application
    // hPrevInstance allways NULL - the previous instacne of the applications
    // hInstance is used as the pointer to the global Qapp when using Qt
    RCHINSTANCE hInstance=0, hPrevInstance=0;

	int nCmdShow;
//#endif
 int msgcount = 96,status,i,j,k,n,cstate,delim,showsplash,selection,licensed;
 HighLevelEvent *e;
 RCCHAR fcl[300],tok[300],filename[300],title[300],*filelist[MAX_OPENLIST],buffer[MAX_OPENLIST*300],key[300],*cl;
 double fontsize;
 HKEY hkey;
 FILE *infile;
 WSADATA wsaData;
 cadwindow=0;

 qDebug() << "Entering RealCAD main(...)";
 //qDebug() << "RC_NUM_NUMBERED_ICON_RESOURCES" << RC_NUM_NUMBERED_ICON_RESOURCES;
 //qDebug() << "RC_NUM_NAMED_CURSOR_RESOURCES" << RC_NUM_NAMED_CURSOR_RESOURCES;
 //qDebug() << "RC_NUM_NUMBERED_CURSOR_RESOURCES" << RC_NUM_NUMBERED_CURSOR_RESOURCES;

//#ifdef _USING_QT
    // creating the QT application object here passing it the command line arguments
    //QApplication app(argc, argv);

    //debug
    //qDebug() << "Creating the application object";

    // we have two hard coded style sheets Light and dark
    // which one is used is in the ini file
    // it is possible to load a custom style sheet if set in the ini file
    // stylesheet=<name.qss>
    // if there is a .qss or something in the it will be loaded
    // or if the command line -stylesheet xyz.qss is used that will be loaded
    // order is:
    // 1. command line
    // 2. ini file
    // default is the hard coded light style.

    //
    //QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps,true);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    //
    RCApplication rcapp(argc, argv);
    app = &rcapp;

    GlobalEventHandler *gev = new GlobalEventHandler;
    rcapp.instance()->installEventFilter(gev);

   QString allArgs;
   for(int ia=0; ia<argc; ia++)
   {
       qDebug() << "args [" << ia << "] : " << QString(argv[ia]);
       allArgs.append(QString(argv[ia]));
       allArgs.append(QString("\n"));
   }

#ifdef Q_OS_MAC
    // KMJ: trying to remove the dictation and emoji menus
    // not working yet
#if 0
    CocoaInitializer initializer;
    fixMacMenus();
#endif
#endif

    qDebug() << "devicePixelRatio : " << app->devicePixelRatio();
    qDebug() << "High dpi scaling : " << app->testAttribute(Qt::AA_EnableHighDpiScaling);
    qDebug() << "isVirtualDesktop" << app->desktop()->isVirtualDesktop();
    qDebug() << "screenCount" << app->desktop()->screenCount();
    qDebug() << "primaryScreen" << app->desktop()->primaryScreen();
    qDebug() << "availableGeometry - all screens" << app->desktop()->availableGeometry();
    for(i=0; i< app->desktop()->screenCount(); i++)
    {
        qDebug() << "availableGeometry - screen[" << i << "]" << app->desktop()->availableGeometry(i);
    }

    //QMessageBox::warning(0,"command line",allArgs);

    QStringList qcommandargs = app->arguments();

    /*
    {
        QString qcl,qclall;
        foreach( qcl, qcommandargs)
            qclall += qcl;

        QMessageBox::warning(0,"command line",qclall);
    }
    */

    //Qt::WindowFullscreenButtonHint
    QCoreApplication::setOrganizationDomain("cadinternational.com");
    QCoreApplication::setOrganizationName("CAD International");
    QCoreApplication::setApplicationName(QString(home.getApplicationTitle()));
    QCoreApplication::setApplicationVersion("8.0.0");

    if( !allArgs.contains("-style"))
        app->setStyle("fusion");


    // debug
    /*
    QMessageBox msgBox;
    msgBox.setText(allArgs);
    msgBox.exec();
    for(int ia=0; ia<app->arguments().length(); ia++)
    {
        qDebug() << "args [" << ia << "] : " << app->arguments().at(ia);
    }
    */

    // debug testing here
    //testFindFirst();
    // debug testing end

    // get the local for GUI translation
#ifdef _WIN32
    // Qt Bug?? can't get current local on windows use our own function
    // the problem was that the env LANG was set to EN_us
    char lang[10] = {"en"};
    GetLanguageString( lang );
    QString locale(lang);
    if(strstr(lang,"Spanish"))
        locale = "es";
#else
    QLocale thisLoc = QLocale::system();
    QString locale = thisLoc.name();
    //qDebug() << "locale : " << locale;
    //qDebug() << "Country : " << QLocale::countryToString(thisLoc.country());
#endif
    // set up the Qt translation stuff
    QTranslator translator;
    // get the translation by forcing the file name
    //if(translator.load("RealCAD_es"))
    // get the translation based on the locale
    //if(translator.load(QString("RealCAD_") + locale))
    //    qDebug() << "Translation loaded";
    //else
    //    qDebug() << "Translation NOT loaded";
    app->installTranslator(&translator);

    // set the instance
    hInstance = (RCHINSTANCE)app->instance();
    // initializing the Home object here must be done after the Qapp is created
    // this is a new constructor for use with the Qt version
    home = Home(true);
    QSettings settings(QString(home.getinifilename()),QSettings::IniFormat);

    // debug
    //QMessageBox::information(0,"Title",QString(home.getApplicationTitle()));

#ifdef Q_OS_MAC
    // deployment for the Mac
    // the Mac must check if the RealCAD folder is present
    // if is not there copy it from the app bundle
    QString defPublicFolder;
    //defPublicFolder = "/Users/Shared/" + app->applicationName();
    defPublicFolder = "/Users/Shared/" + QString(home.getApplicationTitle());
    QDir cpTarget(defPublicFolder);
    if(!cpTarget.exists())
    {
        QString applicationPath(home.getexecutablefilename(_RCT("")));
        int contentsPos = applicationPath.indexOf("/Contents") + QString("/Contents").length();
        QString contentsFolder = applicationPath.left(contentsPos) + "/" + app->applicationName();
        QString command;
        command = "cp -R " + contentsFolder + QStringLiteral(" /Users/Shared");
        if(QDir(contentsFolder).exists())
        {
            //QMessageBox::warning(0,"RealCAD",command);
            //QString command("cp -R /Applications/RealCAD.app/Contents/RealCAD /Users/Shared");
            QProcess *myProcess = new QProcess(app);
            myProcess->start(command);
            bool status = myProcess->waitForFinished(-1);
            int count=0;
            while(!cpTarget.exists() && count < 1000)
            {
                sleep(500);
                count++;
            }
            if( !status || !cpTarget.exists() )
                QMessageBox::warning(0,QString(home.getApplicationTitle()),"Cannot copy your "+QString(home.getApplicationTitle())+" Folder\nPlease contact CAD International support for further assistance.");
            command = "chmod -R 777 " + defPublicFolder;
            myProcess->start(command);
            if( !myProcess->waitForFinished(-1))
                QMessageBox::warning(0,QString(home.getApplicationTitle()),"Cannot set permissions for your "+QString(home.getApplicationTitle())+" Folder\nPlease contact CAD International support for further assistance.");

            // restart the app
            QProcess::startDetached(app->applicationFilePath());
            app->exit();
            _Exit(0);
            return 0;
        }
    }
#endif

    // set up the style sheet
    if(app->styleSheet().isEmpty())
    {
        // look in the ini file for the stylesheet value
        RCCHAR styleName[300];
        QString styleSheetText;
        GetPrivateProfileString("GuiStyle","StyleName","Light",styleName,300,home.getinifilename());
        QStringList strings = QString(styleName).split(".");
        if(strings.length() > 1 && strings.at(1).length() > 0)
            styleSheetText = app->getStyleSheetText(QString(home.getpublicfilename(styleName)));
        else if(strings.at(0) == "Dark")
            if(RCGetHDPIYscale() > 1)
                styleSheetText = app->getStyleSheetText(RCDarkStyle2x);
            else
                styleSheetText = app->getStyleSheetText(RCDarkStyle);
        else if(strings.at(0) == "Light")
            if(RCGetHDPIYscale() > 1)
                styleSheetText = app->getStyleSheetText(RCLightStyle2x);
            else
                styleSheetText = app->getStyleSheetText(RCLightStyle);
        else
            if(RCGetHDPIYscale() > 1)
                styleSheetText = app->getStyleSheetText(RCLightStyle2x);
            else
                styleSheetText = app->getStyleSheetText(RCLightStyle);

        if(styleSheetText.isEmpty())
            if(RCGetHDPIYscale() > 1)
                styleSheetText = app->getStyleSheetText(RCLightStyle2x);
            else
                styleSheetText = app->getStyleSheetText(RCLightStyle);

        app->setStyleSheet(styleSheetText);
    }

//#endif

  // KMJ look at using Qt libraries for this
  // start the winsock library
  //WSAStartup(0x101,&wsaData);

  //  Get the first token
  j = 0;
 
  // parse the command line
   // KMJ TODO: fix at this
// #ifdef _USING_QT
  RCCHAR *ocommandline;
  ocommandline = RCGETCOMMANDLINE();
//#endif
  /*
  cl = ocommandline;
  while (cl[j] == ' ') j++;
  if (cl[j] == '"')
    {  delim = '"';  j++;
    }
  else
    delim = ' ';
  k = j;  tok[0] = 0;
  while (cl[k] != 0 && cl[k] != delim)
    {  tok[k - j] = cl[k];
       tok[k - j + 1] = 0;
       k++;
    }
  if (cl[k] == '"') k++;

  if (*tok != '/' && *tok != '-' && strlen(tok) > 5)
    {//  Reset the home directory to the exe directory if we double clicked a file
       GetModuleFileName(0,filename,300);
       if (strrchr(filename,'\\') != 0)
         strrchr(filename,'\\')[0] = 0;
       _chdir(filename);
    }
*/

  // set up the values (settings)
//#ifndef _RCDEBUG
  qDebug() << "Initialising the Values";
  v.clear();
  //v.initialize();
  qDebug() << "Done";
//#endif
#ifdef _DEBUG_DETAILS
//  v.debug();
#endif

  //  Make sure the .CAD extension is registered
  // need a Windows and Mac version of this??
  /*
  RegCreateKey(HKEY_CLASSES_ROOT,".cad",&hkey);
  GetModuleFileName(0,filename,300);
  RegSetValueEx(hkey,"",0,REG_SZ,(LPBYTE)"application/CAD-File",21);
  RegDeleteValue(hkey,"Content Type");
  RegCloseKey(hkey);
  //  Add the shell command entry
  RegCreateKey(HKEY_CLASSES_ROOT,"application/CAD-File\\Shell\\Open\\Command",&hkey);
  GetModuleFileName(0,filename,300);
  strcat(filename," \"%1\"");
  RegSetValueEx(hkey,0,0,REG_SZ,(LPBYTE)filename,strlen(filename));
  RegCloseKey(hkey);

  //  Delete any references to other applications that may have registered this key
  RegCreateKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.cad",&hkey);
  RegDeleteKey(hkey,"OpenWithList");
  RegDeleteKey(hkey,"OpenWithProgids");
  RegCloseKey(hkey);
  */

  // KMJ TODO:fix this
  //_tcscpy(fcl,ocommandline);  _tcslwr(fcl);

/*
  fontsize = 8.0;
  if (strstr(fcl,"-fs") != 0)
    sscanf(strstr(fcl,"-fs")+3,"%lf",&fontsize);
  if (fontsize < 1.0)
    fontsize = 1.0;
  else if (fontsize > 100.0)
    fontsize = 100.0;

  MainWindow::setfontsize(fontsize);
*/
  // these functions are now obsolete in Windows
// not needed for Qt either
//#ifndef _USING_QT
//#if ! defined(_WIN32_WCE)
//  _set_new_handler(0);
//  while (! SetMessageQueue(msgcount)) msgcount -= 8;
//#endif
//#endif

  if (!hPrevInstance)
    if (!InitApplication(hInstance))
      return (FALSE);
  qDebug() << "Calling InitInstance";
  if (!InitInstance(hInstance, nCmdShow,ocommandline))
    return (FALSE);  
  qDebug() << "Done";

  QString paths;
  //paths = "app path dir : " + app->applicationDirPath() + QString("\n");
  //paths += "app file path : " + app->applicationFilePath() + QString("\n");
  //cadwindow->MessageBox(paths.constData(),"init file name",MB_OK);

  //paths = "app file path : " + QString(home.getexecutablefilename(_RCT(""))) + QString("\n");
  //paths += "lic file name : " + QString(home.getlicensefilename()) + QString("\n");
  //paths += "temp path : " + QString(home.gettempfilename(_RCT(""))) + QString("\n");
  //paths += "ini file : " + QString(home.getinifilename()) + QString("\n");
  //paths += "public file : " + QString(home.getpublicfilename("")) + QString("\n");
  //paths += "users HOME : " + QString(getenv("HOME")) + QString("\n");

  //paths = QString(home.getpublicfilename("cad.ini"));
  //cadwindow->MessageBox(paths.constData(),"init file name",MB_OK);

  // KMJ TODO:: look at this
  //program->processpaintmessages();

//#ifndef _USING_QT
  // this function does not do anything
  //check_installation(hInstance);

  //while ((status = program->processmessages(2)) == 1) ;
  //state.sendevent(new NewCommandEvent(9999,0));
//#endif

//#ifdef _USING_QT
  licensed = 1;
//#else
//#if defined(INSTALLLICENSE) && (VERSION != FLEXICAD) && ! defined(_WIN32_WCE)
//if (strstr(ocommandline,"-nocheckatstart") == 0)
    licensed = check_license((char*)"Main CAD Application", (char*)"RealCADKey1", (char*)"RealCADKey2",0,1);
//#endif
//#endif

  if (program->getlicensed() >= 2)
    cadwindow->coordinateentry->disablez();

  // if not licensed force the splash screen on
  if(licensed == 0)
  {
      v.setinteger("wn::showstartupdialog",1);
      WritePrivateProfileString("Integers","wn::showstartupdialog","1",home.getinifilename());
  }

//#if (MENUS != FLEXICADMENUS) && ! defined(_WIN32_WCE)
  /*
  if (strncmp(llabel,"REGISTRATION ID  :  XX-X00-000000XXXXXXX",40) != 0 &&
       strstr(ocommandline,"-nocheckatstart") == 0 && check_license("Main CAD Application","RealCADKey1","RealCADKey2",0,1) == 0)
    {DWORD time;
       Dialog dialog("Unlicensed_dialog",1);
       dialog.process(cadwindow->gethwnd());
       time = timeGetTime();
       while (timeGetTime() - time < 2500)
         {RCMSG msg;
            while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
              {  TranslateAccelerator(cadwindow->gethwnd(),program->gethaccel(),&msg);
                 TranslateMessage(&msg);
                 DispatchMessage(&msg);
              }
         }
    }
  */
//#endif



  j = 0;
  /*
  cl = program->getcommandline();
  while (cl[j] == ' ') j++;
  if (cl[j] == '"')
    {  delim = '"';  j++;
    }
  else
    delim = ' ';
  k = j;  tok[0] = 0;
  while (cl[k] != 0 && cl[k] != delim)
    {  tok[k - j] = cl[k];
       tok[k - j + 1] = 0;
       k++;
    }
  if (cl[k] == '"') k++;
  */
//#if ! defined(_WIN32_WCE)
  /*
  for (i = 0 ; strlen(tok) > 0 ; i++)
    {  if (i == 0 && *tok != '/' && *tok != '-' && strlen(tok) > 5)
         file_open(i,tok);
       else if (i == 1 && *tok != '/' && *tok != '-' && strlen(tok) > 5)
         file_open(i,tok);
       else if (i == 2 && strcmp(tok,"-compare") == 0)
         {  zoom_command6(&cstate,0,0);
         }
       j = k;
       while (cl[j] == ' ') j++;
       if (cl[j] == '"')
         {  delim = '"';  j++;
         }
       else
         delim = ' ';
       k = j;  tok[0] = 0;
       while (cl[k] != 0 && cl[k] != delim)
         {  tok[k - j] = cl[k];
            tok[k - j + 1] = 0;
            k++;
         }
       if (cl[k] == '"') k++;
    }
    */
//#endif

  //showsplash = v.getinteger("wn::showstartupdialog");

  // backup file stuff
  GetTempPath(300,filename);
  strcat(filename,"RealCAD.CAD");
  strcpy(filename,QDir::toNativeSeparators(QString(filename)));
  if (GetPrivateProfileInt("Backup","Disable",0,home.getinifilename()) == 0 && (infile = fopen(filename,"rb")) != 0)
  {
       fclose(infile);
       if (cadwindow->MessageBox("A backup file has been found.  Do you want to open this file?","Load Backup",MB_YESNO) == IDYES)
       {
            file_open(0,filename);

            //  Set the name to untitled - to make sure we don't save to the backup file name
            ResourceString rsnc3(NC+3);
            db.setname(rsnc3.gets());
            db.settitle(rsnc3.gets());
            cadwindow->updatetitle();

            //showsplash = 0;
       }
  }


  // recent files list?
  /*
  for (n = 0 ; n < MAX_OPENLIST ; )
    {  filelist[n] = buffer + n*300;
       strcpy(filelist[n],"");
       strcpy(title,"");
       sprintf(key,"op::file%d",n);
       GetPrivateProfileString(_RCT("NameList"),key,_RCT(""),filelist[n],300,home.getinifilename());
       GetPrivateProfileString(_RCT("TitleList"),key,_RCT(""),title,300,home.getinifilename());
       if (strlen(filelist[n]) > 0 && strlen(title) > 0)
         {  if (strlen(filelist[n]) > 35)
              {  //  Shorten the name
                 strcpy(filename,filelist[n]);
                 for (i = 1 ; strrchr(filename,'\\') != 0 ; i++)
                   {  if (strlen(strrchr(filename,'\\')) > 33)
                        break;
                      strrchr(filename,'\\')[0] = i;
                   }
                
                 if (i > 1)
                   {  strcpy(filelist[n]+3,"...");
                      strcat(filelist[n],strrchr(filename,i-1));
                       
                      while (i > 0)
                        {  if (strchr(filelist[n],i) != 0)
                             strchr(filelist[n],i)[0] = '\\';
                           i--;
                        } 
                   }
                 else
                   {  if (strchr(filename,'\\') != strrchr(filename,'\\'))
                        {  strcpy(filelist[n]+3,"...");
                           strcat(filelist[n],strrchr(filename,'\\')); 
                        } 
                      strcpy(filelist[n]+35,"...");
                   }

              }                

            n++;
         }
       else
         break;
    }
    */

  // load LANDWorks plugin module
  
  /*
  if (GetModuleHandle("Landworks.DLL") != 0)
    strcpy(filename,"LWSPLASH_DIALOG");
  else
    strcpy(filename,"SPLASH_DIALOG");
  */

#if 0
  // splash dialogs stuff
  // ToDO: make the old spash screen in Qt if required
  Dialog dialog(filename);

  dialog.add(new ButtonDialogControl(100));
  dialog.add(new ButtonDialogControl(101));
  dialog.add(new ButtonDialogControl(102));
  dialog.add(new SplashButtonDialogControl(103));
  dialog.add(new SplashButtonDialogControl(104));
  dialog.add(new SplashButtonDialogControl(105));
  dialog.add(new SplashButtonDialogControl(106));
  dialog.add(new SplashButtonDialogControl(111));
  if (! licensed)
    dialog.add(new SplashButtonDialogControl(107));
  strcpy(filename,"");
  dialog.add(new ListDialogControl(110,n,filelist,filename,300));
  dialog.add((DisplayDialogControl *) new SplashDisplayDialogControl(108));
  dialog.add(new SplashCheckBoxDialogControl(109,&showsplash));

  for (; showsplash != 0 ;)
    {  selection = dialog.process(cadwindow->gethwnd());
       if (selection == 100)
         {  //  
            strcpy(filename,v.getstring("fi::lastfilename"));
            if (strlen(filename) > 0)
              {  GetFileTitle(filename,title,300);
                 if (strlen(filename) > strlen(title))
                   {  filename[strlen(filename)-strlen(title)-1] = 0;
                      _chdir(filename);
                   }
              }
            file_command2(&cstate,0,0);

            break;

         }
       else if (selection == 101 || selection == FALSE)
         {//  New - do nothing
            break;
         }
       else if (selection == 102)
         {ResourceString rsnc3(NC+3);
          RCCHAR oldfilename[300];

            strcpy(oldfilename,v.getstring("fi::lastfilename"));

            _chdir(home.getpublicfilename("Template-Drawings"));

            file_command2(&cstate,0,0);

            db.setname(rsnc3.gets());
            db.settitle(rsnc3.gets());
            cadwindow->updatetitle();
            db.setchanged();  //  Make sure user is asked to save

            //  Restore the old filename - don't want users to open templates directly
            v.setstring("fi::lastfilename",oldfilename);
            WritePrivateProfileString(_RCT("Strings"),_RCT("fi::lastfilename"),oldfilename,home.getinifilename());
            GetFileTitle(oldfilename,title,300);
            if (strlen(oldfilename) > strlen(title))
              {  oldfilename[strlen(oldfilename)-strlen(title)-1] = 0;
                 _chdir(oldfilename);
              }
            else
              _chdir(_RCT("c:\\"));

            break;

         }
       else if (selection == 110)
         {  for (i = 0 ; i < n ; i++)
              {  if (strcmp(filelist[i],filename) == 0)
                   {  sprintf(key,"op::file%d",i);
                      GetPrivateProfileString(_RCT("NameList"),key,_RCT(""),filename,300,home.getinifilename());
                      file_open(0,filename);
                      break;
                   }
              }
            break;
         } 

    }
#endif

  // autosave timer start
  if (v.getinteger("db::autosaveenabled") != 0)
    program->setautosavetimer();

  // acceletor table
  LoadAcceleratorTable();

//CCDEBUG
//db.StartCommandCallback = Start;
//db.EndCommandCallback = End;
  
  // wait for user input and process the messages
  //QEventLoop qprocessloop;
  //RCEventLoop qprocessloop;
  //#ifdef _USING_QT
    // show the Qt main window (temporary???)
    //QtMainWindow mw;
    //mw.show();

  //((QtMainWindow*)cadwindow->gethwnd())->showMaximized();
/*
  QWindowList winlist = app->allWindows();
  qDebug() << "windows";
  for(int iw=0; iw<winlist.size(); iw++)
  {
      QWindow *qwin = winlist.at(iw);
      qDebug() << "window (" << iw << ")        : " << qwin;
      qDebug() << "window (" << iw << ") parent : " << qwin->parent();
      qDebug() << "window (" << iw << ") name   : " << qwin->objectName();
      qDebug() << "window (" << iw << ") size   : " << qwin->geometry();
  }
  qDebug() << "---";
  QWidgetList wiglist = app->allWidgets();
  qDebug() << "widget";
  for(int iw=0; iw<wiglist.size(); iw++)
  {
      QWidget *wig = wiglist.at(iw);
      qDebug() << "widget (" << iw << ")        : " << wig;
      qDebug() << "widget (" << iw << ") parent : " << wig->parent();
      qDebug() << "widget (" << iw << ") name   : " << wig->objectName();
      qDebug() << "window (" << iw << ") size   : " << wig->geometry();
  }
  qDebug() << "---";
*/
  /*
    qDebug() << "listing cadwidow controls";
  int cntrls=0;
  Control *c=0;
  for (int i = 0 ; (c=cadwindow->getcontrol(i)) != 0 ; i++)
  {
      qDebug() << "control (" << i <<") : " << c;
      qDebug() << "control (" << i <<") : type name :" << typeid(c).name();
      qDebug() << "control (" << i <<") : parent    :" << c->parent;
      if(c->parent != 0)
          qDebug() << "control (" << i <<") : parent type :" << typeid(c->parent).name();

      if( dynamic_cast<Button*>(c) != 0 )
      {
          qDebug() << "button (" << i <<") : widget : " << ((Button*)c)->getQW();
          if(((Button*)c)->getQW() != 0 )
              qDebug() << "control (" << i <<") : widget name : " << ((QWidget*)((Button*)c)->getQW())->objectName();
      }
      else if(dynamic_cast<ButtonMenu*>(c) != 0)
      {
          qDebug() << "ButtonMenu (" << i <<") : widget : " << ((ButtonMenu*)c)->getQW();
          if(((ButtonMenu*)c)->getQW() != 0 )
              qDebug() << "control (" << i <<") : widget name : " << ((QWidget*)((ButtonMenu*)c)->getQW())->objectName();
      }
      else if(dynamic_cast<CoordinateEntry*>(c) != 0 )
      {
          qDebug() << "CoordinateEntry (" << i <<") : widget : " << ((CoordinateEntry*)c)->getQW();
          if(((CoordinateEntry*)c)->getQW() != 0 )
              qDebug() << "control (" << i <<") : widget name : " << ((QWidget*)((CoordinateEntry*)c)->getQW())->objectName();
      }
      else if(dynamic_cast<StyleEntry*>(c) != 0 )
      {
          qDebug() << "StyleEntry (" << i <<") : widget : " << ((StyleEntry*)c)->getQW();
          if(((StyleEntry*)c)->getQW() != 0 )
              qDebug() << "control (" << i <<") : widget name : " << ((QWidget*)((StyleEntry*)c)->getQW())->objectName();
      }
      else if(dynamic_cast<CoordinateDisplay*>(c) != 0 )
      {
          qDebug() << "CoordinateDisplay (" << i <<") : widget : " << ((CoordinateDisplay*)c)->getQW();
          if(((CoordinateDisplay*)c)->getQW() != 0 )
              qDebug() << "control (" << i <<") : widget name : " << ((QWidget*)((CoordinateDisplay*)c)->getQW())->objectName();
      }
      else if(dynamic_cast<Prompt*>(c) != 0 )
      {
          qDebug() << "Prompt (" << i <<") : widget : " << ((Prompt*)c)->getQW();
          if(((Prompt*)c)->getQW() != 0 )
              qDebug() << "control (" << i <<") : widget name : " << ((QWidget*)((Prompt*)c)->getQW())->objectName();
      }
      if(c->hDC != 0)
      {
          qDebug() << "control (" << i <<") : hDC widget      : " << c->hDC;
          qDebug() << "control (" << i <<") : hDC widget type : " << typeid(c->hDC).name();
          qDebug() << "control (" << i <<") : hDC widget name : " << ((QWidget*)c->hDC)->objectName();
      }
      if(c->parent->gethwnd() != 0)
      {
          qDebug() << "control (" << i <<") : parent widget        : " << c->parent->gethwnd();
          qDebug() << "control (" << i <<") : parent type name     : " << typeid(c->parent->gethwnd()).name();
          qDebug() << "control (" << i <<") : parent object name   : " << ((QWidget*)c->parent->gethwnd())->objectName();
      }
      qDebug() << "";
      cntrls++;
  }
  qDebug() << "cadwidow controls count : " << cntrls;
*/

#ifndef _MAC
  QString qcl;
  foreach( qcl, qcommandargs)
  {
      if(qcl.toLower().right(4) == ".cad")
      {
          file_open(0,qcl.data());
      }
      // KMJ: v8
#ifdef MAJOR_VERSION > 7
      else if(qcl.toLower().right(4) == ".dwg" ||
              qcl.toLower().right(4) == ".dxf" )
      {
          open_dwg(qcl.data());
      }
#endif
  }
#endif

#if 0 // new restore function
  ((QtMainWindow*)cadwindow->gethwnd())->setFocus();
  QSize pingsize = ((QtMainWindow*)cadwindow->gethwnd())->size();
  ((QtMainWindow*)cadwindow->gethwnd())->resize(pingsize.width(),pingsize.height()-1);
  ((QtMainWindow*)cadwindow->gethwnd())->resize(((QtMainWindow*)cadwindow->gethwnd())->size());
  ((QtMainWindow*)cadwindow->gethwnd())->show();
#else
  // which screen are we trying to use??
  qDebug() << "current screen" << app->desktop()->screenNumber((QtMainWindow*)cadwindow->gethwnd());
  // is this screen available?
  qDebug() << "screenCount" << app->desktop()->screenCount();
  //  trying to get the scene to resize so it looks right on some Macs??
  QByteArray winpos = settings.value("WindowPlacement/Main","").toByteArray();

  if(winpos.size() < 1)
  {
      ((QtMainWindow*)cadwindow->gethwnd())->show();
      //((QtMainWindow*)cadwindow->gethwnd())->on_action_Window_Show_Maximised_triggered();
      ((QtMainWindow*)cadwindow->gethwnd())->showMaximized();
      //((QtMainWindow*)cadwindow->gethwnd())->saveSettings();
  }
  else
  {
      //((QtMainWindow*)cadwindow->gethwnd())->move(200,200);
      //((QtMainWindow*)cadwindow->gethwnd())->resize(400,400);
      ((QtMainWindow*)cadwindow->gethwnd())->show();
      //((QtMainWindow*)cadwindow->gethwnd())->showNormal();
      //QSize pingsize = ((QtMainWindow*)cadwindow->gethwnd())->size();
      //((QtMainWindow*)cadwindow->gethwnd())->resize(pingsize);
  }
  // open plugins here after the main window is shown
  program->loaddefaultapps();
#endif

  // look for a FileOpenEvent item in the ini file or (Mac only)
  // look for the first cad file on the command line and try to load it
  // this is how the mac opens files with the open or open with command
  // windows uses the command line see the for windows code above
#ifdef _MAC
  if(!settings.value("FileOpenEvent/filename").toString().isEmpty())
  {
      //QMessageBox::warning(0,"","opening event file");
      QString filename = settings.value("FileOpenEvent/filename").toString();
      //db.initialize();
      //file_open(0,filename.data());
      if(filename.toLower().right(4) == ".cad")
      {
          //QMessageBox::warning(0,"","opening command line file (1)");
          //db.initialize();
          file_open(0,filename.data());
      }
      // KMJ: v8
#ifdef MAJOR_VERSION > 7
      else if (filename.toLower().right(4) == ".dwg" ||
               filename.toLower().right(4) == ".dxf" )
      {
          open_dwg(filename.data());
      }
#endif
      settings.setValue("FileOpenEvent/filename","");
  }
  else
  {
      QString qcl;
      foreach( qcl, qcommandargs)
      {
          if(qcl.toLower().right(4) == ".cad")
          {
              //QMessageBox::warning(0,"","opening command line file (2)");
              file_open(0,qcl.data());
          }
          // KMJ: v8
#ifdef MAJOR_VERSION > 7
          else if (qcl.toLower().right(4) == ".dwg" ||
                   qcl.toLower().right(4) == ".dxf" )
          {
              open_dwg(qcl.data());
          }
#endif
          settings.setValue("FileOpenEvent/filename","");
      }
  }
#endif

  //QMessageBox::warning(0,"","entering event loop");
  // for debugging screen resolution issues
#if 0
  QMessageBox::information(0,"Device Pixel Ratio",
                           QString("Window Device Pixel Ratio is : %1\nApplication Device Pixel Ration is : %2")
                           .arg(((QMainWindow*)cadwindow->gethwnd())->devicePixelRatio()).arg(app->devicePixelRatio()));
#endif

  // set the process idle timer
  //app->mIdleTimer = new QTimer(app);
  //app->connect(app->mIdleTimer, SIGNAL(timeout()), app, SLOT(onIdle()));
  //app->mIdleTimer->start();
  //
  // debugging
#if 0
  QObjectList mainkids = ((QtMainWindow*)cadwindow->gethwnd())->children();
  for(int imkids=0; imkids < mainkids.count(); imkids++)
  {
      qDebug() << "main child at(" << imkids << ") : " << mainkids.at(imkids)->objectName();
  }
#endif


  RCCentralWidget *rcw = (RCCentralWidget*)((QtMainWindow*)cadwindow->gethwnd())->centralWidget();
  qDebug() << "central widget : " << rcw->objectName();
  qDebug() << "central widget geometry : " << rcw->geometry();

#if 0
  QObjectList children = rcw->children();
  for(int icindex=0; icindex<children.size(); icindex++)
  {
      qDebug() << "child at[" << icindex << "] : " << children.at(icindex);
  }

  QVBoxLayout *vbl = rcw->mLayout;
  qDebug() << "central widget layout : " << vbl;
  for(int ilindex=0; ilindex < vbl->count(); ilindex++)
  {
      if(qobject_cast<QWidget*>(vbl->itemAt(ilindex)->widget()) != 0)
      {
          qDebug() << "layout widget at [" << ilindex << "] : " << vbl->itemAt(ilindex)->widget();
          if(qobject_cast<QSplitter*>(vbl->itemAt(ilindex)->widget()) != 0)
          {
              QSplitter *spl = (QSplitter*)vbl->itemAt(ilindex)->widget();
              for(int isindex=0; isindex < spl->count(); isindex++)
              {
                  qDebug() << "splitter widget at [" << isindex << "] : " << spl->widget(isindex);
                  if(qobject_cast<RCView*>(vbl->itemAt(ilindex)->widget()) != 0)
                  {
                      qDebug() << "splitter view at [" << isindex << "] : " << spl->widget(isindex)->objectName();
                      RCView *view = (RCView*)spl->widget(isindex);
                      qDebug() << "splitter view geometry     : " << view->geometry();
                      qDebug() << "splitter view3d     : " << view->getView3dWindow();
                      qDebug() << "splitter view3d scene  : " << view->getView3dWindow()->getScene();
                      qDebug() << "splitter view3d width  : " << view->getView3dWindow()->getwidth();
                      qDebug() << "splitter view3d height : " << view->getView3dWindow()->getheight();

                      qDebug() << "splitter view gv geometry  : " << view->view()->geometry();
                      qDebug() << "splitter view gv viewport  : " << view->view()->viewport();
                      qDebug() << "splitter view gv viewport geometry : " << view->view()->viewport()->geometry();
                      qDebug() << "splitter view gv scenerect : " << view->view()->sceneRect();
                      qDebug() << "splitter view gv scene     : " << view->view()->scene();
                      qDebug() << "splitter view gv scene scenerect : " << view->view()->scene()->sceneRect();
                  }
              }
          }
      }
  }
#endif

  RCView *activeView = ((QtMainWindow*)cadwindow->gethwnd())->activeView();
  qDebug() << "active view : " << activeView->objectName();
  qDebug() << "active view geometry : " << activeView->geometry();

  // KMJ: new splash screen or start up dialog
  if(licensed == 0 || v.getinteger("wn::showstartupdialog"))
      ((QtMainWindow*)cadwindow->gethwnd())->showSplashScreen(licensed);

    rcw->debugView3dList();
    rcw->debug();

    // testing transform decompose
    Transform t1,rotx,roty,rotz;

    t1.rotatex(15 * M_PI / 180.0 ); rotx.apply(t1);
    t1.rotatey(30 * M_PI / 180.0 ); roty.apply(t1);
    t1.rotatez(60 * M_PI / 180.0 ); rotz.apply(t1);

    t1.identity();
    t1.apply(rotz);
    t1.apply(roty);
    t1.apply(rotx);

    double rotateXangle=0.0,rotateYangle=0.0,rotateZangle=0.0;
    Transform::extractEulerAngleXYZ(t1,rotateXangle,rotateYangle,rotateZangle);

    rotateXangle *= 180.0 / M_PI;
    rotateYangle *= 180.0 / M_PI;
    rotateZangle *= 180.0 / M_PI;

    qDebug() << "angle X : " << rotateXangle;
    qDebug() << "angle Y : " << rotateYangle;
    qDebug() << "angle Z : " << rotateZangle;

    //
    //qDebug() << "Entering the event loop";
  int pret = 0;
  app->eventLoopStarted = true;
  pret = app->exec();
  app->eventLoopStarted = false;
  qDebug() << "Done";
  //#else
    /*
     KMJ : this loop is in the RCApplication::notify() function
  for (;;)
  {
       if ((e = commandfile.getnextevent()) == 0)
       {
           if (! program->processmessages(1))
               break;
       }
       else
       {
            state.sendevent(e);
            state.process();
            while ((status = program->processmessages(2)) == 1) ;
            if (status == 0)
              break;
       }
  }
*/
//#endif

  // close down stuff
  // remember the last main window geometry
  // this could be put in the resize event function
  // but that might be a bit slow
  // now done in saveSettings() function from onClose();
#if 0
  if( ! ((QWidget*)cadwindow->gethwnd())->isMinimized())
  {
      WINDOWPLACEMENT wp;
      RCCHAR swp[300];
      RCWPARAM wParam;
      GetWindowPlacement(cadwindow->gethwnd(),&wp);
      sprintf(swp,"%d,%d,%d,%d,%d",
         wp.rcNormalPosition.left,wp.rcNormalPosition.top,
         wp.rcNormalPosition.right - wp.rcNormalPosition.left,
         wp.rcNormalPosition.bottom - wp.rcNormalPosition.top,
         wp.showCmd);
      qDebug() << "saving window placement : " << QString(swp);
      WritePrivateProfileString(_RCT("WindowPlacement"),_RCT("Main"),swp,home.getinifilename());
  }
#endif


  GetTempPath(300,filename);
  strcat(filename,"RealCAD.CAD");
  strcpy(filename,QDir::toNativeSeparators(QString(filename)));
  _unlink(filename);  //  Delete the backup file when the software exists normally.


  /*
   * KMJ: look into this
  if (GetPrivateProfileInt("Exit","FastExit",0,home.getinifilename()) != 0)
#ifdef _MAC
      _Exit(pret);
#else
      _exit(0);
#endif
  */

  CloseInstance();
  // catch any exceptions
#if 0
  try
  {
    delete app;
  }
  catch(...)
  {
      qDebug() << "Caught an exception";
  }
#endif


#ifdef _MAC
  _Exit(pret);
#else
  _exit(pret);
#endif
  return pret;
}

void DebugTraceLine(const char *filename,int linenumber)
{
#if defined(DEBUGEXIT)
  FILE *dfile;
  if ((dfile = fopen("c:\\RealCADLog\\exitlog.log","a")) == 0)
    {  _mkdir("c:\\RealCADLog");
       dfile = fopen("c:\\RealCADLog\\exitlog.log","a");
    }

  if (dfile != 0)
    {  fprintf(dfile,"%s : %d\n",filename,linenumber);
       fclose(dfile);
    }
#endif

}
/*
    KMJ : added since 2016/08
*/

/*
  KMJ: added

  command to select all entities with the same colour as the current selection

*/
void selectsamecolour_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    state.destroyselection(0);
    if ((sele = sellist.next()) != 0)
    {
        sellist.unhighlight();
        sellist.destroy();
        sellist.add(sele);
        sellist.highlight();
        sellist.start();

        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->getcolour() == sele->getcolour())
                {
                    sellist.add(e);
                    e->highlight();
                }
            }
            if (program->getinterrupt()) break;
        }

        state.setselection(sellist,1);

        program->setbusy(0);
    }
    *cstate = ExitState;
}

/*
  KMJ: added

  command to select all entities on the same layer as the current selection

*/
void selectsamelayer_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    state.destroyselection(0);
    if ((sele = sellist.next()) != 0)
    {
        sellist.unhighlight();
        sellist.destroy();
        sellist.add(sele);
        sellist.highlight();
        sellist.start();

        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->getlayer() == sele->getlayer())
                {
                    sellist.add(e);
                    e->highlight();
                }
            }
            if (program->getinterrupt()) break;
        }

        state.setselection(sellist,1);

        program->setbusy(0);
    }
    *cstate = ExitState;
}

/*
  KMJ: added

  command to select all entities with the same style as the current selection

*/
void selectsamestyle_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    state.destroyselection(0);
    if ((sele = sellist.next()) != 0)
    {
        sellist.unhighlight();
        sellist.destroy();
        sellist.add(sele);
        sellist.highlight();
        sellist.start();

        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->getstyle() == sele->getstyle())
                {
                    sellist.add(e);
                    e->highlight();
                }
            }
            if (program->getinterrupt()) break;
        }

        state.setselection(sellist,1);

        program->setbusy(0);
    }
    *cstate = ExitState;
}

/*
  KMJ: added

  command to select all entities with the same weight as the current selection

*/
void selectsameweight_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    state.destroyselection(0);
    if ((sele = sellist.next()) != 0)
    {
        sellist.unhighlight();
        sellist.destroy();
        sellist.add(sele);
        sellist.highlight();
        sellist.start();

        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->getweight() == sele->getweight())
                {
                    sellist.add(e);
                    e->highlight();
                }
            }
            if (program->getinterrupt()) break;
        }

        state.setselection(sellist,1);

        program->setbusy(0);
    }
    *cstate = ExitState;
}

/*
  KMJ: added

  command to select all entities with the same entity type as the current selection

*/
void selectsameentity_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    state.destroyselection(0);
    if ((sele = sellist.next()) != 0)
    {
        sellist.unhighlight();
        sellist.destroy();
        sellist.add(sele);
        sellist.highlight();
        sellist.start();

        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->type() == sele->type())
                {
                    sellist.add(e);
                    e->highlight();
                }
            }
            if (program->getinterrupt()) break;
        }

        state.setselection(sellist,1);

        program->setbusy(0);
    }
    *cstate = ExitState;
}

/*
  KMJ: added

  command to select all entities with the same name as the current selection

  The same name refers to the figure name or the group name

*/
void selectsamename_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    state.destroyselection(0);
    if ((sele = sellist.next()) != 0)
    {
        sellist.unhighlight();
        sellist.destroy();
        sellist.add(sele);
        sellist.highlight();
        sellist.start();

        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->type() == figure_entity  && sele->type() == figure_entity)
                {
                    if(strcmp(((Figure*)e)->getfilename(),((Figure*)sele)->getfilename()) == 0)
                    {
                        sellist.add(e);
                        e->highlight();
                    }
                }
                else if(e->type() == group_entity  && sele->type() == group_entity)
                {
                    if(strcmp(((Group*)e)->getclassname(),((Group*)sele)->getclassname()) == 0)
                    {
                        sellist.add(e);
                        e->highlight();
                    }
                }
            }
            if (program->getinterrupt()) break;
        }

        state.setselection(sellist,1);

        program->setbusy(0);
    }
    *cstate = ExitState;
}


/*
  KMJ: added

  command to hide all entities with the same colour as the current selection

*/
void hidesamecolour_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;
    BitMask colours(256);
    colours = db.dismask.colour;

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    sele->unhighlight();
    state.destroyselection(0);
    cadwindow->drawent(DM_ERASE,0,sele);

    if (sele != 0)
        colours.clear(sele->getcolour());

    db.dismask.colour = colours;
    cadwindow->paintall();
    program->setbusy(0);

    *cstate = ExitState;
#if 0
    Entity *sele,*e;
    EntityList sellist;
    BitMask visible(MaxViews);
    visible.clearall();

    program->setbusy(1);
    cadwindow->startdrawingbatch();

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    sele->unhighlight();
    state.destroyselection(0);
    if (sele != 0)
    {
        cadwindow->drawent(DM_ERASE,0,sele);
        sele->setvisible(visible);
        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->getcolour() == sele->getcolour())
                {
                    cadwindow->drawent(DM_ERASE,0,e);
                    e->setvisible(visible);
                }
            }
            if (program->getinterrupt()) break;
        }
        cadwindow->enddrawingbatch();
        cadwindow->paintall();
        program->setbusy(0);
    }
    *cstate = ExitState;
#endif
}

/*
  KMJ: added

  command to hide all entities on the same layer as the current selection

*/
void hidesamelayer_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;
    BitMask layers(MAX_LAYERS);
    layers = db.dismask.layer;

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    sele->unhighlight();
    state.destroyselection(0);
    cadwindow->drawent(DM_ERASE,0,sele);

    if (sele != 0)
        layers.clear(sele->getlayer());

    db.dismask.layer = layers;
    cadwindow->paintall();
    program->setbusy(0);

    *cstate = ExitState;
#if 0
    Entity *sele,*e;
    EntityList sellist;
    BitMask visible(MaxViews);
    visible.clearall();

    program->setbusy(1);
    cadwindow->startdrawingbatch();

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    sele->unhighlight();
    state.destroyselection(0);
    if (sele != 0)
    {
        cadwindow->drawent(DM_ERASE,0,sele);
        sele->setvisible(visible);
        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->getlayer() == sele->getlayer())
                {
                    cadwindow->drawent(DM_ERASE,0,e);
                    e->setvisible(visible);
                }
            }
            if (program->getinterrupt()) break;
        }
        cadwindow->enddrawingbatch();
        cadwindow->paintall();
        program->setbusy(0);
    }
    *cstate = ExitState;
#endif
}

/*
  KMJ: added

  command to hide all entities with the same style as the current selection

*/
void hidesamestyle_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;
    BitMask styles(256);
    styles = db.dismask.style;

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    sele->unhighlight();
    state.destroyselection(0);
    cadwindow->drawent(DM_ERASE,0,sele);

    if (sele != 0)
        styles.clear(sele->getstyle());

    db.dismask.style = styles;
    cadwindow->paintall();
    program->setbusy(0);

    *cstate = ExitState;

#if 0
    Entity *sele,*e;
    EntityList sellist;
    BitMask visible(MaxViews);
    visible.clearall();

    program->setbusy(1);
    cadwindow->startdrawingbatch();

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    sele->unhighlight();
    state.destroyselection(0);
    if (sele != 0)
    {
        cadwindow->drawent(DM_ERASE,0,sele);
        sele->setvisible(visible);
        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->getstyle() == sele->getstyle())
                {
                    cadwindow->drawent(DM_ERASE,0,e);
                    e->setvisible(visible);
                }
            }
            if (program->getinterrupt()) break;
        }
        cadwindow->enddrawingbatch();
        cadwindow->paintall();
        program->setbusy(0);
    }
    *cstate = ExitState;
#endif
}

/*
  KMJ: added

  command to hide all entities with the same weight as the current selection

*/
void hidesameweight_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;
    BitMask weights(256);
    weights = db.dismask.weight;

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    sele->unhighlight();
    state.destroyselection(0);
    cadwindow->drawent(DM_ERASE,0,sele);

    if (sele != 0)
        weights.clear(sele->getweight());

    db.dismask.weight = weights;
    cadwindow->paintall();
    program->setbusy(0);

    *cstate = ExitState;

#if 0
    Entity *sele,*e;
    EntityList sellist;
    BitMask visible(MaxViews);
    visible.clearall();

    program->setbusy(1);
    cadwindow->startdrawingbatch();

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    sele->unhighlight();
    state.destroyselection(0);
    if (sele != 0)
    {
        cadwindow->drawent(DM_ERASE,0,sele);
        sele->setvisible(visible);
        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->getweight() == sele->getweight())
                {
                    cadwindow->drawent(DM_ERASE,0,e);
                    e->setvisible(visible);
                }
            }
            if (program->getinterrupt()) break;
        }
        cadwindow->enddrawingbatch();
        cadwindow->paintall();
        program->setbusy(0);
    }
    *cstate = ExitState;
#endif
}

/*
  KMJ: added

  command to hide all entities with the same entity type as the current selection

*/
void hidesameentity_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;
    BitMask entities(32);
    entities = db.dismask.entity;

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    sele->unhighlight();
    if(sele->type() == group_entity)
    {
        Entity *ge;
        EntityList glist = ((Group*)sele)->getlist();
        for(glist.start(); (ge = glist.next()) != NULL; )
        {
            ge->unhighlight();
            cadwindow->drawent(DM_ERASE,0,sele);
        }
    }
    state.destroyselection(0);
    cadwindow->drawent(DM_ERASE,0,sele);

    //qDebug() << "Entity type" << sele->type();

    if (sele != 0)
    {
        switch(sele->type())
        {
        case lineardimension_entity :
        case angulardimension_entity :
        case distancebearingdimension_entity:
        case circulardimension_entity :
            entities.clear(dimension_mask);
            break;
        default :
            entities.clear(sele->type());
        }
    }

    db.dismask.entity = entities;
    cadwindow->paintall();
    program->setbusy(0);

    *cstate = ExitState;

#if 0
    Entity *sele,*e;
    EntityList sellist;
    BitMask visible(MaxViews);
    visible.clearall();

    program->setbusy(1);
    cadwindow->startdrawingbatch();

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    sele->unhighlight();
    state.destroyselection(0);
    if (sele != 0)
    {
        cadwindow->drawent(DM_ERASE,0,sele);
        sele->setvisible(visible);
        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->type() == sele->type())
                {
                    cadwindow->drawent(DM_ERASE,0,e);
                    e->setvisible(visible);
                }
            }
            if (program->getinterrupt()) break;
        }
        cadwindow->enddrawingbatch();
        cadwindow->paintall();
        program->setbusy(0);
    }
    *cstate = ExitState;
#endif
}

/*
  KMJ: added

  command to hide all entities with the same name as the current selection

  The same name refers to the figure name or the group name

  NOTE: this command is not used pending a solution to the view visibility issue

*/
void hidesamename_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *sele,*e;
    EntityList sellist;
    BitMask visible(MaxViews);
    visible.clearall();

    program->setbusy(1);
    cadwindow->startdrawingbatch();

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    sele->unhighlight();
    state.destroyselection(0);
    if(sele->type() == figure_entity)
    {
        cadwindow->drawent(DM_ERASE,0,sele);
        sele->setvisible(visible);
    }
    else if(sele->type() == group_entity)
    {
        cadwindow->drawent(DM_ERASE,0,sele);
        sele->setvisible(visible);

        Entity *ge;
        EntityList glist = ((Group*)sele)->getlist();
        for(glist.start(); (ge = glist.next()) != NULL; )
        {
            ge->unhighlight();
            cadwindow->drawent(DM_ERASE,0,ge);
            ge->setvisible(visible);
        }
    }
    if (sele != 0)
    {
        for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
        {
            if (e != sele && e->isvisible(0))
            {
                if(e->type() == figure_entity  && sele->type() == figure_entity)
                {
                    if(strcmp(((Figure*)e)->getfilename(),((Figure*)sele)->getfilename()) == 0)
                    {
                        cadwindow->drawent(DM_ERASE,0,e);
                        e->setvisible(visible);
                    }
                }
                else if(e->type() == group_entity  && sele->type() == group_entity)
                {
                    if(strcmp(((Group*)e)->getclassname(),((Group*)sele)->getclassname()) == 0)
                    {
                        cadwindow->drawent(DM_ERASE,0,e);
                        e->setvisible(visible);
                        Entity *ge;
                        EntityList glist = ((Group*)e)->getlist();
                        for(glist.start(); (ge = glist.next()) != NULL; )
                        {
                            cadwindow->drawent(DM_ERASE,0,ge);
                            ge->setvisible(visible);
                        }
                    }
                }
            }
            if (program->getinterrupt()) break;
        }

        cadwindow->enddrawingbatch();
        cadwindow->paintall();

        program->setbusy(0);
    }
    *cstate = ExitState;
}

/*
 KMJ: added

 command to unhide all entities hidden with the hide same entity name command

*/
void unhideallentities_command(int *cstate,HighLevelEvent *,void **)
{
    Entity *e,*sele;
    EntityList sellist;
    BitMask visible(MaxViews);
    //visible.setall();
    visible = *db.header.getvisible();

    program->setbusy(1);

    sellist = state.getselection().copy();
    sellist.start();
    sele = sellist.next();
    if(sele != 0)
        sele->unhighlight();
    state.destroyselection(0);

    db.dismask.entity.set(point_entity,1);
    db.dismask.entity.set(line_entity,1);
    db.dismask.entity.set(circle_entity,1);
    db.dismask.entity.set(ellipse_entity,1);
    db.dismask.entity.set(curve_entity,1);
    db.dismask.entity.set(plane_entity,1);
    db.dismask.entity.set(ruledsurface_entity,1);
    db.dismask.entity.set(surfaceofrev_entity,1);
    db.dismask.entity.set(text_entity,1);
    db.dismask.entity.set(figure_entity,1);
    db.dismask.entity.set(group_entity,1);
    db.dismask.entity.set(dimension_mask,1);
    db.dismask.entity.set(light_entity,1);
    db.dismask.entity.set(patch_entity,1);
    db.dismask.entity.set(bitmap_entity,1);

    db.dismask.colour.setall();
    db.dismask.layer.setall();
    db.dismask.style.setall();
    db.dismask.weight.setall();

    /*
     // NOTE: pending a solution to the view visibility issue
    for (db.geometry.start(); (e = db.geometry.next()) != NULL ; )
    {
        if(e->type() == figure_entity)
        {
            e->setvisible(visible);
        }
        else if(e->type() == group_entity)
        {
            e->setvisible(visible);
            Entity *ge;
            EntityList glist = ((Group*)e)->getlist();
            for(glist.start(); (ge = glist.next()) != NULL; )
            {
                ge->setvisible(visible);
            }
        }
        if (program->getinterrupt()) break;
    }
    */

    cadwindow->paintall();

    program->setbusy(0);

    *cstate = ExitState;
}
