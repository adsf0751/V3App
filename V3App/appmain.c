
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctosapi.h>
#include <fcntl.h>
#include <pthread.h>
#include "Trans.h"
#include "Print.h"
#include "PrtMsg.h"
#include "SOURCE/INCLUDE/Define_1.h"
#include "SOURCE/FUNCTION/Function.h"
#include "SOURCE/FUNCTION/File.h"
#include "SOURCE/COMM/Ethernet.h"
#include "SOURCE/NCCC/NCCCats.h"
#include "SOURCE/PRINT/Print.h"
#include "SOURCE/FUNCTION/ECR.h"
#include "SOURCE/ECR_Struct.h"
#include "SOURCE/CREDIT/CreditptrByBuffer.h"
#include "SOURCE/FUNCTION/Sqlite.h"
#include "SOURCE/CREDIT/Creditfunc.h"
#include "SOURCE/FUNCTION/Batch.h"
#define _AP_ROOT_PATH_   "./"
#define _SHORT_RECEIPT_U_   "U"
#define _NCCC_TEXT_LOGO_    "財團法人聯合信用卡處理中心"
#define _PRT_12X30_   0x0C1E
#define _SEEK_BEGIN_      d_SEEK_FROM_BEGINNING
#define _SEEK_CURRENT_    d_SEEK_FROM_CURRENT
#define _SEEK_END_        d_SEEK_FROM_EOF
#define d_SEEK_FROM_BEGINNING   0
#define d_SEEK_FROM_CURRENT     1
#define d_SEEK_FROM_EOF         2

#define _EDC_TIMEOUT_           -1  /* 由EDC.dat控制 */
#define AF_INET                 PF_INET
#define	PF_INET                 2	/* IP protocol family.  */
#define SOL_SOCKET              1
#define SO_REUSEADDR            2
#define SOL_TCP                 6	/* TCP level */
#define	TCP_MAXSEG              2	/* Set maximum segment size  */
extern  MY_ECR_DATA myECRTable[43];
extern  char    gszTranDBPath[100 + 1];
unsigned char uszBuffer[PB_CANVAS_X_SIZE * 8 * _BUFFER_MAX_LINE_];
BufferHandle srBhandle;
FONT_ATTRIB srFont_Attrib;
BMPHeight gsrBMPHeight;
char szTableName[30+1] = "NCCC0001";
RTC_NEXSYS  srRTC;
SQLITE_TAG_TABLE TABLE_BATCH_TAG[] = 
{
	{"inTableID"			,"INTEGER"	,"PRIMARY KEY"	,""},	/* Table ID Primary key, sqlite table專用避免PRIMARY KEY重複 */
	{"inCode"			,"INTEGER"	,""		,""},	/* Trans Code */
	{"inOrgCode"			,"INTEGER"	,""		,""},	/* Original Trans Code  */
	{"inPrintOption"    		,"INTEGER"	,""		,""},	/* Print Option Flag */
	{"inHDTIndex"			,"INTEGER"	,""		,""},	/* 紀錄HDTindex */
	{"inCDTIndex"			,"INTEGER"	,""		,""},	/* 紀錄CDTindex */
	{"inCPTIndex"			,"INTEGER"	,""		,""},	/* 紀錄CPTindex */
	{"inTxnResult"			,"INTEGER"	,""		,""},	/* 紀錄交易結果 */
	{"inChipStatus"			,"INTEGER"	,""		,""},	/* 0 NOT_USING_CHIP, 1 EMV_CARD, 2 EMV_EASY_ENTRY_CARD */
	{"inFiscIssuerIDLength"		,"INTEGER"	,""		,""},	/* 金融卡發卡單位代號長度 */
	{"inFiscCardCommentLength"	,"INTEGER"	,""		,""},	/* 金融卡備註欄長度 */
	{"inFiscAccountLength"		,"INTEGER"	,""		,""},	/* 金融卡帳號長度 */
	{"inFiscSTANLength"		,"INTEGER"	,""		,""},	/* 金融卡交易序號長度 */
	{"inESCTransactionCode"		,"INTEGER"	,""		,""},	/* ESC組ISO使用 重新上傳使用 Transaction Code沒辦法存在Batch */
	{"inESCUploadMode"		,"INTEGER"	,""		,""},	/* 標示支不支援ESC */
	{"inESCUploadStatus"		,"INTEGER"	,""		,""},	/* 標示ESC上傳狀態 */
	{"inSignStatus"			,"INTEGER"	,""		,""},	/* 簽名檔狀態(有 免簽 或 Bypass) ESC電文使用 */
	{"inHGCreditHostIndex"		,"INTEGER"	,""		,""},	/* 聯合_HAPPY_GO_信用卡主機 */
	{"inHGCode"			,"INTEGER"	,""		,""},	/* 聯合_HAPPY_GO_交易碼 */
	{"lnTxnAmount"			,"INTEGER"	,""		,""},	/* The transaction amount, such as a SALE */
	{"lnOrgTxnAmount"		,"INTEGER"	,""		,""},	/* The ORG transaction amount, such as a SALE */
	{"lnTipTxnAmount"		,"INTEGER"	,""		,""},	/* The transaction amount, such as a TIP */
	{"lnAdjustTxnAmount"		,"INTEGER"	,""		,""},	/* The transaction amount, such as a ADJUST */
	{"lnTotalTxnAmount"		,"INTEGER"	,""		,""},	/* The transaction amount, such as a TOTAL */
	{"lnOilAmount"			,"INTEGER"	,""		,""},	/* 一般交易使用的加油金 */
	{"lnInvNum"			,"INTEGER"	,""		,""},	/* 調閱編號  */
	{"lnOrgInvNum"			,"INTEGER"	,""		,""},	/* Original 調閱編號  */
	{"lnBatchNum"			,"INTEGER"	,""		,""},	/* Batch Number */
	{"lnOrgBatchNum"    		,"INTEGER"	,""		,""},	/* Original Batch Number */
	{"lnSTANNum"			,"INTEGER"	,""		,""},	/* Stan Number */
	{"lnOrgSTANNum"			,"INTEGER"	,""		,""},	/* Original Stan Number */
	{"lnInstallmentPeriod"		,"INTEGER"	,""		,""},	/* 分期付款_期數 */
	{"lnInstallmentDownPayment"	,"INTEGER"	,""		,""},	/* 分期付款_頭期款 */
	{"lnInstallmentPayment"		,"INTEGER"	,""		,""},	/* 分期付款_每期款 */
	{"lnInstallmentFormalityFee"	,"INTEGER"	,""		,""},	/* 分期付款_手續費 */
	{"lnRedemptionPoints"		,"INTEGER"	,""		,""},	/* 紅利扣抵_扣抵紅利點數 */
	{"lnRedemptionPointsBalance"	,"INTEGER"	,""		,""},	/* 紅利扣抵_剩餘紅利點數 */
	{"lnRedemptionPaidCreditAmount"	,"INTEGER"	,""		,""},	/* 紅利扣抵_支付金額 */
	{"lnHGTransactionType"		,"INTEGER"	,""		,""},	/* 聯合_HAPPY GO_交易類別 */
	{"lnHGPaymentType"		,"INTEGER"	,""		,""},	/* 聯合_HAPPY_GO_支付工具 */
	{"lnHGPaymentTeam"		,"INTEGER"	,""		,""},	/* 聯合_HAPPY_GO_支付工具_主機回_*/
	{"lnHGBalancePoint"		,"INTEGER"	,""		,""},	/* 聯合_HAPPY_GO_剩餘點數 */
	{"lnHGTransactionPoint"		,"INTEGER"	,""		,""},	/* 聯合_HAPPY_GO_交易點數  合計 */
	{"lnHGAmount"			,"INTEGER"	,""		,""},	/* 聯合_HAPPY_GO_扣抵後金額  (商品金額 = lnHGAmount + lnHGRedeemAmt) */
	{"lnHGRedeemAmount"		,"INTEGER"	,""		,""},	/* 聯合_HAPPY_GO_扣抵金額 */
	{"lnHGRefundLackPoint"		,"INTEGER"	,""		,""},	/* 聯合_HAPPY_GO_不足點數 */
	{"lnHGBatchIndex"		,"INTEGER"	,""		,""},	/* 聯合_HAPPY_GO_主機當下批次號碼 */
	{"lnHG_SPDH_OrgInvNum"		,"INTEGER"	,""		,""},	/* HAPPY_GO取消用INV */
	{"lnHGSTAN"			,"INTEGER"	,""		,""},	/* HAPPY_GO STAN */
	{"lnCUPUPlanDiscountedAmount"	,"INTEGER"	,""		,""},	/* 銀聯優計畫折價後金額 */
	{"lnCUPUPlanPreferentialAmount"	,"INTEGER"	,""		,""},	/* 銀聯優計畫優惠金額 */
	{"szAuthCode"			,"BLOB"		,""		,""},	/* Auth Code */
	{"szMPASAuthCode"		,"BLOB"		,""		,""},	/* MPAS Auth Code */
	{"szRespCode"			,"BLOB"		,""		,""},	/* Response Code */
	{"szStoreID"			,"BLOB"		,""		,""},	/* StoreID */
	{"szCardLabel"			,"BLOB"		,""		,""},	/* 卡別  */
	{"szPAN"            		,"BLOB"		,""		,""},	/* 卡號  */
	{"szDate"			,"BLOB"		,""		,""},	/* YYYYMMDD */
	{"szOrgDate"			,"BLOB"		,""		,""},	/* YYYYMMDD */
	{"szTime"			,"BLOB"		,""		,""},	/* HHMMSS */
	{"szOrgTime"			,"BLOB"		,""		,""},	/* HHMMSS */
	{"szCardTime"			,"BLOB"		,""		,""},	/* 晶片卡讀卡時間 , YYYYMMDDHHMMSS */
	{"szRefNo"			,"BLOB"		,""		,""},	/* 序號  */
	{"szExpDate"			,"BLOB"		,""		,""},	/* Expiration date */
	{"szServiceCode"    		,"BLOB"		,""		,""},	/* Service code from track */
	{"szCardHolder"			,"BLOB"		,""		,""},	/* 持卡人名字 */
	{"szAMEX4DBC"			,"BLOB"		,""		,""},		
	{"szFiscIssuerID"		,"BLOB"		,""		,""},	/* 發卡單位代號 */
	{"szFiscCardComment"		,"BLOB"		,""		,""},	/* 金融卡備註欄 */
	{"szFiscAccount"			,"BLOB"		,""		,""},	/* 金融卡帳號 */
	{"szFiscOutAccount"		,"BLOB"		,""		,""},	/* 金融卡轉出帳號 */
	{"szFiscSTAN"			,"BLOB"		,""		,""},	/* 金融卡交易序號 */
	{"szFiscTacLength"		,"BLOB"		,""		,""},	/* 金融卡Tac長度 */
	{"szFiscTac"			,"BLOB"		,""		,""},	/* 金融卡Tac */
	{"szFiscTCC"			,"BLOB"		,""		,""},	/* 端末設備查核碼 */
	{"szFiscMCC"			,"BLOB"		,""		,""},	/* 金融卡MCC */
	{"szFiscRRN"			,"BLOB"		,""		,""},	/* 金融卡調單編號 */
	{"szFiscRefundDate"		,"BLOB"		,""		,""},	/* 金融卡退貨原始交易日期(YYYYMMDD) */
	{"szFiscDateTime"		,"BLOB"		,""		,""},	/* 計算TAC(S2)的交易日期時間 */
	{"szFiscPayDevice"		,"BLOB"		,""		,""},	/* 金融卡付款裝置 1 = 手機 2 = 卡片 */
	{"szFiscMobileDevice"		,"BLOB"		,""		,""},	/* SE 類型，0x05：雲端卡片(Cloud-Based) */
	{"szFiscMobileNFType"		,"BLOB"		,""		,""},	/* 行動金融卡是否需輸入密碼 00不需要 01視情況 02一定要 */
	{"szFiscMobileNFSetting"		,"BLOB"		,""		,""},	/* 近端交易類型設定 0x00：Single Issuer Wallet 0x01：國內Third-Party Wallet 0x02~9：保留 0x0A：其他 */
	{"szInstallmentIndicator"	,"BLOB"		,""		,""},
	{"szRedeemIndicator"		,"BLOB"		,""		,""},
	{"szRedeemSignOfBalance"		,"BLOB"		,""		,""},
	{"szHGCardLabel"			,"BLOB"		,""		,""},	/* HAPPY_GO 卡別 */
	{"szHGPAN"			,"BLOB"		,""		,""},	/* HAPPY_GO Account number */
	{"szHGAuthCode"			,"BLOB"		,""		,""},	/* HAPPY_GO 授權碼 */
	{"szHGRefNo"			,"BLOB"		,""		,""},	/* HAPPY_GO Reference Number */
	{"szHGRespCode"			,"BLOB"		,""		,""},	/* HG Response Code */
	{"szCUP_TN"			,"BLOB"		,""		,""},	/* CUP Trace Number (TN) */
	{"szCUP_TD"			,"BLOB"		,""		,""},	/* CUP Transaction Date (MMDD) */
	{"szCUP_TT"			,"BLOB"		,""		,""},	/* CUP Transaction Time (hhmmss) */
	{"szCUP_RRN"			,"BLOB"		,""		,""},	/* CUP Retrieve Reference Number (CRRN) */
	{"szCUP_STD"			,"BLOB"		,""		,""},	/* CUP Settlement Date(MMDD) Of Host Response */
	{"szCUP_EMVAID"			,"BLOB"		,""		,""},	/* CUP晶片交易存AID帳單列印使用 */
	{"szTranAbbrev"			,"BLOB"		,""		,""},	/* Tran abbrev for reports */
	{"szIssueNumber"			,"BLOB"		,""		,""},
	{"szStore_DREAM_MALL"		,"BLOB"		,""		,""},	/* 存Dream_Mall Account Number And Member ID*/
	{"szDCC_FCNFR"			,"BLOB"		,""		,""},	/* Foreign Currency No. For Rate */
	{"szDCC_AC"			,"BLOB"		,""		,""},	/* Action Code */
	{"szDCC_FCN"			,"BLOB"		,""		,""},	/* Foreign Currency Number */
	{"szDCC_FCA"			,"BLOB"		,""		,""},	/* Foreign Currency Amount */
	{"szDCC_FCMU"			,"BLOB"		,""		,""},	/* Foreign Currency Minor Unit */
	{"szDCC_FCAC"			,"BLOB"		,""		,""},	/* Foreign currcncy Alphabetic Code */
	{"szDCC_ERMU"			,"BLOB"		,""		,""},	/* Exchange Rate Minor Unit */
	{"szDCC_ERV"			,"BLOB"		,""		,""},	/* Exchange Rate Value */
	{"szDCC_IRMU"			,"BLOB"		,""		,""},	/* Inverted Rate Minor Unit */
	{"szDCC_IRV"			,"BLOB"		,""		,""},	/* Inverted Rate Value */
	{"szDCC_IRDU"			,"BLOB"		,""		,""},	/* Inverted Rate Display Unit */
	{"szDCC_MPV"			,"BLOB"		,""		,""},	/* Markup Percentage Value */
	{"szDCC_MPDP"			,"BLOB"		,""		,""},	/* Markup Percentage Decimal Point */
	{"szDCC_CVCN"			,"BLOB"		,""		,""},	/* Commissino Value Currency Number */
	{"szDCC_CVCA"			,"BLOB"		,""		,""},	/* Commission Value Currency Amount */
	{"szDCC_CVCMU"			,"BLOB"		,""		,""},	/* Commission Value Currency Minor Unit */
	{"szDCC_TIPFCA"			,"BLOB"		,""		,""},	/* Tip Foreign Currency Amount */
	{"szDCC_OTD"			,"BLOB"		,""		,""},	/* Original Transaction Date & Time (MMDD) */
	{"szDCC_OTA"			,"BLOB"		,""		,""},	/* Original Transaction Amount */
	{"szProductCode"			,"BLOB"		,""		,""},	/* 產品代碼 */
	{"szAwardNum"			,"BLOB"		,""		,""},	/* 優惠個數 */
	{"szAwardSN"			,"BLOB"		,""		,""},	/* 優惠序號(Award S/N) TID(8Bytes)+YYYYMMDDhhmmss(16 Bytes)，共22Bytes */
	{"szTxnNo"			,"BLOB"		,""		,""},	/* 交易編號 */
	{"szMCP_BANKID"			,"BLOB"		,""		,""},	/* 行動支付標記 金融機構代碼 */
	{"szPayItemCode"			,"BLOB"		,""		,""},	/* 繳費項目代碼 */
	{"szTableTD_Data"		,"BLOB"		,""		,""},	/* Table TD的資料 */
	{"szDFSTraceNum"			,"BLOB"		,""		,""},	/* DFS交易系統追蹤號 */
	{"szCheckNO"			,"BLOB"		,""		,""},	/* 檢查碼，目前FOR SDK使用 */
	{"szEIVI_BANKID"			,"BLOB"		,""		,""},	/* E Invoice vehicle I ssue Bank ID 支援電子發票載具之發卡行代碼 */
	{"szBarCodeData"			,"BLOB"		,""		,""},	/* 掃碼交易資料內容 */
	{"szUnyTransCode"		,"BLOB"		,""		,""},	/* Uny交易碼 */
	{"szUnyMaskedCardNo"		,"BLOB"		,""		,""},	/* 遮掩交易卡號資料(非完整卡號，有遮掩，故另設欄位) */
	{"szEInvoiceHASH"		,"BLOB"		,""		,""},	/* 電子發票HASH 值（發卡銀行若無參加電子發票載具，則此欄位回覆值 為空白 SPACE 。） */
	{"szUnyCardLabelLen"		,"BLOB"		,""		,""},	/* 卡別名稱長度，szMCP_BANKID為Twin卡專用，故另設欄位 */
	{"szUnyCardLabel"		,"BLOB"		,""		,""},	/* 卡別名稱，szMCP_BANKID為Twin卡專用，故另設欄位 */
	{"szFPG_FTC_Invoice"		,"BLOB"		,""		,""},	/* 台塑生醫生技客製化專屬【005】　台亞福懋加油站客製化專屬【006】發票號碼 */
        {"szUPlan_Coupon"        	,"BLOB"		,""		,""},	/* 銀聯優計畫的優惠訊息 */
        {"szUPlan_RemarksInformation"	,"BLOB"		,""		,""},	/* 銀聯優計畫-備註訊息 */
        {"szFlightTicketTransBit"	,"BLOB"		,""		,""},	/* 判斷是否是機票交易 不設定為空值 機票交易為1 非機票交易為2 */
        {"szFlightTicketPDS0523"		,"BLOB"		,""		,""},	/* 出發地機場代碼（PDS 0523）左靠右補空白 */
        {"szFlightTicketPDS0524"		,"BLOB"		,""		,""},	/* 目的地機場代碼（PDS 0524）左靠右補空白 */
        {"szFlightTicketPDS0530"		,"BLOB"		,""		,""},	/* 航班號碼（PDS 0530） */
	{"uszWAVESchemeID"		,"BLOB"		,""		,""},	/* WAVE 使用用於組電文 Field_22 */
	{"uszVOIDBit"			,"BLOB"		,""		,""},	/* 負向交易 */
        {"uszUpload1Bit"			,"BLOB"		,""		,""},	/* Offline交易使用 (原交易advice是否已上傳)*/
        {"uszUpload2Bit"			,"BLOB"		,""		,""},	/* Offline交易使用 (當前交易是否為advice)*/
        {"uszUpload3Bit"			,"BLOB"		,""		,""},	/* Offline交易使用 */
        {"uszReferralBit"		,"BLOB"		,""		,""},	/* ISO Response Code 【01】【02】使用 */
        {"uszOfflineBit"			,"BLOB"		,""		,""},	/* 離線交易 */
        {"uszManualBit"			,"BLOB"		,""		,""},	/* Manual Keyin */
        {"uszNoSignatureBit"		,"BLOB"		,""		,""},	/* 免簽名使用 (免簽名則為true)*/
	{"uszCUPTransBit"		,"BLOB"		,""		,""},	/* 是否為CUP */
	{"uszFiscTransBit"		,"BLOB"		,""		,""},	/* SmartPay交易，是否為金融卡 */
        {"uszContactlessBit"		,"BLOB"		,""		,""},	/* 是否為非接觸式 */
        {"uszEMVFallBackBit"		,"BLOB"		,""		,""},	/* 是否要啟動fallback */
	{"uszInstallmentbit"		,"BLOB"		,""		,""},	/* Installment */
	{"uszRedeemBit"			,"BLOB"		,""		,""},	/* Redemption */
        {"uszForceOnlineBit"		,"BLOB"		,""		,""},	/* 組電文使用 Field_25 Point of Service Condition Code */
        {"uszMail_OrderBit"		,"BLOB"		,""		,""},	/* 組電文使用 Field_25 Point of Service Condition Code */
	{"uszDCCTransBit"		,"BLOB"		,""		,""},	/* 是否為DCC交易 */
	{"uszNCCCDCCRateBit"		,"BLOB"		,""		,""},
	{"uszCVV2Bit"			,"BLOB"		,""		,""},
	{"uszRewardSuspendBit"		,"BLOB"		,""		,""},
	{"uszRewardL1Bit"		,"BLOB"		,""		,""},	/* 要印L1 */
	{"uszRewardL2Bit"		,"BLOB"		,""		,""},	/* 要印L2 */
	{"uszRewardL5Bit"		,"BLOB"		,""		,""},	/* 要印L5 */
	{"uszField24NPSBit"		,"BLOB"		,""		,""},
	{"uszVEPS_SignatureBit"		,"BLOB"		,""		,""},	/* VEPS 免簽名是否成立 */
	{"uszTCUploadBit"		,"BLOB"		,""		,""},	/* TCUpload是否已上傳 */
	{"uszFiscConfirmBit"		,"BLOB"		,""		,""},	/* SmartPay 0220 是否已上傳 */
	{"uszFiscVoidConfirmBit"		,"BLOB"		,""		,""},	/* SmartPay Void 0220 是否已上傳 */
	{"uszPinEnterBit"		,"BLOB"		,""		,""},	/* 此筆交易是否有鍵入密碼(只能確認原交易，若該筆之後的調整或取消不會將此Bit Off) */
	{"uszL2PrintADBit"		,"BLOB"		,""		,""},	/* L2是否印AD，因L2原交易取消要判斷，只好增加 */
	{"uszInstallment"		,"BLOB"		,""		,""},	/* HappyGo分期交易 */
        {"uszRedemption"			,"BLOB"		,""		,""},	/* HappyGo點數兌換 */ 
        {"uszHappyGoSingle"		,"BLOB"		,""		,""},	/* HappyGo交易 */ 
        {"uszHappyGoMulti"		,"BLOB"		,""		,""},	/* HappyGo混合交易 */
	{"uszCLSBatchBit"		,"BLOB"		,""		,""},	/* 是否已接續上傳 */
	{"uszTxNoCheckBit"		,"BLOB"		,""		,""},	/* 商店自存聯卡號遮掩開關 */
	{"uszSpecial00Bit"		,"BLOB"		,""		,""},	/* 特殊卡別參數檔，活動代碼00表示免簽(只紀錄，主要看uszNoSignatureBit) */
	{"uszSpecial01Bit"		,"BLOB"		,""		,""},	/* 特殊卡別參數檔，活動代碼01表示ECR回傳明碼(先決條件ECR卡號遮掩有開才做判斷) */
	{"uszRefundCTLSBit"		,"BLOB"		,""		,""},	/* 用在簽單印(W) 因為送電文contactless bit已OFF轉Manual Key in */
	{"uszMPASTransBit"		,"BLOB"		,""		,""},	/* 標示為小額交易 */
	{"uszMPASReprintBit"		,"BLOB"		,""		,""},	/* 標示該小額交易是否可重印 */
	{"uszMobilePayBit"		,"BLOB"		,""		,""},	/* 判斷是不是行動支付 Table NC */
	{"uszUCARDTransBit"		,"BLOB"		,""		,""},	/* 判斷是否是自由品牌卡 */
	{"uszUnyTransBit"		,"BLOB"		,""		,""},	/* 是否是Uny交易 */
	{"uszESCOrgUploadBit"		,"BLOB"		,""		,""},	/* 用於標明原交易的ESC上傳狀態 */
	{"uszESCTipUploadBit"		,"BLOB"		,""		,""},	/* 用於標明Tip的ESC上傳狀態 */
	{"uszTakaTransBit"		,"BLOB"		,""		,""},	/* 客製化026、021 Taka社員卡交易 */
        {"uszUPlanECRBit"		,"BLOB"		,""		,""},	/* 判斷是否為優計畫ECR規格(用以決定是否回傳hash value) */
	{"uszCUPEMVQRCodeBit"		,"BLOB"		,""		,""},	/* 判斷是否使用銀聯QRCode支付 */
	{"uszUPlanTransBit"		,"BLOB"		,""		,""},	/* 判斷是否為優計畫交易(使用銀聯優惠券) */
	{"uszUpdated"			,"BLOB"		,""		,"DEFAULT 0"},	/* For SQLite使用，pobTran中不存，若設為1則代表該紀錄已不用，初始值設為0 */
	{""},
};
/*
Function        :inCREDIT_PRINT_Receipt_ByBuffer
Date&Time       :2015/8/10 上午 10:24
Describe        :列印信用卡
*/
int inCREDIT_PRINT_Receipt_ByBuffer(TRANSACTION_OBJECT *pobTran)
{
    int inRetVal = VS_ERROR;
    memset(uszBuffer,0x00,sizeof(uszBuffer));
    if ((inRetVal = inCREDIT_PRINT_Logo_ByBuffer(pobTran, uszBuffer, &srFont_Attrib, &srBhandle,&gsrBMPHeight)) != VS_SUCCESS)
    {
        printf("inCREDIT_PRINT_Logo_ByBuffer failed, ret=%d\n",inRetVal);
        return inRetVal;
    }
        
    if ((inRetVal = inCREDIT_PRINT_Tidmid_ByBuffer(pobTran, uszBuffer, &srFont_Attrib, &srBhandle)) != VS_SUCCESS)
    {
        printf("inCREDIT_PRINT_Tidmid_ByBuffer failed, ret=%d\n",inRetVal);
        return inRetVal;
    }    
    if ((inRetVal = inCREDIT_PRINT_Data_ByBuffer(pobTran, uszBuffer, &srFont_Attrib, &srBhandle)) != VS_SUCCESS)
    {
        printf("inCREDIT_PRINT_Data_ByBuffer failed, ret=%d\n",inRetVal);
        return inRetVal;
    }      
    if ((inRetVal = inCREDIT_PRINT_Amount_ByBuffer(pobTran, uszBuffer, &srFont_Attrib, &srBhandle)) != VS_SUCCESS)
    {
        printf("inCREDIT_PRINT_Amount_ByBuffer failed, ret=%d\n",inRetVal);
        return inRetVal;
    }     
    if ((inRetVal = inCREDIT_PRINT_ReceiptEND_ByBuffer(pobTran, uszBuffer, &srFont_Attrib, &srBhandle)) != VS_SUCCESS)
    {
        printf("inCREDIT_PRINT_ReceiptEND_ByBuffer failed, ret=%d\n",inRetVal);
        return inRetVal;
    }                  
    if ((inRetVal = inPRINT_Buffer_OutPut(uszBuffer, &srBhandle)) != VS_SUCCESS)
    {
        printf("inPRINT_Buffer_OutPut failed, ret=%d\n",inRetVal);
        return inRetVal;
    }
    printf("srBhandle current is(%d,%d)\n",srBhandle.inXcurrent,srBhandle.inYcurrent);
    return inRetVal;
}
void vdDisplayPysicalOnline(void)
{
//    int inRetVal = inETHERNET_Open();
//    if (inRetVal != VS_SUCCESS)
//    {
//        printf("inETHERNET_Open Failed\n");
//        return;
//    }
    vdFunc_Display_Ethernet_Status();
//    inRetVal = inETHERNET_Close();
//    if (inRetVal != VS_SUCCESS)
//    {
//        printf("inETHERNET_Close ERR\n");
//    }
}
void *vdThreadDispPysicalOnline(void* argc)
{
    printf("vdThreadDispPysicalOnline START\n");
    int inRetVal = VS_ERROR;
    inRetVal = inETHERNET_Open();
    if (inRetVal != VS_SUCCESS)
    {
        printf("inETHERNET_Open Failed\n");
        return;
    }
    inRetVal = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (inRetVal == VS_ERROR)
    {
        printf("pthread_setcancelstate Fail\n");
        return;
    }
    while(1)
    {   //每10s 更新圖片
        vdDisplayPysicalOnline();
        CTOS_Delay(100000);
    }
//    while(*(int*)stopBit != VS_TRUE)
//    {
//        vdDisplayPysicalOnline();
//        for ( i = 0; i < 100; i++)
//        {   
//            //每10s 更新圖片
//            if (*(int*)stopBit)
//                break;
//            CTOS_Delay(100);
//        } 
//    }
//    if (inETHERNET_Close() != VS_SUCCESS)
//    {
//        printf("inETHERNET_Close ERR\n");
//    }
    printf("vdThreadDispPysicalOnline END\n");
}
/*
Function        :inFunc_Booting_Flow_Print_Image_Initial
Date&Time       :2018/6/5 下午 6:27
Describe        :開機流程列印圖片初始化，為了避免開機更新導致抓不到圖片高度
 */
int inFunc_Booting_Flow_Print_Image_Initial(BMPHeight* gsrBMPHeight) {
    /* 若沒下TMS會沒圖片抓高度，所以不判斷成功或失敗 */
    inPRINT_Buffer_GetHeightFlow(gsrBMPHeight);
    return (VS_SUCCESS);
}
/*
Function        :inBATCH_FuncUpdateTxnRecord_By_Sqlite
Date&Time       :2017/3/14 下午 5:33
Describe        :insert資料到資料庫
*/
int inFuncInsertTxnRecord_By_Sqlite(TRANSACTION_OBJECT *pobTran)
{
	int	inRetVal = VS_SUCCESS;
        SQLITE_ALL_TABLE	srAll;
        inRetVal = inSqlite_Create_Table(gszTranDBPath, szTableName, TABLE_BATCH_TAG);
        if (inRetVal != VS_SUCCESS)
        {
                printf("%s\n", "inBATCH_FuncUpdateTxnRecord_By_Sqlite _TN_BATCH_TABLE_create_fail");
//                inFunc_EDCLock(AT);
        }
        else
        {
             printf("%s\n", "inBATCH_FuncUpdateTxnRecord_By_Sqlite _TN_BATCH_TABLE_create_success");
        }

	memset(&srAll, 0x00, sizeof(SQLITE_ALL_TABLE));
        inRetVal = inSqlite_Table_Link_BRec(pobTran, &srAll, _LS_INSERT_);
        if(inRetVal == VS_SUCCESS)
        {
            inRetVal = inSqlite_Insert_Record(gszTranDBPath, szTableName, &srAll);     
        }
        
//	/* 如果uszUpdate設為1 */
//        if (pobTran->uszUpdateBatchBit == VS_TRUE)
//	{
//		inSqlite_Update_ByInvNum_TranState_Flow(pobTran, _TN_BATCH_TABLE_, pobTran->srBRec.lnOrgInvNum);
//	}

//	/* Insert批次 */
//	inRetVal = inBATCH_Insert_All_Flow(pobTran, _TN_BATCH_TABLE_);
//	if (inRetVal != VS_SUCCESS)
//	{
//                vdUtility_SYSFIN_LogMessage(AT, "inBATCH_FuncUpdateTxnRecord_By_Sqlite _TN_BATCH_TABLE__insert_failed");
//		inFunc_EDCLock(AT);
//	}
//
//	memset(szTableID, 0x00, sizeof(szTableID));
//	/* 抓出TableID */
//	inRetVal = inSqlite_Get_Max_TableID_Flow(pobTran, _TN_BATCH_TABLE_, szTableID);
//	if (inRetVal != VS_SUCCESS)
//	{
//                vdUtility_SYSFIN_LogMessage(AT, "inBATCH_FuncUpdateTxnRecord_By_Sqlite _TN_BATCH_TABLE_get_max_ID_failed");
//		inFunc_EDCLock(AT);
//	}
//	else
//	{
//		if (ginDebug == VS_TRUE)
//		{
//			memset(szDebugMsg, 0x00, sizeof(szDebugMsg));
//			sprintf(szDebugMsg, "inTableID : %d", atoi(szTableID));
//			inLogPrintf(AT, szDebugMsg);
//		}
//		pobTran->inTableID = atoi(szTableID);
//	}
//	
//	/* 算出是否有【EMV】和【Contactless】交易 */
//	if (pobTran->srBRec.inChipStatus != 0			|| 
//	    pobTran->srBRec.uszContactlessBit == VS_TRUE	|| 
//	    pobTran->srBRec.uszEMVFallBackBit == VS_TRUE        ||
//            pobTran->srBRec.uszCUPEMVQRCodeBit == VS_TRUE)
//	{
//		inRetVal = inBATCH_Insert_All_Flow(pobTran, _TN_EMV_TABLE_);
//		if (inRetVal != VS_SUCCESS)
//		{
//                        vdUtility_SYSFIN_LogMessage(AT, "inBATCH_FuncUpdateTxnRecord_By_Sqlite _TN_EMV_TABLE_insert_failed");
//			inFunc_EDCLock(AT);
//		}
//	}
	
	/* 強制同步
	 * Linux 系統中，為了加快資料的讀取速度，寫入硬盤的資料有的時候為了效能，會寫到 filesystem buffer 中，
	 * 這個 buffer 是一塊記憶體空間，如果欲寫入硬碟的資料存此 buffer 中，而系統又突然斷電的話，那資料就會流失!
	 * 可以透sync 指令，將存 buffer 中的資料強制寫入disk中；
	 * 或是在程式中利用fsync() function 
	 * 虹堡文件有提到，這個指令會減少NAND壽命，但為了不掉帳還是要用 
	 * 
	 */
	sync();  
        return (VS_SUCCESS);
}
void vdEthernetMenu(TRANSACTION_OBJECT* pobTran)
{
    int i,inRetVal = VS_ERROR;
    BYTE uszPackBuf[984];
    BYTE uszRecvPacket[_NCCC_ATS_ISO_SEND_ + 1];
    int  inReceiveTimeout = 10;
    int	 inReceiveSize = _COMM_RECEIVE_MAX_LENGTH_;
    char ethernetMenu[][48+1] = {"Config","Connect","TxData","RxData","Export"};
    int  menuItem = sizeof(ethernetMenu) / sizeof(ethernetMenu[0]);
    int inKey = 0;
    CTOS_LCDTClearDisplay();
    inRetVal = inETHERNET_Open();
    if (inRetVal != VS_SUCCESS)
    {
        printf("inETHERNET_Open Failed\n");
        return;
    }
    /*============開啟thread更新網路線狀況============*/
    pthread_t thread;
    if (pthread_create(&thread, NULL, vdThreadDispPysicalOnline, NULL) != 0) 
    {
        printf("pthread_create failed\n");
        return;
    }
    /*============開啟thread更新網路線狀況============*/
    while(1)
    {
        inKey = 0;
        inRetVal = inFunc_GetMenuNum_NewUI(pobTran,ethernetMenu,menuItem,&inKey);
        pthread_cancel(thread);
        if(inRetVal == VS_USER_CANCEL || inRetVal == VS_TIMEOUT)
        {
            if (inETHERNET_END() != VS_SUCCESS)
            {
                printf("inETHERNET_END ERR\n");
            }
            if (inETHERNET_Close() != VS_SUCCESS)
            {
                printf("inETHERNET_Close ERR\n");
            }
            return;
        }
        else
        {
           CTOS_LCDTClearDisplay();//清除原先的選單畫面
           switch(inKey)
            { 
                case 1: 
                {
                    inFunc_Ethernet_Edit(pobTran);
                    /*
                    * inCREDIT_Func_Get_OPT_Amount > inDISP_EnglishFont_Color會改到字型和大小，
                    *  這邊重新呼叫inDISP_Initial()
                    */
                    inDISP_Initial();
                    /*============開啟Ethernet設定============*/
                    inRetVal = inETHERNET_Initial();
                    /*============開啟Ethernet設定============*/  
                    break;
                }
                case 2: 
                {
                    inDISP_PutGraphic(_CONNECTING_, 1, _COORDINATE_Y_LINE_8_1_);
                    CTOS_Delay(1000);
                    CTOS_LCDTClearDisplay();
                    if(inETHERNET_SetConfig() == VS_SUCCESS)
                    {
                        CTOS_LCDTPrintXY(1,1,"Connect Host Success!!!");
                        CTOS_Delay(2000);
                    }
                    else
                    {
                        CTOS_LCDTPrintXY(1, 1, "Connect Host Fail!!!");
                        CTOS_Delay(2000);
                    }
                    break;
                }
                case 3: 
                {
                    memset(uszPackBuf,0x00,sizeof(uszPackBuf));
//                    int inCnt = myPackData(uszPackBuf);
                    int inCnt = mySalePackData(pobTran,uszPackBuf);

                    /*
                     * 原先傳到主機的長度對不起來，原因是inCnt已包含電文前面的長度
                     * 但inETHERNET_Send預設傳入inSendSize 是未包含Message Length，
                     * 所以在此function會自動+2，為了不改動用到function，故再此先讓inSendSize -2
                     */
                    inDISP_PutGraphic(_SEND_, 1, _COORDINATE_Y_LINE_8_1_);
                    CTOS_Delay(1000);
                    CTOS_LCDTClearDisplay();
                    if( inETHERNET_Send(uszPackBuf,inCnt-2,0) == VS_SUCCESS)
                    {
                        CTOS_LCDTPrintXY(1,1,"EthernetTx Success!!!");
                        CTOS_Delay(2000);
                    }
                    else
                    {
                        CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!");
                        CTOS_Delay(2000);
                    }
                    break;
                }
                case 4: 
                {
                    memset(uszRecvPacket,0x00,sizeof(uszRecvPacket));
                    inDISP_PutGraphic(_RECEIVE_, 1, _COORDINATE_Y_LINE_8_1_);
                    CTOS_Delay(1000);
                    inReceiveSize = inETHERNET_Receive(uszRecvPacket,inReceiveSize,inReceiveTimeout);
                    CTOS_LCDTClearDisplay();//清除接收中...圖片，顯示下方文字資訊
                    if(inReceiveSize > 0 )
                    {
                        myUnPackData( uszRecvPacket , inReceiveSize);
                        CTOS_LCDTPrintXY(1,1,"EthernetRx Success!!!");
                        CTOS_Delay(2000);
                    }
                    else
                    {
                       CTOS_LCDTPrintXY(1,1,"EthernetRx Fail!!!");
                       CTOS_Delay(2000); 
                    }
                    break;
                }
               case 5:
               {
                    int inTableCnt = -1;
                    inRetVal = inSqlite_Get_Table_Count(gszTranDBPath,szTableName,&inTableCnt);
                    if(inRetVal == VS_SUCCESS)
                    {
                        if(inTableCnt == 0)
                        {
                            CTOS_LCDTClearDisplay();    
                            CTOS_LCDTPrintXY(1, 1, "No records found");
                            CTOS_Delay(2000);
                        }
                        else
                        {
                            unsigned char szColBuffer[BUFF_SIZE];
                            int  szColLen = 0;
                            unsigned char szValBuffer[BUFF_SIZE];
                            int  szValLen = 0;
                            SQLITE_ALL_TABLE srAll;
                            printf("Get Table Count Successed,inTableCnt is %d\n",inTableCnt);
                            inBATCH_Get_Batch_ByCnt_Enormous_Flow(pobTran,szTableName,_BYCNT_ENORMMOUS_SEARCH_,&srAll);
                            for(i=0; i<inTableCnt; i++)
                            {
                                inRetVal = inBATCH_Get_Batch_ByCnt_Enormous_Flow(pobTran,szTableName,_BYCNT_ENORMMOUS_READ_,&srAll);
                                if(inRetVal == VS_SUCCESS)
                                {
                                    inDISP_PutGraphic(_SEND_, 1, _COORDINATE_Y_LINE_8_1_); 
                                    inSqlite_GetTableData(&srAll,szColBuffer,&szColLen,szValBuffer,&szValLen,INT_TABLE);
//                                    printf("szColBuffer is :\n");
//                                    printf("%s\n",(char*)szColBuffer);
                                    printf("szColLen is %d\n",szColLen);
                                    if(szColLen > 0)
                                    {     
                                        if( inETHERNET_Send(szColBuffer,szColLen,0) != VS_SUCCESS)
                                        {
                                            CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!");        
                                            break;
                                        }
                                        CTOS_Delay(2000);
                                        if( inETHERNET_Send(szValBuffer,szValLen,0) != VS_SUCCESS)
                                        {
                                            CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!");    
                                            break;
                                        }
                                        CTOS_Delay(2000);                                       
                                    }
                                   
                                    inSqlite_GetTableData(&srAll,szColBuffer,&szColLen,szValBuffer,&szValLen,INT64_TABLE);
                                    printf("szColLen is %d\n",szColLen);
                                    if(szColLen > 0)
                                    {
                                        if( inETHERNET_Send(szColBuffer,szColLen,0) != VS_SUCCESS)
                                        {
                                            CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!");
                                            break;
                                        }
                                        CTOS_Delay(2000);
                                        if( inETHERNET_Send(szValBuffer,szValLen,0) != VS_SUCCESS)
                                        {
                                            CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!");    
                                            break;
                                        }
                                        CTOS_Delay(2000);
                                    }
                                        
                                    inSqlite_GetTableData(&srAll,szColBuffer,&szColLen,szValBuffer,&szValLen,CHAR_TABLE);
                                    printf("szColLen is %d\n",szColLen);
                                    if(szColLen > 0)
                                    {
                                        if( inETHERNET_Send(szColBuffer,szColLen,0) != VS_SUCCESS)
                                        {
                                            CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!"); 
                                            break;
                                        }
                                        CTOS_Delay(2000);
                                        if( inETHERNET_Send(szValBuffer,szValLen,0) != VS_SUCCESS)
                                        {
                                            CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!");
                                            break;
                                        }
                                        CTOS_Delay(2000);  
                                    }
                                    
                                    inSqlite_GetTableData(&srAll,szColBuffer,&szColLen,szValBuffer,&szValLen,TEXT_TABLE);
                                    printf("szColLen is %d\n",szColLen);
                                    if(szColLen > 0)
                                    {
                                        if( inETHERNET_Send(szColBuffer,szColLen,0) != VS_SUCCESS)
                                        {
                                            CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!");
                                            break;
                                        }
                                        CTOS_Delay(2000);
                                        if( inETHERNET_Send(szValBuffer,szValLen,0) != VS_SUCCESS)
                                        {
                                            CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!");
                                            break;
                                        }
                                        CTOS_Delay(2000);  
                                    }                                      
                                }
                            }
                            inBATCH_Get_Batch_ByCnt_Enormous_Flow(pobTran,szTableName,_BYCNT_ENORMMOUS_FREE_,&srAll);
//                            CTOS_LCDTClearDisplay();    
//                            CTOS_LCDTPrintXY(1, 1, "Select All Records Success!!!");
//                            CTOS_Delay(2000);
                        }
                    }
                    break;
                }
           }
        }
        CTOS_LCDTClearDisplay();//清除選單選項的畫面
        if (pthread_create(&thread, NULL, vdThreadDispPysicalOnline, NULL) != 0) 
        {
            printf("pthread_create failed\n");
            return;
        }
    }
}
void vdSQLOpMenu(TRANSACTION_OBJECT* pobTran)
{
    int i;
    int inRetVal = VS_ERROR;
    char szMaxTableID[10 + 1] = {0};
    SQLITE_ALL_TABLE srAll;
    char szQuerySql[100+1];
    char sqlMenu[][48+1]={"Insert","ShowOne","ShowAll","DeleteOne"};
    int menuItem = sizeof(sqlMenu)/sizeof(sqlMenu[0]);
    int inKey = 0;

    while(1)
    {
        CTOS_LCDTClearDisplay();
        inKey = 0;
        inRetVal = inFunc_GetMenuNum_NewUI(pobTran,sqlMenu,menuItem,&inKey);
        if(inRetVal == VS_USER_CANCEL || inRetVal == VS_TIMEOUT)
        {
            return;
        }
        else
        {
            switch(inKey)
            { 
                case 1:
                {
                    inRetVal = inFuncInsertTxnRecord_By_Sqlite(pobTran);
                    if (inRetVal == VS_SUCCESS )
                    {
                        CTOS_LCDTClearDisplay();    
                        CTOS_LCDTPrintXY(1, 1, "Insert into Success");
                        CTOS_Delay(2000); 
                    }
                    else
                    {
                        CTOS_LCDTClearDisplay();    
                        CTOS_LCDTPrintXY(1, 1, "Insert into Fail");
                        CTOS_Delay(2000);  
                    }
                    break;
                }
                case 2: 
                { 
                    memset(szMaxTableID, 0x00, sizeof(szMaxTableID));
                    inRetVal = inSqlite_Get_Max_TableID(gszTranDBPath, szTableName, szMaxTableID);
                    if (inRetVal == VS_SUCCESS ) //有抓到table id才會回傳VS_SUCCESS
                    {
                        printf("Max RowID is %s\n",szMaxTableID);
                        if(atoi(szMaxTableID) == 0)
                        {
                            CTOS_LCDTClearDisplay();    
                            CTOS_LCDTPrintXY(1, 1, "No records found");
                            CTOS_Delay(2000);
                        }
                        else
                        {
                            /*============單筆查詢============*/
                            memset(&srAll, 0x00, sizeof(SQLITE_ALL_TABLE));
                            memset(&pobTran->srBRec, 0x00, sizeof(pobTran->srBRec));
                            printf("\nbefore query TxnAmount is %ld\n",pobTran->srBRec.lnTxnAmount);
                            inRetVal = inSqlite_Table_Link_BRec(pobTran, &srAll, _LS_READ_);

                            memset(szQuerySql, 0x00, sizeof(szQuerySql));
                            sprintf(szQuerySql, "SELECT * FROM %s WHERE inTableID = %s ", szTableName,szMaxTableID);

                            inRetVal = inSqlite_Get_Data_By_External_SQL(gszTranDBPath, &srAll, szQuerySql);
                            /*顯示sql結果在監看程式上*/
                            inSqlite_Table_Show(&srAll);
                            printf("\nafter query TxnAmount is %ld\n",pobTran->srBRec.lnTxnAmount);
                            /*============單筆查詢============*/
                            CTOS_LCDTClearDisplay();    
                            CTOS_LCDTPrintXY(1, 1, "Select One Record Success!!!");
                            CTOS_Delay(2000);
                        }
                    }
                    else
                    {
                        CTOS_LCDTClearDisplay();    
                        CTOS_LCDTPrintXY(1, 1, "Select One Record Fail!!!");
                        CTOS_Delay(2000);  
                    }
                    break;   
                }
                case 3: 
                {   
                    int inTableCnt = -1;
                    inRetVal = inSqlite_Get_Table_Count(gszTranDBPath,szTableName,&inTableCnt);
                    if(inRetVal == VS_SUCCESS)
                    {
                        if(inTableCnt == 0)
                        {
                            CTOS_LCDTClearDisplay();    
                            CTOS_LCDTPrintXY(1, 1, "No records found");
                            CTOS_Delay(2000);
                        }
                        else
                        {
                            SQLITE_ALL_TABLE srAll;
                            printf("Get Table Count Successed,inTableCnt is %d\n",inTableCnt);
                            inBATCH_Get_Batch_ByCnt_Enormous_Flow(pobTran,szTableName,_BYCNT_ENORMMOUS_SEARCH_,&srAll);

                            for(i=0; i<inTableCnt; i++)
                            {
                                inRetVal = inBATCH_Get_Batch_ByCnt_Enormous_Flow(pobTran,szTableName,_BYCNT_ENORMMOUS_READ_,&srAll);
                                if(inRetVal == VS_SUCCESS)
                                {
                                    inSqlite_Table_Show(&srAll);
                                }
                            }
                            inBATCH_Get_Batch_ByCnt_Enormous_Flow(pobTran,szTableName,_BYCNT_ENORMMOUS_FREE_,&srAll);
                            CTOS_LCDTClearDisplay();    
                            CTOS_LCDTPrintXY(1, 1, "Select All Records Success!!!");
                            CTOS_Delay(2000);
                        }                           
                    }
                    else
                    {
                        CTOS_LCDTClearDisplay();    
                        CTOS_LCDTPrintXY(1, 1, "Select All Record Fail!!!");
                        CTOS_Delay(2000); 
                    }
                    break;
                }
                case 4: 
                { 
                    int inTableCnt = -1;
                    inRetVal = inSqlite_Get_Table_Count(gszTranDBPath,szTableName,&inTableCnt);
                    if (inRetVal ==VS_SUCCESS)
                    {
                        if(inTableCnt == 0 )
                        {
                            CTOS_LCDTClearDisplay();    
                            CTOS_LCDTPrintXY(1, 1, "No records found");
                            CTOS_Delay(2000);
                        }
                        else
                        {
                            inRetVal = inSqlite_Delete_Record(pobTran,szTableName);
                            if(inRetVal == VS_SUCCESS)
                            {
                                CTOS_LCDTClearDisplay();    
                                CTOS_LCDTPrintXY(1, 1, "Delete Record Success!!!");
                                CTOS_Delay(2000);
                            }
                        }
                            
                    }
                    else
                    {
                        CTOS_LCDTClearDisplay();    
                        CTOS_LCDTPrintXY(1, 1, "Delete Record Fail!!!");
                        CTOS_Delay(2000);  
                    }
                    break;
                }
            }
        }
    }
}
void vdRS232Menu(TRANSACTION_OBJECT* pobTran)
{
    int inRetVal = 0;
    ECR_TABLE   gsrECROb = {.srSetting.uszComPort = d_COM2};
    char rs232Menu[][48+1]={"Config","RxData","TxData"};
    int menuItem = sizeof(rs232Menu)/sizeof(rs232Menu[0]);
    int inKey = 0;
    CTOS_LCDTClearDisplay();
    while(1)
    {
        inKey = 0;
        inRetVal = inFunc_GetMenuNum_NewUI(pobTran,rs232Menu,menuItem,&inKey);

        if(inRetVal == VS_USER_CANCEL || inRetVal == VS_TIMEOUT)
        {
            return;
        }
        else
        {
            switch(inKey)
            { 
                case 1: 
                { 
                    CTOS_LCDTClearDisplay();
                    inDISP_PutGraphic(_CONNECTING_, 1, _COORDINATE_Y_LINE_8_1_);
                    CTOS_Delay(1000);
                    if(inECR_Initial() == VS_SUCCESS)
                    {
                        CTOS_LCDTClearDisplay();
                        CTOS_LCDTPrintXY(1, 1, "RS232ConnSuccess!!!");
                        CTOS_Delay(2000);
                    }
                    else
                    {
                        CTOS_LCDTPrintXY(1, 1, "RS232ConnFail!!!");
                        CTOS_Delay(2000);
                    }
                    break; 
                }
                case 2: 
                {   
                    CTOS_LCDTClearDisplay();
                    inDISP_PutGraphic(_RECEIVE_, 1, _COORDINATE_Y_LINE_8_1_);
                    CTOS_Delay(1000);
                    inRetVal = inRS232_ECR_8N1_Standard_Receive_Packet(pobTran, &gsrECROb);
                    CTOS_LCDTClearDisplay();
                    if( inRetVal == VS_SUCCESS)
                    {
                        CTOS_LCDTPrintXY(1,1,"Rx Data Success!!!");
                        CTOS_Delay(2000);
                    }
                    else
                    {
                        CTOS_LCDTPrintXY(1,1,"Rx Data Fail!!!");
                        CTOS_Delay(2000);
                    }
                    printf("***-------------------------***\n");
                    int i;
                    for(i= 0; i< 43 ;i++)
                    {
                        printf("%s:[%s]\n",myECRTable[i].FieldName,myECRTable[i].Data);
                    }

                    printf("***-------------------------***\n");
                    break;
                }
                case 3: 
                { 
                    CTOS_LCDTClearDisplay();
                    inDISP_PutGraphic(_SEND_, 1, _COORDINATE_Y_LINE_8_1_);
                    CTOS_Delay(1000);
                    inRetVal = inRS232_ECR_8N1_Standard_Send_Packet(pobTran, &gsrECROb);
                    CTOS_LCDTClearDisplay();
                    if(  inRetVal == VS_SUCCESS)
                    {
                        CTOS_LCDTPrintXY(1,1,"Tx Data Success!!!");
                        CTOS_Delay(2000);
                    }
                    else
                    {
                        CTOS_LCDTPrintXY(1,1,"Tx Data Fail!!!");
                        CTOS_Delay(2000);
                    }
                    break;
                }
            }
        }
    }
}

void vdSALEMenu(TRANSACTION_OBJECT* pobTran)
{
    BYTE uszPackBuf[984];
    BYTE uszRecvPacket[_NCCC_ATS_ISO_SEND_ + 1];
    int  inReceiveTimeout = 10;
    int	 inReceiveSize = _COMM_RECEIVE_MAX_LENGTH_;
    unsigned char uszKey = 0;
    int inRetVal = 0;
    CTOS_LCDTClearDisplay();
    CTOS_LCDTPrintXY(1,1,"請輸入數字:");
    while(1)
    {
        uszKey = 0x00;
        uszKey = uszKBD_Key();
        if (uszKey != 0x00)
        {
            if(uszKey >=_KEY_1_ && uszKey <=_KEY_9_)
            {
                pobTran->inMenuKeyin = uszKey;
                CTOS_LCDTClearDisplay();
                inRetVal  = inCREDIT_Func_Get_OPT_Amount(pobTran);
                /*
                    * inCREDIT_Func_Get_OPT_Amount > inDISP_EnglishFont_Color會改到字型和大小，
                    *  這邊重新呼叫inDISP_Initial()
                */
                inDISP_Initial();
                if(  inRetVal == VS_SUCCESS)
                {
                    printf("Amount is %06ld\n",pobTran->srBRec.lnTxnAmount);
                }
                else if (inRetVal == VS_TIMEOUT || inRetVal == VS_USER_CANCEL)
                {
                    return;
                }
                else
                {
                    printf("Get Amount Failed\n");
                }     
                //取得金額後發送電文
                CTOS_LCDTClearDisplay(); 
                inRetVal = inETHERNET_Initial();
                if(inRetVal == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1, 1, "Config Set Success!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1, 1, "Config Set Fail!!!");
                    CTOS_Delay(2000);
                }
                CTOS_LCDTClearDisplay();
                inDISP_PutGraphic(_CONNECTING_, 1, _COORDINATE_Y_LINE_8_1_);
                CTOS_Delay(1000);
                CTOS_LCDTClearDisplay();
                if(inETHERNET_SetConfig() == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1,1,"Connect Host Success!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1, 1, "Connect Host Fail!!!");
                    CTOS_Delay(2000);
                    break;
                }
                memset(uszPackBuf,0x00,sizeof(uszPackBuf));
                int inCnt = mySalePackData(pobTran,uszPackBuf);

                CTOS_LCDTClearDisplay();
                inDISP_PutGraphic(_SEND_, 1, _COORDINATE_Y_LINE_8_1_);
                CTOS_Delay(1000);
                CTOS_LCDTClearDisplay(); 
                if( inETHERNET_Send(uszPackBuf,inCnt-2,0) == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1,1,"EthernetTx Success!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!");
                    CTOS_Delay(2000);
                    break;
                }
                CTOS_LCDTClearDisplay();
                inDISP_PutGraphic(_RECEIVE_, 1, _COORDINATE_Y_LINE_8_1_);
                CTOS_Delay(1000);
                CTOS_LCDTClearDisplay();
                memset(uszRecvPacket,0x00,sizeof(uszRecvPacket));
                inReceiveSize = inETHERNET_Receive(uszRecvPacket,inReceiveSize,inReceiveTimeout);
                if(inReceiveSize > 0 )
                {
                    myUnPackData( uszRecvPacket , inReceiveSize);
                    CTOS_LCDTPrintXY(1,1,"EthernetRx Success!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                   CTOS_LCDTPrintXY(1,1,"EthernetRx Fail!!!");
                   CTOS_Delay(2000);
                   break;
                }
                if (inETHERNET_END() != VS_SUCCESS)
                {
                    printf("inETHERNET_END ERR\n");
                    break;
                }
                if (inETHERNET_Close() != VS_SUCCESS)
                {
                    printf("inETHERNET_Close ERR\n");
                    break;
                }
                inFuncInsertTxnRecord_By_Sqlite(pobTran);
                int inChoice = 0;
                unsigned char  uszkey;
                CTOS_LCDTClearDisplay();
                CTOS_LCDTPrintXY(1,1,"是否列印簽單?");
                inDISP_PutGraphic(_MSG_ENTER_OR_CANCEL_, 0, _COORDINATE_Y_LINE_8_8_);
                inDISP_Timer_Start(_TIMER_NEXSYS_1_, 30);
                while (1)
                {
                        uszkey = -1;
                        inChoice = inDisTouch_TouchSensor_Click_Slide(_Touch_OX_LINE8_8_);
                        uszkey = uszKBD_Key();
                        if (inChoice == _Touch_OX_LINE8_8_ENTER_BUTTON_)
                        {
                                uszkey = _KEY_ENTER_;
                        }
                        else if (inChoice == _Touch_OX_LINE8_8_CANCEL_BUTTON_)
                        {
                                uszkey = _KEY_CANCEL_;
                        }

                        /* Timeout */
                        if (inTimerGet(_TIMER_NEXSYS_1_) == VS_SUCCESS)
                        {
                                uszkey = _KEY_TIMEOUT_;
                        }
                        if(uszkey == _KEY_ENTER_)
                        {
                                //列印簽單
                                inCREDIT_PRINT_Receipt_ByBuffer(pobTran);
                                break;
                        }
                        else if(uszkey == _KEY_CANCEL_ || uszkey == _KEY_TIMEOUT_)
                        {
                                
                                break;
                        }
                }	      
                break;
            }
            else if(uszKey == _KEY_CANCEL_)
            {
                break;
            }
        }
    }
}
int main(int argc, char *argv[]) {
    int  inRetVal = 0;
    TRANSACTION_OBJECT pobTran;
    memset(&pobTran, 0, sizeof(TRANSACTION_OBJECT));
    /*============假資料============*/
    pobTran.srBRec.inCode = _SALE_;
    pobTran.srBRec.inPrintOption = _PRT_MERCH_;
    strcpy(pobTran.srBRec.szCardLabel, "VISA"); //卡別
    strcpy(pobTran.srBRec.szPAN, "493817******1411"); //卡號
    strcpy(pobTran.srBRec.szCheckNO, "56795"); //檢查碼
    strcpy(pobTran.srBRec.szDate, "20260302"); //日期
    strcpy(pobTran.srBRec.szTime, "143059"); //時間
    pobTran.srBRec.lnOrgInvNum = 35;
    pobTran.srBRec.lnBatchNum  = 1;
    strcpy(pobTran.srBRec.szAuthCode, "777777");
    strcpy(pobTran.srBRec.szRefNo, "99202001035");
    strcpy(pobTran.srBRec.szRespCode, "00");
    strcpy(pobTran.srBRec.szStoreID,"2222");
    strcpy(pobTran.srBRec.szCardHolder,"Huang");
    pobTran.srBRec.lnTxnAmount = 19999;
    pobTran.srBRec.uszNoSignatureBit == VS_TRUE;
    /*============假資料============*/
    /*============LCD設定============*/
    inDISP_Initial();
    /*============LCD設定============*/
    
    /* 可查看是否有fs_data路徑(是否有Load img.mci) */
//    inFunc_ls("-R -l", _AP_ROOT_PATH_); 
    CTOS_LCDTPrintXY(1, 1, "System Startup");
    CTOS_LCDTPrintXY(1, 2, "Loading Image...");
    CTOS_LCDTPrintXY(1, 3, "Configuring Printer...");
    CTOS_LCDTPrintXY(1, 3, "Initializing Ethernet...");
    CTOS_LCDTPrintXY(1, 4, "Setting DB...");
    CTOS_Delay(1000);
    //要先Load APP.mci，再來才是Load img.mci
    inFunc_Booting_Flow_Print_Image_Initial(&gsrBMPHeight);
    printf("初始化圖片預設高度\n");
    printf("%s:inBankLogoHeight is %d \n",     _BANK_LOGO_,      gsrBMPHeight.inBankLogoHeight);
    printf("%s:inMerchantLogoHeight is %d \n", _MERCHANT_LOGO_,  gsrBMPHeight.inMerchantLogoHeight);
    printf("%s:inTitleNameHeight is %d \n",    _NAME_LOGO_,      gsrBMPHeight.inTitleNameHeight);
    /*============印表機設定============*/
    CTOS_PrinterFontSelectMode(d_FONT_TTF_MODE);
    //    CTOS_FontTTFSelectFontFile(d_FONT_DEVICE_PRINTER, "ca_default.ttf", 0);
    CTOS_FontTTFSelectFontFile(d_FONT_DEVICE_PRINTER, "tsuyuan.ttf", 0);
    CTOS_FontTTFSelectStyle(d_FONT_DEVICE_PRINTER, d_FONT_STYLE_NORMAL);
    /*============印表機設定============*/
    inPRINT_Buffer_Initial(uszBuffer, _BUFFER_MAX_LINE_, &srFont_Attrib, &srBhandle);
  
    /*============開啟資料庫============*/
    if(inSqlite_Initial() != VS_SUCCESS)
    {
        printf("inSqlite Initial Failed!\n");
        return VS_FALSE;
    }
    /*============開啟資料庫============*/
    char mainMenu[][48+1]={"簽單","乙太","Rs232","_SALE_","Sql"};
    int cnt = sizeof(mainMenu)/sizeof(mainMenu[0]);
    int inkey = 0;
    CTOS_LCDTClearDisplay();

    while(1)
    {
        inkey = 0;
        /*============LCD設定============*/
        inDISP_Initial();
        /*============LCD設定============*/
        inRetVal = inFunc_GetMenuNum_NewUI(&pobTran,mainMenu,cnt,&inkey);

        if(inRetVal == VS_USER_CANCEL || inRetVal == VS_TIMEOUT)
        {
            exit(0);
        }
        else
        {
            switch(inkey)
            { 
                case 1: 
                { 
                   inCREDIT_PRINT_Receipt_ByBuffer(&pobTran);
                   break;
                }
                case 2: 
                {   
                    vdEthernetMenu(&pobTran);
                    break;
                }
                case 3: 
                { 
                    vdRS232Menu(&pobTran);
                    break;
                }
                case 4: 
                {   
                    //期望做到直接跳到輸入金額 > 組裝封包 > 送電文 > 寫入資料庫 > 列印簽單         
                    vdSALEMenu(&pobTran);
                    break;
                }
                case 5:
                {
                    vdSQLOpMenu(&pobTran);
                    break;
                }
            } 
        }
    }  
}
