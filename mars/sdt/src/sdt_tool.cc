#include "sdt_tool.h"
#include "mars/boot/context.h"
#include "sdt/sdt_manager.h"
#include "sdt/sdt_logic.h"
#include "sdt/src/sdt_callback.h"
#include "sdt/constants.h"
#include <vector>
#include <string.h>
#include "sdt/src/checkimpl/dnsquery.h"
#include <stdio.h>
#include "mars/comm/socket/socket_address.h"
#include <iostream>
#include <mars/comm/network/netinfo_util.h>
#include <mars/comm/comm_data.h>

using namespace std;
using namespace mars::sdt;
using namespace mars::boot;

#ifdef __cplusplus
extern "C"
{
#endif

    void add_manager(sdt_callback_func callback)
    {
        SdtManager *sdt_manager = Context::CreateContext("default")->GetManager<SdtManager>();
        if (sdt_manager == NULL)
        {
            sdt_manager = new SdtManager(Context::CreateContext("default"));
            SdtCallBack *const sdt_callback = SdtCallBack::Instance(callback);
            sdt_manager->SetCallBack(sdt_callback);
            Context::CreateContext("default")->AddManager(sdt_manager);
        }
    }

    void remove_manager()
    {
        SdtManager *sdt_manager = Context::CreateContext("default")->GetManager<SdtManager>();
        if (sdt_manager != NULL)
        {
            Context::CreateContext("default")->RemoveManager(sdt_manager);
            delete sdt_manager;
            SdtCallBack::Release();
        }
    }

    int net_diagnose_start(char *hosts[], int num, sdt_callback_func callback)
    {
        add_manager(callback);

        CheckIPPorts shortlink_check_items;
        std::vector<std::string> shortlink_hostlist;

        for (size_t i = 0; i < num; i++)
        {
            shortlink_hostlist.emplace_back(hosts[i]);
        }

        uint16_t shortlink_port = 443;

        for (std::vector<std::string>::iterator iter = shortlink_hostlist.begin(); iter != shortlink_hostlist.end();
             ++iter)
        {
            std::vector<std::string> shortlink_iplist;

            socket_ipinfo_t ip_info;
            std::vector<CheckIPPort> check_ipport_list;
            int dns_ret = socket_gethostbyname((*iter).c_str(), &ip_info, 10 * 1000, NULL);
            if (dns_ret == 0)
            {
                for (int i = 0; i < ip_info.size; i++)
                {
                    CheckIPPort ipport_item(socket_address(ip_info.ip[i]).ip(), shortlink_port);
                    check_ipport_list.push_back(ipport_item);
                }
            }

            if (!check_ipport_list.empty())
                shortlink_check_items.insert(std::pair<std::string, std::vector<CheckIPPort>>(*iter, check_ipport_list));
        }

        SdtManager *sdt_manager = Context::CreateContext("default")->GetManager<SdtManager>();

        if (sdt_manager)
        {
            // SdtCallBack *callback = dynamic_cast<SdtCallBack *>(sdt_manager->callback_);
            // callback->callback_function(NULL, 10);
            sdt_manager->StartActiveCheck(shortlink_check_items, shortlink_check_items, NET_CHECK_BASIC | NET_CHECK_SHORT, 20);
        }
    }

    void net_diagnose_cancel()
    {
        CancelActiveCheck();
        remove_manager();
    }
    void net_diagnose_complete()
    {
        remove_manager();
    }

    void net_info(bool need_wifi_ssid, char **ret_net_info)
    {
        std::string info = GetDetailNetInfo(need_wifi_ssid);
        size_t length = info.length() + 1;
        *ret_net_info = (char *)malloc(length);
        memcpy(*ret_net_info, info.c_str(), length);
    }

#ifdef __cplusplus
}
#endif