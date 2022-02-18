//
// Created by Flo on 17/02/2022.
//

#ifndef NETPONG_PROJECT_SYNCABLEOBJECTOPTIONS_HPP
#define NETPONG_PROJECT_SYNCABLEOBJECTOPTIONS_HPP

#include "SyncableObjectType.hpp"
#include <string>

enum SyncableObjectControl { Local, Remote };

struct SyncableObjectOptions
{
    SyncableObjectOptions(int id, SyncableObjectType entityType, SyncableObjectControl control);
    int id;
    SyncableObjectType entityType;
    SyncableObjectControl control;
};

#endif //NETPONG_PROJECT_SYNCABLEOBJECTOPTIONS_HPP
