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
 * BAL Stub Component of Reader Library Framework.
 * $Author: nxp40786 $
 * $Revision: 332 $
 * $Date: 2013-08-07 17:57:29 +0530 (Wed, 07 Aug 2013) $
 *
 * History:
 *  CHu: Generated 19. May 2009
 *
 */

#include <ph_Status.h>
#include <phbalReg.h>
#include <ph_RefDefs.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>

#ifdef NXPBUILD__PHBAL_REG_STUB

#include "phbalReg_Stub.h"

phStatus_t phbalReg_Stub_Init(phbalReg_Stub_DataParams_t * pDataParams, uint16_t wSizeOfDataParams)
{
  if(sizeof(phbalReg_Stub_DataParams_t) != wSizeOfDataParams) {
    return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
  }PH_ASSERT_NULL (pDataParams);

  pDataParams->wId = PH_COMP_BAL | PHBAL_REG_STUB_ID;

  pDataParams->fd = -1;

  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_GetPortList(phbalReg_Stub_DataParams_t * pDataParams, uint16_t wPortBufSize, uint8_t * pPortNames, uint16_t * pNumOfPorts)
{
  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_SetPort(phbalReg_Stub_DataParams_t * pDataParams, uint8_t * pPortName)
{
  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_OpenPort(phbalReg_Stub_DataParams_t * pDataParams)
{
  printf("Opening Port\n");
  struct termios options;
  int fd;
  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
  if(fd == -1) {
    return PH_ADD_COMPCODE(PH_ERR_NOT_INITIALISED, PH_COMP_BAL);
  }

  tcgetattr(fd, &options);

  cfsetispeed(&options, B115200);         /*IO speed 115.2kBps*/
  cfsetospeed(&options, B115200);

  options.c_cflag |= (CLOCAL | CREAD);    /*Receiver and local mode*/

  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;

  options.c_cflag &= ~PARENB;              /*No parity*/
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;

  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); /*Send raw data*/

  options.c_oflag &= ~OPOST;              /*Receive raw data*/

  options.c_cflag &= ~CRTSCTS;
  options.c_iflag &= ~(IXON | IXOFF | IXANY);
  options.c_iflag &= ~(INPCK | ISTRIP);
  options.c_iflag &= ~(INLCR | IGNCR | ICRNL);

  tcsetattr(fd, TCSANOW, &options);

  pDataParams->fd = fd;
  printf("Port opened\n");

  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_ClosePort(phbalReg_Stub_DataParams_t * pDataParams)
{
  if(pDataParams->fd != -1) {
    close(pDataParams->fd);
  }

  pDataParams->fd = -1;

  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_Exchange(phbalReg_Stub_DataParams_t * pDataParams, uint16_t wOption, uint8_t * pTxBuffer, uint16_t wTxLength, uint16_t wRxBufSize,
    uint8_t * pRxBuffer, uint16_t * pRxLength)
{
  ssize_t n;
  struct pollfd fds[1];

  if(pDataParams->fd == -1) {
    return PH_ADD_COMPCODE(PH_ERR_NOT_INITIALISED, PH_COMP_BAL);
  }
  //#define MYDEBUG
  if(wTxLength > 0) {
	 #ifdef MYDEBUG
     printf("Sending %d byte: [", wTxLength);
     for(int i = 0; i < wTxLength; i++) {
     printf("%02X", pTxBuffer[i]);
     }
     printf("]\n");
     #endif
    n = write(pDataParams->fd, pTxBuffer, wTxLength);
    if(n < 0) {
      printf("Error in write: %s\n", strerror(errno));
      return PH_ADD_COMPCODE(PH_ERR_FAILED, PH_COMP_BAL);
    }
  }

  if(wRxBufSize > 0) {
    fds[0].fd = pDataParams->fd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    n = poll(fds, 1, 1000);
    if(n < 0) {
      printf("Error in poll: %s\n", strerror(errno));
      *pRxLength = 0;
      return PH_ADD_COMPCODE(PH_ERR_FAILED, PH_COMP_BAL);
    }
    if(n > 0) {
    #ifdef MYDEBUG
      printf("Expecting %d byte\n", wRxBufSize);
    #endif
      n = read(pDataParams->fd, pRxBuffer, wRxBufSize);
      if(n < 0) {
        printf("Error in read: %s\n", strerror(errno));
        *pRxLength = 0;
        return PH_ADD_COMPCODE(PH_ERR_FAILED, PH_COMP_BAL);
      }
      #ifdef MYDEBUG
      if(n > 0) {
        printf("Got: [");
        for(i = 0; i < n; i++) {
          printf("%02X", pRxBuffer[i]);
        }
        printf("]\n");
      }
      #endif
      *pRxLength = n;
    }
    else {
      printf("Timeout Cmd:[%02X]\n", pTxBuffer[0]);
      printf("Expecting %d byte\n", wRxBufSize);
      *pRxLength = 0;
    }
  }

  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_SetConfig(phbalReg_Stub_DataParams_t * pDataParams, uint16_t wConfig, uint16_t wValue)
{
  switch(wConfig) {
    case PHBAL_REG_CONFIG_HAL_HW_TYPE:
      if((wValue != PHBAL_REG_HAL_HW_RC523) && (wValue != PHBAL_REG_HAL_HW_RC663)) {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_BAL);
      }
      pDataParams->wHalType = (uint8_t)wValue;

      break;

    default:
      return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_BAL);
  }

  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_GetConfig(phbalReg_Stub_DataParams_t * pDataParams, uint16_t wConfig, uint16_t * pValue)
{
  return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

#endif /* NXPBUILD__PHBAL_REG_STUB */
