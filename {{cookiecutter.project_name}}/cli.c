/**
 * @Author: jankincai
 * @Date:   2021-09-29 09:23:25
 * @Last Modified by:   jankincai
 * @Last Modified time: 2023-02-16 10:08:01
 */
{% set prefix = cookiecutter.project_alias[0] %}#include <vnet/vnet.h>
#include <vnet/plugin/plugin.h>
#include <vlibmemory/api.h>
#include <{{cookiecutter.project_name}}/{{cookiecutter.project_alias}}.h>
// #include <utils/vpp-plugins.h>


extern {{cookiecutter.project_alias}}_main_t {{cookiecutter.project_alias}}_main;



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

    // {{cookiecutter.project_alias}}_create_periodic_process({{prefix}}mp);

    {{prefix}}mp->enable_disable = enable_disable;

    // vpp_plugins_enable_disable_all(&__{{cookiecutter.project_alias}}_enable_disable, (void*){{prefix}}mp, enable_disable);

    vnet_sw_interface_t *si, *sorted_sis = 0;
    vnet_main_t *vnm = vnet_get_main();
    vnet_interface_main_t *im = &vnm->interface_main;

    /* 设置所有网口 */
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


static void auto_enable_disable({{cookiecutter.project_alias}}_main_t *{{ prefix }}mp)
{
    // 根据实际策略情况，编写以下代码，策略为空自动禁用插件，策略不为空则自动启用插件。
    // if ({{ prefix }}mp->count == 0)
    // {
    //     {{cookiecutter.project_alias}}_enable_disable({{ prefix }}mp, 0);
    // }
    // else if ({{ prefix }}mp->enable_disable == 0)
    // {
    //     {{cookiecutter.project_alias}}_enable_disable({{ prefix }}mp, 1);
    // }
}


static clib_error_t *{{cookiecutter.project_alias}}_enable_command_fn(vlib_main_t *vm, unformat_input_t *input, vlib_cli_command_t *cmd)
{
    {{cookiecutter.project_alias}}_main_t *{{ prefix }}mp = &{{cookiecutter.project_alias}}_main;

    if ({{ prefix }}mp->enable_disable == 0)
    {
        {{cookiecutter.project_alias}}_enable_disable({{ prefix }}mp, 1);
    }

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
    {{cookiecutter.project_alias}}_main_t *{{ prefix }}mp = &{{cookiecutter.project_alias}}_main;

    {{cookiecutter.project_alias}}_enable_disable({{ prefix }}mp, 0);

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


static clib_error_t *{{cookiecutter.project_alias}}_loads_command_fn(vlib_main_t *vm, unformat_input_t *input, vlib_cli_command_t *cmd, uint8_t is_add)
{
    {{cookiecutter.project_alias}}_main_t *{{ prefix }}mp = &{{cookiecutter.project_alias}}_main;

    clib_error_t *error = NULL;
    unformat_input_t _line_input, *line_input = &_line_input;
    uint8_t *filestr = NULL;

    if (!unformat_user(input, unformat_line_input, line_input))
        return 0;

    while (unformat_check_input(line_input) != UNFORMAT_END_OF_INPUT)
    {
        if (!unformat(line_input, "%s", &filestr))
        {
            return clib_error_return(0, "unformat error.");
        }
        else
        {
            break;
        }
    }

    // TODO
    // Code

    vec_free(filestr);

    auto_enable_disable({{ prefix }}mp);

    return error;
}


/* *INDENT-OFF* */
VLIB_CLI_COMMAND({{cookiecutter.project_alias}}_loads_command, static) =
{
    .path = "{{cookiecutter.project_name}} loads",
    .short_help = "{{cookiecutter.project_name}} loads <file>",
    .function = {{cookiecutter.project_alias}}_loads_command_fn,
};
/* *INDENT-ON* */


static clib_error_t *{{cookiecutter.project_alias}}_delete_command_fn(vlib_main_t *vm, unformat_input_t *input, vlib_cli_command_t *cmd, uint8_t is_add)
{
    {{cookiecutter.project_alias}}_main_t *{{ prefix }}mp = &{{cookiecutter.project_alias}}_main;

    uint32_t rid = 0;

    while (unformat_check_input(input) != UNFORMAT_END_OF_INPUT)
    {
        if (unformat(input, "%d", &rid))
        {
            // xxx_delete({{ prefix }}mp->rules, (uint16_t)rid);
        }
        else if (unformat(input, "rid %d", &rid))
        {
            // xxx_delete({{ prefix }}mp->rules, (uint16_t)rid);
        }
        else
            break;
    }

    auto_enable_disable({{ prefix }}mp);

    return 0;
}


/* *INDENT-OFF* */
VLIB_CLI_COMMAND({{cookiecutter.project_alias}}_delete_command, static) =
{
    .path = "{{cookiecutter.project_name}} delete",
    .short_help = "{{cookiecutter.project_name}} delete",
    .function = {{cookiecutter.project_alias}}_delete_command_fn,
};
/* *INDENT-ON* */


static clib_error_t *{{cookiecutter.project_alias}}_add_del_command_fn(vlib_main_t *vm, unformat_input_t *input, vlib_cli_command_t *cmd, uint8_t is_add)
{
    {{cookiecutter.project_alias}}_main_t *{{ prefix }}mp = &{{cookiecutter.project_alias}}_main;

    u32 id = 0;

    while (unformat_check_input(input) != UNFORMAT_END_OF_INPUT)
    {
        if (unformat(input, "id %d", &id))
            ;
        else
            break;
    }

    vlib_cli_output(vm, "id:%d", id);

    /* TODO */

    auto_enable_disable({{ prefix }}mp);

    return 0;
}


static clib_error_t *{{cookiecutter.project_alias}}_add_command_fn(vlib_main_t *vm, unformat_input_t *input, vlib_cli_command_t *cmd)
{
    return {{cookiecutter.project_alias}}_add_del_command_fn(vm, input, cmd, 1 /* add */);
}


/* *INDENT-OFF* */
VLIB_CLI_COMMAND({{cookiecutter.project_alias}}_add_command, static) =
{
    .path = "{{cookiecutter.project_name}} add",
    .short_help = "{{cookiecutter.project_name}} add",
    .function = {{cookiecutter.project_alias}}_add_command_fn,
};
/* *INDENT-ON* */


static clib_error_t *{{cookiecutter.project_alias}}_del_command_fn(vlib_main_t *vm, unformat_input_t *input, vlib_cli_command_t *cmd)
{
    return {{cookiecutter.project_alias}}_add_del_command_fn(vm, input, cmd, 0 /* del */);
}


/* *INDENT-OFF* */
VLIB_CLI_COMMAND({{cookiecutter.project_alias}}_del_command, static) =
{
    .path = "{{cookiecutter.project_name}} del",
    .short_help = "{{cookiecutter.project_name}} del",
    .function = {{cookiecutter.project_alias}}_del_command_fn,
};
/* *INDENT-ON* */


static clib_error_t *{{cookiecutter.project_alias}}_show_command_fn(vlib_main_t *vm, unformat_input_t *input, vlib_cli_command_t *cmd)
{
    {{cookiecutter.project_alias}}_main_t *{{ prefix }}mp = &{{cookiecutter.project_alias}}_main;

    vlib_cli_output(vm, "show command");

    /* TODO */

    return 0;
}


/* *INDENT-OFF* */
VLIB_CLI_COMMAND({{cookiecutter.project_alias}}_show_command, static) =
{
    .path = "{{cookiecutter.project_name}} show",
    .short_help = "{{cookiecutter.project_name}} show",
    .function = {{cookiecutter.project_alias}}_show_command_fn,
};
/* *INDENT-ON* */


static clib_error_t *{{cookiecutter.project_alias}}_flush_command_fn(vlib_main_t *vm, unformat_input_t *input, vlib_cli_command_t *cmd)
{
    {{cookiecutter.project_alias}}_main_t *{{ prefix }}mp = &{{cookiecutter.project_alias}}_main;

    /* TODO */

    {{cookiecutter.project_alias}}_enable_disable({{ prefix }}mp, 0);

    return 0;
}


/* *INDENT-OFF* */
VLIB_CLI_COMMAND({{cookiecutter.project_alias}}_flush_command, static) =
{
    .path = "{{cookiecutter.project_name}} flush",
    .short_help = "{{cookiecutter.project_name}} flush",
    .function = {{cookiecutter.project_alias}}_flush_command_fn,
};
/* *INDENT-ON* */
