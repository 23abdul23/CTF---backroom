#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <time.h>

/* Log levels */
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 3

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

#define _LOG(level, fmt, ...) \
    do { \
        if (level >= LOG_LEVEL) { \
            time_t now = time(NULL); \
            struct tm *tm_info = localtime(&now); \
            char ts[16]; \
            strftime(ts, sizeof(ts), "%H:%M:%S", tm_info); \
            const char *level_str = ""; \
            if (level == LOG_LEVEL_DEBUG) level_str = "DEBUG"; \
            else if (level == LOG_LEVEL_INFO) level_str = "INFO "; \
            else if (level == LOG_LEVEL_WARN) level_str = "WARN "; \
            else if (level == LOG_LEVEL_ERROR) level_str = "ERROR"; \
            fprintf(stderr, "[%s] %s: " fmt "\n", ts, level_str, ##__VA_ARGS__); \
            fflush(stderr); \
        } \
    } while(0)

#define log_debug(fmt, ...) _LOG(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) _LOG(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) _LOG(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) _LOG(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)

#endif
