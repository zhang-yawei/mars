#ifndef SDT_EXTERN_CALLBACK_H
#define SDT_EXTERN_CALLBACK_H
#ifdef __cplusplus
extern "C"
{

#endif

    typedef struct SdtDiagnoseResult
    {
        // kPingCheck = 0,
        // kDnsCheck = 1,
        // kNewDnsCheck,
        // kTcpCheck,
        // kHttpCheck,
        // kTracerouteCheck,
        // kReqBufCheck,
        int netcheck_type; // ping dns tcp http

        int error_code;
        int network_type;

        char *ip;          // ping tcp http
        unsigned int port; // tcp http
        int conntime;
        int rtt;       // tcp http rtt, dns resolve time
        char *rtt_str; // ping

        char *url;
        int status_code; // http

        unsigned int checkcount; // ping
        char *loss_rate;         // ping

        char *domain_name; // dns host
        char *local_dns;   // dns
        char *ip1;         // dns
        char *ip2;
        /* data */
    } SdtDiagnoseResult;

    typedef void (*sdt_callback_func)(struct SdtDiagnoseResult results[], int num);

#ifdef __cplusplus
}
#endif

#endif