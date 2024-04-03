#include "sdt_callback.h"
#include "sdt/netchecker_profile.h"
#include <stdio.h>
#include <string.h>

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
                SdtDiagnoseResult result_list[_check_results.size()];
                printf("_check_results.size(): %d", _check_results.size());
                int result_count = 0;
                std::vector<CheckResultProfile>::const_iterator iter = _check_results.begin();
                for (; iter != _check_results.end(); ++iter)
                {

                    SdtDiagnoseResult result = result_list[result_count];
                    result.netcheck_type = iter->netcheck_type;
                    printf("=====iter:%d,result:%d", iter->netcheck_type, result.netcheck_type);
                    result.error_code = iter->error_code;
                    result.network_type = iter->network_type;
                    // strcpy(result.ip, iter->ip.c_str());

                    result.port = iter->port;
                    result.conntime = iter->conntime;
                    result.rtt = iter->rtt;
                    // strcpy(result.rtt_str, iter->rtt_str.c_str());

                    // strcpy(result.url, iter->url.c_str());
                    result.status_code = iter->status_code;
                    result.checkcount = iter->checkcount;
                    // strcpy(result.loss_rate, iter->loss_rate.c_str());
                    // strcpy(result.domain_name, iter->domain_name.c_str());
                    // strcpy(result.local_dns, iter->local_dns.c_str());
                    // strcpy(result.ip1, iter->ip1.c_str());
                    // strcpy(result.ip2, iter->ip2.c_str());

                    result_count++;
                }
                printf("\n");
                for (int i = 0; i < result_count; i++)
                {
                    SdtDiagnoseResult result = result_list[i];
                    printf("******** %d", result.netcheck_type);
                }

                printf("======%p==%d", result_list, result_list[0].netcheck_type);
                this->callback_function(result_list, _check_results.size());
            }
        }
    }
}
