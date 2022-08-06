//
// Created by Mahamadou DOUMBIA [OML DSI] on 25/02/2022.
//

#include "waati.h"
#include "uv.h"

/**
 * Get the current timestamp since epoch (1rst Jan 1970) through uv_hrtime
 * @param vm
 */
void currentHighTimestamp(MVM *vm) {
    MSCEnsureSlots(vm, 1);
    uint64_t time = uv_hrtime();
    MSCSetSlotDouble(vm, 0, time);

}

/**
 * returns milliseconds like JavaScript
 * @param vm
 */
void currentTimestamp(MVM *vm) {
    uv_timeval64_t time;
    MSCEnsureSlots(vm, 1);
    uv_gettimeofday(&time);
    MSCSetSlotDouble(vm, 0, time.tv_sec * 1000 + time.tv_usec / 1000);
}