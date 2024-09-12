//
// Created by Mahamadou DOUMBIA [OML DSI] on 22/02/2022.
//

#include "dumare.h"



#include <string.h>
#include <Value.h>

#include "../runtime/MVM.h"




static Class *mirrorGetSlotClass(Djuru *djuru, int slot) {
    Value classVal = *MSCSlotAtUnsafe(djuru, slot);
    if (!IS_CLASS(classVal)) return NULL;

    return AS_CLASS(classVal);
}
static List *mirrorGetSlotList(Djuru *djuru, int slot) {
    Value classVal = *MSCSlotAtUnsafe(djuru, slot);
    if (!IS_LIST(classVal)) return NULL;

    return AS_LIST(classVal);
}

static Djuru *mirrorGetSlotFiber(Djuru *djuru, int slot) {
    Value fiberVal = *MSCSlotAtUnsafe(djuru, slot);
    if (!IS_FIBER(fiberVal)) return NULL;

    return AS_DJURU(fiberVal);
}

static Closure *mirrorGetSlotClosure(Djuru *djuru, int slot) {
    Value closureVal = *MSCSlotAtUnsafe(djuru, slot);
    if (!IS_CLOSURE(closureVal)) return NULL;

    return AS_CLOSURE(closureVal);
}

static Module *mirrorGetSlotModule(Djuru *djuru, int slot) {
    Value moduleVal = *MSCSlotAtUnsafe(djuru, slot);
    if (!IS_MODULE(moduleVal)) return NULL;

    return AS_MODULE(moduleVal);
}

void mirrorClassMirrorHasMethod(Djuru *djuru) {
    Class *classObj = mirrorGetSlotClass(djuru, 1);
    const char *signature = MSCGetSlotString(djuru, 2);

    bool hasMethod = false;
    if (classObj != NULL &&
        signature != NULL) {
        int symbol = MSCSymbolTableFind(&djuru->vm->methodNames,
                                        signature, strlen(signature));
        hasMethod = MSCClassGetMethod(djuru->vm, classObj, symbol) != NULL;
    }
    MSCSetSlotBool(djuru, 0, hasMethod);
}


void mirrorClassMirrorMethodNames(Djuru *djuru) {
    Class *classObj = mirrorGetSlotClass(djuru, 1);

    if (!classObj) {
        MSCSetSlotNull(djuru, 0);
        return;
    }

    MSCSetSlotNewList(djuru, 0);
    for (size_t symbol = 0; symbol < classObj->methods.count; symbol++) {
        Method *method = MSCClassGetMethod(djuru->vm, classObj, symbol);
        if (method == NULL) continue;

        *MSCSlotAtUnsafe(djuru, 1) = OBJ_VAL(djuru->vm->methodNames.data[symbol]);
        MSCInsertInList(djuru, 0, -1, 1);
    }
}
void mirrorClassMirrorMethods(Djuru *djuru) {
    // Dumare.diaaBo(a).tiidenw.keleKelen {it => it.weele(a, [])}
    Class *classObj = mirrorGetSlotClass(djuru, 1);
    if (!classObj) {
        MSCSetSlotNull(djuru, 0);
        return;
    }

    MSCSetSlotNewList(djuru, 0);
    for (size_t symbol = 0; symbol < classObj->methods.count; symbol++) {
        Method *method = MSCClassGetMethod(djuru->vm, classObj, symbol);
        if (method == NULL) continue;

        *MSCSlotAtUnsafe(djuru, 1) = OBJ_VAL(method->as.closure);
        MSCInsertInList(djuru, 0, -1, 1);
    }
}

void mirrorFiberMirrorMethodAt(Djuru *djuru) {
    Djuru *fiber = mirrorGetSlotFiber(djuru, 1);
    size_t index = (size_t) MSCGetSlotDouble(djuru, 2);
    CallFrame *frame;

    if (fiber == NULL ||
        (frame = &fiber->frames[index])->closure == NULL) {
        MSCSetSlotNull(djuru, 0);
        return;
    }
    *MSCSlotAtUnsafe(djuru, 0) = OBJ_VAL(frame->closure);
}

void mirrorFiberMirrorLineAt(Djuru *djuru) {
    Djuru *fiber = mirrorGetSlotFiber(djuru, 1);
    size_t index = (size_t) MSCGetSlotDouble(djuru, 2);
    CallFrame *frame;
    Function *fn;

    if (fiber == NULL ||
        (frame = &fiber->frames[index]) == NULL ||
        (fn = frame->closure->fn) == NULL ||
        fn->debug->sourceLines.data == NULL) {
        MSCSetSlotNull(djuru, 0);
        return;
    }

    size_t line = (size_t) fn->debug->sourceLines.data[frame->ip - fn->code.data - 1];

    MSCSetSlotDouble(djuru, 0, line);
}

void mirrorFiberMirrorStackFramesCount(Djuru *djuru) {
    Djuru *fiber = mirrorGetSlotFiber(djuru, 1);

    if (fiber == NULL) {
        MSCSetSlotNull(djuru, 0);
        return;
    }

    MSCSetSlotDouble(djuru, 0, fiber->numOfFrames);
}


void mirrorMethodMirrorBoundToClass_(Djuru *djuru) {
    Closure *closureObj = mirrorGetSlotClosure(djuru, 1);

    if (!closureObj) {
        MSCSetSlotNull(djuru, 0);
        return;
    }

    *MSCSlotAtUnsafe(djuru, 0) = OBJ_VAL(closureObj->fn->boundToClass);
}

void mirrorMethodMirrorModule_(Djuru *djuru) {
    Closure *closureObj = mirrorGetSlotClosure(djuru, 1);

    if (!closureObj) {
        MSCSetSlotNull(djuru, 0);
        return;
    }

    *MSCSlotAtUnsafe(djuru, 0) = OBJ_VAL(closureObj->fn->module);
}

void mirrorMethodMirrorSignature_(Djuru *djuru) {
    Closure *closureObj = mirrorGetSlotClosure(djuru, 1);

    if (!closureObj) {
        MSCSetSlotNull(djuru, 0);
        return;
    }

    MSCSetSlotString(djuru, 0, closureObj->fn->debug->name);
}

void mirrorModuleMirrorFromName_(Djuru *djuru) {
    const char *moduleName = MSCGetSlotString(djuru, 1);

    if (!moduleName) {
        MSCSetSlotNull(djuru, 0);
        return;
    }

    // Special case for "core"
    if (strcmp(moduleName, "core") == 0) {
        MSCSetSlotNull(djuru, 1);
    }

    Module *module = MSCGetModule(djuru->vm, *MSCSlotAtUnsafe(djuru, 1));
    if (module != NULL) {
        *MSCSlotAtUnsafe(djuru, 0) = OBJ_VAL(module);
    } else {
        MSCSetSlotNull(djuru, 0);
    }
}

void mirrorModuleMirrorName_(Djuru *djuru) {
    Module *moduleObj = mirrorGetSlotModule(djuru, 1);
    if (!moduleObj) {
        MSCSetSlotNull(djuru, 0);
        return;
    }

    if (moduleObj->name) {
        *MSCSlotAtUnsafe(djuru, 0) = OBJ_VAL(moduleObj->name);
    } else {
        // Special case for "core"
        MSCSetSlotString(djuru, 0, "core");
    }
}


void mirrorObjectMirrorCanInvoke(Djuru *djuru) {
    Class *classObj = MSCGetClassInline(djuru->vm, djuru->stackStart[1]);
    djuru->stackStart[1] = OBJ_VAL(classObj);

    mirrorClassMirrorHasMethod(djuru);
}



