#include "string_buffer.h"
#include <string.h>
#define POWER_OF2_CEIL(n) n--,n |= n >> 1,n |= n >> 2,n |= n >> 4,n |= n >> 8,n |= n >> 16,n++

static MSCHandle* stringBufferClass = NULL;
static int stringBufferCount = 0;
StringBuffer* newStringBuffer(int capacity) {
    StringBuffer* buffer = malloc(sizeof(StringBuffer));
    buffer->data = malloc(sizeof(char) * capacity);
    buffer->data[0]='\0';
    return buffer;
}
bool ensureBufferSize(StringBuffer* buffer, int length) {
    if(buffer->size + length >= buffer->capacity) {
        // increase capacity
        int n = buffer->capacity + length;
        POWER_OF2_CEIL(n);
        buffer->data = (char*)realloc(buffer->data, sizeof(char) * n);
        if(buffer->data == NULL) {
            return false;
        }
        buffer->capacity = n;
    }
    return true;
}
bool stringBufferPush(StringBuffer* buffer, const char* data) {
    int l = strlen(data);
    if(!ensureBufferSize(buffer, l)) {
        return false;
    }
    memcpy(buffer->data + buffer->size, data, l);
    buffer->size+=l;
    buffer->data[buffer->size] = '\0';
    return true;
}
bool stringBufferUnshift(StringBuffer* buffer, const char* data) {
    int l = strlen(data);
    if(!ensureBufferSize(buffer, l)) {
        return false;
    }
    memmove(buffer->data + l, buffer->data, buffer->size);
    memcpy(buffer->data, data, l);
    buffer->size+=l;
    buffer->data[buffer->size] = '\0';
    return true;
}
bool stringBufferInsert(StringBuffer* buffer, long index, const char* data) {
    int l = strlen(data);
    if(!ensureBufferSize(buffer, l)) {
        return false;
    }
    memmove(buffer->data + index + l, buffer->data + index, buffer->size - index);
    memcpy(buffer->data + index, data, l);
    buffer->size+=l;
    buffer->data[buffer->size] = '\0';
    return true;
}
void stringBufferSize(Djuru* djuru) {
     StringBuffer* buffer = *(StringBuffer**)MSCGetSlotExtern(djuru, 0);
    MSCSetSlotDouble(djuru, 0, buffer->size);
}
void allocateStringBuffer(Djuru *djuru) {
    if(stringBufferClass == NULL) {
        MSCGetVariable(djuru, "nafamaw", "StringBuffer", 0);
        stringBufferClass = MSCGetSlotHandle(djuru, 0);
    }
    MSCSetSlotHandle(djuru, 0, stringBufferClass);
    StringBuffer** buffer = (StringBuffer** )MSCSetSlotNewExtern(djuru, 0, 0, sizeof(StringBuffer*));
    (*buffer) = newStringBuffer(MSCGetSlotDouble(djuru, 1));
    (*buffer)->vm = MSCGetCurrentVm(djuru);
    stringBufferCount++;
}
bool stringBufferShrinkEnd(StringBuffer* buffer, long count) {
      if(count > buffer->size) {
        return false;
    }
    buffer->data[buffer->size - count] = '\0';
    buffer->size-=count;
    return true;
}
bool stringBufferShrinkStart(StringBuffer* buffer, long count) {
    if(count > buffer->size) {
        return false;
    }
    memmove(buffer->data, buffer->data + count, buffer->size - count);
    buffer->size-=count;
    return true;
}
bool stringBufferIsEmpty(StringBuffer* buffer) {
    return buffer->size == 0;
}

void stringBufferData(Djuru* djuru) {
     StringBuffer* buffer = *(StringBuffer**)MSCGetSlotExtern(djuru, 0);
     MSCSetSlotString(djuru, 0, buffer->data);
}
void appendStringBuffer(Djuru* djuru) {
     StringBuffer* buffer = *(StringBuffer**)MSCGetSlotExtern(djuru, 0);
     stringBufferPush(buffer, MSCGetSlotString(djuru, 1));
}
void preppendStringBuffer(Djuru* djuru) {
     StringBuffer* buffer = *(StringBuffer**)MSCGetSlotExtern(djuru, 0);
     stringBufferUnshift(buffer, MSCGetSlotString(djuru, 1));
}
void setAtStringBuffer(Djuru* djuru) {
     StringBuffer* buffer = *(StringBuffer**)MSCGetSlotExtern(djuru, 0);
     stringBufferInsert(buffer, (long)MSCGetSlotDouble(djuru, 1), MSCGetSlotString(djuru, 2));
}
void shrinkEndStringBuffer(Djuru* djuru) {
     StringBuffer* buffer = *(StringBuffer**)MSCGetSlotExtern(djuru, 0);
     stringBufferShrinkEnd(buffer, (long)MSCGetSlotDouble(djuru, 1));
    
}
void shrinkStartStringBuffer(Djuru* djuru) {
     StringBuffer* buffer = *(StringBuffer**)MSCGetSlotExtern(djuru, 0);
    stringBufferShrinkStart(buffer,  (long)MSCGetSlotDouble(djuru, 1));
}
void isEmptyStringBuffer(Djuru* djuru) {
     StringBuffer* buffer = *(StringBuffer**)MSCGetSlotExtern(djuru, 0);
     MSCSetSlotBool(djuru, 0, stringBufferIsEmpty(buffer));
}
void releaseStringBuffer(void* data) {
    StringBuffer* buffer = *(StringBuffer**)data;
    stringBufferCount--;
    if(stringBufferCount == 0 && buffer->vm != NULL) {
        // no more buffer, release stringBufferClass handle
        MSCReleaseHandle(buffer->vm, stringBufferClass);
        stringBufferClass = NULL;
    }
    free(buffer->data);
    free(buffer);
}
