//
// Created by Mahamadou DOUMBIA [OML DSI] on 18/01/2024.
//

#include <msc.h>
#include <string.h>
#include <nafamaw.h>
#include "buffer.h"

static void print(MVM *_, const char *text) {
    printf("%s", text);
}

static bool errorPrint(MVM *vm, MSCError type, const char *module_name, int line, const char *message) {
    printf("Error at %s > %d: %s\n", module_name, line, message);
    return true;
}

static const char *readSource(const char *name) {
    FILE *file = fopen(name, "r");
    const char *buffer = NULL;
    if (file) {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek (file, 0, SEEK_SET);
        buffer = malloc((size_t) size);
        if (buffer) {
            fread((void *) buffer, 1, (size_t) size, file);
        }
        fclose(file);
    }
    return buffer;
}

// Looks for a built-in module with [name].
//
// Returns the BuildInModule for it or NULL if not found.
static ModuleRegistry *findModule(const char *name) {
    ModuleRegistry *modules = nafamawModules;
    for (int i = 0; modules[i].name != NULL; i++) {
        if (strcmp(name, modules[i].name) == 0){
            return &modules[i];
        }
    }
    return NULL;
}

// Looks for a class with [name] in [module].
static ClassRegistry *findClass(ModuleRegistry *module, const char *name) {
    for (int i = 0; module->classes[i].name != NULL; i++) {
        if (strcmp(name, module->classes[i].name) == 0) return &module->classes[i];
    }

    return NULL;
}

// Looks for a method with [signature] in [clas].
static MSCExternMethodFn findMethod(ClassRegistry *clas,
                                    bool isStatic, const char *signature) {
    for (int i = 0; clas->methods[i].signature != NULL; i++) {
        MethodRegistry *method = &clas->methods[i];
        if (isStatic == method->isStatic &&
            strcmp(signature, method->signature) == 0) {
            return method->method;
        }
    }

    return NULL;
}

void loadModuleComplete(MVM *vm, const char *name, struct MSCLoadModuleResult result) {
    if (result.source == NULL) return;

    free((void *) result.source);
}

MSCLoadModuleResult loadBuiltInModule(MVM *vm, const char *name) {
    MSCLoadModuleResult result = {0};
    ModuleRegistry *module = findModule(name);
    if (module == NULL) return result;

    size_t length = strlen(*module->source);
    char *copy = (char *) malloc(length + 1);
    memcpy(copy, *module->source, length + 1);

    result.onComplete = loadModuleComplete;
    result.source = copy;
    return result;
}

MSCExternMethodFn bindBuiltInExternMethod(
        MVM *vm, const char *moduleName, const char *className, bool isStatic,
        const char *signature) {
    // TODO: Assert instead of return NULL?
    ModuleRegistry *module = findModule(moduleName);
    if (module == NULL) return NULL;

    ClassRegistry *clas = findClass(module, className);
    if (clas == NULL) return NULL;

    return findMethod(clas, isStatic, signature);
}

MSCExternClassMethods bindBuiltInExternClass(
        MVM *vm, const char *moduleName, const char *className) {
    MSCExternClassMethods methods = {NULL, NULL};

    ModuleRegistry *module = findModule(moduleName);
    if (module == NULL) return methods;

    ClassRegistry *clas = findClass(module, className);
    if (clas == NULL) return methods;

    methods.allocate = findMethod(clas, true, "<allocate>");
    methods.finalize = (MSCFinalizerFn) findMethod(clas, true, "<finalize>");

    return methods;
}
const char* resolveModule(MVM* vm,const char* importer, const char* name) {

    if(strcmp(name, "dumare") == 0 || strcmp(name, "nafamaw") == 0 || strcmp(name, "ensure") == 0 || strcmp(name, "json") == 0) {
        return name;
    }

    return NULL;
}

int main(int argc,  char* argv[]) {
    // StringBuffer* buffer = newStringBuffer(32);
    // stringBufferPush(buffer, "Molobala");
    // stringBufferUnshift(buffer, "Hello");
    // stringBufferInsert(buffer, 5, " ");
    // printf("%s: %d\n", buffer->data, buffer->size);

    if(argc < 2) {
         printf("Invalid usage: nafamawt <file>\n");
         return -1;
    }
    // const char *text = readSource("../tests/mirror/method_invoke_mirror.msc");
    const char *text = readSource(argv[1]);
    if(text == NULL) {
        printf("Failed to read file");
        return 0;
    }

    MSCConfig config;
    MSCInitConfig(&config);
    config.errorHandler = errorPrint;
    config.writeFn = print;
    config.loadModuleFn = loadBuiltInModule;
    config.bindExternMethodFn = (MSCBindExternMethodFn) bindBuiltInExternMethod;
    config.resolveModuleFn = resolveModule;
    config.bindExternClassFn = (MSCBindExternClassFn) bindBuiltInExternClass;
    MVM *vm = MSCNewVM(&config);
    MSCInterpret(vm, "script", text);
    MSCFreeVM(vm);
    free((void *) text);
    return 0;
}