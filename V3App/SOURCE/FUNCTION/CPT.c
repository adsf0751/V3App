#include "../../Trans.h"
#include <stdio.h>
#include <string.h>
#include "CPT.h"
/*
Function        :inGetHostPortNoPrimary
Date&Time       :
Describe        :
*/
extern CPT_REC srCPTRec;
int inGetHostPortNoPrimary(char* szHostPortNoPrimary)
{
        /* 傳進的指標 不得為空  長度需大於0 小於規定最大值 */
        if (szHostPortNoPrimary == NULL || strlen(srCPTRec.szHostPortNoPrimary) <= 0 || strlen(srCPTRec.szHostPortNoPrimary) > 5)
        {
                return (VS_ERROR);
        }
        memcpy(&szHostPortNoPrimary[0], &srCPTRec.szHostPortNoPrimary[0], strlen(srCPTRec.szHostPortNoPrimary));

        return (VS_SUCCESS);
}

/*
Function        :inGetHostIPPrimary
Date&Time       :
Describe        :
*/
int inGetHostIPPrimary(char* szHostIPPrimary)
{
        /* 傳進的指標 不得為空  長度需大於0 小於規定最大值 */
        if (szHostIPPrimary == NULL || strlen(srCPTRec.szHostIPPrimary) <= 0 || strlen(srCPTRec.szHostIPPrimary) > 15)
        {
                return (VS_ERROR);
        }
        memcpy(&szHostIPPrimary[0], &srCPTRec.szHostIPPrimary[0], strlen(srCPTRec.szHostIPPrimary));

        return (VS_SUCCESS);
}
