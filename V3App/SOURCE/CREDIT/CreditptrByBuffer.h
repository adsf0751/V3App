#ifndef CREDITPTRBYBUFFER_H
#define CREDITPTRBYBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include "../../Trans.h"
#include "../../Print.h"
#include "../../PrtMsg.h"
#include "../PRINT/Print.h"
#include "../FUNCTION/Function.h"

typedef struct {
    int inBankLogoHeight; /* 銀行LOGO高度 */
    int inMerchantLogoHeight; /* 商店LOGO高度 */
    int inTitleNameHeight; /* 表頭圖檔高度 */
    int inSloganHeight; /* 企業標語高度 */
    int inCupLegalHeight; /* CUP警語高度 */
    int inInstHeight; /* 分期警語高度 */
    int inNoticeHeight; /* 商店提示與高度 */
} BMPHeight;

BMPHeight gsrBMPHeight;
int inCREDIT_PRINT_MerchantLogo(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, BufferHandle *srBhandle,BMPHeight*	gsrBMPHeight);
int inCREDIT_PRINT_MerchantName(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, BufferHandle *srBhandle,BMPHeight*	gsrBMPHeight);
int inCREDIT_PRINT_Logo_ByBuffer(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle,BMPHeight*	gsrBMPHeight);
int inCREDIT_PRINT_Tidmid_ByBuffer(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle);
int inCREDIT_PRINT_Data_ByBuffer(TRANSACTION_OBJECT *pobTran, unsigned char *uszBuffer, FONT_ATTRIB *srFont_Attrib, BufferHandle *srBhandle);

#ifdef __cplusplus
}
#endif

#endif 