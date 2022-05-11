//
//  Implementation of the 'Matrix' class
//

#include "ncwin.h"

Vector::Vector(int size) : n(size)
{int    i;
  elements = new double[size];
  defined = 1;
  if (elements == NULL)
    {  defined = 0;  return;
    }
  for (i = 0 ; i < n ; i++)
    elements[i] = 0.0;
}

Vector::Vector(Vector &v) : n(v.n)
{int i;
  elements = new double[n];
  defined = 1;
  if (elements == NULL)
    {  defined = 0;  return;
    }
  for (i = 0 ; i < n ; i++)
    elements[i] = v.elements[i];
}

Vector::~Vector()
{ delete [] elements;
}

Vector &Vector::null(void)
{int i;
  for (i = 0 ; i < n ; i++) elements[i] = 0.0;
  return *this;
}

Vector& Vector::operator=(const Vector&v)
{int i;
  for (i = 0 ; i < n ; i++) elements[i] = v.elements[i];
  return *this;
}

void Vector::operator*=(const double x)
{int i;
  for (i = 0 ; i < n ; i++) elements[i] *= x;
}

void Vector::operator/=(const double x)
{int i;
  for (i = 0 ; i < n ; i++) elements[i] /= x;
}

void Vector::operator+=(const Vector &v)
{int i;
  for (i = 0 ; i < n ; i++) elements[i] += v.elements[i];
}

void Vector::operator-=(const Vector &v)
{int i;
  for (i = 0 ; i < n ; i++) elements[i] -= v.elements[i];
}

void Vector::dump(const char *name)
{char mess[300],one[50];
 int i;
  strcpy(mess,name);
  for (i = 0 ; i < n ; i++)
    {  sprintf(one,"  %.12f ",elements[i]);  strcat(mess,one);
    }
  cadwindow->MessageBox(mess,"",MB_ICONSTOP);
}
/*
 * elements[0] = m(0,0)
 * elements[1] = m(0,1)
 * elements[2] = m(0,2)
 * elements[3] = m(0,3)
 *
 * elements[4] = m(1,0)
 * elements[5] = m(1,1)
 * elements[6] = m(1,2)
 * elements[7] = m(1,3)
 *
 * elements[8] = m(2,0)
 * elements[9] = m(2,1)
 * elements[10]= m(2,2)
 * elements[11]= m(2,3)
 *
 * elements[12]= m(3,0)
 * elements[13]= m(3,1)
 * elements[14]= m(3,2)
 * elements[15]= m(3,3)
 *
 * Row major order
 *
 *     [a00 a01 a02 a03]
 * A = [a10 a11 a12 a13]
 *     [a20 a21 a22 a23]
 *     [a30 a31 a32 a33]
 *
 * [0,0][0,1][0,2][0,3]
 * [1,0][1,1][1,2][1,3]
 * [2,0][2,1][2,2][2,3]
 * [3,0][3,1][3,2][3,3]
 *
 * rotate around x axis
 *
 * [1     0     0     0]
 * [0     cosA  -sinA 0]
 * [0     sinA  cosA  0]
 * [0     0     0     1]
 *
 * rotate around y axis
 *
 * [cosA  0     -sinA 0]
 * [0     1     0     0]
 * [sinA  0     cosA  0]
 * [0     0     0     1]
 *
 * rotate around z axis
 *
 * [cosA  -sinA 0     0]
 * [sinA  cosA  0     0]
 * [0     0     1     0]
 * [0     0     0     1]
 *
 */
Matrix::Matrix(int size) : n(size)
{
#ifdef _RCDEBUG
    gMatrixUseCnt++;
    //qDebug() << "(const) matrix use cnt : " << gMatrixUseCnt;
#endif
    int    i,j;
  elements = new double[size*size];
  defined = 1;
  if (elements == NULL)
    {  defined = 0;  return;
    }
  for (i = 0 ; i < n ; i++)
    for (j = 0 ; j < n ; j++)
      elements[i*n+j] = 0.0;
  for (i = 0 ; i < size ; i++)
    elements[i*n+i] = 1.0;
}

Matrix::Matrix(Matrix &m) : n(m.n)
{
#ifdef _RCDEBUG
    gMatrixUseCnt++;
    //qDebug() << "(const) matrix use cnt : " << gMatrixUseCnt;
#endif
    int    i,j;
  elements = new double[n*n];
  defined = 1;
  if (elements == NULL)
    {  defined = 0;  return;
    }
  for (i = 0 ; i < n ; i++)
    for (j = 0 ; j < n ; j++)
      elements[i*n+j] = m.elements[i*n+j];
}

Matrix::~Matrix()
{
#ifdef _RCDEBUG
    gMatrixUseCnt--;
    //qDebug() << "(dest) matrix use cnt : " << gMatrixUseCnt;
#endif
    delete [] elements;
}

Matrix& Matrix::identity()
{int    i,j;
   for (i = 0 ; i < n ; i++)
     for (j = 0 ; j < n ; j++)
       elements[i*n+j] = 0.0;
   for (i = 0 ; i < n ; i++)
     elements[i*n+i] = 1.0;
   return *this;
}

Matrix& Matrix::null()
{int    i,j;
   for (i = 0 ; i < n ; i++)
     for (j = 0 ; j < n ; j++)
       elements[i*n+j] = 0.0;
   return *this;
}

void Matrix::operator+= (const Matrix& m)
{int    i,j;
  for (i = 0 ; i < n ; i++)
    for (j = 0 ; j < n ; j++)
      elements[i*n+j] += m.elements[i*n+j];
}

void Matrix::operator-= (const Matrix& m)
{int     i,j;
   for (i = 0 ; i < n ; i++)
     for (j = 0 ; j < n ; j++)
       elements[i*n+j] -= m.elements[i*n+j];
}

void Matrix::operator*= (const double s)
{int     i,j;
  for (i = 0 ; i < n ; i++)
    for (j = 0 ; j < n ; j++)
      elements[i*n+j] *= s;
}

void Matrix::operator*= (const Matrix& m)
{int     i,j,k;
 Matrix  tmp(n);
   for (i = 0 ; i < n ; i++)
     for (j = 0 ; j < n ; j++)
       {  tmp.elements[i*n+j] = 0.0;
          for (k = 0 ; k < n ; k++) tmp.elements[i*n+j] += elements[i*n+k] * m.elements[k*n+j];
       }
   for (i = 0 ; i < n * n ; i++)
     elements[i] = tmp.elements[i];
}

void Matrix::operator/= (const double s)
{int     i,j;
  for (i = 0 ; i < n ; i++)
    for (j = 0 ; j < n ; j++)
      elements[i*n+j] /= s;
}

void Matrix::dump(const char *name)
{int i,j;
 FILE *dfile = fopen("debug.log","a");
  for (i = 0 ; i < n ; i++)
    {  fprintf(dfile,"%s[%2d] : ",name,i);
       for (j = 0 ; j < n ; j++)
         fprintf(dfile,"  %.12f ",elements[i*n+j]);
       fprintf(dfile,"\n");
    }
  fclose(dfile);
}

void Matrix::debug()
{
#if 0
  int i,j;
  for (i = 0 ; i < n ; i++)
  {
      qDebug() << "[" << i << "]";
       for (j = 0 ; j < n ; j++)
           qDebug() << elements[i*n+j];
       qDebug();
  }
#else
    int i,j;
      for (i = 0 ; i < n ; i++)
      {
          QString buffer;
        for (j = 0 ; j < n ; j++)
        {
            buffer += QString("[%1]").arg(elements[i*n+j]);
            //qDebug("[%f]",elements[i*n+j]);
        }
        qDebug() << buffer;
      }
      qDebug();

#endif
}

Matrix& Matrix::transpose(void)
{int i,j;
 double t;
  for (i = 0 ; i < n ; i++)
    for (j = i+1 ; j < n ; j++)
      {  t = elements[j*n+i];
         elements[j*n+i] = elements[i*n+j];
         elements[i*n+j] = t;
      }
  return *this;
}

bool inverseMatrix4x4(const double *m, double *out)
{
     double inv[16], det;
     int i;

     inv[0] = m[5]  * m[10] * m[15] -
              m[5]  * m[11] * m[14] -
              m[9]  * m[6]  * m[15] +
              m[9]  * m[7]  * m[14] +
              m[13] * m[6]  * m[11] -
              m[13] * m[7]  * m[10];

     inv[4] = -m[4]  * m[10] * m[15] +
               m[4]  * m[11] * m[14] +
               m[8]  * m[6]  * m[15] -
               m[8]  * m[7]  * m[14] -
               m[12] * m[6]  * m[11] +
               m[12] * m[7]  * m[10];

     inv[8] = m[4]  * m[9] * m[15] -
              m[4]  * m[11] * m[13] -
              m[8]  * m[5] * m[15] +
              m[8]  * m[7] * m[13] +
              m[12] * m[5] * m[11] -
              m[12] * m[7] * m[9];

     inv[12] = -m[4]  * m[9] * m[14] +
                m[4]  * m[10] * m[13] +
                m[8]  * m[5] * m[14] -
                m[8]  * m[6] * m[13] -
                m[12] * m[5] * m[10] +
                m[12] * m[6] * m[9];

     inv[1] = -m[1]  * m[10] * m[15] +
               m[1]  * m[11] * m[14] +
               m[9]  * m[2] * m[15] -
               m[9]  * m[3] * m[14] -
               m[13] * m[2] * m[11] +
               m[13] * m[3] * m[10];

     inv[5] = m[0]  * m[10] * m[15] -
              m[0]  * m[11] * m[14] -
              m[8]  * m[2] * m[15] +
              m[8]  * m[3] * m[14] +
              m[12] * m[2] * m[11] -
              m[12] * m[3] * m[10];

     inv[9] = -m[0]  * m[9] * m[15] +
               m[0]  * m[11] * m[13] +
               m[8]  * m[1] * m[15] -
               m[8]  * m[3] * m[13] -
               m[12] * m[1] * m[11] +
               m[12] * m[3] * m[9];

     inv[13] = m[0]  * m[9] * m[14] -
               m[0]  * m[10] * m[13] -
               m[8]  * m[1] * m[14] +
               m[8]  * m[2] * m[13] +
               m[12] * m[1] * m[10] -
               m[12] * m[2] * m[9];

     inv[2] = m[1]  * m[6] * m[15] -
              m[1]  * m[7] * m[14] -
              m[5]  * m[2] * m[15] +
              m[5]  * m[3] * m[14] +
              m[13] * m[2] * m[7] -
              m[13] * m[3] * m[6];

     inv[6] = -m[0]  * m[6] * m[15] +
               m[0]  * m[7] * m[14] +
               m[4]  * m[2] * m[15] -
               m[4]  * m[3] * m[14] -
               m[12] * m[2] * m[7] +
               m[12] * m[3] * m[6];

     inv[10] = m[0]  * m[5] * m[15] -
               m[0]  * m[7] * m[13] -
               m[4]  * m[1] * m[15] +
               m[4]  * m[3] * m[13] +
               m[12] * m[1] * m[7] -
               m[12] * m[3] * m[5];

     inv[14] = -m[0]  * m[5] * m[14] +
                m[0]  * m[6] * m[13] +
                m[4]  * m[1] * m[14] -
                m[4]  * m[2] * m[13] -
                m[12] * m[1] * m[6] +
                m[12] * m[2] * m[5];

     inv[3] = -m[1] * m[6] * m[11] +
               m[1] * m[7] * m[10] +
               m[5] * m[2] * m[11] -
               m[5] * m[3] * m[10] -
               m[9] * m[2] * m[7] +
               m[9] * m[3] * m[6];

     inv[7] = m[0] * m[6] * m[11] -
              m[0] * m[7] * m[10] -
              m[4] * m[2] * m[11] +
              m[4] * m[3] * m[10] +
              m[8] * m[2] * m[7] -
              m[8] * m[3] * m[6];

     inv[11] = -m[0] * m[5] * m[11] +
                m[0] * m[7] * m[9] +
                m[4] * m[1] * m[11] -
                m[4] * m[3] * m[9] -
                m[8] * m[1] * m[7] +
                m[8] * m[3] * m[5];

     inv[15] = m[0] * m[5] * m[10] -
               m[0] * m[6] * m[9] -
               m[4] * m[1] * m[10] +
               m[4] * m[2] * m[9] +
               m[8] * m[1] * m[6] -
               m[8] * m[2] * m[5];

     det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

     if (det == 0.0)
         return false;

     det = 1.0 / det;

     for (i = 0; i < 16; i++)
         out[i] = inv[i] * det;
    return true;
}


Matrix& Matrix::inverse(void)
{
    Matrix inv(4);
    inverseMatrix4x4(elements,inv.elements);
    *this = inv;
    return *this;
}

// integer version
int ideterm(int a[4][4],int n) {
  int det=0, p, h, k, i, j, temp[4][4];
  if(n==1) {
    return a[0][0];
  } else if(n==2) {
    det=(a[0][0]*a[1][1]-a[0][1]*a[1][0]);
    return det;
  } else {
    for(p=0;p<n;p++) {
      h = 0;
      k = 0;
      for(i=1;i<n;i++) {
        for( j=0;j<n;j++) {
          if(j==p) {
            continue;
          }
          temp[h][k] = a[i][j];
          k++;
          if(k==n-1) {
            h++;
            k = 0;
          }
        }
      }
      det=det+a[0][p]*pow(-1,p)*ideterm(temp,n-1);
    }
    return det;
  }
}

// double version
double determ(double *a,int n=4)
{
  double det=0;
  int p, h, k, i, j;
  double temp[4*4];
  if(n==1) {
    return a[0];
  } else if(n==2) {
    det=(a[0]*a[5]-a[1]*a[4]);
    return det;
  } else {
    for(p=0;p<n;p++) {
      h = 0;
      k = 0;
      for(i=1;i<n;i++) {
        for( j=0;j<n;j++) {
          if(j==p) {
            continue;
          }
          temp[h*4+k] = a[i*4+j];
          k++;
          if(k==n-1) {
            h++;
            k = 0;
          }
        }
      }
      det=det+a[p]*pow(-1,p)*determ(temp,n-1);
    }
    return det;
  }
}

double Matrix::det(void)
{
    return determ(elements,4);
}

Matrix& Matrix::operator=(const Matrix& m)
{int i,j;
   for (i = 0 ; i < n ; i++)
     for (j = 0 ; j < n ; j++)
       elements[i*n+j] = m.elements[i*n+j];
   return *this;
}

int Matrix::save(CadFile *outfile)
{int i,j;
  for (i = 0 ; i < n ; i++)
    for (j = 0 ; j < n ; j++)
      if (! outfile->write(&elements[i*n+j])) return 0;
  return 1;
}

int Matrix::load(CadFile *infile)
{int i,j;
  for (i = 0 ; i < n ; i++)
    for (j = 0 ; j < n ; j++)
      if (! infile->read(&elements[i*n+j])) return 0;
  return 1;
}

int Matrix::solve(Vector *x,Vector *b,int pivot)
{int i,j,k,imax,jmax,*row,*col;
 double m;
  if (x->n != b->n || n != x->n) return 0;
  row = new int[n];
  if (row == NULL) return 0;
  col = new int[n];
  if (col == NULL)
    {  delete [] row;  return 0;
    }
  for (i = 0 ; i < n ; i++) row[i] = col[i] = i;
  for (k = 0 ; k < n-1 ; k++)
    {  if (pivot)
         {  for (i = k + 1,imax = k ; i < n ; i++)
              if (fabs(elements[row[i]*n+col[k]]) > fabs(elements[row[imax]*n+col[k]])) imax = i;
            i = row[k];  row[k] = row[imax];  row[imax] = i;
            for (j = k + 1,jmax = k ; j < n ; j++)
              if (fabs(elements[row[k]*n+col[j]]) > fabs(elements[row[k]*n+col[jmax]])) jmax = j;
            j = col[k];  col[k] = col[jmax];  col[jmax] = j;
         }
       if (fabs(elements[row[k] * n + col[k]]) < 1.0E-10)
         {  delete [] row;  delete [] col;  return 0;
         }
       for (i = k + 1 ; i < n ; i++)
         {  m = elements[row[i] * n + col[k]] / elements[row[k] * n + col[k]];
            for (j = k + 1 ; j < n ; j++)
              elements[row[i]*n+col[j]] = elements[row[i]*n+col[j]] -
                                         m * elements[row[k]*n+col[j]];
            b->elements[row[i]] = b->elements[row[i]] - m * b->elements[row[k]];
         }
    }
  for (i = n-1 ; i >= 0 ; i--)
    {  x->elements[col[i]] = b->elements[row[i]];
       for (k = i + 1 ; k < n ; k++)
         x->elements[col[i]] -= elements[row[i]*n + col[k]] * x->elements[col[k]];
       if (fabs(elements[row[i]*n + col[i]]) < 1.0E-10)
         {  delete [] row;  delete [] col;  return 0;
         }
       x->elements[col[i]] /= elements[row[i]*n + col[i]];
    }
  delete [] row;  delete [] col;
  return 1;
}

int Matrix::solve(Point *x,Point *b)
{int i,j,k,*row,*col;
 double m;
  row = new int[n];
  if (row == 0) return 0;
  col = new int[n];
  if (col == 0)
    {  delete [] row;  return 0;
    }
  for (i = 0 ; i < n ; i++) row[i] = col[i] = i;
  for (k = 0 ; k < n-1 ; k++)
    {
//       for (i = k + 1,imax = k ; i < n ; i++)
//         if (fabs(elements[row[i]*n+col[k]]) > fabs(elements[row[imax]*n+col[k]])) imax = i;
//      i = row[k];  row[k] = row[imax];  row[imax] = i;
//       for (j = k + 1,jmax = k ; j < n ; j++)
//         if (fabs(elements[row[k]*n+col[j]]) > fabs(elements[row[k]*n+col[jmax]])) jmax = j;
//       j = col[k];  col[k] = col[jmax];  col[jmax] = j;
       if (fabs(elements[row[k] * n + col[k]]) < 1.0E-5)
         {  delete [] row;  delete [] col;  return 0;
         }
       for (i = k + 1 ; i < n ; i++)
         {  m = elements[row[i] * n + col[k]] / elements[row[k] * n + col[k]];
            for (j = k + 1 ; j < n ; j++)
              elements[row[i]*n+col[j]] = elements[row[i]*n+col[j]] -
                                         m * elements[row[k]*n+col[j]];
            b[row[i]].x = b[row[i]].x - m * b[row[k]].x;
            b[row[i]].y = b[row[i]].y - m * b[row[k]].y;
            b[row[i]].z = b[row[i]].z - m * b[row[k]].z;
         }
    }
  for (i = n-1 ; i >= 0 ; i--)
    {  x[row[i]].x = b[row[i]].x;
       x[row[i]].y = b[row[i]].y;
       x[row[i]].z = b[row[i]].z;
       for (k = i + 1 ; k < n ; k++)
         {  x[row[i]].x -= elements[row[i]*n + col[k]] * x[row[k]].x;
            x[row[i]].y -= elements[row[i]*n + col[k]] * x[row[k]].y;
            x[row[i]].z -= elements[row[i]*n + col[k]] * x[row[k]].z;
         }
       if (fabs(elements[row[i]*n + col[i]]) < 1.0E-5)
         {  delete [] row;  delete [] col;  return 0;
         }
       x[row[i]].x /= elements[row[i]*n + col[i]];
       x[row[i]].y /= elements[row[i]*n + col[i]];
       x[row[i]].z /= elements[row[i]*n + col[i]];
    }
  delete [] row;  delete [] col;
  return 1;
}

//
//  The implementation of the 'Transform' class
//

Transform& Transform::identity(void)
{  mat.identity();
   return *this;
}

/*
   m11 = mat(0,0)
   m12 = mat(0,1)
   m13 = mat(0,2)
   m14 = mat(0,3)

   m21 = mat(1,0)
   m22 = mat(1,1)
   m23 = mat(1,2)
   m24 = mat(1,3)

   m31 = mat(2,0)
   m32 = mat(2,1)
   m33 = mat(2,2)
   m34 = mat(2,3)
 */
void Transform::copy(double *m11,double *m12,double *m13,double *m14,
                           double *m21,double *m22,double *m23,double *m24,
                           double *m31,double *m32,double *m33,double *m34)
{  *m11 = mat(0,0);  *m12 = mat(0,1);  *m13 = mat(0,2);  *m14 = mat(0,3);
   *m21 = mat(1,0);  *m22 = mat(1,1);  *m23 = mat(1,2);  *m24 = mat(1,3);
   *m31 = mat(2,0);  *m32 = mat(2,1);  *m33 = mat(2,2);  *m34 = mat(2,3);
}

Point Transform::transform(Point p)
{Point tmp;
  tmp.x = mat(0,0) * p.x + mat(0,1) * p.y + mat(0,2) * p.z + mat(0,3);
  tmp.y = mat(1,0) * p.x + mat(1,1) * p.y + mat(1,2) * p.z + mat(1,3);
  tmp.z = mat(2,0) * p.x + mat(2,1) * p.y + mat(2,2) * p.z + mat(2,3);
  return tmp;
}

Transform& Transform::apply(const Transform &t)
{Transform tmp;
  tmp.mat = t.mat;
  tmp.mat *= mat;
  mat = tmp.mat;
  return *this;
}

Transform& Transform::rotate(Point o,Point a,double angle)
{double len,s,c;
  if ((len = a.length()) < db.getptoler())
    identity();
  else
    {  a /= len;  s = sin(-angle);  c = cos(-angle);
 
       mat(0,0) =  a.x*a.x + c*(1.0-a.x*a.x);
       mat(0,1) =  a.x*a.y - c*a.x*a.y + s*a.z;
       mat(0,2) =  a.x*a.z - c*a.x*a.z - s*a.y;
       mat(0,3) = -mat(0,0)*o.x - mat(0,1)*o.y - mat(0,2)*o.z + o.x;	
       mat(1,0) =  a.y*a.x - c*a.y*a.x - s*a.z;
       mat(1,1) =  a.y*a.y + c*(1.0-a.y*a.y);
       mat(1,2) =  a.y*a.z - c*a.y*a.z + s*a.x;
       mat(1,3) = -mat(1,0)*o.x - mat(1,1)*o.y - mat(1,2)*o.z + o.y;
       mat(2,0) =  a.z*a.x - c*a.z*a.x + s*a.y;
       mat(2,1) =  a.z*a.y - c*a.z*a.y - s*a.x;
       mat(2,2) =  a.z*a.z + c*(1.0-a.z*a.z);
       mat(2,3) = -mat(2,0)*o.x - mat(2,1)*o.y - mat(2,2)*o.z + o.z;
    }
  return *this;
}

Transform& Transform::tocoordinatesystem(Point xa,Point ya,Point za)
{  mat.identity();
   mat(0,0) = xa.x;  mat(0,1) = ya.x;  mat(0,2) = za.x;
   mat(1,0) = xa.y;  mat(1,1) = ya.y;  mat(1,2) = za.y;
   mat(2,0) = xa.z;  mat(2,1) = ya.z;  mat(2,2) = za.z;
   return *this;
}

Transform& Transform::fromcoordinatesystem(Point xa,Point ya,Point za)
{  mat.identity();
   mat(0,0) = xa.x;  mat(0,1) = xa.y;  mat(0,2) = xa.z;
   mat(1,0) = ya.x;  mat(1,1) = ya.y;  mat(1,2) = ya.z;
   mat(2,0) = za.x;  mat(2,1) = za.y;  mat(2,2) = za.z;
   return *this;
}

Transform& Transform::rotate(Point rx,Point ry,Point rz)
{  mat(0,0) = rx.x;  mat(0,1) = rx.y;  mat(0,2) = rx.z;
   mat(1,0) = ry.x;  mat(1,1) = ry.y;  mat(1,2) = ry.z;
   mat(2,0) = rz.x;  mat(2,1) = rz.y;  mat(2,2) = rz.z;
   return *this;
}
/*
*
* rotate around x axis
*
* [1     0     0     0]
* [0     cosA  -sinA 0]
* [0     sinA  cosA  0]
* [0     0     0     1]
*
*/
Transform& Transform::rotatex(double ax)
{  mat.identity();
   mat(1,1) =  cos(ax);
   mat(1,2) = -sin(ax);
   mat(2,1) =  sin(ax);
   mat(2,2) =  cos(ax);
   return *this;
}
/*
*
* rotate around y axis
*
* [cosA  0     -sinA 0]
* [0     1     0     0]
* [sinA  0     cosA  0]
* [0     0     0     1]
*
*/
Transform& Transform::rotatey(double ay)
{  mat.identity();
#if 0
    // the RealCAD method
   mat(0,0) =  cos(ay);
   mat(0,2) =  -sin(ay);
   mat(2,0) =  sin(ay);
   mat(2,2) =  cos(ay);
#else
    // the text book method
    mat(0,0) =  cos(ay);
    mat(0,2) =  sin(ay);
    mat(2,0) =  -sin(ay);
    mat(2,2) =  cos(ay);
#endif
   return *this;
}
/*
*
* rotate around z axis
*
* [cosA  -sinA 0     0]
* [sinA  cosA  0     0]
* [0     0     1     0]
* [0     0     0     1]
*
*/
Transform& Transform::rotatez(double az)
{  mat.identity();
   mat(0,0) =  cos(az);
   mat(0,1) = -sin(az);
   mat(1,0) =  sin(az);
   mat(1,1) =  cos(az);
   return *this;
}

Transform& Transform::translate(Point p)
{  mat.identity();
   mat(0,3) = p.x;
   mat(1,3) = p.y;
   mat(2,3) = p.z;
   return *this;
}

Transform& Transform::translate(double x,double y,double z)
{  mat.identity();
   mat(0,3) = x;
   mat(1,3) = y;
   mat(2,3) = z;
   return *this;
}

Transform& Transform::scale(double x,double y,double z)
{  mat.identity();
   mat(0,0) = x;
   mat(1,1) = y;
   mat(2,2) = z;
   return *this;
}

Transform& Transform::scale(Point o,Point xa,Point ya,Point za,Point s)
{double ox,oy,oz;
   mat.identity();
   ox = -xa.dot(o);  oy = -ya.dot(o);  oz = -za.dot(o);  
   mat(0,0) = s.x * xa.x * xa.x + s.y * ya.x * ya.x + s.z * za.x * za.x;
   mat(0,1) = s.x * xa.x * xa.y + s.y * ya.x * ya.y + s.z * za.x * za.y;
   mat(0,2) = s.x * xa.x * xa.z + s.y * ya.x * ya.z + s.z * za.x * za.z;
   mat(0,3) = s.x * xa.x * ox   + s.y * ya.x * oy   + s.z * za.x * oz + o.x;
   mat(1,0) = s.x * xa.y * xa.x + s.y * ya.y * ya.x + s.z * za.y * za.x;
   mat(1,1) = s.x * xa.y * xa.y + s.y * ya.y * ya.y + s.z * za.y * za.y;
   mat(1,2) = s.x * xa.y * xa.z + s.y * ya.y * ya.z + s.z * za.y * za.z;
   mat(1,3) = s.x * xa.y * ox   + s.y * ya.y * oy   + s.z * za.y * oz + o.y;
   mat(2,0) = s.x * xa.z * xa.x + s.y * ya.z * ya.x + s.z * za.z * za.x;
   mat(2,1) = s.x * xa.z * xa.y + s.y * ya.z * ya.y + s.z * za.z * za.y;
   mat(2,2) = s.x * xa.z * xa.z + s.y * ya.z * ya.z + s.z * za.z * za.z;
   mat(2,3) = s.x * xa.z * ox   + s.y * ya.z * oy   + s.z * za.z * oz + o.z;
   return *this;
}

Transform& Transform::shearz(double a,double b)
{  mat.identity();
   mat(0,2) = a;
   mat(1,2) = b;
   return *this;
}

Transform& Transform::mirror(Point o,Point n)
{double len;
   mat.identity();
   if ((len = n.length()) > db.getptoler())
     {  n /= len;
        mat(0,0) = 1.0-2.0*n.x*n.x;
        mat(0,1) =    -2.0*n.y*n.x;
        mat(0,2) =    -2.0*n.z*n.x;
        mat(0,3) =  2.0*n.x*(o.x*n.x + o.y*n.y + o.z*n.z);
        mat(1,0) =    -2.0*n.x*n.y;
        mat(1,1) = 1.0-2.0*n.y*n.y;
        mat(1,2) =    -2.0*n.z*n.y;
        mat(1,3) =  2.0*n.y*(o.x*n.x + o.y*n.y + o.z*n.z);
        mat(2,0) =    -2.0*n.x*n.z;
        mat(2,1) =    -2.0*n.y*n.z;
        mat(2,2) = 1.0-2.0*n.z*n.z;
        mat(2,3) =  2.0*n.z*(o.x*n.x + o.y*n.y + o.z*n.z);
     };
   return *this;
}

void Transform::dump(const char *name)
{  mat.dump(name);
}

void Transform::debug()
{  mat.debug();
}

int Transform::save(CadFile *outfile)
{ return mat.save(outfile);
}

int Transform::load(CadFile *infile)
{ return mat.load(infile);
}

Transform& Transform::operator=(const Transform& t)
{ mat = t.mat;
  return *this;
}

Transform& Transform::operator*(const Transform& t)
{
    mat *= t.mat;
    return *this;
}

void Transform::copy(const Matrix& m)
{
    mat = m;
}

Point Transform::translation()
{
    Point t(mat(0,3),mat(1,3),mat(2,3));
    return t;
}

Transform Transform::transpose(void)
{
    Transform t;
    t.copy(mat.transpose());
    return t;
}

Transform Transform::inverse(void)
{
    Transform t;
    t.copy(mat.inverse());
    return t;
}

double Transform::det(void)
{
    return mat.det();
}

void Transform::extractEulerAngleXYZ(const Transform& matrix, double& rotXangle, double& rotYangle, double& rotZangle)
{
    Transform mCopy((Transform&)matrix);
    //rotXangle = atan2(-row(1).z, row(2).z);
    rotXangle = atan2(-mCopy.mat(1,2), mCopy(2,2));

    //float cosYangle = sqrt(pow(row(0).x, 2) + pow(row(0).y, 2));
    double cosYangle = sqrt(pow(mCopy.mat(0,0), 2) + pow(mCopy.mat(0,1), 2));
    //rotYangle = atan2(row(0).z, cosYangle);
    rotYangle = atan2(mCopy.mat(0,2), cosYangle);

    double sinXangle = sin(rotXangle);
    double cosXangle = cos(rotXangle);
    //rotZangle = atan2(cosXangle * row(1).x + sinXangle * row(2).x, cosXangle * row(1).y + sinXangle * row(2).y);
    rotZangle = atan2(cosXangle * mCopy.mat(1,0) + sinXangle * mCopy.mat(2,0), cosXangle * mCopy.mat(1,1) + sinXangle * mCopy.mat(2,1));
}

void Transform::DecomposeTransformationMatrix(const Transform& matrix, Point& translation, Point& rotation, Point& scale)
{
    Transform debugMatrix;
    debugMatrix = matrix;
    qDebug() << "in matrix";
    debugMatrix.debug();
    qDebug();

        // Copy the matrix first - we'll use this to break down each component
        Transform  mCopy((Transform&)matrix);
        Transform rotationmat,rotationmatinv;
        // Start by extracting the translation (and/or any projection) from the given matrix
        translation = mCopy.translation();
        for (int i = 0; i < 3; i++) {
            mCopy(i,3) = mCopy(3,i) = 0.0;
        }
        mCopy(3,3) = 1.0;

        debugMatrix = mCopy;
        qDebug()<< "no translation";
        debugMatrix.debug();
        qDebug();

        // Extract the rotation component - this is done using polar decompostion, where
        // we successively average the matrix with its inverse transpose until there is
        // no/a very small difference between successive averages
        double norm;
        int count = 0;
        rotationmat = mCopy;
        //float epsilon = 0.001;
        double epsilon = 0.000001;
        do {
            Transform nextRotation;
            Transform currInvTranspose =
                rotationmat = rotationmat.transpose().inverse();

            // Go through every component in the matrices and find the next matrix
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    nextRotation(i,j) = static_cast<double>(0.5 *
                        (rotationmat(i,j) + currInvTranspose(i,j)));
                }
            }

            norm = 0.0;
            for (int i = 0; i < 3; i++) {
                //float n = static_cast<float>(
                double n = static_cast<double>(
                    fabs(rotationmat(i,0) - nextRotation(i,0)) +
                    fabs(rotationmat(i,1) - nextRotation(i,1)) +
                    fabs(rotationmat(i,2) - nextRotation(i,2)));
                norm = std::max<double>(norm, n);
            }
            rotationmat = nextRotation;
        } while (count < 1000 && norm > epsilon);

        debugMatrix = rotationmat;
        qDebug() << "rotation matrix after polar decomp";
        debugMatrix.debug();
        qDebug();

        Transform rotateTemp = rotationmat;
        rotationmatinv = rotateTemp.inverse();

        debugMatrix = rotationmatinv;
        qDebug() << "rotation matrix inverse";
        debugMatrix.debug();
        qDebug();

        // The scale is simply the removal of the rotation from the non-translated matrix
        Transform scaleMatrix = rotationmatinv * mCopy;
        scale = Point(scaleMatrix(0,0),
            scaleMatrix(1,1),
            scaleMatrix(2,2));
        scale.x = sqrt(scale.x);
        scale.y = sqrt(scale.y);
        scale.z = sqrt(scale.z);

        scaleMatrix.identity();
        Transform t1;
        t1.scale(scale.x,scale.y,scale.z);
        scaleMatrix.apply(t1);

        debugMatrix = scaleMatrix;
        qDebug() << "scale matrix";
        debugMatrix.debug();
        qDebug();

        scaleMatrix = scaleMatrix.inverse();

        debugMatrix = scaleMatrix;
        qDebug() << "scale matrix inverse";
        debugMatrix.debug();
        qDebug();

        Transform dbrotationmat = scaleMatrix * mCopy;

        debugMatrix = dbrotationmat;
        qDebug() << "rotation matrix no scale";
        debugMatrix.debug();
        qDebug();

#if 1
        rotationmat = scaleMatrix * mCopy;

        // Calculate the normalized rotation matrix and take its determinant to determine whether
        // it had a negative scale or not...
        Point row1(rotationmat(0,0), rotationmat(0,1), rotationmat(0,2));
        Point row2(rotationmat(1,0), rotationmat(1,1), rotationmat(1,2));
        Point row3(rotationmat(2,0), rotationmat(2,1), rotationmat(2,2));
        row1.normalize();
        row2.normalize();
        row3.normalize();
        Transform nRotation;

        nRotation.identity();
        nRotation(0,0) = row1.x;	nRotation(0,1) = row1.y;			nRotation(0,2) = row1.z;
        nRotation(1,0) = row2.x;	nRotation(1,1) = row2.y;	        nRotation(1,2) = row2.z;
        nRotation(2,0) = row3.x;	nRotation(2,1) = row3.y;			nRotation(2,2) = row3.z;

        debugMatrix = nRotation;
        qDebug() << "norm rotation";
        debugMatrix.debug();
        qDebug();

        extractEulerAngleXYZ(dbrotationmat,rotation.x,rotation.y,rotation.z);
#endif
#if 0
        // Calculate the normalized rotation matrix and take its determinant to determine whether
        // it had a negative scale or not...
        Point row1(mCopy(0,0), mCopy(0,1), mCopy(0,2));
        Point row2(mCopy(1,0), mCopy(1,1), mCopy(1,2));
        Point row3(mCopy(2,0), mCopy(2,1), mCopy(2,2));
        row1.normalize();
        row2.normalize();
        row3.normalize();
        Transform nRotation;

        nRotation(0,0) = row1.x;	nRotation(0,1) = row1.y;			nRotation(0,2) = row1.z;
        nRotation(1,0) = row2.x;	nRotation(1,1) = row2.y;	        nRotation(1,2) = row2.z;
        nRotation(2,0) = row3.x;	nRotation(2,1) = row3.y;			nRotation(2,2) = row3.z;

        debugMatrix = nRotation;
        qDebug() << "norm rotation matrix";
        debugMatrix.debug();
        qDebug();

        // Special consideration: if there's a single negative scale
        // (all other combinations of negative scales will
        // be part of the rotation matrix), the determinant of the
        // normalized rotation matrix will be < 0.
        // If this is the case we apply an arbitrary negative to one
        // of the component of the scale.
        double determinant = nRotation.det();
        if (determinant < 0.0) {
            scale.x = (scale.x * -1.0);
        }
        if(scale.x < 0 )
            nRotation(0,0) *= -1;

        rotation.y= asin(-nRotation(0,2));
        if (cos(rotation.y) != 0) {
            rotation.x = atan2(nRotation(1,2), nRotation(2,2));
            rotation.z = -atan2(nRotation(0,1), nRotation(0,0));
        } else {
            rotation.x = atan2(-nRotation(2,0), nRotation(1,1));
            rotation.z = 0;
        }
        qDebug() << "rotation : " << rotation.x << "," << rotation.y << "," << rotation.z;
#endif

}
