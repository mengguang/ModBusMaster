#include <stdio.h>
#include <modbus.h>
#include <stdint.h>
#include <errno.h>
#include "windows.h"

int
modbus_read_device(modbus_t *ctx, int device_address, int register_address, int number_of_register, uint16_t *result) {
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    int rc;
    int i;
    modbus_set_slave(ctx, device_address);
    rc = modbus_read_registers(ctx, register_address, number_of_register, result);
    if (rc == -1) {
        fprintf(stderr, "%s\n", modbus_strerror(errno));
        modbus_close(ctx);
        return -1;
    }
    for (i = 0; i < rc; i++) {
        printf("reg[%d]=%d (0x%04X)\n", i, result[i], result[i]);
    }
    modbus_close(ctx);
    return rc;
}

int main() {
    modbus_t *ctx = NULL;
    uint16_t tab_reg[64];

    ctx = modbus_new_rtu("\\\\.\\COM22", 4800, 'N', 8, 1);
    //ctx = modbus_new_rtu("/dev/ttyUSB0", 4800, 'N', 8, 1);

    if (ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    modbus_set_debug(ctx, TRUE);

//    uint32_t old_response_to_sec;
//    uint32_t old_response_to_usec;

//    /* Save original timeout */
//    modbus_get_response_timeout(ctx, &old_response_to_sec, &old_response_to_usec);
//    printf("old_response_to_sec: %u\nold_response_to_usec: %u\n",
//            old_response_to_sec, old_response_to_usec);
//
//    /* Define a new timeout of 200ms */
//    modbus_set_response_timeout(ctx, 0, 200000);

    //temperature, humidity, co2
    //modbus_read_device(ctx, 1, 0, 3, tab_reg);

    //0-200000Lux light sensor
    //modbus_read_device(ctx, 2, 2, 2, tab_reg);

    //Arduino Random Data
    for (int i = 0; i < 10000; ++i) {
        modbus_read_device(ctx, 10, 1, 1, tab_reg);
        Sleep(50);
    }

    modbus_free(ctx);

    return 0;
}