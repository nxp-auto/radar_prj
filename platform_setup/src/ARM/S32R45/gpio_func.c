/*
 * Copyright (C) 2016-2020 NXP Semiconductors
 * Copyright 2018-2020 NXP
 *
 * SPDX-License-Identifier:		BSD-3-Clause
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "gpio_func.h"

#define SYS_GPIO_FOLDER     "/sys/class/gpio/"
#define GPIO_EXPORT_SPATH   "export"
#define GPIO_UNEXPORT_SPATH "unexport"

//#define GPIO                "gpio"
#define GPIO_VALUE_STR      "/value"
#define GPIO_DIRECTION_STR  "/direction"

#define GPIO_DIR_IN         "in"
#define GPIO_DIR_OUT        "out"

#define GPIO_PIN_CHARS      10u
#define GPIO_BASE_OFFSET    378u

#define READ                "r"
#define WRITE               "w"
#define ERROR               -1

static void get_pin_name(unsigned n, char *pBuf)
{
    if(n > 101)
    {
        n += 16;
    }
    sprintf(pBuf, "P%c_%02d", 'A' + (n/16), n%16);
}

int export_gpio(unsigned pin_no)
{
    FILE *export_file = NULL;

    /* Open for exporting the new pin value */
    export_file = fopen(SYS_GPIO_FOLDER GPIO_EXPORT_SPATH, WRITE);
    if (!export_file)
    {
        printf("Failed to open `" SYS_GPIO_FOLDER GPIO_EXPORT_SPATH "`\n");
        return ERROR;
    }

    /* Export pin */
    fprintf(export_file, "%d", GPIO_BASE_OFFSET + pin_no);

    /* Closing the file descriptor */
    fclose(export_file);
    return 0;
}

int unexport_gpio(unsigned pin_no)
{
    FILE *unexport_file = NULL;

    /* Open for unexporting the pin with a given value  */
    unexport_file = fopen(SYS_GPIO_FOLDER GPIO_UNEXPORT_SPATH, WRITE);
    if (!unexport_file)
    {
        printf("Failed to open `" SYS_GPIO_FOLDER GPIO_UNEXPORT_SPATH "`\n");
        return ERROR;
    }

    /* Unexport pin */
    fprintf(unexport_file, "%d", GPIO_BASE_OFFSET + pin_no);

    /* Closing the file descriptor */
    fclose(unexport_file);
    return 0;
}

int set_direction(unsigned pin_no, enum gpio_dir dir)
{
    FILE *   set_dir = NULL;
    size_t   dir_path_len = strlen(SYS_GPIO_FOLDER) + GPIO_PIN_CHARS + strlen(GPIO_DIRECTION_STR) + 1;
    char     buf[GPIO_PIN_CHARS];

    char *dir_path = (char*)calloc(dir_path_len, sizeof(*dir_path));

    /* Build a path to the GPIO pin wanted */
    get_pin_name(pin_no, buf);
    snprintf(dir_path, dir_path_len, SYS_GPIO_FOLDER "%s" GPIO_DIRECTION_STR, buf);

    /* Open for write into the file descriptor */
    set_dir = fopen(dir_path, WRITE);
    if (!set_dir)
    {
        printf("Failed to open `%s`\n", dir_path);
        return ERROR;
    }
    free(dir_path);

    /* Writing into the direction file of the gpio */
    fprintf(set_dir, "%s", dir == IN ? GPIO_DIR_IN : GPIO_DIR_OUT);

    /* Closing the file descriptor */
    fclose(set_dir);
    return 0;
}

static int gpio_value(unsigned pin_no, int val)
{
    FILE *      gpio_file = NULL;
    const char *file_mod;
    char        buf[GPIO_PIN_CHARS];

    size_t pin_length = strlen(SYS_GPIO_FOLDER) + GPIO_PIN_CHARS + strlen(GPIO_VALUE_STR) + 1;

    char *pin_path = (char*)calloc(pin_length, sizeof(*pin_path));

    get_pin_name(pin_no, buf);
    snprintf(pin_path, pin_length, SYS_GPIO_FOLDER "%s" GPIO_VALUE_STR, buf);

    if (val == -EINVAL)
        file_mod = READ;
    else
        file_mod = WRITE;

    gpio_file = fopen(pin_path, file_mod);
    if (!gpio_file)
    {
        printf("Failed to open `%s`\n", pin_path);
        return ERROR;
    }
    free(pin_path);

    /* Writing into the value file of the gpio */
    if (val == -EINVAL)
        fscanf(gpio_file, "%d", &val);
    else
        fprintf(gpio_file, "%d", val);

    /* Closing the file descriptor */
    fclose(gpio_file);
    return val;
}

int set_value(unsigned pin_no, unsigned val)
{
    return gpio_value(pin_no, val & 0x1) == ERROR ? ERROR : 0;
}

int get_value(unsigned pin_no)
{
    return gpio_value(pin_no, -EINVAL);
}
