#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

typedef void(*logprintf_t)(const char* format, ...);
extern logprintf_t logprintf;

#endif MAIN_H_INCLUDED
