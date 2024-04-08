
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sdt_extern_callback.h"

void sdt_diagnose_result_free(struct SdtDiagnoseResult *result)
{

    if (is_empty_string(result->ip))
    {
        free(result->ip);
    }

    if (is_empty_string(result->rtt_str))
    {
        free(result->rtt_str);
    }
    if (is_empty_string(result->url))
    {
        free(result->url);
    }

    if (is_empty_string(result->loss_rate))
    {
        free(result->loss_rate);
    }

    if (is_empty_string(result->domain_name))
    {
        free(result->domain_name);
    }

    if (is_empty_string(result->local_dns))
    {
        free(result->local_dns);
    }

    if (is_empty_string(result->ip1))
    {
        free(result->ip1);
    }

    if (is_empty_string(result->ip2))
    {
        free(result->ip2);
    }
    free(result);
}

// 判断字符串是否为空
bool is_empty_string(char *str)
{
    if (str != NULL)
    {
        if (strlen(str) == 0)
        {
            return true;
        }
        return false;
    }
    return true;
}