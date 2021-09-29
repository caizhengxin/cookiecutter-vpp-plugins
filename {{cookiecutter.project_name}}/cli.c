/**
 * @Author: jankincai
 * @Date:   2021-09-29 09:23:25
 * @Last Modified by:   jankincai
 * @Last Modified time: 2021-09-29 15:27:46
 */
{% set prefix = cookiecutter.project_alias[0] %}#include <vnet/vnet.h>
#include <vnet/plugin/plugin.h>
#include <vlibmemory/api.h>
#include <{{cookiecutter.project_name}}/{{cookiecutter.project_alias}}.h>


{{cookiecutter.project_alias}}_main_t {{cookiecutter.project_alias}}_main;



static int __{{cookiecutter.project_alias}}_enable_disable(void *szmp, u32 sw_if_index, int enable_disable)
{
    vnet_feature_enable_disable("device-input", "{{cookiecutter.project_name}}-input", sw_if_index, enable_disable, 0, 0);
    vnet_feature_enable_disable("interface-output", "{{cookiecutter.project_name}}-output", sw_if_index, enable_disable, 0, 0);

    return 0;
}


int {{cookiecutter.project_alias}}_enable_disable({{cookiecutter.project_alias}}_main_t *{{prefix}}mp, int enable_disable)
{
    vnet_sw_interface_t *sw;
    int rv = 0;

    {{cookiecutter.project_alias}}_create_periodic_process({{prefix}}mp);

    // vpp_plugins_enable_disable_all(&__{{cookiecutter.project_alias}}_enable_disable, (void*){{prefix}}mp, enable_disable);

    vnet_sw_interface_t *si, *sorted_sis = 0;
    vnet_main_t *vnm = vnet_get_main();
    vnet_interface_main_t *im = &vnm->interface_main;

    /* 获取所有接口 */
    if (vec_len(sorted_sis) == 0)
    {
        sorted_sis = vec_new(vnet_sw_interface_t, pool_elts(im->sw_interfaces));
        _vec_len(sorted_sis) = 0;
        
        pool_foreach(si, im->sw_interfaces)
        {
            int visible = vnet_swif_is_api_visible(si);
            if (visible)
            {
                vec_add1(sorted_sis, si[0]);
            }            
        }
    }

    vec_foreach(si, sorted_sis)
    {
        if (si->sw_if_index != 0)
        {
            __{{cookiecutter.project_alias}}_enable_disable({{prefix}}mp, si->sw_if_index, enable_disable);
        }        
    }

    vec_free(sorted_sis);

    /* Send an event to enable/disable the periodic scanner process */
    vlib_process_signal_event({{prefix}}mp->vlib_main, {{prefix}}mp->periodic_node_index, {{cookiecutter.project_alias.upper()}}_EVENT_PERIODIC_ENABLE_DISABLE, (uword)enable_disable);

    return rv;
}


static clib_error_t *{{cookiecutter.project_alias}}_enable_command_fn(vlib_main_t *vm, unformat_input_t *input, vlib_cli_command_t *cmd)
{
    {{cookiecutter.project_alias}}_enable_disable(&{{cookiecutter.project_alias}}_main, 1);

    return 0;
}


/* *INDENT-OFF* */
VLIB_CLI_COMMAND({{cookiecutter.project_alias}}_enable_command, static) =
{
    .path = "{{cookiecutter.project_name}} enable",
    .short_help = "{{cookiecutter.project_name}} enable",
    .function = {{cookiecutter.project_alias}}_enable_command_fn,
};
/* *INDENT-ON* */


static clib_error_t *{{cookiecutter.project_alias}}_disable_command_fn(vlib_main_t *vm, unformat_input_t *input, vlib_cli_command_t *cmd)
{
    {{cookiecutter.project_alias}}_enable_disable(&{{cookiecutter.project_alias}}_main, 0);

    return 0;
}


/* *INDENT-OFF* */
VLIB_CLI_COMMAND({{cookiecutter.project_alias}}_disable_command, static) =
{
    .path = "{{cookiecutter.project_name}} disable",
    .short_help = "{{cookiecutter.project_name}} disable",
    .function = {{cookiecutter.project_alias}}_disable_command_fn,
};
/* *INDENT-ON* */

