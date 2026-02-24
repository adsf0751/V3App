
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctosapi.h>
#include <fcntl.h>
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
extern MY_ECR_DATA myECRTable[43];
extern  char		gszTranDBPath[100 + 1];
char szTableName[30+1] = "NCCC0001";
SQLITE_TAG_TABLE TABLE_BATCH_TAG[] = 
{
	{"inTableID"			,"INTEGER"	,"PRIMARY KEY"	,""},	/* Table ID Primary key, sqlite table專用避免PRIMARY KEY重複 */
	{"inCode"			,"INTEGER"	,""		,""},	/* Trans Code */
	{"inOrgCode"			,"INTEGER"	,""		,""},	/* Original Trans Code  */
	{"inPrintOption"		,"INTEGER"	,""		,""},	/* Print Option Flag */
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
	{"lnOrgBatchNum"		,"INTEGER"	,""		,""},	/* Original Batch Number */
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
	{"szPAN"			,"BLOB"		,""		,""},	/* 卡號  */
	{"szDate"			,"BLOB"		,""		,""},	/* YYYYMMDD */
	{"szOrgDate"			,"BLOB"		,""		,""},	/* YYYYMMDD */
	{"szTime"			,"BLOB"		,""		,""},	/* HHMMSS */
	{"szOrgTime"			,"BLOB"		,""		,""},	/* HHMMSS */
	{"szCardTime"			,"BLOB"		,""		,""},	/* 晶片卡讀卡時間 , YYYYMMDDHHMMSS */
	{"szRefNo"			,"BLOB"		,""		,""},	/* 序號  */
	{"szExpDate"			,"BLOB"		,""		,""},	/* Expiration date */
	{"szServiceCode"		,"BLOB"		,""		,""},	/* Service code from track */
	{"szCardHolder"			,"BLOB"		,""		,""},	/* 持卡人名字 */
	{"szAMEX4DBC"			,"BLOB"		,""		,""},		
	{"szFiscIssuerID"		,"BLOB"		,""		,""},	/* 發卡單位代號 */
	{"szFiscCardComment"		,"BLOB"		,""		,""},	/* 金融卡備註欄 */
	{"szFiscAccount"		,"BLOB"		,""		,""},	/* 金融卡帳號 */
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
	{"szFiscMobileNFSetting"	,"BLOB"		,""		,""},	/* 近端交易類型設定 0x00：Single Issuer Wallet 0x01：國內Third-Party Wallet 0x02~9：保留 0x0A：其他 */
	{"szInstallmentIndicator"	,"BLOB"		,""		,""},
	{"szRedeemIndicator"		,"BLOB"		,""		,""},
	{"szRedeemSignOfBalance"	,"BLOB"		,""		,""},
	{"szHGCardLabel"		,"BLOB"		,""		,""},	/* HAPPY_GO 卡別 */
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
	{"szIssueNumber"		,"BLOB"		,""		,""},
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
	{"szProductCode"		,"BLOB"		,""		,""},	/* 產品代碼 */
	{"szAwardNum"			,"BLOB"		,""		,""},	/* 優惠個數 */
	{"szAwardSN"			,"BLOB"		,""		,""},	/* 優惠序號(Award S/N) TID(8Bytes)+YYYYMMDDhhmmss(16 Bytes)，共22Bytes */
	{"szTxnNo"			,"BLOB"		,""		,""},	/* 交易編號 */
	{"szMCP_BANKID"			,"BLOB"		,""		,""},	/* 行動支付標記 金融機構代碼 */
	{"szPayItemCode"		,"BLOB"		,""		,""},	/* 繳費項目代碼 */
	{"szTableTD_Data"		,"BLOB"		,""		,""},	/* Table TD的資料 */
	{"szDFSTraceNum"		,"BLOB"		,""		,""},	/* DFS交易系統追蹤號 */
	{"szCheckNO"			,"BLOB"		,""		,""},	/* 檢查碼，目前FOR SDK使用 */
	{"szEIVI_BANKID"		,"BLOB"		,""		,""},	/* E Invoice vehicle I ssue Bank ID 支援電子發票載具之發卡行代碼 */
	{"szBarCodeData"		,"BLOB"		,""		,""},	/* 掃碼交易資料內容 */
	{"szUnyTransCode"		,"BLOB"		,""		,""},	/* Uny交易碼 */
	{"szUnyMaskedCardNo"		,"BLOB"		,""		,""},	/* 遮掩交易卡號資料(非完整卡號，有遮掩，故另設欄位) */
	{"szEInvoiceHASH"		,"BLOB"		,""		,""},	/* 電子發票HASH 值（發卡銀行若無參加電子發票載具，則此欄位回覆值 為空白 SPACE 。） */
	{"szUnyCardLabelLen"		,"BLOB"		,""		,""},	/* 卡別名稱長度，szMCP_BANKID為Twin卡專用，故另設欄位 */
	{"szUnyCardLabel"		,"BLOB"		,""		,""},	/* 卡別名稱，szMCP_BANKID為Twin卡專用，故另設欄位 */
	{"szFPG_FTC_Invoice"		,"BLOB"		,""		,""},	/* 台塑生醫生技客製化專屬【005】　台亞福懋加油站客製化專屬【006】發票號碼 */
        {"szUPlan_Coupon"               ,"BLOB"		,""		,""},	/* 銀聯優計畫的優惠訊息 */
        {"szUPlan_RemarksInformation"   ,"BLOB"		,""		,""},	/* 銀聯優計畫-備註訊息 */
        {"szFlightTicketTransBit"       ,"BLOB"		,""		,""},	/* 判斷是否是機票交易 不設定為空值 機票交易為1 非機票交易為2 */
        {"szFlightTicketPDS0523"        ,"BLOB"		,""		,""},	/* 出發地機場代碼（PDS 0523）左靠右補空白 */
        {"szFlightTicketPDS0524"        ,"BLOB"		,""		,""},	/* 目的地機場代碼（PDS 0524）左靠右補空白 */
        {"szFlightTicketPDS0530"        ,"BLOB"		,""		,""},	/* 航班號碼（PDS 0530） */
	{"uszWAVESchemeID"		,"BLOB"		,""		,""},	/* WAVE 使用用於組電文 Field_22 */
	{"uszVOIDBit"			,"BLOB"		,""		,""},	/* 負向交易 */
        {"uszUpload1Bit"		,"BLOB"		,""		,""},	/* Offline交易使用 (原交易advice是否已上傳)*/
        {"uszUpload2Bit"		,"BLOB"		,""		,""},	/* Offline交易使用 (當前交易是否為advice)*/
        {"uszUpload3Bit"		,"BLOB"		,""		,""},	/* Offline交易使用 */
        {"uszReferralBit"		,"BLOB"		,""		,""},	/* ISO Response Code 【01】【02】使用 */
        {"uszOfflineBit"		,"BLOB"		,""		,""},	/* 離線交易 */
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
	{"uszFiscVoidConfirmBit"	,"BLOB"		,""		,""},	/* SmartPay Void 0220 是否已上傳 */
	{"uszPinEnterBit"		,"BLOB"		,""		,""},	/* 此筆交易是否有鍵入密碼(只能確認原交易，若該筆之後的調整或取消不會將此Bit Off) */
	{"uszL2PrintADBit"		,"BLOB"		,""		,""},	/* L2是否印AD，因L2原交易取消要判斷，只好增加 */
	{"uszInstallment"		,"BLOB"		,""		,""},	/* HappyGo分期交易 */
        {"uszRedemption"		,"BLOB"		,""		,""},	/* HappyGo點數兌換 */ 
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
Function        :inFunc_Booting_Flow_Print_Image_Initial
Date&Time       :2018/6/5 下午 6:27
Describe        :開機流程列印圖片初始化，為了避免開機更新導致抓不到圖片高度
 */
int inFunc_Booting_Flow_Print_Image_Initial(TRANSACTION_OBJECT *pobTran,BMPHeight* gsrBMPHeight) {
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
void vdEthernetMenu(void)
{
    BYTE key;
    int breakFlag = 0;
    int inRetVal = VS_ERROR;
    BYTE uszPackBuf[984];
    memset(uszPackBuf,0x00,sizeof(uszPackBuf));
    int inCnt = myPackData(uszPackBuf);
    unsigned char   uszTemplate[60 + 1];
    BYTE uszRecvPacket[_NCCC_ATS_ISO_SEND_ + 1];
    int  inReceiveTimeout = 10;
    int	 inReceiveSize = _COMM_RECEIVE_MAX_LENGTH_;
    while(1)
    {
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1, 1, "Ethernet Menu");
        CTOS_LCDTPrintXY(1, 2, "1:Set Config");
        CTOS_LCDTPrintXY(1, 3, "2:Get Config");
        CTOS_LCDTPrintXY(1, 4, "3:Connect");
        CTOS_LCDTPrintXY(1, 5, "4:Tx Data");
        CTOS_LCDTPrintXY(1, 6, "5.Rx Data");
//        CTOS_LCDTPrintXY(1, 7, "6.Ping");
        CTOS_LCDTPrintXY(1, 16, "X.Exit");
        CTOS_KBDGet(&key);
        breakFlag = 0;
        switch(key)
        { 
            case d_KBD_1: 
            { 
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
                break;                
            }
            case d_KBD_2: 
            {   
                CTOS_LCDTClearDisplay();
                memset(uszTemplate, 0x00, sizeof(uszTemplate));               
                memcpy(uszTemplate,"IP:", 3);
                vdEthernetGetNetWorkValue(d_ETHERNET_CONFIG_IP,(unsigned char*)uszTemplate);
                CTOS_LCDTPrintXY(1,1,uszTemplate);

                memset(uszTemplate, 0x00, sizeof(uszTemplate));               
                memcpy(uszTemplate,"GW:", 3);
                vdEthernetGetNetWorkValue(d_ETHERNET_CONFIG_GATEWAY,(unsigned char*)uszTemplate);
                CTOS_LCDTPrintXY(1,2,uszTemplate);

                memset(uszTemplate, 0x00, sizeof(uszTemplate));               
                memcpy(uszTemplate,"MASK:", 5);
                vdEthernetGetNetWorkValue(d_ETHERNET_CONFIG_MASK,(unsigned char*)uszTemplate);
                CTOS_LCDTPrintXY(1,3,uszTemplate);

                CTOS_Delay(2000);
                break;
            }
            case d_KBD_3: 
            { 
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
            case d_KBD_4: 
            {
                CTOS_LCDTClearDisplay(); 
                /*
                 * 原先傳到主機的長度對不起來，原因是inCnt已包含電文前面的長度
                 * 但inETHERNET_Send預設傳入inSendSize 是未包含Message Length，
                 * 所以在此function會自動+2，為了不改動用到function，故再此先讓inSendSize -2
                 */
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
            case d_KBD_5: 
            {   //BUG:Tx完按Rx可以正常使用，但只按RX多次會報錯，先記錄，暫時不改
                CTOS_LCDTClearDisplay(); 
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
                }
                break;
            }
            case d_KBD_6: 
            {   //TODO:未使用到ETHERNET_PING()
                break;
            }
            case d_KBD_CANCEL: 
            { 
                breakFlag = 1;
                CTOS_LCDTClearDisplay(); 
                if(inETHERNET_END() == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1,1,"EthernetDisconnSuccess!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1,1,"EthernetDisconnFail!!!");
                    CTOS_Delay(2000);
                }
                break;
            }
        }
        if(breakFlag)
            break;
    }
}

void vdRS232Menu(TRANSACTION_OBJECT* pobTran)
{
    BYTE key;
    int breakFlag = 0;
    int inRetVal = 0;
    ECR_TABLE   gsrECROb = {.srSetting.uszComPort = d_COM2};
    while(1)
    {
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1, 1, "Rs232 Menu");
        CTOS_LCDTPrintXY(1, 2, "1:Set Config");
        
        CTOS_LCDTPrintXY(1, 3, "2:Rx Data");
        CTOS_LCDTPrintXY(1, 4, "3:Tx Data");
        CTOS_LCDTPrintXY(1, 16, "X.Exit");
        CTOS_KBDGet(&key);
        breakFlag = 0;
        switch(key)
        { 
            case d_KBD_1: 
            { 
                CTOS_LCDTClearDisplay();               
                if(inECR_Initial() == VS_SUCCESS)
                {
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
            case d_KBD_2: 
            {   
                CTOS_LCDTClearDisplay();
                CTOS_LCDTPrintXY(1, 1, "Processing....");
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
            //BUG:單純做Tx會傳送電文時失敗, 超過重試次數，但做Rx+Tx可以，不確定是否為正常流程
            case d_KBD_3: 
            {              
                CTOS_LCDTClearDisplay();
                CTOS_LCDTPrintXY(1, 1, "Processing....");
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
            
            case d_KBD_CANCEL: 
            { 
                breakFlag = 1;
                CTOS_LCDTClearDisplay(); 
                if(inRS232_Close(gsrECROb.srSetting.uszComPort) == VS_SUCCESS)
                {
                    CTOS_LCDTPrintXY(1,1,"RS232DisconnSuccess!!!");
                    CTOS_Delay(2000);
                }
                else
                {
                    CTOS_LCDTPrintXY(1,1,"RS232DisconnFail!!!");
                    CTOS_Delay(2000);
                }
                break;
            }
        }
        if(breakFlag)
            break;
    }
}

void vdSALEMenu(TRANSACTION_OBJECT* pobTran)
{
    BYTE key;
    BYTE uszPackBuf[984];
    memset(uszPackBuf,0x00,sizeof(uszPackBuf));
    unsigned char uszKey = 0;
    int breakFlag = 0;
    int inRetVal = 0;
    SQLITE_ALL_TABLE	srAll;
    CTOS_LCDTClearDisplay(); 
    CTOS_LCDTPrintXY(1,1,"請輸入數字:");
    while(1)
    {
        uszKey = 0x00;
        uszKey = uszKBD_Key();
        if (uszKey != 0x00)
        {    
            CTOS_LCDTClearDisplay();
            switch (uszKey)
            {
                case _KEY_1_:
                case _KEY_2_:
                case _KEY_3_:
                case _KEY_4_:
                case _KEY_5_:
                case _KEY_6_:
                case _KEY_7_:
                case _KEY_8_:
                case _KEY_9_:
            //        case _KEY_0_:
            //        case _KEY_F1_:
            //        case _KEY_F2_:
            //        case _KEY_F3_:
            //        case _KEY_F4_:
            //        case _KEY_CLEAR_:
            //        case _KEY_DOT_:
            //        case _KEY_ENTER_:
            //        case _KEY_FUNCTION_:
            //        case _KEY_CANCEL_:
                    pobTran->inMenuKeyin = uszKey;
                    
                    inRetVal  = inCREDIT_Func_Get_OPT_Amount(pobTran);
                    if(  inRetVal == VS_SUCCESS)
                    {
                        printf("Amount is %06ld\n",pobTran->srBRec.lnTxnAmount);
                    }
                    else
                    {
                        printf("Get Amount Failed\n");
                        return ;
                    }         
                    break;
            }
            //取得金額後發送電文
            CTOS_LCDTClearDisplay();
//            int inCnt = mySalePackData(pobTran,uszPackBuf);
//
//            inRetVal = inETHERNET_Initial();
//            if(inRetVal == VS_SUCCESS)
//            {
//                CTOS_LCDTPrintXY(1, 1, "Config Set Success!!!");
//                CTOS_Delay(2000);
//            }
//            else
//            {
//                CTOS_LCDTPrintXY(1, 1, "Config Set Fail!!!");
//                CTOS_Delay(2000);
//            }
//            CTOS_LCDTClearDisplay();
//            if(inETHERNET_SetConfig() == VS_SUCCESS)
//            {
//                CTOS_LCDTPrintXY(1,1,"Connect Host Success!!!");
//                CTOS_Delay(2000);
//            }
//            else
//            {
//                CTOS_LCDTPrintXY(1, 1, "Connect Host Fail!!!");
//                CTOS_Delay(2000);
//            }
//            
//            CTOS_LCDTClearDisplay(); 
//            if( inETHERNET_Send(uszPackBuf,inCnt-2,0) == VS_SUCCESS)
//            {
//                CTOS_LCDTPrintXY(1,1,"EthernetTx Success!!!");
//                CTOS_Delay(2000);
//            }
//            else
//            {
//                CTOS_LCDTPrintXY(1,1,"EthernetTx Fail!!!");
//                CTOS_Delay(2000);
//            }
            /*
             * TODO:
             * 1.Insert into table 可以存每筆交易，
             * 可能包含 流水號、MID、TID、金額、交易類型、建立時間等
             * 2.列印簽單            
             */
            inFuncInsertTxnRecord_By_Sqlite(pobTran);
            
            char    szMaxTableID[10 + 1] = {0};
            memset(szMaxTableID, 0x00, sizeof(szMaxTableID));
            inRetVal = inSqlite_Get_Max_TableID(gszTranDBPath, szTableName, szMaxTableID);
            if (inRetVal != VS_SUCCESS)
            {
                    return ;
            }
            else
            {
                printf("Max RowID is %s\n",szMaxTableID);
                /*============單筆查詢============*/
                memset(&srAll, 0x00, sizeof(SQLITE_ALL_TABLE));
                memset(&pobTran->srBRec, 0x00, sizeof(pobTran->srBRec));
                printf("\nbefore query TxnAmount is %ld\n",pobTran->srBRec.lnTxnAmount);
                inRetVal = inSqlite_Table_Link_BRec(pobTran, &srAll, _LS_READ_);
                char szQuerySql[100+1];
                memset(szQuerySql, 0x00, sizeof(szQuerySql));
                sprintf(szQuerySql, "SELECT * FROM %s WHERE inTableID = %s ", szTableName,szMaxTableID);
                inRetVal = inSqlite_Get_Data_By_External_SQL(gszTranDBPath, &srAll, szQuerySql);
                inSqlite_Table_Show(&srAll);
                printf("\nafter query TxnAmount is %ld\n",pobTran->srBRec.lnTxnAmount);
                /*============單筆查詢============*/
            }
            break;
        }
    }
}


int main(int argc, char *argv[]) {
    BYTE key;
    CTOS_LCDTClearDisplay();
    int  inRetVal = 0;
    unsigned char uszBuffer[PB_CANVAS_X_SIZE * 8 * _BUFFER_MAX_LINE_];
    BufferHandle srBhandle;
    FONT_ATTRIB srFont_Attrib;
    TRANSACTION_OBJECT pobTran;
    memset(&pobTran, 0, sizeof(TRANSACTION_OBJECT));
    /*============假資料============*/
    pobTran.srBRec.inPrintOption = _PRT_CUST_;
    strcpy(pobTran.srBRec.szCardLabel, "VISA"); //卡別
    strcpy(pobTran.srBRec.szPAN, "493817******1411"); //卡號
    strcpy(pobTran.srBRec.szDate, "1223"); //日期
    strcpy(pobTran.srBRec.szTime, "143059"); //時間
    pobTran.srBRec.lnOrgInvNum = 1;
    pobTran.srBRec.lnBatchNum  = 1;
    strcpy(pobTran.srBRec.szAuthCode, "777777");
    strcpy(pobTran.srBRec.szRefNo, "999999999999");
    /*============假資料============*/
    
    BMPHeight gsrBMPHeight;
    /* 可查看是否有fs_data路徑(是否有Load img.mci) */
//    inFunc_ls("-R -l", _AP_ROOT_PATH_); 
    CTOS_LCDTPrintXY(1, 1, "System Startup");
    CTOS_LCDTPrintXY(1, 2, "Loading Image...");
    CTOS_LCDTPrintXY(1, 3, "Configuring Printer...");
    CTOS_LCDTPrintXY(1, 4, "DB Setting..."); 
    CTOS_Delay(1000);
    /*============LCD設定============*/
    inDISP_Initial();
    /*============LCD設定============*/
    //要先Load APP.mci，再來才是Load img.mci
    inFunc_Booting_Flow_Print_Image_Initial(&pobTran,&gsrBMPHeight);
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
    int breakFlag = 0;
    inPRINT_Buffer_Initial(uszBuffer, _BUFFER_MAX_LINE_, &srFont_Attrib, &srBhandle);
    
    /*============開啟資料庫============*/
    if(inSqlite_Initial() != VS_SUCCESS)
    {
        printf("inSqlite Initial Failed!\n");
        return VS_FALSE;
    }
    /*============開啟資料庫============*/
    
    while(1)
    {
        CTOS_LCDTClearDisplay();
        CTOS_LCDTPrintXY(1, 1, "Main Menu");
        CTOS_LCDTPrintXY(1, 2, "1:Print Receipt");
        CTOS_LCDTPrintXY(1, 3, "2:Ethernet COMM");
        CTOS_LCDTPrintXY(1, 4, "3:Rs232 COMM");
        CTOS_LCDTPrintXY(1, 5, "4:__SALE__");
        CTOS_LCDTPrintXY(1, 16, "X:Exit");
        CTOS_KBDGet(&key);
        breakFlag = 0;
        switch(key)
        { 
            case d_KBD_1: 
            { 
                /*
                 * TODO:改放在一個function table 呼叫下方列印簽單。
                 */
                if ((inRetVal = inCREDIT_PRINT_Logo_ByBuffer(&pobTran, uszBuffer, &srFont_Attrib, &srBhandle,&gsrBMPHeight)) != VS_SUCCESS)
                    printf("inCREDIT_PRINT_Logo_ByBuffer failed, ret=%d\n",inRetVal);
                if ((inRetVal = inCREDIT_PRINT_Tidmid_ByBuffer(&pobTran, uszBuffer, &srFont_Attrib, &srBhandle)) != VS_SUCCESS)
                    printf("inCREDIT_PRINT_Tidmid_ByBuffer failed, ret=%d\n",inRetVal);
                if ((inRetVal = inCREDIT_PRINT_Data_ByBuffer(&pobTran, uszBuffer, &srFont_Attrib, &srBhandle)) != VS_SUCCESS)
                    printf("inCREDIT_PRINT_Data_ByBuffer failed, ret=%d\n",inRetVal);
                if ((inRetVal = inPRINT_Buffer_OutPut(uszBuffer, &srBhandle)) != VS_SUCCESS)
                    printf("inPRINT_Buffer_OutPut failed, ret=%d\n",inRetVal);
               break;
            }
            //BUG:進入選單直接選擇跳出會因為 跳出做disconnet 但因為沒有open過而顯示diconn fail
            case d_KBD_2: 
            {   
                vdEthernetMenu();
                break;
            }
            case d_KBD_3: 
            { 
                vdRS232Menu(&pobTran);
                break;
            }
            case d_KBD_4: 
            {   
                //期望做到直接跳到輸入金額 > 組裝封包 > 送電文 > 寫入資料庫 > 列印簽單               
                vdSALEMenu(&pobTran);
                break;
            }
            case d_KBD_CANCEL: 
            { 
                breakFlag = 1;
                break;
            }
        }
        if(breakFlag)
            break;
    }
    exit(0);
}
