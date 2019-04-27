#include <stdio.h>
#include <modbus.h>

int main() {
    modbus_t *ctx;
    uint16_t tab_reg[64];
    int rc;
    int i;

    ctx = modbus_new_rtu("\\\\.\\COM22", 4800, 'N', 8, 1);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    modbus_set_debug(ctx, TRUE);
    modbus_set_slave(ctx, 1);

//    if (modbus_rtu_set_serial_mode(ctx,MODBUS_RTU_RS485) == -1) {
//        fprintf(stderr, "modbus_rtu_set_serial_mode failed: %s\n", modbus_strerror(errno));
//        modbus_free(ctx);
//        return -1;
//    }

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    uint32_t old_response_to_sec;
    uint32_t old_response_to_usec;

    /* Save original timeout */
    modbus_get_response_timeout(ctx, &old_response_to_sec, &old_response_to_usec);
    printf("old_response_to_sec: %u\nold_response_to_usec: %u\n", old_response_to_sec, old_response_to_usec);

    /* Define a new timeout of 200ms */
    modbus_set_response_timeout(ctx, 0, 200000);

    rc = modbus_read_registers(ctx, 0, 3, tab_reg);
    if (rc == -1) {
        fprintf(stderr, "%s\n", modbus_strerror(errno));
        return -1;
    }

    for (i = 0; i < rc; i++) {
        printf("reg[%d]=%d (0x%04X)\n", i, tab_reg[i], tab_reg[i]);
    }

    modbus_close(ctx);
    modbus_free(ctx);
    return 0;
}