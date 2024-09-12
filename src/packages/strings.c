//
// Created by Mahamadou DOUMBIA [OML DSI] on 28/02/2022.
//

#include "strings.h"
#include "tclGlobMatch.h"
#include <string.h>
#include "utf8.h"


// [str, strL, patt, pattL]
void stringsGlobMatch(Djuru *djuru) {
    const char* str = MSCGetSlotString(djuru, 1);
    int strLen = (int)MSCGetSlotDouble(djuru, 2);
    const char* patter = MSCGetSlotString(djuru, 3);
    int pattLen = (int)MSCGetSlotDouble(djuru, 4);
    int res = TclByteArrayMatch(str, strLen, patter, pattLen, 0);
    MSCSetSlotBool(djuru, 0, (bool)res);
}
/*
From: utf8.h README
Various functions provided will do case insensitive compares, or transform utf8 strings
from one case to another. Given the vastness of unicode, and the authors lack of understanding beyond latin
codepoints on whether case means anything, the following categories are the only ones that will be checked in case insensitive code:
    ASCII
    Latin-1 Supplement
    Latin Extended-A
    Latin Extended-B
    Greek and Coptic
    Cyrillic
*/
void stringsUpcase(Djuru *djuru) {
    const char* string = MSCGetSlotString(djuru, 1);
    size_t ssize = strlen(string);
    char *str = (char*)malloc(ssize + 1);
    memcpy(str, string, ssize);
    utf8upr(str);
    MSCSetSlotString(djuru, 0, str);
    free(str);
}
void stringsDowncase(Djuru *djuru) {
    const char* string = MSCGetSlotString(djuru, 1);
    size_t ssize = strlen(string);
    char *str = (char*)malloc(ssize + 1);
    memcpy(str, string, ssize);
    utf8lwr(str);
    MSCSetSlotString(djuru, 0, str);
    free(str);
}