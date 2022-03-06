class OneReal;
class QChar;
typedef QChar RCCHAR;

class OneReal
  {
    friend class Values;
   private:
     RCCHAR *name;
     double value;
   public:
     OneReal(char *,double);
     OneReal(RCCHAR *,double);
     ~OneReal();
     int save(CadFile *);
     static OneReal *load(CadFile *);
     void save(void);
     void load(void);
  };

class OneBitMask
  {friend class Values;
   private:
     RCCHAR *name;
     BitMask *value;
   public:
     OneBitMask(char *,const BitMask&);
     OneBitMask(RCCHAR *,const BitMask&);
     ~OneBitMask();
     int save(CadFile *);
     static OneBitMask *load(CadFile *);
     void save(void);
     void load(void);
  };

class OneInteger
  {friend class Values;
   private:
     RCCHAR *name;
     int value;
   public:
     OneInteger(char *,int);
     OneInteger(RCCHAR *,int);
     ~OneInteger();
     int save(CadFile *);
     static OneInteger *load(CadFile *);
     void save(void);
     void load(void);
  };

class OneLong
  {friend class Values;
   private:
     RCCHAR *name;
     long value;
   public:
     OneLong(char *,long);
     OneLong(RCCHAR *,long);
     ~OneLong();
     int save(CadFile *);
     static OneLong *load(CadFile *);
     void save(void);
     void load(void);
  };

class OneString
  {friend class Values;
   private:
     RCCHAR *name;
     RCCHAR *value;
   public:
     OneString() { name=0; value=0; }
     OneString(char *,RCCHAR *);
     OneString(RCCHAR *,RCCHAR *);
     ~OneString();
     int save(CadFile *);
     static OneString *load(CadFile *);
     void save(void);
     void load(void);
  };

class OneRCString //: public OneString
  {friend class Values;
   private:
     RCCHAR *name;
     RCCHAR *value;
   public:
     OneRCString(char *,RCCHAR *);
     OneRCString(RCCHAR *,RCCHAR *);
     ~OneRCString();
     int save(CadFile *);
     static OneRCString *load(CadFile *);
     void save(void);
     void load(void);
  };

class Values
  {private:
     GeneralList reals;
     GeneralList integers;
     GeneralList strings;
     GeneralList RCstrings;
     GeneralList longs;
     GeneralList bitmasks;
   public:
     Values(int empty = 0);
     ~Values();
     void addbm(RCCHAR *,const BitMask&);
     void addbm(const char *,const BitMask&);
     BitMask *getbm(RCCHAR *);
     BitMask *getbm(const char *);
     void setbm(RCCHAR *,const BitMask&);
     void setbm(const char *,const BitMask&);
     void addreal(RCCHAR *,double);
     void addreal(const char *,double);
     double getreal(RCCHAR *);
     double getreal(const char *);
     void setreal(RCCHAR *,double);
     void setreal(const char *,double);
     void addinteger(RCCHAR *,int);
     void addinteger(const char *,int);
     int getinteger(RCCHAR *);
     int getinteger(const char *);
     void setinteger(RCCHAR *,int);
     void setinteger(const char *,int);
     void addlong(RCCHAR *,long);
     void addlong(const char *,long);
     long getlong(RCCHAR *);
     long getlong(const char *);
     void setlong(RCCHAR *,long);
     void setlong(const char *,long);
     void addstring(const char *,RCCHAR *);
     void addstring(RCCHAR *,RCCHAR *);
     void setstring(RCCHAR *,RCCHAR *);
     void setstring(const char *,RCCHAR *);
     void setstring(const char *,const char *);
     void addstring(const char *,const char *);
     RCCHAR *getstring(const char *);
     RCCHAR *getstring(const char *, RCCHAR *);
     RCCHAR *getstring(RCCHAR *);
     RCCHAR *getstring(RCCHAR *, RCCHAR *);
     int getfirststring(const char *,RCCHAR *,RCCHAR *);
     int getfirststring(RCCHAR *,RCCHAR *,RCCHAR *);
     int getnextstring(const char *,RCCHAR *,RCCHAR *);
     int getnextstring(RCCHAR *,RCCHAR *,RCCHAR *);
     //
#ifdef _USING_QTCHAR
     //void addstring(char *,RCCHAR *);
     //RCCHAR *getstring(char *, RCCHAR *);
     //int getfirststring(char *,char *,RCCHAR *);
     //int getnextstring(char *,char *,RCCHAR *);
     //void setstring(char *,RCCHAR *);
	 short countStrings();
#endif
	 //
     void clear(void);
     void initialize();
     int save(CadFile *);
     int load(CadFile *,int);
     void save(void);
     // KMJ
     void debug(int flag=0);
  };

