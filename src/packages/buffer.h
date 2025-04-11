#ifndef MOSC_NAFAMAW_STRINGBUFFER_H
#define MOSC_NAFAMAW_STRINGBUFFER_H
#include "msc.h"
typedef struct {
    MVM* vm;
    long size;
    long capacity;
    char* data;
} Buffer;

Buffer* newBuffer(int capacity);
bool ensureBufferSize(Buffer* buffer, int length);
bool bufferPush(Buffer* buffer, const char* data, int l);
bool bufferUnshift(Buffer* buffer, const char* data, int l);
bool bufferInsert(Buffer* buffer, long index, const char* data, int l);
bool bufferShrinkEnd(Buffer* buffer, long count);
bool bufferShrinkStart(Buffer* buffer, long count);
bool bufferIsEmpty(Buffer* buffer);
void allocateBuffer(Djuru *djuru);
void appendBuffer(Djuru* djuru);
void preppendBuffer(Djuru* djuru);
void setAtBuffer(Djuru* djuru);
void bufferSize(Djuru* djuru);
void bufferRange(Djuru* djuru);
void bufferData(Djuru* djuru);
void shrinkEndBuffer(Djuru* djuru);
void shrinkStartBuffer(Djuru* djuru);
void isEmptyBuffer(Djuru* djuru);
void releaseBuffer(void* data);


#endif //MOSC_NAFAMAW_STRINGBUFFER_H