/*
 *         Copyright (c), NXP Semiconductors Bangalore / India
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
 * STUB OSAL Component of Reader Library Framework.
 * $Author: nxp53811 $
 * $Revision:  $
 * $Date: 2013-09-25 $
 *
 * History:
 *
 */

#include <stdio.h>
#include <ph_Status.h>
#include <ph_NxpBuild.h>

#ifdef NXPBUILD__PH_OSAL_STUB

#include <phOsal.h>
#include "phOsal_Stub.h"

/*==============================================================================================
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_Stub_Init(phOsal_Stub_DataParams_t * pDataParams)
{
  pDataParams->wId = PH_COMP_OSAL | PH_OSAL_STUB_ID;
  // printf("->%s\n", __PRETTY_FUNCTION__);
  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

/*==============================================================================================
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_Stub_Timer_Init(phOsal_Stub_DataParams_t * pDataParams)
{
  // printf("->%s\n", __PRETTY_FUNCTION__);
  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

/*==============================================================================================
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_Stub_Timer_Create(phOsal_Stub_DataParams_t *pDataParams, uint32_t * pTimerId)
{
  *pTimerId = 0;
  printf ("->%s\n", __PRETTY_FUNCTION__);
  if(*pTimerId == -1) {
    /* No timer found, need to return error */
    return PH_ADD_COMPCODE(PH_OSAL_ERR_NO_FREE_TIMER, PH_COMP_OSAL);
  }
  else {
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
  }
}

/*==============================================================================================
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_Stub_Timer_Reset(phOsal_Stub_DataParams_t *pDataParams, uint32_t dwTimerId)
{
  // printf ("->%s\n", __PRETTY_FUNCTION__);
  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

/*==============================================================================================
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_Stub_Timer_Wait(phOsal_Stub_DataParams_t * pDataParams, uint8_t bTimerDelayUnit, uint16_t wDelay)
{
  struct timespec t;

  if(bTimerDelayUnit == PH_OSAL_TIMER_UNIT_MS) {
    t.tv_sec = wDelay / 1000;
    t.tv_nsec = (wDelay % 1000) * 1000 * 1000;
    /* Delay in terms of Milli seconds */
  }
  else {
    /* Delay in terms of Micro seconds */
    t.tv_sec = 0;
    t.tv_nsec = wDelay * 1000;
  }

  nanosleep(&t, NULL);

  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

/*==============================================================================================
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 *    16 bit timer is possible to set in this range:
 *       microseconds:  min. - real minimum setting time is 2us
 *                      max. - maximum time setting is 1350ms (theoretical 1365us)
 *       miliseconds:   min. -
 *
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_Stub_Timer_Start(phOsal_Stub_DataParams_t * pDataParams, uint32_t dwTimerId, uint32_t dwRegTimeCnt, uint16_t wOption,
    ppCallBck_t pApplicationCallback, void *pContext)
{
  phStatus_t status;
  printf ("->%s\n", __PRETTY_FUNCTION__);
  status = PH_ERR_SUCCESS;
  if(status == PH_ERR_SUCCESS) {
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
  }
  else {
    return PH_ADD_COMPCODE(PH_OSAL_ERR_INVALID_TIMER, PH_COMP_OSAL);
  }
}

/*==============================================================================================
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_Stub_Timer_Stop(phOsal_Stub_DataParams_t * pDataParams, uint32_t dwTimerId)
{
  phStatus_t status;
  printf ("->%s\n", __PRETTY_FUNCTION__);
  status = PH_ERR_SUCCESS;
  if(status == PH_ERR_SUCCESS) {
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
  }
  else {
    return PH_ADD_COMPCODE(PH_OSAL_ERR_INVALID_TIMER, PH_COMP_OSAL);
  }
}

/*==============================================================================================
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_Stub_Timer_Delete(phOsal_Stub_DataParams_t *pDataParams, uint32_t dwTimerId)
{
  phStatus_t status;
  printf ("->%s\n", __PRETTY_FUNCTION__);
  status = PH_ERR_SUCCESS;
  if(status == PH_ERR_INTERNAL_ERROR) {
    return PH_ADD_COMPCODE(PH_OSAL_ERR_INVALID_TIMER, PH_COMP_OSAL);
  }
  else {
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
  }
}

/*==============================================================================================
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_Stub_Timer_ExecCallback(phOsal_Stub_DataParams_t *pDataParams, uint32_t dwTimerId)
{
  phStatus_t status = PH_ERR_SUCCESS;
  printf ("->%s\n", __PRETTY_FUNCTION__);
  if(dwTimerId < 10) {
    /* Call the call back function */
    if(pDataParams->gTimers[dwTimerId].pApplicationCallback != NULL) {
      (pDataParams->gTimers[dwTimerId].pApplicationCallback)(dwTimerId, pDataParams->gTimers[dwTimerId].pContext);
    }
  }

  status = PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
  return status;
}

/*==============================================================================================
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_Stub_GetMemory(phOsal_Stub_DataParams_t *pDataParams, uint32_t dwLength, void ** pMem)
{
  phStatus_t status = PH_ERR_SUCCESS;

  PHOSAL_UNUSED_VARIABLE(pDataParams);

  *pMem = (void *)malloc(dwLength);

  if(*pMem == NULL) {
    status = PH_ERR_RESOURCE_ERROR;
  }

  return status;
}

/*==============================================================================================
 * FUNCTION:
 *
 * DESCRIPTION:
 *
 ---------------------------------------------------------------------------------------------*/
phStatus_t phOsal_Stub_FreeMemory(phOsal_Stub_DataParams_t *pDataParams, void * ptr)
{
  phStatus_t status = PH_ERR_SUCCESS;

  PHOSAL_UNUSED_VARIABLE(pDataParams);
  if(NULL != ptr)
    free(ptr);

  return status;
}

#endif /* NXPBUILD__PH_OSAL_STUB */

/*==============================================================================================
 * 		End of file
 =============================================================================================*/

