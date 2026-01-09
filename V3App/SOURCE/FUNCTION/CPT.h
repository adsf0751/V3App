typedef struct
{
        char szCommIndex[2 + 1];                /* 通訊參數索引 */
        char szTPDU[10 + 1];                    /* 60nnnnnnnn */
        char szNII[3 + 1];                      /* 網路識別碼 */
        char szHostTelPrimary[15 + 1];          /* 第一授權撥接電話 */
        char szHostTelSecond[15 + 1];           /* 第二授權撥接電話 */
        char szReferralTel[15 + 1];             /* Call Bank 撥接電話 */
        char szHostIPPrimary[15 + 1];           /* 第一授權主機 IP Address  */
        char szHostPortNoPrimary[5 + 1];        /* 第一授權主機 Port No. */
        char szHostIPSecond[15 + 1];            /* 第二授權主機 IP Address */
        char szHostPortNoSecond[5 + 1];         /* 第二授權主機 Port No. */
        char szTCPHeadFormat[1 + 1];            /* TCP 電文長度之格式。( B=BCD，H=Binary)，預設值= H。 */
        char szCarrierTimeOut[2 + 1];           /* 連線等候時間。(超過等候時間，自動撥第二授權電話或IP Address) */
        char szHostResponseTimeOut[2 + 1];      /* 授權等候時間。(超過等候時間，自動斷線) */
}CPT_REC;
CPT_REC srCPTRec;	/* construct CPT record */

int inGetHostIPPrimary(char*);
int inGetHostIPPrimary(char*);