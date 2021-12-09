/*
 * @Date: 2021-12-09 14:41:11
 * @LastEditors: kafier
 * @LastEditTime: 2021-12-09 14:46:39
 */
#include "../AsyncLogging.h"
#include "../Logging.h"

#include <stdio.h>

off_t kRollSize = 500 * 1000 * 1000;

AsyncLogging* g_asyncLog = NULL;

