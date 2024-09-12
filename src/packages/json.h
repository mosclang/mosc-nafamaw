//
// Created by Mahamadou DOUMBIA [OML DSI] on 25/02/2022.
//

#ifndef MOSC_NAFAMAW_JSON_H
#define MOSC_NAFAMAW_JSON_H

#include "msc.h"

void jsonStreamBegin(Djuru *djuru);
void jsonStreamEnd(Djuru *djuru);
void jsonStreamValue(Djuru *djuru);
void jsonStreamErrorMessage(Djuru *djuru);
void jsonStreamLineNumber(Djuru *djuru);
void jsonStreamPos(Djuru *djuru);
void jsonStreamNext(Djuru *djuru);

#endif //MOSC_NAFAMAW_JSON_H
