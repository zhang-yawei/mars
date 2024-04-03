
#ifndef SDT_CALLBACK_h
#define SDT_CALLBACK_h

#include "sdt/sdt.h"
#include "sdt/src/sdt_extern_callback.h"

namespace mars
{
    namespace sdt
    {

        class SdtCallBack : public Callback
        {

        public:
            static SdtCallBack *Instance(sdt_callback_func func);
            static void Release();
            sdt_callback_func callback_function = nullptr;

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