#pragma once

#define TEMP_VAR(var)           X_TEMP_VAR(var, __LINE__)
#define X_TEMP_VAR(var, line)   Y_TEMP_VAR(var, line)
#define Y_TEMP_VAR(var, line)   var ## line
