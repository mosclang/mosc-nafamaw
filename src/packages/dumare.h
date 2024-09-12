//
// Created by Mahamadou DOUMBIA [OML DSI] on 22/02/2022.
//

#ifndef MOSC_DUMARE_H
#define MOSC_DUMARE_H

#include "../api/msc.h"
#include "../common/common.h"



void mirrorClassMirrorHasMethod(Djuru *djuru);
void mirrorClassMirrorMethodNames(Djuru *djuru);
void mirrorMethodMirrorBoundToClass_(Djuru *djuru);
void mirrorMethodMirrorModule_(Djuru *djuru);
void mirrorMethodMirrorSignature_(Djuru *djuru);
void mirrorFiberMirrorLineAt(Djuru *djuru);
void mirrorFiberMirrorMethodAt(Djuru *djuru);
void mirrorFiberMirrorStackFramesCount(Djuru *djuru);
void mirrorModuleMirrorFromName_(Djuru *djuru);
void mirrorModuleMirrorName_(Djuru *djuru);
void mirrorObjectMirrorCanInvoke(Djuru *djuru);
void mirrorClassMirrorMethods(Djuru *djuru);


#endif //MOSC_DUMARE_H
