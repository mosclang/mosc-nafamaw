#ifndef MOSC_NAFAMAW_STRINGBUFFER_H
#define MOSC_NAFAMAW_STRINGBUFFER_H
#include "msc.h"
typedef struct {
    MVM* vm;
    long size;
    long capacity;
    char* data;
} StringBuffer;

StringBuffer* newStringBuffer(int capacity);
bool ensureBufferSize(StringBuffer* buffer, int length);
bool stringBufferPush(StringBuffer* buffer, const char* data);
bool stringBufferUnshift(StringBuffer* buffer, const char* data);
bool stringBufferInsert(StringBuffer* buffer, long index, const char* data);
bool stringBufferShrinkEnd(StringBuffer* buffer, long count);
bool stringBufferShrinkStart(StringBuffer* buffer, long count);
bool stringBufferIsEmpty(StringBuffer* buffer);
void allocateStringBuffer(Djuru *djuru);
void appendStringBuffer(Djuru* djuru);
void preppendStringBuffer(Djuru* djuru);
void setAtStringBuffer(Djuru* djuru);
void stringBufferSize(Djuru* djuru);
void stringBufferData(Djuru* djuru);
void shrinkEndStringBuffer(Djuru* djuru);
void shrinkStartStringBuffer(Djuru* djuru);
void isEmptyStringBuffer(Djuru* djuru);
void releaseStringBuffer(void* data);


#endif //MOSC_NAFAMAW_STRINGBUFFER_H