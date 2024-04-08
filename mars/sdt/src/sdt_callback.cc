#include "sdt_callback.h"
#include "sdt/netchecker_profile.h"
#include <stdio.h>
#include <string.h>
#include <string>

namespace mars
{
    namespace sdt
    {
        SdtCallBack *instance_ = NULL;

        SdtCallBack *SdtCallBack::Instance(sdt_callback_func func)
        {
            if (instance_ == NULL)
            {
                instance_ = new SdtCallBack(func);
            }

            return instance_;
        }

        void SdtCallBack::Release()
        {
            delete instance_;
            instance_ = NULL;
        }

        void SdtCallBack::ReportNetCheckResult(const std::vector<CheckResultProfile> &_check_results)
        {
            if (this->callback_function != nullptr)
            {
                struct SdtDiagnoseResult *result_list = new SdtDiagnoseResult[_check_results.size()];

                int result_count = 0;
                std::vector<CheckResultProfile>::const_iterator iter = _check_results.begin();
                for (; iter != _check_results.end(); ++iter)
                {
                    result_list[result_count].netcheck_type = iter->netcheck_type;

                    result_list[result_count].error_code = iter->error_code;
                    result_list[result_count].network_type = iter->network_type;

                    result_list[result_count].port = iter->port;
                    result_list[result_count].conntime = iter->conntime;
                    result_list[result_count].rtt = iter->rtt;
                    result_list[result_count].status_code = iter->status_code;
                    result_list[result_count].checkcount = iter->checkcount;

                    sdt::malloc_and_copy_string_to_char(&(result_list[result_count].ip), iter->ip);
                    sdt::malloc_and_copy_string_to_char(&(result_list[result_count].rtt_str), iter->rtt_str);
                    sdt::malloc_and_copy_string_to_char(&(result_list[result_count].url), iter->url);
                    sdt::malloc_and_copy_string_to_char(&(result_list[result_count].domain_name), iter->domain_name);
                    sdt::malloc_and_copy_string_to_char(&(result_list[result_count].local_dns), iter->local_dns);
                    sdt::malloc_and_copy_string_to_char(&(result_list[result_count].ip1), iter->ip1);
                    sdt::malloc_and_copy_string_to_char(&(result_list[result_count].ip2), iter->ip2);
                    sdt::malloc_and_copy_string_to_char(&(result_list[result_count].loss_rate), iter->loss_rate);

                    result_count++;
                }
                for (int i = 0; i < result_count; i++)
                {
                    printf("------check_type:%s", result_list[i].ip);
                }

                this->callback_function(result_list, _check_results.size());
                // 释放动态分配的内存
                delete[] result_list;
            }
        }

        void sdt::malloc_and_copy_string_to_char(char **des_char, std::string res_string)
        {
            if (!res_string.empty())
            {
                *des_char = (char *)malloc(res_string.size() + 1);
                memset(*des_char, 0, res_string.size() + 1);
                strcpy(*des_char, res_string.c_str());
            }
            else
            {
                *des_char = NULL;
            }
        }
    }
}
