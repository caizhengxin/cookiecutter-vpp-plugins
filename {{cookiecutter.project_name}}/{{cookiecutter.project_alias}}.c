/**
 * @Author: jankincai
 * @Date:   2021-09-22 11:04:14
 * @Last Modified by:   jankincai
 * @Last Modified time: 2023-02-20 16:20:11
 */
/*
 * {{cookiecutter.project_alias}}.c - skeleton vpp engine plug-in
 *
 * Copyright (c) <current-year> <your-organization>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
{% set prefix = cookiecutter.project_alias[0] %}#include <vnet/vnet.h>
#include <vnet/plugin/plugin.h>
#include <{{cookiecutter.project_name}}/{{cookiecutter.project_alias}}.h>

#include <vlibapi/api.h>
#include <vlibmemory/api.h>
#include <vpp/app/version.h>
#include <stdbool.h>

#include <{{cookiecutter.project_name}}/{{cookiecutter.project_alias}}.api_enum.h>
#include <{{cookiecutter.project_name}}/{{cookiecutter.project_alias}}.api_types.h>

#define REPLY_MSG_ID_BASE {{prefix}}mp->msg_id_base
#include <vlibapi/api_helper_macros.h>

{{cookiecutter.project_alias}}_main_t {{cookiecutter.project_alias}}_main;


/* API message handler */
static void vl_api_{{cookiecutter.project_alias}}_enable_disable_t_handler(vl_api_{{cookiecutter.project_alias}}_enable_disable_t * mp)
{
    vl_api_{{cookiecutter.project_alias}}_enable_disable_reply_t * rmp;
    {{cookiecutter.project_alias}}_main_t * {{prefix}}mp = &{{cookiecutter.project_alias}}_main;
    int rv;

    rv = {{cookiecutter.project_alias}}_enable_disable({{prefix}}mp, (int)(mp->enable_disable));

    REPLY_MACRO(VL_API_{{cookiecutter.project_alias.upper()}}_ENABLE_DISABLE_REPLY);
}


/* API definitions */
#include <{{cookiecutter.project_name}}/{{cookiecutter.project_alias}}.api.c>


static clib_error_t *{{cookiecutter.project_alias}}_init(vlib_main_t *vm)
{
    {{cookiecutter.project_alias}}_main_t *{{prefix}}mp = &{{cookiecutter.project_alias}}_main;
    clib_error_t *error = 0;

    {{prefix}}mp->vlib_main = vm;
    {{prefix}}mp->vnet_main = vnet_get_main();

    /* Add our API messages to the global name_crc hash table */
    {{prefix}}mp->msg_id_base = setup_message_id_table();

    {{prefix}}mp->enable_disable = false;

    return error;
}

VLIB_INIT_FUNCTION({{cookiecutter.project_alias}}_init);


/* *INDENT-OFF* */
VNET_FEATURE_INIT({{cookiecutter.project_alias}}_input, static) =
{
    .arc_name = "device-input",
    .node_name = "{{cookiecutter.project_name}}-input",
    .runs_before = VNET_FEATURES("ethernet-input"),
};
/* *INDENT-ON */


/* *INDENT-OFF* */
VNET_FEATURE_INIT({{cookiecutter.project_alias}}_output, static) =
{
    .arc_name = "interface-output",
    .node_name = "{{cookiecutter.project_name}}-output",
    // .runs_before = VNET_FEATURES(""),
};
/* *INDENT-ON */


/* *INDENT-OFF* */
VLIB_PLUGIN_REGISTER() =
{
    .version = VPP_BUILD_VER,
    .description = "{{cookiecutter.project_name}} plugin description goes here",
};
/* *INDENT-ON* */

/*
 * fd.io coding-style-patch-verification: ON
 *
 * Local Variables:
 * eval: (c-set-style "gnu")
 * End:
 */
