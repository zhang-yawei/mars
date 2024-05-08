
#ifndef SDT_CALLBACK_h
#define SDT_CALLBACK_h

#include "sdt/sdt.h"
#include "sdt/src/sdt_extern_callback.h"

namespace mars
{
    namespace sdt
    {
        void malloc_and_copy_string_to_char(char **des_char, std::string res_string);

        bool is_empty_string(char *str);

        void sdt_diagnose_result_free(struct SdtDiagnoseResult *result);

        class SdtCallBack : public Callback
        {

        public:
            static SdtCallBack *Instance(sdt_callback_func func);
            static void Release();
            void sdtRelease();

            sdt_callback_func callback_function = nullptr;

            struct SdtDiagnoseResult *pre_result_list = nullptr;

            int pre_result_list_length = 0;

            void ReportNetCheckResult(const std::vector<CheckResultProfile> &_check_results);

            ~SdtCallBack(){};
            SdtCallBack(sdt_callback_func func)
            {
                callback_function = func;
            }
        };

    }
}

#endif