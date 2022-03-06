
#include "ncwin.h"
#include <QGLWidget>
#include "RCView.h"

//
//  options.test(11)    Always draw, even when hiding.
//
Text::Text(char *f,Point org,Point xa,Point ya,double w,double h,double s,double a,Byte hc,char *t,char *l,const BitMask &o) :
    options(32)
{
    //Text(CHAR2RCCHR(f),org,xa,ya,w,h,s,a,hc,CHAR2RCCHR(t),CHAR2RCCHR(l),o);
#ifdef USING_WIDGETS
    qgbox=0;
    qgfill=0;

    tmptext=0;
    tmplabel=0;
    tmpfont=0;
#endif

    if ((font = new RCCHAR[RCSTRLEN(f)+1]) != NULL)
        RCSTRCPY(font,f);
    else {
        defined = 0;
        return;
    }
    origin = org;
    xaxis = xa;
    yaxis = ya;
    options = o;
    width = w;
    height = h;
    vertspacing = s;
    angle = a;
    if ((text = new RCCHAR[RCSTRLEN(t)+1]) != NULL)
        RCSTRCPY(text,t);
    else {
        delete [] font;
        defined = 0;
        return;
    }

    if (l != 0) {
        if ((label = new RCCHAR[RCSTRLEN(l)+1]) != NULL)
            RCSTRCPY(label,l);
        else {
            delete [] font;
            delete [] text;
            defined = 0;
            return;
        }
    } else
        label = 0;

    halocolour = hc;
}

Text::Text(RCCHAR *f,Point org,Point xa,Point ya,double w,double h,double s,double a,Byte hc,RCCHAR *t,RCCHAR *l,const BitMask &o) :
    options(32)
{
#ifdef USING_WIDGETS
    qgbox=0;
    qgfill=0;

    tmptext=0;
    tmplabel=0;
    tmpfont=0;
#endif

    if ((font = new RCCHAR[RCSTRLEN(f)+1]) != NULL)
        RCSTRCPY(font,f);
    else {
        defined = 0;
        return;
    }
    origin = org;
    xaxis = xa;
    yaxis = ya;
    options = o;
    width = w;
    height = h;
    vertspacing = s;
    angle = a;
    if ((text = new RCCHAR[RCSTRLEN(t)+1]) != NULL)
        RCSTRCPY(text,t);
    else {
        delete [] font;
        defined = 0;
        return;
    }

    if (l != 0 && strlen(l) > 0) {
        if ((label = new RCCHAR[RCSTRLEN(l)+1]) != NULL)
            RCSTRCPY(label,l);
        else {
            delete [] font;
            delete [] text;
            defined = 0;
            return;
        }
    } else
        label = 0;

    halocolour = hc;

}

Text::Text(const EntityHeader &header,RCCHAR *f,Point org,Point xa,Point ya,double w,double h,double s,double a,Byte hc,RCCHAR *t,RCCHAR *l,const BitMask &o) :
    Entity(header),options(32)
{
#ifdef USING_WIDGETS
    qgbox=0;
    qgfill=0;

    tmptext=0;
    tmplabel=0;
    tmpfont=0;
#endif

    if ((font = new RCCHAR[RCSTRLEN(f)+1]) != NULL)
        RCSTRCPY(font,f);
    else {
        defined = 0;
        return;
    }
    origin = org;
    xaxis = xa;
    yaxis = ya;
    options = o;
    width = w;
    height = h;
    vertspacing = s;
    angle = a;
    if ((text = new RCCHAR[RCSTRLEN(t)+1]) != NULL)
        RCSTRCPY(text,t);
    else {
        delete [] font;
        defined = 0;
        return;
    }
    if (l != 0 && strlen(l) > 0) {
        if ((label = new RCCHAR[RCSTRLEN(l)+1]) != NULL)
            RCSTRCPY(label,l);
        else {
            delete [] font;
            delete [] text;
            defined = 0;
            return;
        }
    } else
        label = 0;

    halocolour = hc;

}

Text::Text(const EntityHeader &header,char *f,Point org,Point xa,Point ya,double w,double h,double s,double a,Byte hc,char *t,char *l,const BitMask &o) :
    Entity(header),options(32)
{
#ifdef USING_WIDGETS
    qgbox=0;
    qgfill=0;

    tmptext=0;
    tmplabel=0;
    tmpfont=0;
#endif

    if ((font = new RCCHAR[RCSTRLEN(f)+1]) != NULL)
        RCSTRCPY(font,f);
    else {
        defined = 0;
        return;
    }
    origin = org;
    xaxis = xa;
    yaxis = ya;
    options = o;
    width = w;
    height = h;
    vertspacing = s;
    angle = a;
    if ((text = new RCCHAR[RCSTRLEN(t)+1]) != NULL)
        RCSTRCPY(text,t);
    else {
        delete [] font;
        defined = 0;
        return;
    }
    if (l != 0 && strlen(l) > 0) {
        if ((label = new RCCHAR[RCSTRLEN(l)+1]) != NULL)
            RCSTRCPY(label,l);
        else {
            delete [] font;
            delete [] text;
            defined = 0;
            return;
        }
    } else
        label = 0;

    halocolour = hc;
}

Text::~Text()
{
    delete [] font;
    delete [] text;
    delete [] label;

#ifdef USING_WIDGETS
    delete [] tmptext;
    delete [] tmplabel;
    delete [] tmpfont;
#endif
}

double Text::pick(View3dSurface *v,double x,double y,Point *pnear)
{
#ifdef USING_WIDGETS
    Point p1,p2,p3,p4,p5,zero(0.0,0.0,0.0);
    RCHFONT oldhfont,hfont;
    //BYTE fbCharSet;
    int ih,iw,maxw,maxh,w,ll,nlines;
    double scale,oheight,rh,rw,a,l,y1,cosa,sina,dir1,dir2,dir3,dir4,xmaxw,xmaxh,xw,tmHeight,tmDescent,tmAscent;
    //TEXTMETRICW tm;
    Point o,xa,ya,co;
    RCCHAR *s;
    Transform t;
    View3dWindow *window;

    //window = dynamic_cast<View3dWindow *>(v);
    //if (window != 0 && window->getoffscreenbitmap() != 0)
    //    v = window->getoffscreenbitmap();

    if (pnear == NULL && v->displaylist.defined(this))
        return v->displaylist.pick(this,x,y);

#if 0
    if (options.test(10)) {
        xa = xaxis;
        ya = yaxis;
        t.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
        xa = t.transform(xa);
        ya = t.transform(ya);
        o = origin;
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
        hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                            options.test(7),options.test(6),0,fbCharSet,
                            OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);

        if (hfont != NULL)
            oldhfont = (RCHFONT)SelectObject(v->gethdc(),hfont);
        GetTextMetricsW(v->gethdc(),&tm);

        if (tm.tmHeight == 0) tm.tmHeight = 1;
        if (options.test(9))
            scale = v->getplotscale() / double(tm.tmHeight) * height;
        else
            scale = height / double(tm.tmHeight);
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
        else if (options.test(3))
            oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
        else
            oheight = -(tm.tmHeight - tm.tmDescent) * scale;
        xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
        xmaxw = 0;
        y1 = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            SIZE size;
            GetTextExtentPoint32W(v->gethdc(),s,ll,&size);

            xw = size.cx * scale;

            if (xw > xmaxw) xmaxw = xw;
            y1 -= (tm.tmHeight * (1.0 + vertspacing)) * scale;
            if (s[ll] == 0) break;
        }
        p1 = o + ya * (oheight + y1 - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
        if (options.test(1))
            p1 = p1 - xa * xmaxw / 2.0;
        else if (options.test(2))
            p1 = p1 - xa * xmaxw;
        p2 = p1 + xa * xmaxw;
        p3 = p2 + ya * xmaxh;
        p4 = p3 - xa * xmaxw;
        p1 = v->modeltoscreen(p1);
        p2 = v->modeltoscreen(p2);
        p3 = v->modeltoscreen(p3);
        p4 = v->modeltoscreen(p4);
        p5.setxyz(x,y,0.0);
        dir1 = (p2 - p1).cross(p5 - p1).z;
        dir2 = (p3 - p2).cross(p5 - p2).z;
        dir3 = (p4 - p3).cross(p5 - p3).z;
        dir4 = (p1 - p4).cross(p5 - p4).z;
        if (hfont != NULL) {
            SelectObject(v->gethdc(),oldhfont);
            DeleteObject(hfont);
        }
        if (xmaxw > 0.0 && xmaxh > 0.0 &&
                (dir1 <= 0.0 && dir2 <= 0.0 && dir3 <= 0.0 && dir4 <= 0.0 ||
                 dir1 >= 0.0 && dir2 >= 0.0 && dir3 >= 0.0 && dir4 >= 0.0)) {
            if (pnear != NULL) *pnear = origin;
            return 0.0;
        }
    } else
#endif
    {
        p1 = v->modeltoscreen(origin);
        //if (options.test(0))
        //  fbCharSet = SYMBOL_CHARSET;
        //else
        //  fbCharSet = ANSI_CHARSET;
        xa = v->modeltouv(origin + xaxis) - v->modeltouv(origin);
        if ((l = xa.length()) < db.getptoler()) return -1.0;
        xa /= l;
        if (xa.x < -1.0) xa.x = -1.0;
        else if (xa.x > 1.0) xa.x = 1.0;
        a = acos(xa.x);
        if (xa.y < 0.0) a = 2.0 * M_PI - a;

        if (options.test(9))
            scale = v->getplotscale();
        else
            scale = 1.0;

        rh = height * v->getheight() / (v->getvmax() - v->getvmin()) * scale;
        if (options.test(13))  // Height is in points
            rh *= 25.4 / 72.0;
        rw = width * v->getwidth() / (v->getumax() - v->getumin()) * scale;
        if (rh > MAXTEXTHEIGHT) ih = (int)MAXTEXTHEIGHT;
        else if (rh < -MAXTEXTHEIGHT) ih = (int)-MAXTEXTHEIGHT;
        else ih = int(rh);
        if (rw > MAXTEXTHEIGHT) iw = (int)MAXTEXTHEIGHT;
        else if (rw < -MAXTEXTHEIGHT) iw = (int)-MAXTEXTHEIGHT;
        else iw = int(rw);
        if (ih == 0) return -1.0;

        //hfont = CreateFontW(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + v->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + v->getrotation2d())* 10.0),
        //                   (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);

        // calculate the Qt font pixel height required for the text height setting
        // calculate the point height for the font
        QString refName(font);
        QFont refFont(refName);
        refFont.setPointSize((int)rh);// use rh as points
        QFontMetrics refFm(refFont);
        int refHeight = refFm.height();// get the height as pixels for rh points
        // how many points did we get for for 1 pixel of rh
        double ptsperpx = rh / refHeight;
        // how many points to get the same height in pixels
        int fontHeight = (int)(rh * ptsperpx);

        if(fontHeight < 1.0)
            return -1.0;

        // set up a device independent font
        QString qfname(font);
        QFont qfont(qfname);
        qfont.setStyleStrategy(QFont::ForceOutline);
        qfont.setPointSize(fontHeight);
        // set the font weight
        if(options.test(5))
            qfont.setWeight(QFont::Bold);
        else
            qfont.setWeight(QFont::Normal);
        // set italic
        qfont.setItalic(options.test(7));
        // set underline
        qfont.setUnderline(options.test(6));

        // get the font metrics
        QFontMetricsF fm(qfont);

        // calculate the strtch factor
        // this is done by scaling the font in width
        //double fstretch = width  / (height > 0.0 ? height : 1.0);
        //fstretch = fstretch == 0.0 ? 1.0 : fstretch;
        double fstretch = 1.0;
        if(width == 0.0)
            fstretch = 1.0;
        else
            fstretch = fabs(width);

        tmHeight = fm.height();
        tmDescent = fm.descent();
        tmAscent = fm.ascent();

        sina = sin(a + angle / 180.0 * M_PI);
        cosa = cos(a + angle / 180.0 * M_PI);

        //if (hfont != NULL)
        //  oldhfont = (RCHFONT) SelectObject(v->gethdc(),hfont);
        //GetTextMetricsW(v->gethdc(),&tm);

        //for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        //for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += (*s == 0x000D || *s == 0x000A);
        // calculate the number of lines
        // text on windows have CR+LF line endings MAC OS X has LF line endings
        // find the line feed or carriage return or line feed and carriage return
        // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
        for (s = text,nlines = 1 ; *s!=0 ; s++) {
            nlines += (*s == 0x000D || *s == 0x000A) ;
            if(*s == 0x000D && (*s+1 != 0) && *(s+1) == 0x000A)
                s++;
        }


        if (! options.test(3) && ! options.test(4))
            oheight = -nlines * tmHeight + tmDescent - (nlines - 1) * tmHeight * vertspacing;
        else if (options.test(3))
            oheight = -(nlines + (nlines - 1) * vertspacing) * tmHeight / 2;
        else
            oheight = 0.0;
        p1.x += oheight * sina;
        p1.y -= oheight * cosa;
        p5 = p1;

        maxh = int(tmHeight * (nlines + (nlines - 1)*vertspacing));
        maxw = 0;

        for (s = text ; *s!=0 ;  s += ll + 1) {
            //for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            //for (ll = 0 ;  (s[ll] != 0x000D) && (s[ll] != 0x000A) && (s[ll] != 0) ; ll++) ;
            // text on windows have CR+LF line endings MAC OS X has LF line endings
            // find the line feed or carriage return or line feed and carriage return
            // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
            for (ll = 0 ; s[ll] != 0; ll++) {
                if(s[ll] == 0x000D || s[ll] == 0x000A) {
                    if(s[ll] == 0x000D && s[ll+1] != 0 && s[ll+1] == 0x000A)
                        ll++;
                    break;
                }
            }

            //SIZE size;
            //GetTextExtentPoint32W(v->gethdc(),s,ll,&size);
            QString qs1(s);
            w = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,qs1).width();

            if (w > maxw) maxw = w;
            p2 = p1;
            if (options.test(1)) {
                p2.x -= w * fstretch * cosa / 2.0;
                p2.y -= w * fstretch * sina / 2.0;
            } else if (options.test(2)) {
                p2.x -= w * fstretch * cosa;
                p2.y -= w * fstretch * sina;
            }
            p1.x += tmHeight * (1.0 * vertspacing) * sina;
            p1.y -= tmHeight * (1.0 + vertspacing) * cosa;
            if (s[ll] == 0) break;
        }
        if (options.test(1)) {
            p5.x -= maxw * fstretch * cosa / 2.0;
            p5.y -= maxw * fstretch * sina / 2.0;
        } else if (options.test(2)) {
            p5.x -= maxw * fstretch * cosa;
            p5.y -= maxw * fstretch * sina;
        }
        p1 = p5;
        p2.setxyz(p5.x + maxw * fstretch * cosa,p5.y + maxw * fstretch * sina,0.0);
        p3.setxyz(p5.x + maxw * fstretch * cosa + maxh * sina,p5.y + maxw * fstretch * sina - maxh * cosa,0.0);
        p4.setxyz(p5.x + maxh * sina,p5.y - maxh * cosa,0.0);
        p5.setxyz(x,y,0.0);
        dir1 = (p2 - p1).cross(p5 - p1).z;
        dir2 = (p3 - p2).cross(p5 - p2).z;
        dir3 = (p4 - p3).cross(p5 - p3).z;
        dir4 = (p1 - p4).cross(p5 - p4).z;

        /*
        if (hfont != NULL)
          {  SelectObject(v->gethdc(),oldhfont);
             DeleteObject(hfont);
          }
        */
        if (maxw > 0 && maxh > 0 &&
                (dir1 <= 0.0 && dir2 <= 0.0 && dir3 <= 0.0 && dir4 <= 0.0 ||
                 dir1 >= 0.0 && dir2 >= 0.0 && dir3 >= 0.0 && dir4 >= 0.0)) {
            if (pnear != NULL) *pnear = origin;
            return 0.0;
        }

    }
    return -1.0;
#else
    Point p1,p2,p3,p4,p5,zero(0.0,0.0,0.0);
    RCHFONT oldhfont,hfont;
    BYTE fbCharSet;
    int ih,iw,maxw,maxh,w,ll,nlines;
    double scale,oheight,rh,rw,a,l,y1,cosa,sina,dir1,dir2,dir3,dir4,xmaxw,xmaxh,xw;
#ifdef _USING_QTCHAR
    TEXTMETRICW tm;
#else
    TEXTMETRIC tm;
#endif
    Point o,xa,ya,co;
    RCCHAR *s;
    Transform t;
    View3dWindow *window;

    window = dynamic_cast<View3dWindow *>(v);
    if (window != 0 && window->getoffscreenbitmap() != 0)
        v = window->getoffscreenbitmap();

    if (pnear == NULL && v->displaylist.defined(this))
        return v->displaylist.pick(this,x,y);
    if (options.test(10)) {
        xa = xaxis;
        ya = yaxis;
        t.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
        xa = t.transform(xa);
        ya = t.transform(ya);
        o = origin;
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
#ifdef _USING_QTCHAR
        hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                            options.test(7),options.test(6),0,fbCharSet,
                            OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
        hfont = CreateFont(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                           options.test(7),options.test(6),0,fbCharSet,
                           OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,RCPRINTABLE(font));
#endif
        if (hfont != NULL)
            oldhfont = (RCHFONT)SelectObject(v->gethdc(),hfont);
#ifdef _USING_QTCHAR
        GetTextMetricsW(v->gethdc(),&tm);
#else
        GetTextMetrics(v->gethdc(),&tm);
#endif
        if (tm.tmHeight == 0) tm.tmHeight = 1;
        if (options.test(9))
            scale = v->getplotscale() / double(tm.tmHeight) * height;
        else
            scale = height / double(tm.tmHeight);
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
        else if (options.test(3))
            oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
        else
            oheight = -(tm.tmHeight - tm.tmDescent) * scale;
        xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
        xmaxw = 0;
        y1 = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
#if defined(WIN32)
            SIZE size;
#ifdef _USING_QTCHAR
            GetTextExtentPoint32W(v->gethdc(),s,ll,&size);
#else
            GetTextExtentPoint(v->gethdc(),(TCHAR*)s,ll,&size);
#endif
            xw = size.cx * scale;
#else
            long r;
            r = GetTextExtent(v->gethdc(),s,ll);
            xw = LOWORD(r) * scale;
#endif
            if (xw > xmaxw) xmaxw = xw;
            y1 -= (tm.tmHeight * (1.0 + vertspacing)) * scale;
            if (s[ll] == 0) break;
        }
        p1 = o + ya * (oheight + y1 - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
        if (options.test(1))
            p1 = p1 - xa * xmaxw / 2.0;
        else if (options.test(2))
            p1 = p1 - xa * xmaxw;
        p2 = p1 + xa * xmaxw;
        p3 = p2 + ya * xmaxh;
        p4 = p3 - xa * xmaxw;
        p1 = v->modeltoscreen(p1);
        p2 = v->modeltoscreen(p2);
        p3 = v->modeltoscreen(p3);
        p4 = v->modeltoscreen(p4);
        p5.setxyz(x,y,0.0);
        dir1 = (p2 - p1).cross(p5 - p1).z;
        dir2 = (p3 - p2).cross(p5 - p2).z;
        dir3 = (p4 - p3).cross(p5 - p3).z;
        dir4 = (p1 - p4).cross(p5 - p4).z;
        if (hfont != NULL) {
            SelectObject(v->gethdc(),oldhfont);
            DeleteObject(hfont);
        }
        if (xmaxw > 0.0 && xmaxh > 0.0 &&
                (dir1 <= 0.0 && dir2 <= 0.0 && dir3 <= 0.0 && dir4 <= 0.0 ||
                 dir1 >= 0.0 && dir2 >= 0.0 && dir3 >= 0.0 && dir4 >= 0.0)) {
            if (pnear != NULL) *pnear = origin;
            return 0.0;
        }
    } else {
        p1 = v->modeltoscreen(origin);
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
        xa = v->modeltouv(origin + xaxis) - v->modeltouv(origin);
        if ((l = xa.length()) < db.getptoler()) return -1.0;
        xa /= l;
        if (xa.x < -1.0) xa.x = -1.0;
        else if (xa.x > 1.0) xa.x = 1.0;
        a = acos(xa.x);
        if (xa.y < 0.0) a = 2.0 * M_PI - a;

        if (options.test(9))
            scale = v->getplotscale();
        else
            scale = 1.0;
        rh = height * v->getheight() / (v->getvmax() - v->getvmin()) * scale;
        if (options.test(13))  // Height is in points
            rh *= 25.4 / 72.0;
        rw = width * v->getwidth() / (v->getumax() - v->getumin()) * scale;
        if (rh > MAXTEXTHEIGHT) ih = (int)MAXTEXTHEIGHT;
        else if (rh < -MAXTEXTHEIGHT) ih = (int)-MAXTEXTHEIGHT;
        else ih = int(rh);
        if (rw > MAXTEXTHEIGHT) iw = (int)MAXTEXTHEIGHT;
        else if (rw < -MAXTEXTHEIGHT) iw = (int)-MAXTEXTHEIGHT;
        else iw = int(rw);
        if (ih == 0) return -1.0;
#ifdef _USING_QTCHAR
        hfont = CreateFontW(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + v->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + v->getrotation2d())* 10.0),
                            (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
        hfont = CreateFont(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + v->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + v->getrotation2d())* 10.0),
                           (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,RCPRINTABLE(font));
#endif
        sina = sin(a + angle / 180.0 * M_PI);
        cosa = cos(a + angle / 180.0 * M_PI);
        if (hfont != NULL)
            oldhfont = (RCHFONT) SelectObject(v->gethdc(),hfont);
#ifdef _USING_QTCHAR
        GetTextMetricsW(v->gethdc(),&tm);
#else
        GetTextMetrics(v->gethdc(),&tm);
#endif
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = -nlines * tm.tmHeight + tm.tmDescent - (nlines - 1) * tm.tmHeight * vertspacing;
        else if (options.test(3))
            oheight = -(nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2;
        else
            oheight = 0.0;
        p1.x += oheight * sina;
        p1.y -= oheight * cosa;
        p5 = p1;
        maxh = int(tm.tmHeight * (nlines + (nlines - 1)*vertspacing));
        maxw = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
#if defined(WIN32)
            SIZE size;
#ifdef _USING_QTCHAR
            GetTextExtentPoint32W(v->gethdc(),s,ll,&size);
#else
            GetTextExtentPoint(v->gethdc(),(TCHAR*)s,ll,&size);
#endif
            w = size.cx;
#else
            long r;
            r = GetTextExtent(v->gethdc(),s,ll);
            w = LOWORD(r);
#endif
            if (w > maxw) maxw = w;
            p2 = p1;
            if (options.test(1)) {
                p2.x -= w * cosa / 2.0;
                p2.y -= w * sina / 2.0;
            } else if (options.test(2)) {
                p2.x -= w * cosa;
                p2.y -= w * sina;
            }
            p1.x += tm.tmHeight * (1.0 * vertspacing) * sina;
            p1.y -= tm.tmHeight * (1.0 + vertspacing) * cosa;
            if (s[ll] == 0) break;
        }
        if (options.test(1)) {
            p5.x -= maxw * cosa / 2.0;
            p5.y -= maxw * sina / 2.0;
        } else if (options.test(2)) {
            p5.x -= maxw * cosa;
            p5.y -= maxw * sina;
        }
        p1 = p5;
        p2.setxyz(p5.x + maxw * cosa,p5.y + maxw * sina,0.0);
        p3.setxyz(p5.x + maxw * cosa + maxh * sina,p5.y + maxw * sina - maxh * cosa,0.0);
        p4.setxyz(p5.x + maxh * sina,p5.y - maxh * cosa,0.0);
        p5.setxyz(x,y,0.0);
        dir1 = (p2 - p1).cross(p5 - p1).z;
        dir2 = (p3 - p2).cross(p5 - p2).z;
        dir3 = (p4 - p3).cross(p5 - p3).z;
        dir4 = (p1 - p4).cross(p5 - p4).z;
        if (hfont != NULL) {
            SelectObject(v->gethdc(),oldhfont);
            DeleteObject(hfont);
        }
        if (maxw > 0 && maxh > 0 &&
                (dir1 <= 0.0 && dir2 <= 0.0 && dir3 <= 0.0 && dir4 <= 0.0 ||
                 dir1 >= 0.0 && dir2 >= 0.0 && dir3 >= 0.0 && dir4 >= 0.0)) {
            if (pnear != NULL) *pnear = origin;
            return 0.0;
        }
    }
    return -1.0;
#endif
}

int Text::pickoriginpoint(View3dSurface *vs,int snap,int x,int y,Point *pos)
{
    double d1;
    int end;

    end = 0;
    if ((d1 = vs->pick(origin,snap,x,y)) >= 0.0) {
        end = 3;
        *pos = origin;
    }

    return end;
}

int Text::pick(View3dSurface *vs,int insidew,int x1,int y1,int x2,int y2)
{
#ifdef USING_WIDGETS
    Point p1,p2,p3,p4,p5,xa,ya,o,zero(0.0,0.0,0.0);
    RCHFONT oldhfont,hfont;
//BYTE fbCharSet;
    int in,ih,iw,maxw,maxh,w,ll,nlines;
    double scale,oheight,rh,rw,a,l,cosa,sina,xmaxh,xmaxw,xw,tmHeight,tmDescent,tmAscent;
//TEXTMETRICW tm;
    RCCHAR *s;
    Transform t;
    View3dWindow *window;

    window = dynamic_cast<View3dWindow *>(vs);
    if (window != 0 && window->getoffscreenbitmap() != 0)
        vs = window->getoffscreenbitmap();

    if (vs->displaylist.defined(this))
        return vs->displaylist.pick(this,insidew,x1,y1,x2,y2);

#if 0
    if (options.test(10)) {
        xa = xaxis;
        ya = yaxis;
        t.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
        xa = t.transform(xa);
        ya = t.transform(ya);
        o = origin;
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
        hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                            options.test(7),options.test(6),0,fbCharSet,
                            OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);

        if (hfont != NULL)
            oldhfont = (RCHFONT) SelectObject(vs->gethdc(),hfont);
        GetTextMetricsW(vs->gethdc(),&tm);

        if (tm.tmHeight == 0) tm.tmHeight = 1;
        if (options.test(9))
            scale = vs->getplotscale() / double(tm.tmHeight) * height;
        else
            scale = height / double(tm.tmHeight);
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
        else if (options.test(3))
            oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
        else
            oheight = -(tm.tmHeight - tm.tmDescent) * scale;
        xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
        xmaxw = 0;
        y1 = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            SIZE size;
            GetTextExtentPoint32W(vs->gethdc(),s,ll,&size);

            xw = size.cx * scale;

            if (xw > xmaxw) xmaxw = xw;
            y1 -= int((tm.tmHeight * (1.0 + vertspacing)) * scale);
            if (s[ll] == 0) break;
        }
        p1 = o + ya * (oheight + y1 - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
        if (options.test(1))
            p1 = p1 - xa * xmaxw / 2.0;
        else if (options.test(2))
            p1 = p1 - xa * xmaxw;
        p2 = p1 + xa * xmaxw;
        p3 = p2 + ya * xmaxh;
        p4 = p3 - xa * xmaxw;
        p1 = vs->modeltoscreen(p1);
        p2 = vs->modeltoscreen(p2);
        p3 = vs->modeltoscreen(p3);
        p4 = vs->modeltoscreen(p4);
        in = (x1 < p1.x && p1.x < x2 && y1 < p1.y && p1.y < y2) +
             (x1 < p2.x && p2.x < x2 && y1 < p2.y && p2.y < y2) +
             (x1 < p3.x && p3.x < x2 && y1 < p3.y && p3.y < y2) +
             (x1 < p4.x && p4.x < x2 && y1 < p4.y && p4.y < y2);
        if (hfont != NULL) {
            SelectObject(vs->gethdc(),oldhfont);
            DeleteObject(hfont);
        }
        return (in == 4 && insidew || in == 0 && ! insidew);
    } else
#endif
    {
        p1 = vs->modeltoscreen(origin);
        //if (options.test(0))
        //  fbCharSet = SYMBOL_CHARSET;
        //else
        //  fbCharSet = ANSI_CHARSET;
        xa = vs->modeltouv(origin + xaxis) - vs->modeltouv(origin);
        if ((l = xa.length()) < db.getptoler()) return 0;
        xa /= l;
        if (xa.x < -1.0) xa.x = -1.0;
        else if (xa.x > 1.0) xa.x = 1.0;
        a = acos(xa.x);
        if (xa.y < 0.0) a = 2.0 * M_PI - a;
        if (options.test(9))
            scale = vs->getplotscale();
        else
            scale = 1.0;
        rh = height * vs->getheight() / (vs->getvmax() - vs->getvmin()) * scale;
        if (options.test(13))  // Height is in points
            rh *= 25.4 / 72.0;
        rw = width * vs->getwidth() / (vs->getumax() - vs->getumin()) * scale;
        if (rh > MAXTEXTHEIGHT) ih = (int)MAXTEXTHEIGHT;
        else if (rh < -MAXTEXTHEIGHT) ih = (int)-MAXTEXTHEIGHT;
        else ih = int(rh);
        if (rw > MAXTEXTHEIGHT) iw = (int)MAXTEXTHEIGHT;
        else if (rw < -MAXTEXTHEIGHT) iw = (int)-MAXTEXTHEIGHT;
        else iw = int(rw);
        if (ih == 0) {
            in = (x1 < p1.x && p1.x < x2 && y1 < p1.y && p1.y < y2);
            return (in && insidew || ! in && ! insidew);
        }

        //hfont = CreateFontW(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + vs->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + vs->getrotation2d())* 10.0),
        //                   (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
        // calculate the Qt font pixel height required for the text height setting
        // calculate the point height for the font
        QString refName(font);
        QFont refFont(refName);
        refFont.setPointSize((int)rh);// use rh as points
        QFontMetrics refFm(refFont);
        int refHeight = refFm.height();// get the height as pixels for rh points
        // how many points did we get for for 1 pixel of rh
        double ptsperpx = rh / refHeight;
        // how many points to get the same height in pixels
        int fontHeight = (int)(rh * ptsperpx);

        if(fontHeight < 1.0)
            return 0;

        // set up a device independent font
        QString qfname(font);
        QFont qfont(qfname);
        qfont.setStyleStrategy(QFont::ForceOutline);
        qfont.setPointSize(fontHeight);
        // set the font weight
        if(options.test(5))
            qfont.setWeight(QFont::Bold);
        else
            qfont.setWeight(QFont::Normal);
        // set italic
        qfont.setItalic(options.test(7));
        // set underline
        qfont.setUnderline(options.test(6));

        // get the font metrics
        QFontMetricsF fm(qfont);

        // calculate the strtch factor
        // this is done by scaling the font in width
        //double fstretch = width  / (height > 0.0 ? height : 1.0);
        //fstretch = fstretch == 0.0 ? 1.0 : fstretch;
        double fstretch = 1.0;
        if(width == 0.0)
            fstretch = 1.0;
        else
            fstretch = fabs(width);



        tmHeight = fm.height();
        tmDescent = fm.descent();
        tmAscent = fm.ascent();

        sina = sin(a + angle / 180.0 * M_PI);
        cosa = cos(a + angle / 180.0 * M_PI);

        //if (hfont != NULL)
        //  oldhfont = (RCHFONT) SelectObject(vs->gethdc(),hfont);
        //GetTextMetricsW(vs->gethdc(),&tm);

        //for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        //for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += (*s == 0x000D || *s == 0x000A);
        // calculate the number of lines
        // text on windows have CR+LF line endings MAC OS X has LF line endings
        // find the line feed or carriage return or line feed and carriage return
        // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
        for (s = text,nlines = 1 ; *s!=0 ; s++) {
            nlines += (*s == 0x000D || *s == 0x000A) ;
            if(*s == 0x000D && (*s+1 != 0) && *(s+1) == 0x000A)
                s++;
        }

        if (! options.test(3) && ! options.test(4))
            oheight = -nlines * tmHeight + tmDescent - (nlines - 1) * tmHeight * vertspacing;
        else if (options.test(3))
            oheight = -(nlines + (nlines - 1) * vertspacing) * tmHeight / 2;
        else
            oheight = 0.0;
        p1.x += oheight * sina;
        p1.y -= oheight * cosa;
        p5 = p1;

        maxh = int(tmHeight * (nlines + (nlines - 1)*vertspacing));
        maxw = 0;

        for (s = text ; *s!=0 ;  s += ll + 2) {
            //for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            //for (ll = 0 ;  (s[ll] != 0x000D) && (s[ll] != 0x000A) && (s[ll] != 0) ; ll++) ;
            // text on windows have CR+LF line endings MAC OS X has LF line endings
            // find the line feed or carriage return or line feed and carriage return
            // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
            //for (ll = 0 ;  (s[ll] != 0x000D) && (s[ll] != 0x000A) && (s[ll] != 0) ; ll++) ;
            for (ll = 0 ; s[ll] != 0; ll++) {
                if(s[ll] == 0x000D || s[ll] == 0x000A) {
                    if(s[ll] == 0x000D && s[ll+1] != 0 && s[ll+1] == 0x000A)
                        ll++;
                    break;
                }
            }
            //SIZE size;
            // GetTextExtentPoint32W(vs->gethdc(),s,ll,&size);
            // w = size.cx;
            QString qs1(s);
            w = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,qs1).width();

            if (w > maxw) maxw = w;
            p2 = p1;
            if (options.test(1)) {
                p2.x -= w * fstretch * cosa / 2.0;
                p2.y -= w * fstretch * sina / 2.0;
            } else if (options.test(2)) {
                p2.x -= w * fstretch * cosa;
                p2.y -= w * fstretch * sina;
            }
            p1.x += tmHeight * (1.0 + vertspacing) * sina;
            p1.y -= tmHeight * (1.0 + vertspacing) * cosa;
            if (s[ll] == 0) break;
        }
        if (options.test(1)) {
            p5.x -= maxw * fstretch * cosa / 2.0;
            p5.y -= maxw * fstretch * sina / 2.0;
        } else if (options.test(2)) {
            p5.x -= maxw * fstretch * cosa;
            p5.y -= maxw * fstretch * sina;
        }
        p1 = p5;
        p2.setxyz(p5.x + maxw * fstretch * cosa,p5.y + maxw * fstretch * sina,0.0);
        p3.setxyz(p5.x + maxw * fstretch * cosa + maxh * sina,p5.y + maxw * fstretch * sina - maxh * cosa,0.0);
        p4.setxyz(p5.x + maxh * sina,p5.y - maxh * cosa,0.0);
        in = (x1 < p1.x && p1.x < x2 && y1 < p1.y && p1.y < y2) +
             (x1 < p2.x && p2.x < x2 && y1 < p2.y && p2.y < y2) +
             (x1 < p3.x && p3.x < x2 && y1 < p3.y && p3.y < y2) +
             (x1 < p4.x && p4.x < x2 && y1 < p4.y && p4.y < y2);
        //if (hfont != NULL)
        //  {  SelectObject(vs->gethdc(),oldhfont);
        //     DeleteObject(hfont);
        //  }
        return (in == 4 && insidew || in == 0 && ! insidew);
    }
#else
    Point p1,p2,p3,p4,p5,xa,ya,o,zero(0.0,0.0,0.0);
    RCHFONT oldhfont,hfont;
    BYTE fbCharSet;
    int in,ih,iw,maxw,maxh,w,ll,nlines;
    double scale,oheight,rh,rw,a,l,cosa,sina,xmaxh,xmaxw,xw;
#ifdef _USING_QTCHAR
    TEXTMETRICW tm;
#else
    TEXTMETRIC tm;
#endif
    RCCHAR *s;
    Transform t;
    View3dWindow *window;

    window = dynamic_cast<View3dWindow *>(vs);
    if (window != 0 && window->getoffscreenbitmap() != 0)
        vs = window->getoffscreenbitmap();

    if (vs->displaylist.defined(this))
        return vs->displaylist.pick(this,insidew,x1,y1,x2,y2);
    if (options.test(10)) {
        xa = xaxis;
        ya = yaxis;
        t.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
        xa = t.transform(xa);
        ya = t.transform(ya);
        o = origin;
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
#ifdef _USING_QTCHAR
        hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                            options.test(7),options.test(6),0,fbCharSet,
                            OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
        hfont = CreateFont(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                           options.test(7),options.test(6),0,fbCharSet,
                           OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,RCPRINTABLE(font));
#endif
        if (hfont != NULL)
            oldhfont = (RCHFONT) SelectObject(vs->gethdc(),hfont);
#ifdef _USING_QTCHAR
        GetTextMetricsW(vs->gethdc(),&tm);
#else
        GetTextMetrics(vs->gethdc(),&tm);
#endif
        if (tm.tmHeight == 0) tm.tmHeight = 1;
        if (options.test(9))
            scale = vs->getplotscale() / double(tm.tmHeight) * height;
        else
            scale = height / double(tm.tmHeight);
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
        else if (options.test(3))
            oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
        else
            oheight = -(tm.tmHeight - tm.tmDescent) * scale;
        xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
        xmaxw = 0;
        y1 = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
#if defined(WIN32)
            SIZE size;
#ifdef _USING_QTCHAR
            GetTextExtentPoint32W(vs->gethdc(),s,ll,&size);
#else
            GetTextExtentPoint(vs->gethdc(),(TCHAR*)s,ll,&size);
#endif
            xw = size.cx * scale;
#else
            long r;
            r = GetTextExtent(vs->gethdc(),s,ll);
            xw = LOWORD(r) * scale;
#endif
            if (xw > xmaxw) xmaxw = xw;
            y1 -= int((tm.tmHeight * (1.0 + vertspacing)) * scale);
            if (s[ll] == 0) break;
        }
        p1 = o + ya * (oheight + y1 - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
        if (options.test(1))
            p1 = p1 - xa * xmaxw / 2.0;
        else if (options.test(2))
            p1 = p1 - xa * xmaxw;
        p2 = p1 + xa * xmaxw;
        p3 = p2 + ya * xmaxh;
        p4 = p3 - xa * xmaxw;
        p1 = vs->modeltoscreen(p1);
        p2 = vs->modeltoscreen(p2);
        p3 = vs->modeltoscreen(p3);
        p4 = vs->modeltoscreen(p4);
        in = (x1 < p1.x && p1.x < x2 && y1 < p1.y && p1.y < y2) +
             (x1 < p2.x && p2.x < x2 && y1 < p2.y && p2.y < y2) +
             (x1 < p3.x && p3.x < x2 && y1 < p3.y && p3.y < y2) +
             (x1 < p4.x && p4.x < x2 && y1 < p4.y && p4.y < y2);
        if (hfont != NULL) {
            SelectObject(vs->gethdc(),oldhfont);
            DeleteObject(hfont);
        }
        return (in == 4 && insidew || in == 0 && ! insidew);
    } else {
        p1 = vs->modeltoscreen(origin);
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
        xa = vs->modeltouv(origin + xaxis) - vs->modeltouv(origin);
        if ((l = xa.length()) < db.getptoler()) return 0;
        xa /= l;
        if (xa.x < -1.0) xa.x = -1.0;
        else if (xa.x > 1.0) xa.x = 1.0;
        a = acos(xa.x);
        if (xa.y < 0.0) a = 2.0 * M_PI - a;
        if (options.test(9))
            scale = vs->getplotscale();
        else
            scale = 1.0;
        rh = height * vs->getheight() / (vs->getvmax() - vs->getvmin()) * scale;
        if (options.test(13))  // Height is in points
            rh *= 25.4 / 72.0;
        rw = width * vs->getwidth() / (vs->getumax() - vs->getumin()) * scale;
        if (rh > MAXTEXTHEIGHT) ih = (int)MAXTEXTHEIGHT;
        else if (rh < -MAXTEXTHEIGHT) ih = (int)-MAXTEXTHEIGHT;
        else ih = int(rh);
        if (rw > MAXTEXTHEIGHT) iw = (int)MAXTEXTHEIGHT;
        else if (rw < -MAXTEXTHEIGHT) iw = (int)-MAXTEXTHEIGHT;
        else iw = int(rw);
        if (ih == 0) {
            in = (x1 < p1.x && p1.x < x2 && y1 < p1.y && p1.y < y2);
            return (in && insidew || ! in && ! insidew);
        }
#ifdef _USING_QTCHAR
        hfont = CreateFontW(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + vs->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + vs->getrotation2d())* 10.0),
                            (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
        hfont = CreateFont(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + vs->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + vs->getrotation2d())* 10.0),
                           (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,RCPRINTABLE(font));
#endif
        sina = sin(a + angle / 180.0 * M_PI);
        cosa = cos(a + angle / 180.0 * M_PI);
        if (hfont != NULL)
            oldhfont = (RCHFONT) SelectObject(vs->gethdc(),hfont);
#ifdef _USING_QTCHAR
        GetTextMetricsW(vs->gethdc(),&tm);
#else
        GetTextMetrics(vs->gethdc(),&tm);
#endif
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = -nlines * tm.tmHeight + tm.tmDescent - (nlines - 1) * tm.tmHeight * vertspacing;
        else if (options.test(3))
            oheight = -(nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2;
        else
            oheight = 0.0;
        p1.x += oheight * sina;
        p1.y -= oheight * cosa;
        p5 = p1;
        maxh = int(tm.tmHeight * (nlines + (nlines - 1)*vertspacing));
        maxw = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
#if defined(WIN32)
            SIZE size;
#ifdef _USING_QTCHAR
            GetTextExtentPoint32W(vs->gethdc(),s,ll,&size);
#else
            GetTextExtentPoint(vs->gethdc(),(TCHAR*)s,ll,&size);
#endif
            w = size.cx;
#else
            long r;
            r = GetTextExtent(vs->gethdc(),s,ll);
            w = LOWORD(r);
#endif
            if (w > maxw) maxw = w;
            p2 = p1;
            if (options.test(1)) {
                p2.x -= w * cosa / 2.0;
                p2.y -= w * sina / 2.0;
            } else if (options.test(2)) {
                p2.x -= w * cosa;
                p2.y -= w * sina;
            }
            p1.x += tm.tmHeight * (1.0 + vertspacing) * sina;
            p1.y -= tm.tmHeight * (1.0 + vertspacing) * cosa;
            if (s[ll] == 0) break;
        }
        if (options.test(1)) {
            p5.x -= maxw * cosa / 2.0;
            p5.y -= maxw * sina / 2.0;
        } else if (options.test(2)) {
            p5.x -= maxw * cosa;
            p5.y -= maxw * sina;
        }
        p1 = p5;
        p2.setxyz(p5.x + maxw * cosa,p5.y + maxw * sina,0.0);
        p3.setxyz(p5.x + maxw * cosa + maxh * sina,p5.y + maxw * sina - maxh * cosa,0.0);
        p4.setxyz(p5.x + maxh * sina,p5.y - maxh * cosa,0.0);
        in = (x1 < p1.x && p1.x < x2 && y1 < p1.y && p1.y < y2) +
             (x1 < p2.x && p2.x < x2 && y1 < p2.y && p2.y < y2) +
             (x1 < p3.x && p3.x < x2 && y1 < p3.y && p3.y < y2) +
             (x1 < p4.x && p4.x < x2 && y1 < p4.y && p4.y < y2);
        if (hfont != NULL) {
            SelectObject(vs->gethdc(),oldhfont);
            DeleteObject(hfont);
        }
        return (in == 4 && insidew || in == 0 && ! insidew);
    }
#endif
}

int Text::pick(View3dSurface *vs,int nvertices,Point *pwvertices,int inside)
{
#ifdef USING_WIDGETS
    Point p1,p2,p3,p4,p5,zero(0.0,0.0,0.0);
    RCHFONT oldhfont,hfont;
    //BYTE fbCharSet;
    int ih,iw,maxw,maxh,w,ll,nlines,picked;
    double scale,oheight,rh,rw,a,l,y1,cosa,sina,xmaxw,xmaxh,xw,tmHeight,tmDescent,tmAscent;
    //TEXTMETRICW tm;
    Point o,xa,ya,co;
    RCCHAR *s;
    Transform t;
    View3dWindow *window;

    window = dynamic_cast<View3dWindow *>(vs);
    if (window != 0 && window->getoffscreenbitmap() != 0)
        vs = window->getoffscreenbitmap();


    picked = inside != 2;
#if 0
    if (options.test(10)) {
        xa = xaxis;
        ya = yaxis;
        t.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
        xa = t.transform(xa);
        ya = t.transform(ya);
        o = origin;
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
        hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                            options.test(7),options.test(6),0,fbCharSet,
                            OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);

        if (hfont != NULL)
            oldhfont = (RCHFONT)SelectObject(vs->gethdc(),hfont);
        GetTextMetricsW(vs->gethdc(),&tm);

        if (tm.tmHeight == 0) tm.tmHeight = 1;
        if (options.test(9))
            scale = vs->getplotscale() / double(tm.tmHeight) * height;
        else
            scale = height / double(tm.tmHeight);
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
        else if (options.test(3))
            oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
        else
            oheight = -(tm.tmHeight - tm.tmDescent) * scale;
        xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
        xmaxw = 0;
        y1 = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            SIZE size;
            GetTextExtentPoint32W(vs->gethdc(),s,ll,&size);

            xw = size.cx * scale;

            if (xw > xmaxw) xmaxw = xw;
            y1 -= (tm.tmHeight * (1.0 + vertspacing)) * scale;
            if (s[ll] == 0) break;
        }
        p1 = o + ya * (oheight + y1 - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
        if (options.test(1))
            p1 = p1 - xa * xmaxw / 2.0;
        else if (options.test(2))
            p1 = p1 - xa * xmaxw;
        p2 = p1 + xa * xmaxw;
        p3 = p2 + ya * xmaxh;
        p4 = p3 - xa * xmaxw;

        if (hfont != NULL) {
            SelectObject(vs->gethdc(),oldhfont);
            DeleteObject(hfont);
        }

        if (vs->pick(nvertices,pwvertices,p1,p2,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p2,p3,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p3,p4,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p4,p1,inside,&picked))
            return picked;

    } else
#endif
    {
        p1 = vs->modeltoscreen(origin);
        //if (options.test(0))
        //  fbCharSet = SYMBOL_CHARSET;
        //else
        //  fbCharSet = ANSI_CHARSET;
        xa = vs->modeltouv(origin + xaxis) - vs->modeltouv(origin);
        if ((l = xa.length()) < db.getptoler()) return 0;
        xa /= l;
        if (xa.x < -1.0) xa.x = -1.0;
        else if (xa.x > 1.0) xa.x = 1.0;
        a = acos(xa.x);
        if (xa.y < 0.0) a = 2.0 * M_PI - a;

        if (options.test(9))
            scale = vs->getplotscale();
        else
            scale = 1.0;

        rh = height * vs->getheight() / (vs->getvmax() - vs->getvmin()) * scale;
        if (options.test(13))  // Height is in points
            rh *= 25.4 / 72.0;
        rw = width * vs->getwidth() / (vs->getumax() - vs->getumin()) * scale;
        if (rh > MAXTEXTHEIGHT) ih = (int)MAXTEXTHEIGHT;
        else if (rh < -MAXTEXTHEIGHT) ih = (int)-MAXTEXTHEIGHT;
        else ih = int(rh);
        if (rw > MAXTEXTHEIGHT) iw = (int)MAXTEXTHEIGHT;
        else if (rw < -MAXTEXTHEIGHT) iw = (int)-MAXTEXTHEIGHT;
        else iw = int(rw);
        if (ih == 0) return 0;

        //hfont = CreateFontW(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + vs->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + vs->getrotation2d())* 10.0),
        //                   (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);

        // calculate the Qt font pixel height required for the text height setting
        // calculate the point height for the font
        QString refName(font);
        QFont refFont(refName);
        refFont.setPointSize((int)rh);// use rh as points
        QFontMetrics refFm(refFont);
        int refHeight = refFm.height();// get the height as pixels for rh points
        // how many points did we get for for 1 pixel of rh
        double ptsperpx = rh / refHeight;
        // how many points to get the same height in pixels
        int fontHeight = (int)(rh * ptsperpx);

        if(fontHeight < 1.0)
            return 0;

        // set up a device independent font
        QString qfname(font);
        QFont qfont(qfname);
        qfont.setStyleStrategy(QFont::ForceOutline);
        qfont.setPointSize(fontHeight);
        // set the font weight
        if(options.test(5))
            qfont.setWeight(QFont::Bold);
        else
            qfont.setWeight(QFont::Normal);
        // set italic
        qfont.setItalic(options.test(7));
        // set underline
        qfont.setUnderline(options.test(6));

        // get the font metrics
        QFontMetricsF fm(qfont);

        // calculate the strtch factor
        // this is done by scaling the font in width
        //double fstretch = width  / (height > 0.0 ? height : 1.0);
        //fstretch = fstretch == 0.0 ? 1.0 : fstretch;
        double fstretch = 1.0;
        if(width == 0.0)
            fstretch = 1.0;
        else
            fstretch = fabs(width);

        tmHeight = fm.height();
        tmDescent = fm.descent();
        tmAscent = fm.ascent();

        sina = sin(a + angle / 180.0 * M_PI);
        cosa = cos(a + angle / 180.0 * M_PI);

        //if (hfont != NULL)
        //  oldhfont = (RCHFONT) SelectObject(vs->gethdc(),hfont);
        //GetTextMetricsW(vs->gethdc(),&tm);

        //for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        //for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += (*s == 0x000D || *s == 0x000A);
        // calculate the number of lines
        // text on windows have CR+LF line endings MAC OS X has LF line endings
        // find the line feed or carriage return or line feed and carriage return
        // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
        for (s = text,nlines = 1 ; *s!=0 ; s++) {
            nlines += (*s == 0x000D || *s == 0x000A) ;
            if(*s == 0x000D && (*s+1 != 0) && *(s+1) == 0x000A)
                s++;
        }

        if (! options.test(3) && ! options.test(4))
            oheight = -nlines * tmHeight + tmDescent - (nlines - 1) * tmHeight * vertspacing;
        else if (options.test(3))
            oheight = -(nlines + (nlines - 1) * vertspacing) * tmHeight / 2;
        else
            oheight = 0.0;
        p1.x += oheight * sina;
        p1.y -= oheight * cosa;
        p5 = p1;

        maxh = int(tmHeight * (nlines + (nlines - 1)*vertspacing));
        maxw = 0;

        for (s = text ; *s!=0 ;  s += ll + 1) {
            //for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            //for (ll = 0 ;  (s[ll] != 0x000D) && (s[ll] != 0x000A) && (s[ll] != 0) ; ll++) ;
            // text on windows have CR+LF line endings MAC OS X has LF line endings
            // find the line feed or carriage return or line feed and carriage return
            // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
            //for (ll = 0 ;  (s[ll] != 0x000D) && (s[ll] != 0x000A) && (s[ll] != 0) ; ll++) ;
            for (ll = 0 ; s[ll] != 0; ll++) {
                if(s[ll] == 0x000D || s[ll] == 0x000A) {
                    if(s[ll] == 0x000D && s[ll+1] != 0 && s[ll+1] == 0x000A)
                        ll++;
                    break;
                }
            }
            //SIZE size;
            //GetTextExtentPoint32W(vs->gethdc(),s,ll,&size);
            //w = size.cx;
            QString qs1(s);
            w = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,qs1).width();

            if (w > maxw) maxw = w;
            p2 = p1;
            if (options.test(1)) {
                p2.x -= w * fstretch * cosa / 2.0;
                p2.y -= w * fstretch * sina / 2.0;
            } else if (options.test(2)) {
                p2.x -= w * fstretch * cosa;
                p2.y -= w * fstretch * sina;
            }
            p1.x += tmHeight * (1.0 * vertspacing) * sina;
            p1.y -= tmHeight * (1.0 + vertspacing) * cosa;
            if (s[ll] == 0) break;
        }
        if (options.test(1)) {
            p5.x -= maxw * fstretch * cosa / 2.0;
            p5.y -= maxw * fstretch * sina / 2.0;
        } else if (options.test(2)) {
            p5.x -= maxw * fstretch * cosa;
            p5.y -= maxw * fstretch * sina;
        }
        p1 = p5;
        p2.setxyz(p5.x + maxw * fstretch * cosa,p5.y + maxw * fstretch * sina,0.0);
        p3.setxyz(p5.x + maxw * fstretch * cosa + maxh * sina,p5.y + maxw * fstretch * sina - maxh * cosa,0.0);
        p4.setxyz(p5.x + maxh * sina,p5.y - maxh * cosa,0.0);

        //if (hfont != NULL)
        //  {  SelectObject(vs->gethdc(),oldhfont);
        //     DeleteObject(hfont);
        //  }

        p1 = vs->uvtomodel(vs->screentouv(p1));
        p2 = vs->uvtomodel(vs->screentouv(p2));
        p3 = vs->uvtomodel(vs->screentouv(p3));
        p4 = vs->uvtomodel(vs->screentouv(p4));

        if (vs->pick(nvertices,pwvertices,p1,p2,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p2,p3,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p3,p4,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p4,p1,inside,&picked))
            return picked;

    }

    return picked;
#else
    Point p1,p2,p3,p4,p5,zero(0.0,0.0,0.0);
    RCHFONT oldhfont,hfont;
    BYTE fbCharSet;
    int ih,iw,maxw,maxh,w,ll,nlines,picked;
    double scale,oheight,rh,rw,a,l,y1,cosa,sina,xmaxw,xmaxh,xw;
#ifdef _USING_QTCHAR
    TEXTMETRICW tm;
#else
    TEXTMETRIC tm;
#endif
    Point o,xa,ya,co;
    RCCHAR *s;
    Transform t;
    View3dWindow *window;

    window = dynamic_cast<View3dWindow *>(vs);
    if (window != 0 && window->getoffscreenbitmap() != 0)
        vs = window->getoffscreenbitmap();


    picked = inside != 2;
    if (options.test(10)) {
        xa = xaxis;
        ya = yaxis;
        t.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
        xa = t.transform(xa);
        ya = t.transform(ya);
        o = origin;
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
#ifdef _USING_QTCHAR
        hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                            options.test(7),options.test(6),0,fbCharSet,
                            OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
        hfont = CreateFont(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                           options.test(7),options.test(6),0,fbCharSet,
                           OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,RCPRINTABLE(font));
#endif
        if (hfont != NULL)
            oldhfont = (RCHFONT)SelectObject(vs->gethdc(),hfont);
#ifdef _USING_QTCHAR
        GetTextMetricsW(vs->gethdc(),&tm);
#else
        GetTextMetrics(vs->gethdc(),&tm);
#endif
        if (tm.tmHeight == 0) tm.tmHeight = 1;
        if (options.test(9))
            scale = vs->getplotscale() / double(tm.tmHeight) * height;
        else
            scale = height / double(tm.tmHeight);
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
        else if (options.test(3))
            oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
        else
            oheight = -(tm.tmHeight - tm.tmDescent) * scale;
        xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
        xmaxw = 0;
        y1 = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
#if defined(WIN32)
            SIZE size;
#ifdef _USING_QTCHAR
            GetTextExtentPoint32W(vs->gethdc(),s,ll,&size);
#else
            GetTextExtentPoint(vs->gethdc(),(TCHAR*)s,ll,&size);
#endif
            xw = size.cx * scale;
#else
            long r;
            r = GetTextExtent(vs->gethdc(),s,ll);
            xw = LOWORD(r) * scale;
#endif
            if (xw > xmaxw) xmaxw = xw;
            y1 -= (tm.tmHeight * (1.0 + vertspacing)) * scale;
            if (s[ll] == 0) break;
        }
        p1 = o + ya * (oheight + y1 - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
        if (options.test(1))
            p1 = p1 - xa * xmaxw / 2.0;
        else if (options.test(2))
            p1 = p1 - xa * xmaxw;
        p2 = p1 + xa * xmaxw;
        p3 = p2 + ya * xmaxh;
        p4 = p3 - xa * xmaxw;

        if (hfont != NULL) {
            SelectObject(vs->gethdc(),oldhfont);
            DeleteObject(hfont);
        }

        if (vs->pick(nvertices,pwvertices,p1,p2,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p2,p3,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p3,p4,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p4,p1,inside,&picked))
            return picked;

    } else {
        p1 = vs->modeltoscreen(origin);
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
        xa = vs->modeltouv(origin + xaxis) - vs->modeltouv(origin);
        if ((l = xa.length()) < db.getptoler()) return 0;
        xa /= l;
        if (xa.x < -1.0) xa.x = -1.0;
        else if (xa.x > 1.0) xa.x = 1.0;
        a = acos(xa.x);
        if (xa.y < 0.0) a = 2.0 * M_PI - a;

        if (options.test(9))
            scale = vs->getplotscale();
        else
            scale = 1.0;
        rh = height * vs->getheight() / (vs->getvmax() - vs->getvmin()) * scale;
        if (options.test(13))  // Height is in points
            rh *= 25.4 / 72.0;
        rw = width * vs->getwidth() / (vs->getumax() - vs->getumin()) * scale;
        if (rh > MAXTEXTHEIGHT) ih = (int)MAXTEXTHEIGHT;
        else if (rh < -MAXTEXTHEIGHT) ih = (int)-MAXTEXTHEIGHT;
        else ih = int(rh);
        if (rw > MAXTEXTHEIGHT) iw = (int)MAXTEXTHEIGHT;
        else if (rw < -MAXTEXTHEIGHT) iw = (int)-MAXTEXTHEIGHT;
        else iw = int(rw);
        if (ih == 0) return 0;
#ifdef _USING_QTCHAR
        hfont = CreateFontW(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + vs->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + vs->getrotation2d())* 10.0),
                            (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
        hfont = CreateFont(options.test(13) ? -ih : ih,iw,int((a * 180.0 / M_PI + angle + vs->getrotation2d()) * 10.0),int((a * 180.0 / M_PI + angle + vs->getrotation2d())* 10.0),
                           (options.test(5) ? FW_BOLD :FW_NORMAL),options.test(7),options.test(6),0,fbCharSet,OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,RCPRINTABLE(font));
#endif
        sina = sin(a + angle / 180.0 * M_PI);
        cosa = cos(a + angle / 180.0 * M_PI);
        if (hfont != NULL)
            oldhfont = (RCHFONT) SelectObject(vs->gethdc(),hfont);
#ifdef _USING_QTCHAR
        GetTextMetricsW(vs->gethdc(),&tm);
#else
        GetTextMetrics(vs->gethdc(),&tm);
#endif
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = -nlines * tm.tmHeight + tm.tmDescent - (nlines - 1) * tm.tmHeight * vertspacing;
        else if (options.test(3))
            oheight = -(nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2;
        else
            oheight = 0.0;
        p1.x += oheight * sina;
        p1.y -= oheight * cosa;
        p5 = p1;
        maxh = int(tm.tmHeight * (nlines + (nlines - 1)*vertspacing));
        maxw = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
#if defined(WIN32)
            SIZE size;
#ifdef _USING_QTCHAR
            GetTextExtentPoint32W(vs->gethdc(),s,ll,&size);
#else
            GetTextExtentPoint(vs->gethdc(),(TCHAR*)s,ll,&size);
#endif
            w = size.cx;
#else
            long r;
            r = GetTextExtent(vs->gethdc(),s,ll);
            w = LOWORD(r);
#endif
            if (w > maxw) maxw = w;
            p2 = p1;
            if (options.test(1)) {
                p2.x -= w * cosa / 2.0;
                p2.y -= w * sina / 2.0;
            } else if (options.test(2)) {
                p2.x -= w * cosa;
                p2.y -= w * sina;
            }
            p1.x += tm.tmHeight * (1.0 * vertspacing) * sina;
            p1.y -= tm.tmHeight * (1.0 + vertspacing) * cosa;
            if (s[ll] == 0) break;
        }
        if (options.test(1)) {
            p5.x -= maxw * cosa / 2.0;
            p5.y -= maxw * sina / 2.0;
        } else if (options.test(2)) {
            p5.x -= maxw * cosa;
            p5.y -= maxw * sina;
        }
        p1 = p5;
        p2.setxyz(p5.x + maxw * cosa,p5.y + maxw * sina,0.0);
        p3.setxyz(p5.x + maxw * cosa + maxh * sina,p5.y + maxw * sina - maxh * cosa,0.0);
        p4.setxyz(p5.x + maxh * sina,p5.y - maxh * cosa,0.0);

        if (hfont != NULL) {
            SelectObject(vs->gethdc(),oldhfont);
            DeleteObject(hfont);
        }

        p1 = vs->uvtomodel(vs->screentouv(p1));
        p2 = vs->uvtomodel(vs->screentouv(p2));
        p3 = vs->uvtomodel(vs->screentouv(p3));
        p4 = vs->uvtomodel(vs->screentouv(p4));

        if (vs->pick(nvertices,pwvertices,p1,p2,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p2,p3,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p3,p4,inside,&picked))
            return picked;
        if (vs->pick(nvertices,pwvertices,p4,p1,inside,&picked))
            return picked;

    }

    return picked;
#endif
}


#define GGO_BUF_SIZE 4096

void Text::addtodisplaylist(View3dSurface *v,int child)
{
    Point zero(0.0,0.0,0.0),o,xa,ya,p1,p2,p3,p4,pt1,pt2;
    Transform tr;
    int fbCharSet,ll,i,j,k,i1,i2,nlines,ih,iw,maxh,maxw,w,hjust,vjust;
    double scale,y,l,a,rh,rw,sina,cosa,oheight,xmaxh,xmaxw,xw;
    char buffer[GGO_BUF_SIZE];
    RCCHAR *s;
    RCHFONT hfont,oldhfont;
#ifdef _USING_QTCHAR
    TEXTMETRICW tm;
#else
    TEXTMETRIC tm;
#endif
    DWORD status;
#ifdef _USING_QTCHAR
    LOGFONTW lf;
#else
    LOGFONT lf;
#endif
    long size;

    if (options.test(10)) {
#if ! defined(_WIN32_WCE)
        RCMAT2 mat2;
        RCGLYPHMETRICS gm;
        TTPOLYGONHEADER *ttph;
        TTPOLYCURVE *ttpc;

        for (k = child,size = 0 ; k < 2 ; k++) {
            if (! child) {
                if (k == 0)
                    v->displaylist.setcountmode();
                else
                    v->displaylist.setsavemode();
                if (! v->displaylist.start(this,size)) return;
            }
            xa = xaxis;
            ya = yaxis;
            tr.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
            xa = tr.transform(xa);
            ya = tr.transform(ya);
            o = origin;
            mat2.eM11.value = 1;
            mat2.eM11.fract = 0;
            mat2.eM12.value = 0;
            mat2.eM12.fract = 0;
            mat2.eM21.value = 0;
            mat2.eM21.fract = 0;
            mat2.eM22.value = 1;
            mat2.eM22.fract = 0;
            if (options.test(0))
                fbCharSet = SYMBOL_CHARSET;
            else
                fbCharSet = ANSI_CHARSET;
#ifdef _USING_QTCHAR
            hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                                options.test(7),options.test(6),0,fbCharSet,
                                OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
            hfont = CreateFont(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                               options.test(7),options.test(6),0,fbCharSet,
                               OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,RCPRINTABLE(font));
#endif
            if (hfont != NULL)
                oldhfont = (RCHFONT) SelectObject(v->gethdc(),hfont);
#ifdef _USING_QTCHAR
            GetTextMetricsW(v->gethdc(),&tm);
#else
            GetTextMetrics(v->gethdc(),&tm);
#endif
            if (tm.tmHeight == 0) tm.tmHeight = 1;
            if (options.test(9))
                scale = v->getplotscale() / double(tm.tmHeight) * height;
            else
                scale = height / double(tm.tmHeight);
            for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
            if (! options.test(3) && ! options.test(4))
                oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
            else if (options.test(3))
                oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
            else
                oheight = -(tm.tmHeight - tm.tmDescent) * scale;
            xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
            xmaxw = 0;
            y = 0;
            for (s = text ; *s!=0 ;  s += ll + 2) {
                for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
#if defined(WIN32)
                SIZE size;
#ifdef _USING_QTCHAR
                GetTextExtentPoint32W(v->gethdc(),s,ll,&size);
#else
                GetTextExtentPoint(v->gethdc(),(TCHAR*)s,ll,&size);
#endif
                xw = size.cx * scale;
#else
                long r;
                r = GetTextExtent(v->gethdc(),s,ll);
                xw = LOWORD(r) * scale;
#endif
                if (xw > xmaxw) xmaxw = xw;
                if (options.test(1))
                    p1.setxyz(-xw / 2.0,oheight + y,0.0);
                else if (options.test(2))
                    p1.setxyz(-xw,oheight + y,0.0);
                else
                    p1.setxyz(0.0,oheight + y,0.0);
                for (i = 0 ; i < ll ; i++) {
#ifdef _USING_QTCHAR
                    status = GetGlyphOutlineW(v->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2);
#else
                    status = GetGlyphOutline(v->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2);
#endif
                    i1 = 0;
                    while (i1 < (int)status) {
                        ttph = (TTPOLYGONHEADER *) (buffer + i1);
                        i2 = int(i1 + ttph->cb);
                        i1 += sizeof(TTPOLYGONHEADER);
                        p2 = p4 = o + xa * (p1.x + (ttph->pfxStart.x.value + ttph->pfxStart.x.fract / 65535.0) * scale) +
                                  ya * (p1.y + (ttph->pfxStart.y.value + ttph->pfxStart.y.fract / 65535.0) * scale);
                        while (i1 < i2) {
                            ttpc = (TTPOLYCURVE *) (buffer + i1);
                            for (j = 0 ; j < ttpc->cpfx ; j++) {
                                p3 = o + xa * (p1.x + (ttpc->apfx[j].x.value + ttpc->apfx[j].x.fract / 65535.0) * scale) +
                                     ya * (p1.y + (ttpc->apfx[j].y.value + ttpc->apfx[j].y.fract / 65535.0) * scale);
                                pt1 = v->modeltoview(p2);
                                pt2 = v->modeltoview(p3);
                                if (v->clip3dfb(&pt1,&pt2)) {
                                    pt1 = v->viewtoscreen(pt1);
                                    pt2 = v->viewtoscreen(pt2);
                                    if (! v->displaylist.moveto(pt1.x,pt1.y)) return;
                                    if (! v->displaylist.lineto(pt2.x,pt2.y)) return;
                                }
                                p2 = p3;
                            }
                            i1 += sizeof(TTPOLYCURVE) + (ttpc->cpfx - 1) * sizeof(POINTFX);
                        }
                        pt1 = v->modeltoview(p2);
                        pt2 = v->modeltoview(p4);
                        if (v->clip3dfb(&pt1,&pt2)) {
                            pt1 = v->viewtoscreen(pt1);
                            pt2 = v->viewtoscreen(pt2);
                            if (! v->displaylist.moveto(pt1.x,pt1.y)) return;
                            if (! v->displaylist.lineto(pt2.x,pt2.y)) return;
                        }
                    }
                    p1.x += gm.gmCellIncX * scale;
                    p1.y += gm.gmCellIncY * scale;
                }
                y -= (tm.tmHeight * (1.0 + vertspacing)) * scale;
                if (s[ll] == 0) break;
            }
            if (options.test(8)) {
                p1 = o + ya * (oheight + y - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
                if (options.test(1))
                    p1 = p1 - xa * xmaxw / 2.0;
                else if (options.test(2))
                    p1 = p1 - xa * xmaxw;
                p2 = p1 + xa * xmaxw;
                p3 = p2 + ya * xmaxh;
                p4 = p3 - xa * xmaxw;
                pt1 = v->modeltoview(p1);
                pt2 = v->modeltoview(p2);
                if (v->clip3dfb(&pt1,&pt2)) {
                    pt1 = v->viewtoscreen(pt1);
                    pt2 = v->viewtoscreen(pt2);
                    if (! v->displaylist.moveto(pt1.x,pt1.y)) return;
                    if (! v->displaylist.lineto(pt2.x,pt2.y)) return;
                }
                pt1 = v->modeltoview(p2);
                pt2 = v->modeltoview(p3);
                if (v->clip3dfb(&pt1,&pt2)) {
                    pt1 = v->viewtoscreen(pt1);
                    pt2 = v->viewtoscreen(pt2);
                    if (! v->displaylist.moveto(pt1.x,pt1.y)) return;
                    if (! v->displaylist.lineto(pt2.x,pt2.y)) return;
                }
                pt1 = v->modeltoview(p3);
                pt2 = v->modeltoview(p4);
                if (v->clip3dfb(&pt1,&pt2)) {
                    pt1 = v->viewtoscreen(pt1);
                    pt2 = v->viewtoscreen(pt2);
                    if (! v->displaylist.moveto(pt1.x,pt1.y)) return;
                    if (! v->displaylist.lineto(pt2.x,pt2.y)) return;
                }
                pt1 = v->modeltoview(p4);
                pt2 = v->modeltoview(p1);
                if (v->clip3dfb(&pt1,&pt2)) {
                    pt1 = v->viewtoscreen(pt1);
                    pt2 = v->viewtoscreen(pt2);
                    if (! v->displaylist.moveto(pt1.x,pt1.y)) return;
                    if (! v->displaylist.lineto(pt2.x,pt2.y)) return;
                }
            }
            if (hfont != NULL) {
                SelectObject(v->gethdc(),oldhfont);
                DeleteObject(hfont);
            }
            size = v->displaylist.getcount();
        }
        if (! child && ! v->displaylist.end()) return;
#endif
    } else {
        Point xa1,ya1,o1;
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! child) {
            v->displaylist.setsavemode();
#ifdef _USING_QTCHAR
            if (! v->displaylist.start(this,9+sizeof(LOGFONTW)+nlines*12+RCSTRLEN(text) + 1 + options.test(8) * 72)) return;
#else
            if (! v->displaylist.start(this,9+sizeof(LOGFONT)+nlines*12+RCSTRLEN(text) + 1 + options.test(8) * 72)) return;
#endif
        }
        p1 = v->modeltoscreen(origin);
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;

        xa1 = v->modeltoscreen(origin + xaxis);
        v->transform2d(&xa1.x,&xa1.y);
        ya1 = v->modeltoscreen(origin + yaxis);
        v->transform2d(&ya1.x,&ya1.y);
        o1 = v->modeltoscreen(origin);
        v->transform2d(&o1.x,&o1.y);
        xa = xa1 - o1;
        xa.z = 0.0;
        if ((l = xa.length()) < db.getptoler()) {
            if (! child)
                v->displaylist.end();
        }
        xa /= l;
        ya = ya1 - o1;
        ya.z = 0.0;
        if ((l = ya.length()) < db.getptoler()) {
            if (! child)
                v->displaylist.end();
        }
        ya /= l;

        hjust = options.test(1) + options.test(2) * 2;
        vjust = options.test(3) + options.test(4) * 2;


        //  Compare the transformed z axis to the zaxis calculated from the transformed x and y axis to see if the sense has changed
        Point origin1,xaxis1,yaxis1,zaxis1,zaxis2;
        origin1 = v->modeltoscreen(origin);
        xaxis1 = v->modeltoscreen(origin + xaxis) - origin1;
        yaxis1 = v->modeltoscreen(origin + yaxis) - origin1;
        zaxis1 = v->modeltoscreen(origin + xaxis.cross(yaxis)) - origin1;
        zaxis2 = xaxis1.cross(yaxis1);

        if (zaxis1.dot(zaxis2) > 0.0) {
            //  If the text has been mirrored, need to adjust the justifications
            if (yaxis.dot(v->getvaxis()) * yaxis1.y <= 0) {
                ya = -ya;
                vjust = 2 - vjust;
            } else {
                xa = -xa;
                hjust = 2 - hjust;
            }
        }

        if (xa.x < -1.0) xa.x = -1.0;
        else if (xa.x > 1.0) xa.x = 1.0;
        a = acos(xa.x);
        if (xa.y > 0.0) a = 2.0 * M_PI - a;

        if (options.test(9))
            scale = v->getplotscale();
        else
            scale = 1.0;
        rh = height * v->getheight() / (v->getvmax() - v->getvmin()) * scale;
        if (options.test(13))  // Height is in points
            rh *= 25.4 / 72.0;
        rw = width * v->getwidth() / (v->getumax() - v->getumin()) * scale;
        if (rh > MAXTEXTHEIGHT) ih = (int)MAXTEXTHEIGHT;
        else if (rh < (int)-MAXTEXTHEIGHT) ih = (int)-MAXTEXTHEIGHT;
        else ih = int(rh);
        if (rw > MAXTEXTHEIGHT) iw = (int)MAXTEXTHEIGHT;
        else if (rw < (int)-MAXTEXTHEIGHT) iw = (int)-MAXTEXTHEIGHT;
        else iw = int(rw);
        if (ih == 0) {
            if (! child)
                v->displaylist.end();
            return;
        }
        lf.lfHeight = options.test(13) ? -ih : ih;
        lf.lfWidth = iw;
        lf.lfEscapement = int((a * 180.0 / M_PI + angle + v->getrotation2d()) * 10.0);
        lf.lfOrientation = int((a * 180.0 / M_PI + angle + v->getrotation2d())* 10.0);
        lf.lfWeight = (options.test(5) ? FW_BOLD : FW_NORMAL);
        lf.lfItalic = (BYTE)options.test(7);
        lf.lfUnderline = (BYTE)options.test(6);
        lf.lfStrikeOut = 0;
        lf.lfCharSet = (BYTE)fbCharSet;
        lf.lfOutPrecision = OUT_STROKE_PRECIS;
        lf.lfClipPrecision = CLIP_STROKE_PRECIS;
        lf.lfQuality = PROOF_QUALITY;
        lf.lfPitchAndFamily = 0;
#if ! defined(_WIN32_WCE)
        strcpy((RCCHAR*)lf.lfFaceName, font);
        //strcpy(lf.lfFaceName,font);
//WCEFIX
#endif
        if (! v->displaylist.selectfont(rw,rh,&lf)) return;
#ifdef _USING_QTCHAR
        hfont = CreateFontIndirectW(&lf);
#else
        hfont = CreateFontIndirect(&lf);
#endif
        sina = sin(a + (angle + v->getrotation2d()) / 180.0 * M_PI);
        cosa = cos(a + (angle + v->getrotation2d()) / 180.0 * M_PI);
        if (hfont != NULL)
            oldhfont = (RCHFONT) SelectObject(v->gethdc(),hfont);
#ifdef _USING_QTCHAR
        GetTextMetricsW(v->gethdc(),&tm);
#else
        GetTextMetrics(v->gethdc(),&tm);
#endif
        if (vjust == 0)
            oheight = -nlines * tm.tmHeight + tm.tmDescent - (nlines - 1) * tm.tmHeight * vertspacing;
        else if (vjust == 1)
            oheight = -(nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2;
        else
            oheight = 0.0;
        p1.x += oheight * sina;
        p1.y -= oheight * cosa;
        p3 = p1;
        maxh = int(tm.tmHeight * (nlines + (nlines - 1)*vertspacing));
        maxw = 0;

        if (hjust == 1) {
            p3.x -= maxw * cosa / 2.0;
            p3.y -= maxw * sina / 2.0;
        } else if (hjust == 2) {
            p3.x -= maxw * cosa;
            p3.y -= maxw * sina;
        }

        //  Underlined.
        if (options.test(12)) {
            Point hp1,hp2,hp3,hp4;
            hp1 = Point(p3.x,p3.y,0.0);
            hp2 = Point(p3.x + maxw * cosa,p3.y + maxw * sina,0.0);
            hp3 = Point(p3.x + maxw * cosa + maxh * sina,p3.y + maxw * sina - maxh * cosa,0.0);
            hp4 = Point(p3.x + maxh * sina,p3.y - maxh * cosa,0.0);

            Polygon3d polygon3d(hp1,hp2,hp3,hp4);




        }

        //  Boxed.
        if (options.test(8)) {
            if (! v->displaylist.moveto(p3.x,p3.y)) return;
            if (! v->displaylist.lineto(p3.x + maxw * cosa,p3.y + maxw * sina)) return;
            if (! v->displaylist.lineto(p3.x + maxw * cosa + maxh * sina,p3.y + maxw * sina - maxh * cosa)) return;
            if (! v->displaylist.lineto(p3.x + maxh * sina,p3.y - maxh * cosa)) return;
            if (! v->displaylist.lineto(p3.x,p3.y)) return;
        }



        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
#if defined(WIN32)
            SIZE size;
#ifdef _USING_QTCHAR
            GetTextExtentPoint32W(v->gethdc(), s,ll,&size);
#else
            GetTextExtentPoint(v->gethdc(),(TCHAR*)s,ll,&size);
#endif
            w = size.cx;
#else
            long r;
            r = GetTextExtent(v->gethdc(),s,ll);
            w = LOWORD(r);
#endif
            if (w > maxw) maxw = w;
            p2 = p1;
            if (hjust == 1) {
                p2.x -= w * cosa / 2.0;
                p2.y -= w * sina / 2.0;
            } else if (hjust == 2) {
                p2.x -= w * cosa;
                p2.y -= w * sina;
            }
            if (! v->displaylist.textout(p2.x,p2.y,s,ll)) return;
            p1.x += tm.tmHeight * (1.0 + vertspacing) * sina;
            p1.y -= tm.tmHeight * (1.0 + vertspacing) * cosa;
            if (s[ll] == 0) break;
        }

        if (hfont != NULL) {
            SelectObject(v->gethdc(),oldhfont);
            DeleteObject(hfont);
        }
        if (! child && ! v->displaylist.end()) return;
    }
}

void DrawQtTextGL(EntityHeader &header, const Point &position, const QFont &qfname, const QString &text, const QTransform &matrix, View3dSurface *v)
{
    QPainterPath path;

    // add the text to a path
    path.addText(0.0,0.0,qfname,text);
    // get the outline polygon
    QList<QPolygonF> textpolys = path.toSubpathPolygons(matrix);
    // draw the outine from the polygon vertices

    //bool AAon = glIsEnabled(GL_MULTISAMPLE);
    //if(!AAon)
    //    glEnable(GL_MULTISAMPLE);

    // use a one degree spline if the line style is not continuous
    if(header.getstyle() != 0)
    {
        for(int ip=0; ip<textpolys.size(); ip++)
        {
            QPolygonF thisPoly = textpolys.at(ip);
            // get a polygon
            Point *points = new Point[thisPoly.size()+1];
            for(int ie=0; ie<thisPoly.size(); ie++)
            {
                points[ie] = v->screentomodel(Point(thisPoly.at(ie).x(),v->getheight()-1-thisPoly.at(ie).y(),0.0));
            }
            points[thisPoly.size()] = points[0];
            BitMask visible(MaxViews);
            visible = *header.getvisible();
            Curve curve(header,thisPoly.size(),1,points,0,0,visible);
            curve.drawGL(v->getDrawmode(),0,v);
            delete [] points;
        }
    }
    else
    {
#if 0
    // using a gl line
    for(int ip=0; ip<textpolys.size(); ip++) {
        QPolygonF thisPoly = textpolys.at(ip);
        // get a polygon
        for(int ie=0; ie<thisPoly.size()-1; ie++) {
            // get an edge
            QPointF qp1,qp2;
            qp1 = thisPoly.at(ie);
            qp2 = thisPoly.at(ie+1);

            Point txp1 = v->screentomodel(Point(qp1.x(),v->getheight()-1-qp1.y(),0.0));
            Point txp2 = v->screentomodel(Point(qp2.x(),v->getheight()-1-qp2.y(),0.0));
            // use our own line drawing for thick lines
            DrawLine(txp1,txp2,v->curGLWeight);

        }
    }
#else
    // using a gl line strip
    for(int ip=0; ip<textpolys.size(); ip++)
    {
        QPolygonF thisPoly = textpolys.at(ip);
        // get a polygon
        Point *points = new Point[thisPoly.size()];
        for(int ie=0; ie<thisPoly.size(); ie++)
        {
            // get an edge
            points[ie] = v->screentomodel(Point(thisPoly.at(ie).x(),v->getheight()-1-thisPoly.at(ie).y(),0.0));
        }
        DrawLineStrip(points,thisPoly.size(),v->curGLWeight);
        delete [] points;
    }
#endif
    }

    //if(!AAon)
    //    glDisable(GL_MULTISAMPLE);
}

void FillQtTextGL(const Point &position, const QFont &font,const QString &text, const QTransform &matrix, View3dSurface *v)
{
    DPoint o, xa, ya,**verts;
    int nloops, *nvert;
    QPainterPath path;

    qDebug() << "font is bold : " << font.bold();

    // add the text to a path
    path.addText(0.0,0.0,font,text);
    //Point op = v->screentomodel(Point(position.x,double(v->getheight())-1.0-position.y,position.z));
    //path.addText(op.x,op.y,font,text);

    // get the outline polygon
    QList<QPolygonF> textpolys = path.toSubpathPolygons(matrix);

    if(textpolys.size() == 0) return;

    // count the number of loop and vertices
    nloops=textpolys.size();
    nvert=0;

    if((nvert = new int[nloops]) == NULL ) return;
    if((verts = new Point *[nloops]) == NULL) {
        delete [] nvert;
        return;
    }

    for(int ip=0; ip<textpolys.size(); ip++) {
        QPolygonF thisPoly = textpolys.at(ip);
        // get a polygon
        nvert[ip] = thisPoly.size()-1;
        verts[ip] = new Point[nvert[ip]];
        for(int ie=0; ie<thisPoly.size()-1; ie++) {
            // get a vertex
            QPointF qp;
            qp = thisPoly.at(ie);
            Point txp = v->screentomodel(Point(qp.x(),v->getheight()-1.0-qp.y()-0.5,position.z));
            //Point txp = Point(qp.x(),qp.y(),op.z);
            verts[ip][ie] = Point(txp.x,txp.y,txp.z);
        }
    }


    if(!glIsEnabled(GL_MULTISAMPLE)) {
        glEnable(GL_MULTISAMPLE);
        DrawPolygon(position,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),nloops,nvert,verts);
        glDisable(GL_MULTISAMPLE);
    } else
        DrawPolygon(position,Point(1.0,0.0,0.0),Point(0.0,1.0,0.0),nloops,nvert,verts);

    delete [] nvert;
    delete [] verts;
}

#ifdef USING_WIDGETS
char *Text::getchartext(void)
{
    delete [] tmptext;
    tmptext = new char[strlen(text)+1];
    strcpy(tmptext,qPrintable(QString(text)));
    return tmptext;
}

char *Text::getcharlabel(void)
{
    delete [] tmplabel;
    tmplabel = new char[strlen(label)+1];
    strcpy(tmplabel,qPrintable(QString(label)));
    return tmplabel;
}

char *Text::getcharfont(void)
{
    delete [] tmpfont;
    tmpfont = new char[strlen(font)+1];
    strcpy(tmpfont,qPrintable(QString(font)));
    return tmpfont;
}

//#define OLD_TEXT
void Text::drawGL(int drawmode, Transform *t, View3dSurface *v)
{
#if 1
    Point p1,p2,p3,p4,p1d;
    Point zero(0.0,0.0,0.0),o,xa,ya;
    int i,j,tlen,halo,i1,i2,status,metafile,ih,iw,ll,nlines,maxh,maxw,*dx,hjust,vjust;
    double scale,rh,w,rw,rtw,y,oheight,a,l,cosa,sina,xmaxh,xmaxw,xw,tmHeight,tmDescent,tmAscent,ppscale;
    RCCHAR *s;
    EntityHeader header(colour,layer,style,weight);

    // delete the qgi
    draw(DM_ERASE,t,v);

    if(drawmode == DM_ERASE)
        return;

    if (getselected() && drawmode == DM_NORMAL)
        drawmode = DM_SELECT;

    v->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);
    RCCOLORREF pcol = v->getcolour();
    SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
    GLfloat lwidth = v->curPen.widthF();
    glLineWidth(v->curGLWeight);

    Point xa1,ya1,o1;
    double angle1;

    for (halo = 0 ; halo < 1 + options.test(12) ; halo++) {
        if (t == NULL) {
            p1 = v->modeltoview(origin);
            p2 = v->modeltoview(origin);
            //p1 = v->modeltransform.transform(p1);
            //p2 = v->modeltransform.transform(p2);
        } else {
            p1 = v->modeltoview(t->transform(origin));
            p2 = v->modeltoview(t->transform(origin));
            //p1 = t->transform(v->modeltransform.transform(p1));
            //p2 = t->transform(v->modeltransform.transform(p3));
        }

        //p1 = v->viewtoscreen(p1);

        if (t == NULL)
            p1 = v->modeltoscreen(origin);
        else
            p1 = v->modeltoscreen(t->transform(origin));

        p1d = p1;
        v->transform2d(&p1.x,&p1.y);

        xa1 = v->modeltoscreen(t == 0 ? origin + xaxis : t->transform(origin + xaxis));
        v->transform2d(&xa1.x,&xa1.y);

        ya1 = v->modeltoscreen(t == 0 ? origin + yaxis : t->transform(origin + yaxis));
        v->transform2d(&ya1.x,&ya1.y);

        o1 = v->modeltoscreen(t == 0 ? origin : t->transform(origin));
        v->transform2d(&o1.x,&o1.y);

        xa = xa1 - o1;
        xa.z = 0.0;
        if ((l = xa.length()) < 1.0E-10) {
            //delete [] dx;
            return;
        }
        xa /= l;
        ya = ya1 - o1;
        ya.z = 0.0;
        if ((l = ya.length()) < 1.0E-10) {
            //delete [] dx;
            return;
        }
        ya /= l;

        hjust = options.test(1) + options.test(2) * 2;
        vjust = options.test(3) + options.test(4) * 2;

        //  Compare the transformed z axis to the zaxis calculated from the transformed x and y axis to see if the sense has changed
        Point origin1,xaxis1,yaxis1,zaxis1,zaxis2;
        origin1 = v->modeltoscreen(t == 0 ? origin : t->transform(origin));
        xaxis1 = v->modeltoscreen(t == 0 ? origin + xaxis : t->transform(origin + xaxis)) - origin1;
        yaxis1 = v->modeltoscreen(t == 0 ? origin + yaxis : t->transform(origin + yaxis)) - origin1;
        zaxis1 = v->modeltoscreen(t == 0 ? origin + xaxis.cross(yaxis) : t->transform(origin + xaxis.cross(yaxis))) - origin1;
        zaxis2 = xaxis1.cross(yaxis1);

        angle1 = -angle;

        // KMJ : TODO: check this
        if (zaxis1.dot(zaxis2) > 0.0) {
            //  If the text has been mirrored, need to adjust the justifications
            sina = sin(angle1 / 180.0 * M_PI);
            cosa = cos(angle1 / 180.0 * M_PI);
            if ((-xaxis * sina + yaxis * cosa).dot(-xaxis1 * sina + yaxis1 * cosa) <= 0) {
                ya = -ya;
                vjust = 2 - vjust;
                angle1 = -angle1;
            } else if ((xaxis * cosa + yaxis * sina).dot(xaxis1 * cosa + yaxis1 * sina) < 0.0) {
                xa = -xa;
                hjust = 2 - hjust;
                angle1 = -angle1;
            }
        }


        if (xa.x < -1.0) xa.x = -1.0;
        else if (xa.x > 1.0) xa.x = 1.0;
        a = acos(xa.x);
        if (xa.y > 0.0) a = 2.0 * M_PI - a;
        ya = xa.cross(ya).cross(xa).normalize();

        // scale is the plot scale used for "scaled" text
        if (options.test(9))
            scale = v->getplotscale();
        else
            scale = 1.0;

#ifndef OLD_TEXT
        // new text sizing code
        // height is the text height in mm
        // RC text height means the distance between one line of text and the next
        // in Qt the is the height + leading or the line spacing of the text
        // v->getheight() / (v->getvmax() - v->getvmin()) is the current zoom scale
        // we can set the Qt font height in points or pixels
        // we must set the Qt text height in screen space i.e pixel space
        //qDebug() << "text height : " << height;
        //qDebug() << "text zoom scale : " << v->getheight() / (v->getvmax() - v->getvmin());
        //qDebug() << "text plot scale : " << scale;

        rh = height * v->getheight() / (v->getvmax() - v->getvmin()) * scale;
        if (options.test(13)) // height is in points option conveting points to mm
            rh = height * 25.4 / 72.0 * v->getheight() / (v->getvmax() - v->getvmin()) * scale;

        rw = width * v->getwidth() / (v->getumax() - v->getumin()) * scale;

        if (rh > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT;
        else if (rh < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT;
        else ih = int(rh);
        if (rw > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT;
        else if (rw < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT;
        else iw = int(rw);
        if (ih == 0) {
            //delete [] dx;
            return;
        }
#else
        // need to find the correct font height that is required for the given height in points for Qt
        // KMJ : Height of a font must be in points for Qt
        // rh is the mm height between two line of text this is the font height plus the font descent height
        // KMJ : rh is the floating point height in pixels
        rh = height * v->getheight() / (v->getvmax() - v->getvmin()) * scale;
        if (options.test(13)) // height is in points option
            rh = height * 25.4 / 72.0 * v->getheight() / (v->getvmax() - v->getvmin()) * scale;
            //rh *= 25.4 / 72.0;

        rw = width * v->getwidth() / (v->getumax() - v->getumin()) * scale;

        if (rh > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT;
        else if (rh < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT;
        else ih = int(rh);
        if (rw > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT;
        else if (rw < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT;
        else iw = int(rw);
        if (ih == 0) {
            //delete [] dx;
            return;
        }
#endif
        //if(rh < 2)
        //    return;

#ifndef OLD_TEXT
        // the RealCAD text height means the line height or distance between lines of text
        // RealCAD ignores the leading of text so we must calculate the pixel or point height
        // to get the correct line height

        // create a reference font
        QString refName(font);
        QFont refFont(refName);
        //QFontMetricsF refFm(refFont);

        refFont.setPointSizeF(100.0);// use rh as points
        //refFont.setPointSizeF(rh*100.0);// use rh as points
        //refFont.setPixelSize(rh);// use rh as pixels
        //refFont.setPixelSize((int)rh*100);// use rh as pixels
        // get the metrics
        QFontMetricsF refFm(refFont);
        // get the height
        double refHeight = refFm.height();
        // scale the size to get the line height
        //double factor = rh * 100.0 / refHeight;
        double factor = 100.0 / refHeight;
        double fontHeight = rh * factor ;

        if(fontHeight < 1.0)
            return;

        // set up a device dependent font
        QString qfname(font);
        QFont qfont(qfname);
        //qfont.setHintingPreference(QFont::PreferNoHinting);
        qfont.setHintingPreference(QFont::PreferDefaultHinting);
        //qfont.setLetterSpacing(QFont::PercentageSpacing, 100.0);
        //qfont.setLetterSpacing(QFont::AbsoluteSpacing, 0.0);
        //qfont.setFixedPitch(true);
        //qfont.setFixedPitch(false);
        //qfont.setKerning(true);
        //qfont.setKerning(false);
        //qfont.setStyleStrategy(QFont::ForceOutline);
        // setting the font size in pixels
        //qfont.setPixelSize(fontHeight);
        qfont.setPointSizeF(fontHeight);
#else
        // calculate the Qt font pixel height required for the text height setting
        // calculate the point height for the font
        QString refName(font);
        QFont refFont(refName);
        //refFont.setHintingPreference(QFont::PreferNoHinting);
        refFont.setHintingPreference(QFont::PreferDefaultHinting);
        //refFont.setLetterSpacing(QFont::PercentageSpacing, 100.0);
        //refFont.setFixedPitch(true);
        //refFont.setKerning(true);
        refFont.setPointSize((int)rh);// use rh as points
        //refFont.setPixelSize(rh);// use rh as pixels
        QFontMetricsF refFm(refFont);
        double refHeight = refFm.height();// get the height as pixels for rh points
        // how many points did we get for for 1 pixel of rh
        double ptsperpx = rh / refHeight;
        // how many points to get the same height in pixels
        double fontHeight = (rh * ptsperpx);

        // KMJ: Qt fonts must be greater than zero height
        // checking here for zero font height
        //qDebug() << "fontHeight :" << fontHeight;
        if(int(fontHeight) <= 1)
            return;

        // set up a device independent font
        QString qfname(font);
        QFont qfont(qfname);
        //qfont.setHintingPreference(QFont::PreferNoHinting);
        qfont.setHintingPreference(QFont::PreferDefaultHinting);
        //qfont.setLetterSpacing(QFont::PercentageSpacing, 100.0);
        //qfont.setLetterSpacing(QFont::AbsoluteSpacing, 0.0);
        //qfont.setFixedPitch(true);
        //qfont.setKerning(true);
        //qfont.setKerning(false);
        //qfont.setFixedPitch(true);
        //qfont.setStyleStrategy(QFont::ForceOutline);
        qfont.setPointSizeF(fontHeight);
#endif
        //qDebug() << "font is exact match" << qfont.exactMatch();
        // set the font weight
        if(options.test(5))
            qfont.setWeight(QFont::Bold);
        else
            qfont.setWeight(QFont::Normal);
        // set italic
        qfont.setItalic(options.test(7));
        // set underline
        qfont.setUnderline(options.test(6));

        //qDebug() << "font Style :" << qfont.style();
        QFontInfo qfi(qfont);
        //qDebug() << "fontinfo family :" << qfi.family();
        //qDebug() << "fontinfo style name :" << qfi.styleName();
        //qDebug() << "fontinfo matches :" << qfi.exactMatch();
        //qDebug() << "fontinfo Style   :" << qfi.style();
        //qDebug() << "fontinfo point sizeF :" << qfi.pointSizeF();
        //qDebug() << "fontinfo point size  :" << qfi.pointSize();
        //qDebug() << "fontinfo pixel size  :" << qfi.pixelSize();

        /*
        if (options.test(13)) // height is in points option
        {
            qDebug() << "Model text height(mm)  :" << height * 0.352778;
            qDebug() << "Model text height(pt)  :" << height;
            qDebug() << "Qt font point size     :" << fontHeight;
        }
        else
        {
            qDebug() << "Model text height(mm)  :" << height;
            qDebug() << "Model text height(pt)  :" << height * 2.83465;
            qDebug() << "Qt font point size     :" << fontHeight;
        }
        */
        // get the font metrics
        QFontMetricsF fm(qfont);
        /*
        qDebug() << "fm pixel height        :" << fm.height();
        qDebug() << "fm pixel ascent        :" << fm.ascent();
        qDebug() << "fm pixel descent       :" << fm.descent();
        qDebug() << "fm pixel leading       :" << fm.leading();
        qDebug() << "fm pixel line spacing  :" << fm.lineSpacing();
        */
        QFontMetrics fmi(qfont);
        /*
        qDebug() << "fmi pixel height        :" << fmi.height();
        qDebug() << "fmi pixel ascent        :" << fmi.ascent();
        qDebug() << "fmi pixel descent       :" << fmi.descent();
        qDebug() << "fmi pixel leading       :" << fmi.leading();
        qDebug() << "fmi pixel line spacing  :" << fmi.lineSpacing();
        */
        //qDebug() << "average char width :" << fm.averageCharWidth();

        // calculate the stretch factor
        // this is done by scaling the font in width
        //double fstretch = width  / (height > 0.0 ? height : 1.0);
        //fstretch = fstretch == 0.0 ? 1.0 : fstretch;
        double fstretch = 1.0;
        if(width == 0.0)
            fstretch = 1.0;
        else
            fstretch = fabs(width);

        sina = sin(a - angle1 / 180.0 * M_PI);
        cosa = cos(a - angle1 / 180.0 * M_PI);

        metafile = 0;
        //metafile = GetDeviceCaps(v->gethdc(),TECHNOLOGY) == DT_METAFILE;

#ifndef OLD_TEXT
        tmHeight  = fm.height();
        tmDescent = fm.descent();
        tmAscent  = fm.ascent();
        // the line spacing can be adjusted so RealCAD ignores the leading/
        // this could be added by changing this conditional if required
#if 1
        int tmLeading = 0;
        int tmLineSpacing = tmHeight;
#else
        int tmLeading = fmi.leading();
        int tmLineSpacing = fmi.lineSpacing();
#endif
#else
        tmHeight  = fm.height();
        tmDescent = fm.descent();
        tmAscent  = fm.ascent();
#endif

        // not used
        /*
        double fipxHeight = qfi.pixelSize();
        double fiHeight = qfi.pointSizeF();
        double fiDescent = int(fm.descent() + 0.5);
        */
        //tmHeight = fmi.height();
        //tmDescent = fmi.descent();
        //tmAscent = fmi.ascent();

        // calculate the number of lines
        // text on windows have CR+LF line endings MAC OS X has LF line endings
        // find the line feed or carriage return or line feed and carriage return
        // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
        for (s = text,nlines = 1 ; *s!=0 ; s++) {
            nlines += (*s == 0x000D || *s == 0x000A) ;
            if(*s == 0x000D && (*s+1 != 0) && *(s+1) == 0x000A)
                s++;
        }


#ifndef OLD_TEXT
        // calculate the vertical justification and the line spacing
        if (vjust == 0) // bottom
            oheight = -nlines * (tmHeight + tmLeading) + tmDescent - (nlines - 1) * (tmHeight + tmLeading) * vertspacing;
            //oheight = -nlines * tmHeight + tmDescent - (nlines - 1) * tmHeight * vertspacing;
        else if (vjust == 1) // center
            oheight = -(nlines + (nlines - 1) * vertspacing) * tmHeight / 2;
        else
            oheight = 0.0; // top
#else
        // calculate the vertical justification and the line spacing
        if (vjust == 0) // bottom
            oheight = -nlines * tmHeight + tmDescent - (nlines - 1) * tmHeight * vertspacing;
        else if (vjust == 1) // center
            oheight = -(nlines + (nlines - 1) * vertspacing) * tmHeight / 2;
        else
            oheight = 0.0; // top
#endif
        /*
         // not used
        double fioheight= oheight;
        if (vjust == 0) // bottom
            fioheight = -nlines * fiHeight + fiDescent - (nlines - 1) * fiHeight * vertspacing;
        else if (vjust == 1) // center
            fioheight = -(nlines + (nlines - 1) * vertspacing) * fiHeight / 2;
        else
            fioheight = 0.0; // top
        */

        //p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
        //p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;
        p1.x += oheight * sina;
        p1.y += oheight * cosa;
        p3 = p1;

        //maxh = int(rh * (nlines + (nlines - 1)*vertspacing));
        maxh = (rh * (nlines + (nlines - 1)*vertspacing));
        maxw = 0;

        // walk each line of text and display it line by line
        ll=0;
        for (s = text ; *s!=0 ;  s += ll + 1) { // jump at the end of each line
            // text on windows have CR+LF line endings MAC OS X has LF line endings
            // find the line feed or carriage return or line feed and carriage return
            // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
            //for (ll = 0 ;  (s[ll] != 0x000D) && (s[ll] != 0x000A) && (s[ll] != 0) ; ll++) ;
            for (ll = 0 ; s[ll] != 0; ll++) {
                if(s[ll] == 0x000D || s[ll] == 0x000A) {
                    if(s[ll] == 0x000D && s[ll+1] != 0 && s[ll+1] == 0x000A)
                        ll++;
                    break;
                }
            }

            // KMJ: TODO: do metafile stuff
            // get the width
            if (metafile) {
                w = fm.width(QString(s,ll));
            } else if (drawmode == DM_INVERT) {
                QString qss = QString().append(s,ll);
                rtw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignRight|Qt::AlignTop,qss).width();
                //rtw = fm.width(qss);
                //w = (int) rtw;
                w = rtw;
            } else {
                QString qss = QString().append(s,ll);
                rtw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignRight|Qt::AlignTop,qss).width();
                //rtw = fm.width(qss);
                //rtw = fm.boundingRect(qss).width();
                //w = (int) rtw;
                w = rtw;
            }
            if (w > maxw)
                maxw = w;

            //qDebug() << "text : " << QString().append(s,ll);
            //qDebug() << "bounding rect W : " << fm.boundingRect(QRect(0,0,0,0),Qt::AlignRight|Qt::AlignTop,QString().append(s,ll)).width();
            //qDebug() << "bounding rect H : " << fm.boundingRect(QRect(0,0,0,0),Qt::AlignRight|Qt::AlignTop,QString().append(s,ll)).height();

            // calculate the horizontal justification
            p2 = p1;
            //p2.x = int(p1.x+0.5);
            //p2.y = int(p1.y+0.5);
            //p2.z = p1.z;

            if (hjust == 1) {
                p2.x -= rtw*fstretch * cosa / 2.0;
                p2.y += rtw*fstretch * sina / 2.0;
            } else if (hjust == 2) {
                p2.x -= rtw*fstretch * cosa;
                p2.y += rtw*fstretch * sina;
            }

            // draw the halo box before (behind) the text with OpenGL
            if (drawmode != DM_SELECT && options.test(12) && halo == 0) {
                Point *hp,hp3;
                Point hp1[4];
                int nvert;

                hp3 = p3;

                if (hjust == 1) {
                    hp3.x -= maxw * fstretch * cosa / 2.0;
                    hp3.y += maxw * fstretch * sina / 2.0;
                } else if (hjust == 2) {
                    hp3.x -= maxw * fstretch * cosa;
                    hp3.y += maxw * fstretch * sina;
                }

                //if(t)
                //   p3 = t->transform(p3);

                hp3.x += -ih / 10.0 * cosa - ih/10.0 * sina;
                hp3.y += +ih / 10.0 * sina - ih/10.0 * cosa;

                v->setupGL(linestylescale,0.0,drawmode,halocolour,style,weight,0);
                RCCOLORREF pcol = v->getcolour();
                SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
                GLfloat lwidth = v->curPen.widthF();
                glLineWidth(v->curGLWeight);

#if 1
                hp1[0] = Point(hp3.x,v->getheight() - 1.0 - hp3.y,0.0);
                hp1[1] = Point(hp3.x + (maxw*fstretch+ih/5.0) * cosa,v->getheight() - 1.0 - ((hp3.y - (maxw*fstretch+ih/5.0) * sina)),0.0);
                hp1[2] = Point(hp3.x + (maxw*fstretch+ih/5.0) * cosa + (maxh+ih/5.0) * sina,v->getheight() - 1.0 - ((hp3.y - (maxw*fstretch+ih/5.0) * sina + (maxh+ih/5.0) * cosa)),0.0);
                hp1[3] = Point(hp3.x + (maxh+ih/5.0) * sina,v->getheight() - 1.0 - ((hp3.y + (maxh+ih/5.0) * cosa)),0.0);
#else
                hp1[0] = Point(int(hp3.x),v->getheight() - 1.0 - int(hp3.y),0.0);
                hp1[1] = Point(int(hp3.x + (maxw*fstretch+ih/5.0) * cosa),v->getheight() - 1.0 - int((hp3.y - (maxw*fstretch+ih/5.0) * sina)),0.0);
                hp1[2] = Point(int(hp3.x + (maxw*fstretch+ih/5.0) * cosa + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((hp3.y - (maxw*fstretch+ih/5.0) * sina + (maxh+ih/5.0) * cosa)),0.0);
                hp1[3] = Point(int(hp3.x + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((hp3.y + (maxh+ih/5.0) * cosa)),0.0);
#endif
                hp1[0] = v->screentomodel(hp1[0]);
                hp1[1] = v->screentomodel(hp1[1]);
                hp1[2] = v->screentomodel(hp1[2]);
                hp1[3] = v->screentomodel(hp1[3]);

                nvert = 4;
                hp = hp1;
                //v->fill(0,&hp,&nvert,1,1);
                v->fillGL(&hp,&nvert,1,1);

                v->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);

                //p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
                //p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;

            }

            // set up for text
            v->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);
            RCCOLORREF pcol = v->getcolour();
            SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
            GLfloat lwidth = v->curPen.widthF();
            glLineWidth(v->curGLWeight);

            // draw the text
            if(options.test(10) /*|| drawmode == DM_SCROLLING || drawmode == DM_SELECT */)
            { // outline font
                if (drawmode != DM_INVERT)
                {
                    double x,y;

                    if (v->gettransform2d())
                    {
                        if(fm.height() > 0.0)
                        {
                            SIZE size;
                            x = p2.x;
                            y = p2.y;
                            size.cx = fm.width(QString(s,ll));
                            size.cy = fm.height();

                            if (size.cx < size.cy) size.cx = size.cy;
                            if (size.cy < size.cx) size.cy = size.cx;

                            if (p1d.x + size.cx * 2 > 0.0 && p1d.x - size.cx * 2 < v->getwidth() &&
                                p1d.y + size.cy * 2 > 0.0 && p1d.y - size.cy * 2 < v->getheight())
                            {
                                QString oString;
                                oString.append(s,ll);
                                double da = a * 180.0 / M_PI - angle1;
                                QTransform stf;
                                stf.translate(x,y);
                                stf.rotate(-da);
                                stf.scale(fstretch, 1.0);
                                stf.translate(0.0,tmAscent);
                                // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                                // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                                DrawQtTextGL(header,Point(0.0,0.0,0.0),qfont,oString,stf,v);
                                //FillQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
                            }
                        }
                    } else {
                        if(fm.height() > 0.0)
                        {
                            SIZE size;
                            // KMJ: TODO: check metafile stuff
                            if (metafile)
                            {
                                size.cx = fm.width(QString(s,ll));
                                size.cy = fm.height();
                                ;//GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
                            } else {
                                size.cx = fm.width(QString(s,ll));
                                size.cy = fm.height();
                            }

                            if (p2.x + size.cx > 0.0 && p2.x - size.cx < v->getwidth() &&
                                p2.y + size.cy > 0.0 && p2.y - size.cy < v->getheight())
                            {
                                QString oString;
                                oString.append(s,ll);
                                double da = a * 180.0 / M_PI - angle1;
                                QTransform stf;
                                stf.translate(p2.x,p2.y);
                                stf.rotate(-da);
                                stf.scale(fstretch, 1.0);
                                stf.translate(0.0,tmAscent);
                                // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                                // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                                DrawQtTextGL(header,Point(0.0,0.0,0.0),qfont,oString,stf,v);
                                //FillQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
                            } else
                                p2.x += 1.0E-30;
                        }
                    }
                } else {
                    if(fm.height() > 0.0)
                    {
                        SIZE size;

                        size.cx = (long)fm.width(QString(s,ll));
                        size.cy = (long)fm.height();

                        //KMJ: TODO: check when transformed
                        if (v->gettransform2d())
                        {
                            if (size.cx < size.cy) size.cx = size.cy;
                            if (size.cy < size.cx) size.cy = size.cx;
                        } else
                            p1d = p2;

                        if (p1d.x + size.cx > 0.0 && p1d.x - size.cx < v->getwidth() &&
                                p1d.y + size.cy > 0.0 && p1d.y - size.cy < v->getheight())
                        {
                            QString oString;
                            oString.append(s,ll);
                            // add the text to a path
                            double da = a * 180.0 / M_PI - angle1;
                            QTransform stf;
                            stf.translate(p2.x,p2.y);
                            stf.rotate(-da);
                            stf.scale(fstretch, 1.0);
                            stf.translate(0.0,tmAscent);
                            // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            DrawQtTextGL(header,Point(0.0,0.0,0.0),qfont,oString,stf,v);
                            //FillQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
                        }
                    }
                }
            } else { // normal filled font
                if (drawmode != DM_INVERT)
                {
                    double x,y;

                    // what is this for??
                    //SetBkMode(v->gethdc(),TRANSPARENT);

                    // KMJ: TODO: check when 2D transform is used
                    if (v->gettransform2d())
                    {
                        if(fm.height() > 0.0)
                        {
                            SIZE size;
                            x = p2.x;
                            y = p2.y;
                            size.cx = fm.width(QString(s,ll));
                            size.cy = fm.height();

                            if (size.cx < size.cy) size.cx = size.cy;
                            if (size.cy < size.cx) size.cy = size.cx;

                            if (p1d.x + size.cx * 2 > 0.0 && p1d.x - size.cx * 2 < v->getwidth() &&
                                p1d.y + size.cy * 2 > 0.0 && p1d.y - size.cy * 2 < v->getheight())
                            {

                                // add the text to a path
                                double da = a * 180.0 / M_PI - angle1;
                                QTransform stf;
                                stf.translate(x,y);
                                stf.rotate(-angle1);
                                stf.scale(fstretch, 1.0);
                                stf.translate(0.0,tmAscent);
                                // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                                // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                                //DrawQtTextGL(Point(0.0,0.0,0.0),qfont,QString(s),stf,v);
                                FillQtTextGL(Point(0.0,0.0,0.0),qfont,QString(s),stf,v);
                            }
                        }
                    } else {
                        if(fm.height() > 0.0)
                        {
                            SIZE size;
                            // KMJ: TODO: check metafile stuff
                            if (metafile)
                            {
                                ;//GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
                            } else {
    #ifndef OLD_TEXT
                                //size.cx = fmi.width(QString(s,ll));
                                //size.cx = fmi.width(QString(s,ll));
                                size.cx = fm.width(QString(s,ll));
                                size.cy = fm.height();
    #else
                                size.cx = fm.width(QString(s,ll));
                                size.cy = fm.height();
    #endif
                            }

                            //SetTextAlign(v->gethdc(),TA_TOP | TA_LEFT);

                            if (size.cx < size.cy)
                                size.cx = size.cy;
                            if (size.cy < size.cx)
                                size.cy = size.cx;

                            if (p2.x + size.cx > 0.0 && p2.x - size.cx < v->getwidth() &&
                                p2.y + size.cy > 0.0 && p2.y - size.cy < v->getheight())
                            {
                                QString oString;
                                oString.append(s,ll);
                                // add the text to a path
                                double da = a * 180.0 / M_PI - angle1;
                                QTransform stf;
                                stf.translate(p2.x,p2.y);
                                stf.rotate(-da);
                                stf.scale(fstretch, 1.0);
                                stf.translate(0.0,tmAscent + tmLeading);
                                //stf.translate(0.0,tmAscent);

                                // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                                // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                                /*
                                qDebug() << "output string : " << oString;
                                qDebug() << "origin.x :" << origin.x;
                                qDebug() << "origin.y :" << origin.y;
                                //Point ops = v->modeltoscreen(origin);
                                Point ops = v->modeltoscreen(Point(10.0,10.0,0.0));
                                qDebug() << "origin to screen x :" << ops.x;
                                qDebug() << "origin to screen y :" << ops.y;
                                v->transform2d(&ops.x,&ops.y);
                                qDebug() << "origin transf 2d x :" << ops.x;
                                qDebug() << "origin transf 2d y :" << ops.y;
                                //v->untransform2d(&ops.x,&ops.y);
                                //qDebug() << "origin to utransf 2d  x (after 2d) :" << ops.x;
                                //qDebug() << "origin to utransf 2d y (after 2d) :" << ops.y;
                                Point opm;
                                //opm = v->screentomodel(Point(ops.x,ops.y,0.0));
                                v->coordinate3d(rint(ops.x),rint(ops.y),&opm);
                                qDebug() << "origin to model x :" << opm.x;
                                qDebug() << "origin to model y :" << opm.y;
                                qDebug() << "p2.x :" << p2.x;
                                qDebug() << "p2.y :" << p2.y;
                                Point mp2 = v->screentomodel(p2);
                                qDebug() << "mp2.x :" << mp2.x;
                                qDebug() << "mp2.y :" << mp2.y;
                                */

                                if(oString.length() > 0)
                                    FillQtTextGL(Point(0.0,0.0,p2.z),qfont,oString,stf,v);
                            } else
                                p2.x += 1.0E-30;
                        }
                    }
                    // what is this for??
                    //SetBkMode(v->gethdc(),OPAQUE);
                } else {
                    if(fm.height() > 0.0)
                    {
                        SIZE size;

                        size.cx = (long)fm.width(QString(s,ll));
                        size.cy = (long)fm.height();

                        //KMJ: TODO: check when transformed
                        if (v->gettransform2d())
                        {
                            if (size.cx < size.cy) size.cx = size.cy;
                            if (size.cy < size.cx) size.cy = size.cx;
                        } else
                            p1d = p2;

                        if (p1d.x + size.cx > 0.0 && p1d.x - size.cx < v->getwidth() &&
                            p1d.y + size.cy > 0.0 && p1d.y - size.cy < v->getheight())
                        {
                            QString oString;
                            oString.append(s,ll);
                            // add the text to a path
                            double da = a * 180.0 / M_PI - angle1;
                            QTransform stf;
                            stf.translate(p2.x,p2.y);
                            stf.rotate(-da);
                            stf.scale(fstretch, 1.0);
                            stf.translate(0.0,tmAscent);
                            // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            //DrawQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
                            FillQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
                        }
                    }
                }
            } // end if outline font

            //p1.x += rh * (1.0 + vertspacing) * sina;
            //p1.y += rh * (1.0 + vertspacing) * cosa;

            // debug
            /*
            Point pl1,pl2;
            pl1 = p1;
            pl1.y = v->getheight()-1-p1.y-tmAscent;
            pl2.x = pl1.x + maxw;
            pl2.y = pl1.y;
            pl2.z = pl1.z;
            //

            v->line3dGL(v->screentomodel(pl1),v->screentomodel(pl2));
            pl1 = p1;
            pl1.y = v->getheight()-1-p1.y;
            pl2.x = pl1.x + maxw;
            pl2.y = pl1.y;
            pl2.z = pl1.z;
            v->line3dGL(v->screentomodel(pl1),v->screentomodel(pl2));
            */

            //p1.x += tmLineSpacing * (fabs(vertspacing) > 0 ? vertspacing : 1.0) * sina;
            //p1.y += tmLineSpacing * (fabs(vertspacing) > 0 ? vertspacing : 1.0) * cosa;
            p1.x += tmLineSpacing * (1.0 + vertspacing) * sina;
            p1.y += tmLineSpacing * (1.0 + vertspacing) * cosa;

            if (s[ll] == 0) break;
        }// end of line of text walking loop


        /*
         * // not sure why this is here probably don't need it
        if (hjust == 1) {
            p3.x -= maxw * fstretch * cosa / 2.0;
            p3.y += maxw * fstretch * sina / 2.0;
        } else if (hjust == 2) {
            p3.x -= maxw * fstretch * cosa;
            p3.y += maxw * fstretch * sina;
        }
        */

        // draw the box
        if (options.test(8)) {
            Point bp3;
            double x1,y1,x2,y2;

            v->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);
            RCCOLORREF pcol = v->getcolour();
            SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
            GLfloat lwidth = v->curPen.widthF();
            glLineWidth(v->curGLWeight);

            bp3 = p3;

            if (hjust == 1) {
                bp3.x -= maxw * fstretch * cosa / 2.0;
                bp3.y += maxw * fstretch * sina / 2.0;
            } else if (hjust == 2) {
                bp3.x -= maxw * fstretch * cosa;
                bp3.y += maxw * fstretch * sina;
            }

            bp3.x += -ih / 10.0 * cosa - ih/10.0 * sina;
            bp3.y += +ih / 10.0 * sina - ih/10.0 * cosa;

            // top line
            x1 = bp3.x;
            y1 = bp3.y;
            x2 = bp3.x + (maxw*fstretch+ih/5.0) * cosa;
            y2 = bp3.y - (maxw*fstretch+ih/5.0) * sina;

            if(0)
                //if (v->gethwnd() != 0)
            {
                //change(int c,int l,int s,int w)
                Line line(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                line.EntityHeader::change(colour,0,style,weight);
                line.drawGL(drawmode,t,v);
                //v->line3dGL(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
            } else { //if (v->clip2d(&x1,&y1,&x2,&y2))
                //Line line(v->screentomodel(Point(x1,v->getheight() - 1.0- y1,0.0)),v->screentomodel(Point(x2,v->getheight() - 1.0 - y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(int(x1),v->getheight() - 1.0 - y1,0.0)),v->screentomodel(Point(int(x2),v->getheight() -  1.0 - int(y2),0.0)));
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
            }

            // right vertical line
            x1 = bp3.x + (maxw*fstretch+ih/5.0) * cosa;
            y1 = bp3.y - (maxw*fstretch+ih/5.0) * sina;
            x2 = bp3.x + (maxw*fstretch+ih/5.0) * cosa + (maxh+ih/5.0) * sina;
            y2 = bp3.y - (maxw*fstretch+ih/5.0) * sina + (maxh+ih/5.0) * cosa;
            if(0)
                //if (v->gethwnd() != 0)
            {
                Line line(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                line.EntityHeader::change(colour,0,style,weight);
                line.drawGL(drawmode,t,v);
                //v->line3dGL(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //qline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
            } else { //if (v->clip2d(&x1,&y1,&x2,&y2))
                //Line line(v->screentomodel(Point(x1,v->getheight() - 1.0 - y1,0.0)),v->screentomodel(Point(x2,v->getheight()- 1.0  - y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(int(x1),v->getheight() - 1.0 - int(y1),0.0)),v->screentomodel(Point(int(x2),v->getheight() - 1.0 - int(y2),0.0)));
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
            }

            // bottom line
            x1 = bp3.x + (maxw*fstretch+ih/5.0) * cosa + (maxh+ih/5.0) * sina;
            y1 = bp3.y - (maxw*fstretch+ih/5.0) * sina + (maxh+ih/5.0) * cosa;
            x2 = bp3.x + (maxh+ih/5.0) * sina;
            y2 = bp3.y + (maxh+ih/5.0) * cosa;
            if(0)
                //if (v->gethwnd() != 0)
            {
                //Line line(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //qline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
            } else { //if (v->clip2d(&x1,&y1,&x2,&y2))
                //Line line(v->screentomodel(Point(x1,v->getheight() - 1.0 - y1,0.0)),v->screentomodel(Point(x2,v->getheight() - 1.0 - y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(int(x1),v->getheight() - 1.0 - int(y1),0.0)),v->screentomodel(Point(int(x2),v->getheight() - 1.0 - int(y2),0.0)));
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
            }

            // left vertical line
            x1 = bp3.x + (maxh+ih/5.0) * sina;
            y1 = bp3.y + (maxh+ih/5.0) * cosa;
            x2 = bp3.x;
            y2 = bp3.y;
            if(0)
                //if (v->gethwnd() != 0)
            {
                //Line line(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //qline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
            } else { //if (v->clip2d(&x1,&y1,&x2,&y2))
                //Line line(v->screentomodel(Point(x1,v->getheight() - 1.0  - y1,0.0)),v->screentomodel(Point(x2,v->getheight() - 1.0  - y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(int(x1),v->getheight() - 1.0 - int(y1),0.0)),v->screentomodel(Point(int(x2),v->getheight() - 1.0 - int(y2),0.0)));
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
            }

            //p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
            //p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;

        }
    }
#else
    Point p1,p2,p3,p4,p1d;

    RCHFONT cwoldhfont,oldhfont,hfont,printerhfont,printeroldhfont,hreferencefont;
    //BYTE fbCharSet;
    int i,j,tlen,halo,i1,i2,status,metafile,ih,iw,ll,nlines,maxh,maxw,*dx,hjust,vjust;
    double scale,rh,w,rw,rtw,y,oheight,a,l,cosa,sina,xmaxh,xmaxw,xw,tmHeight,tmDescent,tmAscent,ppscale;
    //#ifdef _USING_QTCHAR
    // TEXTMETRICW tm;
    //#else
    // TEXTMETRIC tm;
    //#endif
    Point zero(0.0,0.0,0.0),o,xa,ya;
    char buffer[GGO_BUF_SIZE];
    RCCHAR *s;
    Transform tr;
    SIZE size;

    QGraphicsItem *qgline=0,*qgtext=0,*qgrect=0;
    QList<QGraphicsItem *> glist;

    // delete the qgi
    draw(DM_ERASE,t,v);


    if(drawmode == DM_ERASE)
        return;

    //if(cadwindow->getcurrentsurface()->getrepaintstyle() != Wireframe)
    //    return;

    if (getselected() && drawmode == DM_NORMAL)
        drawmode = DM_SELECT;

    v->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);
    RCCOLORREF pcol = v->getcolour();
    SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
    GLfloat lwidth = v->curPen.widthF();
    glLineWidth(v->curGLWeight);

#if 0
    if (0 && t == NULL) {
        if (v->displaylist.draw(this,drawmode)) {
            return;
        }
        if (! v->displaylist.geterrorstatus())
            addtodisplaylist(v,0);
        if (v->displaylist.draw(this,drawmode)) {
            return;
        }
    }
#endif

    // transform the origin point
    // get the rotations
    // get the font metrics
    // get the bounding box H and W
    // draw the halo
    // draw the text
    // draw the bounding box

    Point xa1,ya1,o1;
    double angle1;

    //int dxlen = RCSTRLEN(text)+1;
    //dx = new int[RCSTRLEN(text)+1];

    //halo = 0 ;
    for (halo = 0 ; halo < 1 + options.test(12) ; halo++) {
        if (t == NULL) {
            p1 = v->modeltoview(origin);
            p2 = v->modeltoview(origin);
            //p1 = v->modeltransform.transform(p1);
            //p2 = v->modeltransform.transform(p2);
        } else {
            p1 = v->modeltoview(t->transform(origin));
            p2 = v->modeltoview(t->transform(origin));
            //p1 = t->transform(v->modeltransform.transform(p1));
            //p2 = t->transform(v->modeltransform.transform(p3));
        }
        //if (! v->clip3dfb(&p1,&p2))
        //    return;

        if (t == NULL)
            p1 = v->modeltoscreen(origin);
        else
            p1 = v->modeltoscreen(t->transform(origin));

        p1d = p1;
        v->transform2d(&p1.x,&p1.y);

        //if (options.test(0))
        //  fbCharSet = SYMBOL_CHARSET;
        //else
        //  fbCharSet = ANSI_CHARSET;//DEFAULT_CHARSET;//

        xa1 = v->modeltoscreen(t == 0 ? origin + xaxis : t->transform(origin + xaxis));
        v->transform2d(&xa1.x,&xa1.y);

        ya1 = v->modeltoscreen(t == 0 ? origin + yaxis : t->transform(origin + yaxis));
        v->transform2d(&ya1.x,&ya1.y);

        o1 = v->modeltoscreen(t == 0 ? origin : t->transform(origin));
        v->transform2d(&o1.x,&o1.y);

        xa = xa1 - o1;
        xa.z = 0.0;
        if ((l = xa.length()) < 1.0E-10) {
            //delete [] dx;
            return;
        }
        xa /= l;
        ya = ya1 - o1;
        ya.z = 0.0;
        if ((l = ya.length()) < 1.0E-10) {
            //delete [] dx;
            return;
        }
        ya /= l;

        hjust = options.test(1) + options.test(2) * 2;
        vjust = options.test(3) + options.test(4) * 2;

        //  Compare the transformed z axis to the zaxis calculated from the transformed x and y axis to see if the sense has changed
        Point origin1,xaxis1,yaxis1,zaxis1,zaxis2;
        origin1 = v->modeltoscreen(t == 0 ? origin : t->transform(origin));
        xaxis1 = v->modeltoscreen(t == 0 ? origin + xaxis : t->transform(origin + xaxis)) - origin1;
        yaxis1 = v->modeltoscreen(t == 0 ? origin + yaxis : t->transform(origin + yaxis)) - origin1;
        zaxis1 = v->modeltoscreen(t == 0 ? origin + xaxis.cross(yaxis) : t->transform(origin + xaxis.cross(yaxis))) - origin1;
        zaxis2 = xaxis1.cross(yaxis1);

        angle1 = -angle;

        // KMJ : TODO: check this
        if (zaxis1.dot(zaxis2) > 0.0) {
            //  If the text has been mirrored, need to adjust the justifications
            sina = sin(angle1 / 180.0 * M_PI);
            cosa = cos(angle1 / 180.0 * M_PI);
            if ((-xaxis * sina + yaxis * cosa).dot(-xaxis1 * sina + yaxis1 * cosa) <= 0) {
                ya = -ya;
                vjust = 2 - vjust;
                angle1 = -angle1;
            } else if ((xaxis * cosa + yaxis * sina).dot(xaxis1 * cosa + yaxis1 * sina) < 0.0) {
                xa = -xa;
                hjust = 2 - hjust;
                angle1 = -angle1;
            }
        }


        if (xa.x < -1.0) xa.x = -1.0;
        else if (xa.x > 1.0) xa.x = 1.0;
        a = acos(xa.x);
        if (xa.y > 0.0) a = 2.0 * M_PI - a;
        ya = xa.cross(ya).cross(xa).normalize();

        if (options.test(9))
            scale = v->getplotscale();
        else
            scale = 1.0;

        // rh is the height between two line of text this is the font height plus the font descent height
        // need to find the correct font height that is required for the given height in points for Qt
        // KMJ : Height of a font must be in points for Qt
        rh = height * v->getheight() / (v->getvmax() - v->getvmin()) * scale;
        if (options.test(13)) // height is in points option
            rh *= 25.4 / 72.0;

        rw = width * v->getwidth() / (v->getumax() - v->getumin()) * scale;
        if (rh > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT;
        else if (rh < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT;
        else ih = int(rh);
        if (rw > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT;
        else if (rw < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT;
        else iw = int(rw);
        if (ih == 0) {
            //delete [] dx;
            return;
        }

        // calculate the Qt font pixel height required for the text height setting
        // calculate the point height for the font
        QString refName(font);
        QFont refFont(refName);
        //refFont.setHintingPreference(QFont::PreferNoHinting);
        //refFont.setLetterSpacing(QFont::PercentageSpacing, 100.0);
        refFont.setKerning(false);
        //refFont.setPointSize((int)rh);// use rh as points
        refFont.setPointSizeF(rh);// use rh as points
        QFontMetricsF refFm(refFont);
        double refHeight = refFm.height();// get the height as pixels for rh points
        // how many points did we get for for 1 pixel of rh
        double ptsperpx = rh / refHeight;
        // how many points to get the same height in pixels
        int fontHeight = int(rh * ptsperpx);

        // KMJ: Qt fonts must be greater than zero height
        // checking here for zero font height
        if(fontHeight <= 0.0)
            return;

        // set up a device independent font
        QString qfname(font);
        QFont qfont(qfname);
        //qfont.setHintingPreference(QFont::PreferNoHinting);
        //qfont.setLetterSpacing(QFont::PercentageSpacing, 100.0);
        qfont.setKerning(false);
        //qfont.setStyleStrategy(QFont::ForceOutline);
        qfont.setPointSize(fontHeight);
        // set the font weight
        if(options.test(5))
            qfont.setWeight(QFont::Bold);
        else
            qfont.setWeight(QFont::Normal);
        // set italic
        qfont.setItalic(options.test(7));
        // set underline
        qfont.setUnderline(options.test(6));

        // get the font metrics
        QFontMetricsF fm(qfont);

        // calculate the stretch factor
        // this is done by scaling the font in width
        //double fstretch = width  / (height > 0.0 ? height : 1.0);
        //fstretch = fstretch == 0.0 ? 1.0 : fstretch;
        double fstretch = 1.0;
        if(width == 0.0)
            fstretch = 1.0;
        else
            fstretch = fabs(width);

        sina = sin(a - angle1 / 180.0 * M_PI);
        cosa = cos(a - angle1 / 180.0 * M_PI);

        metafile = 0;
        //metafile = GetDeviceCaps(v->gethdc(),TECHNOLOGY) == DT_METAFILE;

        tmHeight = fm.height();
        tmDescent = fm.descent();
        tmAscent = fm.ascent();

        // calculate the number of lines
        // text on windows have CR+LF line endings MAC OS X has LF line endings
        // find the line feed or carriage return or line feed and carriage return
        // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
        for (s = text,nlines = 1 ; *s!=0 ; s++) {
            nlines += (*s == 0x000D || *s == 0x000A) ;
            if(*s == 0x000D && (*s+1 != 0) && *(s+1) == 0x000A)
                s++;
        }

        //KMJ: TODO: do the printer stuff
        if (0 && v->getprinterhdc() != 0) {
            if (options.test(9))  //  In paper space, use the actual text size if scaled.
                scale = 1.0;
            else
                scale = 1.0 / v->getplotscale();
            if (v->gettransform2d())
                ppscale = v->getheight() / (v->getvmax() - v->getvmin()) * v->getplotscale() *
                          cadwindow->printpreviewwindow->getprintscale() / (double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) / 25.4);
            else
                ppscale = v->getheight() / (v->getvmax() - v->getvmin()) *
                          cadwindow->printpreviewwindow->getprintscale() / (double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) / 25.4);

            rh /= ppscale;
            rw /= ppscale;
            //               rh = height * scale / cadwindow->printpreviewwindow->getprintscale() * double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) / 25.4;
            //               rw = width * scale / cadwindow->printpreviewwindow->getprintscale() * double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSY)) / 25.4;
            if (rh > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT;
            else if (rh < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT;
            else ih = int(rh);
            if (rw > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT;
            else if (rw < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT;
            else iw = int(rw);
            if (ih == 0) {
                if (hfont != 0) {
                    if (metafile)
                        SelectObject(cadwindow->gethdc(),cwoldhfont);
                    else if (drawmode == DM_INVERT)
                        SelectObject(cadwindow->getoffscreenhdc(),cwoldhfont);
                    SelectObject(v->gethdc(),oldhfont);
                    DeleteObject(hfont);
                }
                return;
            }
            //lf.lfHeight = options.test(13) ? -ih : ih;
            //lf.lfWidth = iw;
            //printerhfont = CreateFontIndirectW(&lf);

            //printeroldhfont = (RCHFONT) SelectObject(v->getprinterhdc(),printerhfont);
            //GetTextMetricsW(v->getprinterhdc(),&tm);
            //tmHeight = tm.tmHeight * ppscale;
            //tmDescent = tm.tmDescent * ppscale;
        }

        // calculate the vertical justification and the line spacing
        if (vjust == 0) // bottom
            oheight = -nlines * tmHeight + tmDescent - (nlines - 1) * tmHeight * vertspacing;
        else if (vjust == 1) // center
            oheight = -(nlines + (nlines - 1) * vertspacing) * tmHeight / 2;
        else
            oheight = 0.0; // top

        //p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
        //p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;
        p1.x += oheight * sina;
        p1.y += oheight * cosa;
        p3 = p1;

        maxh = int(rh * (nlines + (nlines - 1)*vertspacing));
        maxw = 0;

        // walk each line of text and display it line by line
        ll=0;
        for (s = text ; *s!=0 ;  s += ll + 1) { // jump at the end of each line
            // text on windows have CR+LF line endings MAC OS X has LF line endings
            // find the line feed or carriage return or line feed and carriage return
            // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
            //for (ll = 0 ;  (s[ll] != 0x000D) && (s[ll] != 0x000A) && (s[ll] != 0) ; ll++) ;
            for (ll = 0 ; s[ll] != 0; ll++) {
                if(s[ll] == 0x000D || s[ll] == 0x000A) {
                    if(s[ll] == 0x000D && s[ll+1] != 0 && s[ll+1] == 0x000A)
                        ll++;
                    break;
                }
            }

            // KMJ: TODO: do metafile stuff
            // get the width
            if (metafile) {
                w = fm.width(QString(s,ll));
            } else if (drawmode == DM_INVERT) {
                QString qss = QString().append(s,ll);
                rtw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignLeft|Qt::AlignTop,qss).width();
                w = (int) rtw;
            } else {
                QString qss = QString().append(s,ll);
                rtw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignRight|Qt::AlignTop,qss).width();
                w = (int) rtw;
            }
            if (w > maxw) maxw = w;

            // calculate the horizontal justification
            p2 = p1;
            if (hjust == 1) {
                p2.x -= rtw*fstretch * cosa / 2.0;
                p2.y += rtw*fstretch * sina / 2.0;
            } else if (hjust == 2) {
                p2.x -= rtw*fstretch * cosa;
                p2.y += rtw*fstretch * sina;
            }

            // draw the halo box before (behind) the text with OpenGL
            if (drawmode != DM_INVERT && options.test(12) && halo == 0) {
                Point *hp,hp3;
                Point hp1[4];
                int nvert;

                hp3 = p3;

                if (hjust == 1) {
                    hp3.x -= maxw * fstretch * cosa / 2.0;
                    hp3.y += maxw * fstretch * sina / 2.0;
                } else if (hjust == 2) {
                    hp3.x -= maxw * fstretch * cosa;
                    hp3.y += maxw * fstretch * sina;
                }

                //if(t)
                //   p3 = t->transform(p3);

                hp3.x += -ih / 10.0 * cosa - ih/10.0 * sina;
                hp3.y += +ih / 10.0 * sina - ih/10.0 * cosa;

                v->setupGL(linestylescale,0.0,drawmode,halocolour,style,weight,0);
                RCCOLORREF pcol = v->getcolour();
                SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
                GLfloat lwidth = v->curPen.widthF();
                glLineWidth(v->curGLWeight);

                hp1[0] = Point(int(hp3.x),v->getheight() - 1.0 - int(hp3.y),0.0);
                hp1[1] = Point(int(hp3.x + (maxw*fstretch+ih/5.0) * cosa),v->getheight() - 1.0 - int((hp3.y - (maxw*fstretch+ih/5.0) * sina)),0.0);
                hp1[2] = Point(int(hp3.x + (maxw*fstretch+ih/5.0) * cosa + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((hp3.y - (maxw*fstretch+ih/5.0) * sina + (maxh+ih/5.0) * cosa)),0.0);
                hp1[3] = Point(int(hp3.x + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((hp3.y + (maxh+ih/5.0) * cosa)),0.0);

                hp1[0] = v->screentomodel(hp1[0]);
                hp1[1] = v->screentomodel(hp1[1]);
                hp1[2] = v->screentomodel(hp1[2]);
                hp1[3] = v->screentomodel(hp1[3]);

                nvert = 4;
                hp = hp1;
                //v->fill(0,&hp,&nvert,1,1);
                v->fillGL(&hp,&nvert,1,1);

                v->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);

                //p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
                //p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;

            }

            // set up for text
            v->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);
            RCCOLORREF pcol = v->getcolour();
            SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
            GLfloat lwidth = v->curPen.widthF();
            glLineWidth(v->curGLWeight);

            // draw the text
            if(options.test(10)) { // outline font
                if (drawmode != DM_INVERT) {
                    double x,y;

                    if (v->gettransform2d()) {
                        SIZE size;
                        x = p2.x;
                        y = p2.y;
                        size.cx = fm.width(QString(s,ll));
                        size.cy = fm.height();

                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;

                        if (p1d.x + size.cx * 2 > 0.0 && p1d.x - size.cx * 2 < v->getwidth() &&
                                p1d.y + size.cy * 2 > 0.0 && p1d.y - size.cy * 2 < v->getheight()) {
                            QString oString;
                            oString.append(s,ll);
                            double da = a * 180.0 / M_PI - angle1;
                            QTransform stf;
                            stf.translate(x,y);
                            stf.rotate(-da);
                            stf.scale(fstretch, 1.0);
                            stf.translate(0.0,tmAscent);
                            // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            DrawQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
                            //FillQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
#if 0
                            QPainterPath path;

                            //if(qgpath != 0)
                            {
                                // drawing the outline ourselves
                                QTransform stf;
                                QGraphicsItem *qpline = 0;
                                //QList<QGraphicsItem *> gplist;
                                QString oString;
                                oString.append(s,ll);
                                // add the text to a path
                                path.addText(0.0,0.0,qfont,oString);
                                double da = a * 180.0 / M_PI - angle1;
                                stf.translate(x,y);
                                stf.rotate(-da);
                                stf.scale(fstretch, 1.0);
                                stf.translate(0.0,tmAscent);
                                // get the outline polygon
                                QList<QPolygonF> textpolys = path.toSubpathPolygons(stf);
                                // draw the outine from the polygon vertices
                                for(int ip=0; ip<textpolys.size(); ip++) {
                                    QPolygonF thisPoly = textpolys.at(ip);
                                    // get a polygon
                                    for(int ie=0; ie<thisPoly.size()-1; ie++) {
                                        // get an edge
                                        QPointF qp1,qp2;
                                        qp1 = thisPoly.at(ie);
                                        qp2 = thisPoly.at(ie+1);
                                        qpline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(qp1.x(),v->getheight()-1-qp1.y(),0.0)))
                                                           ,v->uvtomodel(v->screentouv(Point(qp2.x(),v->getheight()-1-qp2.y(),0.0))));
                                        if(qpline)
                                            glist.append(qpline);
                                    }
                                }
                            }
#endif
                        }
                    } else {
                        SIZE size;
                        // KMJ: TODO: check metafile stuff
                        if (metafile) {
                            size.cx = fm.width(QString(s,ll));
                            size.cy = fm.height();
                            ;//GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
                        } else {
                            size.cx = fm.width(QString(s,ll));
                            size.cy = fm.height();
                        }

                        if (p2.x + size.cx > 0.0 && p2.x - size.cx < v->getwidth() &&
                                p2.y + size.cy > 0.0 && p2.y - size.cy < v->getheight()) {
                            QString oString;
                            oString.append(s,ll);
                            double da = a * 180.0 / M_PI - angle1;
                            QTransform stf;
                            stf.translate(p2.x,p2.y);
                            stf.rotate(-da);
                            stf.scale(fstretch, 1.0);
                            stf.translate(0.0,tmAscent);
                            // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            DrawQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
                            //FillQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
#if 0
                            QGraphicsPathItem *qgpath = new QGraphicsPathItem();
                            QPainterPath path;

                            if(qgpath != 0) {
                                // drawing the outline ourselves
                                QTransform stf;
                                QGraphicsItem *qpline = 0;
                                //QList<QGraphicsItem *> gplist;
                                QString oString;
                                oString.append(s,ll);
                                // add the text to a path
                                path.addText(0.0,0.0,qfont,oString);
                                double da = a * 180.0 / M_PI - angle1;
                                stf.translate(p2.x,p2.y);
                                stf.rotate(-da);
                                stf.scale(fstretch, 1.0);
                                stf.translate(0.0,tmAscent);
                                // get the outline polygon
                                QList<QPolygonF> textpolys = path.toSubpathPolygons(stf);
                                // draw the outine from the polygon vertices

                                glBegin(GL_LINES);
                                for(int ip=0; ip<textpolys.size(); ip++) {
                                    QPolygonF thisPoly = textpolys.at(ip);
                                    // get a polygon
                                    for(int ie=0; ie<thisPoly.size()-1; ie++) {
                                        // get an edge
                                        QPointF qp1,qp2;
                                        qp1 = thisPoly.at(ie);
                                        qp2 = thisPoly.at(ie+1);
                                        //qpline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(qp1.x(),v->getheight()-1-qp1.y(),0.0)))
                                        //                    ,v->uvtomodel(v->screentouv(Point(qp2.x(),v->getheight()-1-qp2.y(),0.0))));
                                        //if(qpline)
                                        //    glist.append(qpline);

                                        Point txp = v->screentomodel(Point(qp1.x(),v->getheight()-1-qp1.y(),0.0));
                                        glVertex3d(txp.x,txp.y,txp.z);
                                        txp = v->screentomodel(Point(qp2.x(),v->getheight()-1-qp2.y(),0.0));
                                        glVertex3d(txp.x,txp.y,txp.z);
                                    }
                                }
                                glEnd();
                            }
#endif
                        } else
                            p2.x += 1.0E-30;
                    }
                } else {
                    SIZE size;

                    size.cx = (long)fm.width(QString(s,ll));
                    size.cy = (long)fm.height();

                    //KMJ: TODO: check when transformed
                    if (v->gettransform2d()) {
                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;
                    } else
                        p1d = p2;

                    if (p1d.x + size.cx > 0.0 && p1d.x - size.cx < v->getwidth() &&
                            p1d.y + size.cy > 0.0 && p1d.y - size.cy < v->getheight()) {
                        QString oString;
                        oString.append(s,ll);
                        // add the text to a path
                        double da = a * 180.0 / M_PI - angle1;
                        QTransform stf;
                        stf.translate(p2.x,p2.y);
                        stf.rotate(-da);
                        stf.scale(fstretch, 1.0);
                        stf.translate(0.0,tmAscent);
                        // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                        // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                        DrawQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
                        //FillQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
#if 0
                        QGraphicsPathItem *qgpath = new QGraphicsPathItem();
                        QPainterPath path;

                        if(qgpath != 0) {
                            // drawing the outline ourselves
                            QTransform stf;
                            QGraphicsItem *qpline = 0;
                            //QList<QGraphicsItem *> gplist;
                            QString oString;
                            oString.append(s,ll);
                            // add the text to a path
                            path.addText(0.0,0.0,qfont,oString);
                            double da = a * 180.0 / M_PI - angle1;
                            stf.translate(p2.x,p2.y);
                            stf.rotate(-da);
                            stf.scale(fstretch, 1.0);
                            stf.translate(0.0,tmAscent);
                            // get the outline polygon
                            QList<QPolygonF> textpolys = path.toSubpathPolygons(stf);
                            // draw the outine from the polygon vertices
                            for(int ip=0; ip<textpolys.size(); ip++) {
                                QPolygonF thisPoly = textpolys.at(ip);
                                // get a polygon
                                for(int ie=0; ie<thisPoly.size()-1; ie++) {
                                    // get an edge
                                    QPointF qp1,qp2;
                                    qp1 = thisPoly.at(ie);
                                    qp2 = thisPoly.at(ie+1);
                                    qpline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(qp1.x(),v->getheight()-1-qp1.y(),0.0)))
                                                       ,v->uvtomodel(v->screentouv(Point(qp2.x(),v->getheight()-1-qp2.y(),0.0))));
                                    if(qpline)
                                        glist.append(qpline);
                                }
                            }
                        }
#endif
                    }
                }
            } else { // normal filled font
                if (drawmode != DM_INVERT) {
                    double x,y;

                    // what is this for??
                    //SetBkMode(v->gethdc(),TRANSPARENT);

                    // KMJ: TODO: check when 2D transform is used
                    if (v->gettransform2d()) {
                        SIZE size;
                        x = p2.x;
                        y = p2.y;
                        size.cx = fm.width(QString(s,ll));
                        size.cy = fm.height();

                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;

                        if (p1d.x + size.cx * 2 > 0.0 && p1d.x - size.cx * 2 < v->getwidth() &&
                                p1d.y + size.cy * 2 > 0.0 && p1d.y - size.cy * 2 < v->getheight()) {

                            // add the text to a path
                            double da = a * 180.0 / M_PI - angle1;
                            QTransform stf;
                            stf.translate(x,y);
                            stf.rotate(-angle1);
                            stf.scale(fstretch, 1.0);
                            stf.translate(0.0,tmAscent);
                            // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            //DrawQtTextGL(Point(0.0,0.0,0.0),qfont,QString(s),stf,v);
                            FillQtTextGL(Point(0.0,0.0,0.0),qfont,QString(s),stf,v);
#if 0
                            QGraphicsSimpleTextItem *stxt = v->getScene()->addSimpleText(QString(s));
                            stxt->setBrush(QBrush(v->curTextColour));
                            stxt->setFont(qfont);
                            stxt->setRotation(-angle1);
                            stxt->setPos(x,y);
                            QTransform stf;
                            stf.scale(fstretch, 1.0);
                            stxt->setTransform(stf,true);
                            if(stxt !=0)
                                glist.append(stxt);
#endif

                        }
                    } else {
                        SIZE size;
                        // KMJ: TODO: check metafile stuff
                        if (metafile) {
                            ;//GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
                        } else {
                            size.cx = fm.width(QString(s,ll));
                            size.cy = fm.height();
                        }

                        //SetTextAlign(v->gethdc(),TA_TOP | TA_LEFT);

                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;

                        if (p2.x + size.cx > 0.0 && p2.x - size.cx < v->getwidth() &&
                                p2.y + size.cy > 0.0 && p2.y - size.cy < v->getheight()) {
                            QString oString;
                            oString.append(s,ll);
                            // add the text to a path
                            double da = a * 180.0 / M_PI - angle1;
                            QTransform stf;
                            stf.translate(p2.x,p2.y);
                            stf.rotate(-da);
                            stf.scale(fstretch, 1.0);
                            stf.translate(0.0,tmAscent);
                            // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                            //DrawQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
                            if(oString.length() > 0)
                                FillQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
#if 0
                            QString oString = QString().append(s,ll);
                            QGraphicsSimpleTextItem *stxt = new QGraphicsSimpleTextItem(oString);
                            //qDebug() << "Text is  : " << stxt;
                            //qDebug() << "value is : " << stxt->text();
                            if(stxt != 0) {
                                stxt->setFont(qfont);
                                stxt->setBrush(QBrush(v->curTextColour));
                                //qDebug() << "text red   : " << db.colourtable[getcolour()].red;
                                //qDebug() << "text green : " << db.colourtable[getcolour()].green;
                                //qDebug() << "text blue  : " << db.colourtable[getcolour()].blue;
                                //stxt->setBrush(QBrush(QColor(db.colourtable[getcolour()].red,db.colourtable[getcolour()].green,db.colourtable[getcolour()].blue)));
                                //qDebug() << "text color : " << v->curTextColour;
                                double da = a * 180.0 / M_PI - angle1;
                                QTransform stf;
                                stf.translate(p2.x,p2.y);
                                stf.rotate(-da);
                                stf.scale(fstretch, 1.0);
                                stxt->setTransform(stf,true);
                                v->getScene()->addItem(stxt);
                                if(stxt !=0)
                                    glist.append(stxt);
                            }
#endif
                        } else
                            p2.x += 1.0E-30;
                    }
                    // what is this for??
                    //SetBkMode(v->gethdc(),OPAQUE);
                } else {
                    SIZE size;

                    size.cx = (long)fm.width(QString(s,ll));
                    size.cy = (long)fm.height();

                    //KMJ: TODO: check when transformed
                    if (v->gettransform2d()) {
                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;
                    } else
                        p1d = p2;

                    if (p1d.x + size.cx > 0.0 && p1d.x - size.cx < v->getwidth() &&
                            p1d.y + size.cy > 0.0 && p1d.y - size.cy < v->getheight()) {
                        QString oString;
                        oString.append(s,ll);
                        // add the text to a path
                        double da = a * 180.0 / M_PI - angle1;
                        QTransform stf;
                        stf.translate(p2.x,p2.y);
                        stf.rotate(-da);
                        stf.scale(fstretch, 1.0);
                        stf.translate(0.0,tmAscent);
                        // DrawQtTextGL(const Point &position, const QFont &qfname,const QString &text, const,const QTransform &matrix)
                        // FillQtTextGL(const Point &position,const QFont &qfname,const QString &text, const,const QTransform &matrix)
                        //DrawQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
                        FillQtTextGL(Point(0.0,0.0,0.0),qfont,oString,stf,v);
#if 0
                        QString oString;
                        oString.append(s,ll);
                        //qDebug() << "drawing text : " << QString(oString);
                        QGraphicsSimpleTextItem *stxt = new QGraphicsSimpleTextItem(oString);
                        if(stxt != 0) {
                            stxt->setFont(qfont);
                            stxt->setBrush(QBrush(v->curTextColour));
                            double da = a * 180.0 / M_PI - angle1;
                            QTransform stf;
                            stf.translate(p2.x,p2.y);
                            stf.rotate(-da);
                            stf.scale(fstretch,1.0);
                            stxt->setTransform(stf,true);
                            v->getScene()->addItem(stxt);
                            if(stxt !=0)
                                glist.append(stxt);
                        }
#endif
                    }
                }
            } // end if outline font

            p1.x += rh * (1.0 + vertspacing) * sina;
            p1.y += rh * (1.0 + vertspacing) * cosa;

            if (s[ll] == 0) break;
        }// end of line of text walking loop
#if 0
        if (hjust == 1) {
            p3.x -= maxw * fstretch * cosa / 2.0;
            p3.y += maxw * fstretch * sina / 2.0;
        } else if (hjust == 2) {
            p3.x -= maxw * fstretch * cosa;
            p3.y += maxw * fstretch * sina;
        }

        // draw the halo box
        if (drawmode != DM_INVERT && options.test(12) && halo == 0) {
            Point *hp;
            Point hp1[4];
            int nvert;

            //if(t)
            //   p3 = t->transform(p3);

            p3.x += -ih / 10.0 * cosa - ih/10.0 * sina;
            p3.y += +ih / 10.0 * sina - ih/10.0 * cosa;

            v->setupGL(linestylescale,0.0,drawmode,halocolour,style,weight,0);
            RCCOLORREF pcol = v->getcolour();
            SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
            GLfloat lwidth = v->curPen.widthF();
            glLineWidth(v->curGLWeight);

            hp1[0] = Point(int(p3.x),v->getheight() - 1.0 - int(p3.y),0.0);
            hp1[1] = Point(int(p3.x + (maxw*fstretch+ih/5.0) * cosa),v->getheight() - 1.0 - int((p3.y - (maxw*fstretch+ih/5.0) * sina)),0.0);
            hp1[2] = Point(int(p3.x + (maxw*fstretch+ih/5.0) * cosa + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((p3.y - (maxw*fstretch+ih/5.0) * sina + (maxh+ih/5.0) * cosa)),0.0);
            hp1[3] = Point(int(p3.x + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((p3.y + (maxh+ih/5.0) * cosa)),0.0);

            hp1[0] = v->screentomodel(hp1[0]);
            hp1[1] = v->screentomodel(hp1[1]);
            hp1[2] = v->screentomodel(hp1[2]);
            hp1[3] = v->screentomodel(hp1[3]);

            nvert = 4;
            hp = hp1;
            //v->fill(0,&hp,&nvert,1,1);
            v->fillGL(&hp,&nvert,1,1);

            v->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);

            p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
            p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;

        }
#endif
        // box drawn after the halo
        if (options.test(8)) {
            Point bp3;
            double x1,y1,x2,y2;

            v->setupGL(linestylescale,0.0,drawmode,colour,style,weight,0);
            RCCOLORREF pcol = v->getcolour();
            SetNoMaterial(GetRValue(pcol),GetGValue(pcol),GetBValue(pcol));
            GLfloat lwidth = v->curPen.widthF();
            glLineWidth(v->curGLWeight);

            bp3 = p3;

            if (hjust == 1) {
                bp3.x -= maxw * fstretch * cosa / 2.0;
                bp3.y += maxw * fstretch * sina / 2.0;
            } else if (hjust == 2) {
                bp3.x -= maxw * fstretch * cosa;
                bp3.y += maxw * fstretch * sina;
            }

            bp3.x += -ih / 10.0 * cosa - ih/10.0 * sina;
            bp3.y += +ih / 10.0 * sina - ih/10.0 * cosa;

            // top line
            x1 = bp3.x;
            y1 = bp3.y;
            x2 = bp3.x + (maxw*fstretch+ih/5.0) * cosa;
            y2 = bp3.y - (maxw*fstretch+ih/5.0) * sina;

            if(0)
                //if (v->gethwnd() != 0)
            {
                //change(int c,int l,int s,int w)
                Line line(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                line.EntityHeader::change(colour,0,style,weight);
                line.drawGL(drawmode,t,v);
                //v->line3dGL(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
            } else { //if (v->clip2d(&x1,&y1,&x2,&y2))
                //Line line(v->screentomodel(Point(x1,v->getheight() - 1.0- y1,0.0)),v->screentomodel(Point(x2,v->getheight() - 1.0 - y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(x1,v->getheight() - y1,0.0)),v->screentomodel(Point(x2,v->getheight() - y2,0.0)));
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
            }

            // right vertical line
            x1 = bp3.x + (maxw*fstretch+ih/5.0) * cosa;
            y1 = bp3.y - (maxw*fstretch+ih/5.0) * sina;
            x2 = bp3.x + (maxw*fstretch+ih/5.0) * cosa + (maxh+ih/5.0) * sina;
            y2 = bp3.y - (maxw*fstretch+ih/5.0) * sina + (maxh+ih/5.0) * cosa;
            if(0)
                //if (v->gethwnd() != 0)
            {
                Line line(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                line.EntityHeader::change(colour,0,style,weight);
                line.drawGL(drawmode,t,v);
                //v->line3dGL(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //qline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
            } else { //if (v->clip2d(&x1,&y1,&x2,&y2))
                //Line line(v->screentomodel(Point(x1,v->getheight() - 1.0 - y1,0.0)),v->screentomodel(Point(x2,v->getheight()- 1.0  - y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(x1,v->getheight() - y1,0.0)),v->screentomodel(Point(x2,v->getheight() - y2,0.0)));
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
            }

            // bottom line
            x1 = bp3.x + (maxw*fstretch+ih/5.0) * cosa + (maxh+ih/5.0) * sina;
            y1 = bp3.y - (maxw*fstretch+ih/5.0) * sina + (maxh+ih/5.0) * cosa;
            x2 = bp3.x + (maxh+ih/5.0) * sina;
            y2 = bp3.y + (maxh+ih/5.0) * cosa;
            if(0)
                //if (v->gethwnd() != 0)
            {
                //Line line(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //qline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
            } else { //if (v->clip2d(&x1,&y1,&x2,&y2))
                //Line line(v->screentomodel(Point(x1,v->getheight() - 1.0 - y1,0.0)),v->screentomodel(Point(x2,v->getheight() - 1.0 - y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(x1,v->getheight() - y1,0.0)),v->screentomodel(Point(x2,v->getheight() - y2,0.0)));
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
            }

            // left vertical line
            x1 = bp3.x + (maxh+ih/5.0) * sina;
            y1 = bp3.y + (maxh+ih/5.0) * cosa;
            x2 = bp3.x;
            y2 = bp3.y;
            if(0)
                //if (v->gethwnd() != 0)
            {
                //Line line(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(x1,y1,0.0)),v->screentomodel(Point(x2,y2,0.0)));
                //qline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
            } else { //if (v->clip2d(&x1,&y1,&x2,&y2))
                //Line line(v->screentomodel(Point(x1,v->getheight() - 1.0  - y1,0.0)),v->screentomodel(Point(x2,v->getheight() - 1.0  - y2,0.0)));
                //line.EntityHeader::change(colour,0,style,weight);
                //line.drawGL(drawmode,t,v);
                v->line3dGL(v->screentomodel(Point(x1,v->getheight() - y1,0.0)),v->screentomodel(Point(x2,v->getheight() - y2,0.0)));
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
            }

            //p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
            //p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;

        }

        // KMJ: TODO: check the metafile stuff
        /*
        if (hfont != NULL)
         {  if (metafile)
               SelectObject(cadwindow->gethdc(),cwoldhfont);
             else if (drawmode == DM_INVERT)
               SelectObject(cadwindow->getoffscreenhdc(),cwoldhfont);
             oldhfont = (RCHFONT) SelectObject(v->gethdc(),oldhfont);
             DeleteObject(hfont);
             DeleteObject(hreferencefont);
          }
         */
        // KMJ: TODO: check the priniter stuff
        /*
        if (v->getprinterhdc() != 0)
        {
            SelectObject(v->getprinterhdc(),printeroldhfont);
            DeleteObject(printerhfont);
        }
        */
        //} // end old if outline font
        //delete [] dx;
    }// end halo loop

#endif
}
#endif

void Text::draw(int drawmode,Transform *t,View3dSurface *v)
{
#ifdef USING_WIDGETS
#if 1
    Point p1,p2,p3,p4,p1d;
    RCHFONT cwoldhfont,oldhfont,hfont,printerhfont,printeroldhfont,hreferencefont;
    //BYTE fbCharSet;
    int i,j,tlen,halo,i1,i2,status,metafile,ih,iw,ll,nlines,maxh,maxw,*dx,hjust,vjust;
    double scale,rh,w,rw,rtw,y,oheight,a,l,cosa,sina,xmaxh,xmaxw,xw,tmHeight,tmDescent,tmAscent,ppscale;
    //#ifdef _USING_QTCHAR
    // TEXTMETRICW tm;
    //#else
    // TEXTMETRIC tm;
    //#endif
    Point zero(0.0,0.0,0.0),o,xa,ya;
    char buffer[GGO_BUF_SIZE];
    RCCHAR *s;
    Transform tr;
    SIZE size;

    //QGraphicsItem *qgline=0,*qgtext=0,*qgrect=0;
    QList<QGraphicsItem *> glist;
    QGraphicsPathItem *qgpath = 0;

    if(drawmode != DM_PRINT) {
        if(qgi && qgi != qgi->parentItem() )
            qgi->setParentItem(0);

        if(qgi && qgi->scene() != v->getScene())
            v->getScene()->removeItem(qgi);
        //else if(qgi && v->getScene())
        //    v->getScene()->removeItem(qgi);
        delete qgi;
        qgi=0;
        qgbox=0;
        qgfill=0;

        //if(dynamic_cast<View3dSubSurface*>(v) != 0)
        //    ((View3dSubSurface*)v)->qgicp=0;

    }

    if(drawmode == DM_ERASE)
        return;

    //if(cadwindow->getcurrentsurface()->getrepaintstyle() != Wireframe)
    //    return;

    if (getselected() && drawmode == DM_NORMAL)
        drawmode = DM_SELECT;
    v->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

    if (0 && t == NULL) {
        if (v->displaylist.draw(this,drawmode)) {
            return;
        }
        if (! v->displaylist.geterrorstatus())
            addtodisplaylist(v,0);
        if (v->displaylist.draw(this,drawmode)) {
            return;
        }
    }

    // transform the origin point
    // get the rotations
    // get the font metrics
    // get the bounding box H and W
    // draw the halo
    // draw the text
    // draw the bounding box

    Point xa1,ya1,o1;
    double angle1;

    //int dxlen = RCSTRLEN(text)+1;
    //dx = new int[RCSTRLEN(text)+1];

    //halo = 0 ;
    for (halo = 0 ; halo < 1 + options.test(12) ; halo++) {
        if (t == NULL) {
            p1 = v->modeltoview(origin);
            p2 = v->modeltoview(origin);
        } else {
            p1 = v->modeltoview(t->transform(origin));
            p2 = v->modeltoview(t->transform(origin));
        }
        if (! v->clip3dfb(&p1,&p2))
            return;

        if (t == NULL)
            p1 = v->modeltoscreen(origin);
        else
            p1 = v->modeltoscreen(t->transform(origin));

        p1d = p1;
        v->transform2d(&p1.x,&p1.y);

        //if (options.test(0))
        //  fbCharSet = SYMBOL_CHARSET;
        //else
        //  fbCharSet = ANSI_CHARSET;//DEFAULT_CHARSET;//

        xa1 = v->modeltoscreen(t == 0 ? origin + xaxis : t->transform(origin + xaxis));
        v->transform2d(&xa1.x,&xa1.y);

        ya1 = v->modeltoscreen(t == 0 ? origin + yaxis : t->transform(origin + yaxis));
        v->transform2d(&ya1.x,&ya1.y);

        o1 = v->modeltoscreen(t == 0 ? origin : t->transform(origin));
        v->transform2d(&o1.x,&o1.y);

        xa = xa1 - o1;
        xa.z = 0.0;
        if ((l = xa.length()) < 1.0E-10) {
            //delete [] dx;
            return;
        }
        xa /= l;
        ya = ya1 - o1;
        ya.z = 0.0;
        if ((l = ya.length()) < 1.0E-10) {
            //delete [] dx;
            return;
        }
        ya /= l;

        hjust = options.test(1) + options.test(2) * 2;
        vjust = options.test(3) + options.test(4) * 2;

        //  Compare the transformed z axis to the zaxis calculated from the transformed x and y axis to see if the sense has changed
        Point origin1,xaxis1,yaxis1,zaxis1,zaxis2;
        origin1 = v->modeltoscreen(t == 0 ? origin : t->transform(origin));
        xaxis1 = v->modeltoscreen(t == 0 ? origin + xaxis : t->transform(origin + xaxis)) - origin1;
        yaxis1 = v->modeltoscreen(t == 0 ? origin + yaxis : t->transform(origin + yaxis)) - origin1;
        zaxis1 = v->modeltoscreen(t == 0 ? origin + xaxis.cross(yaxis) : t->transform(origin + xaxis.cross(yaxis))) - origin1;
        zaxis2 = xaxis1.cross(yaxis1);

        angle1 = -angle;

        // KMJ : TODO: check this
        if (zaxis1.dot(zaxis2) > 0.0) {
            //  If the text has been mirrored, need to adjust the justifications
            sina = sin(angle1 / 180.0 * M_PI);
            cosa = cos(angle1 / 180.0 * M_PI);
            if ((-xaxis * sina + yaxis * cosa).dot(-xaxis1 * sina + yaxis1 * cosa) <= 0) {
                ya = -ya;
                vjust = 2 - vjust;
                angle1 = -angle1;
            } else if ((xaxis * cosa + yaxis * sina).dot(xaxis1 * cosa + yaxis1 * sina) < 0.0) {
                xa = -xa;
                hjust = 2 - hjust;
                angle1 = -angle1;
            }
        }


        if (xa.x < -1.0) xa.x = -1.0;
        else if (xa.x > 1.0) xa.x = 1.0;
        a = acos(xa.x);
        if (xa.y > 0.0) a = 2.0 * M_PI - a;
        ya = xa.cross(ya).cross(xa).normalize();

        qDebug() << "plotscale : " << v->getplotscale();

        if (options.test(9))
            scale = v->getplotscale();
        else
            scale = 1.0;

        // rh is the height between two line of text this is the font height plus the font descent height
        // need to find the correct font height that is required for the given height in points for Qt
        // KMJ : Height of a font must be in points for Qt
        rh = height * v->getheight() / (v->getvmax() - v->getvmin()) * scale;
        if (options.test(13)) // height is in points option
            rh *= 25.4 / 72.0;//rh = height * v->getheight() / (v->getvmax() - v->getvmin()) * scale;// *= 0.352777778

        rw = width * v->getwidth() / (v->getumax() - v->getumin()) * scale;
        if (rh > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT;
        else if (rh < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT;
        else ih = int(rh);
        if (rw > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT;
        else if (rw < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT;
        else iw = int(rw);
        if (ih == 0) {
            //delete [] dx;
            return;
        }

        // calculate the Qt font pixel height required for the text height setting
        // calculate the point height for the font
        QString refName(font);
        QFont refFont(refName);
        //refFont.setHintingPreference(QFont::PreferNoHinting);
        refFont.setHintingPreference(QFont::PreferDefaultHinting);
        //refFont.setLetterSpacing(QFont::PercentageSpacing, 100.0);
        //refFont.setFixedPitch(true);
        //refFont.setKerning(true);
        //refFont.setPointSize((int)rh);// use rh as points
        refFont.setPointSizeF(rh);// use rh as points
        QFontMetricsF refFm(refFont);
        //int refHeight = refFm.height();// get the height as pixels for rh points
        double refHeight = refFm.height();// get the height as pixels for rh points
        // how many points did we get for for 1 pixel of rh
        double ptsperpx = rh / refHeight;
        // how many points to get the same height in pixels
        //int fontHeight = (int)(rh * ptsperpx);
        double fontHeight = (rh * ptsperpx);

        // KMJ: Qt fonts must be greater than zero height
        // checking here for zero font height
        //qDebug() << "fontHeight :" << fontHeight;
        if(fontHeight < 1.0)
            return;

        // set up a device independent font
        QString qfname(font);
        QFont qfont(qfname);
        qfont.setHintingPreference(QFont::PreferDefaultHinting);
        //qfont.setHintingPreference(QFont::PreferNoHinting);
        //qfont.setLetterSpacing(QFont::PercentageSpacing, 100.0);
        //qfont.setFixedPitch(true);
        //qfont.setKerning(true);
        //qfont.setPointSize(fontHeight);
        qfont.setPointSizeF(fontHeight);
        // set the font weight
        if(options.test(5))
            qfont.setWeight(QFont::Bold);
        else
            qfont.setWeight(QFont::Normal);
        // set italic
        qfont.setItalic(options.test(7));
        // set underline
        qfont.setUnderline(options.test(6));

        // get the font metrics
        QFontMetricsF fm(qfont);

        // calculate the stretch factor
        // this is done by scaling the font in width
        //double fstretch = width  / (height > 0.0 ? height : 1.0);
        //fstretch = fstretch == 0.0 ? 1.0 : fstretch;
        double fstretch = 1.0;
        if(width == 0.0)
            fstretch = 1.0;
        else
            fstretch = fabs(width);

        sina = sin(a - angle1 / 180.0 * M_PI);
        cosa = cos(a - angle1 / 180.0 * M_PI);

        metafile = 0;
        //metafile = GetDeviceCaps(v->gethdc(),TECHNOLOGY) == DT_METAFILE;

        tmHeight = fm.height();
        tmDescent = fm.descent();
        tmAscent = fm.ascent();

        // calculate the number of lines
        //for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += (*s == 0x000D || *s == 0x000A);
        for (s = text,nlines = 1 ; *s!=0 ; s++)
        {
            nlines += (*s == 0x000D || *s == 0x000A) ;
            if(*s == 0x000D && (*s+1 != 0) && *(s+1) == 0x000A)
                s++;
        }

        //KMJ: TODO: do the printer stuff
        if (0 && v->getprinterhdc() != 0) {
            if (options.test(9))  //  In paper space, use the actual text size if scaled.
                scale = 1.0;
            else
                scale = 1.0 / v->getplotscale();
            if (v->gettransform2d())
                ppscale = v->getheight() / (v->getvmax() - v->getvmin()) * v->getplotscale() *
                          cadwindow->printpreviewwindow->getprintscale() / (double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) / 25.4);
            else
                ppscale = v->getheight() / (v->getvmax() - v->getvmin()) *
                          cadwindow->printpreviewwindow->getprintscale() / (double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) / 25.4);

            rh /= ppscale;
            rw /= ppscale;
            //               rh = height * scale / cadwindow->printpreviewwindow->getprintscale() * double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) / 25.4;
            //               rw = width * scale / cadwindow->printpreviewwindow->getprintscale() * double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSY)) / 25.4;
            if (rh > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT;
            else if (rh < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT;
            else ih = int(rh);
            if (rw > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT;
            else if (rw < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT;
            else iw = int(rw);
            if (ih == 0) {
                if (hfont != 0) {
                    if (metafile)
                        SelectObject(cadwindow->gethdc(),cwoldhfont);
                    else if (drawmode == DM_INVERT)
                        SelectObject(cadwindow->getoffscreenhdc(),cwoldhfont);
                    SelectObject(v->gethdc(),oldhfont);
                    DeleteObject(hfont);
                }
                return;
            }
            //lf.lfHeight = options.test(13) ? -ih : ih;
            //lf.lfWidth = iw;
            //printerhfont = CreateFontIndirectW(&lf);

            //printeroldhfont = (RCHFONT) SelectObject(v->getprinterhdc(),printerhfont);
            //GetTextMetricsW(v->getprinterhdc(),&tm);
            //tmHeight = tm.tmHeight * ppscale;
            //tmDescent = tm.tmDescent * ppscale;
        }

        // calculate the vertical justification and the line spacing
        if (vjust == 0) // bottom
            oheight = -nlines * tmHeight + tmDescent - (nlines - 1) * tmHeight * vertspacing;
        else if (vjust == 1) // center
            oheight = -(nlines + (nlines - 1) * vertspacing) * tmHeight / 2;
        else
            oheight = 0.0; // top

        //p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
        //p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;
        p1.x += oheight * sina;
        p1.y += oheight * cosa;
        p3 = p1;

        maxh = int(rh * (nlines + (nlines - 1)*vertspacing));
        maxw = 0;

        // walk each line of text and display it line by line
        ll=0;
        for (s = text ; *s!=0 ;  s += ll + 1) { // jump at the end of each line
            // text on windows have CR+LF line endings MAC OS X has LF line endings
            // find the line feed or carriage return or line feed and carriage return
            // 0x000A = LINE FEED,  0x000D = CARRIAGE RETURN
            //for (ll = 0 ;  (s[ll] != 0x000D) && (s[ll] != 0x000A) && (s[ll] != 0) ; ll++) ;
            for (ll = 0 ; s[ll] != 0; ll++)
            {
                if(s[ll] == 0x000D || s[ll] == 0x000A) {
                    if(s[ll] == 0x000D && s[ll+1] != 0 && s[ll+1] == 0x000A)
                        ll++;
                    break;
                }
            }

            // KMJ: TODO: do metafile stuff
            // get the width
            if (metafile) {
                w = fm.width(QString(s,ll));
            } else if (drawmode == DM_INVERT) {
                QString qss = QString().append(s,ll);
                rtw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignRight|Qt::AlignTop,qss).width();
                w = (int) rtw;
            } else {
                QString qss = QString().append(s,ll);
                rtw = fm.boundingRect(QRect(0,0,0,0),Qt::AlignRight|Qt::AlignTop,qss).width();
                w = (int) rtw;
            }
            if (w > maxw) maxw = w;

            // calculate the horizontal justification
            p2 = p1;
            if (hjust == 1) {
                p2.x -= rtw*fstretch * cosa / 2.0;
                p2.y += rtw*fstretch * sina / 2.0;
            } else if (hjust == 2) {
                p2.x -= rtw*fstretch * cosa;
                p2.y += rtw*fstretch * sina;
            }

            // draw the text
            if(options.test(10)) { // outline font
                if (drawmode != DM_INVERT) {
                    double x,y;

                    if (v->gettransform2d()) {
                        SIZE size;
                        x = p2.x;
                        y = p2.y;
                        size.cx = fm.width(QString(s,ll));
                        size.cy = fm.height();

                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;

                        if (p1d.x + size.cx * 2 > 0.0 && p1d.x - size.cx * 2 < v->getwidth() &&
                                p1d.y + size.cy * 2 > 0.0 && p1d.y - size.cy * 2 < v->getheight()) {
                            qgpath = new QGraphicsPathItem();
                            QPainterPath path;

                            if(qgpath != 0)
                            {
                                // drawing the outline ourselves
                                QTransform stf;
                                QGraphicsItem *qpline = 0;
                                //QList<QGraphicsItem *> gplist;
                                QString oString;
                                oString.append(s,ll);
                                // add the text to a path
                                path.addText(0.0,0.0,qfont,oString);
                                qgpath->setPath(path);
                                qgpath->setPen(v->curPen);
                                double da = a * 180.0 / M_PI - angle1;
                                stf.translate(x,y);
                                stf.rotate(-da);
                                stf.scale(fstretch, 1.0);
                                stf.translate(0.0,tmAscent);
                                // draw the outine from the polygon vertices
                                qgpath->setTransform(stf);
                                v->getScene()->addItem(qgpath);
                                // get the outline polygon
                                /*
                                QList<QPolygonF> textpolys = path.toSubpathPolygons(stf);
                                for(int ip=0; ip<textpolys.size(); ip++) {
                                    QPolygonF thisPoly = textpolys.at(ip);
                                    // get a polygon
                                    for(int ie=0; ie<thisPoly.size()-1; ie++) {
                                        // get an edge
                                        QPointF qp1,qp2;
                                        qp1 = thisPoly.at(ie);
                                        qp2 = thisPoly.at(ie+1);
                                        //qpline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(qp1.x(),v->getheight()-1-qp1.y(),0.0)))
                                        //                  ,v->uvtomodel(v->screentouv(Point(qp2.x(),v->getheight()-1-qp2.y(),0.0))));
                                        qpline = v->line3d((QGraphicsItem*)0,Point(qp1.x(),v->getheight()-1-qp1.y(),0.0),Point(qp2.x(),v->getheight()-1-qp2.y(),0.0));
                                        if(qpline)
                                            glist.append(qpline);
                                    }
                                }
                                */
                            }
                        }
                    } else {
                        SIZE size;
                        // KMJ: TODO: check metafile stuff
                        if (metafile) {
                            size.cx = fm.width(QString(s,ll));
                            size.cy = fm.height();
                            ;//GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
                        } else {
                            size.cx = fm.width(QString(s,ll));
                            size.cy = fm.height();
                        }

                        if (p2.x + size.cx > 0.0 && p2.x - size.cx < v->getwidth() &&
                                p2.y + size.cy > 0.0 && p2.y - size.cy < v->getheight()) {
                            qgpath = new QGraphicsPathItem();
                            QPainterPath path;

                            if(qgpath != 0) {
                                // drawing the outline ourselves
                                QTransform stf;
                                QGraphicsItem *qpline = 0;
                                //QList<QGraphicsItem *> gplist;
                                QString oString;
                                oString.append(s,ll);
                                // add the text to a path
                                path.addText(0.0,0.0,qfont,oString);
                                qgpath->setPath(path);
                                qgpath->setPen(v->curPen);
                                double da = a * 180.0 / M_PI - angle1;
                                stf.translate(p2.x,p2.y);
                                stf.rotate(-da);
                                stf.scale(fstretch, 1.0);
                                stf.translate(0.0,tmAscent);
                                // draw the outine from the polygon vertices
                                qgpath->setTransform(stf);
                                v->getScene()->addItem(qgpath);
                                /*
                                // get the outline polygon
                                QList<QPolygonF> textpolys = path.toSubpathPolygons(stf);
                                // draw the outine from the polygon vertices
                                for(int ip=0; ip<textpolys.size(); ip++) {
                                    QPolygonF thisPoly = textpolys.at(ip);
                                    // get a polygon
                                    for(int ie=0; ie<thisPoly.size()-1; ie++) {
                                        // get an edge
                                        QPointF qp1,qp2;
                                        qp1 = thisPoly.at(ie);
                                        qp2 = thisPoly.at(ie+1);
                                        qpline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(qp1.x(),v->getheight()-1-qp1.y(),0.0)))
                                                           ,v->uvtomodel(v->screentouv(Point(qp2.x(),v->getheight()-1-qp2.y(),0.0))));
                                        if(qpline)
                                            glist.append(qpline);
                                    }
                                }
                                */
                            }
                        } else
                            p2.x += 1.0E-30;
                    }
                } else {
                    SIZE size;

                    size.cx = (long)fm.width(QString(s,ll));
                    size.cy = (long)fm.height();

                    //KMJ: TODO: check when transformed
                    if (v->gettransform2d()) {
                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;
                    } else
                        p1d = p2;

                    if (p1d.x + size.cx > 0.0 && p1d.x - size.cx < v->getwidth() &&
                            p1d.y + size.cy > 0.0 && p1d.y - size.cy < v->getheight()) {
                        //if (size.cx > size.cy)
                        // size.cy = size.cx;
                        //else
                        //  size.cx = size.cy;

                        qgpath = new QGraphicsPathItem();
                        QPainterPath path;

                        if(qgpath != 0) {
                            // drawing the outline ourselves
                            QTransform stf;
                            QGraphicsItem *qpline = 0;
                            //QList<QGraphicsItem *> gplist;
                            QString oString;
                            oString.append(s,ll);
                            // add the text to a path
                            path.addText(0.0,0.0,qfont,oString);
                            qgpath->setPath(path);
                            qgpath->setPen(v->curPen);
                            double da = a * 180.0 / M_PI - angle1;
                            stf.translate(p2.x,p2.y);
                            stf.rotate(-da);
                            stf.scale(fstretch, 1.0);
                            stf.translate(0.0,tmAscent);
                            // draw the outine from the polygon vertices
                            qgpath->setTransform(stf);
                            v->getScene()->addItem(qgpath);
                            /*
                            // get the outline polygon
                            QList<QPolygonF> textpolys = path.toSubpathPolygons(stf);
                            // draw the outine from the polygon vertices
                            for(int ip=0; ip<textpolys.size(); ip++) {
                                QPolygonF thisPoly = textpolys.at(ip);
                                // get a polygon
                                for(int ie=0; ie<thisPoly.size()-1; ie++) {
                                    // get an edge
                                    QPointF qp1,qp2;
                                    qp1 = thisPoly.at(ie);
                                    qp2 = thisPoly.at(ie+1);
                                    qpline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(qp1.x(),v->getheight()-1-qp1.y(),0.0)))
                                                       ,v->uvtomodel(v->screentouv(Point(qp2.x(),v->getheight()-1-qp2.y(),0.0))));
                                    if(qpline)
                                        glist.append(qpline);
                                }
                            }
                            */
                        }
                    }
                }
            } else { // normal filled font
                if (drawmode != DM_INVERT) {
                    double x,y;

                    // what is this for??
                    //SetBkMode(v->gethdc(),TRANSPARENT);

                    // KMJ: TODO: check when 2D transform is used
                    if (v->gettransform2d()) {
                        SIZE size;
                        x = p2.x;
                        y = p2.y;
                        size.cx = fm.width(QString(s,ll));
                        size.cy = fm.height();

                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;

                        if (p1d.x + size.cx * 2 > 0.0 && p1d.x - size.cx * 2 < v->getwidth() &&
                                p1d.y + size.cy * 2 > 0.0 && p1d.y - size.cy * 2 < v->getheight()) {
                            QString oString = QString().append(s,ll);
                            QGraphicsSimpleTextItem *stxt = v->getScene()->addSimpleText(oString);
                            stxt->setFont(qfont);
                            stxt->setBrush(QBrush(v->curTextColour));
                            double da = a * 180.0 / M_PI - angle1;
                            QTransform stf;
//                                  stxt->setRotation(-angle1);
//                                  stxt->setPos(x,y);
                            stf.translate(x,y);
                            stf.rotate(-da);
                            stf.scale(fstretch, 1.0);
                            stxt->setTransform(stf,true);
                            if(stxt !=0)
                                glist.append(stxt);
                        }
                    } else {
                        SIZE size;
                        // KMJ: TODO: check metafile stuff
                        if (metafile) {
                            ;//GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
                        } else {
                            size.cx = fm.width(QString(s,ll));
                            size.cy = fm.height();
                        }

                        //SetTextAlign(v->gethdc(),TA_TOP | TA_LEFT);

                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;

                        if (p2.x + size.cx > 0.0 && p2.x - size.cx < v->getwidth() &&
                                p2.y + size.cy > 0.0 && p2.y - size.cy < v->getheight()) {

                            //QString oString;
                            //oString.append(s,ll);
                            QString oString = QString().append(s,ll);
                            QGraphicsSimpleTextItem *stxt = new QGraphicsSimpleTextItem(oString);
                            //qDebug() << "Text is  : " << stxt;
                            //qDebug() << "value is : " << stxt->text();
                            if(stxt != 0) {
                                stxt->setFont(qfont);
                                stxt->setBrush(QBrush(v->curTextColour));
                                //qDebug() << "text red   : " << db.colourtable[getcolour()].red;
                                //qDebug() << "text green : " << db.colourtable[getcolour()].green;
                                //qDebug() << "text blue  : " << db.colourtable[getcolour()].blue;
                                //stxt->setBrush(QBrush(QColor(db.colourtable[getcolour()].red,db.colourtable[getcolour()].green,db.colourtable[getcolour()].blue)));
                                //qDebug() << "text color : " << v->curTextColour;
                                double da = a * 180.0 / M_PI - angle1;
                                QTransform stf;
                                stf.translate(p2.x,p2.y);
                                stf.rotate(-da);
                                stf.scale(fstretch, 1.0);
                                stxt->setTransform(stf,true);
                                v->getScene()->addItem(stxt);
                                if(stxt !=0)
                                    glist.append(stxt);
                            }
                        } else
                            p2.x += 1.0E-30;
                    }
                    // what is this for??
                    //SetBkMode(v->gethdc(),OPAQUE);
                } else {
                    SIZE size;

                    size.cx = (long)fm.width(QString(s,ll));
                    size.cy = (long)fm.height();

                    //KMJ: TODO: check when transformed
                    if (v->gettransform2d()) {
                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;
                    } else
                        p1d = p2;

                    if (p1d.x + size.cx > 0.0 && p1d.x - size.cx < v->getwidth() &&
                            p1d.y + size.cy > 0.0 && p1d.y - size.cy < v->getheight()) {
                        //if (size.cx > size.cy)
                        //  size.cy = size.cx;
                        //else
                        //  size.cx = size.cy;

                        QString oString;
                        oString.append(s,ll);
                        //qDebug() << "drawing text : " << QString(oString);
                        QGraphicsSimpleTextItem *stxt = new QGraphicsSimpleTextItem(oString);
                        if(stxt != 0) {
                            stxt->setFont(qfont);
                            stxt->setBrush(QBrush(v->curTextColour));
                            double da = a * 180.0 / M_PI - angle1;
                            QTransform stf;
                            stf.translate(p2.x,p2.y);
                            stf.rotate(-da);
                            stf.scale(fstretch,1.0);
                            stxt->setTransform(stf,true);
                            v->getScene()->addItem(stxt);
                            if(stxt !=0)
                                glist.append(stxt);
                        }
                    }
                }
            } // end if outline font

            p1.x += rh * (1.0 + vertspacing) * sina;
            p1.y += rh * (1.0 + vertspacing) * cosa;

            if (s[ll] == 0) break;
        }// end of line of text walking loop

        if (hjust == 1) {
            p3.x -= maxw * fstretch * cosa / 2.0;
            p3.y += maxw * fstretch * sina / 2.0;
        } else if (hjust == 2) {
            p3.x -= maxw * fstretch * cosa;
            p3.y += maxw * fstretch * sina;
        }

        // draw the halo box
        if (drawmode != DM_INVERT && options.test(12) && halo == 0) {
            Point *hp[1];
            Point hp1[4];
            int nvert;

            if(drawmode != DM_PRINT) {
                if(qgfill && qgfill->scene() != v->getScene())
                    qgfill->scene()->removeItem(qgfill);
                //v->getScene()->removeItem(qgfill);
                delete qgfill;
                qgfill = 0;
            }

            //if(t)
            //   p3 = t->transform(p3);

            p3.x += -ih / 10.0 * cosa - ih/10.0 * sina;
            p3.y += +ih / 10.0 * sina - ih/10.0 * cosa;

            v->setup(linestylescale,0.0,drawmode,halocolour,style,weight,0);

            hp1[0] = Point(int(p3.x),v->getheight() - 1.0 - int(p3.y),0.0);
            hp1[1] = Point(int(p3.x + (maxw*fstretch+ih/5.0) * cosa),v->getheight() - 1.0 - int((p3.y - (maxw*fstretch+ih/5.0) * sina)),0.0);
            hp1[2] = Point(int(p3.x + (maxw*fstretch+ih/5.0) * cosa + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((p3.y - (maxw*fstretch+ih/5.0) * sina + (maxh+ih/5.0) * cosa)),0.0);
            hp1[3] = Point(int(p3.x + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((p3.y + (maxh+ih/5.0) * cosa)),0.0);

            nvert = 4;
            hp[0] = hp1;
            qgfill = v->fill((QGraphicsItem*)0,hp,&nvert,1,1);

            v->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

            p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
            p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;

        }

        // box drawn after the halo
        if (options.test(8)) {
            QGraphicsItem *qline = 0;
            QList<QGraphicsItem *> glist;

            double x1,y1,x2,y2;

            if(drawmode != DM_PRINT) {
                if(qgbox && qgbox->scene() != v->getScene())
                    qgbox->scene()->removeItem(qgbox);
                //v->getScene()->removeItem(qgbox);
                delete qgbox;
                qgbox = 0;
            }

            //if(t)
            //   p3 = t->transform(p3);

            p3.x += -ih / 10.0 * cosa - ih/10.0 * sina;
            p3.y += +ih / 10.0 * sina - ih/10.0 * cosa;

            // top line
            x1 = p3.x;
            y1 = p3.y;
            x2 = p3.x + (maxw*fstretch+ih/5.0) * cosa;
            y2 = p3.y - (maxw*fstretch+ih/5.0) * sina;


            if(0)
                //if (v->gethwnd() != 0)
            {
                qline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                if( qline != 0 ) glist.append( qline );
            } else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
                qline = v->line(int(x1),int(y1),int(x2),int(y2),1);
                if( qline != 0 ) glist.append( qline );
            }

            // right vertical line
            x1 = p3.x + (maxw*fstretch+ih/5.0) * cosa;
            y1 = p3.y - (maxw*fstretch+ih/5.0) * sina;
            x2 = p3.x + (maxw*fstretch+ih/5.0) * cosa + (maxh+ih/5.0) * sina;
            y2 = p3.y - (maxw*fstretch+ih/5.0) * sina + (maxh+ih/5.0) * cosa;
            if(0)
                //if (v->gethwnd() != 0)
            {
                qline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                if( qline != 0 ) glist.append( qline );
            } else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
                qline = v->line(int(x1),int(y1),int(x2),int(y2),1);
                if( qline != 0 ) glist.append( qline );
            }

            // bottom line
            x1 = p3.x + (maxw*fstretch+ih/5.0) * cosa + (maxh+ih/5.0) * sina;
            y1 = p3.y - (maxw*fstretch+ih/5.0) * sina + (maxh+ih/5.0) * cosa;
            x2 = p3.x + (maxh+ih/5.0) * sina;
            y2 = p3.y + (maxh+ih/5.0) * cosa;
            if(0)
                //if (v->gethwnd() != 0)
            {
                qline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                if( qline != 0 ) glist.append( qline );
            } else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
                qline = v->line(int(x1),int(y1),int(x2),int(y2),1);
                if( qline != 0 ) glist.append( qline );
            }

            // left vertical line
            x1 = p3.x + (maxh+ih/5.0) * sina;
            y1 = p3.y + (maxh+ih/5.0) * cosa;
            x2 = p3.x;
            y2 = p3.y;
            if(0)
                //if (v->gethwnd() != 0)
            {
                qline = v->line3d((QGraphicsItem*)0,v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                if( qline != 0 ) glist.append( qline );
            } else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                //qline = v->line(int(x1),v->getheight() - 1.0 -int(y1),int(x2),v->getheight() - 1.0 -int(y2));
                qline = v->line(int(x1),int(y1),int(x2),int(y2),1);
                if( qline != 0 ) glist.append( qline );
            }

            p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
            p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;

            if(glist.size() > 0)
                qgbox = v->getScene()->createItemGroup(glist);
            glist.clear();

        }

        // KMJ: TODO: check the metafile stuff
        /*
        if (hfont != NULL)
         {  if (metafile)
               SelectObject(cadwindow->gethdc(),cwoldhfont);
             else if (drawmode == DM_INVERT)
               SelectObject(cadwindow->getoffscreenhdc(),cwoldhfont);
             oldhfont = (RCHFONT) SelectObject(v->gethdc(),oldhfont);
             DeleteObject(hfont);
             DeleteObject(hreferencefont);
          }
         */
        // KMJ: TODO: check the priniter stuff
        /*
        if (v->getprinterhdc() != 0)
        {
            SelectObject(v->getprinterhdc(),printeroldhfont);
            DeleteObject(printerhfont);
        }
        */
        //} // end old if outline font
        //delete [] dx;
    }// end halo loop

    if(qgfill) {
        QGraphicsItem *qgtext=0;
        QGraphicsItemGroup *qgroup = new QGraphicsItemGroup();
        if(qgroup) {
            qgroup->addToGroup(qgfill);
            if(qgbox) qgroup->addToGroup(qgbox);
            if(glist.size() > 0) qgtext = v->getScene()->createItemGroup(glist);
            if(qgtext) qgroup->addToGroup(qgtext);
            v->getScene()->addItem(qgroup);
            qgi = qgroup;
            if(qgi) {
                if(getselected())
                    qgi->setZValue(QGMAXZVALUE+QGZVALINC);
                else
                    qgi->setZValue(zValue);

            }
        }
    } else {
        if(qgpath)
            qgi=qgpath;

        if(glist.size() > 0) qgi = v->getScene()->createItemGroup(glist);
        if(qgi)
        {
            if(qgbox) ((QGraphicsItemGroup*)qgi)->addToGroup(qgbox);
            if(getselected())
                qgi->setZValue(QGMAXZVALUE+QGZVALINC);
            else
                qgi->setZValue(zValue);

        }
    }

#ifdef NEW_RUBBERB
    // highjack the qgi for the rubberband
    if(drawmode == DM_INVERT) {
        if(qgi) {
            if(v->qgirb == 0) {
                v->qgirb = new QGraphicsItemGroup();
                v->getScene()->addItem(v->qgirb);
                ((QGraphicsItemGroup*)v->qgirb)->addToGroup(qgi);
            } else
                ((QGraphicsItemGroup*)v->qgirb)->addToGroup(qgi);
        }
        qgi=0;
    }
#endif
    if(qgi && drawmode == DM_PRINT)
        qgi->setZValue(zValue);
    if(qgi && dynamic_cast<View3dSubSurface*>(v) != 0)
        if(((View3dSubSurface*)v)->qgicp != 0 &&
           ((View3dSubSurface*)v)->qgicp != qgi->parentItem())
            qgi->setParentItem(((View3dSubSurface*)v)->qgicp);


    glist.clear();


//#endif
#else
    Point p1,p2,p3,p4,p1d;
    BYTE fbCharSet;
    int i,j,tlen,halo,i1,i2,status,metafile,ih,iw,w,ll,nlines,maxh,maxw,*dx,hjust,vjust;
    double scale,rh,rw,rtw,y,oheight,a,l,cosa,sina,xmaxh,xmaxw,xw,tmHeight,tmDescent,ppscale;
    Point zero(0.0,0.0,0.0),o,xa,ya;
    RCCHAR *s;
    Transform tr;
    SIZE size;

    if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
    v->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

    if (0 && t == NULL) {
        if (v->displaylist.draw(this,drawmode)) return;
        if (! v->displaylist.geterrorstatus())
            addtodisplaylist(v,0);
        if (v->displaylist.draw(this,drawmode)) return;
    }
    if (options.test(10)) { // outline
#if ! defined(_WIN32_WCE)
        RCGLYPHMETRICS gm;
        RCMAT2 mat2;
        TTPOLYGONHEADER *ttph;
        TTPOLYCURVE *ttpc;

        xa = xaxis;
        ya = yaxis;
        tr.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
        xa = tr.transform(xa);
        ya = tr.transform(ya);
        if (t == 0)
            o = origin;
        else {
            o = t->transform(origin);
            xa = t->transform(origin+xa) - o;
            ya = t->transform(origin+ya) - o;
        }
        mat2.eM11.value = 1;
        mat2.eM11.fract = 0;
        mat2.eM12.value = 0;
        mat2.eM12.fract = 0;
        mat2.eM21.value = 0;
        mat2.eM21.fract = 0;
        mat2.eM22.value = 1;
        mat2.eM22.fract = 0;
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
#ifdef _USING_QTCHAR
        hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                            options.test(7),options.test(6),0,fbCharSet,
                            OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
        hfont = CreateFont(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                           options.test(7),options.test(6),0,fbCharSet,
                           OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,RCPRINTABLE(font));
#endif
        metafile = GetDeviceCaps(v->gethdc(),TECHNOLOGY) == DT_METAFILE;
        if (hfont != NULL) {
            oldhfont = (RCHFONT) SelectObject(v->gethdc(),hfont);
            if (metafile)
                cwoldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
        }
#ifdef _USING_QTCHAR
        if (metafile)
            GetTextMetricsW(cadwindow->gethdc(),&tm);
        else
            GetTextMetricsW(v->gethdc(),&tm);
#else
        if (metafile)
            GetTextMetrics(cadwindow->gethdc(),&tm);
        else
            GetTextMetrics(v->gethdc(),&tm);
#endif

        if (tm.tmHeight == 0) tm.tmHeight = 1;
        if (options.test(9))
            scale = v->getplotscale() / double(tm.tmHeight) * height;
        else
            scale = height / double(tm.tmHeight);
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
        else if (options.test(3))
            oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
        else
            oheight = -(tm.tmHeight - tm.tmDescent) * scale;
        xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
        xmaxw = 0;
        y = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            if (metafile) {
#if defined(WIN32)
                SIZE size;
#ifdef _USING_QTCHAR
                GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
#else
                GetTextExtentPoint(cadwindow->gethdc(),(TCHAR*)s,ll,&size);
#endif
                xw = size.cx * scale;
#else
                long r;
                r = GetTextExtent(cadwindow->gethdc(),s,ll);
                xw = LOWORD(r) * scale;
#endif
            } else {
#if defined(WIN32)
                SIZE size;
#ifdef _USING_QTCHAR
                GetTextExtentPoint32W(v->gethdc(), s,ll,&size);
#else
                GetTextExtentPoint(v->gethdc(),(TCHAR*)s,ll,&size);
#endif
                xw = size.cx * scale;
#else
                long r;
                r = GetTextExtent(v->gethdc(),s,ll);
                xw = LOWORD(r) * scale;
#endif
            }
            if (xw > xmaxw) xmaxw = xw;
            if (options.test(1))
                p1.setxyz(-xw / 2.0,oheight + y,0.0);
            else if (options.test(2))
                p1.setxyz(-xw,oheight + y,0.0);
            else
                p1.setxyz(0.0,oheight + y,0.0);
            if (1 || drawmode != DM_INVERT) {
                for (i = 0 ; i < ll ; i++) {
#ifdef _USING_QTCHAR
                    if (metafile)
                        status = int(GetGlyphOutlineW(cadwindow->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
                    else
                        status = int(GetGlyphOutlineW(v->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
#else
                    if (metafile)
                        status = int(GetGlyphOutline(cadwindow->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
                    else
                        status = int(GetGlyphOutline(v->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
#endif
                    i1 = 0;
                    while (i1 < status) {
                        ttph = (TTPOLYGONHEADER *) (buffer + i1);
                        i2 = int(i1 + ttph->cb);
                        i1 += sizeof(TTPOLYGONHEADER);
                        p2 = p4 = o + xa * (p1.x + (ttph->pfxStart.x.value + ttph->pfxStart.x.fract / 65535.0) * scale) +
                                  ya * (p1.y + (ttph->pfxStart.y.value + ttph->pfxStart.y.fract / 65535.0) * scale);
                        while (i1 < i2) {
                            ttpc = (TTPOLYCURVE *) (buffer + i1);
                            for (j = 0 ; j < ttpc->cpfx ; j++) {
                                p3 = o + xa * (p1.x + (ttpc->apfx[j].x.value + ttpc->apfx[j].x.fract / 65535.0) * scale) +
                                     ya * (p1.y + (ttpc->apfx[j].y.value + ttpc->apfx[j].y.fract / 65535.0) * scale);
                                v->line3d(p2,p3);
                                p2 = p3;
                            }
                            i1 += sizeof(TTPOLYCURVE) + (ttpc->cpfx - 1) * sizeof(POINTFX);
                        }
                        v->line3d(p2,p4);
                    }
                    p1.x += gm.gmCellIncX * scale;
                    p1.y += gm.gmCellIncY * scale;
                }
            }
            y -= (tm.tmHeight * (1.0 + vertspacing)) * scale;
            if (s[ll] == 0) break;
        }
        if (options.test(8)) {
            p1 = o + ya * (oheight + y - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
            if (options.test(1))
                p1 = p1 - xa * xmaxw / 2.0;
            else if (options.test(2))
                p1 = p1 - xa * xmaxw;
            p2 = p1 + xa * xmaxw;
            p3 = p2 + ya * xmaxh;
            p4 = p3 - xa * xmaxw;
            v->line3d(p1,p2);
            v->line3d(p2,p3);
            v->line3d(p3,p4);
            v->line3d(p4,p1);
        }
        if (hfont != NULL) {
            if (metafile)
                SelectObject(cadwindow->gethdc(),cwoldhfont);
            SelectObject(v->gethdc(),oldhfont);
            DeleteObject(hfont);
        }
#endif
    } else { // not outline
        metafile = false;
        Point xa1,ya1,o1;
        double angle1;

        int dxlen = RCSTRLEN(text)+1;
        dx = new int[RCSTRLEN(text)+1];

        for (halo = 0 ; halo < 1 + options.test(12) ; halo++) {
            if (t == NULL) {
                p1 = v->modeltoview(origin);
                p2 = v->modeltoview(origin);
            } else {
                p1 = v->modeltoview(t->transform(origin));
                p2 = v->modeltoview(t->transform(origin));
            }
            if (! v->clip3dfb(&p1,&p2))
                break;   //  Text origin is clipped - don't draw it

            if (t == NULL)
                p1 = v->modeltoscreen(origin);
            else
                p1 = v->modeltoscreen(t->transform(origin));
            p1d = p1;
            v->transform2d(&p1.x,&p1.y);

            if (options.test(0))
                fbCharSet = SYMBOL_CHARSET;
            else
                fbCharSet = ANSI_CHARSET;//DEFAULT_CHARSET;//

            xa1 = v->modeltoscreen(t == 0 ? origin + xaxis : t->transform(origin + xaxis));
            v->transform2d(&xa1.x,&xa1.y);
            ya1 = v->modeltoscreen(t == 0 ? origin + yaxis : t->transform(origin + yaxis));
            v->transform2d(&ya1.x,&ya1.y);
            o1 = v->modeltoscreen(t == 0 ? origin : t->transform(origin));
            v->transform2d(&o1.x,&o1.y);
            xa = xa1 - o1;
            xa.z = 0.0;
            if ((l = xa.length()) < 1.0E-10) {
                delete [] dx;
                return;
            }
            xa /= l;
            ya = ya1 - o1;
            ya.z = 0.0;
            if ((l = ya.length()) < 1.0E-10) {
                delete [] dx;
                return;
            }
            ya /= l;


            hjust = options.test(1) + options.test(2) * 2;
            vjust = options.test(3) + options.test(4) * 2;

            //  Compare the transformed z axis to the zaxis calculated from the transformed x and y axis to see if the sense has changed
            Point origin1,xaxis1,yaxis1,zaxis1,zaxis2;
            origin1 = v->modeltoscreen(t == 0 ? origin : t->transform(origin));
            xaxis1 = v->modeltoscreen(t == 0 ? origin + xaxis : t->transform(origin + xaxis)) - origin1;
            yaxis1 = v->modeltoscreen(t == 0 ? origin + yaxis : t->transform(origin + yaxis)) - origin1;
            zaxis1 = v->modeltoscreen(t == 0 ? origin + xaxis.cross(yaxis) : t->transform(origin + xaxis.cross(yaxis))) - origin1;
            zaxis2 = xaxis1.cross(yaxis1);

            angle1 = angle;

            if (zaxis1.dot(zaxis2) > 0.0) {
                //  If the text has been mirrored, need to adjust the justifications
                sina = sin(angle1 / 180.0 * M_PI);
                cosa = cos(angle1 / 180.0 * M_PI);
                if ((-xaxis * sina + yaxis * cosa).dot(-xaxis1 * sina + yaxis1 * cosa) <= 0) {
                    ya = -ya;
                    vjust = 2 - vjust;
                    angle1 = -angle1;
                } else if ((xaxis * cosa + yaxis * sina).dot(xaxis1 * cosa + yaxis1 * sina) < 0.0) {
                    xa = -xa;
                    hjust = 2 - hjust;
                    angle1 = -angle1;
                }
            }


            if (xa.x < -1.0) xa.x = -1.0;
            else if (xa.x > 1.0) xa.x = 1.0;
            a = acos(xa.x);
            if (xa.y > 0.0) a = 2.0 * M_PI - a;
            ya = xa.cross(ya).cross(xa).normalize();

            if (options.test(9))
                scale = v->getplotscale();
            else
                scale = 1.0;
            rh = height * v->getheight() / (v->getvmax() - v->getvmin()) * scale;

            // KMJ : Height of a font must be in points for Qt
            if (options.test(13))  // Height is in points
                rh *= 25.4 / 72.0;
            else // convert mm to points for Qt
                rh *= 0.352777778;

            rw = width * v->getwidth() / (v->getumax() - v->getumin()) * scale;
            if (rh > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT;
            else if (rh < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT;
            else ih = int(rh);
            if (rw > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT;
            else if (rw < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT;
            else iw = int(rw);
            if (ih == 0) {
                delete [] dx;
                return;
            }

            //hfont = CreateFontIndirectW(&lf);
            QString qfname(font);
            //qfname = "SansSerif [" + QString(font) + "]";
            QFont qfont(qfname,ih);
            qfont.setStyleStrategy(QFont::ForceOutline);
            //qfont.setLetterSpacing(SpacingType type, qreal spacing);
            qfont.setPixelSize((int)(rh*2.83464567));
            //v->getScene()->setFont(qfont);
            //QFontMetrics fm1(v->getScene()->font());
            QFontMetrics fm1(qfont);
            //qDebug() << "font name : " << QFontInfo(v->getScene()->font()).family();
            //qDebug() << "font height : " << fm1.height();
            //qDebug() << "font box    : " << fm1.boundingRect(QString(text));

            // set the font weight
            if(options.test(5))
                qfont.setWeight(QFont::Bold);
            else
                qfont.setWeight(QFont::Normal);

            // set italic
            qfont.setItalic(options.test(7));

            // set underline
            qfont.setUnderline(options.test(6));

            //lf.lfHeight = 100;
            //lf.lfWidth = (int)(lf.lfHeight * width / (height > 0.0 ? height : 1.0));
            double fstretch = width  / (height > 0.0 ? height : 1.0);
            fstretch = fstretch == 0.0 ? 1.0 : fstretch;
            qDebug() << "font width stretch : " << fstretch;

            sina = sin(a + angle1 / 180.0 * M_PI);
            cosa = cos(a + angle1 / 180.0 * M_PI);

            //
            //  On a rotated font, the tmHeight, and tmDescent are wrong
            //   This seems to be some problem with GetTextMetrics
            //
            //QFontMetrics fm(v->getScene()->font());
            //tmHeight = fm1.ascent();
            tmHeight = fm1.height();
            tmDescent = fm1.descent();

            qDebug() << "text width : " << fm1.width(QString(text));
            qDebug() << "text height : " << fm1.height();

            for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';

            if (0 && v->getprinterhdc() != 0) {
                if (options.test(9))  //  In paper space, use the actual text size if scaled.
                    scale = 1.0;
                else
                    scale = 1.0 / v->getplotscale();
                if (v->gettransform2d())
                    ppscale = v->getheight() / (v->getvmax() - v->getvmin()) * v->getplotscale() *
                              cadwindow->printpreviewwindow->getprintscale() / (double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) / 25.4);
                else
                    ppscale = v->getheight() / (v->getvmax() - v->getvmin()) *
                              cadwindow->printpreviewwindow->getprintscale() / (double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) / 25.4);

                rh /= ppscale;
                rw /= ppscale;
                if (rh > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT;
                else if (rh < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT;
                else ih = int(rh);
                if (rw > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT;
                else if (rw < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT;
                else iw = int(rw);

                if (ih == 0) {
                    if (hfont != 0) {
                        if (metafile)
                            SelectObject(cadwindow->gethdc(),cwoldhfont);
                        else if (drawmode == DM_INVERT)
                            SelectObject(cadwindow->getoffscreenhdc(),cwoldhfont);
                        SelectObject(v->gethdc(),oldhfont);
                        DeleteObject(hfont);
                    }
                    return;
                }

                /*
                lf.lfHeight = options.test(13) ? -ih : ih;
                lf.lfWidth = iw;
                printerhfont = CreateFontIndirectW(&lf);
                printeroldhfont = (RCHFONT) SelectObject(v->getprinterhdc(),printerhfont);
                GetTextMetricsW(v->getprinterhdc(),&tm);
                */
                tmHeight = tm.tmHeight * ppscale;
                tmDescent = tm.tmDescent * ppscale;
            }

            if (vjust == 0)
                oheight = -nlines * rh + tmDescent - (nlines - 1) * rh * vertspacing;
            else if (vjust == 1)
                oheight = -(nlines + (nlines - 1) * vertspacing) * rh / 2;
            else
                oheight = 0.0;
            p1.x += oheight * sina;
            p1.y += oheight * cosa;
            p3 = p1;
            maxh = int(rh * (nlines + (nlines - 1)*vertspacing));
            maxw = 0;

            for (s = text ; *s!=0 ;  s += ll + 2) {
                for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
                if (metafile) {
                    SIZE size;
                    //GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
                    //QFontMetrics fm1(v->getScene()->font());
                    //w = size.cx;
                    w = fm1.width(QString(s),ll);
                } else if (drawmode == DM_INVERT) {
                    SIZE size;
                    //SelectObject(cadwindow->getoffscreenhdc(),hreferencefont);
                    //GetTextExtentPoint32(cadwindow->getoffscreenhdc(),(TCHAR*)s,ll,&size);
                    //GetTextExtentPoint32W(cadwindow->getoffscreenhdc(),s,ll,&size);
                    //QFontMetrics fm(v->getScene()->font());
                    //SelectObject(cadwindow->getoffscreenhdc(),hfont);
                    //rtw = size.cx * rh / 100.0;
                    rtw = fm1.width(QString(s),ll) * rh /*/ 100.0*/;
                    w = (int) rtw;
                } else {
                    SIZE size;
                    //SelectObject(v->gethdc(),hreferencefont);
                    //GetTextExtentPoint32(v->gethdc(),(TCHAR*)s,ll,&size);
                    //GetTextExtentPoint32W(v->gethdc(), s,ll,&size);
                    //QFontMetrics fm(v->getScene()->font());
                    //SelectObject(v->gethdc(),hfont);
                    //rtw = size.cx * rh / 100.0;
                    rtw = fm1.width(QString(s),ll) * rh /*/ 100.0*/;
                    w = (int) rtw;
                }
                if (w > maxw) maxw = w;
                p2 = p1;
                if (hjust == 1) {
                    p2.x -= rtw * cosa / 2.0;
                    p2.y += rtw * sina / 2.0;
                } else if (hjust == 2) {
                    p2.x -= rtw * cosa;
                    p2.y += rtw * sina;
                }

                tlen = 0;
                for (i = 0 ; i < ll ; i++) {
                    //GetTextExtentPoint32W(v->gethdc(), s,i+1,&size);
                    //QFontMetrics fm(v->getScene()->font());
                    //dx[i] = int(size.cx * rh / 100.0 - tlen);
                    dx[i] = int(fm1.width(i+1) * rh /*/ 100.0*/ - tlen);
                    tlen += dx[i];
                }

                if (drawmode != DM_INVERT) {
                    double x,y;

                    SetBkMode(v->gethdc(),TRANSPARENT);

                    if (v->gettransform2d()) {
                        SIZE size;
                        x = p2.x;
                        y = p2.y;
                        //GetTextExtentPoint32(v->gethdc(),(TCHAR*)s,ll,&size);
                        //GetTextExtentPoint32W(v->gethdc(), s,ll,&size);
                        //QFontMetrics fm(v->getScene()->font());
                        size.cx = fm1.width(QString(s),ll);
                        size.cy = fm1.height();
                        SetTextAlign(v->gethdc(),TA_TOP | TA_LEFT);
                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;

                        if (p1d.x + size.cx * 2 > 0.0 && p1d.x - size.cx * 2 < v->getwidth() &&
                                p1d.y + size.cy * 2 > 0.0 && p1d.y - size.cy * 2 < v->getheight()) {

                            if(qgi) {
                                //qDebug() << "Text::draw() deleting(2) : qgi : " << qgi;
                                v->getScene()->removeItem(qgi);
                            }
                            delete qgi;
                            qgi=0;

                            //ExtTextOutW(v->gethdc(),int(x),int(y),0,0,s,ll,dx);
                            //QGraphicsTextItem *stxt = v->getScene()->addText(QString(s));
                            QGraphicsSimpleTextItem *stxt = v->getScene()->addSimpleText(QString(s));
                            stxt->setBrush(QBrush(v->curTextColour));
                            stxt->setFont(qfont);
                            stxt->setPos(p2.x,p2.y/*-(rh*2.83464567)*/);
                            // scale the width
                            //QTransform txttf = stxt->transform();
                            QTransform stf;
                            stf.scale(fstretch, 1.0);
                            stxt->setTransform(stf,true);
                            //stxt->setRotation((-a * 180.0 / M_PI));
                            stxt->setRotation(a * 180.0 / M_PI - angle1);
                            //stxt->setRotation(a * 180.0 / M_PI + angle1);

                            qgi = stxt;
                        }
                    } else {
                        SIZE size;
                        //QFontMetrics fm(v->getScene()->font());
                        if (metafile)
                            GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
                        else {
                            //GetTextExtentPoint(v->gethdc(),(TCHAR*)s,ll,&size);
                            // GetTextExtentPoint32W(v->gethdc(), s,ll,&size);
                            //QFont qf(v->getScene()->font());
                            //qDebug() << "Font Family : " << qf.family();
                            //QFontMetrics fm(v->getScene()->font());
                            QString qs(s);
                            //qDebug() << "Text : " << qs;
                            //qDebug() << "font height : " << fm.height();
                            //qDebug() << "Text Length : " << qs.size();
                            size.cx = fm1.width(qs);
                            size.cy = fm1.height();
                        }
                        //SetTextAlign(v->gethdc(),TA_TOP | TA_LEFT);

                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;

                        //if (p2.x + size.cx > 0.0 && p2.x - size.cx < v->getwidth() &&
                        //    p2.y + size.cy > 0.0 && p2.y - size.cy < v->getheight())
                        {
                            //ExtTextOutW(v->gethdc(),int(p2.x),int(p2.y),0,0,(LPCWSTR)s,ll,dx);
                            //ExtTextOutW(v->gethdc(),int(p2.x),int(p2.y),0,0,s,ll,NULL);
                            QString qs(s);

                            if(qgi) {
                                //qDebug() << "Text::draw() deleting(2) : qgi : " << qgi;
                                v->getScene()->removeItem(qgi);
                            }
                            delete qgi;
                            qgi=0;

                            //QGraphicsTextItem *stxt = v->getScene()->addText(qs);
                            QGraphicsSimpleTextItem *stxt = v->getScene()->addSimpleText(QString(qs));
                            stxt->setBrush(QBrush(v->curTextColour));
                            stxt->setFont(qfont);
                            stxt->setPos(p2.x,p2.y/*-(rh*2.83464567)*/);
                            QTransform stf;
                            stf.scale(fstretch, 1.0);
                            stxt->setTransform(stf,true);
                            //stxt->setRotation((-a * 180.0 / M_PI));
                            stxt->setRotation(a * 180.0 / M_PI - angle1);
                            //stxt->setRotation(a * 180.0 / M_PI + angle1);
                            qgi = stxt;
                        }
                        //else
                        //  p2.x += 1.0E-30;
                    }
                    SetBkMode(v->gethdc(),OPAQUE);
                } else {
                    SIZE size;

                    //GetTextExtentPoint32(cadwindow->getoffscreenhdc(),(LPCSTR)s,ll,&size);
                    //GetTextExtentPoint32W(cadwindow->getoffscreenhdc(),s,ll,&size);
                    //GetTextExtentPoint32W(v->gethdc(),s,ll,&size);
                    QString qs(s);
                    size.cx = fm1.width(qs);
                    size.cy = fm1.height();

                    // add a % to the box
                    //size.cx = tlen ;//* 4.0;
                    //SetBkMode(cadwindow->getoffscreenhdc(),OPAQUE);
                    //SetBkColor(cadwindow->getoffscreenhdc(),RGB(0,0,0));
                    //SetTextColor(cadwindow->getoffscreenhdc(),RGB(255,255,255));

                    if (v->gettransform2d()) {
                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;
                    } else
                        p1d = p2;

                    if (p1d.x + size.cx > 0.0 && p1d.x - size.cx < v->getwidth() &&
                            p1d.y + size.cy > 0.0 && p1d.y - size.cy < v->getheight()) {
                        RECT r;
                        if (size.cx > size.cy)
                            size.cy = size.cx;
                        else
                            size.cx = size.cy;
                        //r.left = int(p2.x)-size.cx;
                        //r.right = int(p2.x)+size.cy;
                        //r.top = int(p2.y)-size.cy;
                        //r.bottom = int(p2.y)+size.cy;
                        //FillRect(cadwindow->getoffscreenhdc(),&r,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
                        //SetTextAlign(cadwindow->getoffscreenhdc(),TA_TOP | TA_LEFT);
                        //ExtTextOutW(cadwindow->getoffscreenhdc(),int(p2.x),int(p2.y),0,0,(LPCWSTR)s,ll,dx);
                        //ExtTextOutW(cadwindow->getoffscreenhdc(),int(p2.x),int(p2.y),0,0,s,ll,NULL);
                        if(qgi) {
                            //qDebug() << "Text::draw() deleting(2) : qgi : " << qgi;
                            v->getScene()->removeItem(qgi);
                        }
                        delete qgi;
                        qgi=0;

                        //QGraphicsTextItem *stxt = v->getScene()->addText(QString(s));
                        QGraphicsSimpleTextItem *stxt = v->getScene()->addSimpleText(qs);
                        stxt->setBrush(QBrush(v->curTextColour));
                        stxt->setFont(qfont);
                        stxt->setPos(p2.x,p2.y/*-(rh*2.83464567)*/);
                        QTransform stf;
                        stf.scale(fstretch, 1.0);
                        stxt->setTransform(stf,true);
                        //stxt->setRotation((-a * 180.0 / M_PI));
                        stxt->setRotation(a * 180.0 / M_PI - angle1);
                        //stxt->setRotation(a * 180.0 / M_PI + angle1);
                        qgi = stxt;
                        //BitBlt(v->gethdc(),int(p2.x)-size.cx,int(p2.y)-size.cy,size.cx*2,size.cy*2,cadwindow->getoffscreenhdc(),int(p2.x)-size.cx,int(p2.y)-size.cy,SRCINVERT);
                    }
                }

                p1.x += rh * (1.0 + vertspacing) * sina;
                p1.y += rh * (1.0 + vertspacing) * cosa;
                if (s[ll] == 0) break;
            }

            if (hjust == 1) {
                p3.x -= maxw * cosa / 2.0;
                p3.y += maxw * sina / 2.0;
            } else if (hjust == 2) {
                p3.x -= maxw * cosa;
                p3.y += maxw * sina;
            }

            //  Underlined.
            if (drawmode != DM_INVERT && options.test(12) && halo == 0) {
                Point *hp[1];
                Point hp1[4];
                int nvert;

                p3.x += -ih / 10.0 * cosa - ih/10.0 * sina;
                p3.y += +ih / 10.0 * sina - ih/10.0 * cosa;

                v->setup(linestylescale,0.0,drawmode,halocolour,style,weight,0);

                hp1[0] = Point(int(p3.x),v->getheight() - 1.0 - int(p3.y),0.0);
                hp1[1] = Point(int(p3.x + (maxw+ih/5.0) * cosa),v->getheight() - 1.0 - int((p3.y - (maxw+ih/5.0) * sina)),0.0);
                hp1[2] = Point(int(p3.x + (maxw+ih/5.0) * cosa + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((p3.y - (maxw+ih/5.0) * sina + (maxh+ih/5.0) * cosa)),0.0);
                hp1[3] = Point(int(p3.x + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((p3.y + (maxh+ih/5.0) * cosa)),0.0);

                nvert = 4;
                hp[0] = hp1;
                v->fill(hp,&nvert,1,1);

                v->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

                p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
                p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;

            }

            if (options.test(8)) {
                double x1,y1,x2,y2;

                p3.x += -ih / 10.0 * cosa - ih/10.0 * sina;
                p3.y += +ih / 10.0 * sina - ih/10.0 * cosa;

                x1 = p3.x;
                y1 = p3.y;
                x2 = p3.x + (maxw+ih/5.0) * cosa;
                y2 = p3.y - (maxw+ih/5.0) * sina;

                if (v->gethwnd() != 0)
                    v->line3d(v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                    MoveToEx(v->gethdc(),int(x1),int(y1),0);
                    LineTo(v->gethdc(),int(x2),int(y2));
                }

                x1 = p3.x + (maxw+ih/5.0) * cosa;
                y1 = p3.y - (maxw+ih/5.0) * sina;
                x2 = p3.x + (maxw+ih/5.0) * cosa + (maxh+ih/5.0) * sina;
                y2 = p3.y - (maxw+ih/5.0) * sina + (maxh+ih/5.0) * cosa;
                if (v->gethwnd() != 0)
                    v->line3d(v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                    MoveToEx(v->gethdc(),int(x1),int(y1),0);
                    LineTo(v->gethdc(),int(x2),int(y2));
                }

                x1 = p3.x + (maxw+ih/5.0) * cosa + (maxh+ih/5.0) * sina;
                y1 = p3.y - (maxw+ih/5.0) * sina + (maxh+ih/5.0) * cosa;
                x2 = p3.x + (maxh+ih/5.0) * sina;
                y2 = p3.y + (maxh+ih/5.0) * cosa;
                if (v->gethwnd() != 0)
                    v->line3d(v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                    MoveToEx(v->gethdc(),int(x1),int(y1),0);
                    LineTo(v->gethdc(),int(x2),int(y2));
                }

                x1 = p3.x + (maxh+ih/5.0) * sina;
                y1 = p3.y + (maxh+ih/5.0) * cosa;
                x2 = p3.x;
                y2 = p3.y;
                if (v->gethwnd() != 0)
                    v->line3d(v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                    MoveToEx(v->gethdc(),int(x1),int(y1),0);
                    LineTo(v->gethdc(),int(x2),int(y2));
                }

                p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
                p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;


            }
        }
        delete [] dx;
    }
#endif
#else
    Point p1,p2,p3,p4,p1d;
#ifdef _USING_QTCHAR
    LOGFONTW lf;
#else
    LOGFONT lf;
#endif
    RCHFONT cwoldhfont,oldhfont,hfont,printerhfont,printeroldhfont,hreferencefont;
    BYTE fbCharSet;
    int i,j,tlen,halo,i1,i2,status,metafile,ih,iw,w,ll,nlines,maxh,maxw,*dx,hjust,vjust;
    double scale,rh,rw,rtw,y,oheight,a,l,cosa,sina,xmaxh,xmaxw,xw,tmHeight,tmDescent,ppscale;
#ifdef _USING_QTCHAR
    TEXTMETRICW tm;
#else
    TEXTMETRIC tm;
#endif
    Point zero(0.0,0.0,0.0),o,xa,ya;
    char buffer[GGO_BUF_SIZE];
    RCCHAR *s;
    Transform tr;
    SIZE size;

    if (getselected() && drawmode == DM_NORMAL) drawmode = DM_SELECT;
    v->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

    if (0 && t == NULL) {
        if (v->displaylist.draw(this,drawmode)) return;
        if (! v->displaylist.geterrorstatus())
            addtodisplaylist(v,0);
        if (v->displaylist.draw(this,drawmode)) return;
    }
    if (options.test(10)) {
#if ! defined(_WIN32_WCE)
        RCGLYPHMETRICS gm;
        RCMAT2 mat2;
        TTPOLYGONHEADER *ttph;
        TTPOLYCURVE *ttpc;

        xa = xaxis;
        ya = yaxis;
        tr.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
        xa = tr.transform(xa);
        ya = tr.transform(ya);
        if (t == 0)
            o = origin;
        else {
            o = t->transform(origin);
            xa = t->transform(origin+xa) - o;
            ya = t->transform(origin+ya) - o;
        }
        mat2.eM11.value = 1;
        mat2.eM11.fract = 0;
        mat2.eM12.value = 0;
        mat2.eM12.fract = 0;
        mat2.eM21.value = 0;
        mat2.eM21.fract = 0;
        mat2.eM22.value = 1;
        mat2.eM22.fract = 0;
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
#ifdef _USING_QTCHAR
        hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                            options.test(7),options.test(6),0,fbCharSet,
                            OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
        hfont = CreateFont(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                           options.test(7),options.test(6),0,fbCharSet,
                           OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,RCPRINTABLE(font));
#endif
        metafile = GetDeviceCaps(v->gethdc(),TECHNOLOGY) == DT_METAFILE;
        if (hfont != NULL) {
            oldhfont = (RCHFONT) SelectObject(v->gethdc(),hfont);
            if (metafile)
                cwoldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
        }
#ifdef _USING_QTCHAR
        if (metafile)
            GetTextMetricsW(cadwindow->gethdc(),&tm);
        else
            GetTextMetricsW(v->gethdc(),&tm);
#else
        if (metafile)
            GetTextMetrics(cadwindow->gethdc(),&tm);
        else
            GetTextMetrics(v->gethdc(),&tm);
#endif

        if (tm.tmHeight == 0) tm.tmHeight = 1;
        if (options.test(9))
            scale = v->getplotscale() / double(tm.tmHeight) * height;
        else
            scale = height / double(tm.tmHeight);
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
        else if (options.test(3))
            oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
        else
            oheight = -(tm.tmHeight - tm.tmDescent) * scale;
        xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
        xmaxw = 0;
        y = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            if (metafile) {
#if defined(WIN32)
                SIZE size;
#ifdef _USING_QTCHAR
                GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
#else
                GetTextExtentPoint(cadwindow->gethdc(),(TCHAR*)s,ll,&size);
#endif
                xw = size.cx * scale;
#else
                long r;
                r = GetTextExtent(cadwindow->gethdc(),s,ll);
                xw = LOWORD(r) * scale;
#endif
            } else {
#if defined(WIN32)
                SIZE size;
#ifdef _USING_QTCHAR
                GetTextExtentPoint32W(v->gethdc(), s,ll,&size);
#else
                GetTextExtentPoint(v->gethdc(),(TCHAR*)s,ll,&size);
#endif
                xw = size.cx * scale;
#else
                long r;
                r = GetTextExtent(v->gethdc(),s,ll);
                xw = LOWORD(r) * scale;
#endif
            }
            if (xw > xmaxw) xmaxw = xw;
            if (options.test(1))
                p1.setxyz(-xw / 2.0,oheight + y,0.0);
            else if (options.test(2))
                p1.setxyz(-xw,oheight + y,0.0);
            else
                p1.setxyz(0.0,oheight + y,0.0);
            if (1 || drawmode != DM_INVERT) {
                for (i = 0 ; i < ll ; i++) {
#ifdef _USING_QTCHAR
                    if (metafile)
                        status = int(GetGlyphOutlineW(cadwindow->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
                    else
                        status = int(GetGlyphOutlineW(v->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
#else
                    if (metafile)
                        status = int(GetGlyphOutline(cadwindow->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
                    else
                        status = int(GetGlyphOutline(v->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
#endif
                    i1 = 0;
                    while (i1 < status) {
                        ttph = (TTPOLYGONHEADER *) (buffer + i1);
                        i2 = int(i1 + ttph->cb);
                        i1 += sizeof(TTPOLYGONHEADER);
                        p2 = p4 = o + xa * (p1.x + (ttph->pfxStart.x.value + ttph->pfxStart.x.fract / 65535.0) * scale) +
                                  ya * (p1.y + (ttph->pfxStart.y.value + ttph->pfxStart.y.fract / 65535.0) * scale);
                        while (i1 < i2) {
                            ttpc = (TTPOLYCURVE *) (buffer + i1);
                            for (j = 0 ; j < ttpc->cpfx ; j++) {
                                p3 = o + xa * (p1.x + (ttpc->apfx[j].x.value + ttpc->apfx[j].x.fract / 65535.0) * scale) +
                                     ya * (p1.y + (ttpc->apfx[j].y.value + ttpc->apfx[j].y.fract / 65535.0) * scale);
                                v->line3d(p2,p3);
                                p2 = p3;
                            }
                            i1 += sizeof(TTPOLYCURVE) + (ttpc->cpfx - 1) * sizeof(POINTFX);
                        }
                        v->line3d(p2,p4);
                    }
                    p1.x += gm.gmCellIncX * scale;
                    p1.y += gm.gmCellIncY * scale;
                }
            }
            y -= (tm.tmHeight * (1.0 + vertspacing)) * scale;
            if (s[ll] == 0) break;
        }
        if (options.test(8)) {
            p1 = o + ya * (oheight + y - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
            if (options.test(1))
                p1 = p1 - xa * xmaxw / 2.0;
            else if (options.test(2))
                p1 = p1 - xa * xmaxw;
            p2 = p1 + xa * xmaxw;
            p3 = p2 + ya * xmaxh;
            p4 = p3 - xa * xmaxw;
            v->line3d(p1,p2);
            v->line3d(p2,p3);
            v->line3d(p3,p4);
            v->line3d(p4,p1);
        }
        if (hfont != NULL) {
            if (metafile)
                SelectObject(cadwindow->gethdc(),cwoldhfont);
            SelectObject(v->gethdc(),oldhfont);
            DeleteObject(hfont);
        }
#endif
    } else {
        Point xa1,ya1,o1;
        double angle1;

        int dxlen = RCSTRLEN(text)+1;
        dx = new int[RCSTRLEN(text)+1];

        for (halo = 0 ; halo < 1 + options.test(12) ; halo++) {
            if (t == NULL) {
                p1 = v->modeltoview(origin);
                p2 = v->modeltoview(origin);
            } else {
                p1 = v->modeltoview(t->transform(origin));
                p2 = v->modeltoview(t->transform(origin));
            }
            if (! v->clip3dfb(&p1,&p2))
                break;   //  Text origin is clipped - don't draw it

            if (t == NULL)
                p1 = v->modeltoscreen(origin);
            else
                p1 = v->modeltoscreen(t->transform(origin));
            p1d = p1;
            v->transform2d(&p1.x,&p1.y);
            if (options.test(0))
                fbCharSet = SYMBOL_CHARSET;
            else
                fbCharSet = ANSI_CHARSET;//DEFAULT_CHARSET;//
            xa1 = v->modeltoscreen(t == 0 ? origin + xaxis : t->transform(origin + xaxis));
            v->transform2d(&xa1.x,&xa1.y);
            ya1 = v->modeltoscreen(t == 0 ? origin + yaxis : t->transform(origin + yaxis));
            v->transform2d(&ya1.x,&ya1.y);
            o1 = v->modeltoscreen(t == 0 ? origin : t->transform(origin));
            v->transform2d(&o1.x,&o1.y);
            xa = xa1 - o1;
            xa.z = 0.0;
            if ((l = xa.length()) < 1.0E-10) {
                delete [] dx;
                return;
            }
            xa /= l;
            ya = ya1 - o1;
            ya.z = 0.0;
            if ((l = ya.length()) < 1.0E-10) {
                delete [] dx;
                return;
            }
            ya /= l;


            hjust = options.test(1) + options.test(2) * 2;
            vjust = options.test(3) + options.test(4) * 2;

            //  Compare the transformed z axis to the zaxis calculated from the transformed x and y axis to see if the sense has changed
            Point origin1,xaxis1,yaxis1,zaxis1,zaxis2;
            origin1 = v->modeltoscreen(t == 0 ? origin : t->transform(origin));
            xaxis1 = v->modeltoscreen(t == 0 ? origin + xaxis : t->transform(origin + xaxis)) - origin1;
            yaxis1 = v->modeltoscreen(t == 0 ? origin + yaxis : t->transform(origin + yaxis)) - origin1;
            zaxis1 = v->modeltoscreen(t == 0 ? origin + xaxis.cross(yaxis) : t->transform(origin + xaxis.cross(yaxis))) - origin1;
            zaxis2 = xaxis1.cross(yaxis1);

            angle1 = angle;

            if (zaxis1.dot(zaxis2) > 0.0) {
                //  If the text has been mirrored, need to adjust the justifications
                sina = sin(angle1 / 180.0 * M_PI);
                cosa = cos(angle1 / 180.0 * M_PI);
                if ((-xaxis * sina + yaxis * cosa).dot(-xaxis1 * sina + yaxis1 * cosa) <= 0) {
                    ya = -ya;
                    vjust = 2 - vjust;
                    angle1 = -angle1;
                } else if ((xaxis * cosa + yaxis * sina).dot(xaxis1 * cosa + yaxis1 * sina) < 0.0) {
                    xa = -xa;
                    hjust = 2 - hjust;
                    angle1 = -angle1;
                }
            }


            if (xa.x < -1.0) xa.x = -1.0;
            else if (xa.x > 1.0) xa.x = 1.0;
            a = acos(xa.x);
            if (xa.y > 0.0) a = 2.0 * M_PI - a;
            ya = xa.cross(ya).cross(xa).normalize();

            if (options.test(9))
                scale = v->getplotscale();
            else
                scale = 1.0;
            rh = height * v->getheight() / (v->getvmax() - v->getvmin()) * scale;
            if (options.test(13))  // Height is in points
                rh *= 25.4 / 72.0;
            rw = width * v->getwidth() / (v->getumax() - v->getumin()) * scale;
            if (rh > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT;
            else if (rh < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT;
            else ih = int(rh);
            if (rw > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT;
            else if (rw < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT;
            else iw = int(rw);
            if (ih == 0) {
                delete [] dx;
                return;
            }

            lf.lfHeight = options.test(13) ? -ih : ih;
            lf.lfWidth = iw;
            lf.lfEscapement = int((a * 180.0 / M_PI + angle1) * 10.0);
            lf.lfOrientation = int((a * 180.0 / M_PI + angle1)* 10.0);
            lf.lfWeight = (options.test(5) ? FW_BOLD : FW_NORMAL);
            lf.lfItalic = (BYTE)options.test(7);
            lf.lfUnderline = (BYTE)options.test(6);
            lf.lfStrikeOut = 0;
            lf.lfCharSet = fbCharSet;
            lf.lfOutPrecision = OUT_STROKE_PRECIS;
            lf.lfClipPrecision = CLIP_STROKE_PRECIS;
            lf.lfQuality = PROOF_QUALITY;
            lf.lfPitchAndFamily = 0;
#if ! defined(_WIN32_WCE)
            //strcpy(lf.lfFaceName,(TCHAR*)font);
            RCSTRCPY((RCCHAR*)lf.lfFaceName, font);
#endif

#ifdef _USING_QTCHAR
            hfont = CreateFontIndirectW(&lf);

            lf.lfHeight = 100;
            lf.lfWidth = (int)(lf.lfHeight * width / (height > 0.0 ? height : 1.0));
            hreferencefont = CreateFontIndirectW(&lf);
#else
            hfont = CreateFontIndirect(&lf);

            lf.lfHeight = 100;
            lf.lfWidth = (int)(lf.lfHeight * width / (height > 0.0 ? height : 1.0));
            hreferencefont = CreateFontIndirect(&lf);
#endif
            sina = sin(a + angle1 / 180.0 * M_PI);
            cosa = cos(a + angle1 / 180.0 * M_PI);
            metafile = GetDeviceCaps(v->gethdc(),TECHNOLOGY) == DT_METAFILE;
            if (hfont != NULL) {
                oldhfont = (RCHFONT) SelectObject(v->gethdc(),hfont);
                if (metafile)
                    cwoldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
                else if (drawmode == DM_INVERT)
                    cwoldhfont = (RCHFONT) SelectObject(cadwindow->getoffscreenhdc(),hfont);
            }
#ifdef _USING_QTCHAR
            if (metafile)
                GetTextMetricsW(cadwindow->gethdc(),&tm);
            else if (drawmode == DM_INVERT)
                GetTextMetricsW(cadwindow->getoffscreenhdc(),&tm);
            else
                GetTextMetricsW(v->gethdc(),&tm);
#else
            if (metafile)
                GetTextMetrics(cadwindow->gethdc(),&tm);
            else if (drawmode == DM_INVERT)
                GetTextMetrics(cadwindow->getoffscreenhdc(),&tm);
            else
                GetTextMetrics(v->gethdc(),&tm);
#endif

            //
            //  On a rotated font, the tmHeight, and tmDescent are wrong
            //   This seems to be some problem with GetTextMetrics
            //
            tmHeight = tm.tmHeight;
            tmDescent = tm.tmDescent;
            for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
            if (0 && v->getprinterhdc() != 0) {
                if (options.test(9))  //  In paper space, use the actual text size if scaled.
                    scale = 1.0;
                else
                    scale = 1.0 / v->getplotscale();
                if (v->gettransform2d())
                    ppscale = v->getheight() / (v->getvmax() - v->getvmin()) * v->getplotscale() *
                              cadwindow->printpreviewwindow->getprintscale() / (double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) / 25.4);
                else
                    ppscale = v->getheight() / (v->getvmax() - v->getvmin()) *
                              cadwindow->printpreviewwindow->getprintscale() / (double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) / 25.4);


                rh /= ppscale;
                rw /= ppscale;
//               rh = height * scale / cadwindow->printpreviewwindow->getprintscale() * double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSX)) / 25.4;
//               rw = width * scale / cadwindow->printpreviewwindow->getprintscale() * double(GetDeviceCaps(v->getprinterhdc(),LOGPIXELSY)) / 25.4;
                if (rh > MAXTEXTHEIGHT) ih = (int) MAXTEXTHEIGHT;
                else if (rh < -MAXTEXTHEIGHT) ih = (int) -MAXTEXTHEIGHT;
                else ih = int(rh);
                if (rw > MAXTEXTHEIGHT) iw = (int) MAXTEXTHEIGHT;
                else if (rw < -MAXTEXTHEIGHT) iw = (int) -MAXTEXTHEIGHT;
                else iw = int(rw);
                if (ih == 0) {
                    if (hfont != 0) {
                        if (metafile)
                            SelectObject(cadwindow->gethdc(),cwoldhfont);
                        else if (drawmode == DM_INVERT)
                            SelectObject(cadwindow->getoffscreenhdc(),cwoldhfont);
                        SelectObject(v->gethdc(),oldhfont);
                        DeleteObject(hfont);
                    }
                    return;
                }
#ifdef _USING_QTCHAR
                lf.lfHeight = options.test(13) ? -ih : ih;
                lf.lfWidth = iw;
                printerhfont = CreateFontIndirectW(&lf);
#else
                lf.lfHeight = options.test(13) ? -ih : ih;
                lf.lfWidth = iw;
                printerhfont = CreateFontIndirect(&lf);
#endif
                printeroldhfont = (RCHFONT) SelectObject(v->getprinterhdc(),printerhfont);
#ifdef _USING_QTCHAR
                GetTextMetricsW(v->getprinterhdc(),&tm);
#else
                GetTextMetrics(v->getprinterhdc(),&tm);
#endif
                tmHeight = tm.tmHeight * ppscale;
                tmDescent = tm.tmDescent * ppscale;
            }
            if (vjust == 0)
                oheight = -nlines * rh + tmDescent - (nlines - 1) * rh * vertspacing;
            else if (vjust == 1)
                oheight = -(nlines + (nlines - 1) * vertspacing) * rh / 2;
            else
                oheight = 0.0;
            p1.x += oheight * sina;
            p1.y += oheight * cosa;
            p3 = p1;
            maxh = int(rh * (nlines + (nlines - 1)*vertspacing));
            maxw = 0;

            for (s = text ; *s!=0 ;  s += ll + 2) {
                for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
                if (metafile) {
#if defined(WIN32)
                    SIZE size;
#ifdef _USING_QTCHAR
                    GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
#else
                    GetTextExtentPoint(cadwindow->gethdc(),(TCHAR*)s,ll,&size);
#endif
                    w = size.cx;
#else
                    long r;
                    r = GetTextExtent(cadwindow->gethdc(),s,ll);
                    w = LOWORD(r);
#endif
                } else if (drawmode == DM_INVERT) {
#if defined(WIN32)
                    SIZE size;
                    SelectObject(cadwindow->getoffscreenhdc(),hreferencefont);
#ifdef _USING_QTCHAR
                    //GetTextExtentPoint32(cadwindow->getoffscreenhdc(),(TCHAR*)s,ll,&size);
                    GetTextExtentPoint32W(cadwindow->getoffscreenhdc(),s,ll,&size);
#else
                    GetTextExtentPoint(cadwindow->getoffscreenhdc(),(TCHAR*)s,ll,&size);
#endif
                    SelectObject(cadwindow->getoffscreenhdc(),hfont);
                    rtw = size.cx * rh / 100.0;
                    w = (int) rtw;

#else
                    long r;
                    r = GetTextExtent(cadwindow->getoffscreenhdc(),s,ll);
                    w = LOWORD(r);
#endif
                } else {
                    SIZE size;
                    SelectObject(v->gethdc(),hreferencefont);
#ifdef _USING_QTCHAR
                    //GetTextExtentPoint32(v->gethdc(),(TCHAR*)s,ll,&size);
                    GetTextExtentPoint32W(v->gethdc(), s,ll,&size);
#else
                    GetTextExtentPoint(v->gethdc(),(TCHAR*)s,ll,&size);
#endif
                    SelectObject(v->gethdc(),hfont);
                    rtw = size.cx * rh / 100.0;
                    w = (int) rtw;
                }
                if (w > maxw) maxw = w;
                p2 = p1;
                if (hjust == 1) {
                    p2.x -= rtw * cosa / 2.0;
                    p2.y += rtw * sina / 2.0;
                } else if (hjust == 2) {
                    p2.x -= rtw * cosa;
                    p2.y += rtw * sina;
                }


                SelectObject(v->gethdc(),hreferencefont);
                tlen = 0;
                for (i = 0 ; i < ll ; i++) {
#ifdef _USING_QTCHAR
                    GetTextExtentPoint32W(v->gethdc(), s,i+1,&size);
#else
                    GetTextExtentPoint32(v->gethdc(),(TCHAR*)s,i+1,&size);
#endif
                    dx[i] = int(size.cx * rh / 100.0 - tlen);
                    tlen += dx[i];
                }
                SelectObject(v->gethdc(),hfont);

                if (drawmode != DM_INVERT) {
                    double x,y;

                    SetBkMode(v->gethdc(),TRANSPARENT);

                    if (v->gettransform2d()) {
                        SIZE size;
                        x = p2.x;
                        y = p2.y;
#ifdef _USING_QTCHAR
                        //GetTextExtentPoint32(v->gethdc(),(TCHAR*)s,ll,&size);
                        GetTextExtentPoint32W(v->gethdc(), s,ll,&size);
#else
                        GetTextExtentPoint(v->gethdc(),(TCHAR*)s,ll,&size);
#endif
                        SetTextAlign(v->gethdc(),TA_TOP | TA_LEFT);
                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;

                        if (p1d.x + size.cx * 2 > 0.0 && p1d.x - size.cx * 2 < v->getwidth() &&
                                p1d.y + size.cy * 2 > 0.0 && p1d.y - size.cy * 2 < v->getheight())
#ifdef _USING_QTCHAR
                            ExtTextOutW(v->gethdc(),int(x),int(y),0,0,s,ll,dx);

#else
                            ExtTextOut(v->gethdc(),int(x),int(y),0,0,(TCHAR*)s,ll,dx);
#endif
                    } else {
                        SIZE size;
#ifdef _USING_QTCHAR
                        if (metafile)
                            GetTextExtentPoint32W(cadwindow->gethdc(), s,ll,&size);
                        else
                            //GetTextExtentPoint(v->gethdc(),(TCHAR*)s,ll,&size);
                            GetTextExtentPoint32W(v->gethdc(), s,ll,&size);
#else
                        if (metafile)
                            GetTextExtentPoint(cadwindow->gethdc(),(TCHAR*)s,ll,&size);
                        else
                            GetTextExtentPoint32(v->gethdc(),(TCHAR*)s,ll,&size);
#endif
                        SetTextAlign(v->gethdc(),TA_TOP | TA_LEFT);

                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;

                        if (p2.x + size.cx > 0.0 && p2.x - size.cx < v->getwidth() &&
                                p2.y + size.cy > 0.0 && p2.y - size.cy < v->getheight()) {
#ifdef _USING_QTCHAR
                            //ExtTextOutW(v->gethdc(),int(p2.x),int(p2.y),0,0,(LPCWSTR)s,ll,dx);
                            ExtTextOutW(v->gethdc(),int(p2.x),int(p2.y),0,0,s,ll,NULL);
#else
                            ExtTextOut(v->gethdc(),int(p2.x),int(p2.y),0,0,(TCHAR*)s,ll,dx);
#endif
                        } else
                            p2.x += 1.0E-30;
                    }
                    SetBkMode(v->gethdc(),OPAQUE);
                } else {
                    SIZE size;

#ifdef _USING_QTCHAR
                    //GetTextExtentPoint32(cadwindow->getoffscreenhdc(),(LPCSTR)s,ll,&size);
                    GetTextExtentPoint32W(cadwindow->getoffscreenhdc(),s,ll,&size);
#else
                    GetTextExtentPoint(cadwindow->getoffscreenhdc(),(TCHAR*)s,ll,&size);
#endif
                    // add a % to the box
                    size.cx = tlen ;//* 4.0;
                    SetBkMode(cadwindow->getoffscreenhdc(),OPAQUE);
                    SetBkColor(cadwindow->getoffscreenhdc(),RGB(0,0,0));
                    SetTextColor(cadwindow->getoffscreenhdc(),RGB(255,255,255));

                    if (v->gettransform2d()) {
                        if (size.cx < size.cy) size.cx = size.cy;
                        if (size.cy < size.cx) size.cy = size.cx;
                    } else
                        p1d = p2;

                    if (p1d.x + size.cx > 0.0 && p1d.x - size.cx < v->getwidth() &&
                            p1d.y + size.cy > 0.0 && p1d.y - size.cy < v->getheight()) {
                        RECT r;
                        if (size.cx > size.cy)
                            size.cy = size.cx;
                        else
                            size.cx = size.cy;
                        r.left = int(p2.x)-size.cx;
                        r.right = int(p2.x)+size.cy;
                        r.top = int(p2.y)-size.cy;
                        r.bottom = int(p2.y)+size.cy;
                        FillRect(cadwindow->getoffscreenhdc(),&r,(RCHBRUSH)GetStockObject(BLACK_BRUSH));
                        SetTextAlign(cadwindow->getoffscreenhdc(),TA_TOP | TA_LEFT);
#ifdef _USING_QTCHAR
                        //ExtTextOutW(cadwindow->getoffscreenhdc(),int(p2.x),int(p2.y),0,0,(LPCWSTR)s,ll,dx);
                        ExtTextOutW(cadwindow->getoffscreenhdc(),int(p2.x),int(p2.y),0,0,s,ll,NULL);
#else
                        ExtTextOut(cadwindow->getoffscreenhdc(),int(p2.x),int(p2.y),0,0,(TCHAR*)s,ll,dx);
#endif
                        BitBlt(v->gethdc(),int(p2.x)-size.cx,int(p2.y)-size.cy,size.cx*2,size.cy*2,cadwindow->getoffscreenhdc(),int(p2.x)-size.cx,int(p2.y)-size.cy,SRCINVERT);
                    }
                }

                p1.x += rh * (1.0 + vertspacing) * sina;
                p1.y += rh * (1.0 + vertspacing) * cosa;
                if (s[ll] == 0) break;
            }

            if (hjust == 1) {
                p3.x -= maxw * cosa / 2.0;
                p3.y += maxw * sina / 2.0;
            } else if (hjust == 2) {
                p3.x -= maxw * cosa;
                p3.y += maxw * sina;
            }

            //  Underlined.
            if (drawmode != DM_INVERT && options.test(12) && halo == 0) {
                Point *hp[1];
                Point hp1[4];
                int nvert;

                p3.x += -ih / 10.0 * cosa - ih/10.0 * sina;
                p3.y += +ih / 10.0 * sina - ih/10.0 * cosa;

                v->setup(linestylescale,0.0,drawmode,halocolour,style,weight,0);

                hp1[0] = Point(int(p3.x),v->getheight() - 1.0 - int(p3.y),0.0);
                hp1[1] = Point(int(p3.x + (maxw+ih/5.0) * cosa),v->getheight() - 1.0 - int((p3.y - (maxw+ih/5.0) * sina)),0.0);
                hp1[2] = Point(int(p3.x + (maxw+ih/5.0) * cosa + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((p3.y - (maxw+ih/5.0) * sina + (maxh+ih/5.0) * cosa)),0.0);
                hp1[3] = Point(int(p3.x + (maxh+ih/5.0) * sina),v->getheight() - 1.0 - int((p3.y + (maxh+ih/5.0) * cosa)),0.0);

                nvert = 4;
                hp[0] = hp1;
                v->fill(hp,&nvert,1,1);

                v->setup(linestylescale,0.0,drawmode,colour,style,weight,0);

                p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
                p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;

            }

            if (options.test(8)) {
                double x1,y1,x2,y2;

                p3.x += -ih / 10.0 * cosa - ih/10.0 * sina;
                p3.y += +ih / 10.0 * sina - ih/10.0 * cosa;

                x1 = p3.x;
                y1 = p3.y;
                x2 = p3.x + (maxw+ih/5.0) * cosa;
                y2 = p3.y - (maxw+ih/5.0) * sina;

                if (v->gethwnd() != 0)
                    v->line3d(v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                    MoveToEx(v->gethdc(),int(x1),int(y1),0);
                    LineTo(v->gethdc(),int(x2),int(y2));
                }

                x1 = p3.x + (maxw+ih/5.0) * cosa;
                y1 = p3.y - (maxw+ih/5.0) * sina;
                x2 = p3.x + (maxw+ih/5.0) * cosa + (maxh+ih/5.0) * sina;
                y2 = p3.y - (maxw+ih/5.0) * sina + (maxh+ih/5.0) * cosa;
                if (v->gethwnd() != 0)
                    v->line3d(v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                    MoveToEx(v->gethdc(),int(x1),int(y1),0);
                    LineTo(v->gethdc(),int(x2),int(y2));
                }

                x1 = p3.x + (maxw+ih/5.0) * cosa + (maxh+ih/5.0) * sina;
                y1 = p3.y - (maxw+ih/5.0) * sina + (maxh+ih/5.0) * cosa;
                x2 = p3.x + (maxh+ih/5.0) * sina;
                y2 = p3.y + (maxh+ih/5.0) * cosa;
                if (v->gethwnd() != 0)
                    v->line3d(v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                    MoveToEx(v->gethdc(),int(x1),int(y1),0);
                    LineTo(v->gethdc(),int(x2),int(y2));
                }

                x1 = p3.x + (maxh+ih/5.0) * sina;
                y1 = p3.y + (maxh+ih/5.0) * cosa;
                x2 = p3.x;
                y2 = p3.y;
                if (v->gethwnd() != 0)
                    v->line3d(v->uvtomodel(v->screentouv(Point(x1,v->getheight()-1-y1,0.0))),v->uvtomodel(v->screentouv(Point(x2,v->getheight()-1-y2,0.0))));
                else if (v->clip2d(&x1,&y1,&x2,&y2)) {
                    MoveToEx(v->gethdc(),int(x1),int(y1),0);
                    LineTo(v->gethdc(),int(x2),int(y2));
                }

                p3.x -= -ih / 10.0 * cosa - ih/10.0 * sina;
                p3.y -= +ih / 10.0 * sina - ih/10.0 * cosa;


            }

            if (hfont != NULL) {
                if (metafile)
                    SelectObject(cadwindow->gethdc(),cwoldhfont);
                else if (drawmode == DM_INVERT)
                    SelectObject(cadwindow->getoffscreenhdc(),cwoldhfont);
                oldhfont = (RCHFONT) SelectObject(v->gethdc(),oldhfont);
                DeleteObject(hfont);
                DeleteObject(hreferencefont);
            }

            if (v->getprinterhdc() != 0) {
                //SelectObject(v->getprinterhdc(),printeroldhfont);
                //DeleteObject(printerhfont);
            }

        }
        delete [] dx;
    }
#endif
}

int Text::hideGL(HideImage *image)
{
    if (options.test(10))
    {
        // text is pending
        //draw(image->drawmode == 1 ? DM_PRINT : DM_NORMAL,0,image->getsurface());
    }
    else if (options.test(11))
    {
        //  Always draw the dimension text
        drawGL(image->drawmode == 1 ? DM_PRINT : DM_NORMAL,0,image->getsurface());
    }

    return 1;
}

int Text::hide(HideImage *image)
{
    if (options.test(10))
    {
        // text is pending
        //draw(image->drawmode == 1 ? DM_PRINT : DM_NORMAL,0,image->getsurface());
    }
    else if (options.test(11))
    {
        //  Always draw the dimension text
        draw(image->drawmode == 1 ? DM_PRINT : DM_NORMAL,0,image->getsurface());
    }

    return 1;

#if 0
    Point zero(0.0,0.0,0.0),o,xa,ya,p1,p2,p3,p4,pt1,pt2;
    Transform tr;
    int fbCharSet,ll,i,j,i1,i2,metafile,nlines,status;
    double scale,y,oheight,xmaxh,xmaxw,xw;
    char buffer[GGO_BUF_SIZE];
    RCCHAR *s;
    RCHFONT hfont,oldhfont,cwoldhfont;
    if (options.test(10))
    {
#if ! defined(_WIN32_WCE)
        MAT2 mat2;
#ifdef _USING_QTCHAR
        TEXTMETRICW tm;
#else
        TEXTMETRIC tm;
#endif
        RCGLYPHMETRICS gm;
        TTPOLYGONHEADER *ttph;
        TTPOLYCURVE *ttpc;
        image->getsurface()->setup(linestylescale,0.0,DM_NORMAL,colour,style,weight,0);
        xa = xaxis;
        ya = yaxis;
        tr.rotate(zero,xa.cross(ya),angle * M_PI / 180.0);
        xa = tr.transform(xa);
        ya = tr.transform(ya);
        o = origin;
        mat2.eM11.value = 1;
        mat2.eM11.fract = 0;
        mat2.eM12.value = 0;
        mat2.eM12.fract = 0;
        mat2.eM21.value = 0;
        mat2.eM21.fract = 0;
        mat2.eM22.value = 1;
        mat2.eM22.fract = 0;
        if (options.test(0))
            fbCharSet = SYMBOL_CHARSET;
        else
            fbCharSet = ANSI_CHARSET;
#ifdef _USING_QTCHAR
        hfont = CreateFontW(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                            options.test(7),options.test(6),0,fbCharSet,
                            OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0, font);
#else
        hfont = CreateFont(100,width == 0.0 || height == 0.0 ? 0 : int(100 * width / height),0,0,(options.test(5) ? FW_BOLD :FW_NORMAL),
                           options.test(7),options.test(6),0,fbCharSet,
                           OUT_STROKE_PRECIS,CLIP_STROKE_PRECIS,PROOF_QUALITY,0,RCPRINTABLE(font));
#endif
        metafile = GetDeviceCaps(image->getsurface()->gethdc(),TECHNOLOGY) == DT_METAFILE;
        if (hfont != NULL) {
            oldhfont = (RCHFONT) SelectObject(image->getsurface()->gethdc(),hfont);
            if (metafile)
                cwoldhfont = (RCHFONT) SelectObject(cadwindow->gethdc(),hfont);
        }
#ifdef _USING_QTCHAR
        if (metafile)
            GetTextMetricsW(cadwindow->gethdc(),&tm);
        else
            GetTextMetricsW(image->getsurface()->gethdc(),&tm);
#else
        if (metafile)
            GetTextMetrics(cadwindow->gethdc(),&tm);
        else
            GetTextMetrics(image->getsurface()->gethdc(),&tm);
#endif
        if (tm.tmHeight == 0) tm.tmHeight = 1;
        if (options.test(9))
            scale = image->getsurface()->getplotscale() / double(tm.tmHeight) * height;
        else
            scale = height / double(tm.tmHeight);
        for (s = text,nlines = 1 ; *s!=0 ; s++) nlines += *s == '\r';
        if (! options.test(3) && ! options.test(4))
            oheight = ((nlines - 1) * tm.tmHeight + (nlines - 1) * tm.tmHeight * vertspacing) * scale;
        else if (options.test(3))
            oheight = ((nlines + (nlines - 1) * vertspacing) * tm.tmHeight / 2 - tm.tmHeight + tm.tmDescent) * scale;
        else
            oheight = -(tm.tmHeight - tm.tmDescent) * scale;
        xmaxh = (tm.tmHeight * (nlines + (nlines - 1)*vertspacing)) * scale;
        xmaxw = 0;
        y = 0;
        for (s = text ; *s!=0 ;  s += ll + 2) {
            for (ll = 0 ;  (s[ll] != '\r') && (s[ll] != 0) ; ll++) ;
            if (metafile) {
#if defined(WIN32)
                SIZE size;
#ifdef _USING_QTCHAR
                GetTextExtentPoint32W(cadwindow->gethdc(),s,ll,&size);
#else
                GetTextExtentPoint(cadwindow->gethdc(),(TCHAR*)s,ll,&size);
#endif
                xw = size.cx * scale;
#else
                long r;
                r = GetTextExtent(cadwindow->gethdc(),s,ll);
                xw = LOWORD(r) * scale;
#endif
            } else {
#if defined(WIN32)
                SIZE size;
#ifdef _USING_QTCHAR
                GetTextExtentPoint32W(image->getsurface()->gethdc(),s,ll,&size);
#else
                GetTextExtentPoint(image->getsurface()->gethdc(),(TCHAR*)s,ll,&size);
#endif
                xw = size.cx * scale;
#else
                long r;
                r = GetTextExtent(image->getsurface()->gethdc(),s,ll);
                xw = LOWORD(r) * scale;
#endif
            }
            if (xw > xmaxw) xmaxw = xw;
            if (options.test(1))
                p1.setxyz(-xw / 2.0,oheight + y,0.0);
            else if (options.test(2))
                p1.setxyz(-xw,oheight + y,0.0);
            else
                p1.setxyz(0.0,oheight + y,0.0);
            for (i = 0 ; i < ll ; i++) {
#ifdef _USING_QTCHAR
                if (metafile)
                    status = int(GetGlyphOutlineW(cadwindow->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
                else
                    status = int(GetGlyphOutlineW(image->getsurface()->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
#else
                if (metafile)
                    status = int(GetGlyphOutline(cadwindow->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
                else
                    status = int(GetGlyphOutline(image->getsurface()->gethdc(),RCCHAR2UINT(s[i]),GGO_NATIVE,&gm,GGO_BUF_SIZE,buffer,&mat2));
#endif
                i1 = 0;
                while (i1 < status) {
                    ttph = (TTPOLYGONHEADER *) (buffer + i1);
                    i2 = int(i1 + ttph->cb);
                    i1 += sizeof(TTPOLYGONHEADER);
                    p2 = p4 = o + xa * (p1.x + (ttph->pfxStart.x.value + ttph->pfxStart.x.fract / 65535.0) * scale) +
                              ya * (p1.y + (ttph->pfxStart.y.value + ttph->pfxStart.y.fract / 65535.0) * scale);
                    while (i1 < i2) {
                        ttpc = (TTPOLYCURVE *) (buffer + i1);
                        for (j = 0 ; j < ttpc->cpfx ; j++) {
                            p3 = o + xa * (p1.x + (ttpc->apfx[j].x.value + ttpc->apfx[j].x.fract / 65535.0) * scale) +
                                 ya * (p1.y + (ttpc->apfx[j].y.value + ttpc->apfx[j].y.fract / 65535.0) * scale);
                            image->hide(this,p2,p3);
                            p2 = p3;
                        }
                        i1 += sizeof(TTPOLYCURVE) + (ttpc->cpfx - 1) * sizeof(POINTFX);
                    }
                    image->hide(this,p2,p4);
                }
                p1.x += gm.gmCellIncX * scale;
                p1.y += gm.gmCellIncY * scale;
            }
            y -= (tm.tmHeight * (1.0 + vertspacing)) * scale;
            if (s[ll] == 0) break;
        }
        if (options.test(8)) {
            p1 = o + ya * (oheight + y - tm.tmDescent * scale + (tm.tmHeight * (1.0 + vertspacing)) * scale);
            if (options.test(1))
                p1 = p1 - xa * xmaxw / 2.0;
            else if (options.test(2))
                p1 = p1 - xa * xmaxw;
            p2 = p1 + xa * xmaxw;
            p3 = p2 + ya * xmaxh;
            p4 = p3 - xa * xmaxw;
            image->hide(this,p1,p2);
            image->hide(this,p2,p3);
            image->hide(this,p3,p4);
            image->hide(this,p4,p1);
        }
        if (hfont != NULL) {
            if (metafile)
                SelectObject(cadwindow->gethdc(),cwoldhfont);
            SelectObject(image->getsurface()->gethdc(),oldhfont);
            DeleteObject(hfont);
        }
#endif

    } else if (options.test(11)) {
        //  Always draw the dimension text
        draw(DM_NORMAL,0,image->getsurface());
    }
    return 1;
#endif
}

int Text::npoints(void)
{
    return 1;
}

Point Text::point(int)
{
    return origin;
}

Point Text::nearp(Point)
{
    return origin;
}

int Text::stretchent(Database *db,Transform &t,SortedPointTable *,int erase)
{
    if (! isvisible(0)) return 1;
    if (status & 8) return 1;
    status |= 8;
    if (copye != NULL) return 1;
    if ((status & 1) == 1) {
        if ((status & 16) == 0) draw(DM_ERASE);
        status |= 16;
        return move(db,t,erase);
    }
    return 1;
}

int Text::move(Database *db,Transform &t,int erase)
{
    Point o,xa,ya,za;
    Entity *e;
    double scale,ptoler,cosa,sina;
    int hjust,vjust;

    if (copye != NULL) return 1;
    for (alist.start() ; (e = alist.next()) != NULL ; )
        if ((e->getstatus() & 1) == 0)
            return db->geometry.add(clone(t));
    if (erase) draw(DM_ERASE);
    if (db != 0) {
        db->saveundo(UD_MOVED,this);
        status |= 32;  // Moved
    }
    cadwindow->invalidatedisplaylist(this);
    status |= 1;
    o = t.transform(origin);
    xa = t.transform(xaxis+origin) - o;
    ya = t.transform(yaxis+origin) - o;
    scale = (xa.length() + ya.length()) / 2.0;
    xa.normalize();
    ya.normalize();
    if (scale > 0.999 && scale < 1.001)
        scale = 1.0;  //  If the scale is nearly 1.0, leave it at 1.
    if (! options.test(9)) { // scaled text
        height *= scale;
        //width *= scale; // don't scale the width
    }
    za = xa.cross(ya);
    ya = za.cross(xa);
    if ((xa.cross(ya)).dot(xaxis.cross(yaxis)) < 0.0) {
        //  If the text has been mirrored, need to adjust the justifications

        sina = sin(angle / 180.0 * M_PI);
        cosa = cos(angle / 180.0 * M_PI);

        if ((-xa * sina + ya * cosa).dot(-xaxis * sina + yaxis * cosa) < 0.0) {
            ya = -ya;
            vjust = options.test(3) + options.test(4) * 2;
            vjust = 2 - vjust;
            options.set(3,vjust&1);
            options.set(4,vjust&2);
            za = xa.cross(ya);
            angle = -angle;
        } else if ((xa * cosa + ya * sina).dot(xaxis * cosa + yaxis * sina) < 0.0) {
            xa = -xa;
            hjust = options.test(1) + options.test(2) * 2;
            hjust = 2 - hjust;
            options.set(1,hjust&1);
            options.set(2,hjust&2);
            za = xa.cross(ya);
            angle = -angle;
        }


    }
    ptoler = db != 0 ? db->getptoler() : 0.0001;
    if (xa.length() > ptoler && ya.length() > ptoler) {
        xaxis = xa;
        yaxis = ya;
    }
    origin = o;
    copye = this;
    if (db != 0 && getposition() != 0 && db->MovedCallback != 0)
        db->MovedCallback(this);
    return 1;
}

Entity *Text::clone(Transform &t)
{
    Point o,xa,ya,za;
    double scale;
    int hjust,vjust;
    BitMask op(32);
    double an,cosa,sina;

    if (copye != NULL) return copye;
    o = t.transform(origin);
    xa = t.transform(xaxis+origin) - o;
    ya = t.transform(yaxis+origin) - o;
    scale = (xa.length() + ya.length()) / 2.0;
    if (scale > 0.999 && scale < 1.001)
        scale = 1.0;  //  If the scale is nearly 1.0, leave it at 1.
    za = xa.cross(ya);
    ya = za.cross(xa);
    if (xa.length() < db.getptoler() || ya.length() < db.getptoler())
        return 0;
    xa.normalize();
    ya.normalize();
    op = options;
    an = angle;
    if ((xa.cross(ya)).dot(xaxis.cross(yaxis)) < 0.0) {
        //  If the text has been mirrored, need to adjust the justifications
        sina = sin(angle / 180.0 * M_PI);
        cosa = cos(angle / 180.0 * M_PI);

        if ((-xa * sina + ya * cosa).dot(-xaxis * sina + yaxis * cosa) < 0.0) {
            ya = -ya;
            vjust = options.test(3) + options.test(4) * 2;
            vjust = 2 - vjust;
            op.set(3,vjust&1);
            op.set(4,vjust&2);
            za = xa.cross(ya);
            an = -an;
        } else if ((xa * cosa + ya * sina).dot(xaxis * cosa + yaxis * sina) < 0.0) {
            xa = -xa;
            hjust = options.test(1) + options.test(2) * 2;
            hjust = 2 - hjust;
            op.set(1,hjust&1);
            op.set(2,hjust&2);
            za = xa.cross(ya);
            an = -an;
        }

    }
    copye = new Text(*this,font,o,xa,ya,width*scale,height*scale,vertspacing,an,halocolour,text,label,op);
    if (copye == NULL || ! copye->getdefined()) {
        delete copye;
        copye = NULL;
    }
    copye->cloneattributes(this);
    return copye;
}

int Text::project(Database *db,Transform &t,int)
{
    return db->geometry.add(clone(t));
}

void Text::change(const BitMask &change,const EntityHeader &eh,RCCHAR *f,double w,double h,
                  double s,double a,Byte hc,RCCHAR *t,RCCHAR *l,const BitMask &op)
{
    db.saveundo(UD_CHANGEATTRIBUTES,this);
    if (getposition() != 0)
        draw(DM_ERASE);
    cadwindow->invalidatedisplaylist(this);
    if (change.test(0)) colour = eh.getcolour();
    if (change.test(1)) layer = eh.getlayer();
    if (change.test(2)) style = eh.getstyle();
    if (change.test(3)) weight = eh.getweight();
    if (change.test(4)) {
        delete [] font;
        if ((font = new RCCHAR[RCSTRLEN(f)+1]) != NULL)
            RCSTRCPY(font,f);
        options.set(0,op.test(0));
    }
    if (change.test(8)) height = h;
    if (change.test(9)) width = w;
    if (change.test(10)) angle = a;
    if (change.test(11)) {
        options.set(1,op.test(1));
        options.set(2,op.test(2));
    }
    if (change.test(12)) {
        options.set(3,op.test(3));
        options.set(4,op.test(4));
    }
    if (change.test(13)) vertspacing = s;
    if (change.test(14)) options.set(5,op.test(5));
    if (change.test(15)) options.set(6,op.test(6));
    if (change.test(16)) options.set(7,op.test(7));
    if (change.test(17)) options.set(8,op.test(8));
    if (change.test(18)) options.set(10,op.test(10));
    if (change.test(19)) options.set(9,op.test(9));
    if (change.test(20)) options.set(11,op.test(11));
    if (change.test(21)) options.set(12,op.test(12));
    if (change.test(22)) halocolour = hc;
    if (change.test(23)) options.set(13,op.test(13));
    if (change.test(30)) {
        delete [] label;
        if ((label = new RCCHAR[RCSTRLEN(l)+1]) != NULL)
            RCSTRCPY(label,l);
    }
    if (change.test(31)) {
        delete [] text;
        if ((text = new RCCHAR[RCSTRLEN(t)+1]) != NULL)
            RCSTRCPY(text,t);
    }
    //if (getposition() != 0)
    //    draw(DM_NORMAL);
}

int CALLBACK countfonts(LOGFONT *,TEXTMETRIC *,int fonttype,LPSTR lParam)
{
    int *nfonts = (int *) lParam;
    if (fonttype & TRUETYPE_FONTTYPE)
        (*nfonts)++;
    return 1;
}

int CALLBACK countfontsw(LOGFONTW *,TEXTMETRICW *,int fonttype,LPSTR lParam)
{
    int *nfonts = (int *) lParam;
    if (fonttype & TRUETYPE_FONTTYPE)
        (*nfonts)++;
    return 1;
}

struct SF_Data {
    TCHAR **fonts;
    RCCHAR **rcfonts;
    Byte *charsets;
    int i;
};

int CALLBACK savefonts(LOGFONT *lfont,TEXTMETRIC *,int fonttype,LPSTR lParam)
{
    SF_Data *sfdata = (SF_Data *) lParam;
    if (fonttype & TRUETYPE_FONTTYPE) {
        if ((sfdata->fonts[sfdata->i] = new TCHAR [_tcslen(lfont->lfFaceName)+1]) == NULL)
            return 0;
        _tcscpy(sfdata->fonts[sfdata->i],lfont->lfFaceName);
        sfdata->charsets[sfdata->i] = lfont->lfCharSet;
        sfdata->i++;
    }
    return 1;
}

int CALLBACK savefontsw(LOGFONTW *lfont,TEXTMETRICW *,int fonttype,LPSTR lParam)
{
    SF_Data *sfdata = (SF_Data *) lParam;
    if (fonttype & TRUETYPE_FONTTYPE) {
        if ((sfdata->rcfonts[sfdata->i] = new RCCHAR [wcslen(lfont->lfFaceName)+1]) == NULL)
            return 0;
        RCSTRCPY(sfdata->rcfonts[sfdata->i],(RCCHAR*)lfont->lfFaceName);
        sfdata->charsets[sfdata->i] = lfont->lfCharSet;
        sfdata->i++;
    }
    return 1;
}

int Text::getfonts(RCCHAR *font,int *nfonts,RCCHAR ***fonts,Byte **charsets,int *fontid)
{
#ifdef USING_WIDGETS
    int i;
    *nfonts = 0;
    *fonts = 0;
    *charsets = 0;
    *fontid = 0;

    // get the Qt fonts
    QFontDatabase database;

    *nfonts = database.families().size();
    if (*nfonts == 0) return 0;
    // add 1 font for the *Varies* text
    //*nfonts++;
    if ((*fonts = new RCCHAR *[*nfonts]) == NULL) return 0;
    /*
    if ((*charsets = new Byte[*nfonts]) == NULL)
    {
        delete [] *fonts;  return 0;
    }
    */
    // add the *Varies* text first
    i=0;
    //(*fonts)[i] = new RCCHAR[strlen("*Varies*")+1];
    //RCSTRCPY((*fonts)[i],"*Varies*");
    //i++;
    foreach (const QString &family, database.families()) {
        //qDebug() << "family : " << family;
        (*fonts)[i] = new RCCHAR[family.length()+1];
        RCSTRCPY((*fonts)[i],family.data());
        //qDebug() << "fonts[" << i << "] : " << QString((*fonts)[i]);
        i++;
    }

    *fontid = 0;
    for (i = 0 ; i < *nfonts ; i++) {
        if ((*fonts)[i] == 0) {
            *nfonts = i;
            break;
        }
        if (RCSTRCMP(font,(*fonts)[i]) == 0)
            *fontid = i;
    }

    //qDebug() << "font id fonts[" << *fontid << "] : " << QString((*fonts)[*fontid]);

    return 1;
#else
    struct SF_Data sfdata;
    FONTENUMPROCW fcb;
    int i;
    *nfonts = 0;
    *fonts = 0;
    *charsets = 0;
    *fontid = 0;
    fcb = (FONTENUMPROCW)MakeProcInstance( countfontsw,program->gethinst());
    EnumFontFamiliesW(cadwindow->gethdc(),NULL,fcb,(RCLPARAM)nfonts);
    FreeProcInstance( fcb);
    if (*nfonts == 0) return 0;
    if ((*fonts = new RCCHAR *[*nfonts]) == NULL) return 0;
    if ((*charsets = new Byte[*nfonts]) == NULL) {
        delete [] *fonts;
        return 0;
    }
    for (i = 0 ; i < *nfonts ; i++)
        (*fonts)[i] = 0;
    sfdata.rcfonts = (RCCHAR**)*fonts;
    sfdata.charsets = *charsets;
    sfdata.i = 0;
    fcb = (FONTENUMPROCW)MakeProcInstance( savefontsw,program->gethinst());

    RCINT eff = (RCINT)EnumFontFamiliesW(cadwindow->gethdc(),NULL,fcb,(LPARAM)&sfdata);
    if ( ! eff /*EnumFontFamiliesW(cadwindow->gethdc(),NULL,fcb,(LPARAM)&sfdata)*/ ) {
        for (i = 0 ; i < sfdata.i ; i++)
            delete [] fonts[i];

        delete [] *fonts;
        delete [] *charsets;
        FreeProcInstance( fcb);
        return 0;
    }
    FreeProcInstance( fcb);
    *fontid = 0;
    for (i = 0 ; i < *nfonts ; i++) {
        if ((*fonts)[i] == 0) {
            *nfonts = i;
            break;
        }
        if (RCSTRCMP(font,(*fonts)[i]) == 0)
            *fontid = i;
    }
    return 1;
#endif
}

int Text::process_property(int action,int id,void *data,RCCHAR *text)
{
    BitMask change_mask(32),new_options(32);
    int i,result,cstate;
    double wt;

    RCSTRING vertical_justification_l[4] =  { "Bottom", "Centre", "Top", "?" };
    RCSTRING horizontal_justification_l[4] = { "Left", "Centre", "Right", "?" };

    if (action == PP_GETNUMPROPERTIES)
        id = 10000;

    if (id-- == 0) {
        if (action == PP_GETLABEL) {
            RCSTRCPY(text,"Entity");
            return 1;
        }
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            result = ((Entity *)data)->getcolour();
            cstate = 1001;
            colour_command(&cstate,0,(void **)&result);
            change_mask.set(0,1);
            if (result != ((Entity *)data)->getcolour())
                ((Entity *)data)->change(change_mask,result,0,0,0);
            return 0;
        } else if (action == PP_GETVALUE)
            sprintf(text,"%d",getcolour());
        else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Colour");

    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            result = ((Entity *)data)->getlayer();
            cstate = 1001;
            layer_command(&cstate,0,(void **)&result);
            change_mask.set(1,1);
            if (result != ((Entity *)data)->getlayer())
                ((Entity *)data)->change(change_mask,0,result,0,0);
            return 0;
        } else if (action == PP_GETVALUE)
            sprintf(text,"%d",getlayer());
        else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Layer");
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            result = ((Entity *)data)->getstyle();
            cstate = 1001;
            style_command(&cstate,0,(void **)&result);
            change_mask.set(2,1);
            if (result != ((Entity *)data)->getstyle())
                ((Entity *)data)->change(change_mask,0,0,result,0);
            return 0;
        } else if (action == PP_GETVALUE)
            sprintf(text,"%d",getstyle());
        else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Style");
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            wt = db.lineweights.weighttomm(((Entity *)data)->getweight());
            cstate = 1001;
            weight_command(&cstate,0,(void **)&wt);
            change_mask.set(3,1);
            if (wt != db.lineweights.weighttomm(((Entity *)data)->getweight()))
                ((Entity *)data)->change(change_mask,0,0,0,db.lineweights.mmtoweight(wt));
            return 0;
        } else if (action == PP_GETVALUE)
            sprintf(text,"%.2lf",db.lineweights.weighttomm(getweight()));
        else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Weight");
    }

    if (id-- == 0) {
        if (action == PP_GETLABEL) {
            RCSTRCPY(text,"Text");
            return 1;
        }
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            RCCHAR text[4096];
            Dialog dialog(_RCT("LTextinsert_Dialog"));
            RCSTRCPY(text,Text::text);
            dialog.add(new StringDialogControl(100,text,4096));
            if (dialog.process() == TRUE) {
                change_mask.set(31,1);
                change(change_mask,*this,0,0.0,0.0,0.0,0.0,0,text,0,new_options);
            }
            return 0;
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Text");
        else if (action == PP_GETVALUE)
            RCSTRCPY(text,Text::text);
    }


    if (id-- == 0) {
        if (action == PP_CLICK) {
            int i,nfonts,fontid;
            RCCHAR  **fonts;
            Byte *charsets;
            RCCHAR font[300];

            RCSTRCPY(text,"");
            if (Text::getfonts(font,&nfonts,&fonts,&charsets,&fontid)) {
                for (i = 0 ; i < nfonts ; i++) {
                    if (text[0] == 0)
                        RCSTRCPY(text,fonts[i]);
                    else {
                        RCSTRCAT(text,_RCT("\t"));
                        RCSTRCAT(text,fonts[i]);
                    }
                    delete [] fonts[i];
                }
                delete [] fonts;
                delete [] charsets;
            }

            return 3;   //  Sort the list

        } else if (action == PP_GETVALUE)
            RCSTRCPY(text,font);
        else if (action == PP_SETVALUE) {
            change_mask.set(4,1);
            change(change_mask,*this,text,0.0,0.0,0.0,0.0,0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Font");
    }

    if (id-- == 0) {
        if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETVALUE)
            db.formatnumber(text,height,FormatForEditField);
        else if (action == PP_SETVALUE) {
            Expression e(text);
            double x;
            change_mask.set(8,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0,0.0,x * db.getmainunitscale(),0.0,0.0,0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Height");

    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            RCSTRCPY(text,"No\tYes");
            return 2;
        } else if (action == PP_GETVALUE)
            RCSTRCPY(text,options.test(TextSizeInPoints) ? "Yes" : "No");
        else if (action == PP_SETVALUE) {
            change_mask.set(23,1);
            new_options.set(TextSizeInPoints,RCSTRICMP(text, "Yes") == 0);
            change(change_mask,*this,0,0.0,0.0,0.0,0.0,0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Height in Points");
    }

    if (id-- == 0) {
        if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETVALUE)
            db.formatnumber(text,width,FormatForEditField);
        else if (action == PP_SETVALUE) {
            Expression e(text);
            double x;
            change_mask.set(9,1);
            if (e.evaluate(&x))
                change(change_mask,*this,0,x * db.getmainunitscale(),0.0,0.0,0.0,0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Width");
    }

    if (id-- == 0) {
        if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETVALUE)
            sprintf(text,"%.4lf",angle);
        else if (action == PP_SETVALUE) {
            change_mask.set(10,1);
            change(change_mask,*this,0,0.0,0.0,0.0,RCATOF(text),0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Angle");
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            RCSTRCPY(text,"Left\tCentre\tRight");
            return 2;
        } else if (action == PP_GETVALUE)
            RCSTRCPY(text,&horizontal_justification_l[options.test(TextHorizontalJustification1) + options.test(TextHorizontalJustification2) * 2]);
        else if (action == PP_SETVALUE) {
            change_mask.set(11,1);
            for (i = 0 ; i < 3 ; i++)
                if (RCSTRCMP(text,&horizontal_justification_l[i]) == 0) {
                    new_options.set(TextHorizontalJustification1,i&1);
                    new_options.set(TextHorizontalJustification2,i&2);
                    change(change_mask,*this,0,0,0,0,0,0,0,0,new_options);
                }
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Horz Just");
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            RCSTRCPY(text,"Bottom\tCentre\tTop");
            return 2;
        } else if (action == PP_GETVALUE)
            RCSTRCPY(text,&vertical_justification_l[options.test(TextVerticalJustification1) + options.test(TextVerticalJustification2) * 2]);
        else if (action == PP_SETVALUE) {
            change_mask.set(12,1);
            for (i = 0 ; i < 3 ; i++)
                if (RCSTRCMP(text,&vertical_justification_l[i]) == 0) {
                    new_options.set(TextVerticalJustification1,i&1);
                    new_options.set(TextVerticalJustification2,i&2);
                    change(change_mask,*this,0,0,0,0,0,0,0,0,new_options);
                }
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Vert Just");
    }

    if (id-- == 0) {
        if (action == PP_CLICK)
            return 1;
        else if (action == PP_GETVALUE)
            sprintf(text,"%.4lf",vertspacing,0);
        else if (action == PP_SETVALUE) {
            change_mask.set(13,1);
            change(change_mask,*this,0,0.0,0.0,RCATOF(text),0.0,0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Vert Spacing");
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            RCSTRCPY(text,"No\tYes");
            return 2;
        } else if (action == PP_GETVALUE)
            RCSTRCPY(text,options.test(TextBold) ? "Yes" : "No");
        else if (action == PP_SETVALUE) {
            change_mask.set(14,1);
            new_options.set(TextBold,RCSTRICMP(text,"Yes") == 0);
            change(change_mask,*this,0,0.0,0.0,0.0,0.0,0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Bold");
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            RCSTRCPY(text,"No\tYes");
            return 2;
        } else if (action == PP_GETVALUE)
            RCSTRCPY(text,options.test(TextItalic) ? "Yes" : "No");
        else if (action == PP_SETVALUE) {
            change_mask.set(16,1);
            new_options.set(TextItalic,RCSTRICMP(text,"Yes") == 0);
            change(change_mask,*this,0,0.0,0.0,0.0,0.0,0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Italic");
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            RCSTRCPY(text,"No\tYes");
            return 2;
        } else if (action == PP_GETVALUE)
            RCSTRCPY(text,options.test(TextHaloed) ? "Yes" : "No");
        else if (action == PP_SETVALUE) {
            change_mask.set(21,1);
            new_options.set(TextHaloed,RCSTRICMP(text,"Yes") == 0);
            change(change_mask,*this,0,0.0,0.0,0.0,0.0,0,0,0,new_options);
            return 1;
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Halo");
    }

    if (options.test(TextHaloed) && id-- == 0) {
        if (action == PP_CLICK) {
            result = halocolour;
            cstate = 1001;
            colour_command(&cstate,0,(void **)&result);
            if (result != halocolour) {
                change_mask.set(22,1);
                change(change_mask,*this,0,0.0,0.0,0.0,0.0,result,0,0,new_options);
            }
            return 0;
        } else if (action == PP_GETVALUE)
            sprintf(text,"%d",halocolour);
        else if (action == PP_SETVALUE) {
            change_mask.set(22,1);
            change(change_mask,*this,0,0.0,0.0,0.0,0.0,RCATOI(text),0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Halo Colour");
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            RCSTRCPY(text,"No\tYes");
            return 2;
        } else if (action == PP_GETVALUE )
            RCSTRCPY(text,options.test(TextUnderLined) ? "Yes" : "No");
        else if (action == PP_SETVALUE) {
            change_mask.set(15,1);
            new_options.set(TextUnderLined,RCSTRICMP(text,"Yes") == 0);
            change(change_mask,*this,0,0.0,RCATOF(text),0.0,0.0,0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Underline");
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            RCSTRCPY(text,"No\tYes");
            return 2;
        } else if (action == PP_GETVALUE)
            RCSTRCPY(text,options.test(TextBoxed) ? "Yes" : "No");
        else if (action == PP_SETVALUE) {
            change_mask.set(17,1);
            new_options.set(TextBoxed,RCSTRICMP(text,"Yes") == 0);
            change(change_mask,*this,0,0.0,RCATOF(text),0.0,0.0,0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Boxed");
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            RCSTRCPY(text,"No\tYes");
            return 2;
        } else if (action == PP_GETVALUE)
            RCSTRCPY(text,options.test(TextOutline) ? "Yes" : "No");
        else if (action == PP_SETVALUE) {
            change_mask.set(18,1);
            new_options.set(TextOutline,RCSTRICMP(text,"Yes") == 0);
            change(change_mask,*this,0,0.0,RCATOF(text),0.0,0.0,0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Outline");
    }

    if (id-- == 0) {
        if (action == PP_CLICK) {
            RCSTRCPY(text,"No\tYes");
            return 2;
        } else if (action == PP_GETVALUE)
            RCSTRCPY(text,options.test(TextPaperSized) ? "Yes" : "No");
        else if (action == PP_SETVALUE) {
            change_mask.set(19,1);
            new_options.set(TextPaperSized,RCSTRICMP(text,"Yes") == 0);
            change(change_mask,*this,0,0.0,RCATOF(text),0.0,0.0,0,0,0,new_options);
        } else if (action == PP_GETLABEL)
            RCSTRCPY(text,"Scaled");
    }

    if (action == PP_GETNUMPROPERTIES)
        return 10000 - id;

    return 0;
}




