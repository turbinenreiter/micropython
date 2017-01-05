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

sd_bus_vtable *ump_vtable = NULL;
mp_obj_t *fun_table = NULL;
int tab_len = 0;
static sd_bus_vtable start = SD_BUS_VTABLE_START(0);
static sd_bus_vtable end = SD_BUS_VTABLE_END;

// find_fun ********************************************************************
int find_fun(const char *member) {
    int i;
    for(i=0; i<tab_len+1; i++) {
        if(strcmp(member, ump_vtable[i+1].x.method.member) == 0) {
            break;
        }
    }
    return i;
}

// method_call_0 ***************************************************************
static int method_call_0(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    int fun_no = find_fun(sd_bus_message_get_member(m));
    const char *out_type = ump_vtable[fun_no+1].x.method.result;
    mp_uint_t out_len = 1;
    mp_obj_t out_obj = NULL;

    /* Reply with the response */
    if (out_type[0] == 'a') {
        switch(out_type[1]) {
            case 'b': // TODO pick up here
                out_obj = mp_obj_new_list(out_len,
                                          mp_call_function_0(fun_table[fun_no]));
                return sd_bus_reply_method_return(m, out_type, out_len,
                                                  1);//mp_obj_get_int(out_obj));
            case 'y': case 'n': case 'i': case 'x': case 'q': case 'u': case 't':
                out_obj = mp_call_function_0(fun_table[fun_no]);
                return sd_bus_reply_method_return(m, out_type, out_len,
                                                  mp_obj_get_int(out_obj));
            case 'd':
                out_obj = mp_call_function_0(fun_table[fun_no]);
                return sd_bus_reply_method_return(m, out_type, out_len,
                                                  mp_obj_get_float(out_obj));
            case 's':
                out_obj = mp_call_function_0(fun_table[fun_no]);
                return sd_bus_reply_method_return(m, out_type, out_len,
                                                  qstr_str(
                                                  mp_obj_str_get_qstr(out_obj)));
        }
    } else {
        switch(out_type[0]) {
            case 'b':
                out_obj = mp_call_function_0(fun_table[fun_no]);
                return sd_bus_reply_method_return(m, out_type, out_len,
                                                  mp_obj_get_int(out_obj));
            case 'y': case 'n': case 'i': case 'x': case 'q': case 'u': case 't':
                out_obj = mp_call_function_0(fun_table[fun_no]);
                return sd_bus_reply_method_return(m, out_type, out_len,
                                                  mp_obj_get_int(out_obj));
            case 'd':
                out_obj = mp_call_function_0(fun_table[fun_no]);
                return sd_bus_reply_method_return(m, out_type, out_len,
                                                  mp_obj_get_float(out_obj));
            case 's':
                out_obj = mp_call_function_0(fun_table[fun_no]);
                return sd_bus_reply_method_return(m, out_type, out_len,
                                                  qstr_str(
                                                  mp_obj_str_get_qstr(out_obj)));
        }
    }
    return -1;
}

// method_call_1 ***************************************************************
static int method_call_1(sd_bus_message *m, void *userdata, sd_bus_error *ret_error) {
    int fun_no = find_fun(sd_bus_message_get_member(m));
    const char *inp_type = ump_vtable[fun_no+1].x.method.signature;
    const char *out_type = ump_vtable[fun_no+1].x.method.result;
    void *ptr_inp = NULL;
    mp_obj_t inp_obj = NULL;
    int out_len = 1;
    mp_obj_t out_obj = NULL;

    /* Read the parameters */
    switch(inp_type[0]) {
        case 'b':
            ptr_inp = m_malloc(sizeof(int));
            sd_bus_message_read(m, inp_type, ptr_inp);
            inp_obj = mp_obj_new_bool((mp_int_t)*(int*)ptr_inp);
            break;
        case 'y': case 'n': case 'i': case 'x': case 'q': case 'u': case 't':
            ptr_inp = m_malloc(sizeof(int64_t));
            sd_bus_message_read(m, inp_type, ptr_inp);
            inp_obj = mp_obj_new_int_from_ll(*(int64_t*)ptr_inp);
            break;
        case 'd':
            ptr_inp = m_malloc(sizeof(double));
            sd_bus_message_read(m, inp_type, ptr_inp);
            inp_obj = mp_obj_new_float((mp_float_t)*(double*)ptr_inp);
            break;
        case 's':
            ptr_inp = m_malloc(sizeof(char*));
            sd_bus_message_read(m, inp_type, ptr_inp);
            inp_obj = mp_obj_new_str(*(char**)ptr_inp,
                                     (mp_uint_t)sizeof(*(char**)ptr_inp), 1);
            break;
    }

    /* Reply with the response */
    if (out_type[0] == 'a') {
        switch(out_type[1]) {
                case 'b':
                    out_obj = mp_call_function_1(fun_table[fun_no], inp_obj);
                    return sd_bus_reply_method_return(m, out_type, out_len,
                                                      mp_obj_get_int(out_obj));
                case 'y': case 'n': case 'i': case 'x': case 'q': case 'u': case 't':
                    out_obj = mp_call_function_1(fun_table[fun_no], inp_obj);
                    return sd_bus_reply_method_return(m, out_type, out_len,
                                                      mp_obj_get_int(out_obj));
                case 'd':
                    out_obj = mp_call_function_1(fun_table[fun_no], inp_obj);
                    return sd_bus_reply_method_return(m, out_type, out_len,
                                                      mp_obj_get_float(out_obj));
                case 's':
                    out_obj = mp_call_function_1(fun_table[fun_no], inp_obj);
                    return sd_bus_reply_method_return(m, out_type, out_len,
                                                      qstr_str(
                                                      mp_obj_str_get_qstr(out_obj)));
            }
    } else {
        switch(out_type[0]) {
            case 'b':
                out_obj = mp_call_function_1(fun_table[fun_no], inp_obj);
                return sd_bus_reply_method_return(m, out_type,
                                                  mp_obj_get_int(out_obj));
            case 'y': case 'n': case 'i': case 'x': case 'q': case 'u': case 't':
                out_obj = mp_call_function_1(fun_table[fun_no], inp_obj);
                return sd_bus_reply_method_return(m, out_type,
                                                  mp_obj_get_int(out_obj));
            case 'd':
                out_obj = mp_call_function_1(fun_table[fun_no], inp_obj);
                return sd_bus_reply_method_return(m, out_type,
                                                  mp_obj_get_float(out_obj));
            case 's':
                out_obj = mp_call_function_1(fun_table[fun_no], inp_obj);
                return sd_bus_reply_method_return(m, out_type,
                                                  qstr_str(
                                                  mp_obj_str_get_qstr(out_obj)));
        }
    }
    return -1;
}

// call_table ******************************************************************
int (*call_table[2]) (sd_bus_message *m, void *userdata, sd_bus_error *ret_error) = {
    method_call_0, method_call_1};

// dbus.register ***************************************************************
STATIC mp_obj_t mod_dbus_register(mp_obj_t function, mp_obj_t inp, mp_obj_t outp) {
    qstr qstr_fun_name = mp_obj_fun_get_name(function);
    qstr qstr_inp = mp_obj_str_get_qstr(inp);
    qstr qstr_outp = mp_obj_str_get_qstr(outp);

    fun_table = m_realloc(fun_table,
                          sizeof(mp_obj_t)*tab_len,
                          sizeof(mp_obj_t)*tab_len+1);
    fun_table[tab_len] = function;

    ump_vtable = m_realloc(ump_vtable,
                            sizeof(struct sd_bus_vtable)*(tab_len+2),
                            sizeof(struct sd_bus_vtable)*(tab_len+3));
    tab_len++;

    int inp_type;
    const char* str_inp = qstr_str(qstr_inp);
    if (strcmp(str_inp, "NULL") == 0) {
        str_inp = NULL;
        inp_type = 0;
    } else {
        inp_type = 1;
    }

    sd_bus_vtable new_elem = SD_BUS_METHOD(
                   qstr_str(qstr_fun_name),
                   str_inp,
                   qstr_str(qstr_outp),
                   call_table[inp_type],
                   SD_BUS_VTABLE_UNPRIVILEGED);

    ump_vtable[0] = start;
    ump_vtable[tab_len] = new_elem;
    ump_vtable[tab_len+1] = end;

    return MP_OBJ_NEW_QSTR(qstr_fun_name);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_dbus_register_obj, mod_dbus_register);

// dbus.run ********************************************************************
STATIC mp_obj_t mod_dbus_run(mp_obj_t interface_name, mp_obj_t object_path) {
    const char *name = mp_obj_str_get_str(interface_name);
    const char *path = mp_obj_str_get_str(object_path);

    sd_bus_slot *slot = NULL;
    sd_bus *bus = NULL;
    int r;

    /* Connect to the user bus */
    r = sd_bus_open_user(&bus);
    if (r < 0) {
            fprintf(stderr, "Failed to connect to system bus: %s\n", strerror(-r));
            goto finish;
    }

    /* Install the object */
    r = sd_bus_add_object_vtable(bus,
                                 &slot,
                                 path,
                                 name,
                                 ump_vtable,
                                 NULL);
    if (r < 0) {
            fprintf(stderr, "Failed to issue method call: %s\n", strerror(-r));
            goto finish;
    }

    /* Request a name */
    r = sd_bus_request_name(bus, name, 0);
    if (r < 0) {
            fprintf(stderr, "Failed to acquire service name: %s\n", strerror(-r));
            goto finish;
    }

    for (;;) {
            /* Process requests */
            r = sd_bus_process(bus, NULL);
            if (r < 0) {
                    fprintf(stderr, "Failed to process bus: %s\n", strerror(-r));
                    goto finish;
            }

            /* Wait for the next request to process */
            r = sd_bus_wait(bus, (uint64_t) -1);
            if (r < 0) {
                    fprintf(stderr, "Failed to wait on bus: %s\n", strerror(-r));
                    goto finish;
            }
    }

finish:
    sd_bus_slot_unref(slot);
    sd_bus_unref(bus);
    return MP_OBJ_NEW_SMALL_INT(r);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_dbus_run_obj, mod_dbus_run);

// dbus bindings ***************************************************************
STATIC const mp_rom_map_elem_t mp_module_dbus_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_dbus) },
    { MP_ROM_QSTR(MP_QSTR_register), (mp_obj_t)&mod_dbus_register_obj },
    { MP_ROM_QSTR(MP_QSTR_run), (mp_obj_t)&mod_dbus_run_obj },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_dbus_globals, mp_module_dbus_globals_table);

const mp_obj_module_t mp_module_dbus = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_dbus_globals,
};
