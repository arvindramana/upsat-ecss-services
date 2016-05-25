#include "event_reporting_service.h"

#undef __FILE_ID__
#define __FILE_ID__ 666

static uint8_t strNo[] = "No";

SAT_returnState event_app(tc_tm_pkt * pkt) {

    //event_log(pkt->data, EV_DATA_SIZE);

    return SATR_OK;
}


SAT_returnState event_crt_pkt_api(uint8_t *buf, uint8_t *f, uint16_t fi, uint32_t l, uint8_t *e, uint16_t *size, SAT_returnState mode) {

    uint8_t sub_type;
    uint8_t res_crc;

    if(mode == SATR_OK) { sub_type = TM_EV_NORMAL_REPORT; }
    else { sub_type = TM_EV_ERROR_REPORT; }

    buf[0] = HLDLC_START_FLAG;
    buf[1] = 0x08;
    buf[2] = SYSTEM_APP_ID;
    buf[3] = 0xC0;
    buf[4] = 5;

    buf[7] = 16;
    buf[8] = TC_EVENT_SERVICE;
    buf[9] = sub_type;
    buf[10] = DBG_APP_ID;

    if(strnlen(e, 200) > 200) { e = strNo; }

    if(mode == SATR_OK) { sprintf((char*)&buf[11], "Event %s,%d,%d,%s\n", f, fi, l, e); }
    else { sprintf((char*)&buf[11], "Error %s,%d,%d,%s\n", f, fi, l, e); }

    *size = strnlen(&buf[11], 200);
    event_log(&buf[11], *size);

    *size += 11 + 1;
    buf[*size] = 0;

    buf[5] = 0;
    buf[6] = *size - 6 -2 + 2;
    checkSum(&buf[1], *size - 1, &res_crc);
    buf[(*size)+1] = res_crc;
    buf[(*size)+2] = HLDLC_START_FLAG;

    *size += 3;

    return SATR_OK;
}

//SAT_returnState event_crt_api(uint8_t *buf, uint16_t *size) {



