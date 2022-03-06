
#include "ncwin.h"

int BitMask::nbitsperlong = 32;

BitMask::BitMask(int n)
{int i;
  nbits = short(n);
  nbitsperlong = sizeof(RCLONG) * 8;
  if (nbits <= nbitsperlong)
    {  direct = 1;
       bits = 0;
    }
  else
    {  direct = 0;
       if ((pbits = new RCLONG[(nbits + nbitsperlong - 1) / nbitsperlong]) != NULL)
         {  for (i = 0 ; i < (nbits + nbitsperlong - 1) / nbitsperlong ; i++)
              pbits[i] = 0;
         }
    }
}

BitMask::BitMask(const BitMask &bitmask)
{int i;
  nbitsperlong = sizeof(RCLONG) * 8;
  nbits = bitmask.nbits;
  direct = bitmask.direct;
  if (direct)
    bits = bitmask.bits;
  else
    { if ((pbits = new RCLONG[(nbits + nbitsperlong - 1) / nbitsperlong]) != NULL)
        {  for (i = 0 ; i < (nbits + nbitsperlong - 1) / nbitsperlong ; i++)
              pbits[i] = bitmask.pbits[i];
         }
    }
}

BitMask::~BitMask()
{ if (! direct) delete [] pbits;
}

BitMask& BitMask::operator=(const BitMask &bitmask)
{int i,nlongs1,nlongs2;
  nlongs1 = (nbits + nbitsperlong - 1) / nbitsperlong;
  nlongs2 = (bitmask.nbits + bitmask.nbitsperlong - 1) / bitmask.nbitsperlong;
  if (direct)
    {  if (bitmask.direct)
         bits = bitmask.bits;
       else
         bits = bitmask.pbits[0];
    }
  else
    {  for (i = 0 ; i < nlongs1 ; i++)
         {  if (i < nlongs2)
              {  if (bitmask.direct)
                   pbits[i] = bitmask.bits;
                 else
                   pbits[i] = bitmask.pbits[i];
              }
            else
              pbits[i] = 0;
         }
    }
  return *this;
}

void BitMask::operator&=(const BitMask &bitmask)
{int i,nlongs1,nlongs2;
  nlongs1 = (nbits + nbitsperlong - 1) / nbitsperlong;
  nlongs2 = (bitmask.nbits + bitmask.nbitsperlong - 1) / bitmask.nbitsperlong;
  if (direct)
    {  if (bitmask.direct)
         bits = bits & bitmask.bits;
       else
         bits = bits & bitmask.pbits[0];
    }
  else
    {  for (i = 0 ; i < nlongs1 ; i++)
         {  if (i < nlongs2)
              {  if (bitmask.direct)
                   pbits[i] = pbits[i] & bitmask.bits;
                 else
                   pbits[i] = pbits[i] & bitmask.pbits[i];
              }
            else
              pbits[i] = 0;
         }
    }
}

void BitMask::operator|=(const BitMask &bitmask)
{int i,nlongs1,nlongs2;
  nlongs1 = (nbits + nbitsperlong - 1) / nbitsperlong;
  nlongs2 = (bitmask.nbits + bitmask.nbitsperlong - 1) / bitmask.nbitsperlong;
  if (direct)
    {  if (bitmask.direct)
         bits = bits | bitmask.bits;
       else
         bits = bits | bitmask.pbits[0];
    }
  else
    {  for (i = 0 ; i < nlongs1 ; i++)
         {  if (i < nlongs2)
              {  if (bitmask.direct)
                   pbits[i] = pbits[i] | bitmask.bits;
                 else
                   pbits[i] = pbits[i] | bitmask.pbits[i];
              }
         }
    }
}

void BitMask::set(const int bit)
{ if (bit < 0 || bit >= nbits) return;
  if (direct)
    bits = bits | (1/*L*/ << bit);
  else
    pbits[bit / nbitsperlong] = pbits[bit / nbitsperlong] | (1/*L*/ << (bit % nbitsperlong));
}

void BitMask::set(const int bit,const int value)
{ 
  if (bit < 0 || bit >= nbits) return;
  if (value)
    {  if (direct)
         bits = bits | (1/*L*/ << bit);
       else
         pbits[bit / nbitsperlong] = pbits[bit / nbitsperlong] | (1/*L*/ << (bit % nbitsperlong));
    }
  else
    {  if (direct)
         bits = bits & ~(1/*L*/ << bit);
       else
         pbits[bit / nbitsperlong] = pbits[bit / nbitsperlong] & ~(1/*L*/ << (bit % nbitsperlong));
    }
}

void BitMask::toggle(const int bit)
{ if (bit < 0 || bit >= nbits) return;
  if (direct)
    bits ^= (1/*L*/ << bit);
  else
    pbits[bit / nbitsperlong] ^= (1/*L*/ << (bit % nbitsperlong));
}

void BitMask::clearandset(const int bit,...)
{va_list ap;
 int nextbit;
  clearall();
  if (bit == end_list) return;
  set(bit);
  va_start(ap,bit);
  while ((nextbit = va_arg(ap,int)) != end_list) set(nextbit);
  va_end(ap);
}

void BitMask::setandclear(const int bit,...)
{va_list ap;
 int nextbit;
  setall();
  if (bit == end_list) return;
  clear(bit);
  va_start(ap,bit);
  while ((nextbit = va_arg(ap,int)) != end_list) clear(nextbit);
  va_end(ap);
}

void BitMask::clear(const int bit)
{ if (bit < 0 || bit >= nbits) return;
  if (direct)
    bits = bits & ~(1/*L*/ << bit);
  else
    pbits[bit / nbitsperlong] = pbits[bit / nbitsperlong] & ~(1/*L*/ << (bit % nbitsperlong));
}

int BitMask::test(const int bit) const
{ if (bit < 0 || bit >= nbits)
    return 1;
  if (direct)
    return (bits & (1/*L*/ << bit)) != 0;
  else
    return ((pbits[bit / nbitsperlong] & (1/*L*/ << (bit % nbitsperlong))) != 0);
}

int BitMask::getnbitsset(void)
{long b;
 int i,j,n,nlongs;
  if (direct)
    {  b = bits;
       for (i = 0,n = 0 ; i < nbits ; i++,b >>= 1)
         n += b & 1;
    }
  else
    {  nlongs = (nbits + nbitsperlong - 1) / nbitsperlong;
       for (j = 0,n = 0 ; j < nlongs ; j++)
         {  b = pbits[j];
            for (i = 0 ; i < nbitsperlong ; i++,b >>= 1)
              n += b & 1;
         }
    }
  return n;
}

void BitMask::setall(void)
{int i;
  if (direct)
    bits = -1;//L;
  else
    {  for (i = 0 ; i < (nbits + nbitsperlong - 1) / nbitsperlong ; i++)
         pbits[i] = -1;//L;
    }
}

void BitMask::clearall(void)
{int i;
  if (direct)
    bits = 0;
  else
    {  for (i = 0 ; i < (nbits + nbitsperlong - 1) / nbitsperlong ; i++)
         pbits[i] = 0;
    }
}

int BitMask::save(CadFile *cadfile) const
{long l;
 int i;
  if (! cadfile->write(&nbits)) return 0;
  for (i = 0 ; i < (nbits + nbitsperlong - 1) / nbitsperlong ; i++)
    {  if (direct)
         l = bits;
       else
         l = pbits[i];
       if (! cadfile->write(&l)) return 0;
    }
  return 1;
}

BitMask *BitMask::load(CadFile *cadfile)
{short nbits;
 int i;
  if (! cadfile->read(&nbits)) return NULL;
  BitMask *bitmask = new BitMask(nbits);
  if (bitmask == NULL) return NULL;
  if (! bitmask->getdefined())
    {  delete bitmask;  return NULL;
    }
  for (i = 0 ; i < (bitmask->nbits + bitmask->nbitsperlong - 1) / bitmask->nbitsperlong ; i++)
    {  if (bitmask->direct)
         {  if (! cadfile->read(&bitmask->bits))
              {  delete bitmask;  return 0;
              }
         }
       else
         {  if (! cadfile->read(bitmask->pbits+i))
              {  delete bitmask;  return 0;
              }
         }
    }
  return bitmask;
}

void BitMask::save(char *svalue)
{int i;
  for (i = 0 ; i < nbits ; i++)
    svalue[i] = (char) (test(i) + '0');
  svalue[nbits] = 0;
}

void BitMask::load(char *svalue)
{int i;
  for (i = 0 ; svalue[i] != 0 ; i++)
    set(i,svalue[i] - '0');
}

char *BitMask::saveascii(char *buffer) const
{int i,lword;
 
  strcpy(buffer,"");
  lword = 0;
  for (i = 0 ; i < nbits ; i++)
    {  lword = (lword << 1) | test(i);
       if ((i % 32) == 31)
         {  sprintf(buffer+strlen(buffer),"%8.8lx",lword);
            lword = 0;
         }
    }

  return buffer;

}

void BitMask::loadascii(char *value)
{char text[300],*endptr;
 int i,j,lword;

  for (i = 0 ; i < (int)strlen(value) ; i += 8)
    {  strncpy(text,value + i,8);
       text[8] = 0;
       lword = strtol(text,&endptr,16);
       for (j = 0 ; j < 32 ; j++)
         set((i/8)*32+j,(lword&(0x80000000>>j)) != 0);
    }
    
}

