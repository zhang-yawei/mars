#ifndef SDT_SRC_SDTTOOL_H_
#define SDT_SRC_SDTTOOL_H_
// #include "sdt/src/sdt_extern_callback.h"
#include "sdt_extern_callback.h"
#ifdef __cplusplus
extern "C"
{

#endif

    int net_diagnose_start(char *hosts[], int num, sdt_callback_func callback);
    void net_diagnose_cancel();
    void net_diagnose_complete();

    void net_info(bool need_wifi_ssid, char **ret_net_info);

#ifdef __cplusplus
}
#endif

#endif // SDT_SRC_CHECKIMPL_DNSQUERY_H_
