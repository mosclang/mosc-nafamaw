#include "buffer.h"
#include <string.h>
#define POWER_OF2_CEIL(n) n--,n |= n >> 1,n |= n >> 2,n |= n >> 4,n |= n >> 8,n |= n >> 16,n++

static MSCHandle* bufferClass = NULL;
static int bufferCount = 0;
Buffer* newBuffer(int capacity) {
    Buffer* buffer = malloc(sizeof(Buffer));
    buffer->data = malloc(sizeof(char) * capacity);
    buffer->data[0]='\0';
    return buffer;
}
bool ensureBufferSize(Buffer* buffer, int length) {
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
bool bufferPush(Buffer* buffer, const char* data, int l) {
    // int l = strlen(data);
    if(!ensureBufferSize(buffer, l)) {
        return false;
    }
    memcpy(buffer->data + buffer->size, data, l);
    buffer->size+=l;
    buffer->data[buffer->size] = '\0';
    return true;
}
bool bufferUnshift(Buffer* buffer, const char* data, int l) {
    // int l = strlen(data);
    if(!ensureBufferSize(buffer, l)) {
        return false;
    }
    memmove(buffer->data + l, buffer->data, buffer->size);
    memcpy(buffer->data, data, l);
    buffer->size+=l;
    buffer->data[buffer->size] = '\0';
    return true;
}

bool bufferInsert(Buffer* buffer, long index, const char* data, int l) {
    // int l = strlen(data);
    if(!ensureBufferSize(buffer, l)) {
        return false;
    }
    memmove(buffer->data + index + l, buffer->data + index, buffer->size - index);
    memcpy(buffer->data + index, data, l);
    buffer->size+=l;
    buffer->data[buffer->size] = '\0';
    return true;
}
void bufferSize(Djuru* djuru) {
     Buffer* buffer = *(Buffer**)MSCGetSlotExtern(djuru, 0);
    MSCSetSlotDouble(djuru, 0, buffer->size);
}
void allocateBuffer(Djuru *djuru) {
    if(bufferClass == NULL) {
        MSCGetVariable(djuru, "nafamaw", "ByteArray", 0);
        bufferClass = MSCGetSlotHandle(djuru, 0);
    }
    MSCSetSlotHandle(djuru, 0, bufferClass);
    Buffer** buffer = (Buffer** )MSCSetSlotNewExtern(djuru, 0, 0, sizeof(Buffer*));
    (*buffer) = newBuffer(MSCGetSlotDouble(djuru, 1));
    (*buffer)->vm = MSCGetCurrentVm(djuru);
    bufferCount++;
}
bool bufferShrinkEnd(Buffer* buffer, long count) {
      if(count > buffer->size) {
        return false;
    }
    buffer->data[buffer->size - count] = '\0';
    buffer->size-=count;
    return true;
}
bool bufferShrinkStart(Buffer* buffer, long count) {
    if(count > buffer->size) {
        return false;
    }
    memmove(buffer->data, buffer->data + count, buffer->size - count);
    buffer->size-=count;
    return true;
}
bool bufferIsEmpty(Buffer* buffer) {
    return buffer->size == 0;
}

void bufferRange(Djuru* djuru) {
  Buffer* buffer = *(Buffer**)MSCGetSlotExtern(djuru, 0);
  int from = MSCGetSlotDouble(djuru, 1);
  int to = MSCGetSlotDouble(djuru, 2);
  int step = from < to ? 1 : - 1;
  int size = abs(to - from);
  char* res = malloc(sizeof(char) * (size + 1));
  for(int i = 0;i < size;i++) {
    res[i] = buffer->data[from + (i * step)];
  }
  res[size] = '\0';
  MSCSetSlotBytes(djuru, 0, res, size);
  free(res);
}
void bufferData(Djuru* djuru) {
     Buffer* buffer = *(Buffer**)MSCGetSlotExtern(djuru, 0);
     MSCSetSlotBytes(djuru, 0, buffer->data, buffer->size);
}
void appendBuffer(Djuru* djuru) {
    Buffer* buffer = *(Buffer**)MSCGetSlotExtern(djuru, 0);
    int slotType = MSCGetSlotType(djuru, 1);
    if(slotType == MSC_TYPE_STRING) {
      int size;
      const char* str = MSCGetSlotBytes(djuru, 1, &size);
      bufferPush(buffer, str, size);
    } else if(slotType == MSC_TYPE_NUM) {
      char b = (char)MSCGetSlotDouble(djuru, 1);
      bufferPush(buffer, &b, 1);
    }
}
void preppendBuffer(Djuru* djuru) {
    Buffer* buffer = *(Buffer**)MSCGetSlotExtern(djuru, 0);
    int slotType = MSCGetSlotType(djuru, 1);
    if(slotType == MSC_TYPE_STRING) {
      int size;
      const char* str = MSCGetSlotBytes(djuru, 1, &size);
      bufferUnshift(buffer, str, size);
    } else if(slotType == MSC_TYPE_NUM) {
      char b = (char)MSCGetSlotDouble(djuru, 1);
      bufferUnshift(buffer, &b, 1);
    }
}
void setAtBuffer(Djuru* djuru) {
     Buffer* buffer = *(Buffer**)MSCGetSlotExtern(djuru, 0);
     int slotType = MSCGetSlotType(djuru, 2);
     if(slotType == MSC_TYPE_STRING) {
      int size;
        const char* str = MSCGetSlotBytes(djuru, 2, &size);
        bufferInsert(buffer, (long)MSCGetSlotDouble(djuru, 1), str, size);
     } else if(slotType == MSC_TYPE_NUM) {
        char b = (char)MSCGetSlotDouble(djuru, 2);
        bufferInsert(buffer, (long)MSCGetSlotDouble(djuru, 1), &(b), 1);
     }
}
void shrinkEndBuffer(Djuru* djuru) {
     Buffer* buffer = *(Buffer**)MSCGetSlotExtern(djuru, 0);
     bufferShrinkEnd(buffer, (long)MSCGetSlotDouble(djuru, 1));
    
}
void shrinkStartBuffer(Djuru* djuru) {
     Buffer* buffer = *(Buffer**)MSCGetSlotExtern(djuru, 0);
    bufferShrinkStart(buffer,  (long)MSCGetSlotDouble(djuru, 1));
}
void isEmptyBuffer(Djuru* djuru) {
     Buffer* buffer = *(Buffer**)MSCGetSlotExtern(djuru, 0);
     MSCSetSlotBool(djuru, 0, bufferIsEmpty(buffer));
}
void releaseBuffer(void* data) {
    Buffer* buffer = *(Buffer**)data;
    bufferCount--;
    if(bufferCount == 0 && buffer->vm != NULL) {
        // no more buffer, release bufferClass handle
        MSCReleaseHandle(buffer->vm, bufferClass);
        bufferClass = NULL;
    }
    free(buffer->data);
    free(buffer);
}
