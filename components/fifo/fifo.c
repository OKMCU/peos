/*******************************************************************************
 * Copyright (c) 2019-2020, Single-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Change Logs:
 * Date         Author       Notes
 * 2019-10-28   Wentao SUN   first version
 *
 ******************************************************************************/
 
/* Includes ------------------------------------------------------------------*/
#include "st.h"
#include "components/fifo/fifo.h"

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct FIFOPage {
    st_uint8_t buf[FIFO_PAGE_SIZE];
    st_uint16_t head;
    st_uint16_t tail;
    struct FIFOPage *nextPage;
} FIFOPage_t;

typedef struct {
    st_uint32_t datalen;
    FIFOPage_t *headPage;
    FIFOPage_t *tailPage;
} FIFOHandle_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported function implementations -----------------------------------------*/
void *fifo_create(void)
{
    void *handle;
    
    handle = st_mem_alloc(sizeof(FIFOHandle_t));
    if(handle != NULL)
    {
        st_memset(handle, 0x00, sizeof(FIFOHandle_t));
    }
    
    return handle;
}

void fifo_delete(void *fifo)
{
    FIFOHandle_t *fifoHandle = NULL;
    FIFOPage_t *fifoPageSav = NULL;
    fifoHandle = (FIFOHandle_t *)fifo;
    
    while(fifoHandle->headPage != NULL)
    {
        fifoPageSav = fifoHandle->headPage->nextPage;
        st_mem_free(fifoHandle->headPage);
        fifoHandle->headPage = fifoPageSav;
    }
    
    st_mem_free(fifoHandle);
}

st_uint8_t *fifo_put(void *fifo, st_uint8_t byte)
{
    st_uint8_t *pos;
    FIFOHandle_t *fifoHandle = NULL;
    FIFOPage_t *fifoPage = NULL;
    fifoHandle = (FIFOHandle_t *)fifo;

    if(fifoHandle->tailPage == NULL)
    {
        ST_ASSERT(fifoHandle->headPage == NULL);
        fifoPage = st_mem_alloc(sizeof(FIFOPage_t));
        if(fifoPage == NULL)
        {
            return NULL;
        }
        fifoPage->head = 0;
        fifoPage->tail = 0;
        fifoPage->nextPage = NULL;
        fifoHandle->headPage = fifoPage;
        fifoHandle->tailPage = fifoPage;
    }

    if(fifoHandle->tailPage->tail >= FIFO_PAGE_SIZE)
    {
        ST_ASSERT(fifoHandle->tailPage->nextPage == NULL);
        fifoPage = st_mem_alloc(sizeof(FIFOPage_t));
        if(fifoPage == NULL)
        {
            return NULL;
        }
        fifoPage->head = 0;
        fifoPage->tail = 0;
        fifoPage->nextPage = NULL;
        fifoHandle->tailPage->nextPage = fifoPage;
        fifoHandle->tailPage = fifoPage;
        
    }

    pos = fifoHandle->tailPage->buf + fifoHandle->tailPage->tail;
    *pos = byte;
    fifoHandle->tailPage->tail++;
    fifoHandle->datalen++;

    return pos;
    
}

st_uint32_t fifo_len(void *fifo)
{
    return ((FIFOHandle_t *)fifo)->datalen;
}

st_uint8_t fifo_get(void *fifo)
{
    st_uint8_t u8tmp = 0;
    FIFOHandle_t *fifoHandle = NULL;
    FIFOPage_t *fifoPage = NULL;
    fifoHandle = (FIFOHandle_t *)fifo;

    if(fifoHandle->datalen > 0)
    {
        u8tmp = fifoHandle->headPage->buf[fifoHandle->headPage->head++];
        fifoHandle->datalen--;
        if(fifoHandle->headPage->head == fifoHandle->headPage->tail)
        {
            fifoPage = fifoHandle->headPage->nextPage;
            st_mem_free(fifoHandle->headPage);
            fifoHandle->headPage = fifoPage;
            if(fifoHandle->headPage == NULL)
            {
                fifoHandle->tailPage = NULL;
            }
        }
    }
    return u8tmp;
}

/* Private function implementations ------------------------------------------*/

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
