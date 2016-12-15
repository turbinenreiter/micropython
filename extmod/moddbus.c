/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Sebastian Plamauer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/mpconfig.h"

#include "py/obj.h"
#include "py/runtime.h"

#include <systemd/sd-bus.h>

STATIC mp_obj_t mod_dbus_test(mp_obj_t input) {
    const char *inp_str = mp_obj_str_get_str(input);
    return mp_obj_new_str(inp_str, strlen(inp_str), false);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_dbus_test_obj, mod_dbus_test);

static int method_call(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
        int64_t x;
        int r;

        /* Read the parameters */
        r = sd_bus_message_read(m, "x", &x);
        if (r < 0) {
                fprintf(stderr, "Failed to parse parameters: %s\n", strerror(-r));
                return r;
        }

        /* Reply with the response */
        return sd_bus_reply_method_return(m, "x", x);
}

static const sd_bus_vtable test_vtable[] = {
        SD_BUS_VTABLE_START(0),
        SD_BUS_METHOD("Call", "x", "x", method_call, SD_BUS_VTABLE_UNPRIVILEGED),
        SD_BUS_VTABLE_END
};

STATIC mp_obj_t mod_dbus_start(mp_obj_t interface_name, mp_obj_t object_path) {
    const char *name = mp_obj_str_get_str(interface_name);
    const char *path = mp_obj_str_get_str(object_path);

    sd_bus_slot *slot = NULL;
    sd_bus *bus = NULL;
    int r;

    r = sd_bus_open_user(&bus);
    r = sd_bus_add_object_vtable(bus,
                                 &slot,
                                 path,
                                 name,
                                 test_vtable,
                                 NULL);

    r = sd_bus_request_name(bus, name,0);

    for (;;) {
        r = sd_bus_process(bus, NULL);
        if (r>0) {
            continue;
        }
        r = sd_bus_wait(bus, (uint64_t) -1);
    }

    return MP_OBJ_NEW_SMALL_INT(r);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_dbus_start_obj, mod_dbus_start);

STATIC const mp_rom_map_elem_t mp_module_dbus_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_dbus) },
    { MP_ROM_QSTR(MP_QSTR_test), (mp_obj_t)&mod_dbus_test_obj },
    { MP_ROM_QSTR(MP_QSTR_start), (mp_obj_t)&mod_dbus_start_obj },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_dbus_globals, mp_module_dbus_globals_table);

const mp_obj_module_t mp_module_dbus = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_dbus_globals,
};
