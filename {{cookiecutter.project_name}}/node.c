/**
 * @Author: jankincai
 * @Date:   2021-09-02 21:10:40
 * @Last Modified by:   jankincai
 * @Last Modified time: 2021-09-29 15:17:14
 */
#include <netinet/in.h>
#include <vlib/vlib.h>
#include <vnet/vnet.h>
#include <vnet/pg/pg.h>
#include <vppinfra/error.h>
#include <{{cookiecutter.project_name}}/{{cookiecutter.project_alias}}.h>

typedef struct 
{
    u32 next_index;
    u32 sw_if_index;
    u8 new_src_mac[6];
    u8 new_dst_mac[6];
}{{cookiecutter.project_alias}}_trace_t;

#ifndef CLIB_MARCH_VARIANT
static u8 *my_format_mac_address(u8 *s, va_list *args)
{
    u8 *a = va_arg(*args, u8 *);
    return format(s, "%02x:%02x:%02x:%02x:%02x:%02x", a[0], a[1], a[2], a[3], a[4], a[5]);
}

/* packet trace format function */
static u8 *format_{{cookiecutter.project_alias}}_trace(u8 *s, va_list *args)
{
    CLIB_UNUSED (vlib_main_t *vm) = va_arg(*args, vlib_main_t *);
    CLIB_UNUSED (vlib_node_t *node) = va_arg(*args, vlib_node_t *);
    {{cookiecutter.project_alias}}_trace_t *t = va_arg(*args, {{cookiecutter.project_alias}}_trace_t *);
  
    s = format(s, "{{cookiecutter.project_alias.upper()}}: sw_if_index %d, next index %d\n", t->sw_if_index, t->next_index);
    s = format(s, "  new src %U -> new dst %U", my_format_mac_address, t->new_src_mac, my_format_mac_address, t->new_dst_mac);

    return s;
}

vlib_node_registration_t {{cookiecutter.project_alias}}_node;

#endif /* CLIB_MARCH_VARIANT */

#define foreach_{{cookiecutter.project_alias}}_error \
_(SWAPPED, "Mac swap packets processed")

typedef enum {
#define _(sym,str) {{cookiecutter.project_alias.upper()}}_ERROR_##sym,
    foreach_{{cookiecutter.project_alias}}_error
#undef _
    {{cookiecutter.project_alias.upper()}}_N_ERROR,
}{{cookiecutter.project_alias}}_error_t;


#ifndef CLIB_MARCH_VARIANT
static char * {{cookiecutter.project_alias}}_error_strings[] = 
{
#define _(sym,string) string,
    foreach_{{cookiecutter.project_alias}}_error
#undef _
};
#endif /* CLIB_MARCH_VARIANT */

typedef enum 
{
    {{cookiecutter.project_alias.upper()}}_NEXT_ERROR_DROP,
    {{cookiecutter.project_alias.upper()}}_N_NEXT,
}{{cookiecutter.project_alias}}_next_t;


static int {{cookiecutter.project_alias}}_node_func(vlib_main_t *vm, vlib_node_runtime_t *node, vlib_frame_t *frame, uint8_t is_input)
{
    u32 n_left_from, *from, *to_next;
    {{cookiecutter.project_alias}}_next_t next_index;
    u32 pkts_swapped = 0;

    from = vlib_frame_vector_args(frame);
    n_left_from = frame->n_vectors;
    next_index = node->cached_next_index;

    while (n_left_from > 0)
    {
        u32 n_left_to_next;

        vlib_get_next_frame(vm, node, next_index, to_next, n_left_to_next);

        while (n_left_from > 0 && n_left_to_next > 0)
        {
            u32 bi0;
            vlib_buffer_t * b0;
            u32 next0;
            u32 sw_if_index0, sw_if_index_rx, sw_if_index_tx;
            u8 tmp0[6];
            ethernet_header_t *en0;

            /* speculatively enqueue b0 to the current next frame */
            bi0 = from[0];
            to_next[0] = bi0;
            from += 1;
            to_next += 1;
            n_left_from -= 1;
            n_left_to_next -= 1;

            b0 = vlib_get_buffer(vm, bi0);

            ASSERT(b0->current_data == 0);

            sw_if_index0 = vnet_buffer(b0)->sw_if_index[VLIB_RX];
            sw_if_index_rx = sw_if_index0;
            sw_if_index_tx = vnet_buffer(b0)->sw_if_index[VLIB_TX];

            vnet_feature_next(&next0, b0);

            // 发送数据包到输出队列, 一般挂载output节点
            // vnet_buffer(b0).sw_if_index[VLIB_TX] = sw_if_index0;
          
            // en0 = vlib_buffer_get_current (b0);
            en0 = (ethernet_header_t*)b0->data;

            uint16_t ethtype = ntohs(en0->type);


            if (PREDICT_FALSE((node->flags & VLIB_NODE_FLAG_TRACE) && (b0->flags & VLIB_BUFFER_IS_TRACED)))
            {
                {{cookiecutter.project_alias}}_trace_t *t = vlib_add_trace(vm, node, b0, sizeof(*t));
                t->sw_if_index = sw_if_index0;
                t->next_index = next0;
                clib_memcpy(t->new_src_mac, en0->src_address, sizeof(t->new_src_mac));
                clib_memcpy(t->new_dst_mac, en0->dst_address, sizeof(t->new_dst_mac));
            }

            pkts_swapped += 1;

            /* verify speculative enqueue, maybe switch current next frame */
            vlib_validate_buffer_enqueue_x1(vm, node, next_index, to_next, n_left_to_next, bi0, next0);
        }

        vlib_put_next_frame(vm, node, next_index, n_left_to_next);
    }

    vlib_node_increment_counter(vm, {{cookiecutter.project_alias}}_node.index, {{cookiecutter.project_alias.upper()}}_ERROR_SWAPPED, pkts_swapped);

    return frame->n_vectors;
}


VLIB_NODE_FN({{cookiecutter.project_alias}}_input) (vlib_main_t *vm, vlib_node_runtime_t *node, vlib_frame_t *frame)
{
    return {{cookiecutter.project_alias}}_node_func(vm, node, frame, 1 /* input */);
}


/* *INDENT-OFF* */
#ifndef CLIB_MARCH_VARIANT
VLIB_REGISTER_NODE({{cookiecutter.project_alias}}_input) = 
{
    .name = "{{cookiecutter.project_name}}-input",
    .vector_size = sizeof(u32),
    .format_trace = format_{{cookiecutter.project_alias}}_trace,
    .type = VLIB_NODE_TYPE_INTERNAL,
    .n_errors = ARRAY_LEN({{cookiecutter.project_alias}}_error_strings),
    .error_strings = {{cookiecutter.project_alias}}_error_strings,
    .n_next_nodes = {{cookiecutter.project_alias.upper()}}_N_NEXT,
    /* edit / add dispositions here */
    .next_nodes = {
        [{{cookiecutter.project_alias.upper()}}_NEXT_ERROR_DROP] = "error-drop",
    },
};
#endif /* CLIB_MARCH_VARIANT */
/* *INDENT-ON* */


VLIB_NODE_FN({{cookiecutter.project_alias}}_output) (vlib_main_t *vm, vlib_node_runtime_t *node, vlib_frame_t *frame)
{
    return {{cookiecutter.project_alias}}_node_func(vm, node, frame, 0 /* output */);
}

/* *INDENT-OFF* */
#ifndef CLIB_MARCH_VARIANT
VLIB_REGISTER_NODE({{cookiecutter.project_alias}}_output) = 
{
    .name = "{{cookiecutter.project_name}}-output",
    .vector_size = sizeof(u32),
    .format_trace = format_{{cookiecutter.project_alias}}_trace,
    .type = VLIB_NODE_TYPE_INTERNAL,
    .n_errors = ARRAY_LEN({{cookiecutter.project_alias}}_error_strings),
    .error_strings = {{cookiecutter.project_alias}}_error_strings,
    .n_next_nodes = {{cookiecutter.project_alias.upper()}}_N_NEXT,
    /* edit / add dispositions here */
    .next_nodes = {
        [{{cookiecutter.project_alias | upper()}}_NEXT_ERROR_DROP] = "error-drop",
    },
};
#endif /* CLIB_MARCH_VARIANT */
/* *INDENT-ON* */

/*
 * fd.io coding-style-patch-verification: ON
 *
 * Local Variables:
 * eval: (c-set-style "gnu")
 * End:
 */
