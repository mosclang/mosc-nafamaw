//
// Created by Mahamadou DOUMBIA [OML DSI] on 25/02/2022.
//

#include "waati.h"
#include "uv.h"

/**
 * Get the current timestamp since epoch (1rst Jan 1970) through uv_hrtime
 * @param djuru
 */
void currentHighTimestamp(Djuru *djuru) {
    MSCEnsureSlots(djuru, 1);
    uint64_t time = uv_hrtime();
    MSCSetSlotDouble(djuru, 0, time);

}

/**
 * returns milliseconds like JavaScript
 * @param djuru
 */
void currentTimestamp(Djuru *djuru) {
    uv_timeval64_t time;
    MSCEnsureSlots(djuru, 1);
    uv_gettimeofday(&time);
    MSCSetSlotDouble(djuru, 0, time.tv_sec * 1000 + time.tv_usec / 1000);
}