typedef struct
{
        int inFieldNum; /* Field Number */
        int inFieldType; /* Field Type */
        unsigned char uszDispAscii; /* 是否顯示其 ASCII 字元 */
        int inFieldLen; /* Field Len */
} ISO_FIELD_TYPE_NCCC_ATS_TABLE;

#define _NCCC_ATS_CHECK_BIT_MAP_(x, b)	((x) & (1U<<(b)))

#define _NCCC_ATS_NULL_TX_                  0
#define _NCCC_ATS_MAX_BIT_MAP_CNT_          40
#define _NCCC_ATS_MTI_SIZE_                 2
#define _NCCC_ATS_PCODE_SIZE_               3
#define _NCCC_ATS_TPDU_SIZE_                5
#define _NCCC_ATS_BIT_MAP_SIZE_             8
#define _NCCC_ATS_RRN_SIZE_                 12 /* RRN */
#define _NCCC_ATS_ISO_SEND_                 1536
#define _NCCC_ATS_ISO_RECV_                 1536
#define _NCCC_ATS_ISO_ASC_                  1 /* a */
#define _NCCC_ATS_ISO_BCD_                  2
#define _NCCC_ATS_ISO_NIBBLE_2_             3 /* ..nibble */
#define _NCCC_ATS_ISO_NIBBLE_3_             4 /* ...nibble */
#define _NCCC_ATS_ISO_BYTE_2_               5 /* ..ans */
#define _NCCC_ATS_ISO_BYTE_3_               6 /* ...ans */
#define _NCCC_ATS_ISO_BYTE_2_H_             7 /* ..ans */
#define _NCCC_ATS_ISO_BYTE_3_H_             8 /* ...ans */
#define _NCCC_ATS_ISO_ASC_1_                9 /* a */
#define _NCCC_ATS_ISO_BYTE_1_               10/* ..ans */

#define _FISC_ISSUER_ID_SIZE_               8
#define _FISC_ACCOUNT_SIZE_                 16
#define _FISC_CARD_COMMENT_SIZE_            30/* FISC 備註欄長度*/
#define BYTE unsigned char

int myPackData(BYTE *uszPackBuf);