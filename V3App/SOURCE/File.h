#define _SEEK_BEGIN_      d_SEEK_FROM_BEGINNING
#define _SEEK_CURRENT_    d_SEEK_FROM_CURRENT
#define _SEEK_END_        d_SEEK_FROM_EOF

int inFILE_Close(unsigned long *);
int inFILE_OpenReadOnly(unsigned long *, unsigned char *);
int inFILE_Seek(unsigned long , unsigned long , int ) ;
int inFILE_Read(unsigned long *, unsigned char *, unsigned long );

