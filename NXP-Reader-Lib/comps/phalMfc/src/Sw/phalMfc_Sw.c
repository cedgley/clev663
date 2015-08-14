/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Software MIFARE(R) Component of Reader Library Framework.
* $Author: nxp40786 $
* $Revision: 161 $
* $Date: 2013-06-05 14:04:36 +0530 (Wed, 05 Jun 2013) $
*
* History:
*  CHu: Generated 31. July 2009
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <des.h>
#include <ph_Status.h>
#include <phhalHw.h>
#include <phalMfc.h>
#include <phpalMifare.h>
#include <ph_RefDefs.h>
#include <phKeyStore.h>

#ifdef NXPBUILD__PHAL_MFC_SW

#include "phalMfc_Sw.h"
#include "../phalMfc_Int.h"
/*
static*/ /* const */ /*uint8_t KeyULC1[8] =	{
										0x49U, 0x45U, 0x4DU, 0x4BU, \
										0x41U, 0x45U, 0x52U, 0x42U
										};
										
static*/ /* const */ /*uint8_t KeyULC2[8] =	{
										0x21U, 0x4EU, 0x41U, 0x43U, \
										0x55U, 0x4FU, 0x59U, 0x46U
										};*/
										
static /* const */ uint8_t KeyULC3[24] = { /** Mifare ultralight default key1: IEMKAERB key2: !NACUOYF (BREAKMEIFYOUCAN! backwards) key3 = key1 */
										 0x49U, 0x45U, 0x4DU, 0x4BU, \
										 0x41U, 0x45U, 0x52U, 0x42U, \
																	 \
										 0x21U, 0x4EU, 0x41U, 0x43U, \
										 0x55U, 0x4FU, 0x59U, 0x46U, \
																	 \
										 0x49U, 0x45U, 0x4DU, 0x4BU, \
										 0x41U, 0x45U, 0x52U, 0x42U  \
										 };

										 
static /* const */ uint8_t KeyULC3_wrong[24] = { /** Definitely a wrong key */
										 0x6CU, 0x6FU, 0x6CU, 0x6FU, \
										 0x6CU, 0x6FU, 0x6CU, 0x6FU, \
																	 \
										 0x6CU, 0x6FU, 0x6CU, 0x6FU, \
										 0x6CU, 0x6FU, 0x6CU, 0x6FU, \
																	 \
										 0x6CU, 0x6FU, 0x6CU, 0x6FU, \
										 0x6CU, 0x6FU, 0x6CU, 0x6FU, \
										 };
										 
										 
uint16_t * phalMfulc_Sw_GetKey(uint16_t key[24])
{
	memcpy(key, KeyULC3, 24);
	return key;
}

phStatus_t phalMfc_Sw_Init(
                           phalMfc_Sw_DataParams_t * pDataParams,
                           uint16_t wSizeOfDataParams, 
                           void * pPalMifareDataParams,
                           void * pKeyStoreDataParams
                           )
{
    if (sizeof(phalMfc_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFC);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pPalMifareDataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFC | PHAL_MFC_SW_ID;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;
    pDataParams->pKeyStoreDataParams    = pKeyStoreDataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC);
}

phStatus_t phalMfulc_Sw_Init(
                           phalMfc_Sw_DataParams_t * pDataParams,
                           uint16_t wSizeOfDataParams, 
                           void * pPalMifareDataParams
                           )
{
    if (sizeof(phalMfc_Sw_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_AL_MFC);
    }
	PH_ASSERT_NULL (pDataParams);
	PH_ASSERT_NULL (pPalMifareDataParams);

    /* init private data */
    pDataParams->wId                    = PH_COMP_AL_MFC | PHAL_MFC_SW_ID;
    pDataParams->pPalMifareDataParams   = pPalMifareDataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC);
}

phStatus_t phalMfc_Sw_Authenticate(
                                   phalMfc_Sw_DataParams_t * pDataParams,
                                   uint8_t bBlockNo,
                                   uint8_t bKeyType,
                                   uint16_t wKeyNo,
                                   uint16_t wKeyVersion,
                                   uint8_t * pUid,
                                   uint8_t bUidLength
                                   )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aKey[PHHAL_HW_MFC_KEY_LENGTH * 2];
    uint8_t     PH_MEMLOC_REM aKeyUC[PHHAL_HW_MFULC_KEY_LENGTH];
    uint8_t *   PH_MEMLOC_REM pKey;
    uint16_t    PH_MEMLOC_REM bKeystoreKeyType;
    
    int Uidstart = bUidLength - 4;

    /* check if software key store is available. */
    if (pDataParams->pKeyStoreDataParams == NULL)
    {
        /* There is no software keystore available. */
        if ((bKeyType & 0x7F) == PHHAL_HW_MFULC_KEY) Uidstart = 0;
        return phpalMifare_MfcAuthenticateKeyNo(
            pDataParams->pPalMifareDataParams,
            bBlockNo,
            bKeyType,
            wKeyNo,
            wKeyVersion,
            &pUid[Uidstart]);
    }
    else
    {
        /* Software key store found. */

        /* Bail out if we haven't got a keystore */
        if (pDataParams->pKeyStoreDataParams == NULL)
        {
            return PH_ADD_COMPCODE(PH_ERR_KEY, PH_COMP_HAL);
        }

        /* retrieve KeyA & KeyB from keystore */
        PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_GetKey(
            pDataParams->pKeyStoreDataParams,
            wKeyNo,
            wKeyVersion,
            sizeof(aKeyUC),
            aKeyUC,
            &bKeystoreKeyType));
            
        /* check key type */
        if ((bKeystoreKeyType != PH_KEYSTORE_KEY_TYPE_MIFARE) && (bKeystoreKeyType != PH_KEYSTORE_KEY_TYPE_2K3DES))
        {
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }

        /* Evaluate which key to use */
        if ((bKeyType & 0x7F) == PHHAL_HW_MFC_KEYA)
        {
            /* Use KeyA */
            pKey = aKey;
        }
        else if ((bKeyType & 0x7F) == PHHAL_HW_MFC_KEYB)
        {
            /* Use KeyB */
            pKey = &aKey[PHHAL_HW_MFC_KEY_LENGTH];
        }
        else if ((bKeyType & 0x7F) == PHHAL_HW_MFULC_KEY)
        {
            /* Use KeyUC */
            pKey = aKeyUC;
			Uidstart = 0;
        }
        else
        {	
            return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_HAL);
        }
		
        /*return phpalMifare_MfcAuthenticate(
            pDataParams->pPalMifareDataParams,
            bBlockNo,
            bKeyType,
            pKey,
            &pUid[bUidLength - 4]);*/
            return phpalMifare_MfcAuthenticate(
            pDataParams->pPalMifareDataParams,
            bBlockNo,
            bKeyType,
            pKey,
            &pUid[Uidstart]); /**Changed from last 4 bytes to first 4 bytes only when MFUC*/
    }
}

phStatus_t phalMfc_Sw_Read(
                           phalMfc_Sw_DataParams_t * pDataParams,
                           uint8_t bBlockNo,
                           uint8_t * pBlockData
                           )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM wRxLength;
    int debug = 0;

    /* build command frame */
    aCommand[0] = PHAL_MFC_CMD_READ;
    aCommand[1] = bBlockNo;

    /* transmit the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    if(debug) printf("wRxLength %d\n", wRxLength);
    /* check received length */
    if (wRxLength != PHAL_MFC_DATA_BLOCK_LENGTH)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFC);
    }

    /* copy received data block */
    memcpy(pBlockData, pRxBuffer, wRxLength);  /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC); 
}

phStatus_t phalMfulc_Sw_Auth(phalMfc_Sw_DataParams_t * pDataParams, uint8_t * pBlockData)
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t			PH_MEMLOC_REM aCommand[2];
    uint8_t			PH_MEMLOC_REM authStep2[17];/* Nonce2 and Step 1 proof (encrypted)*/
    uint8_t *		PH_MEMLOC_REM pRxBuffer;
    uint16_t		PH_MEMLOC_REM wRxLength;
    uint8_t			nonce1[8];					/* Encrypted received nonce */
    uint8_t			nonce1_de[8];				/* Decrypted received nonce */
    uint8_t			nonce1_proof[8];			/* Nonce1 lshifted 8 bits */
    uint8_t			nonce1_proof_cbc[8];		/* Nonce1 after applying cbc */
    uint8_t			nonce1_proof_en[8];			/* Nonce1 encrypted */
    uint8_t			nonce2_chall[8];			/* Nonce2 challenge to send */
    uint8_t			nonce2_chall_cbc[8];		/* Nonce2 after applying cbc */
    uint8_t			nonce2_chall_en[8];			/* Nonce2 encrypted */
    uint8_t			nonce3_en[8];				/* Encrypted received nonce2 */
    uint8_t			nonce3_de[8];				/* Decrypted received nonce2 */
    uint8_t			nonce3_no_cbc[8];			/* Decrypted nonce2 cbc stripped */
    uint8_t			nonce3_no_cbc_unshifted[8];	/* Decrypted nonce2 cbc stripped */
    
    unsigned char schedule_en[3][16][6];		/* Schedule for encryption */
    unsigned char schedule_de[3][16][6];		/* Schedule for decryption */
    
    int i;
    int debug_function = 0; /*Change to 1 if you want to see the entire process*/

    /* build command frame */
    aCommand[0] = PHAL_MFULC_CMD_AUTH;
    aCommand[1] = 0;

    /* transmit the command frame for auth*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    /*printf("%dpRxBuffer", wRxLength); 							* This is sometimes useful to make sure nothing strange gets in
	for(i = 0; i < wRxLength; i++) printf(" %02x", pRxBuffer[i]);
	printf("\n");*/

    /* check received length */
    if (wRxLength != PHAL_MFULC_CHALL1_LENGTH)
    {
        return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFC);
    }

    /* copy received data block */
	memcpy(pBlockData, pRxBuffer, wRxLength);
	
	/* set schedules for encrypting and decrypting */
	three_des_key_schedule((unsigned char *) KeyULC3, schedule_en, 1);
	three_des_key_schedule((unsigned char *) KeyULC3, schedule_de, 0);

	memcpy(nonce1, &pBlockData[1], 8);
	/* decrypt step1 of authentication process */
	three_des_crypt((unsigned char *) nonce1, (unsigned char *) nonce1_de, schedule_de);
	
	memcpy(nonce1_proof, &nonce1_de[1], 7); /*Performs shift-left of 8bits (1B)*/
	nonce1_proof[7] = nonce1_de[0];
	
	for(i = 0; i < 8; i++)
	{									/*Random function can be substituted to be more random*/
		nonce2_chall[i] = (uint16_t) rand();	/*For now this will do*/
		nonce2_chall_cbc[i] = nonce2_chall[i] ^ nonce1[i]; /* Apply CBC algorithm - consists of XOR'ing current encoding with previous ciphertext */
	}
	
	/* encrypt RndA - step2 of authentication process */
	three_des_crypt((unsigned char *) nonce2_chall_cbc, (unsigned char *) nonce2_chall_en, schedule_en);
	
	
	/* Apply CBC algorithm - consists of XOR'ing current encoding with previous ciphertext */
	for(i = 0; i < 8; i++) nonce1_proof_cbc[i] = nonce1_proof[i] ^ nonce2_chall_en[i];
	
	/* encrypt RndB' - step2 of authentication process */
	three_des_crypt((unsigned char *) nonce1_proof_cbc, (unsigned char *) nonce1_proof_en, schedule_en);
	
	
    authStep2[0] = PHAL_MFULC_CMD_AUTH2; /* Protocol requires AFh then the 16 encrypted Bytes of challenge2 and proof of challenge1*/
    memcpy(&authStep2[1], nonce2_chall_en, 8); /*ek(RndA)*/
    memcpy(&authStep2[9], nonce1_proof_en, 8); /*ek(RndB')*/
    
    if(debug_function){
		printf("RndB Decrypted:");
		for (i = 0; i < 8; i++) printf(" %02x", nonce1_de[i]);
		puts("");
		
		printf("RndB':");
		for (i = 0; i < 8; i++) printf(" %02x", nonce1_proof[i]);
		puts("");
		
		printf("RndA:");
		for(i = 0; i < 8; i++) printf(" %02x", nonce2_chall[i]);
		puts("");
		
		printf("RndA CBC:");
		for (i = 0; i < 8; i++) printf(" %02x", nonce2_chall_cbc[i]);
		puts("");
		
		printf("ek(RndA):");
		for (i = 0; i < 8; i++) printf(" %02x", nonce2_chall_en[i]);
		puts("");
		
		printf("RndB' CBC:");
		for (i = 0; i < 8; i++) printf(" %02x", nonce1_proof_cbc[i]);
		puts("");
		
		printf("ek(RndB'):");
		for (i = 0; i < 8; i++) printf(" %02x", nonce1_proof_en[i]);
		puts("");
		
		printf("authStep2:");
		for(i = 0; i< 17; i++) printf(" %02x", authStep2[i]);
		puts("");
	}

    /* transmit the command frame */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        authStep2,
        17,
        &pRxBuffer,
        &wRxLength
        ));

        
    if (wRxLength != PHAL_MFULC_CHALL1_LENGTH) return PH_ADD_COMPCODE(PH_ERR_PROTOCOL_ERROR, PH_COMP_AL_MFC);
    
	memcpy(pBlockData, pRxBuffer, wRxLength);
	
	if (pBlockData[0] != PHAL_MFULC_CHALL2_RESP) return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFC);
	
	memcpy(nonce3_en, &pBlockData[1], 8);
	
	/* decrypt final step of authentication process */
	three_des_crypt((unsigned char *) nonce3_en, (unsigned char *) nonce3_de, schedule_de);
	
	/* Apply CBC algorithm - consists of XOR'ing current encoding with previous ciphertext */
	for (i = 0; i < 8; i++) nonce3_no_cbc[i] = nonce3_de[i] ^ nonce1_proof_en[i]; 
														  /* This removes the CBC */
	
	memcpy(&nonce3_no_cbc_unshifted[1], nonce3_no_cbc, 7); /*Performs shift-right of 8bits (1B)*/
	nonce3_no_cbc_unshifted[0] = nonce3_no_cbc[7];
	
	if(debug_function)
	{
		
		printf("ek(RndA'):");
		for (i = 0; i < 8; i++) printf(" %02x", nonce3_en[i]);
		puts("");
	
		printf("RndA' with cbc:");
		for (i = 0; i < 8; i++) printf(" %02x", nonce3_de[i]);
		puts("");
	
		printf("RndA':");
		for (i = 0; i < 8; i++) printf(" %02x", nonce3_no_cbc[i]);
		puts("");
	
		printf("RndA?????:");
		for (i = 0; i < 8; i++) printf(" %02x", nonce3_no_cbc_unshifted[i]);
		puts("");
	}
	
	for (i = 0; i < 8; i++)
	{
		if(nonce3_no_cbc_unshifted[i] != nonce2_chall[i])
			return PH_ADD_COMPCODE(PH_ERR_AUTH_ERROR, PH_COMP_AL_MFC);
	}
	
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC);
}

phStatus_t phalMfc_Sw_ReadValue(
                                phalMfc_Sw_DataParams_t * pDataParams,
                                uint8_t bBlockNo,
                                uint8_t * pValue,
                                uint8_t * pAddrData
                                )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bBuffer[PHAL_MFC_DATA_BLOCK_LENGTH];

    /* perform read operation */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_Sw_Read(pDataParams, bBlockNo, bBuffer));

    /* check format of value block */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_Int_CheckValueBlockFormat(bBuffer));

    /* return received contents */
    *pAddrData = bBuffer[12];
    memcpy(pValue, bBuffer, PHAL_MFC_VALUE_BLOCK_LENGTH);  /* PRQA S 3200 */

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC);
}

phStatus_t phalMfc_Sw_Write(
                            phalMfc_Sw_DataParams_t * pDataParams,
                            uint8_t bBlockNo,
                            uint8_t * pBlockData
                            )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM aCommand[2];
    uint8_t *   PH_MEMLOC_REM pRxBuffer;
    uint16_t    PH_MEMLOC_REM wRxLength;

    /* build command frame */
    aCommand[0] = PHAL_MFC_CMD_WRITE;
    aCommand[1] = bBlockNo;

    /* transmit the command frame (first part) */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        2,
        &pRxBuffer,
        &wRxLength
        ));

    /* transmit the data (second part) */
    PH_CHECK_SUCCESS_FCT(statusTmp, phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        pBlockData,
        PHAL_MFC_DATA_BLOCK_LENGTH,
        &pRxBuffer,
        &wRxLength
        ));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC);
}

phStatus_t phalMfc_Sw_WriteValue(
                                 phalMfc_Sw_DataParams_t * pDataParams,
                                 uint8_t bBlockNo,
                                 uint8_t * pValue,
                                 uint8_t bAddrData
                                 )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bBlockData[PHAL_MFC_VALUE_BLOCK_LENGTH];

    /* build a valid value block */
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_Int_CreateValueBlock(pValue, bAddrData,bBlockData ));
    
    /* perform the write operation */
    return phalMfc_Sw_Write(pDataParams, bBlockNo, bBlockData);
}

phStatus_t phalMfc_Sw_Increment(
                                phalMfc_Sw_DataParams_t * pDataParams,
                                uint8_t bBlockNo,
                                uint8_t * pValue
                                )
{
    return phalMfc_Int_Value(pDataParams->pPalMifareDataParams, PHAL_MFC_CMD_INCREMENT, bBlockNo, pValue);
}

phStatus_t phalMfc_Sw_Decrement(
                                phalMfc_Sw_DataParams_t * pDataParams,
                                uint8_t bBlockNo,
                                uint8_t * pValue
                                )
{
    return phalMfc_Int_Value(pDataParams->pPalMifareDataParams, PHAL_MFC_CMD_DECREMENT, bBlockNo, pValue);
}

phStatus_t phalMfc_Sw_Transfer(
                               phalMfc_Sw_DataParams_t * pDataParams,
                               uint8_t bBlockNo
                               )
{
    /* transmit the command frame */
    return phalMfc_Int_Transfer(
        pDataParams->pPalMifareDataParams,
        bBlockNo
        );
}

phStatus_t phalMfc_Sw_Restore(
                              phalMfc_Sw_DataParams_t * pDataParams,
                              uint8_t bBlockNo
                              )
{
    uint8_t PH_MEMLOC_REM bBuffer[PHAL_MFC_VALUE_BLOCK_LENGTH];

    /* restore needs four dummy bytes */
    memset(bBuffer, 0x00, PHAL_MFC_VALUE_BLOCK_LENGTH);  /* PRQA S 3200 */

    return phalMfc_Int_Value(pDataParams->pPalMifareDataParams, PHAL_MFC_CMD_RESTORE, bBlockNo, bBuffer);
}

phStatus_t phalMfc_Sw_IncrementTransfer(
                                        phalMfc_Sw_DataParams_t * pDataParams,
                                        uint8_t bSrcBlockNo,
                                        uint8_t bDstBlockNo,
                                        uint8_t * pValue
                                        )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_Sw_Increment(pDataParams, bSrcBlockNo, pValue));
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_Sw_Transfer(pDataParams, bDstBlockNo));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC);
}

phStatus_t phalMfc_Sw_DecrementTransfer(
                                        phalMfc_Sw_DataParams_t * pDataParams,
                                        uint8_t bSrcBlockNo,
                                        uint8_t bDstBlockNo,
                                        uint8_t * pValue
                                        )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_Sw_Decrement(pDataParams, bSrcBlockNo, pValue));
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_Sw_Transfer(pDataParams, bDstBlockNo));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC);
}

phStatus_t phalMfc_Sw_RestoreTransfer(
                                      phalMfc_Sw_DataParams_t * pDataParams,
                                      uint8_t bSrcBlockNo,
                                      uint8_t bDstBlockNo                                  
                                      )
{
    phStatus_t PH_MEMLOC_REM statusTmp;
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_Sw_Restore(pDataParams, bSrcBlockNo));
    PH_CHECK_SUCCESS_FCT(statusTmp, phalMfc_Sw_Transfer(pDataParams, bDstBlockNo));

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_AL_MFC);
}

phStatus_t phalMfc_Sw_PersonalizeUid(
                                     phalMfc_Sw_DataParams_t * pDataParams,
                                     uint8_t bUidType
                                     )
{
    uint8_t     PH_MEMLOC_REM aCommand[2];
    uint8_t     PH_MEMLOC_REM * pRecv;
    uint16_t    PH_MEMLOC_REM wRxlen;

    /* build command frame */
    aCommand[0] = PHAL_MFC_CMD_PERSOUID;
    aCommand[1] = bUidType;

    /* transmit the command frame */
    return phpalMifare_ExchangeL3(
        pDataParams->pPalMifareDataParams,
        PH_EXCHANGE_DEFAULT,
        aCommand,
        2,
        &pRecv,
        &wRxlen
        );
}

#endif /* NXPBUILD__PHAL_MFC_SW */
