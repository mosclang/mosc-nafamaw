//
// Created by Mahamadou DOUMBIA [OML DSI] on 25/02/2022.
//

#include "json.h"
#include "pdjson.h"

json_stream jsonStream[1];

void jsonStreamBegin(Djuru *djuru) {
    const char *value = MSCGetSlotString(djuru, 1);
    json_open_string(jsonStream, value);
    json_set_streaming(jsonStream, 0);
}

void jsonStreamEnd(Djuru *djuru) {
    json_reset(jsonStream);
    json_close(jsonStream);
}

void jsonStreamValue(Djuru *djuru) {
    const char *value = json_get_string(jsonStream, 0);
    MSCSetSlotString(djuru, 0, value);
}

void jsonStreamErrorMessage(Djuru *djuru) {
    const char *error = json_get_error(jsonStream);
    if (error) {
        MSCSetSlotString(djuru, 0, error);
        return;
    }
    MSCSetSlotString(djuru, 0, "");
}

void jsonStreamLineNumber(Djuru *djuru) {
    MSCSetSlotDouble(djuru, 0, json_get_lineno(jsonStream));
}

void jsonStreamPos(Djuru *djuru) {
    MSCSetSlotDouble(djuru, 0, json_get_position(jsonStream));
}

void jsonStreamNext(Djuru *djuru) {
    enum json_type type = json_next(jsonStream);
    // 0 in the enum seems to be reserved for no more tokens
    if (type > 0) {
        MSCSetSlotDouble(djuru, 0, type);
        return;
    }
    MSCSetSlotNull(djuru, 0);
}