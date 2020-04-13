
#define BUFF_UNIT   uint32_t

typedef struct  {
    BUFF_UNIT* buff;
    uint32_t size;
    uint32_t buff_size;
    uint32_t head;
    uint32_t tail;
}QUE_STRUCT;

QUE_STRUCT que_1;

#define MAX_BUFF_SIZE   9600

typedef enum {
    QUE_ERROR_NOT_INITED = -1,
    QUE_ERROR_INIT_FAIL = -2,
    QUE_ERROR_BUFF = -3,
    QUE_ERROR_SIZE = -4,
}QUE_ERROR_x;

/**
  * @brief  Initializing Queue
  * @param  *pQue_Str   : Target Queue Struct
  * @param  size        : Queue Buffer Size
  * @retval size        : Queue Data Size   ( > 0 )
                        : QUE_ERROR_x       ( < 0 )
  */
static int32_t InitQue(QUE_STRUCT* pQue_Str, uint16_t size)
{
    if (size > MAX_BUFF_SIZE)
        return QUE_ERROR_SIZE;

    pQue_Str->buff = NULL;
    pQue_Str->buff = (BUFF_UNIT*)calloc(sizeof(BUFF_UNIT), size);

    if (pQue_Str->buff == NULL)
        return QUE_ERROR_INIT_FAIL;

    pQue_Str->buff_size = size;
    
    pQue_Str->size = 0;
    pQue_Str->head = 0;
    pQue_Str->tail = 0;

    return size;
}


/**
  * @brief  enqueue buff data
  * @param  *pQue_Str   : Target Queue Struct
  * @param  *pdata      : Data to be enqueue
  * @param  size        : Size of data to enqueue
  * @retval size        : Queue Data Size   ( > 0 )
                        : QUE_ERROR_x       ( < 0 )
  */
static int32_t enque(QUE_STRUCT* pQue_Str, BUFF_UNIT* pdata, int32_t size)
{
    int16_t freeSize = (pQue_Str->buff_size - pQue_Str->size);

    if (pQue_Str->buff == NULL)
        return QUE_ERROR_NOT_INITED;

    if (pQue_Str->buff_size == 0)
        return QUE_ERROR_NOT_INITED;

    if (freeSize < size)
        return QUE_ERROR_SIZE;

    if ((pQue_Str->head + size) <= pQue_Str->buff_size)
    {
        memcpy(&pQue_Str->buff[pQue_Str->head], pdata, sizeof(BUFF_UNIT) * size);
        pQue_Str->head += size;
    }
    else {
        int start_Idx = pQue_Str->head;
        int section1Size = pQue_Str->buff_size - pQue_Str->head;
        int section2Size = size - section1Size;
        memcpy(&pQue_Str->buff[pQue_Str->head], &pdata[0], sizeof(BUFF_UNIT) * section1Size);

        pQue_Str->head += section1Size;
        if (pQue_Str->head >= pQue_Str->buff_size)
            pQue_Str->head = 0;

        memcpy(&pQue_Str->buff[pQue_Str->head], &pdata[section1Size], sizeof(BUFF_UNIT) * section2Size);
        pQue_Str->head += section2Size;
        if (pQue_Str->head >= pQue_Str->buff_size)
            pQue_Str->head = 0;
    }
    pQue_Str->size += size;

    return pQue_Str->size;
}


/**
  * @brief  enqueue single Data
  * @param  *pQue_Str   : Target Queue Struct
  * @param  data        : Data to be enqueue
  * @retval size        : Queue Data Size   ( > 0 )
                        : QUE_ERROR_x       ( < 0 )
  */
static int32_t enque(QUE_STRUCT* pQue_Str, BUFF_UNIT data)
{
    int16_t freeSize = (pQue_Str->buff_size - pQue_Str->size);

    if (pQue_Str->buff == NULL)
        return QUE_ERROR_NOT_INITED;

    if (pQue_Str->buff_size == 0)
        return QUE_ERROR_NOT_INITED;

    if (freeSize < 1)
        return QUE_ERROR_SIZE;

    pQue_Str->buff[pQue_Str->head] = data;
    pQue_Str->head += 1;
    if (pQue_Str->head >= pQue_Str->buff_size)
        pQue_Str->head = 0;

    pQue_Str->size++;

    return pQue_Str->size;
}



/**
  * @brief  dequeue buff data
  * @param  *pQue_Str   : Target Queue Struct
  * @param  *pdata      : Pointer of dequeue data
  * @param  size        : Size of data to dequeue
  * @retval size        : Queue Data Size   ( > 0 )
                        : QUE_ERROR_x       ( < 0 )
  */ 
static int32_t deque(QUE_STRUCT* pQue_Str, BUFF_UNIT* pdata, int32_t size)
{
    if (pQue_Str->buff == NULL)
        return QUE_ERROR_NOT_INITED;

    if (pQue_Str->buff_size == 0)
        return QUE_ERROR_NOT_INITED;

    if (pQue_Str->size < size)
        return QUE_ERROR_SIZE;

    if ((pQue_Str->tail + size) <= pQue_Str->buff_size)
    {
        int start_Idx = pQue_Str->tail;
        memcpy(pdata , &pQue_Str->buff[start_Idx], sizeof(BUFF_UNIT) * size);
        pQue_Str->tail += size;
    }
    else {
        int start_Idx = pQue_Str->tail;
        int section1Size = pQue_Str->buff_size - pQue_Str->tail;
        int section2Size = size - section1Size;
        memcpy(&pdata[0], &pQue_Str->buff[start_Idx], sizeof(BUFF_UNIT) * section1Size);

        pQue_Str->tail += section1Size;
        if (pQue_Str->tail >= pQue_Str->buff_size)
            pQue_Str->tail = 0;

        memcpy(&pdata[section1Size], &pQue_Str->buff[pQue_Str->tail], sizeof(BUFF_UNIT) * section2Size);

        pQue_Str->tail += section2Size;
        if (pQue_Str->tail >= pQue_Str->buff_size)
            pQue_Str->tail = 0;
    }
    pQue_Str->size += size;

    return pQue_Str->size;
}


/**
  * @brief  dequeue single data
  * @param  *pQue_Str   : Target Queue Struct
  * @param  *pdata      : Pointer of dequeue data
  * @retval size        : Queue Data Size   ( > 0 )
                        : QUE_ERROR_x       ( < 0 )
  */
static int32_t deque(QUE_STRUCT* pQue_Str, BUFF_UNIT* pdata)
{
    if (pQue_Str->buff == NULL)
        return QUE_ERROR_NOT_INITED;

    if (pQue_Str->buff_size == 0)
        return QUE_ERROR_NOT_INITED;
    
    if (pQue_Str->size < 1)
        return QUE_ERROR_SIZE;

    *pdata = pQue_Str->buff[pQue_Str->tail];
    pQue_Str->buff[pQue_Str->tail] = NULL;
    pQue_Str->tail++;
    if (pQue_Str->tail >= pQue_Str->buff_size)
        pQue_Str->tail = 0;

    pQue_Str->size--;

    return pQue_Str->size;
}
